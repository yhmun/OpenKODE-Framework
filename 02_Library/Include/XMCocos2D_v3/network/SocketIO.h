/* -----------------------------------------------------------------------------------
 *
 *      File            SocketIO.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2013      Chris Hannon.
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

/****************************************************************************

*based on the SocketIO library created by LearnBoost at http://socket.io
*using spec version 1 found at https://github.com/LearnBoost/socket.io-spec

Usage is described below, a full working example can be found in TestCpp under ExtionsTest/NetworkTest/SocketIOTest

creating a new connection to a socket.io server running at localhost:3000

	SIOClient *client = SocketIO::connect(*delegate, "ws://localhost:3000");

the connection process will begin and if successful delegate::onOpen will be called
if the connection process results in an error, delegate::onError will be called with the err msg

sending a message to the server

	client->send("Hello!");

emitting an event to be handled by the server, argument json formatting is up to you

	client->emit("eventname", "[{\"arg\":\"value\"}]");

registering an event callback, target should be a member function in a subclass of SIODelegate
CC_CALLBACK_2 is used to wrap the callback with std::bind and store as an SIOEvent

	client->on("eventname", CC_CALLBACK_2(TargetClass::targetfunc, *targetclass_instance));

event target function should match this pattern, *this pointer will be made available

	void TargetClass::targetfunc(SIOClient *, const std::string&)

disconnect from the endpoint by calling disconnect(), onClose will be called on the delegate once complete
in the onClose method the pointer should be set to NULL or used to connect to a new endpoint

	client->disconnect();

 ****************************************************************************/

#ifndef __SocketIO_h__
#define __SocketIO_h__

#include "../base/CCDictionary.h"
#include <map>

namespace network {

//forward declarations
class SIOClientImpl;
class SIOClient;

/**
 *  @brief Singleton and wrapper class to provide static creation method as well as registry of all sockets
 */
class SocketIO
{
	public :

				 SocketIO ( KDvoid );
		virtual ~SocketIO ( KDvoid );

		static SocketIO*			instance ( KDvoid );

		/**
		 *  @brief The delegate class to process socket.io events
		 */
		class SIODelegate
		{
			public :

				virtual ~SIODelegate ( KDvoid ) { }

				virtual KDvoid		onConnect	( SIOClient* pClient ) = 0;
				virtual KDvoid		onMessage	( SIOClient* pClient, const std::string& sData ) = 0;
				virtual KDvoid		onClose		( SIOClient* pClient ) = 0;
				virtual KDvoid		onError		( SIOClient* pClient, const std::string& sData ) = 0;
		};

		/**
		 *  @brief  Static client creation method, similar to socketio.connect(uri) in JS
		 *  @param  delegate The delegate which want to receive events from the socket.io client
		 *  @param  uri      The URI of the socket.io server
		 *  @return An initialized SIOClient if connected successfully, otherwise NULL
		 */
		static SIOClient*			connect ( SocketIO::SIODelegate& tDelegate, const std::string& sUri );
	
	private :

		static SocketIO*			m_pInst;

		cocos2d::Dictionary*		m_pSockets;

		SIOClientImpl*				getSocket ( const std::string& sUri );

		KDvoid						addSocket ( const std::string& sUri, SIOClientImpl* pSocket );
		KDvoid						removeSocket ( const std::string& sUri );

		friend class SIOClientImpl;
	
};

// c++11 style callbacks entities will be created using CC_CALLBACK (which uses std::bind)
typedef std::function<KDvoid(SIOClient*, const std::string&)> SIOEvent;

// c++11 map to callbacks
typedef std::map<std::string, SIOEvent> EventRegistry;

/**
 *  @brief A single connection to a socket.io endpoint
 */
class SIOClient : public cocos2d::Object
{
	private :

		KDint						m_nPort;
		std::string					m_sHost, m_sPath, m_sTag;
		KDbool						m_bConnected;
		SIOClientImpl*				m_pSocket;

		SocketIO::SIODelegate*		m_pDelegate;

		EventRegistry				m_pEventRegistry;

		KDvoid						fireEvent ( const std::string& sEventName, const std::string& sData );

		KDvoid						onOpen ( KDvoid );
		KDvoid						onConnect ( KDvoid );
		KDvoid						receivedDisconnect ( KDvoid );

		friend class SIOClientImpl;

	public :

				 SIOClient ( const std::string& sHost, KDint nPort, const std::string& sPath, SIOClientImpl* pImpl, SocketIO::SIODelegate& pDelegate );
		virtual ~SIOClient ( KDvoid );

		/**
		 *  @brief Returns the delegate for the client
		 */
		SocketIO::SIODelegate*		getDelegate ( KDvoid ) { return m_pDelegate; }

		/**
		 *  @brief Disconnect from the endpoint, onClose will be called on the delegate when comlpete
		 */
		KDvoid						disconnect ( KDvoid );

		/**
		 *  @brief Send a message to the socket.io server
		 */
		KDvoid						send ( std::string s );

		/**
		 *  @brief The delegate class to process socket.io events
		 */
		KDvoid						emit ( std::string sEventname, std::string sArgs );

		/**
		 *  @brief Used to resgister a socket.io event callback
		 *		   Event argument should be passed using CC_CALLBACK2(&Base::function, this)
		 */
		KDvoid						on ( const std::string& sEventName, SIOEvent e );
	
		inline KDvoid				setTag ( const KDchar* pTag )
		{
			m_sTag = pTag;
		};
    
		inline const KDchar*		getTag ( KDvoid )
		{
			return m_sTag.c_str ( );
		};
};

}

#endif	// __SocketIO_h__
