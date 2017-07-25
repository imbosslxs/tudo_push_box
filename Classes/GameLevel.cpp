#include "GameLevel.h"
#include "GameResource.h"
#include "GameHelper.h"

using namespace cocos2d;
using namespace std;
using namespace tudo_push_box;


Node *GameLevel::InitLevelWithN(int _n) {
	m_objects.clear();
	m_objects.reserve(300);

	json_object_type jo = GameResource::Instance()->GetLevelData(_n);
	level = _n;
	time = jo["time"].get_int();
	size_w = jo["size"][0].get_int();
	size_h = jo["size"][1].get_int();

	json_object_type level_cfg = GameResource::Instance()->GetGameCfg()["level_cfg"];

	unit_size = Size(level_cfg["unit_size"][0].get_double(), level_cfg["unit_size"][1].get_double());

	player_move_speed = level_cfg["player_speed"].get_int();
	player_move_speed_ctime = 0;

	auto edge_png = level_cfg["edge_png"].get_string();
	auto wall_png = level_cfg["wall_png"].get_string();
	auto box_png = level_cfg["box_png"].get_string();
	auto player_png = level_cfg["player_png"].get_string();
	auto target_png = level_cfg["target_png"].get_string();

	m_node = Node::create();
	m_node->setAnchorPoint(Vec2(0, 0));

	//add a white background
	auto white_background = DrawNode::create();
	Vec2 white_background_rect[] = {Vec2(0,0),
									Vec2(size_w*unit_size.width,0),
									Vec2(size_w*unit_size.width,size_h*unit_size.height),
									Vec2(0,size_h*unit_size.height)};
	white_background->drawPolygon(white_background_rect,4,Color4F(1,1,1,1),1,Color4F(1,1,1,1));
	m_node->addChild(white_background,0);

	//create map
	//to create an object
	//1. new an object
	//2. create sprite
	//3. add to m_node
	//5. set pos
	//6. set real pos
	//7. push_back

	int n = jo["walls"].children.size();
	for (int i = 0; i < n; ++i) {
		int x = jo["walls"][i][0].get_int();
		int y = size_h-1-jo["walls"][i][1].get_int();

		GameLevelObject* o = new GameLevelObject_wall(x, y);
		o->m_node = Sprite::create(wall_png);
		m_node->addChild(o->m_node);
		SetObjectPos(o, x, y);

		m_objects.push_back(o);
	}

	n = jo["targets"].children.size();
	for (int i = 0; i < n; ++i) {
		int x = jo["targets"][i][0].get_int();
		int y = size_h-1-jo["targets"][i][1].get_int();
		auto o = new GameLevelObject_target(x, y);
		o->m_node = Sprite::create(target_png);
		m_node->addChild(o->m_node);
		SetObjectPos(o, x, y);

		m_objects.push_back(o);
	}

	n = jo["boxes"].children.size();
	for (int i = 0; i < n; ++i) {
		int x = jo["boxes"][i][0].get_int();
		int y = size_h-1-jo["boxes"][i][1].get_int();
		auto o = new GameLevelObject_box(x, y);
		o->m_node = Sprite::create(box_png);
		m_node->addChild(o->m_node);
		SetObjectPos(o, x, y);

		m_objects.push_back(o);
	}


	int x = jo["spawn_point"][0].get_int();
	int y = size_h-1-jo["spawn_point"][1].get_int();
	auto o = new GameLevelObject_player(x, y);
	o->m_node = Sprite::create(player_png);
	m_node->addChild(o->m_node);
	SetObjectPos(o, x, y);
	m_objects.push_back(o);

	player_id = m_objects.size() - 1;

	//debug
	//msg_box(convert_int_to_string(m_node->getChildrenCount()));

	//add edges
	//set clipping node
	//1.create clipping node,draw node
	//2.set rect_vs,
	//3.draw rect
	//4.set stencil
	//5.add s
	//6.set positon

	//top
	auto s = Sprite::create(edge_png);
	s->setAnchorPoint(Vec2(0, 0));
	auto c_s = ClippingNode::create();
	auto d_s = DrawNode::create();
	Vec2 rect_vs[] = {Vec2(0,0),
						Vec2((size_w * unit_size.width) + 2.f * s->getContentSize().height,0),
						Vec2((size_w * unit_size.width) + 2.f * s->getContentSize().height,s->getContentSize().height),
						Vec2(0,s->getContentSize().height)};
	auto rect_color = Color4F(0,1,0,1);
	d_s->drawPolygon(rect_vs,4,rect_color,1,rect_color);
	c_s->setStencil(d_s);
	s->setPositionX(-s->getContentSize().height);
	c_s->addChild(s);
	c_s->setPosition(Vec2(-s->getContentSize().height, (size_h * unit_size.height)));
	m_node->addChild(c_s);

	//bottom
	s = Sprite::create(edge_png);
	s->setAnchorPoint(Vec2(0, 0));
	c_s = ClippingNode::create();
	d_s = DrawNode::create();
	d_s->drawPolygon(rect_vs,4,rect_color,1,rect_color);
	c_s->setStencil(d_s);
	s->setPositionX(-s->getContentSize().height);
	c_s->addChild(s);
	c_s->setPosition(Vec2(-s->getContentSize().height, -s->getContentSize().height));
	m_node->addChild(c_s);

	//left
	s = Sprite::create(edge_png);
	s->setAnchorPoint(Vec2(0, 0));
	c_s = ClippingNode::create();
	d_s = DrawNode::create();
	rect_vs[1].x = (size_h*unit_size.height)+2.f*s->getContentSize().height;
	rect_vs[2].x = (size_h*unit_size.height)+2.f*s->getContentSize().height;
	d_s->drawPolygon(rect_vs,4,rect_color,1,rect_color);
	c_s->setStencil(d_s);
	s->setPositionX(-s->getContentSize().height);
	c_s->addChild(s);
	c_s->setRotation(-90.f);
	c_s->setPosition(Vec2(0, -s->getContentSize().height));
	m_node->addChild(c_s);

	//right
	s = Sprite::create(edge_png);
	s->setAnchorPoint(Vec2(0, 0));
	c_s = ClippingNode::create();
	d_s = DrawNode::create();
	d_s->drawPolygon(rect_vs,4,rect_color,1,rect_color);
	c_s->setStencil(d_s);
	s->setPositionX(-s->getContentSize().height);
	c_s->addChild(s);
	c_s->setRotation(-90.f);
	c_s->setPosition(Vec2((size_w*unit_size.width)+s->getContentSize().height, -s->getContentSize().height));
	m_node->addChild(c_s);


	return m_node;
}


