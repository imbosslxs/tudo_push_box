#include "GameResource.h"
#include "GameHelper.h"

using namespace cocos2d;
using namespace tudo_push_box;
using namespace std;


GameResource *GameResource::Instance(){
	static GameResource gr;
	return &gr;
}

void GameResource::LoadGameCfgFile(string path){
	string text = FileUtils::getInstance()->getStringFromFile(path);
	//msg_box(text,"a");
	game_cfg = json_object_type::phraze(text);
	//msg_box("ok");
	//msg_box(json_object_type::json_object_type_to_string(game_cfg,0),"b");
	//msg_box(convert_int_to_string(GetGameDataNowLevel()));
	//SaveGameData(5);
	//msg_box(convert_int_to_string(GetLevelNums()));
}

void GameResource::SaveGameData(int now_level){
	auto save_data = json_object_type::phraze("{\"now_level\":0}");
	save_data["now_level"].put_int(now_level);
	string data_file = FileUtils::getInstance()->getWritablePath() + game_cfg["save_file"].get_string();
	//msg_box(data_file);
	fstream f_out(data_file,ios::out | ios::trunc);
	f_out<<json_object_type::json_object_type_to_string(save_data);
	f_out.close();
}

int GameResource::GetGameDataNowLevel(){
	string data_file = FileUtils::getInstance()->getWritablePath() + game_cfg["save_file"].get_string();
	if(FileUtils::getInstance()->isFileExist(data_file)){
		string text = FileUtils::getInstance()->getStringFromFile(data_file);
		auto save_data = json_object_type::phraze(text);
		return save_data["now_level"].get_int();
	}else{
		return 0;
	}
}

int GameResource::GetLevelNums(){
	int i=0;
	for(;FileUtils::getInstance()->isFileExist(MakeLeveFilePath(i));++i);
	return i;
}

json_object_type GameResource::GetLevelData(int n){
	string text = FileUtils::getInstance()->getStringFromFile(MakeLeveFilePath(n));
	return json_object_type::phraze(text);
}

string GameResource::MakeLeveFilePath(int l){
	return GetLevelDir()+std::string("/level_")+convert_int_to_string(l)+string(".json");
}
