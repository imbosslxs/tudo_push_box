#ifndef _H_GAMECLASS_
#define _H_GAMECLASS_

#include "cocos2d.h"
#include "state/StateMachine.h"
#include "GameLevel.h"

namespace tudo_push_box{

	class GameClass;

	//Since cocos2dx did not want the dev use a static function for scheduler
	//so I have to create a class extend cocos2d object to use it
	//what?
	class GameClassScheduler :public cocos2d::Layer{
		GameClass *m_gc;
	public:
		virtual bool init();
		void SetGameClass(GameClass *_gc){m_gc = _gc;}
		void Update(float dt);
		CREATE_FUNC(GameClassScheduler);
	};

	class GameClass{

		cocos2d::Scene* m_game_main_scene;
		cocos2d::Layer* m_game_main_layer;
		GameClassScheduler* m_game_scheduler;

		StateMachine<GameClass> *m_state_machine;

		int now_level,max_level;

		GameLevel m_game_level;



	public:
		bool up_key_down,down_key_down,left_key_down,right_key_down;
		
		GameClass():m_game_main_scene(0),
					m_state_machine(0){}
		~GameClass(){}

		static GameClass *Instance();

		void Init();

		void Update(float);

		int GetNowLevel(){return now_level;}
		void SetNowLevel(int _n){now_level = _n;}
		int GetMaxLevel(){return max_level;}
		void SetMaxLevel(int _n){max_level = _n;}

		cocos2d::Scene* GetGameMainScene(){return m_game_main_scene;}
		cocos2d::Layer* GetGameMainLayer(){return m_game_main_layer;}

		GameLevel *GetGameLevel(){return &m_game_level;}

		cocos2d::Size GetVisibleSize(){return cocos2d::Director::getInstance()->getVisibleSize();}
		cocos2d::Vec2 GetVisibleOrigin(){return cocos2d::Director::getInstance()->getVisibleOrigin();}

		StateMachine<GameClass> *GetStateMachine(){return m_state_machine;}
	};
}


#endif