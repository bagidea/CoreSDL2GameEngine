#include "GameEngine.h"

struct ClipPos
{
	int x, y, width, height;
	ClipPos(int _x, int _y) : x(_x), y(_y), width(0), height(0){};
	ClipPos(int _x, int _y, int _width, int _height) : x(_x), y(_y), width(_width), height(_height){};
};

class MapCollision
{
public:
	static std::vector<std::vector<int>> CurrentMap;
	static void AddMap(std::vector<std::vector<int>> Map);
	static bool Check(int x, int y);
};

std::vector<std::vector<int>> MapCollision::CurrentMap;

void MapCollision::AddMap(std::vector<std::vector<int>> Map)
{
	MapCollision::CurrentMap = Map;
}

bool MapCollision::Check(int x, int y)
{
	if(MapCollision::CurrentMap[y][x] == 0)
		return true;
	return false;
}

class GameAsset
{
public:
	static Texture* Effect1;
	static Texture* Effect2;
};

Texture* GameAsset::Effect1 = NULL;
Texture* GameAsset::Effect2 = NULL;

class Effect : public Sprite
{
public:
	Effect(Texture* texture, int c, int r, int start, int end);
private:
	void Start();
	void Update();
	int c, r, start, end;
};

Effect::Effect(Texture* texture, int c, int r, int start, int end) : Sprite(texture)
{
	this->c = c;
	this->r = r;
	this->start = start;
	this->end = end;
}

void Effect::Start()
{
	CreateSpriteSheet(c, r);
	SetPivot(GetWidth()/2, GetHeight()/2);
	AddTag("Eff", start, end, TAG_MODE_ONCE);
	Play();
}

void Effect::Update()
{
	if(IsTag() == "Eff" && !IsPlay())
	{
		Scene::Current->RemoveChild(this);
		delete this;
	}
}

class Kunai : public Sprite
{
public:
	Kunai(Texture* texture, int dirX);
private:
	void Start();
	void Update();
	int dirX, speed;
};

Kunai::Kunai(Texture* texture, int dirX) : Sprite(texture){this->dirX = dirX;}

void Kunai::Start()
{
	speed = 15;
	SetPivot(GetWidth()/2, GetHeight()/2);
	if(dirX < 0)
		SetSize(-GetWidth(), GetHeight());
}

void Kunai::Update()
{
	SetPosition(GetX()+(speed*dirX), GetY());
	if(!MapCollision::Check(GetX()/128, GetY()/128) || GetX() < 0 || GetX() > 20*128)
	{
		Effect* eff = new Effect(GameAsset::Effect1, 5, 2, 1, 8);
		eff->SetFPS(30);
		eff->SetPosition(GetX(), GetY());
		Scene::Current->AddChild(eff);
		Scene::Current->RemoveChild(this);
		delete this;
	}
}

class Player : public Sprite
{
public:
	Player(Texture* texture);
	~Player();
	void KeyboardEvent(int key_event, int key_code);
private:
	void Start();
	void Update();
	void Clean();
	bool _left, _right, _jump, jump, _attack, attack, _shoot, shoot;
	int speed, gravity;
	int speedY;
	Texture* kunai_tex;
};

Player::Player(Texture* texture) : Sprite(texture){}
Player::~Player(){Clean();}

void Player::KeyboardEvent(int key_event, int key_code)
{
	if(key_event == KEYDOWN)
	{
		if(key_code == KEY_LEFT)
			_left = true;
		else if(key_code == KEY_RIGHT)
			_right = true;
		else if(key_code == KEY_SPACE && !_jump)
			_jump = true;
		else if(key_code == KEY_z)
			_attack = true;
		else if(key_code == KEY_x)
			_shoot = true;
	}
	else if(key_event == KEYUP)
	{
		if(key_code == KEY_LEFT)
			_left = false;
		else if(key_code == KEY_RIGHT)
			_right = false;
		else if(key_code == KEY_SPACE)
			_jump = false;
		else if(key_code == KEY_z)
			_attack = false;
		else if(key_code == KEY_x)
			_shoot = false;
	}
}


