#ifndef _H_GAMELEVEL_
#define _H_GAMELEVEL_

#include <vector>
#include <string>
#include "cocos2d.h"
#include "json_object_type/json_object_type.h"

namespace tudo_push_box {
class GameLevelObject {
public:
	int type;
	cocos2d::Node *m_node;
	int x, y;
	void SetXY(int _x, int _y) {x = _x, y = _y;}
	int GetX() {return x;}
	int GetY() {return y;}
	void SetPos(cocos2d::Vec2 p) {if (m_node) m_node->setPosition(p);}
	cocos2d::Vec2 GetPos() {if (m_node)return m_node->getPosition(); else return cocos2d::Vec2(0, 0);}
};
class GameLevelObject_wall : public GameLevelObject {
public:
	GameLevelObject_wall(int _x, int _y) {
		type = 0;
		m_node = 0;
		x  = _x;
		y = _y;
	}
};
class GameLevelObject_box : public GameLevelObject {
public:
	GameLevelObject_box(int _x, int _y) {
		type = 1;
		m_node = 0;
		x  = _x;
		y = _y;
	}
};
class GameLevelObject_target : public GameLevelObject {
public:
	GameLevelObject_target(int _x, int _y) {
		type = 2;
		m_node = 0;
		x  = _x;
		y = _y;
	}
};
class GameLevelObject_player : public GameLevelObject {
public:
	//0 = up,1 = down,2 = left,3 = right
	int direction;
	GameLevelObject_player(int _x, int _y) {
		type = 3;
		m_node = 0;
		x  = _x;
		y = _y;
		direction = 0;
	}
};


class GameLevel {
public:
	cocos2d::Node *m_node;

	int level;
	int time;
	int size_w, size_h;

	cocos2d::Size unit_size;

	std::vector<GameLevelObject*>m_objects;
	int player_id;
	int player_move_speed;
	int player_move_speed_ctime;

	void SetObjectPos(GameLevelObject* o, int x, int y) {
		o->m_node->setAnchorPoint(cocos2d::Vec2(0, 0));
		o->SetXY(x, y);
		o->SetPos(RealPos(x, y));
	}

	cocos2d::Vec2 RealPos(int _x, int _y);

	cocos2d::Node *InitLevelWithN(int);

	//util
	std::string FormatTime(int min, int sec, int msec);
	bool MoveObject(GameLevelObject* o, int d, int depth = 0);
	bool IsWin();
	bool IsLose();
//util


	void Release();

	GameLevel() {}
	~GameLevel() {}
};

}



#endif