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
	static std::shared_ptr<Texture> Effect1;
	static std::shared_ptr<Texture> Effect2;
	static std::shared_ptr<AudioTrack> AudioHit1;
	static std::shared_ptr<AudioTrack> AudioHit2;
};

std::shared_ptr<Texture> GameAsset::Effect1 = nullptr;
std::shared_ptr<Texture> GameAsset::Effect2 = nullptr;
std::shared_ptr<AudioTrack> GameAsset::AudioHit1 = nullptr;
std::shared_ptr<AudioTrack> GameAsset::AudioHit2 = nullptr;

class Effect :  public Sprite,
		public std::enable_shared_from_this<Sprite>
{
public:
	Effect(std::shared_ptr<Texture> texture, int c, int r, int start, int end);
private:
	void Start();
	void Update();
	int c, r, start, end;
};

Effect::Effect(std::shared_ptr<Texture> texture, int c, int r, int start, int end) : Sprite(texture)
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
		Scene::Current->RemoveChild(shared_from_this());
}

class Kunai :   public Sprite,
		public std::enable_shared_from_this<Sprite>
{
public:
	Kunai(std::shared_ptr<Texture> texture, int dirX);
private:
	void Start();
	void Update();
	int dirX, speed;
};

Kunai::Kunai(std::shared_ptr<Texture> texture, int dirX) : Sprite(texture){this->dirX = dirX;}

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
		std::shared_ptr<Effect> eff(new Effect(GameAsset::Effect1, 5, 2, 1, 8));
		eff->SetFPS(30);
		eff->SetPosition(GetX(), GetY());
		Scene::Current->AddChild(eff);
		GameAsset::AudioHit2->Play();
		Scene::Current->RemoveChild(shared_from_this());
	}
}

class Player : public Sprite
{
public:
	Player(std::shared_ptr<Texture> texture);
	void KeyboardEvent(int key_event, int key_code);
	int GetScore();
private:
	void Start();
	void Update();
	int score;
	bool _left, _right, _jump, jump, _attack, attack, _shoot, shoot;
	int speed, gravity;
	int speedY;
	std::shared_ptr<Texture> kunai_tex;
};

Player::Player(std::shared_ptr<Texture> texture) : Sprite(texture){}

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

int Player::GetScore()
{
	return score;
}

