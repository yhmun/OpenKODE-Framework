#ifndef _GAME_OVER_SCENE_H_
#define _GAME_OVER_SCENE_H_

 class GameOverLayer : public CCLayerColor
{
	public :

		 CREATE_FUNC ( GameOverLayer );

	protected :

		 GameOverLayer ( );
		 virtual ~GameOverLayer();

	protected :

		 KDbool			init();
		 KDvoid			gameOverDone();
		 CC_SYNTHESIZE_READONLY ( CCLabelTTF*, m_pLabel, Label );
};

class GameOverScene : public CCScene
{
	public :

		CREATE_FUNC ( GameOverScene );

	protected :
				 GameOverScene ( KDvoid );
		virtual ~GameOverScene ( KDvoid );

	protected :

		virtual KDbool		init ( KDvoid );
		CC_SYNTHESIZE_READONLY ( GameOverLayer*, m_pLayer, Layer );
};

#endif // _GAME_OVER_SCENE_H_