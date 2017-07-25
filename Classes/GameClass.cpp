#include "GameClass.h"
#include "ui/CocosGUI.h"
#include "state/GameClassStates.h"
#include "GameResource.h"
#include "GameHelper.h"

#include <string>
#include <sstream>

using namespace cocos2d;
using namespace tudo_push_box;
using namespace cocos2d::ui;
using namespace std;



GameClass *GameClass::Instance(){
    static GameClass gc;
    return &gc;
}

void GameClass::Init(){
	m_game_main_scene = Scene::create();
    m_game_main_layer = Layer::create();
    m_game_main_scene->addChild(m_game_main_layer);

    auto visibleSize = this->GetVisibleSize();
    auto origin = this->GetVisibleOrigin();

    //cfg
    now_level = GameResource::Instance()->GetGameDataNowLevel();
    max_level = GameResource::Instance()->GetLevelNums()-1;
    if(now_level > max_level){
        now_level = 0;
        GameResource::Instance()->SaveGameData(now_level);
    }

    auto W = visibleSize.width;
    auto H = visibleSize.height;
    auto ox = origin.x;
    auto oy = origin.y;
#define CENTER_POS (Vec2(W/2+ox,H/2+oy))

    //add a white back ground
    auto s = Sprite::create("ui/white_bg.png");
    s->setPosition(CENTER_POS);
    m_game_main_layer->addChild(s,0,"white_bg");

    //add a shadow
    s = Sprite::create("ui/shadow.png");
    s->setPosition(CENTER_POS);
    m_game_main_layer->addChild(s,2,"shadow");

	
    //create main menu layer
    auto main_menu_layer = Layer::create();
    m_game_main_layer->addChild(main_menu_layer,0,"main_menu_layer");
    main_menu_layer->setVisible(false);

    //add a title
    s = Sprite::create("ui/title.png");
    s->setPosition(CENTER_POS);
    main_menu_layer->addChild(s);

    //add a start button
    auto start_button = Button::create("ui/main_menu/start_button_0.png",
                                        "ui/main_menu/start_button_1.png");
    start_button->setPosition(CENTER_POS);
    start_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::StartMenuState::Instance());
        }
    });
    main_menu_layer->addChild(start_button,0,"start_button");

    //add a about button
    auto about_button = Button::create("ui/main_menu/about_button_0.png",
                                        "ui/main_menu/about_button_1.png");
    about_button->setPosition(CENTER_POS);
    about_button->setPositionY(start_button->getPositionY()-start_button->getContentSize().height/2-50.f);
    about_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            //msg_box(string("a:")+convert_int_to_string((int(GameClass::Instance()->GetStateMachine()))));

            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::AboutMenuState::Instance());
        }
    });
    main_menu_layer->addChild(about_button,0,"about_button");

    //add a quit button
    auto quit_button = Button::create("ui/main_menu/quit_button_0.png",
                                        "ui/main_menu/quit_button_1.png");
    quit_button->setPosition(CENTER_POS);
    quit_button->setPositionY(about_button->getPositionY()-about_button->getContentSize().height/2-50.f);
    quit_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            Director::getInstance()->end();
        }
    });
    main_menu_layer->addChild(quit_button,0,"quit_button");

    //add a bgm on button
    auto bgm_on_button = Button::create("ui/main_menu/bgm_on_button_0.png",
                                        "ui/main_menu/bgm_on_button_1.png");
    bgm_on_button->setPosition(Vec2((W-ox)-bgm_on_button->getContentSize().width-10,bgm_on_button->getContentSize().height+50));
    bgm_on_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){

    });
    main_menu_layer->addChild(bgm_on_button,0,"bgm_on_button");
    //add a bgm off button
    auto bgm_off_button = Button::create("ui/main_menu/bgm_off_button_0.png",
                                        "ui/main_menu/bgm_off_button_1.png");
    bgm_off_button->setPosition(Vec2((W-ox)-bgm_on_button->getContentSize().width-10,bgm_on_button->getContentSize().height+50));
    bgm_off_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){

    });
    bgm_off_button->setVisible(false);
    main_menu_layer->addChild(bgm_off_button,0,"bgm_off_button");

    //add a sound on button
    auto sound_on_button = Button::create("ui/main_menu/sound_on_button_0.png",
                                        "ui/main_menu/sound_on_button_1.png");
    sound_on_button->setPosition(Vec2(bgm_on_button->getPositionX()-sound_on_button->getContentSize().width-10,sound_on_button->getContentSize().height+50));
    sound_on_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){

    });
    main_menu_layer->addChild(sound_on_button,0,"sound_on_button");
    //add a sound off button
    auto sound_off_button = Button::create("ui/main_menu/sound_off_button_0.png",
                                        "ui/main_menu/sound_off_button_1.png");
    sound_off_button->setPosition(Vec2(bgm_on_button->getPositionX()-sound_off_button->getContentSize().width-10,sound_off_button->getContentSize().height+50));
    sound_off_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){

    });
    main_menu_layer->addChild(sound_off_button,0,"sound_off_button");

    //create about_menu_layer
    auto about_menu_layer = Layer::create();
    m_game_main_layer->addChild(about_menu_layer,0,"about_menu_layer");
    about_menu_layer->setVisible(false);

    float about_menu_layer_fix_y = -50.f;

    //add a msg box
    s = Sprite::create("ui/about_menu/about_box.png");
    s->setPosition(CENTER_POS);
    about_menu_layer->addChild(s,0,"about_box");

    //add a back button
    auto back_button = Button::create("ui/back_button_0.png",
                                        "ui/back_button_1.png");
    back_button->setPosition(Vec2((W-ox)-back_button->getContentSize().width-50,H * 1.6 / 5.0+about_menu_layer_fix_y));
    back_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::MainMenuState::Instance());
        }
    });
    about_menu_layer->addChild(back_button,0,"back_button");

    //create start menu
    auto start_menu_layer = Layer::create();
    m_game_main_layer->addChild(start_menu_layer,0,"start_menu_layer");
    start_menu_layer->setVisible(false);

    //add a title
    s = Sprite::create("ui/title.png");
    s->setPosition(CENTER_POS);
    start_menu_layer->addChild(s);

    float start_menu_layer_fix_y = -200.f;

    //add a level label
    s = Sprite::create("ui/start_menu/level_label.png");
    s->setPosition(Vec2(s->getContentSize().width/2.f+(-ox),(H-oy)*(3.5f/5.f)+start_menu_layer_fix_y));
    start_menu_layer->addChild(s);

    //add a bottom line of level selector
    s = Sprite::create("ui/start_menu/level_selector/d_bar.png");
    s->setPosition(Vec2(410.f+(-ox),(H-oy)*(3.2f/5.f)+start_menu_layer_fix_y));
    start_menu_layer->addChild(s);

    //add left button of level selector
    auto ls_left_button = Button::create("ui/start_menu/level_selector/l_button_0.png",
                                        "ui/start_menu/level_selector/l_button_1.png");
    ls_left_button->setPosition(Vec2(410.f+(-ox)-ls_left_button->getContentSize().width*2.f,(H-oy)*(3.2f/5.f)+ls_left_button->getContentSize().height+start_menu_layer_fix_y));
    ls_left_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            //msg_box("left!");
            auto ls_l = (Label*)GameClass::Instance()->GetGameMainLayer()->getChildByName("start_menu_layer")->getChildByName("ls_number_label");
            int n = GameClass::Instance()->GetNowLevel();
            n -= 1;
            if(n < 0){
                n = 0;
            }
            GameClass::Instance()->SetNowLevel(n);
            ls_l->setString(convert_int_to_string(n));
        }
    });
    start_menu_layer->addChild(ls_left_button,0,"ls_left_button");
    //add right button of level selector
    auto ls_right_button = Button::create("ui/start_menu/level_selector/r_button_0.png",
                                        "ui/start_menu/level_selector/r_button_1.png");
    ls_right_button->setPosition(Vec2(410.f+(-ox)+ls_right_button->getContentSize().width*2.f,(H-oy)*(3.2f/5.f)+ls_right_button->getContentSize().height+start_menu_layer_fix_y));
    ls_right_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            //msg_box("right!");
            auto ls_l = (Label*)GameClass::Instance()->GetGameMainLayer()->getChildByName("start_menu_layer")->getChildByName("ls_number_label");
            int n = GameClass::Instance()->GetNowLevel();
            n += 1;
            if(n > GameResource::Instance()->GetGameDataNowLevel()){
                n = GameResource::Instance()->GetGameDataNowLevel();
            }
            GameClass::Instance()->SetNowLevel(n);
            ls_l->setString(convert_int_to_string(n));
        }
    });
    start_menu_layer->addChild(ls_right_button,0,"ls_right_button");

    //add a number label of level selector
    auto ls_number_label = Label::createWithCharMap("ui/fonts/ls_number_font.png",70,70,'/');
    ls_number_label->setString(convert_int_to_string(now_level));
    ls_number_label->setAdditionalKerning(-30.f);
    ls_number_label->setPosition(Vec2(395.f+(-ox),(H-oy)*(3.2f/5.f)+50.f+start_menu_layer_fix_y));
    start_menu_layer->addChild(ls_number_label,0,"ls_number_label");

    //add a time limit label
    s = Sprite::create("ui/start_menu/time_limit_label.png");
    s->setPosition(Vec2(s->getContentSize().width/2.f+(-ox),(H-oy)*(3.5f/5.f)-120.f+start_menu_layer_fix_y));
    start_menu_layer->addChild(s);

    //add a on button
    auto time_limit_on_button = Button::create("ui/start_menu/on_button_0.png",
                                        "ui/start_menu/on_button_1.png");
    time_limit_on_button->setPosition(Vec2(410.f+(-ox),(H-oy)*(3.2f/5.f)-100.f+start_menu_layer_fix_y));
    time_limit_on_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            GameClass::Instance()->GetGameMainLayer()->getChildByName("start_menu_layer")->getChildByName("time_limit_on_button")->setVisible(false);
            GameClass::Instance()->GetGameMainLayer()->getChildByName("start_menu_layer")->getChildByName("time_limit_off_button")->setVisible(true);
        }
    });
    start_menu_layer->addChild(time_limit_on_button,0,"time_limit_on_button");
    //add a off button
    auto time_limit_off_button = Button::create("ui/start_menu/off_button_0.png",
                                        "ui/start_menu/off_button_1.png");
    time_limit_off_button->setPosition(Vec2(410.f+(-ox),(H-oy)*(3.2f/5.f)-100.f+start_menu_layer_fix_y));
    time_limit_off_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            GameClass::Instance()->GetGameMainLayer()->getChildByName("start_menu_layer")->getChildByName("time_limit_on_button")->setVisible(true);
            GameClass::Instance()->GetGameMainLayer()->getChildByName("start_menu_layer")->getChildByName("time_limit_off_button")->setVisible(false);
        }
    });
    start_menu_layer->addChild(time_limit_off_button,0,"time_limit_off_button");
    time_limit_off_button->setVisible(false);

    //add a go button
    auto go_button = Button::create("ui/start_menu/go_button_0.png",
                                        "ui/start_menu/go_button_1.png");
    go_button->setPosition(Vec2((W-go_button->getContentSize().width)/2+100.f-ox,(H-oy)*(4.f/5.f)-240.f- go_button->getContentSize().height+start_menu_layer_fix_y));
    go_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::EnterGamingState::Instance());
        }
    });
    start_menu_layer->addChild(go_button,0,"go_button");

    //add a back button
    back_button = Button::create("ui/back_button_0.png",
                                        "ui/back_button_1.png");
    back_button->setPosition(Vec2((W-back_button->getContentSize().width)/2-ox,(-oy)+back_button->getContentSize().height+200+start_menu_layer_fix_y));
    back_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::MainMenuState::Instance());
        }
    });
    start_menu_layer->addChild(back_button,0,"back_button");

    //create a gaming layer
    auto gaming_layer = Layer::create();
    m_game_main_layer->addChild(gaming_layer,0,"gaming_layer");
    gaming_layer->setVisible(false);
    gaming_layer->setPositionY(-150.f);

    //add a time remain sprite
    s = Sprite::create("ui/Gaming/time_remain_label.png");
    s->setAnchorPoint(Vec2(0,0));
    s->setPosition(Vec2(100.f-ox,(H-s->getContentSize().height)-oy+110.f));
    gaming_layer->addChild(s);

    //add a time remain label
    auto time_remain_label = Label::createWithCharMap("ui/fonts/ls_number_font.png",70,70,'/');
    time_remain_label->setString("99/99/99");
    time_remain_label->setAdditionalKerning(-30.f);
    time_remain_label->setAnchorPoint(Vec2(0,0));
    time_remain_label->setPosition(Vec2(W/2+(-ox),(H+60.f-oy)));
    gaming_layer->addChild(time_remain_label,0,"time_remain_label");

    //add a pause button
    auto pause_button = Button::create("ui/Gaming/pause_button_0.png",
                                        "ui/Gaming/pause_button_1.png");
    pause_button->setPosition(Vec2(100.f-ox,(-oy)+200.f));
    pause_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::PauseGamingState::Instance());
        }
    });
    gaming_layer->addChild(pause_button,0,"pause_button");

    //create a pause menu layer
    auto pause_menu_layer = Layer::create();
    gaming_layer->addChild(pause_menu_layer,1,"pause_menu_layer");
    pause_menu_layer->setVisible(false);

    //add a msg box
    s = Sprite::create("ui/pause_menu/msg_box.png");
    s->setPosition(CENTER_POS);
    pause_menu_layer->addChild(s);

    //add a main_menu_button
    auto pause_main_menu_button = Button::create("ui/pause_menu/main_menu_button_0.png",
                                        "ui/pause_menu/main_menu_button_1.png");
    pause_main_menu_button->setPosition(Vec2(150.f-ox,(-oy)+260.f));
    pause_main_menu_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::ExitGamingState::Instance());
        }
    });
    pause_menu_layer->addChild(pause_main_menu_button,0,"pause_main_menu_button");

    //add a continue_button
    auto pause_continu_button = Button::create("ui/pause_menu/continue_button_0.png",
                                        "ui/pause_menu/continue_button_1.png");
    pause_continu_button->setPosition(Vec2(400.f-ox,(-oy)+260.f));
    pause_continu_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::GamingState::Instance());
        }
    });
    pause_menu_layer->addChild(pause_continu_button,0,"pause_continu_button");

    //add a try_again_button
    auto pause_try_again_button = Button::create("ui/pause_menu/try_again_button_0.png",
                                        "ui/pause_menu/try_again_button_1.png");
    pause_try_again_button->setPosition(Vec2(650.f-ox,(-oy)+260.f));
    pause_try_again_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::RestartGamingState::Instance());
        }
    });
    pause_menu_layer->addChild(pause_try_again_button,0,"pause_try_again_button");

    //create win menu layer
    auto win_menu_layer = Label::create();
    gaming_layer->addChild(win_menu_layer,1,"win_menu_layer");
    win_menu_layer->setVisible(false);

    //add a msg box
    s = Sprite::create("ui/win_menu/msg_box.png");
    s->setPosition(CENTER_POS);
    win_menu_layer->addChild(s);

    //add a main_menu_button
    auto win_main_menu_button = Button::create("ui/win_menu/main_menu_button_0.png",
                                        "ui/win_menu/main_menu_button_1.png");
    win_main_menu_button->setPosition(Vec2(150.f-ox,(-oy)+260.f));
    win_main_menu_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::ExitGamingState::Instance());
        }
    });
    win_menu_layer->addChild(win_main_menu_button,0,"win_main_menu_button");

    //add a next_level_button
    auto win_next_level_button = Button::create("ui/win_menu/next_level_button_0.png",
                                        "ui/win_menu/next_level_button_1.png");
    win_next_level_button->setPosition(Vec2(400.f-ox,(-oy)+260.f));
    win_next_level_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine()){
                GameClass::Instance()->SetNowLevel(GameClass::Instance()->GetNowLevel()+1);
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::RestartGamingState::Instance());
            }
        }
    });
    win_menu_layer->addChild(win_next_level_button,0,"win_next_level_button");

    //add a try_again_button
    auto win_try_again_button = Button::create("ui/win_menu/try_again_button_0.png",
                                        "ui/win_menu/try_again_button_1.png");
    win_try_again_button->setPosition(Vec2(650.f-ox,(-oy)+260.f));
    win_try_again_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::RestartGamingState::Instance());
        }
    });
    win_menu_layer->addChild(win_try_again_button,0,"win_try_again_button");


    //create lose menu layer
    auto lose_menu_layer = Label::create();
    gaming_layer->addChild(lose_menu_layer,1,"lose_menu_layer");
    lose_menu_layer->setVisible(false);

    //add a msg box
    s = Sprite::create("ui/lose_menu/msg_box.png");
    s->setPosition(CENTER_POS);
    lose_menu_layer->addChild(s);

    //add a main_menu_button
    auto lose_main_menu_button = Button::create("ui/lose_menu/main_menu_button_0.png",
                                        "ui/lose_menu/main_menu_button_1.png");
    lose_main_menu_button->setPosition(Vec2(150.f-ox,(-oy)+260.f));
    lose_main_menu_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::ExitGamingState::Instance());
        }
    });
    lose_menu_layer->addChild(lose_main_menu_button,0,"lose_main_menu_button");

    //add a try_again_button
    auto lose_try_again_button = Button::create("ui/lose_menu/try_again_button_0.png",
                                        "ui/lose_menu/try_again_button_1.png");
    lose_try_again_button->setPosition(Vec2(650.f-ox,(-oy)+260.f));
    lose_try_again_button->addTouchEventListener([&](Ref *pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            if(GameClass::Instance()->GetStateMachine())
                GameClass::Instance()->GetStateMachine()->ChangeState(GameClassStates::RestartGamingState::Instance());
        }
    });
    lose_menu_layer->addChild(lose_try_again_button,0,"lose_try_again_button");



    //Init state machine
    m_state_machine = new StateMachine<GameClass>(this);
    m_state_machine->ChangeState(GameClassStates::MainMenuState::Instance());
    m_state_machine->SetGlobleState(GameClassStates::GlobleState::Instance());
    //msg_box(string("b:")+convert_int_to_string((int(m_state_machine))));

    //Add a every time scheduler
    //m_game_main_scene->schedule(schedule_selector(GameClassScheduler::Update));
    m_game_scheduler = GameClassScheduler::create();
    m_game_scheduler->SetGameClass(this);
    m_game_main_scene->addChild(m_game_scheduler,0,"game_scheduler");

    //add key listener
    auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = [&](EventKeyboard::KeyCode c, Event* e) {
		/*KEY_LEFT_ARROW,
			KEY_RIGHT_ARROW,
			KEY_UP_ARROW,
			KEY_DOWN_ARROW,*/
		if(c == EventKeyboard::KeyCode::KEY_UP_ARROW || c == EventKeyboard::KeyCode::KEY_W)up_key_down=true;
		else if(c == EventKeyboard::KeyCode::KEY_DOWN_ARROW || c == EventKeyboard::KeyCode::KEY_S)down_key_down=true;
		else if(c == EventKeyboard::KeyCode::KEY_LEFT_ARROW || c == EventKeyboard::KeyCode::KEY_A)left_key_down=true;
		else if(c == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || c == EventKeyboard::KeyCode::KEY_D)right_key_down=true;
	};
    key_listener->onKeyReleased = [&](EventKeyboard::KeyCode c, Event* e) {
        if(c == EventKeyboard::KeyCode::KEY_UP_ARROW || c == EventKeyboard::KeyCode::KEY_W)up_key_down=false;
        else if(c == EventKeyboard::KeyCode::KEY_DOWN_ARROW || c == EventKeyboard::KeyCode::KEY_S)down_key_down=false;
        else if(c == EventKeyboard::KeyCode::KEY_LEFT_ARROW || c == EventKeyboard::KeyCode::KEY_A)left_key_down=false;
        else if(c == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || c == EventKeyboard::KeyCode::KEY_D)right_key_down=false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, m_game_main_scene);


    //add touch listener
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = [](Touch* t, Event* e){return true;};
    touch_listener->onTouchMoved = [&](Touch* t, Event* e){
        auto start_touch_pos = t->getStartLocation();
        auto now_touch_pos = t->getLocation();
        auto offset = GameResource::Instance()->GetTouchOffset();
        if(now_touch_pos.y - start_touch_pos.y > offset) up_key_down = true;
        else up_key_down = false;
        if(now_touch_pos.y - start_touch_pos.y < -offset) down_key_down = true;
        else down_key_down = false;
        if(now_touch_pos.x - start_touch_pos.x < -offset) left_key_down = true;
        else left_key_down = false;
        if(now_touch_pos.x - start_touch_pos.x > offset) right_key_down = true;
        else right_key_down = false;
        //msg_box(convert_int_to_string(now_touch_pos.x - start_touch_pos.x) + string(" | ")+convert_int_to_string(now_touch_pos.y - start_touch_pos.y));
    };
    touch_listener->onTouchEnded = [&](Touch* t, Event* e){
        up_key_down = false;
        down_key_down = false;
        left_key_down = false;
        right_key_down = false;
        
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener,m_game_main_scene);
 
}

bool GameClassScheduler::init(){
    schedule(schedule_selector(GameClassScheduler::Update));
    m_gc = 0;
    return true;
}
void GameClassScheduler::Update(float dt){
    if(m_gc)
     m_gc->Update(dt);
}

void GameClass::Update(float dt){
	m_state_machine->Update();
}