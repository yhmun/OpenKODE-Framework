/* --------------------------------------------------------------------------
 *
 *      File            APSMacroCPP.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __APSMacroCPP_h__
#define __APSMacroCPP_h__

#define NS_APS_BEGIN					namespace cocos2d {
#define NS_APS_END						}

#define APS_SAFE_DELETE( _p_ )			if ( _p_ ) { delete   _p_; _p_ = 0; }
#define APS_SAFE_DELETE_ARRAY( _p_ )    if ( _p_ ) { delete[] _p_; _p_ = 0; }
#define APS_SAFE_FREE( _p_ )			if ( _p_ ) { kdFree ( _p_ ); _p_ = 0; }
#define APS_SAFE_RELEASE( _p_ )			if ( _p_ ) { ( _p_ )->release ( ); _p_ = 0; }
#define APS_SAFE_RETAIN( _p_ )			if ( _p_ ) { ( _p_ )->retain  ( ); }
#define APS_BREAK_IF( _cond_ )			if ( _cond_ ) { break; }


/**
 *	Example :
 *	APS_PROPERTY_KEY ( Position ) 
 *
 *	is the same as
 *
 *	public : static const std::string&  getPositionKey ( KDvoid );
 */
#define APS_PROPERTY_KEY( __FUNC_NAME__ )	public : static const std::string&  get##__FUNC_NAME__##Key ( KDvoid );


/**
 *	Example :
 *	APS_SYNTHESIZE_READONLY ( CCPoint, m_tPosition, Position )
 *
 *	is the same as
 *
 *	protected : CCPoint  m_tPosition;
 *	public : inline CCPoint  getPosition ( KDvoid ) const { return m_tPosition; }
 */
#define APS_SYNTHESIZE_READONLY( _varType_, _varName_, _funName_ )									\
protected : _varType_  _varName_;																	\
public : inline _varType_  get##_funName_ ( KDvoid ) const { return _varName_; }


/**
 *	Example :
 *	APS_SYNTHESIZE_READONLY_WITH_KEY ( CCPoint, m_tPosition, Position ) 
 *
 *	is the same as
 *	
 *	protected : CCPoint m_tPosition;
 *	public : inline CCPoint getPosition ( KDvoid ) const { return m_tPosition; }
 *	public : static const std::string& getPositionKey ( KDvoid );
 */
#define APS_SYNTHESIZE_READONLY_WITH_KEY( _varType_, _varName_, _funName_ )							\
APS_SYNTHESIZE_READONLY ( _varType_, _varName_, _funName_ )											\
APS_PROPERTY_KEY ( _funName_ )


/**
 *	Example :
 *	APS_SYNTHESIZE ( CCPoint, m_tPosition, Position )
 *
 *	is the same as
 *
 *	protected : CCPoint m_tPosition;
 *	public : inline CCPoint getPosition ( KDvoid ) const { return m_tPosition; }
 *	public : inline KDvoid  setPosition ( CCPoint var ) { m_tPosition = var; }
 */
#define APS_SYNTHESIZE( _varType_, _varName_, _funName_ )											\
protected : _varType_ _varName_;																	\
public : inline _varType_  get##_funName_ ( KDvoid ) const { return _varName_; }					\
public : inline KDvoid     set##_funName_ ( _varType_ var ) { _varName_ = var; }

/**
 *	Example :
 *	APS_SYNTHESIZE_REF ( CCPoint, m_tPosition, Position )
 *
 *	is the same as
 *
 *	protected : CCPoint m_tPosition;
 *	public : inline CCPoint&  getPosition ( KDvoid ) const { return m_tPosition; }
 *	public : inline KDvoid    setPosition ( CCPoint& var ) { m_tPosition = var; }
 */
#define APS_SYNTHESIZE_REF( _varType_, _varName_, _funName_ )										\
protected : _varType_  _varName_;																	\
public : inline _varType_&  get##_funName_ ( KDvoid ) const { return _varName_; }					\
public : inline KDvoid      set##_funName_ ( _varType_& var ) { _varName_ = var; }