void Player::Start()
{
	score = 0;
	_left = _right = _jump = jump = _attack = attack = _shoot = shoot = false;
	speed = 5;
	speedY = 0;
	gravity = 1;
	CreateSpriteSheet(10, 6);
	SetPivot(GetWidth()/2, GetHeight()-1);
	SetPosition(128, 8*128);
	AddTag("Idle", 1, 10, TAG_MODE_LOOP);
	AddTag("Run", 11, 20, TAG_MODE_LOOP);
	AddTag("Jump", 21, 30, TAG_MODE_ONCE);
	AddTag("Attack", 31, 40, TAG_MODE_ONCE);
	AddTag("Shoot", 41, 50, TAG_MODE_ONCE);
	AddTag("Dead", 51, 60, TAG_MODE_ONCE);
	GotoAndPlay("Idle");
	kunai_tex = std::shared_ptr<Texture>(new Texture("images/Kunai.png"));
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
		GameAsset::AudioHit1->Play();
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
		std::shared_ptr<Kunai> k(new Kunai(kunai_tex, ((GetWidth() > 0)?1:-1)));
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

class Zombie : public Sprite
{
public:
	Zombie(std::shared_ptr<Texture> texture);
private:
	void Start();
	void Update();
};

Zombie::Zombie(std::shared_ptr<Texture> texture) : Sprite(texture){}

void Zombie::Start()
{
	CreateSpriteSheet(10, 4);
	SetPivot(GetWidth()/2, GetHeight()-1);
	SetPosition(512, 8*128);
	AddTag("Idle", 1, 10, TAG_MODE_LOOP);
	AddTag("Run", 11, 20, TAG_MODE_LOOP);
	AddTag("Attack", 21, 28, TAG_MODE_ONCE);
	AddTag("Dead", 31, 40, TAG_MODE_ONCE);
	GotoAndPlay("Idle");
}

void Zombie::Update()
{
}

class Level : public Scene
{
public:
	Level();
	~Level();
private:
	void Start();
	void UI();
	void UI_Update();
	void Update();
	void Clean();
	std::shared_ptr<Texture> bg_tex;
	std::shared_ptr<Texture> bg_cloud_tex;
	std::shared_ptr<Texture> tile_tex;
	std::shared_ptr<Texture> objects_tex;
	std::shared_ptr<Texture> hart_tex;
	std::shared_ptr<Sprite> bg;
	std::shared_ptr<Sprite> bg_cloud1;
	std::shared_ptr<Sprite> bg_cloud2;
	std::vector<std::shared_ptr<Sprite>> boxs;
	std::vector<std::shared_ptr<Sprite>> objects;
	std::shared_ptr<Sprite> hart1;
	std::shared_ptr<Sprite> hart2;
	std::shared_ptr<Sprite> hart3;
	std::shared_ptr<Text> score_txt;
	std::shared_ptr<Text> score_value_txt;
	std::shared_ptr<Texture> ninja_tex;
	std::shared_ptr<Player> player;
	std::shared_ptr<Texture> zombie_tex;
	std::shared_ptr<Zombie> zombie;
	std::shared_ptr<AudioMusic> music;
};

Level::Level() : Scene(){}
Level::~Level(){Clean();}

void Level::Start()
{
	bg_tex = std::shared_ptr<Texture>(new Texture("images/BG.png"));
	bg = std::shared_ptr<Sprite>(new Sprite(bg_tex));
	bg->SetSize(GameEngine::ScreenWidth, GameEngine::ScreenHeight);
	AddChild(bg);

	bg_cloud_tex = std::shared_ptr<Texture>(new Texture("images/Cloud.png"));
	bg_cloud1 = std::shared_ptr<Sprite>(new Sprite(bg_cloud_tex));
	bg_cloud1->SetSize(2560, 1024);
	bg_cloud1->SetPosition(0, -128);
	bg_cloud2 = std::shared_ptr<Sprite>(new Sprite(bg_cloud_tex));
	bg_cloud2->SetSize(2560, 1024);
	bg_cloud2->SetPosition(bg_cloud1->GetWidth(), -128);
	AddChild(bg_cloud1);
	AddChild(bg_cloud2);

	std::vector<std::vector<int>> map_clip = 
	{
		{0, 0},
		{128, 0},
		{256, 0},
		{384, 0},
		{512, 0},
		{0, 128},
		{128, 128},
		{256, 128},
		{384, 128},
		{512, 128},
		{0, 256},
		{128, 256},
		{256, 256},
		{384, 256},
		{512, 256},
		{0, 384},
		{128, 384},
		{256, 384},
		{384, 384},
		{512, 384}
	};

	std::vector<std::vector<int>> map_game = 
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

	std::vector<std::vector<int>> map_top_game = 
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

	objects_tex = std::shared_ptr<Texture>(new Texture("images/Objects.png"));

	std::vector<std::vector<int>> objs = 
	{
		{0, 0, 384, 256},
		{384, 0, 128, 128},
		{512, 0, 128, 128},
		{640, 0, 128, 128},
		{384, 128, 128, 128},
		{512, 128, 128, 128},
		{640, 128, 128, 128},
		{0, 256, 256, 128},
		{256, 256, 128, 128},
		{384, 256, 128, 128}
	};

	std::vector<std::vector<int>> o_pos =
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

	for(int i = 0; i < objs.size(); i++)
	{
		std::vector<int> posC = objs[i];
		std::shared_ptr<Sprite> obj(new Sprite(objects_tex));
		obj->CropImage(posC[0], posC[1], posC[2], posC[3]);
		obj->SetPivot(obj->GetWidth()/2, obj->GetHeight());
		obj->SetPosition(o_pos[i][0], o_pos[i][1]);
		AddChild(obj);
		objects.push_back(obj);
	}

	tile_tex = std::shared_ptr<Texture>(new Texture("images/Tiles.png"));

	for(int r = 0; r < collision.size(); r++)
	{
		for(int c = 0; c < collision[r].size(); c++)
		{
			if(map_game[r][c] != 0)
			{
				std::vector<int> posC = map_clip[map_game[r][c]-1];
				std::shared_ptr<Sprite> box(new Sprite(tile_tex));
				box->CropImage(posC[0], posC[1], 128, 128);
				box->SetPosition(128*c, 128*r);
				AddChild(box);
				boxs.push_back(box);
				if(map_top_game[r][c] != 0)
				{
					std::vector<int> posC = map_clip[map_top_game[r][c]-1];
					std::shared_ptr<Sprite> box(new Sprite(tile_tex));
					box->CropImage(posC[0], posC[1], 128, 128);
					box->SetPosition(128*c, 128*r);
					AddChild(box);
					boxs.push_back(box);
				}
			}
		}
	}

	ninja_tex = std::shared_ptr<Texture>(new Texture("images/Ninja.png"));
	player = std::shared_ptr<Player>(new Player(ninja_tex));
	AddChild(player);

	SetCameraPosition(player->GetX()-GameEngine::ScreenWidth/2, player->GetY()-128-GameEngine::ScreenHeight/2);

	zombie_tex = std::shared_ptr<Texture>(new Texture("images/Zombie.png"));
	zombie = std::shared_ptr<Zombie>(new Zombie(zombie_tex));
	AddChild(zombie);

	GameAsset::Effect1 = std::shared_ptr<Texture>(new Texture("images/Effect1.png"));
	GameAsset::Effect2 = std::shared_ptr<Texture>(new Texture("images/Effect2.png"));

	music = LoadAudioMusic("audios/Music.mp3");
	music->SetVolume(20);
	music->Play();

	GameAsset::AudioHit1 = LoadAudioTrack("audios/Hit.wav");
	GameAsset::AudioHit2 = LoadAudioTrack("audios/Hit2.wav");

	UI();
}

void Level::UI()
{
	hart_tex = std::shared_ptr<Texture>(new Texture("images/HartIcon.png"));
	hart1 = std::shared_ptr<Sprite>(new Sprite(hart_tex));
	hart2 = std::shared_ptr<Sprite>(new Sprite(hart_tex));
	hart3 = std::shared_ptr<Sprite>(new Sprite(hart_tex));
	hart1->CreateSpriteSheet(2, 1);
	hart2->CreateSpriteSheet(2, 1);
	hart3->CreateSpriteSheet(2, 1);
	hart1->AddTag("False", 2);
	hart1->AddTag("True", 1);
	hart2->AddTag("False", 2);
	hart2->AddTag("True", 1);
	hart3->AddTag("False", 2);
	hart3->AddTag("True", 1);
	hart1->SetPosition(GetCameraX()+50, GetCameraY()+20);
	hart2->SetPosition(GetCameraX()+100, GetCameraY()+20);
	hart3->SetPosition(GetCameraX()+150, GetCameraY()+20);
	AddChild(hart1);
	AddChild(hart2);
	AddChild(hart3);

	score_txt = std::shared_ptr<Text>(new Text("SCORE"));
	score_txt->SetSize(20);
	score_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	score_txt->SetColor(COLOR_WHITE);
	score_txt->SetPosition(GetCameraX()+GameEngine::ScreenWidth/2, GetCameraY()+20);
	AddText(score_txt);

	score_value_txt = std::shared_ptr<Text>(new Text("0"));
	score_value_txt->SetSize(30);
	score_value_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	score_value_txt->SetColor(COLOR_WHITE);
	score_value_txt->SetPosition(GetCameraX()+GameEngine::ScreenWidth/2, GetCameraY()+50);
	AddText(score_value_txt);
}

void Level::UI_Update()
{
	hart1->SetPosition(GetCameraX()+50, GetCameraY()+20);
	hart2->SetPosition(GetCameraX()+100, GetCameraY()+20);
	hart3->SetPosition(GetCameraX()+150, GetCameraY()+20);
	score_txt->SetPosition(GetCameraX()+GameEngine::ScreenWidth/2, GetCameraY()+20);
	score_value_txt->SetText(std::to_string(player->GetScore()));
	score_value_txt->SetPosition(GetCameraX()+GameEngine::ScreenWidth/2, GetCameraY()+50);
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

	UI_Update();
}

void Level::Clean()
{
	objects.clear();
	boxs.clear();
}

int main(int argc, char* argv[])
{
	std::unique_ptr<GameEngine> game(new GameEngine("Mini Game", 1280, 720));

	std::shared_ptr<Level> scene(new Level());

	game->SetScene(scene);

	game->Run();

	return 0;
}
