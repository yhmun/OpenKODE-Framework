/* --------------------------------------------------------------------------
 *
 *      File            cpShapeNode.cpp
 *      Description     Provide Drawing for Shapes
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2009 Robert Blackwood 02/22/2009
 *
 *         Mobile Bros. All rights reserved.
 *
 *         http://www.mobile-bros.com  
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
 *      version 2 of the License, or (at your option) any later version.
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

#include "Precompiled.h"
#include "extensions/CCSpaceManager/CCSpaceManager.h"
#include "support/CCPointExtension.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"

NS_CC_BEGIN

#define CP_CIRCLE_PT_COUNT	26
#define CP_SEG_PT_COUNT		26

static const KDint    SMGR_sPtCount = CP_SEG_PT_COUNT;
static const GLfloat  SMGR_sPt [ CP_SEG_PT_COUNT + CP_SEG_PT_COUNT ] = 
{
	 0.0000f,  1.0000f,
	 0.2588f,  0.9659f,
 	 0.5000f,  0.8660f,
	 0.7071f,  0.7071f,
	 0.8660f,  0.5000f,
	 0.9659f,  0.2588f,
	 1.0000f,  0.0000f,
	 0.9659f, -0.2588f,
	 0.8660f, -0.5000f,
	 0.7071f, -0.7071f,
	 0.5000f, -0.8660f,
	 0.2588f, -0.9659f,
	 0.0000f, -1.0000f,
	
	 0.0000f, -1.0000f,
	-0.2588f, -0.9659f,
	-0.5000f, -0.8660f,
	-0.7071f, -0.7071f,
	-0.8660f, -0.5000f,
	-0.9659f, -0.2588f,
	-1.0000f, -0.0000f,
	-0.9659f,  0.2588f,
	-0.8660f,  0.5000f,
	-0.7071f,  0.7071f,
	-0.5000f,  0.8660f,
	-0.2588f,  0.9659f,
	 0.0000f,  1.0000f,
};

static const KDint   SMGR_cPtCount = CP_CIRCLE_PT_COUNT;
static const GLfloat SMGR_cPt [ CP_CIRCLE_PT_COUNT + CP_CIRCLE_PT_COUNT ] = 
{
	 0.0000f,  1.0000f,
	 0.2588f,  0.9659f,
	 0.5000f,  0.8660f,
	 0.7071f,  0.7071f,
	 0.8660f,  0.5000f,
	 0.9659f,  0.2588f,
	 1.0000f,  0.0000f,
	 0.9659f, -0.2588f,
	 0.8660f, -0.5000f,
	 0.7071f, -0.7071f,
	 0.5000f, -0.8660f,
	 0.2588f, -0.9659f,
	 0.0000f, -1.0000f,
	-0.2588f, -0.9659f,
	-0.5000f, -0.8660f,
	-0.7071f, -0.7071f,
	-0.8660f, -0.5000f,
	-0.9659f, -0.2588f,
	-1.0000f, -0.0000f,
	-0.9659f,  0.2588f,
	-0.8660f,  0.5000f,
	-0.7071f,  0.7071f,
	-0.5000f,  0.8660f,
	-0.2588f,  0.9659f,
	 0.0000f,  1.0000f,
	 0.0000f,  0.4500f, // For an extra line to see the rotation.
};

KDvoid cpShapeNodePreDrawState ( CCGLProgram* pShader )
{
	pShader->use ( );
	pShader->setUniformsForBuiltins ( );
    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );
}

KDvoid cpShapeNodePostDrawState ( KDvoid )
{

}

static KDvoid _drawCircleShape ( GLfloat* pVertices, KDint nVerticesCount, KDbool bFill, KDbool bDrawLine )
{
	KDint  nExtraPtOffset = bDrawLine ? 0 : 1;
	
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pVertices );
	
	if ( bFill )
	{
		glDrawArrays ( GL_TRIANGLE_FAN, 0, nVerticesCount - nExtraPtOffset - 1 );
	}
    else
	{
        glDrawArrays ( GL_LINE_STRIP, 0, nVerticesCount - nExtraPtOffset );
	}

	CC_INCREMENT_GL_DRAWS ( 1 );
}

static KDvoid _drawPolyShape ( GLfloat* pVertices, KDint nVerticesCount, KDbool bFill )
{
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pVertices );
    
	if ( bFill )
	{
		glDrawArrays ( GL_TRIANGLE_FAN, 0, nVerticesCount );
	}
    else
	{
        glDrawArrays ( GL_LINE_LOOP, 0, nVerticesCount );
	}

	CC_INCREMENT_GL_DRAWS ( 1 );
}

static KDvoid _drawSegmentShape ( GLfloat* pVertices, KDint nVerticesCount, KDbool bFill )
{
	_drawPolyShape ( pVertices, nVerticesCount, bFill );
}

static KDvoid cacheSentinel ( GLfloat** ppVertices, KDint* pVerticesCount, KDint nNewCount )
{
	if ( *pVerticesCount != nNewCount )
	{
		kdFree ( *ppVertices );
		*ppVertices = KD_NULL;
	}
	
	if ( *ppVertices == KD_NULL )
	{
		*ppVertices = (GLfloat*) kdMalloc ( sizeof ( GLfloat ) * 2 * nNewCount );
		*pVerticesCount = nNewCount;
	}
}

static KDvoid  _cacheCircle ( cpShape* pShape, cpVect tPos, cpVect tRotation, GLfloat** ppVertices, KDint* pVerticesCount )
{	
    KDbool   bRotate = !ccpFuzzyEqual ( tRotation, CCPointZero, 0.1f );
	cpFloat  fRadius = cpCircleShapeGetRadius ( pShape );
    cpVect   tOffset = cpCircleShapeGetOffset ( pShape );
    cpVect   tPoint;

	cacheSentinel ( ppVertices, pVerticesCount, SMGR_cPtCount );

	for ( KDint i = 0; i < SMGR_cPtCount; i++ )
	{		
        tPoint = cpv ( tOffset.x + ( SMGR_cPt [ i * 2 + 0 ] * fRadius ),
                       tOffset.y + ( SMGR_cPt [ i * 2 + 1 ] * fRadius ) );
        if ( bRotate )
		{
            tPoint = cpvrotate ( tPoint, tRotation );
		}

		( *ppVertices ) [ i * 2 + 0 ] = tPos.x + tPoint.x;
		( *ppVertices ) [ i * 2 + 1 ] = tPos.y + tPoint.y;
	}
}

static KDvoid  _cachePoly ( cpShape* pShape, cpVect tPos, cpVect tRotation, GLfloat** ppVertices, KDint* pVerticesCount )
{	
    KDbool  bRotate = !ccpFuzzyEqual ( tRotation, CCPointZero, 0.1f );
	KDint   nCount  = cpPolyShapeGetNumVerts ( pShape );
 
	cacheSentinel ( ppVertices, pVerticesCount, nCount );

	for ( KDint i = 0; i < nCount; i++ )
	{		
        cpVect v = cpPolyShapeGetVert ( pShape, i );

        if ( bRotate )
		{
            v = cpvrotate ( v, tRotation );
		}

		( *ppVertices ) [ i * 2 + 0 ] = tPos.x + v.x;
		( *ppVertices ) [ i * 2 + 1 ] = tPos.y + v.y;
	}
}

static KDvoid _cacheSegment ( cpShape* pShape, cpVect tPos, cpVect tRotation, GLfloat** ppVertices, KDint* pVerticesCount )
{	
	cpVect  tA = cpSegmentShapeGetA ( pShape );
	cpVect  tB = cpSegmentShapeGetB ( pShape );
	cpFloat fRadius = cpSegmentShapeGetRadius ( pShape );
	
	if ( fRadius )
	{
		cacheSentinel ( ppVertices, pVerticesCount, SMGR_sPtCount );
	
        KDbool  bRotate = !ccpFuzzyEqual ( tRotation, CCPointZero, 0.1f );            
		cpVect  tDelta  = cpvsub ( tB, tA );
		cpVect  tMid    = cpvmult ( cpvadd ( tA, tB ), 0.5f );
		cpFloat fLen    = cpvlength ( tDelta );
		cpFloat fHalf   = fLen / 2;
		cpVect  tNorm   = cpvmult ( tDelta, 1 / fLen );
		cpVect  tPoint;
		
		for ( KDint i = 0; i < SMGR_sPtCount; i++ )
		{
			tPoint.x = SMGR_sPt [ i * 2 + 0 ] * fRadius;
			tPoint.y = SMGR_sPt [ i * 2 + 1 ] * fRadius;
			
			if ( i < SMGR_sPtCount / 2 )
			{
				tPoint.x += fHalf;
			}
			else
			{
				tPoint.x -= fHalf;
			}
			
			tPoint = cpvrotate ( tPoint, tNorm );
			tPoint = cpvadd ( tPoint, tMid );
            
            if ( bRotate )
			{
                tPoint = cpvrotate ( tPoint, tRotation );
			}
			
			( *ppVertices ) [ i * 2 + 0 ] = tPos.x + tPoint.x;
			( *ppVertices ) [ i * 2 + 1 ] = tPos.y + tPoint.y;
		}
	}
	else 
	{
		cacheSentinel ( ppVertices, pVerticesCount, 2 );
		
		( *ppVertices ) [0] = tPos.x + tA.x;
		( *ppVertices ) [1] = tPos.y + tA.y;
		( *ppVertices ) [2] = tPos.x + tB.x;
		( *ppVertices ) [3] = tPos.y + tB.y;
	}
}

KDvoid cpShapeNodeEfficientDrawAt ( cpShape* pShape, cpVect tPoint, cpVect tRotation )
{
    GLfloat*	 pVertices = KD_NULL;
	KDint		 nVerticesCount = 0;
    cpShapeType  eType = pShape->CP_PRIVATE ( klass )->type;
    
    switch ( eType )
	{
		case CP_CIRCLE_SHAPE :
            _cacheCircle ( pShape, tPoint, tRotation, &pVertices, &nVerticesCount );
			_drawCircleShape ( pVertices, nVerticesCount, KD_FALSE, KD_TRUE );
			break;

		case CP_SEGMENT_SHAPE :
            _cacheSegment ( pShape, tPoint, tRotation, &pVertices, &nVerticesCount );
			_drawSegmentShape ( pVertices, nVerticesCount, KD_FALSE );
			break;

		case CP_POLY_SHAPE :
            _cachePoly ( pShape, tPoint, tRotation, &pVertices, &nVerticesCount );
			_drawPolyShape ( pVertices, nVerticesCount, KD_FALSE );
			break;

		default :
			break;
	}
    
    if ( pVertices )
	{
        kdFree ( pVertices );
	}
}

KDvoid cpShapeNodeDrawAt ( cpShape* pShape, cpVect tPoint, cpVect tRotation )
{    
	cpShapeNodePreDrawState  ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_Position_uColor ) );
    cpShapeNodeEfficientDrawAt ( pShape, tPoint, tRotation );
	cpShapeNodePostDrawState ( );
}

cpShapeNode* cpShapeNode::create ( cpShape* pShape )
{
	cpShapeNode*  pRet = new cpShapeNode ( );
	
	if ( pRet && pRet->initWithShape ( pShape ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool cpShapeNode::initWithShape ( cpShape* pShape )
{
	if ( !cpCCNode::initWithShape ( pShape ) )
	{
		return KD_FALSE;
	}
	
	m_fPointSize		= 3;
	m_fLineWidth		= 1;
	m_bSmoothDraw		= KD_FALSE;	
	m_bFillShape		= KD_TRUE;
	m_bDrawDecoration	= KD_TRUE;
	m_bCacheDraw		= KD_TRUE;
	m_pVertices			= KD_NULL;
	m_nVerticesCount	= 0;
	m_pShape			= KD_NULL;
	
	// Invalid type, force initial cache
	m_eLastType			= CP_NUM_SHAPES;
	
	// Set internals
	this->setContentSize ( CCSize ( pShape->bb.r - pShape->bb.l, pShape->bb.t - pShape->bb.b ) );
	this->setAnchorPoint ( ccp ( 0, 0 ) );
   
	this->setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_Position_uColor ) );

#if defined ( USE_OPEN_GLES2 )    
    m_nColorLocation = glGetUniformLocation ( this->getShaderProgram ( )->getProgram ( ), "u_color" );
    m_nPointLocation = glGetUniformLocation ( this->getShaderProgram ( )->getProgram ( ), "u_pointSize" );
#endif

	return KD_TRUE;
}

cpShapeNode::cpShapeNode ( KDvoid )
{
	m_pShape    = KD_NULL;
	m_pVertices = KD_NULL;
}

cpShapeNode::~cpShapeNode ( KDvoid )
{
	CC_SAFE_FREE ( m_pVertices );
}

KDvoid cpShapeNode::setShape ( cpShape* pShape )
{
	cpCCNode::setShape ( pShape );
	m_pShape = pShape;
}

cpShape* cpShapeNode::getShape ( KDvoid )
{
	if ( m_pShape )
	{
		return m_pShape;
	}
	else
	{
		return m_pImplementation->getShape ( );
	}
}

KDvoid cpShapeNode::cacheCircle ( KDvoid )
{
	_cacheCircle ( this->getShape ( ), cpvzero, cpvzero, &m_pVertices, &m_nVerticesCount );
}

KDvoid cpShapeNode::cachePoly ( KDvoid )
{
	_cachePoly ( this->getShape ( ), cpvzero, cpvzero, &m_pVertices, &m_nVerticesCount );
}

KDvoid cpShapeNode::cacheSegment ( KDvoid )
{
	_cacheSegment ( this->getShape ( ), cpvzero, cpvzero, &m_pVertices, &m_nVerticesCount );
}

KDvoid cpShapeNode::preDrawState ( KDvoid )
{
	this->getShaderProgram ( )->use ( );
	this->getShaderProgram ( )->setUniformsForBuiltins ( );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

	ccColor4F  tColor = ccc4FFromccc3B ( m_tDisplayedColor );
	tColor.a = m_cDisplayedOpacity / 255.f;

	// ccPointSize ( m_fPointSize );
	// ccLineWidth ( m_fLineWidth );

#if defined ( USE_OPEN_GLES2 )
	this->getShaderProgram ( )->setUniformLocationWith4fv ( m_nColorLocation, (GLfloat*) &tColor.r, 1 );    
#else
	glColor4f ( tColor.r, tColor.g, tColor.b, tColor.a );
#endif
}

KDvoid cpShapeNode::postDrawState ( KDvoid )
{
	cpShapeNodePostDrawState ( );
}

KDvoid cpShapeNode::draw ( KDvoid )
{
    cpShape*  pShape = this->getShape ( );
    
	if ( pShape == KD_NULL )
	{
		return;
	}

	cpShapeType  eType = pShape->CP_PRIVATE ( klass )->type;
	
	// need to update verts? type changed, or don't want to cache
	KDbool  bUpdate = ( eType != m_eLastType ) || ( !m_bCacheDraw );
	m_eLastType = eType;	

	this->preDrawState ( );
	
    switch ( eType )
    {
        case CP_CIRCLE_SHAPE :
            
			if ( bUpdate ) 
			{
				this->cacheCircle ( );
			}
            this->drawCircleShape ( );

            break;

        case CP_SEGMENT_SHAPE :
            
			if ( bUpdate ) 
			{
				this->cacheSegment ( );
			}
            this->drawSegmentShape ( );
            
			break;

        case CP_POLY_SHAPE :
            
			if ( bUpdate ) 
			{
				this->cachePoly ( );
			}
            this->drawPolyShape ( );
            
			break;

        default :
            break;
    }
    
	this->postDrawState ( );
	
}

KDvoid cpShapeNode::drawCircleShape ( KDvoid )
{
	_drawCircleShape ( m_pVertices, m_nVerticesCount, m_bFillShape, m_bDrawDecoration );
}

KDvoid cpShapeNode::drawSegmentShape ( KDvoid )
{
	_drawSegmentShape ( m_pVertices, m_nVerticesCount, m_bFillShape );
}

KDvoid cpShapeNode::drawPolyShape ( KDvoid )
{
	_drawPolyShape ( m_pVertices, m_nVerticesCount, m_bFillShape );
}

/*


@interface cpShapeTextureNode (Private)
-(void) cacheCoordsAndColors;
-(int) getTextureWidth;
-(int) getTextureHeight;
-(void) cacheSentinel:(int)count;
@end

@implementation cpShapeTextureNode
@synthesize texture = _texture;
@synthesize textureOffset = _textureOffset;
@synthesize textureRotation = _textureRotation;

+ (id) nodeWithShape:(cpShape *)shape file:(NSString*)file
{
	return [[[self alloc] initWithShape:shape file:file] autorelease];
}

+ (id) nodeWithShape:(cpShape*)shape texture:(CCTexture2D*)texture
{
	return [[[self alloc] initWithShape:shape texture:texture] autorelease];
}

+ (id) nodeWithShape:(cpShape*)shape batchNode:(cpShapeTextureBatchNode*)batchNode
{
	return [[[self alloc] initWithShape:shape batchNode:batchNode] autorelease];
}

- (id) initWithShape:(cpShape *)shape file:(NSString*)file
{
	return [self initWithShape:shape texture:[[CCTextureCache sharedTextureCache] addImage:file]];
}

- (id) initWithShape:(cpShape*)shape texture:(CCTexture2D*)texture
{
	[super initWithShape:shape];
	
	_color = ccc4(255, 255, 255, 255);
	_textureOffset = ccp(0,0);
	_textureRotation = 0;
	self.texture = texture;
	
	//set texture to repeat
	ccTexParams params = {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
	[_texture setTexParameters:&params];
	
	return self;
}

- (id) initWithShape:(cpShape*)shape batchNode:(cpShapeTextureBatchNode*)batchNode
{
	[self initWithShape:shape texture:nil];
	_batchNode = batchNode;
	
	return self;
}

- (void) dealloc
{
	free(_coordinates);
	free(_colors);
	self.texture = nil;
	[super dealloc];
}

-(int) getTextureWidth
{
	if (_batchNode)
		return _batchNode.texture.pixelsWide;
	else
		return _texture.pixelsWide;
}

-(int) getTextureHeight
{
	if (_batchNode)
		return _batchNode.texture.pixelsHigh;
	else
		return _texture.pixelsHigh;	
}

- (void) draw
{
	if (!_batchNode && _texture)
		glBindTexture(GL_TEXTURE_2D, _texture.name);

	[super draw];
}

- (void) preDrawState
{
	//override to do nothing		
}

- (void) postDrawState
{
	//override to do nothing
}

- (void) drawCircleShape
{	
	[self drawPolyShape];
}

- (void) drawSegmentShape
{
	[self drawPolyShape];
}

- (void) drawPolyShape;
{
	glVertexPointer(2, GL_FLOAT, 0, _vertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, _colors);
	glTexCoordPointer(2, GL_FLOAT, 0, _coordinates);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, _vertices_count);
}

- (void) cacheSentinel:(int)count
{	
	if (_vertices_count != count)
	{
		free(_colors);
		free(_coordinates);
		
		_colors = nil;
		_coordinates = nil;
	}
	
	if (_colors == nil)
		_colors = malloc(sizeof(ccColor4B)*2*count);
	if (_coordinates == nil)
		_coordinates = malloc(sizeof(GLfloat)*2*count);
}

- (void) cacheCircle
{
	[super cacheCircle];
    [self cacheSentinel:_vertices_count];
	[self cacheCoordsAndColors];
}

- (void) cachePoly
{
	[super cachePoly];
    [self cacheSentinel:_vertices_count];
	[self cacheCoordsAndColors];	
}

- (void) cacheSegment
{
	[super cacheSegment];
    [self cacheSentinel:_vertices_count];
	[self cacheCoordsAndColors];
}

-(void) cacheCoordsAndColors
{
	ccColor4B color = ccc4(_color.r, _color.g, _color.b, _color.a);
	
	const float width = [self getTextureWidth];	
	const float height = [self getTextureHeight];
	CGPoint t_rot = ccpForAngle(-CC_DEGREES_TO_RADIANS(_textureRotation));
	
	for (int i = 0; i < _vertices_count; i++)
	{	
		CGPoint coord = ccp(_vertices[i*2]/width, -_vertices[i*2+1]/height);
		if (_textureRotation)
			coord = ccpRotate(coord, t_rot);
		
		_coordinates[i*2] = coord.x + _textureOffset.x;
		_coordinates[i*2+1] = coord.y + _textureOffset.y;
		
		_colors[i*2] = color;
		_colors[i*2+1] = color;
	}	
}

@end

@implementation cpShapeTextureBatchNode
@synthesize texture = _texture;

+ (id) nodeWithFile:(NSString*)file
{
	return [[[self alloc] initWithFile:file] autorelease];
}

+ (id) nodeWithTexture:(CCTexture2D*)texture;
{
	return [[[self alloc] initWithTexture:texture] autorelease];
}

- (id) initWithFile:(NSString*)file;
{
	return [self initWithTexture:[[CCTextureCache sharedTextureCache] addImage:file]];
}

- (id) initWithTexture:(CCTexture2D*)texture;
{
	[super init];
	
	self.texture = texture;
	
	//set texture to repeat
	ccTexParams params = {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
	[_texture setTexParameters:&params];
	
	return self;
}

- (void) dealloc
{
	self.texture = nil;
	[super dealloc];
}

- (void) visit
{
	//cheap way to batch... not as efficient as CC implementation
	glBindTexture(GL_TEXTURE_2D, _texture.name);

	[super visit];
}

*/

NS_CC_END