/* -----------------------------------------------------------------------------------
 *
 *      File            WebSocket.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2013      James Chen.
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#include "network/WebSocket.h"

#include <thread>
#include <mutex>
#include <queue>
#include <signal.h>
#include <errno.h>

#include "XMWebsockets/libwebsockets.h"
#include "2d/CCDirector.h"
#include "2d/CCScheduler.h"

using namespace cocos2d;

namespace network {

class WsMessage
{
public:
    WsMessage() : what(0), obj(NULL){}
    unsigned int what; // message type
    void* obj;
};

/**
 *  @brief Websocket thread helper, it's used for sending message between UI thread and websocket thread.
 */
class WsThreadHelper : public cocos2d::Object
{
public:
    WsThreadHelper();
    ~WsThreadHelper();
        
    // Creates a new thread
    bool createThread(const WebSocket& ws);
    // Quits sub-thread (websocket thread).
    void quitSubThread();
    
    // Schedule callback function
    virtual void update(float dt);
    
    // Sends message to UI thread. It's needed to be invoked in sub-thread.
    void sendMessageToUIThread(WsMessage *msg);
    
    // Sends message to sub-thread(websocket thread). It's needs to be invoked in UI thread.
    void sendMessageToSubThread(WsMessage *msg);
    
    // Waits the sub-thread (websocket thread) to exit,
    void joinSubThread();
    
    
protected:
    void wsThreadEntryFunc();
    
private:
    std::list<WsMessage*>* _UIWsMessageQueue;
    std::list<WsMessage*>* _subThreadWsMessageQueue;
    std::mutex   _UIWsMessageQueueMutex;
    std::mutex   _subThreadWsMessageQueueMutex;
    std::thread* _subThreadInstance;
    WebSocket* _ws;
    bool _needQuit;
    friend class WebSocket;
};

// Wrapper for converting websocket callback from static function to member function of WebSocket class.
class WebSocketCallbackWrapper {
public:
    
    static int onSocketCallback(struct libwebsocket_context *ctx,
                                struct libwebsocket *wsi,
                                enum libwebsocket_callback_reasons reason,
                                void *user, void *in, size_t len)
    {
        // Gets the user data from context. We know that it's a 'WebSocket' instance.
        WebSocket* wsInstance = (WebSocket*)libwebsocket_context_user(ctx);
        if (wsInstance)
        {
            return wsInstance->onSocketCallback(ctx, wsi, reason, user, in, len);
        }
        return 0;
    }
};

// Implementation of WsThreadHelper
WsThreadHelper::WsThreadHelper()
: _subThreadInstance(nullptr)
, _ws(NULL)
, _needQuit(false)
{
    _UIWsMessageQueue = new std::list<WsMessage*>();
    _subThreadWsMessageQueue = new std::list<WsMessage*>();
    
    Director::getInstance()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

WsThreadHelper::~WsThreadHelper()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    joinSubThread();
    CC_SAFE_DELETE(_subThreadInstance);
    delete _UIWsMessageQueue;
    delete _subThreadWsMessageQueue;
}

bool WsThreadHelper::createThread(const WebSocket& ws)
{
    _ws = const_cast<WebSocket*>(&ws);
    
    // Creates websocket thread
    _subThreadInstance = new std::thread(&WsThreadHelper::wsThreadEntryFunc, this);
    return true;
}

void WsThreadHelper::quitSubThread()
{
    _needQuit = true;
}

void WsThreadHelper::wsThreadEntryFunc()
{
    _ws->onSubThreadStarted();
    
    while (!_needQuit)
    {
        if (_ws->onSubThreadLoop())
        {
            break;
        }
    }
    
    _ws->onSubThreadEnded();
}

void WsThreadHelper::sendMessageToUIThread(WsMessage *msg)
{
    std::lock_guard<std::mutex> lk(_UIWsMessageQueueMutex);
    _UIWsMessageQueue->push_back(msg);
}

void WsThreadHelper::sendMessageToSubThread(WsMessage *msg)
{
    std::lock_guard<std::mutex> lk(_subThreadWsMessageQueueMutex);
    _subThreadWsMessageQueue->push_back(msg);
}

void WsThreadHelper::joinSubThread()
{
    if (_subThreadInstance->joinable())
    {
        _subThreadInstance->join();
    }
}

void WsThreadHelper::update(float dt)
{
    WsMessage *msg = NULL;

    // Returns quickly if no message
    std::lock_guard<std::mutex> lk(_UIWsMessageQueueMutex);

    if (0 == _UIWsMessageQueue->size())
    {
        return;
    }
    
    // Gets message
    msg = *(_UIWsMessageQueue->begin());
    _UIWsMessageQueue->pop_front();
    
    if (_ws)
    {
        _ws->onUIThreadReceiveMessage(msg);
    }
    
    CC_SAFE_DELETE(msg);
}

