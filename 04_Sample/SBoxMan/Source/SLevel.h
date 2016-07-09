/* --------------------------------------------------------------------------
 *
 *      File            SLevel.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-07
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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

#ifndef __SLevel_h__
#define __SLevel_h__

class SLevel : public CCObject
{
	public :
				 SLevel ( KDvoid );
		virtual ~SLevel ( KDvoid );
 
		static   SLevel*		create ( CCDictionary* pDictionary );

	public :

		virtual KDbool			initWithDictionary ( CCDictionary* pDictionary );

		virtual KDvoid			resetMapElements ( KDvoid );

		virtual CCDictionary*	toDictionary ( KDvoid );

		virtual const KDchar*	getBackgroundMusic ( KDvoid );

		virtual const KDchar*	getPushEffect ( KDvoid );

		virtual const KDchar*	getWinEffect ( KDvoid );

		virtual const KDchar*	getBackgroundThumb ( KDvoid );

		virtual KDint			getLevel ( KDvoid );

		virtual CCArray*		getMapElements ( KDvoid );

	protected :

		std::string				m_sBackgroundMusic;
		std::string				m_sPushEffect;
		std::string				m_sWinEffect;
		KDint					m_nLevel;
		std::string				m_sBackgroundThumb;
		CCArray*				m_pMapElements;
		std::string				m_sMapName;
		CCString*				m_pMapString;
};

/*

@interface SLevel : NSObject

@property (nonatomic, readonly) NSString * backgroundMusic;
@property (nonatomic, readonly) NSString * pushEffect;
@property (nonatomic, readonly) NSString * winEffect;
@property (nonatomic, readonly) NSInteger  level;
@property (nonatomic, readonly) NSString * backgroundThumb;
@property (nonatomic, readonly) NSArray * mapElements;

+ (id) levelWithDictionary:(NSDictionary *) dictionary;

- (id) initWithDictionary:(NSDictionary *) dictionary;

- (void) resetMapElements;

- (NSDictionary *) toDictionary;

@end
*/
#endif	// __SLevel_h__