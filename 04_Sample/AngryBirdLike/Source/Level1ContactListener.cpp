#include "Precompiled.h"
#include "Level1ContactListener.h"

Level1ContactListener::Level1ContactListener ( )
{

}

Level1ContactListener::~Level1ContactListener ( )
{

}

KDvoid Level1ContactListener::BeginContact ( b2Contact* pContact )
{

}

KDvoid Level1ContactListener::EndContact ( b2Contact* pContact )
{

}

KDvoid Level1ContactListener::PreSolve ( b2Contact* pContact, const b2Manifold* pOldManifold )
{

}

KDvoid Level1ContactListener::PostSolve ( b2Contact* pContact, const b2ContactImpulse* pImpulse )
{
		
    if ( pContact->GetFixtureA ( )->GetUserData ( ) != KD_NULL || pContact->GetFixtureB ( )->GetUserData ( ) !=KD_NULL )
    {
        KDint nCount = pContact->GetManifold ( )->pointCount;

        b2Fixture* pFdef = pContact->GetFixtureA ( );
	    b2Filter bfilterA = pFdef->GetFilterData ( );
		pFdef = pContact->GetFixtureB ( );
	    b2Filter bfilterB= pFdef->GetFilterData ( );

        KDfloat fMaxImpulse = 0.0f;

        for ( KDint i = 0; i < nCount; ++i )
        {
            fMaxImpulse = b2Max ( fMaxImpulse, pImpulse->normalImpulses [ i ] );
        }
        KDfloat fBreakThreshold = 1.0f;

		if ( bfilterA.categoryBits==0x0004 ||bfilterB.categoryBits==0x0004 )
		{
			fBreakThreshold = 3.5f;
		}
		if ( bfilterA.categoryBits==0x0008 ||bfilterB.categoryBits==0x0008 )
		{			
			fBreakThreshold = 2.0f;
		}

        if ( fMaxImpulse > fBreakThreshold )
        {
			if (pContact->GetFixtureA()->GetUserData() !=KD_NULL )
			{
                m_pContacts.insert ( pContact->GetFixtureA ( )->GetBody ( ) );
			}
			if ( pContact->GetFixtureB ( )->GetUserData ( ) != KD_NULL )
			{
				m_pContacts.insert ( pContact->GetFixtureB ( )->GetBody ( ) );
			}
        }
    }
}