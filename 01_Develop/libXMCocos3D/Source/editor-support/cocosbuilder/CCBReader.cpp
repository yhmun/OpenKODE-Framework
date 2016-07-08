/* -----------------------------------------------------------------------------------
 *
 *      File            CCBReader.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#include "editor-support/cocosbuilder/CCBReader.h"

#include <algorithm>

#include "editor-support/cocosbuilder/CCNodeLoader.h"
#include "editor-support/cocosbuilder/CCNodeLoaderLibrary.h"
#include "editor-support/cocosbuilder/CCNodeLoaderListener.h"
#include "editor-support/cocosbuilder/CCBMemberVariableAssigner.h"
#include "editor-support/cocosbuilder/CCBSelectorResolver.h"
#include "editor-support/cocosbuilder/CCBAnimationManager.h"
#include "editor-support/cocosbuilder/CCBSequenceProperty.h"
#include "editor-support/cocosbuilder/CCBKeyframe.h"
#include "editor-support/cocosbuilder/CCBValue.h"
#include "base/CCData.h"
#include "base/CCInteger.h"
#include "2d/CCDirector.h"
#include "2d/textures/CCTextureCache.h"
#include "2d/sprite_nodes/CCSpriteFrameCache.h"
#include "platform/CCFileUtils.h"

#include <ctype.h>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

namespace cocosbuilder {;

/*************************************************************************
 Implementation of CCBFile
 *************************************************************************/

CCBFile::CCBFile():m_pCCBFileNode(NULL) {}

CCBFile* CCBFile::create()
{
    CCBFile *ret = new CCBFile();
    
    if (ret)
    {
        ret->autorelease();
    }
    
    return ret;
}

Node* CCBFile::getCCBFileNode()
{
    return m_pCCBFileNode;
}

void CCBFile::setCCBFileNode(Node *pNode)
{
    CC_SAFE_RELEASE(m_pCCBFileNode);
    m_pCCBFileNode = pNode;
    CC_SAFE_RETAIN(m_pCCBFileNode);
}

/*************************************************************************
 Implementation of CCBReader
 *************************************************************************/

CCBReader::CCBReader(NodeLoaderLibrary * pNodeLoaderLibrary, CCBMemberVariableAssigner * pCCBMemberVariableAssigner, CCBSelectorResolver * pCCBSelectorResolver, NodeLoaderListener * pNodeLoaderListener) 
: m_pData(NULL)
, m_pBytes(NULL)
, m_nCurrentByte(-1)
, m_nCurrentBit(-1)
, m_pOwner(NULL)
, m_pActionManager(NULL)
, m_pActionManagers(NULL)
, m_pAnimatedProps(NULL)
, m_pOwnerOutletNodes(NULL)
, m_pNodesWithAnimationManagers(NULL)
, m_pAnimationManagersForNodes(NULL)
, m_pOwnerCallbackNodes(NULL)
{
    this->m_pNodeLoaderLibrary = pNodeLoaderLibrary;
    this->m_pNodeLoaderLibrary->retain();
    this->m_pCCBMemberVariableAssigner = pCCBMemberVariableAssigner;
    this->m_pCCBSelectorResolver = pCCBSelectorResolver;
    this->m_pNodeLoaderListener = pNodeLoaderListener;
    init();
}

CCBReader::CCBReader(CCBReader * ccbReader) 
: m_pData(NULL)
, m_pBytes(NULL)
, m_nCurrentByte(-1)
, m_nCurrentBit(-1)
, m_pOwner(NULL)
, m_pActionManager(NULL)
, m_pActionManagers(NULL)
, m_pAnimatedProps(NULL)
, m_pOwnerOutletNodes(NULL)
, m_pNodesWithAnimationManagers(NULL)
, m_pAnimationManagersForNodes(NULL)
, m_pOwnerCallbackNodes(NULL)
{
    this->m_aLoadedSpriteSheets = ccbReader->m_aLoadedSpriteSheets;
    this->m_pNodeLoaderLibrary = ccbReader->m_pNodeLoaderLibrary;
    this->m_pNodeLoaderLibrary->retain();

    this->m_pCCBMemberVariableAssigner = ccbReader->m_pCCBMemberVariableAssigner;
    this->m_pCCBSelectorResolver = ccbReader->m_pCCBSelectorResolver;
    this->m_pNodeLoaderListener = ccbReader->m_pNodeLoaderListener;
    
    this->m_sCCBRootPath = ccbReader->getCCBRootPath();
    
    init();
}

