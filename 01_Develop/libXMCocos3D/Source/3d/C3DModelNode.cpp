#include "3d/Base.h"
#include "3d/C3DModelNode.h"
#include "3d/C3DModel.h"
#include "3d/C3DMesh.h"
#include "3d/MeshPart.h"
#include "3d/C3DMeshSkin.h"
#include "3d/C3DScene.h"
#include "3d/C3DTechnique.h"
#include "3d/C3DPass.h"
#include "3d/C3DNode.h"
#include "3d/C3DSkinModel.h"
#include "3d/C3DMaterial.h"

#include "3d/BonePart.h"
#include "3d/Morph.h"
#include "3d/C3DVertexDeclaration.h"

#include "3d/C3DLightFilter.h"
#include "3d/MaterialParameter.h"

#include "3d/C3DStat.h"
#include "3d/C3DAnimatedLight.h"
#include "3d/C3DShadowMap.h"
#include "3d/C3DMaterialManager.h"
#include "3d/C3DRenderChannel.h"
#include "3d/C3DRenderSystem.h"

namespace cocos3d
{

C3DModelNode::C3DModelNode(const char* id)
    : C3DNode(id), _model(NULL)
{
	
}

C3DModelNode::~C3DModelNode()
{
	if (_model)
		_model->setNode(NULL);
	SAFE_RELEASE(_model);
}

C3DModelNode* C3DModelNode::create(const char* id)
{
    return new C3DModelNode(id);
}



C3DNode::Type C3DModelNode::getType() const
{
	return C3DNode::NodeType_Model;
}

void C3DModelNode::setModel(C3DModel* model)
{
    if (_model != model)
    {
        if (_model)
        {
            _model->setNode(NULL);
            SAFE_RELEASE(_model);
        }

        _model = model;

        if (_model)
        {
            _model->retain();
            _model->setNode(this);
        }
    }

}

C3DModel* C3DModelNode::getModel()
{
    return _model;
}

void C3DModelNode::draw()
{
	if (_model)
	{
		C3DMaterial* m = _model->getMaterial();
		if (m)
		{
			C3DMaterial::TechniqueUsage techUsage = 
                getScene()->isInShadowPass() ? C3DMaterial::TECH_USAGE_SHADOWMAP : C3DMaterial::TECH_USAGE_SCREEN;

            C3DTechnique* technique = m->getTechnique(techUsage);
            if (!technique)
                return;

            
			C3DRenderChannel* channel = technique->getChannel();
            if (channel == NULL)
            {
                RenderChannelManager* mgr = C3DRenderSystem::getInstance()->getRenderChannelManager();
                channel = mgr->getRenderChannel("opacity");
            }
			if(channel != NULL)
			{
				C3DAABB worldSpaceBox(*(_model->getMesh()->getBoundingBox()));
				
				if(this->getType() == C3DNode::NodeType_SceneModel)
				{				
					worldSpaceBox.transform(getWorldMatrix());
				}
				else if(this->getType() == C3DNode::NodeType_SuperModel)
				{
					C3DMeshSkin* skin =  (static_cast<C3DSkinModel*>(_model))->getSkin();
					if (skin && skin->getRootJoint()->getParent())
					{
						worldSpaceBox.transform(getWorldMatrix() * skin->getRootJoint()->getParent()->getWorldMatrix());
					}
					else
					{
						worldSpaceBox.transform(getWorldMatrix());            
					}

				}
				
				worldSpaceBox.transform(getViewMatrix());

				C3DVector3 center = C3DVector3((worldSpaceBox.getCenter()).x,(worldSpaceBox.getCenter()).y,(worldSpaceBox.getCenter()).z);
				
				channel->addItem(center.z, _model);
			}
			else
			{
				_model->draw();
			}
		}
	}	

}

const C3DMatrix& C3DModelNode::getWorldMatrix()
{
    if (_dirtyBits & NODE_DIRTY_WORLD)
    {
        // Clear our dirty flag immediately to prevent this block from being entered if our
        // parent calls our getWorldMatrix() method as a result of the following calculations.
        _dirtyBits &= ~NODE_DIRTY_WORLD;

        // If we have a parent, multiply our parent world transform by our local
        // transform to obtain our final resolved world transform.
        C3DNode* parent = getParent();
		if (parent)
        {

			if(parent->getType()==C3DNode::NodeType_SuperModel)
			{
				_world = getMatrix();
				
			}
			else
			{
				C3DMatrix::multiply(parent->getWorldMatrix(), getMatrix(), &_world);
			}
            
        }
        else
        {
            _world = getMatrix();
        }

        // Our world matrix was just updated, so call getWorldMatrix() on all child nodes
        // to force their resolved world matrices to be updated.
		for(std::vector<C3DNode*>::const_iterator iter=_children.begin(); iter!=_children.end(); ++iter)       
        {
			(*iter)->getWorldMatrix();           
        }
    }

    return _world;
}

C3DNode* C3DModelNode::clone(CloneContext& context) const
{
	C3DModelNode* other = new C3DModelNode(_id.c_str());
	other->copyFrom(this, context);
	other->_id = _id;
	C3DModel* newModel = _model ? _model->clone(context) : NULL;
	other->setModel(newModel);
	other->autorelease();
	return other;
}

}
