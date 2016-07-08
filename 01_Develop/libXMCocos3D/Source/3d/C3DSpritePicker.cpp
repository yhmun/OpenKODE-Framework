#include "3d/C3DSpritePicker.h"

#include "3d/C3DSprite.h"

#include "3d/C3DTransform.h"
#include "3d/C3DMatrix.h"
#include "3d/C3DScene.h"
#include "3d/C3DCamera.h"

#include "3d/BoundingBox.h"
#include "3d/C3DRay.h"
#include "3d/C3DRenderSystem.h"

namespace cocos3d
{

C3DSpritePicker::C3DSpritePicker(C3DScene* scene) : _scene(scene) 
{
	
}

C3DSpritePicker::~C3DSpritePicker()
{
	
}

C3DSprite* C3DSpritePicker::pick( float x,float y )
{
	/*~
	std::map<std::string,C3DSprite*>* entities = _scene->getSpriteNodes();
			

    C3DRay* ray = new C3DRay();
	_scene->getActiveCamera()->getAxes(C3DRenderSystem::getInstance()->getViewport(),x,y,ray);

	for( std::map<std::string,C3DSprite*>::const_iterator iter = entities->begin(); iter != entities->end(); ++iter )
	{
		C3DSprite * pCurr = iter->second;		

	//	BoundingBox* bb = pCurr->getBoundingBox();
		C3DOBB* obb = pCurr->getOBB();		

	//	bool res = ray->intersects(bb);
		bool res = ray->intersects(obb);
		if(res == true)
		{
            SAFE_DELETE(ray);
			return pCurr;
		}
		
	}
    SAFE_DELETE(ray);
	*/
	return NULL;

	
}

void C3DSpritePicker::update(long elapsedTime)
{
	
}



}