CCBReader::CCBReader()
: m_pData(NULL)
, m_pBytes(NULL)
, m_nCurrentByte(-1)
, m_nCurrentBit(-1)
, m_pOwner(NULL)
, m_pActionManager(NULL)
, m_pActionManagers(NULL)
, m_pNodeLoaderLibrary(NULL)
, m_pNodeLoaderListener(NULL)
, m_pCCBMemberVariableAssigner(NULL)
, m_pCCBSelectorResolver(NULL)
, m_pNodesWithAnimationManagers(NULL)
, m_pAnimationManagersForNodes(NULL)
{
    init();
}

CCBReader::~CCBReader()
{
    CC_SAFE_RELEASE(m_pOwner);
    CC_SAFE_RELEASE(m_pData);

    this->m_pNodeLoaderLibrary->release();

    CC_SAFE_RELEASE(m_pOwnerOutletNodes);
    m_aOwnerOutletNames.clear();
    CC_SAFE_RELEASE(m_pOwnerCallbackNodes);
    m_sOwnerCallbackNames.clear();
    CC_SAFE_RELEASE(m_pOwnerOwnerCallbackControlEvents);
    
    // Clear string cache.
    this->m_aStringCache.clear();
    CC_SAFE_RELEASE(m_pNodesWithAnimationManagers);
    CC_SAFE_RELEASE(m_pAnimationManagersForNodes);

    setAnimationManager(NULL);
}

void CCBReader::setCCBRootPath(const char* ccbRootPath)
{
    CCASSERT(ccbRootPath != NULL, "");
    m_sCCBRootPath = ccbRootPath;
}

const std::string& CCBReader::getCCBRootPath() const
{
    return m_sCCBRootPath;
}

bool CCBReader::init()
{
    m_pOwnerOutletNodes = new Array();
    m_pOwnerOutletNodes->init();
    m_pOwnerCallbackNodes = new Array();
    m_pOwnerCallbackNodes->init();
    m_pOwnerOwnerCallbackControlEvents = new Array();
    m_pOwnerOwnerCallbackControlEvents->init();

    // Setup action manager
    CCBAnimationManager *pActionManager = new CCBAnimationManager();
    setAnimationManager(pActionManager);
    pActionManager->release();
    
    // Setup resolution scale and container size
    m_pActionManager->setRootContainerSize(Director::getInstance()->getWinSize());
    
    return true;
}

CCBAnimationManager* CCBReader::getAnimationManager()
{
    return m_pActionManager;
}

void CCBReader::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
    CC_SAFE_RELEASE(m_pActionManager);
    m_pActionManager = pAnimationManager;
    CC_SAFE_RETAIN(m_pActionManager);
}

Dictionary* CCBReader::getAnimationManagers()
{
    return m_pActionManagers;
}

void CCBReader::setAnimationManagers(Dictionary* x)
{
    m_pActionManagers = x;
}

CCBMemberVariableAssigner * CCBReader::getCCBMemberVariableAssigner() {
    return this->m_pCCBMemberVariableAssigner;
}

CCBSelectorResolver * CCBReader::getCCBSelectorResolver() {
    return this->m_pCCBSelectorResolver;
}

set<string>* CCBReader::getAnimatedProperties()
{
    return m_pAnimatedProps;
}

set<string>& CCBReader::getLoadedSpriteSheet()
{
    return m_aLoadedSpriteSheets;
}

Object* CCBReader::getOwner()
{
    return m_pOwner;
}

Node* CCBReader::readNodeGraphFromFile(const char *pCCBFileName)
{
    return this->readNodeGraphFromFile(pCCBFileName, NULL);
}

Node* CCBReader::readNodeGraphFromFile(const char* pCCBFileName, Object* pOwner) 
{
    return this->readNodeGraphFromFile(pCCBFileName, pOwner, Director::getInstance()->getWinSize());
}

