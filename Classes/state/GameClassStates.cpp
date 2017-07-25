#include "GameClassStates.h"
#include "../GameClass.h"
#include "../GameResource.h"
#include "GameHelper.h"
#include <string>



using namespace cocos2d;
using namespace tudo_push_box;
using namespace GameClassStates;
using namespace std;


//====================GlobleState
GlobleState *GlobleState::Instance() {
	static GlobleState s;
	return &s;
}
void GlobleState::Enter(GameClass* gc) {

}
void GlobleState::Excute(GameClass* gc) {

}
void GlobleState::Exit(GameClass* gc) {

}

//====================MainMenuState
MainMenuState *MainMenuState::Instance() {
	static MainMenuState s;
	return &s;
}
void MainMenuState::Enter(GameClass* gc) {
	gc->GetGameMainLayer()->getChildByName("main_menu_layer")->setVisible(true);
}
void MainMenuState::Excute(GameClass* gc) {

}
void MainMenuState::Exit(GameClass* gc) {
	gc->GetGameMainLayer()->getChildByName("main_menu_layer")->setVisible(false);
}

//====================AboutMenuState
AboutMenuState *AboutMenuState::Instance() {
	static AboutMenuState s;
	return &s;
}
void AboutMenuState::Enter(GameClass* gc) {
	gc->GetGameMainLayer()->getChildByName("about_menu_layer")->setVisible(true);
	gc->GetGameMainLayer()->getChildByName("main_menu_layer")->setVisible(true);
}
void AboutMenuState::Excute(GameClass* gc) {

}
void AboutMenuState::Exit(GameClass* gc) {
	gc->GetGameMainLayer()->getChildByName("about_menu_layer")->setVisible(false);
}

//====================StartMenuState
StartMenuState *StartMenuState::Instance() {
	static StartMenuState s;
	return &s;
}
void StartMenuState::Enter(GameClass* gc) {
	gc->SetNowLevel(GameResource::Instance()->GetGameDataNowLevel());
	((Label*)gc->GetGameMainLayer()->getChildByName("start_menu_layer")->getChildByName("ls_number_label"))->setString(convert_int_to_string(gc->GetNowLevel()));
	gc->GetGameMainLayer()->getChildByName("start_menu_layer")->setVisible(true);
}
void StartMenuState::Excute(GameClass* gc) {

}
void StartMenuState::Exit(GameClass* gc) {
	gc->GetGameMainLayer()->getChildByName("start_menu_layer")->setVisible(false);
}


//====================EnterGamingState
EnterGamingState *EnterGamingState::Instance() {
	static EnterGamingState s;
	return &s;
}
void EnterGamingState::Enter(GameClass* gc) {
	auto gaming_layer = gc->GetGameMainLayer()->getChildByName("gaming_layer");
	gaming_layer->setVisible(true);
	auto start_menu_layer = gc->GetGameMainLayer()->getChildByName("start_menu_layer");

	auto visibleSize = gc->GetVisibleSize();
	auto origin = gc->GetVisibleOrigin();
	auto W = visibleSize.width;
	auto H = visibleSize.height;
	auto ox = origin.x;
	auto oy = origin.y;

	//Load level
	auto game_level_main_layer = gc->GetGameLevel()->InitLevelWithN(gc->GetNowLevel());
	//msg_box(convert_int_to_string(gc->GetNowLevel()));

	//msg_box(convert_int_to_string(gc->GetGameLevel()->time));
	if (start_menu_layer->getChildByName("time_limit_off_button")->isVisible())
		gc->GetGameLevel()->time = -1;

	gaming_layer->addChild(game_level_main_layer, 0, "game_level_main_layer");
	game_level_main_layer->setPosition(Vec2((W - (gc->GetGameLevel()->size_w * gc->GetGameLevel()->unit_size.width)) / 2 - ox, (H - (gc->GetGameLevel()->size_h * gc->GetGameLevel()->unit_size.height))/2.f));// + 40.f - oy));
	//game_level_main_layer->setPosition(Vec2(0,0));
}
void EnterGamingState::Excute(GameClass* gc) {
	gc->GetStateMachine()->ChangeState(GamingState::Instance());
}
void EnterGamingState::Exit(GameClass* gc) {

}

