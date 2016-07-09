#include <set>
#include <algorithm>

class Level1ContactListener : public b2ContactListener
{
	public:
		
		Level1ContactListener  ( );
	    ~Level1ContactListener ( );

		virtual void		BeginContact	( b2Contact* pContact );
		virtual void		EndContact		( b2Contact* pContact );
		virtual void		PreSolve		( b2Contact* pContact, const b2Manifold* pOldManifold );    
		virtual void		PostSolve		( b2Contact* pContact, const b2ContactImpulse* pImpulse );

		std::set<b2Body*>	m_pContacts;
};