Node* CCBReader::readNodeGraphFromFile(const char *pCCBFileName, Object *pOwner, const Size &parentSize)
{
    if (NULL == pCCBFileName || strlen(pCCBFileName) == 0)
    {
        return NULL;
    }

    std::string strCCBFileName(pCCBFileName);
    std::string strSuffix(".ccbi");
    // Add ccbi suffix
    if (!CCBReader::endsWith(strCCBFileName.c_str(), strSuffix.c_str()))
    {
        strCCBFileName += strSuffix;
    }

    std::string strPath = FileUtils::getInstance()->fullPathForFilename(strCCBFileName.c_str());
    KDint32 size = 0;

    unsigned char * pBytes = FileUtils::getInstance()->getFileData(strPath.c_str(), "rb", &size);
    Data *data = new Data(pBytes, size);
    kdFree(pBytes);

    Node *ret =  this->readNodeGraphFromData(data, pOwner, parentSize);
    
    data->release();
    
    return ret;
}

Node* CCBReader::readNodeGraphFromData(Data *pData, Object *pOwner, const Size &parentSize)
{
   m_pData = pData;
    CC_SAFE_RETAIN(m_pData);
    m_pBytes =m_pData->getBytes();
    m_nCurrentByte = 0;
    m_nCurrentBit = 0;
    m_pOwner = pOwner;
    CC_SAFE_RETAIN(m_pOwner);

    m_pActionManager->setRootContainerSize(parentSize);
    m_pActionManager->m_pOwner = m_pOwner;
    
    Dictionary* animationManagers = Dictionary::create();
    Node *pNodeGraph = readFileWithCleanUp(true, animationManagers);
    
    if (pNodeGraph && m_pActionManager->getAutoPlaySequenceId() != -1)
    {
        // Auto play animations
        m_pActionManager->runAnimationsForSequenceIdTweenDuration(m_pActionManager->getAutoPlaySequenceId(), 0);
    }
    
    // Assign actionManagers to userObject
    if(m_bJsControlled)
    {
        m_pNodesWithAnimationManagers = new Array();
        m_pNodesWithAnimationManagers->init();
        m_pAnimationManagersForNodes = new Array();
        m_pAnimationManagersForNodes->init();
    }
    
    DictElement* pElement = NULL;
    CCDICT_FOREACH(animationManagers, pElement)
    {
        Node* pNode = (Node*)pElement->getIntKey();
        CCBAnimationManager* manager = static_cast<CCBAnimationManager*>(animationManagers->objectForKey((KDintptr)pNode));
        pNode->setUserObject(manager);

        if (m_bJsControlled)
        {
            m_pNodesWithAnimationManagers->addObject(pNode);
            m_pAnimationManagersForNodes->addObject(manager);
        }
    }
    
    return pNodeGraph;
}

Scene* CCBReader::createSceneWithNodeGraphFromFile(const char *pCCBFileName)
{
    return createSceneWithNodeGraphFromFile(pCCBFileName, NULL);
}

Scene* CCBReader::createSceneWithNodeGraphFromFile(const char *pCCBFileName, Object *pOwner)
{
    return createSceneWithNodeGraphFromFile(pCCBFileName, pOwner, Director::getInstance()->getWinSize());
}

Scene* CCBReader::createSceneWithNodeGraphFromFile(const char *pCCBFileName, Object *pOwner, const Size &parentSize)
{
    Node *pNode = readNodeGraphFromFile(pCCBFileName, pOwner, parentSize);
    Scene *pScene = Scene::create();
    pScene->addChild(pNode);
    
    return pScene;
}

void CCBReader::cleanUpNodeGraph(Node *pNode)
{
    pNode->setUserObject(NULL);
    
    Object *pChild = NULL;
    CCARRAY_FOREACH(pNode->getChildren(), pChild)
    {
        cleanUpNodeGraph(static_cast<Node*>(pChild));
    }
}

Node* CCBReader::readFileWithCleanUp(bool bCleanUp, Dictionary* am)
{
    if (! readHeader())
    {
        return NULL;
    }
    
    if (! readStringCache())
    {
        return NULL;
    }
    
    if (! readSequences())
    {
        return NULL;
    }
    
    setAnimationManagers(am);

    Node *pNode = readNodeGraph(NULL);

    m_pActionManagers->setObject(m_pActionManager, KDintptr(pNode));

    if (bCleanUp)
    {
        cleanUpNodeGraph(pNode);
    }
    
    return pNode;
}

bool CCBReader::readStringCache() {
    int numStrings = this->readInt(false);

    for(int i = 0; i < numStrings; i++) {
        this->m_aStringCache.push_back(this->readUTF8());
    }

    return true;
}