enum WS_MSG {
    WS_MSG_TO_SUBTRHEAD_SENDING_STRING = 0,
    WS_MSG_TO_SUBTRHEAD_SENDING_BINARY,
    WS_MSG_TO_UITHREAD_OPEN,
    WS_MSG_TO_UITHREAD_MESSAGE,
    WS_MSG_TO_UITHREAD_ERROR,
    WS_MSG_TO_UITHREAD_CLOSE
};

WebSocket::WebSocket()
: m_eReadyState(State::CONNECTING)
, m_uPort(80)
, m_pWsHelper(nullptr)
, m_pWsInstance(nullptr)
, m_pWsContext(nullptr)
, m_pDelegate(nullptr)
, m_nSSLConnection(0)
, m_pWsProtocols(nullptr)
{
}

WebSocket::~WebSocket()
{
    close();
    CC_SAFE_RELEASE(m_pWsHelper);
    
    for (int i = 0; m_pWsProtocols[i].callback != nullptr; ++i)
    {
        CC_SAFE_DELETE_ARRAY(m_pWsProtocols[i].name);
    }
	CC_SAFE_DELETE_ARRAY(m_pWsProtocols);
}

bool WebSocket::init(const Delegate& delegate,
                     const std::string& url,
                     const std::vector<std::string>* protocols/* = NULL*/)
{
    bool ret = false;
    bool useSSL = false;
    std::string host = url;
    int pos = 0;
    int port = 80;
    
    m_pDelegate = const_cast<Delegate*>(&delegate);
    
    //ws://
    pos = host.find("ws://");
    if (pos == 0) host.erase(0,5);
    
    pos = host.find("wss://");
    if (pos == 0)
    {
        host.erase(0,6);
        useSSL = true;
    }
    
    pos = host.find(":");
    if (pos >= 0) port = kdAtoi(host.substr(pos+1, host.size()).c_str());
    
    pos = host.find("/", 0);
    std::string path = "/";
    if (pos >= 0) path += host.substr(pos + 1, host.size());
    
    pos = host.find(":");
    if(pos >= 0){
        host.erase(pos, host.size());
    }else if((pos = host.find("/"))>=0) {
    	host.erase(pos, host.size());
    }
    
    m_sHost = host;
    m_uPort = port;
    m_sPath = path;
    m_nSSLConnection = useSSL ? 1 : 0;
    
    CCLOG("[WebSocket::init] _host: %s, _port: %d, _path: %s", m_sHost.c_str(), m_uPort, m_sPath.c_str());

    int protocolCount = 0;
    if (protocols && protocols->size() > 0)
    {
        protocolCount = protocols->size();
    }
    else
    {
        protocolCount = 1;
    }
    
	m_pWsProtocols = new libwebsocket_protocols[protocolCount+1];
	memset(m_pWsProtocols, 0, sizeof(libwebsocket_protocols)*(protocolCount+1));

    if (protocols && protocols->size() > 0)
    {
        int i = 0;
        for (std::vector<std::string>::const_iterator iter = protocols->begin(); iter != protocols->end(); ++iter, ++i)
        {
            char* name = new char[(*iter).length()+1];
            strcpy(name, (*iter).c_str());
            m_pWsProtocols[i].name = name;
            m_pWsProtocols[i].callback = WebSocketCallbackWrapper::onSocketCallback;
        }
    }
    else
    {
        char* name = new char[20];
        strcpy(name, "default-protocol");
        m_pWsProtocols[0].name = name;
        m_pWsProtocols[0].callback = WebSocketCallbackWrapper::onSocketCallback;
    }
    
    // WebSocket thread needs to be invoked at the end of this method.
    m_pWsHelper = new WsThreadHelper();
    ret = m_pWsHelper->createThread(*this);
    
    return ret;
}

void WebSocket::send(const std::string& message)
{
    if (m_eReadyState == State::OPEN)
    {
        // In main thread
        WsMessage* msg = new WsMessage();
        msg->what = WS_MSG_TO_SUBTRHEAD_SENDING_STRING;
        Data* data = new Data();
        data->bytes = new char[message.length()+1];
        strcpy(data->bytes, message.c_str());
        data->len = message.length();
        msg->obj = data;
        m_pWsHelper->sendMessageToSubThread(msg);
    }
}

void WebSocket::send(const unsigned char* binaryMsg, unsigned int len)
{
    CCASSERT(binaryMsg != nullptr && len > 0, "parameter invalid.");

    if (m_eReadyState == State::OPEN)
    {
        // In main thread
        WsMessage* msg = new WsMessage();
        msg->what = WS_MSG_TO_SUBTRHEAD_SENDING_BINARY;
        Data* data = new Data();
        data->bytes = new char[len];
        memcpy((void*)data->bytes, (void*)binaryMsg, len);
        data->len = len;
        msg->obj = data;
        m_pWsHelper->sendMessageToSubThread(msg);
    }
}

