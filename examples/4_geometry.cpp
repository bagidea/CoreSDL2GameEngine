#include "GameEngine.h"

class Object : public Text
{
public:
	Object(std::string text);
private:
	void Start();
	void GeometryBack();
	void GeometryFront();
};

Object::Object(std::string text) : Text(text){}

void Object::Start()
{
	SetSize(12);
	SetColor(Color(150, 150, 150, 255));
}

void Object::GeometryBack()
{
	int mX = Mouse::mouseX;
	int mY = Mouse::mouseY;
	if(mX > GetX()-3 && mX < GetX()+178 && mY >= GetY() && mY <= GetY()+16)
		Scene::Current->DrawFillRect(GetX()-5, GetY()-2, 180, 18, Color(20, 20, 20, 100));
}

void Object::GeometryFront()
{
}

class Stage : public Scene
{
public:
	Stage();
	void KeyboardEvent(int key_event, int key_code);
private:
	void Start();
	void Update();
	void GeometryBack();
	void DrawGrid();
	bool ui_show;
	std::shared_ptr<Text> load_txt;
	std::shared_ptr<Text> textures_txt;
	std::shared_ptr<Text> objects_txt;
	std::shared_ptr<Text> transform_txt;
	std::shared_ptr<Text> position_txt;
	std::shared_ptr<Text> posX_txt;
	std::shared_ptr<Text> posY_txt;
	std::shared_ptr<Text> size_txt;
	std::shared_ptr<Text> sizeW_txt;
	std::shared_ptr<Text> sizeH_txt;
	std::shared_ptr<Text> rotation_txt;
	std::shared_ptr<Text> rotationV_txt;
	std::shared_ptr<Text> name_txt;
	std::shared_ptr<Texture> new_tex;
	std::shared_ptr<Texture> open_tex;
	std::shared_ptr<Texture> save_tex;
	std::shared_ptr<Sprite> i_new;
	std::shared_ptr<Sprite> i_open;
	std::shared_ptr<Sprite> i_save;
	std::shared_ptr<Texture> ninja_tex;
	std::shared_ptr<Sprite> ninja;
	std::vector<std::shared_ptr<Object>> texture_list_item;
	std::vector<std::shared_ptr<Object>> object_list_item;
};

Stage::Stage() : Scene(){}

void Stage::KeyboardEvent(int key_event, int key_code)
{
	if(key_event == KEYDOWN)
	{
		if(key_code == KEY_F1)
			ui_show = !ui_show;
	}
}