bool CCBReader::readHeader()
{
    /* If no bytes loaded, don't crash about it. */
    if(this->m_pBytes == NULL) {
        return false;
    }

    /* Read magic bytes */
    int magicBytes = *((int*)(this->m_pBytes + this->m_nCurrentByte));
    this->m_nCurrentByte += 4;

    if(CC_SWAP_INT32_BIG_TO_HOST(magicBytes) != (*reinterpret_cast<const int*>("ccbi"))) {
        return false; 
    }

    /* Read version. */
    int version = this->readInt(false);
    if(version != CCB_VERSION) {
        log("WARNING! Incompatible ccbi file version (file: %d reader: %d)", version, CCB_VERSION);
        return false;
    }

    // Read JS check
    m_bJsControlled = this->readBool();
    m_pActionManager->m_bJsControlled = m_bJsControlled;

    return true;
}

unsigned char CCBReader::readByte()
{
    unsigned char byte = this->m_pBytes[this->m_nCurrentByte];
    this->m_nCurrentByte++;
    return byte;
}

bool CCBReader::readBool()
{
    return 0 == this->readByte() ? false : true;
}

std::string CCBReader::readUTF8()
{
    std::string ret;

    int b0 = this->readByte();
    int b1 = this->readByte();

    int numBytes = b0 << 8 | b1;

    char* pStr = (char*) kdMalloc(numBytes+1);
    memcpy(pStr, m_pBytes+m_nCurrentByte, numBytes);
    pStr[numBytes] = '\0';
    ret = pStr;
    kdFree(pStr);

    m_nCurrentByte += numBytes;

    return ret;
}

bool CCBReader::getBit() {
    bool bit;
    unsigned char byte = *(this->m_pBytes + this->m_nCurrentByte);
    if(byte & (1 << this->m_nCurrentBit)) {
        bit = true;
    } else {
        bit = false;
    }

    this->m_nCurrentBit++;

    if(this->m_nCurrentBit >= 8) {
        this->m_nCurrentBit = 0;
        this->m_nCurrentByte++;
    }

    return bit;
}

void CCBReader::alignBits() {
    if(this->m_nCurrentBit) {
        this->m_nCurrentBit = 0;
        this->m_nCurrentByte++;
    }
}

int CCBReader::readInt(bool pSigned) {
    // Read encoded int
    int numBits = 0;
    while(!this->getBit()) {
        numBits++;
    }
    
    long long current = 0;
    for(int a = numBits - 1; a >= 0; a--) {
        if(this->getBit()) {
            current |= 1LL << a;
        }
    }
    current |= 1LL << numBits;
    
    int num;
    if(pSigned) {
        int s = current % 2;
        if(s) {
            num = static_cast<int>(current / 2);
        } else {
            num = static_cast<int>(-current / 2);
        }
    } else {
        num = static_cast<int>(current - 1);
    }
    
    this->alignBits();
    
    return num;
}


float CCBReader::readFloat()
{
    FloatType type = static_cast<FloatType>(this->readByte());
    
    switch (type)
    {
        case FloatType::_0:
            return 0;    
        case FloatType::_1:
            return 1;
        case FloatType::MINUS1:
            return -1;
        case FloatType::_05:
            return 0.5f;
        case FloatType::INTEGER:
            return (float)this->readInt(true);
        default:
            {
                /* using a memcpy since the compiler isn't
                 * doing the float ptr math correctly on device.
                 * TODO still applies in C++ ? */
                unsigned char* pF = (this->m_pBytes + this->m_nCurrentByte);
                float f = 0;
                
                // N.B - in order to avoid an unaligned memory access crash on 'memcpy()' the the (void*) casts of the source and
                // destination pointers are EXTREMELY important for the ARM compiler.
                //
                // Without a (void*) cast, the ARM compiler makes the assumption that the float* pointer is naturally aligned
                // according to it's type size (aligned along 4 byte boundaries) and thus tries to call a more optimized
                // version of memcpy() which makes this alignment assumption also. When reading back from a file of course our pointers
                // may not be aligned, hence we need to avoid the compiler making this assumption. The (void*) cast serves this purpose,
                // and causes the ARM compiler to choose the slower, more generalized (unaligned) version of memcpy()
                //
                // For more about this compiler behavior, see:
                // http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka3934.html
                kdMemcpy((void*) &f, (const void*) pF, sizeof(float));
                
                this->m_nCurrentByte += sizeof(float);
                return f;
            }
    }
}

std::string CCBReader::readCachedString()
{
    int n = this->readInt(false);
    return this->m_aStringCache[n];
}