void WebSocket::close()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(m_pWsHelper);
    
    if (m_eReadyState == State::CLOSING || m_eReadyState == State::CLOSED)
    {
        return;
    }
    
    CCLOG("websocket (%p) connection closed by client", this);
    m_eReadyState = State::CLOSED;

    m_pWsHelper->joinSubThread();
    
    // onClose callback needs to be invoked at the end of this method
    // since websocket instance may be deleted in 'onClose'.
    m_pDelegate->onClose(this);
}

WebSocket::State WebSocket::getReadyState()
{
    return m_eReadyState;
}

int WebSocket::onSubThreadLoop()
{
    if (m_eReadyState == State::CLOSED || m_eReadyState == State::CLOSING)
    {
        libwebsocket_context_destroy(m_pWsContext);
        // return 1 to exit the loop.
        return 1;
    }
    
    if (m_pWsContext && m_eReadyState != State::CLOSED && m_eReadyState != State::CLOSING)
    {
        libwebsocket_service(m_pWsContext, 0);
    }
    
    // Sleep 50 ms
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // return 0 to continue the loop.
    return 0;
}

void WebSocket::onSubThreadStarted()
{
	struct lws_context_creation_info info;
	memset(&info, 0, sizeof info);
    
	/*
	 * create the websocket context.  This tracks open connections and
	 * knows how to route any traffic and which protocol version to use,
	 * and if each connection is client or server side.
	 *
	 * For this client-only demo, we tell it to not listen on any port.
	 */
    
	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = m_pWsProtocols;
#ifndef LWS_NO_EXTENSIONS
	info.extensions = libwebsocket_get_internal_extensions();
#endif
	info.gid = -1;
	info.uid = -1;
    info.user = (void*)this;
    
	m_pWsContext = libwebsocket_create_context(&info);
    
	if(nullptr != m_pWsContext)
    {
        m_eReadyState = State::CONNECTING;
        std::string name;
        for (int i = 0; m_pWsProtocols[i].callback != nullptr; ++i)
        {
            name += (m_pWsProtocols[i].name);
            
            if (m_pWsProtocols[i+1].callback != nullptr) name += ", ";
        }
        m_pWsInstance = libwebsocket_client_connect(m_pWsContext, m_sHost.c_str(), m_uPort, m_nSSLConnection,
                                             m_sPath.c_str(), m_sHost.c_str(), m_sHost.c_str(),
                                             name.c_str(), -1);
	}
}

void WebSocket::onSubThreadEnded()
{

}