void Stage::Start()
{
	ui_show = true;
	load_txt = std::shared_ptr<Text>(new Text("LOAD TEXTURE"));
	load_txt->SetSize(15);
	load_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	load_txt->SetPosition(105, 18);
	AddText(load_txt);

	//Textures
	textures_txt = std::shared_ptr<Text>(new Text("Textures"));
	textures_txt->SetSize(15);
	textures_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	textures_txt->SetColor(COLOR_WHITE);
	textures_txt->SetPosition(105, 55);
	AddText(textures_txt);

	for(int i = 0; i < 13; i++)
	{
		texture_list_item.push_back(std::shared_ptr<Object>(new Object("Texture Item")));
		texture_list_item[i]->SetPosition(20, i*20+82);
		AddText(texture_list_item[i]);
	}

	//Objects
	objects_txt = std::shared_ptr<Text>(new Text("Objects"));
	objects_txt->SetSize(15);
	objects_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	objects_txt->SetColor(COLOR_WHITE);
	objects_txt->SetPosition(105, 360);
	AddText(objects_txt);

	for(int i = 0; i < 16; i++)
	{
		object_list_item.push_back(std::shared_ptr<Object>(new Object("Object Item")));
		object_list_item[i]->SetPosition(20, i*20+388);
		AddText(object_list_item[i]);
	}

	transform_txt = std::shared_ptr<Text>(new Text("Transform"));
	transform_txt->SetSize(15);
	transform_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	transform_txt->SetColor(COLOR_WHITE);
	transform_txt->SetPosition(GameEngine::ScreenWidth-110, 10);
	AddText(transform_txt);

	position_txt = std::shared_ptr<Text>(new Text("Position  [X, Y]"));
	position_txt->SetSize(12);
	position_txt->SetColor(COLOR_WHITE);
	position_txt->SetPosition(GameEngine::ScreenWidth-200, 45);
	AddText(position_txt);

	posX_txt = std::shared_ptr<Text>(new Text("0"));
	posY_txt = std::shared_ptr<Text>(new Text("0"));
	posX_txt->SetSize(12);
	posX_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	posX_txt->SetPosition(GameEngine::ScreenWidth-137, 68);
	posY_txt->SetSize(12);
	posY_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	posY_txt->SetPosition(GameEngine::ScreenWidth-75, 68);
	AddText(posX_txt);
	AddText(posY_txt);

	size_txt = std::shared_ptr<Text>(new Text("Size  [Width, Height]"));
	size_txt->SetSize(12);
	size_txt->SetColor(COLOR_WHITE);
	size_txt->SetPosition(GameEngine::ScreenWidth-200, 100);
	AddText(size_txt);

	sizeW_txt = std::shared_ptr<Text>(new Text("0"));
	sizeH_txt = std::shared_ptr<Text>(new Text("0"));
	sizeW_txt->SetSize(12);
	sizeW_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	sizeW_txt->SetPosition(GameEngine::ScreenWidth-137, 123);
	sizeH_txt->SetSize(12);
	sizeH_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	sizeH_txt->SetPosition(GameEngine::ScreenWidth-75, 123);
	AddText(sizeW_txt);
	AddText(sizeH_txt);

	rotation_txt = std::shared_ptr<Text>(new Text("Rotation"));
	rotation_txt->SetSize(12);
	rotation_txt->SetColor(COLOR_WHITE);
	rotation_txt->SetPosition(GameEngine::ScreenWidth-200, 160);
	AddText(rotation_txt);

	rotationV_txt = std::shared_ptr<Text>(new Text("0"));
	rotationV_txt->SetSize(12);
	rotationV_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	rotationV_txt->SetPosition(GameEngine::ScreenWidth-110, 158);
	AddText(rotationV_txt);

	//Name
	name_txt = std::shared_ptr<Text>(new Text("Name"));
	name_txt->SetSize(15);
	name_txt->SetAlign(TEXT_ALIGN_MIDDLE);
	name_txt->SetColor(COLOR_WHITE);
	name_txt->SetPosition(GameEngine::ScreenWidth-110, 205);
	AddText(name_txt);

	new_tex = std::shared_ptr<Texture>(new Texture("images/Icon/IconNew.png"));
	open_tex = std::shared_ptr<Texture>(new Texture("images/Icon/IconOpen.png"));
	save_tex = std::shared_ptr<Texture>(new Texture("images/Icon/IconSave.png"));

	i_new = std::shared_ptr<Sprite>(new Sprite(new_tex));
	i_open = std::shared_ptr<Sprite>(new Sprite(open_tex));
	i_save = std::shared_ptr<Sprite>(new Sprite(save_tex));

	i_new->SetPosition(GameEngine::ScreenWidth-175, GameEngine::ScreenHeight-60);
	i_open->SetPosition(GameEngine::ScreenWidth-120, GameEngine::ScreenHeight-60);
	i_save->SetPosition(GameEngine::ScreenWidth-65, GameEngine::ScreenHeight-60);

	AddChild(i_new);
	AddChild(i_open);
	AddChild(i_save);
}

void Stage::Update()
{
	load_txt->SetVisible(ui_show);
	textures_txt->SetVisible(ui_show);
	objects_txt->SetVisible(ui_show);
	transform_txt->SetVisible(ui_show);
	position_txt->SetVisible(ui_show);
	posX_txt->SetVisible(ui_show);
	posY_txt->SetVisible(ui_show);
	size_txt->SetVisible(ui_show);
	sizeW_txt->SetVisible(ui_show);
	sizeH_txt->SetVisible(ui_show);
	rotation_txt->SetVisible(ui_show);
	rotationV_txt->SetVisible(ui_show);
	i_new->SetVisible(ui_show);
	i_open->SetVisible(ui_show);
	i_save->SetVisible(ui_show);
	i_save->SetVisible(ui_show);
	for(std::shared_ptr<Object> tex : texture_list_item)
		tex->SetVisible(ui_show);
	for(std::shared_ptr<Object> obj : object_list_item)
		obj->SetVisible(ui_show);
}