Node * CCBReader::readNodeGraph(Node * pParent)
{
    /* Read class name. */
    std::string className = this->readCachedString();

    std::string _jsControlledName;
    
    if(m_bJsControlled) {
        _jsControlledName = this->readCachedString();
    }
    
    // Read assignment type and name
    TargetType memberVarAssignmentType = static_cast<TargetType>(this->readInt(false));
    std::string memberVarAssignmentName;
    if(memberVarAssignmentType != TargetType::NONE)
    {
        memberVarAssignmentName = this->readCachedString();
    }
    
    NodeLoader *ccNodeLoader = this->m_pNodeLoaderLibrary->getNodeLoader(className.c_str());
     
    if (! ccNodeLoader)
    {
        log("no corresponding node loader for %s", className.c_str());
        return NULL;
    }

    Node *node = ccNodeLoader->loadNode(pParent, this);

    // Set root node
    if (! m_pActionManager->getRootNode())
    {
        m_pActionManager->setRootNode(node);
    }
    
    // Assign controller
    if(m_bJsControlled && node == m_pActionManager->getRootNode())
    {
        m_pActionManager->setDocumentControllerName(_jsControlledName);
    }

    // Read animated properties
    Dictionary *seqs = Dictionary::create();
    m_pAnimatedProps = new set<string>();
    
    int numSequence = readInt(false);
    for (int i = 0; i < numSequence; ++i)
    {
        int seqId = readInt(false);
        Dictionary *seqNodeProps = Dictionary::create();
        
        int numProps = readInt(false);
        
        for (int j = 0; j < numProps; ++j)
        {
            CCBSequenceProperty *seqProp = new CCBSequenceProperty();
            seqProp->autorelease();
            
            seqProp->setName(readCachedString().c_str());
            seqProp->setType(readInt(false));
            m_pAnimatedProps->insert(seqProp->getName());
            
            int numKeyframes = readInt(false);
            
            for (int k = 0; k < numKeyframes; ++k)
            {
                CCBKeyframe *keyframe = readKeyframe(static_cast<PropertyType>(seqProp->getType()));
                
                seqProp->getKeyframes()->addObject(keyframe);
            }
            
            seqNodeProps->setObject(seqProp, seqProp->getName());
        }
        
        seqs->setObject(seqNodeProps, seqId);
    }
    
    if (seqs->count() > 0)
    {
        m_pActionManager->addNode(node, seqs);
    }
    
    // Read properties
    ccNodeLoader->parseProperties(node, pParent, this);
    
    bool isCCBFileNode = (NULL == dynamic_cast<CCBFile*>(node)) ? false : true;
    // Handle sub ccb files (remove middle node)
    if (isCCBFileNode)
    {
        CCBFile *ccbFileNode = (CCBFile*)node;
        
        Node *embeddedNode = ccbFileNode->getCCBFileNode();
        embeddedNode->setPosition(ccbFileNode->getPosition());
        embeddedNode->setRotation(ccbFileNode->getRotation());
        embeddedNode->setScaleX(ccbFileNode->getScaleX());
        embeddedNode->setScaleY(ccbFileNode->getScaleY());
        embeddedNode->setTag(ccbFileNode->getTag());
        embeddedNode->setVisible(true);
        //embeddedNode->ignoreAnchorPointForPosition(ccbFileNode->isIgnoreAnchorPointForPosition());
        
        m_pActionManager->moveAnimationsFromNode(ccbFileNode, embeddedNode);

        ccbFileNode->setCCBFileNode(NULL);
        
        node = embeddedNode;
    }

#ifdef CCB_ENABLE_JAVASCRIPT
    /*
     if (memberVarAssignmentType && memberVarAssignmentName && ![memberVarAssignmentName isEqualToString:@""])
     {
     [[JSCocoa sharedController] setObject:node withName:memberVarAssignmentName];
     }*/
#else
    if (memberVarAssignmentType != TargetType::NONE)
    {
        if(!m_bJsControlled)
        {
            Object * target = NULL;
            if(memberVarAssignmentType == TargetType::DOCUMENT_ROOT)
            {
                target = m_pActionManager->getRootNode();
            } 
            else if(memberVarAssignmentType == TargetType::OWNER)
            {
                target = this->m_pOwner;
            }
            
            if(target != NULL)
            {
                CCBMemberVariableAssigner * targetAsCCBMemberVariableAssigner = dynamic_cast<CCBMemberVariableAssigner *>(target);
                
                bool assigned = false;
                if (memberVarAssignmentType != TargetType::NONE)
                {
                    if(targetAsCCBMemberVariableAssigner != NULL)
                    {
                        assigned = targetAsCCBMemberVariableAssigner->onAssignCCBMemberVariable(target, memberVarAssignmentName.c_str(), node);
                    }
                    
                    if(!assigned && this->m_pCCBMemberVariableAssigner != NULL)
                    {
                        assigned = this->m_pCCBMemberVariableAssigner->onAssignCCBMemberVariable(target, memberVarAssignmentName.c_str(), node);
                    }
                }
            }
        }
        else
        {
            if(memberVarAssignmentType == TargetType::DOCUMENT_ROOT)
            {
                m_pActionManager->addDocumentOutletName(memberVarAssignmentName);
                m_pActionManager->addDocumentOutletNode(node);
            }
            else
            {
                m_aOwnerOutletNames.push_back(memberVarAssignmentName);
                m_pOwnerOutletNodes->addObject(node);
            }
        }
    }
    
    // Assign custom properties.
    if (ccNodeLoader->getCustomProperties()->count() > 0)
    {
        bool customAssigned = false;
        
        if(!m_bJsControlled)
        {
            Object * target = node;
            if(target != NULL)
            {
                CCBMemberVariableAssigner * targetAsCCBMemberVariableAssigner = dynamic_cast<CCBMemberVariableAssigner *>(target);
                if(targetAsCCBMemberVariableAssigner != NULL)
                {
                    Dictionary* pCustomPropeties = ccNodeLoader->getCustomProperties();
                    DictElement* pElement;
                    CCDICT_FOREACH(pCustomPropeties, pElement)
                    {
                        customAssigned = targetAsCCBMemberVariableAssigner->onAssignCCBCustomProperty(target, pElement->getStrKey(), static_cast<CCBValue*>(pElement->getObject()));

                        if(!customAssigned && this->m_pCCBMemberVariableAssigner != NULL)
                        {
                            customAssigned = this->m_pCCBMemberVariableAssigner->onAssignCCBCustomProperty(target, pElement->getStrKey(), static_cast<CCBValue*>(pElement->getObject()));
                        }
                    }
                }
            }
        }
    }

#endif // CCB_ENABLE_JAVASCRIPT
    
    delete m_pAnimatedProps;
    m_pAnimatedProps = NULL;

    /* Read and add children. */
    int numChildren = this->readInt(false);
    for(int i = 0; i < numChildren; i++)
    {
        Node * child = this->readNodeGraph(node);
        node->addChild(child);
    }

    // FIX ISSUE #1860: "onNodeLoaded will be called twice if ccb was added as a CCBFile".
    // If it's a sub-ccb node, skip notification to NodeLoaderListener since it will be
    // notified at LINE #734: Node * child = this->readNodeGraph(node);
    if (!isCCBFileNode)
    {
        // Call onNodeLoaded
        NodeLoaderListener * nodeAsNodeLoaderListener = dynamic_cast<NodeLoaderListener *>(node);
        if(nodeAsNodeLoaderListener != NULL)
        {
            nodeAsNodeLoaderListener->onNodeLoaded(node, ccNodeLoader);
        }
        else if(this->m_pNodeLoaderListener != NULL)
        {
            this->m_pNodeLoaderListener->onNodeLoaded(node, ccNodeLoader);
        }
    }
    return node;
}