int WebSocket::onSocketCallback(struct libwebsocket_context *ctx,
                     struct libwebsocket *wsi,
                     int reason,
                     void *user, void *in, size_t len)
{
	//CCLOG("socket callback for %d reason", reason);
    CCASSERT(m_pWsContext == nullptr || ctx == m_pWsContext, "Invalid context.");
    CCASSERT(m_pWsInstance == nullptr || wsi == nullptr || wsi == m_pWsInstance, "Invaild websocket instance.");

	switch (reason)
    {
        case LWS_CALLBACK_DEL_POLL_FD:
        case LWS_CALLBACK_PROTOCOL_DESTROY:
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            {
                WsMessage* msg = nullptr;
                if (reason == LWS_CALLBACK_CLIENT_CONNECTION_ERROR
                    || (reason == LWS_CALLBACK_PROTOCOL_DESTROY && m_eReadyState == State::CONNECTING)
                    || (reason == LWS_CALLBACK_DEL_POLL_FD && m_eReadyState == State::CONNECTING)
                    )
                {
                    msg = new WsMessage();
                    msg->what = WS_MSG_TO_UITHREAD_ERROR;
                    m_eReadyState = State::CLOSING;
                }
                else if (reason == LWS_CALLBACK_PROTOCOL_DESTROY && m_eReadyState == State::CLOSING)
                {
                    msg = new WsMessage();
                    msg->what = WS_MSG_TO_UITHREAD_CLOSE;
                }

                if (msg)
                {
                    m_pWsHelper->sendMessageToUIThread(msg);
                }
            }
            break;
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            {
                WsMessage* msg = new WsMessage();
                msg->what = WS_MSG_TO_UITHREAD_OPEN;
                m_eReadyState = State::OPEN;
                
                /*
                 * start the ball rolling,
                 * LWS_CALLBACK_CLIENT_WRITEABLE will come next service
                 */
                libwebsocket_callback_on_writable(ctx, wsi);
                m_pWsHelper->sendMessageToUIThread(msg);
            }
            break;
            
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                std::lock_guard<std::mutex> lk(m_pWsHelper->_subThreadWsMessageQueueMutex);
                                               
                std::list<WsMessage*>::iterator iter = m_pWsHelper->_subThreadWsMessageQueue->begin();
                
                int bytesWrite = 0;
                for (; iter != m_pWsHelper->_subThreadWsMessageQueue->end(); ++iter)
                {
                    WsMessage* subThreadMsg = *iter;
                    
                    if ( WS_MSG_TO_SUBTRHEAD_SENDING_STRING == subThreadMsg->what
                      || WS_MSG_TO_SUBTRHEAD_SENDING_BINARY == subThreadMsg->what)
                    {
                        Data* data = (Data*)subThreadMsg->obj;

                        unsigned char* buf = new unsigned char[LWS_SEND_BUFFER_PRE_PADDING
                                                               + data->len + LWS_SEND_BUFFER_POST_PADDING];
                        
                        memset(&buf[LWS_SEND_BUFFER_PRE_PADDING], 0, data->len);
                        memcpy((char*)&buf[LWS_SEND_BUFFER_PRE_PADDING], data->bytes, data->len);
                        
                        enum libwebsocket_write_protocol writeProtocol;
                        
                        if (WS_MSG_TO_SUBTRHEAD_SENDING_STRING == subThreadMsg->what)
                        {
                            writeProtocol = LWS_WRITE_TEXT;
                        }
                        else
                        {
                            writeProtocol = LWS_WRITE_BINARY;
                        }
                        
                        bytesWrite = libwebsocket_write(wsi,  &buf[LWS_SEND_BUFFER_PRE_PADDING], data->len, writeProtocol);
                        
                        if (bytesWrite < 0)
                        {
                            CCLOGERROR("%s", "libwebsocket_write error...");
                        }
                        if (bytesWrite < data->len)
                        {
                            CCLOGERROR("Partial write LWS_CALLBACK_CLIENT_WRITEABLE\n");
                        }
                        
                        CC_SAFE_DELETE_ARRAY(data->bytes);
                        CC_SAFE_DELETE(data);
                        CC_SAFE_DELETE_ARRAY(buf);
                    }
                    
                    CC_SAFE_DELETE(subThreadMsg);
                }

                m_pWsHelper->_subThreadWsMessageQueue->clear();
                
                
                /* get notified as soon as we can write again */
                
                libwebsocket_callback_on_writable(ctx, wsi);
            }
            break;
            
        case LWS_CALLBACK_CLOSED:
            {
                
                CCLOG("%s", "connection closing..");

                m_pWsHelper->quitSubThread();
                
                if (m_eReadyState != State::CLOSED)
                {
                    WsMessage* msg = new WsMessage();
                    m_eReadyState = State::CLOSED;
                    msg->what = WS_MSG_TO_UITHREAD_CLOSE;
                    m_pWsHelper->sendMessageToUIThread(msg);
                }
            }
            break;
            
        case LWS_CALLBACK_CLIENT_RECEIVE:
            {
                if (in && len > 0)
                {
                    WsMessage* msg = new WsMessage();
                    msg->what = WS_MSG_TO_UITHREAD_MESSAGE;
                    
                    char* bytes = NULL;
                    Data* data = new Data();
                    
                    if (lws_frame_is_binary(wsi))
                    {
                        
                        bytes = new char[len];
                        data->isBinary = true;
                    }
                    else
                    {
                        bytes = new char[len+1];
                        bytes[len] = '\0';
                        data->isBinary = false;
                    }

                    memcpy(bytes, in, len);
                    
                    data->bytes = bytes;
                    data->len = len;
                    msg->obj = (void*)data;
                    
                    m_pWsHelper->sendMessageToUIThread(msg);
                }
            }
            break;
        default:
            break;
        
	}
    
	return 0;
}

void WebSocket::onUIThreadReceiveMessage(WsMessage* msg)
{
    switch (msg->what) {
        case WS_MSG_TO_UITHREAD_OPEN:
            {
                m_pDelegate->onOpen(this);
            }
            break;
        case WS_MSG_TO_UITHREAD_MESSAGE:
            {
                Data* data = (Data*)msg->obj;
                m_pDelegate->onMessage(this, *data);
                CC_SAFE_DELETE_ARRAY(data->bytes);
                CC_SAFE_DELETE(data);
            }
            break;
        case WS_MSG_TO_UITHREAD_CLOSE:
            {
                m_pDelegate->onClose(this);
            }
            break;
        case WS_MSG_TO_UITHREAD_ERROR:
            {
                // FIXME: The exact error needs to be checked.
                WebSocket::ErrorCode err = ErrorCode::CONNECTION_FAILURE;
                m_pDelegate->onError(this, err);
            }
            break;
        default:
            break;
    }
}

}