/**
 *	Example :
 *	APS_SYNTHESIZE_WITH_KEY ( CCPoint, m_tPosition, Position ) 
 *
 *	is the same as
 *
 *	protected : CCPoint		 m_tPosition;
 *	public : inline CCPoint  getPosition ( KDvoid ) const { return m_tPosition; }
 *	public : inline KDvoid   setPosition ( CCPoint var ) { m_tPosition = var; }
 *	public : static const std::string&  getPositionKey ( KDvoid );
 */
#define APS_SYNTHESIZE_WITH_KEY( _varType_, _varName_, _funName_ )									\
APS_SYNTHESIZE ( _varType_, _varName_, _funName_ )													\
APS_PROPERTY_KEY ( _funName_ )


/**
 *	Example :
 *	APS_PROPERTY ( CCPoint, m_tPosition, Position ) 
 *
 *	is the same as
 *
 *	protected : CCPoint		  m_tPosition;
 *	public : virtual CCPoint  getPosition ( KDvoid ) const;
 *	public : virtual KDvoid   setPosition ( CCPoint var );
 */
#define APS_PROPERTY( _varType_, _varName_, _funName_ )												\
protected : _varType_	_varName_;																	\
public : virtual _varType_	get##_funName_ ( KDvoid ) const;										\
public : virtual KDvoid		set##_funName_ ( _varType_ var );


/**
 *	Example :
 *	APS_PROPERTY_WITH_KEY ( CCPoint, m_tPosition, Position ) 
 *
 *	is the same as
 *	
 *	protected : CCPoint		  m_tPosition;
 *	public : virtual CCPoint  getPosition ( KDvoid ) const;
 *	public : virtual KDvoid   setPosition ( CCPoint var );
 *	public : static const std::string&  getPositionKey ( KDvoid );
 */
#define APS_PROPERTY_WITH_KEY( _varType_, _varName_, _funName_ )									\
APS_PROPERTY ( _varType_, _varName_, _funName_ )													\
APS_PROPERTY_KEY ( _funName_ )


/**
 *	Example :
 *	APS_SYNTHESIZE_STRING ( m_sTag, Tag ) 
 *
 *	is the same as
 *
 *	protected : std::string		m_sTag;
 *	public : const KDchar*	getTag ( KDvoid ) const { return this->m_sTag.c_str ( ); }
 *	public : KDvoid			setTag ( const KDchar* szVar ) { this->m_sTag = szVar; }
 *	public : const std::string&  getTag_string ( KDvoid ) const { return this->m_sTag; }
 */
#define APS_SYNTHESIZE_STRING( _varName_, _funName_ )												\
protected : std::string		_varName_ ;\
public : const KDchar*	get##_funName_ ( KDvoid ) const { return this->_varName_.c_str ( );	}		\
public : KDvoid			set##_funName_ ( const KDchar* szVar ) { this->_varName_ = szVar; }			\
public : const std::string&  get##_funName_##_string ( KDvoid ) const { return this->_varName_; }


/**
 *	Example :
 *	APS_SYNTHESIZE_STRING_WITH_KEY ( m_sTag, Tag ) 
 *
 *	is the same as
 *
 *	protected : std::string		m_sTag;
 *	public : const KDchar*	getTag ( KDvoid ) const { return this->m_sTag.c_str ( ); }
 *	public : KDvoid			setTag ( const KDchar* szVar ) { this->m_sTag = szVar; }
 *	public : const std::string&  getTag_string ( KDvoid ) const { return this->m_sTag; }
 *	public : static const std::string&  getTagKey ( KDvoid );
 */
#define APS_SYNTHESIZE_STRING_WITH_KEY( _varName_, _funName_ )										\
APS_SYNTHESIZE_STRING ( _varName_, _funName_ )														\
APS_PROPERTY_KEY ( _funName_ )