CCBKeyframe* CCBReader::readKeyframe(PropertyType type)
{
    CCBKeyframe *keyframe = new CCBKeyframe();
    keyframe->autorelease();
    
    keyframe->setTime(readFloat());
    
    CCBKeyframe::EasingType easingType = static_cast<CCBKeyframe::EasingType>(readInt(false));
    float easingOpt = 0;
    Object *value = NULL;
    
    if (easingType == CCBKeyframe::EasingType::CUBIC_IN
        || easingType == CCBKeyframe::EasingType::CUBIC_OUT
        || easingType == CCBKeyframe::EasingType::CUBIC_INOUT
        || easingType == CCBKeyframe::EasingType::ELASTIC_IN
        || easingType == CCBKeyframe::EasingType::ELASTIC_OUT
        || easingType == CCBKeyframe::EasingType::ELASTIC_INOUT)
    {
        easingOpt = readFloat();
    }
    keyframe->setEasingType(easingType);
    keyframe->setEasingOpt(easingOpt);
    
    if (type == PropertyType::CHECK)
    {
        value = CCBValue::create(readBool());
    }
    else if (type == PropertyType::BYTE)
    {
        value = CCBValue::create(readByte());
    }
    else if (type == PropertyType::COLOR3)
    {
        int r = readByte();
        int g = readByte();
        int b = readByte();
        
        Color3B c = Color3B(r,g,b);
        value = Color3BWapper::create(c);
    }
    else if (type == PropertyType::DEGREES)
    {
        value = CCBValue::create(readFloat());
    }
    else if (type == PropertyType::SCALE_LOCK || type == PropertyType::POSITION
	     || type == PropertyType::FLOAT_XY)
    {
        float a = readFloat();
        float b = readFloat();
        
        value = Array::create(CCBValue::create(a),
                                CCBValue::create(b),
                                NULL);
    }
    else if (type == PropertyType::SPRITEFRAME)
    {
        std::string spriteSheet = readCachedString();
        std::string spriteFile = readCachedString();
        
        SpriteFrame* spriteFrame;

        if (spriteSheet.length() == 0)
        {
            spriteFile = m_sCCBRootPath + spriteFile;

            Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(spriteFile.c_str());
            Rect bounds = Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height);
            
            spriteFrame = SpriteFrame::createWithTexture(texture, bounds);
        }
        else
        {
            spriteSheet = m_sCCBRootPath + spriteSheet;
            SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
            
            // Load the sprite sheet only if it is not loaded            
            if (m_aLoadedSpriteSheets.find(spriteSheet) == m_aLoadedSpriteSheets.end())
            {
                frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
                m_aLoadedSpriteSheets.insert(spriteSheet);
            }
            
            spriteFrame = frameCache->getSpriteFrameByName(spriteFile.c_str());
        }
        value = spriteFrame;
    }
    
    keyframe->setValue(value);
    
    return  keyframe;
}