void Player::Start()
{
	_left = _right = _jump = jump = _attack = attack = _shoot = shoot = false;
	speed = 5;
	speedY = 0;
	gravity = 1;
	CreateSpriteSheet(10, 6);
	SetPivot(GetWidth()/2, GetHeight());
	SetPosition(128, 8*128);
	AddTag("Idle", 1, 10, TAG_MODE_LOOP);
	AddTag("Run", 11, 20, TAG_MODE_LOOP);
	AddTag("Jump", 21, 30, TAG_MODE_ONCE);
	AddTag("Attack", 31, 40, TAG_MODE_ONCE);
	AddTag("Shoot", 41, 50, TAG_MODE_ONCE);
	AddTag("Dead", 51, 60, TAG_MODE_ONCE);
	GotoAndPlay("Idle");
	kunai_tex = new Texture("images/Kunai.png");
}

void Player::Update()
{
	if((_left && !attack && !shoot) || (_left && jump && shoot))
	{
		if(GetX()-25 > 0 && MapCollision::Check((GetX()-25-speed)/128, GetY()/128))
			SetPosition(GetX()-speed, GetY());
		SetSize(-abs(GetWidth()), GetHeight());
		if(IsTag() != "Run" && !jump && !shoot)
			GotoAndPlay("Run");
	}
	else if((_right && !attack && !shoot) || (_right && jump && shoot))
	{
		if(GetX()+25 < 20*128 && MapCollision::Check((GetX()+25+speed)/128, GetY()/128))
			SetPosition(GetX()+speed, GetY());
		SetSize(abs(GetWidth()), GetHeight());
		if(IsTag() != "Run" && !jump && !shoot)
			GotoAndPlay("Run");
	}else{
		if(IsTag() != "Idle" && !jump && !attack && !shoot)
			GotoAndPlay("Idle");
	}
	if(_jump && !jump && !attack && !shoot)
	{
		speedY = -25;
		SetPosition(GetX(), GetY()-1);
	}
	if(_attack && !jump && !attack && !shoot)
	{
		if(IsTag() != "Attack")
			GotoAndPlay("Attack");
		attack = true;
	}else{
		if(IsTag() == "Attack" && !IsPlay())
		{
			GotoAndPlay("Idle");
			attack = false;
		}
	}
	if(_shoot && !attack && !shoot)
	{
		if(IsTag() != "Shoot")
			GotoAndPlay("Shoot");
		Kunai* k = new Kunai(kunai_tex, ((GetWidth() > 0)?1:-1));
		k->SetPosition(GetX(), GetY()-40);
		Scene::Current->AddChild(k);
		shoot = true;
	}else{
		if(IsTag() == "Shoot" && !IsPlay())
		{
			GotoAndPlay("Idle");
			shoot = false;
		}
	}
	if((!MapCollision::Check((GetX()+24)/128, (GetY()-50)/128) || !MapCollision::Check((GetX()-24)/128, (GetY()-50)/128)) && speedY < 0)
	{
		speedY = 0;
		SetPosition(GetX(), (GetY()-50)/128*128+128+50);
	}
	if(MapCollision::Check((GetX()+24)/128, (GetY()+speedY+1)/128) && MapCollision::Check((GetX()-24)/128, (GetY()+speedY+1)/128))
	{
		speedY += gravity;
		SetPosition(GetX(), GetY()+speedY);
		if(IsTag() != "Jump" && !shoot)
			GotoAndPlay("Jump");
		jump = true;
	}else{
		SetPosition(GetX(), (GetY()+speedY+1)/128*128-1);
		speedY = 0;
		jump = false;
	}
}

void Player::Clean()
{
	delete kunai_tex;
}

class Level : public Scene
{
public:
	Level();
	~Level();
private:
	void Start();
	void Update();
	void Clean();
	Texture* bg_tex;
	Texture* bg_cloud_tex;
	Texture* tile_tex;
	Texture* objects_tex;
	Sprite* bg;
	Sprite* bg_cloud1;
	Sprite* bg_cloud2;
	std::map<int, ClipPos*> map_clip;
	std::map<int, ClipPos*> objs;
	std::vector<Sprite*> boxs;
	std::vector<Sprite*> objects;
	Texture* ninja_tex;
	Player* player;
};