/**
 *	Example :
 *	APS_FOREACH ( std::vector<APSIndex>, indexes, iter )
 *
 *	is the same as
 *	
 *	for ( std::vector<APSIndex>::iterator iter = ( indexes ).begin ( ), iter_end = ( indexes ).end ( ); iter != iter_end; iter++ )
 */
#define APS_FOREACH( _varType_, _varName_, _iterName_ )												\
for ( _varType_::iterator _iterName_ = ( _varName_ ).begin ( ), iter_end = ( _varName_ ).end ( ); _iterName_ != iter_end; _iterName_++ )


/**
 *	Example :
 *	APS_FOREACH_CONST ( std::vector<APSIndex>, indexes, iter )
 *
 *	is the same as
 *
 *	for ( vector<APSIndex>::const_iterator iter = ( indexes ).begin ( ), iter_end = ( indexes ).end ( ); iter != iter_end; iter++ )
 */
#define APS_FOREACH_CONST( _varType_, _varName_, _iterName_ ) \
for ( _varType_::const_iterator _iterName_ = ( _varName_ ).begin ( ), iter_end = ( _varName_ ).end ( ); _iterName_ != iter_end; _iterName_++ )


//
// Allocating class by class name.
//


/**
 *	Example :
 *	APS_ABSTRACT_CLASS_NAME ( APSGraphic, Graphic )
 *
 *	is the same as
 *
 *	public : static  const std::string& className ( KDvoid ) { static const std::string cn = "Graphic"; return cn; }
 *	public : virtual const std::string& getClassName ( KDvoid ) const { return APSGraphic::className ( ); }
 */
#define APS_ABSTRACT_CLASS_NAME_KEY( __className__, __key__ )															\
public : static  const std::string&     className ( KDvoid ) { static const std::string  cn = #__key__; return cn; }	\
public : virtual const std::string&  getClassName ( KDvoid ) const { return __className__::className ( ); }

/**
 *	Example :
 *	APS_ABSTRACT_CLASS_NAME ( APSGraphic )
 *
 *	is the same as
 *	
 *	public : static  const std::string&    className ( KDvoid ) { static const std::string cn = "APSGraphic"; return cn; }
 *	public : virtual const std::string& getClassName ( KDvoid ) const { return APSGraphic::className ( ); }
 */
#define APS_ABSTRACT_CLASS_NAME( __className__ )																			\
public : static  const std::string&     className ( KDvoid ) { static const std::string cn = #__className__; return cn; }	\
public : virtual const std::string&  getClassName ( KDvoid ) const { return __className__::className ( ); }


/**
 *	Example :
 *	APS_CLASS_NAME ( APSGraphic, Graphic )
 *
 *	is the same as
 *
 *	public : static  const std::string&     className ( KDvoid ) { static const std::string cn = "Graphic"; return cn; }
 *	public : virtual const std::string&  getClassName ( KDvoid ) const { return APSGraphic::className ( ); }
 *	public : static  APSGraphic*  alloc ( KDvoid ) { return new APSGraphic; }
 */
#define APS_CLASS_NAME_KEY( __className__, __key__ )								\
APS_ABSTRACT_CLASS_NAME_KEY ( __className__, __key__ )								\
public : static __className__*  alloc ( KDvoid ) { return new __className__; }



/**
 *	Example :
 *	APS_CLASS_NAME ( APSGraphic )
 *
 *	is the same as
 *
 *	public : static  const std::string&     className ( KDvoid ) { static const std::string cn = "APSGraphic"; return cn; }
 *	public : virtual const std::string&  getClassName ( KDvoid ) const { return APSGraphic::className ( ); }
 *	public : static  APSGraphic*  alloc ( KDvoid ) { return new APSGraphic; }
 */
#define APS_CLASS_NAME( __className__ )												\
APS_ABSTRACT_CLASS_NAME ( __className__ )											\
public : static __className__*  alloc ( KDvoid ) { return new __className__; }


#endif	// __APSMacroCPP_h__