bool CCBReader::readCallbackKeyframesForSeq(CCBSequence* seq)
{
    int numKeyframes = readInt(false);
    if(!numKeyframes) return true;
    
    CCBSequenceProperty* channel = new CCBSequenceProperty();
    channel->autorelease();

    for(int i = 0; i < numKeyframes; ++i) {
      
        float time = readFloat();
        std::string callbackName = readCachedString();
      
        int callbackType = readInt(false);
      
        Array* value = Array::create();
        value->addObject(String::create(callbackName));
        value->addObject(String::createWithFormat("%d", callbackType));
        
        CCBKeyframe* keyframe = new CCBKeyframe();
        keyframe->autorelease();
        
        keyframe->setTime(time);
        keyframe->setValue(value);
        
        if(m_bJsControlled) {
            string callbackIdentifier;
            m_pActionManager->getKeyframeCallbacks()->addObject(String::createWithFormat("%d:%s",callbackType, callbackName.c_str()));
        }
    
        channel->getKeyframes()->addObject(keyframe);
    }
    
    seq->setCallbackChannel(channel);
    
    return true;
}

bool CCBReader::readSoundKeyframesForSeq(CCBSequence* seq) {
    int numKeyframes = readInt(false);
    if(!numKeyframes) return true;
    
    CCBSequenceProperty* channel = new CCBSequenceProperty();
    channel->autorelease();

    for(int i = 0; i < numKeyframes; ++i) {
        
        float time = readFloat();
        std::string soundFile = readCachedString();
        float pitch = readFloat();
        float pan = readFloat();
        float gain = readFloat();
                
        Array* value = Array::create();
        
        value->addObject(String::create(soundFile));
        value->addObject(String::createWithFormat("%f", pitch));
        value->addObject(String::createWithFormat("%f", pan));
        value->addObject(String::createWithFormat("%f", gain));
        
        CCBKeyframe* keyframe = new CCBKeyframe();
        keyframe->setTime(time);
        keyframe->setValue(value);
        channel->getKeyframes()->addObject(keyframe);
        keyframe->release();
    }
    
    seq->setSoundChannel(channel);
    
    return true;
}


Node * CCBReader::readNodeGraph() {
    return this->readNodeGraph(NULL);
}