Vec2 GameLevel::RealPos(int x, int y) {
	return Vec2(x * unit_size.width, y * unit_size.height);
}

void GameLevel::Release(){
	auto p = m_node->getParent();
	p->removeChild(m_node);
	m_node = 0;
	m_objects.clear();
}

//util
string GameLevel::FormatTime(int min, int sec, int msec) {
	string ft;
	string min_s = convert_int_to_string(min);
	if (min_s.size() == 1)
		ft += "0";
	ft += min_s;
	ft += "/";
	string sec_s = convert_int_to_string(sec);
	if (sec_s.size() == 1)
		ft += "0";
	ft += sec_s;
	ft += "/";
	string msec_s = convert_int_to_string(msec);
	if (msec_s.size() == 1)
		ft += "0";
	ft += msec_s;
	return ft;
}

bool GameLevel::MoveObject(GameLevelObject* o, int d, int depth) {
	if (depth > 2) return false;
	depth += 1;
	Vec2 tartget_pos(o->x,o->y);
	if (d == 0) {
		tartget_pos.y -= 1;
		if (tartget_pos.y < 0) tartget_pos.y = size_h - 1;
	} else if (d == 1) {
		tartget_pos.y += 1;
		if (tartget_pos.y >= size_h) tartget_pos.y = 0;
	}
	else if (d == 2) {
		tartget_pos.x -= 1;
		if (tartget_pos.x < 0) tartget_pos.x = size_w - 1;
	}
	else if (d == 3) {
		tartget_pos.x += 1;
		if (tartget_pos.x >= size_w) tartget_pos.x = 0;
	}
	else return false;
	if (o->type == 0) return false;
	if (o->type == 2) return true;

	for (auto &co : m_objects) {
		if (co != o && co->type != 2 && Vec2(co->x,co->y) == tartget_pos) {
			if (!MoveObject(co, d, depth + 1)) {
				return false;
			} else
				break;
		}
	}

	SetObjectPos(o,tartget_pos.x,tartget_pos.y);
	return true;
}

bool GameLevel::IsWin() {
	bool is_win = true;
	for (auto o : m_objects) {
		if (o->type == 2) {
			bool is_fit = false;
			for (auto co : m_objects) {
				if (co->type == 1 && Vec2(co->x,co->y) == Vec2(o->x,o->y)) {
					is_fit = true;
					break;
				}
			}
			if (!is_fit) {
				is_win = false;
				break;
			}
		}
	}
	return is_win;
}
bool GameLevel::IsLose() {
	if (time == 0) return true;
	return false;
}
//util