Level::Level() : Scene(){}
Level::~Level(){Clean();}

void Level::Start()
{
	bg_tex = new Texture("images/BG.png");
	bg = new Sprite(bg_tex);
	bg->SetSize(GameEngine::ScreenWidth, GameEngine::ScreenHeight);
	AddChild(bg);

	bg_cloud_tex = new Texture("images/Cloud.png");
	bg_cloud1 = new Sprite(bg_cloud_tex);
	bg_cloud1->SetSize(2560, 1024);
	bg_cloud1->SetPosition(0, -128);
	bg_cloud2 = new Sprite(bg_cloud_tex);
	bg_cloud2->SetSize(2560, 1024);
	bg_cloud2->SetPosition(bg_cloud1->GetWidth(), -128);
	AddChild(bg_cloud1);
	AddChild(bg_cloud2);

	map_clip[1] = new ClipPos(0, 0);
	map_clip[2] = new ClipPos(128, 0);
	map_clip[3] = new ClipPos(256, 0);
	map_clip[4] = new ClipPos(384, 0);
	map_clip[5] = new ClipPos(512, 0);
	map_clip[6] = new ClipPos(0, 128);
	map_clip[7] = new ClipPos(128, 128);
	map_clip[8] = new ClipPos(256, 128);
	map_clip[9] = new ClipPos(384, 128);
	map_clip[10] = new ClipPos(512, 128);
	map_clip[11] = new ClipPos(0, 256);
	map_clip[12] = new ClipPos(128, 256);
	map_clip[13] = new ClipPos(256, 256);
	map_clip[14] = new ClipPos(384, 256);
	map_clip[15] = new ClipPos(512, 256);
	map_clip[16] = new ClipPos(0, 384);
	map_clip[17] = new ClipPos(128, 384);
	map_clip[18] = new ClipPos(256, 384);
	map_clip[19] = new ClipPos(384, 384);
	map_clip[20] = new ClipPos(512, 384);

	int map_game[10][20] = 
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,16,17,18, 0, 0, 0, 0,16,18, 0},
		{ 0, 0, 0,16,17,18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,16,18, 0, 0, 1, 3},
		{ 0,16,17,18, 0,16,18, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 6, 8},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 9,10, 7, 8, 0, 0, 0, 1, 9,10, 8},
		{ 0, 0, 0, 0, 0, 1, 2, 9,10, 7, 7, 7, 4, 5, 2, 9,10, 7, 7, 8},
		{ 1, 2, 2, 2, 9,10, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8},
		{11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13}
	};

	int map_top_game[10][20] = 
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,20},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,14, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0,20, 0, 0, 0, 0, 0, 0, 0,14, 0, 0},
		{ 0, 0,20, 0, 0, 0,19,15, 0, 0, 0, 0, 0,20, 0, 0,19, 0,15, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	std::vector<std::vector<int>> collision =
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0},
		{0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1},
		{0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	MapCollision::AddMap(collision);

	objects_tex = new Texture("images/Objects.png");

	objs[0] = new ClipPos(0, 0, 384, 256);
	objs[1] = new ClipPos(384, 0, 128, 128);
	objs[2] = new ClipPos(512, 0, 128, 128);
	objs[3] = new ClipPos(640, 0, 128, 128);
	objs[4] = new ClipPos(384, 128, 128, 128);
	objs[5] = new ClipPos(512, 128, 128, 128);
	objs[6] = new ClipPos(640, 128, 128, 128);
	objs[7] = new ClipPos(0, 256, 256, 128);
	objs[8] = new ClipPos(256, 256, 128, 128);
	objs[9] = new ClipPos(384, 256, 128, 128);

	int o_pos[10][2] =
	{
		{8*128+80, 6*128},
		{2*128, 8*128},
		{12*128, 5*128},
		{14*128, 7*128},
		{15*128, 4*128},
		{15*128, 7*128},
		{19*128, 4*128},
		{3*128, 5*128},
		{12*128, 2*128},
		{4*128, 8*128}
	};

	for(int i = 0; i < 10; i++)
	{
		ClipPos posC = *objs[i];
		Sprite* obj = new Sprite(objects_tex);
		obj->CropImage(posC.x, posC.y, posC.width, posC.height);
		obj->SetPivot(obj->GetWidth()/2, obj->GetHeight());
		obj->SetPosition(o_pos[i][0], o_pos[i][1]);
		AddChild(obj);
		objects.push_back(obj);
	}

	tile_tex = new Texture("images/Tiles.png");

	for(int r = 0; r < collision.size(); r++)
	{
		for(int c = 0; c < collision[r].size(); c++)
		{
			if(map_game[r][c] != 0)
			{
				ClipPos posC = *map_clip[map_game[r][c]];
				Sprite* box = new Sprite(tile_tex);
				box->CropImage(posC.x, posC.y, 128, 128);
				box->SetPosition(128*c, 128*r);
				AddChild(box);
				boxs.push_back(box);
				if(map_top_game[r][c] != 0)
				{
					ClipPos posC = *map_clip[map_top_game[r][c]];
					Sprite* box = new Sprite(tile_tex);
					box->CropImage(posC.x, posC.y, 128, 128);
					box->SetPosition(128*c, 128*r);
					AddChild(box);
					boxs.push_back(box);
				}
			}
		}
	}

	ninja_tex = new Texture("images/Ninja.png");
	player = new Player(ninja_tex);
	AddChild(player);

	SetCameraPosition(player->GetX()-GameEngine::ScreenWidth/2, player->GetY()-128-GameEngine::ScreenHeight/2);

	GameAsset::Effect1 = new Texture("images/Effect1.png");
	GameAsset::Effect2 = new Texture("images/Effect2.png");
}

