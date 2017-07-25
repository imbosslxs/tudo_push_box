
#ifndef _H_GAMERESOURCE
#define _H_GAMERESOURCE

#include "cocos2d.h"
#include "json_object_type/json_object_type.h"


namespace tudo_push_box{

	class GameResource{
		json_object_type game_cfg;

		GameResource(){}
	public:
		static GameResource *Instance();

		void LoadGameCfgFile(std::string);

		void SaveGameData(int now_level);

		int GetGameDataNowLevel();
		std::string GetLevelDir(){return game_cfg["levels_dir"].get_string();}
		std::string MakeLeveFilePath(int l);

		json_object_type GetGameCfg(){return game_cfg;}
		std::string GetGameVersion(){return game_cfg["version"].get_string();}

		int GetLevelNums();
		json_object_type GetLevelData(int);
		
		double GetTouchOffset(){return game_cfg["touch_offset"].get_double();}
		
		~GameResource(){}
	};

}


#endif