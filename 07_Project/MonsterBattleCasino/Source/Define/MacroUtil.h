#ifndef macroUtil_H
#define macroUtil_H

#define forloop(p,vector)	\
	for( size_t p = 0, e = vector.size(); p < e ; p++ )

#define VERIFY(x)	if( ! x ) ASSERT(0)

#define RegisterOgreSingleton(x)		template<> x* Ogre::Singleton<x>::ms_Singleton = 0;
#define safeDelete(x)					{if(x) delete(x); (x)=NULL; }
#define safeReleaseObjc(x)				{	if(x)	[ (x) release ];	(x) = nil;	}

template<class T> const T LBOUND( T v, T l )
{
	return v < l ? l : v;
};

template<class T> const T RBOUND( T v, T r )
{
	return v > r ? r : v;
};

template<class T> const T BOUND( T v, T l, T r )
{
	return LBOUND<T>( RBOUND<T>( v, r ), l );
};

#endif