void Level::Update()
{
	int camX = GetCameraX()+(((player->GetX()-GameEngine::ScreenWidth/2)-GetCameraX())/20);
	int camY = GetCameraY()+(((player->GetY()-128-GameEngine::ScreenHeight/2)-GetCameraY())/20);
	SetCameraPosition(camX, camY);
	if(GetCameraX() <= 0)
		SetCameraPosition(0, GetCameraY());
	else if(GetCameraX() >= 20*128-GameEngine::ScreenWidth)
		SetCameraPosition(20*128-GameEngine::ScreenWidth, GetCameraY());
	if(GetCameraY() <= 0)
		SetCameraPosition(GetCameraX(), 0);
	bg->SetPosition(GetCameraX(), GetCameraY());
	bg_cloud1->SetPosition(bg_cloud1->GetX()-3, bg_cloud1->GetY());
	bg_cloud2->SetPosition(bg_cloud2->GetX()-3, bg_cloud2->GetY());
	if(bg_cloud1->GetX() < -bg_cloud1->GetWidth())
		bg_cloud1->SetPosition(bg_cloud2->GetX()+bg_cloud2->GetWidth(), bg_cloud1->GetY());
	if(bg_cloud2->GetX() < -bg_cloud2->GetWidth())
		bg_cloud2->SetPosition(bg_cloud1->GetX()+bg_cloud1->GetWidth(), bg_cloud2->GetY());
}

void Level::Clean()
{
	delete player;
	delete ninja_tex;
	delete bg;
	delete bg_tex;
	delete bg_cloud1;
	delete bg_cloud2;
	delete bg_cloud_tex;
	std::map<int, ClipPos*>::iterator it = map_clip.begin();
	while(it != map_clip.end())
	{
		delete it->second;
		it++;
	}
	map_clip.clear();
	it = objs.begin();
	while(it != objs.end())
	{
		delete it->second;
		it++;
	}
	objs.clear();
	for(int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
	for(int i = 0; i < boxs.size(); i++)
	{
		delete boxs[i];
	}
	boxs.clear();
	delete objects_tex;
	delete tile_tex;
	delete GameAsset::Effect1;
	delete GameAsset::Effect2;
}

int main(int argc, char* argv[])
{
	GameEngine* game = new GameEngine("Mini Game", 1280, 720);

	Level* scene = new Level();

	game->SetScene(scene);

	game->Run();

	delete scene;
	delete game;

	return 0;
}