void Stage::GeometryBack()
{
	DrawGrid();
	if(ui_show)
	{
		int mX = Mouse::mouseX;
		int mY = Mouse::mouseY;
		//BGIcon
		DrawFillRect(GameEngine::ScreenWidth-185, GameEngine::ScreenHeight-65, 180, 60, Color(50, 50, 50, 150));
		DrawFillRect(GameEngine::ScreenWidth-180, GameEngine::ScreenHeight-60, 170, 50, Color(50, 50, 50, 200));
		if(mY > GameEngine::ScreenHeight-60 && mY < GameEngine::ScreenHeight-10)
		{
			if(mX > GameEngine::ScreenWidth-180 && mX < GameEngine::ScreenWidth-120)
				DrawFillRect(GameEngine::ScreenWidth-180, GameEngine::ScreenHeight-60, 60, 50, Color(40, 40, 40, 255));
			else if(mX > GameEngine::ScreenWidth-120 && mX < GameEngine::ScreenWidth-65)
				DrawFillRect(GameEngine::ScreenWidth-120, GameEngine::ScreenHeight-60, 55, 50, Color(40, 40, 40, 255));
			else if(mX > GameEngine::ScreenWidth-65 && mX < GameEngine::ScreenWidth-10)
				DrawFillRect(GameEngine::ScreenWidth-65, GameEngine::ScreenHeight-60, 55, 50, Color(40, 40, 40, 255));
		}
		//Button LOAD TEXTURE
		DrawFillRect(5, 5, 200, 40, Color(200, 150, 255, 150));
		DrawFillRect(10, 10, 190, 30, Color(200, ((mX > 10 && mX < 200 && mY > 10 && mY < 40)?200:150), 255, 255));
		//Textures
		DrawFillRect(5, 50, 200, 300, Color(50, 50, 50, 150));
		DrawFillRect(10, 55, 190, 290, Color(50, 50, 50, 200));
		DrawFillRect(10, 55, 190, 20, Color(30, 30, 30, 255));
		//Objects
		DrawFillRect(5, 355, 200, 360, Color(50, 100, 100, 150));
		DrawFillRect(10, 360, 190, 350, Color(50, 100, 100, 200));
		DrawFillRect(10, 360, 190, 20, Color(30, 80, 80, 255));
		//Transform
		DrawFillRect(GameEngine::ScreenWidth-215, 5, 210, 190, Color(50, 50, 50, 150));
		DrawFillRect(GameEngine::ScreenWidth-210, 10, 200, 178, Color(50, 50, 50, 200));
		DrawFillRect(GameEngine::ScreenWidth-210, 10, 200, 20, Color(30, 30, 30, 255));
		//Position
		DrawFillRect(GameEngine::ScreenWidth-170, 65, 60, 20, COLOR_WHITE);
		DrawFillRect(GameEngine::ScreenWidth-105, 65, 60, 20, COLOR_WHITE);
		DrawRect(GameEngine::ScreenWidth-170, 65, 60, 20, COLOR_BLACK);
		DrawRect(GameEngine::ScreenWidth-105, 65, 60, 20, COLOR_BLACK);
		//Size
		DrawFillRect(GameEngine::ScreenWidth-170, 120, 60, 20, COLOR_WHITE);
		DrawFillRect(GameEngine::ScreenWidth-105, 120, 60, 20, COLOR_WHITE);
		DrawRect(GameEngine::ScreenWidth-170, 120, 60, 20, COLOR_BLACK);
		DrawRect(GameEngine::ScreenWidth-105, 120, 60, 20, COLOR_BLACK);
		//Rotation
		DrawFillRect(GameEngine::ScreenWidth-150, 155, 80, 20, COLOR_WHITE);
		DrawRect(GameEngine::ScreenWidth-150, 155, 80, 20, COLOR_BLACK);
		//Name
		DrawFillRect(GameEngine::ScreenWidth-215, 200, 210, 62, Color(50, 50, 50, 150));
		DrawFillRect(GameEngine::ScreenWidth-210, 205, 200, 50, Color(50, 50, 50, 200));
		DrawFillRect(GameEngine::ScreenWidth-210, 205, 200, 20, Color(30, 30, 30, 255));
		DrawFillRect(GameEngine::ScreenWidth-205, 230, 190, 20, COLOR_WHITE);
	}
}

void Stage::DrawGrid()
{
	int distance = 10;
	for(int i = 1; i*distance < GameEngine::ScreenWidth; i++)
		DrawLine(i*distance, 0, i*distance, GameEngine::ScreenHeight, Color(255, 255, 255, 50));
	for(int i = 1; i*distance < GameEngine::ScreenHeight; i++)
		DrawLine(0, i*distance, GameEngine::ScreenWidth, i*distance, Color(255, 255, 255, 50));
	distance = 40;
	for(int i = 1; i*distance < GameEngine::ScreenWidth; i++)
		DrawLine(i*distance, 0, i*distance, GameEngine::ScreenHeight, COLOR_WHITE);
	for(int i = 1; i*distance < GameEngine::ScreenHeight; i++)
		DrawLine(0, i*distance, GameEngine::ScreenWidth, i*distance, COLOR_WHITE);
}

int main(int argc, char* argv[])
{
	std::unique_ptr<GameEngine> game(new GameEngine("Geometry Basic", 1280, 720));

	std::shared_ptr<Stage> scene(new Stage());
	scene->SetBackgroundColor(Color(200, 200, 200));

	game->SetScene(scene);

	game->Run();

	return 0;
}
