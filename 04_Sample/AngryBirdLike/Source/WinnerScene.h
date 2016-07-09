#ifndef _WINNER_SCENE_H_
 #define _WINNER_SCENE_H_
 

 class WinnerLayer : public CCLayerColor
 {
	public:

		CREATE_FUNC (WinnerLayer);

		CC_SYNTHESIZE_READONLY ( CCLabelTTF*, m_pLabel, Label );


	protected :

				 WinnerLayer ( );
		virtual ~WinnerLayer ( );


	public:

		KDbool			init ( KDvoid );
		
		KDvoid			ExplosionAnimation ( CCPoint position, const KDchar* szImgName );

		KDvoid			WinnerDone ( KDvoid );

    
};

class WinnerScene : public CCScene
{
	public:

		CREATE_FUNC ( WinnerScene );
		CC_SYNTHESIZE_READONLY ( WinnerLayer*, m_pLayer, Layer );

						 WinnerScene ( );
		virtual			~WinnerScene ( );
		
		KDbool			init ( KDvoid );
		

		
};

#endif // _GAME_OVER_SCENE_H_