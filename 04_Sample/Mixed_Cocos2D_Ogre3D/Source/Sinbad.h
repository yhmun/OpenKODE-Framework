/* ------------------------------------------------------------------------------------
 *
 *      File            Sinbad.h
 *      Author          Y.H Mun
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2012 Torus Knot Software Ltd.
 *
 *      For the latest info, see http://www.ogre3d.org/
 *
 * ------------------------------------------------------------------------------------
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ------------------------------------------------------------------------------------ */

#ifndef __Sinbad_h_
#define __Sinbad_h_

#define NUM_ANIMS           13          // number of animations the character has
#define CHAR_HEIGHT         5           // height of character's center of mass above ground
#define CAM_HEIGHT          2           // height of camera above character's center of mass
#define RUN_SPEED           17          // character running speed in units per second
#define TURN_SPEED          500.0f      // character turning in degrees per second
#define ANIM_FADE_SPEED     7.5f        // animation crossfade speed in % of full weight per second
#define JUMP_ACCEL          30.0f       // character jump acceleration in upward units per squared second
#define GRAVITY             90.0f       // gravity in downward units per squared second

class Sinbad
{
    private :
    
        // all the animations our character has, and a null ID
        // some of these affect separate body parts and will be blended together
        enum AnimID
        {
            ANIM_IDLE_BASE          ,
            ANIM_IDLE_TOP           ,
            ANIM_RUN_BASE           ,
            ANIM_RUN_TOP            ,
            ANIM_HANDS_CLOSED       ,
            ANIM_HANDS_RELAXED      ,
            ANIM_DRAW_SWORDS        ,
            ANIM_SLICE_VERTICAL     ,
            ANIM_SLICE_HORIZONTAL   ,
            ANIM_DANCE              ,
            ANIM_JUMP_START         ,
            ANIM_JUMP_LOOP          ,
            ANIM_JUMP_END           ,
            ANIM_NONE
        };
    
    public :
	
        Sinbad ( Camera* pCamera );
    
        KDvoid  addTime ( Real fDeltaTime );
    
		KDvoid  injectKeyDown       ( const KDEventKeypad& tKeypad );
		KDvoid  injectKeyUp         ( const KDEventKeypad& tKeypad );

        KDvoid  injectTouchMoved    ( const KDEventTouches& tTouches );
        KDvoid  injectTouchPressed  ( const KDEventTouches& tTouches );    
    
    private :
    
        KDvoid  setupBody ( SceneManager* pSceneMgr );
    
        KDvoid  setupAnimations ( KDvoid );
        
        KDvoid  setupCamera ( Camera* pCamera );
    
    
        KDvoid  updateBody ( Real fDeltaTime );
    
        KDvoid  updateAnimations ( Real fDeltaTime );
    
        KDvoid  fadeAnimations ( Real fDeltaTime );
    
        KDvoid  updateCamera ( Real fDeltaTime );
    
        KDvoid  updateCameraGoal ( Real fDeltaYaw, Real fDeltaPitch, Real fDeltaZoom );
        
    
        KDvoid  setBaseAnimation ( AnimID eID, bool bReset = false );
        
        KDvoid  setTopAnimation ( AnimID eID, bool bReset = false );
    
    
    private :

        Camera*					m_pCamera;
        SceneNode*				m_pBodyNode;
        SceneNode*				m_pCameraPivot;
        SceneNode*				m_pCameraGoal;
        SceneNode*				m_pCameraNode;
        Real					m_fPivotPitch;
        Entity*					m_pBodyEnt;
        Entity*					m_pSword1;
        Entity*					m_pSword2;
        RibbonTrail*			m_pSwordTrail;
		Ogre::AnimationState*	m_eAnims[NUM_ANIMS];            // master animation list
        AnimID					m_eBaseAnimID;                  // current base (full- or lower-body) animation
        AnimID					m_eTopAnimID;                   // current top (upper-body) animation
        bool					m_bFadingIn[NUM_ANIMS];         // which animations are fading in
        bool					m_bFadingOut[NUM_ANIMS];        // which animations are fading out
        bool					m_bSwordsDrawn;
        Vector3					m_tKeyDirection;                // player's local intended direction based on WASD keys
        Vector3					m_tGoalDirection;               // actual intended direction in world-space
        Real					m_fVerticalVelocity;            // for jumping
        Real					m_fTimer;                       // general timer to see how long animations have been playing
};

#endif // __Sinbad_h_