//====================GamingState
GamingState *GamingState::Instance() {
	static GamingState s;
	return &s;
}
void GamingState::Enter(GameClass* gc) {

}
void GamingState::Excute(GameClass* gc) {
	static DWORD last_time = 0;
	auto gl = gc->GetGameLevel();
	auto l = (Label*) gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("time_remain_label");

	//check if win
	if (gl->IsWin()) {
		//msg_box("You WIn!");
		gc->GetStateMachine()->ChangeState(GamingWinState::Instance());
	}
	//check if lose
	if (gl->IsLose()) {
		//msg_box("Time Up!,you lose!");
		gc->GetStateMachine()->ChangeState(GamingLoseState::Instance());
	}

	//time logic
	if (gl->time == -1) { //No limit time
		l->setString("99/99/99");
	}
	//else if(gl->time == 0){//time up ,game over
	//}
	else if (gl->time > 0) { //update time
		if (GetTickCount() - last_time > 100) {
			last_time = GetTickCount();

			gl->time -= 100;
			if (gl->time < 0) gl->time = 0;

			int msec = gl->time % 1000;
			int sec = gl->time / 1000;
			int min = sec / 60;
			sec = sec % 60;

			l->setString(gl->FormatTime(min, sec, (msec / 1000.0) * 60));
		}
	}

	//level objects logic
	auto player = gl->m_objects[gl->player_id];
	if(GetTickCount() - gl->player_move_speed_ctime > gl->player_move_speed){
		//game_level->player_move_speed_ctime = GetTickCount();

		bool is_moved = false;
		if(gc->up_key_down)is_moved=gl->MoveObject(player,1);
		if(gc->down_key_down)is_moved=gl->MoveObject(player,0);
		if(gc->left_key_down)is_moved=gl->MoveObject(player,2);
		if(gc->right_key_down)is_moved=gl->MoveObject(player,3);
		if(is_moved)gl->player_move_speed_ctime = GetTickCount();
	}

	//sort objects by y
	for(auto o:gl->m_objects){
		o->m_node->setLocalZOrder(10000-o->m_node->getPositionY());
		if(o->type == 2)
			o->m_node->setLocalZOrder(0);
	}

	//when the player is behind an object
	for(auto o:gl->m_objects){
		if(o != player){
			if(o->m_node->getPositionY() == player->m_node->getPositionY()-gl->unit_size.height\
				&& o->m_node->getPositionX() == player->m_node->getPositionX()){
				o->m_node->setOpacity(100);
			}else
				o->m_node->setOpacity(255);
		}
	}

}
void GamingState::Exit(GameClass* gc) {

}

//====================GamingWinState
GamingWinState *GamingWinState::Instance(){
	static GamingWinState s;
	return &s;
}
void GamingWinState::Enter(GameClass* gc){
	
	if(gc->GetNowLevel() > GameResource::Instance()->GetGameDataNowLevel())
		GameResource::Instance()->SaveGameData(gc->GetNowLevel()<gc->GetMaxLevel() ? gc->GetNowLevel()+1 : gc->GetMaxLevel());
	if(gc->GetNowLevel()==gc->GetMaxLevel())
		gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("win_menu_layer")->getChildByName("win_next_level_button")->setVisible(false);
	else
		gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("win_menu_layer")->getChildByName("win_next_level_button")->setVisible(true);
	gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("win_menu_layer")->setVisible(true);
}
void GamingWinState::Excute(GameClass* gc){
	
}
void GamingWinState::Exit(GameClass* gc){
	gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("win_menu_layer")->setVisible(false);
}

//====================GamingLoseState
GamingLoseState *GamingLoseState::Instance(){
	static GamingLoseState s;
	return &s;
}
void GamingLoseState::Enter(GameClass* gc){
	gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("lose_menu_layer")->setVisible(true);

}
void GamingLoseState::Excute(GameClass* gc){
	
}
void GamingLoseState::Exit(GameClass* gc){
	gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("lose_menu_layer")->setVisible(false);
}

//====================ExitGamingState
ExitGamingState *ExitGamingState::Instance(){
	static ExitGamingState s;
	return &s;
}
void ExitGamingState::Enter(GameClass* gc){
	gc->GetGameLevel()->Release();
}
void ExitGamingState::Excute(GameClass* gc){
	gc->GetStateMachine()->ChangeState(MainMenuState::Instance());
}
void ExitGamingState::Exit(GameClass* gc){
	gc->GetGameMainLayer()->getChildByName("gaming_layer")->setVisible(false);
}

//====================RestartGamingState
RestartGamingState *RestartGamingState::Instance(){
	static RestartGamingState s;
	return &s;
}
void RestartGamingState::Enter(GameClass* gc){
	//gc->GetGameMainLayer()->getChildByName("gaming_layer")->removeChild(gc->GetGameLevel()->m_node);
	gc->GetGameLevel()->Release();
}
void RestartGamingState::Excute(GameClass* gc){
	gc->GetStateMachine()->ChangeState(EnterGamingState::Instance());
}
void RestartGamingState::Exit(GameClass* gc){
	
}

//====================PauseGamingState
PauseGamingState *PauseGamingState::Instance(){
	static PauseGamingState s;
	return &s;
}
void PauseGamingState::Enter(GameClass* gc){
	gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("pause_menu_layer")->setVisible(true);
}
void PauseGamingState::Excute(GameClass* gc){
	
}
void PauseGamingState::Exit(GameClass* gc){
	gc->GetGameMainLayer()->getChildByName("gaming_layer")->getChildByName("pause_menu_layer")->setVisible(false);
}