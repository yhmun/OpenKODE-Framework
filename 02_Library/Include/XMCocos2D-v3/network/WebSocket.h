/* -----------------------------------------------------------------------------------
 *
 *      File            WebSocket.h
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

#ifndef __WebSocket_h__
#define __WebSocket_h__

#include <list>
#include <vector>
#include <string>
#include <kode.h>

struct libwebsocket;
struct libwebsocket_context;
struct libwebsocket_protocols;

namespace network {

class WsThreadHelper;
class WsMessage;

class WebSocket
{
	public :

		/**
		 *	@js ctor
		 */
		WebSocket ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~WebSocket ( KDvoid );
    
		/**
		 *  @brief Data structure for message
		 */
		struct Data
		{
			Data ( KDvoid ) : bytes ( KD_NULL ), len ( 0 ), isBinary ( false ) { }

			KDchar*		bytes;
			KDint		len;
			KDbool		isBinary;
		};
    
		/**
		 *  @brief Errors in websocket
		 */
		enum class ErrorCode
		{
			TIME_OUT,
			CONNECTION_FAILURE,
			UNKNOWN,
		};
    
		/**
		 *  Websocket state
		 */
		enum class State
		{
			CONNECTING,
			OPEN,
			CLOSING,
			CLOSED,
		};

		/**
		 *  @brief The delegate class to process websocket events.
		 */
		class Delegate
		{
			public :

			virtual ~Delegate ( KDvoid ) { }

			virtual KDvoid				onOpen		( WebSocket* ws ) = 0;
			virtual KDvoid				onMessage	( WebSocket* ws, const Data& data ) = 0;
			virtual KDvoid				onClose		( WebSocket* ws ) = 0;
			virtual KDvoid				onError		( WebSocket* ws, const ErrorCode& error ) = 0;
		};
    
    
		/**
		 *  @brief  The initialized method for websocket.
		 *          It needs to be invoked right after websocket instance is allocated.
		 *  @param  delegate The delegate which want to receive event from websocket.
		 *  @param  url      The URL of websocket server.
		 *  @return true: Success, false: Failure
		 */
		KDbool							init ( const Delegate& tDelegate, const std::string& sUrl, const std::vector<std::string>* pProtocols = KD_NULL );
    
		/**
		 *  @brief Sends string data to websocket server.
		 */
		KDvoid							send ( const std::string& sMessage );
    
		/**
		 *  @brief Sends binary data to websocket server.
		 */
		KDvoid							send ( const KDubyte* pBinaryMsg, KDuint uLen );
    
		/**
		 *  @brief Closes the connection to server.
		 */
		KDvoid							close ( KDvoid );
    
		/**
		 *  @brief Gets current state of connection.
		 */
		State							getReadyState ( KDvoid );
    
	private:
		virtual KDvoid					onSubThreadStarted	( KDvoid );
		virtual KDint					onSubThreadLoop		( KDvoid );
		virtual KDvoid					onSubThreadEnded	( KDvoid );
		virtual KDvoid					onUIThreadReceiveMessage ( WsMessage* pMsg );
    

		friend class WebSocketCallbackWrapper;

		KDint							onSocketCallback ( struct libwebsocket_context* pCtx, struct libwebsocket* pWsi, KDint nReason, KDvoid* pUser, KDvoid* pIn, size_t uLen );
    
	private :

		State							m_eReadyState;
		std::string						m_sHost;
		KDuint							m_uPort;
		std::string						m_sPath;
    
		friend class WsThreadHelper;
		WsThreadHelper*					m_pWsHelper;
    
		struct libwebsocket*			m_pWsInstance;
		struct libwebsocket_context*	m_pWsContext;
		Delegate*						m_pDelegate;
		KDint							m_nSSLConnection;
		struct libwebsocket_protocols*	m_pWsProtocols;
};

}

#endif	// __WebSocket_h__
