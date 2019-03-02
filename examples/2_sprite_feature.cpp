#include "GameEngine.h"

int main(int argc, char* argv[])
{
	//Create game window size 1027 x 768
	GameEngine* game = new GameEngine("Sprite feature and Animation", 1024, 768);

	//Create scene
	Scene* scene = new Scene();

	//Set game scene
	game->SetScene(scene);

	//Load texture
	Texture* bg_tex = new Texture("images/BG.png");
	Texture* tile_tex = new Texture("images/Tiles.png");
	Texture* tile2_tex = new Texture("images/Objects.png");
	Texture* ninja_tex = new Texture("images/Ninja.png");

	//Create sprite from Texture
	Sprite* BG = new Sprite(bg_tex);
	Sprite* FLOOR_LEFT = new Sprite(tile_tex);
	Sprite* FLOOR_MIDDLE = new Sprite(tile_tex);
	Sprite* FLOOR_RIGHT = new Sprite(tile_tex);
	Sprite* TREE = new Sprite(tile2_tex);
	Sprite* NINJA = new Sprite(ninja_tex);

	//Custom sprite
	// new_width = screen_height/(height/width)
	int new_width = (float)GameEngine::ScreenHeight/((float)(BG->GetHeight()/(float)BG->GetWidth()));
	BG->SetSize(new_width, GameEngine::ScreenHeight);
	BG->SetPosition(GameEngine::ScreenWidth/2-new_width/2, 0);
	//Crop image
	FLOOR_LEFT->CropImage(0, 0, 128, 128);
	FLOOR_LEFT->SetPivot(0, FLOOR_LEFT->GetHeight()/2);
	FLOOR_LEFT->SetPosition(50, 500);
	//Crop image
	FLOOR_MIDDLE->CropImage(128, 0, 128, 128);
	FLOOR_MIDDLE->SetPosition(60+FLOOR_MIDDLE->GetWidth(), 500);
	//Crop image
	FLOOR_RIGHT->CropImage(256, 0, 128, 128);
	FLOOR_RIGHT->SetPosition(70+FLOOR_RIGHT->GetWidth()*2, 500);
	//Crop image
	TREE->CropImage(0, 0, 384, 256);
	TREE->SetPosition(400, 100);
	//Create Sprite
	NINJA->CreateSpriteSheet(10, 6);
	//Add Tag Frame animation
	NINJA->AddTag("Run", 11, 20, TAG_MODE_LOOP);
	NINJA->SetPosition(100, 100);
	//Play animation
	//*** Play(), Goto(tag), GotoAndPlay(tag), Stop()
	//*** IsPlay(), IsTag()
	NINJA->Play();

	//Add sprite to scene game
	scene->AddChild(BG);
	scene->AddChild(FLOOR_LEFT);
	scene->AddChild(FLOOR_MIDDLE);
	scene->AddChild(FLOOR_RIGHT);
	scene->AddChild(TREE);
	scene->AddChild(NINJA);

	//Run game
	game->Run();

	//Delete all
	delete BG;
	delete FLOOR_LEFT;
	delete FLOOR_MIDDLE;
	delete FLOOR_RIGHT;
	delete TREE;
	delete NINJA;
	delete bg_tex;
	delete tile_tex;
	delete tile2_tex;
	delete ninja_tex;
	delete scene;
	delete game;

	return 0;
}