bool CCBReader::readSequences()
{
    Array *sequences = m_pActionManager->getSequences();
    
    int numSeqs = readInt(false);
    
    for (int i = 0; i < numSeqs; i++)
    {
        CCBSequence *seq = new CCBSequence();
        seq->autorelease();
        
        seq->setDuration(readFloat());
        seq->setName(readCachedString().c_str());
        seq->setSequenceId(readInt(false));
        seq->setChainedSequenceId(readInt(true));
        
        if(!readCallbackKeyframesForSeq(seq)) return false;
        if(!readSoundKeyframesForSeq(seq)) return false;
        
        sequences->addObject(seq);
    }
    
    m_pActionManager->setAutoPlaySequenceId(readInt(true));
    return true;
}

std::string CCBReader::lastPathComponent(const char* pPath) {
    std::string path(pPath);
    size_t slashPos = path.find_last_of("/");
    if(slashPos != std::string::npos) {
        return path.substr(slashPos + 1, path.length() - slashPos);
    }
    return path;
}

std::string CCBReader::deletePathExtension(const char* pPath) {
    std::string path(pPath);
    size_t dotPos = path.find_last_of(".");
    if(dotPos != std::string::npos) {
        return path.substr(0, dotPos);
    }
    return path;
}

std::string CCBReader::toLowerCase ( const KDchar* pString ) {
    std::string copy(pString);
    std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
    return copy;
}

bool CCBReader::endsWith(const char* pString, const char* pEnding) {
    std::string string(pString);
    std::string ending(pEnding);
    if(string.length() >= ending.length()) {
        return (string.compare(string.length() - ending.length(), ending.length(), ending) == 0);
    } else {
        return false;
    }
}

bool CCBReader::isJSControlled()
{
    return m_bJsControlled;
}

void CCBReader::addOwnerCallbackName(const std::string& name)
{
    m_sOwnerCallbackNames.push_back(name);
}

void CCBReader::addOwnerCallbackNode(Node *node)
{
    m_pOwnerCallbackNodes->addObject(node);
}

void CCBReader::addOwnerCallbackControlEvents(Control::EventType type)
{
    m_pOwnerOwnerCallbackControlEvents->addObject(Integer::create((int)type));
}

void CCBReader::addDocumentCallbackName(const std::string& name)
{
    m_pActionManager->addDocumentCallbackName(name);
}

void CCBReader::addDocumentCallbackNode(Node *node)
{
    m_pActionManager->addDocumentCallbackNode(node);
}

void CCBReader::addDocumentCallbackControlEvents(Control::EventType eventType)
{
    m_pActionManager->addDocumentCallbackControlEvents(eventType);
}

Array* CCBReader::getOwnerCallbackNames()
{
    Array* pRet = Array::createWithCapacity(m_sOwnerCallbackNames.size());
    std::vector<std::string>::iterator it = m_sOwnerCallbackNames.begin();
    for (; it != m_sOwnerCallbackNames.end(); ++it)
    {
        pRet->addObject(String::create(*it));
    }
    
    return pRet;
}

Array* CCBReader::getOwnerCallbackNodes()
{
    return m_pOwnerCallbackNodes;
}

Array* CCBReader::getOwnerCallbackControlEvents()
{
    return m_pOwnerOwnerCallbackControlEvents;
}

Array* CCBReader::getOwnerOutletNames()
{
    Array* pRet = Array::createWithCapacity(m_aOwnerOutletNames.size());
    std::vector<std::string>::iterator it = m_aOwnerOutletNames.begin();
    for (; it != m_aOwnerOutletNames.end(); ++it)
    {
        pRet->addObject(String::create(*it));
    }
    return pRet;
}

Array* CCBReader::getOwnerOutletNodes()
{
    return m_pOwnerOutletNodes;
}

Array* CCBReader::getNodesWithAnimationManagers()
{
    return m_pNodesWithAnimationManagers;
}

Array* CCBReader::getAnimationManagersForNodes()
{
    return m_pAnimationManagersForNodes;
}

void CCBReader::addOwnerOutletName(std::string name)
{
    m_aOwnerOutletNames.push_back(name);
}

void CCBReader::addOwnerOutletNode(Node *node)
{
    if (NULL == node)
        return;
    
    m_pOwnerOutletNodes->addObject(node);
}

/************************************************************************
 Static functions
 ************************************************************************/

static float __ccbResolutionScale = 1.0f;

float CCBReader::getResolutionScale()
{
    return __ccbResolutionScale;
}

void CCBReader::setResolutionScale(float scale)
{
    __ccbResolutionScale = scale;
}

};
