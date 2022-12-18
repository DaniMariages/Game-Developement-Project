#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "PathFinding.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access

	
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	//Loop para crear varios enemigos
	for (pugi::xml_node itemNode = config.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
	{
			Enemy* enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
			enemy->parameters = itemNode;
	}
	

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	intro = app->tex->Load("Assets/Textures/Menu.png");
	lose = app->tex->Load("Assets/Textures/Lose.png");
	win = app->tex->Load("Assets/Textures/Win.png");
	
	scene = app->scene->INTRO;
	
	// L03: DONE: Load map
	bool retLoad = app->map->Load();

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	//Sets the camera to be centered in isometric map
	if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		uint width, height;
		app->win->GetWindowSize(width, height);
		app->render->camera.x = width / 2;

		// Texture to highligh mouse position 
		mouseTileTex = app->tex->Load("Assets/Maps/path.png");

		// Texture to show path origin 
		originTex = app->tex->Load("Assets/Maps/x.png");
	}

	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {

		// Texture to highligh mouse position 
		mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

		// Texture to show path origin 
		originTex = app->tex->Load("Assets/Maps/x_square.png");
	}

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	switch (scene)
	{
	case app->scene->INTRO:

		app->render->DrawTexture(intro, 0,0);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) scene = app->scene->GAME;

		break;


	case app->scene->GAME:

		//Limites de la camara
		posicion = player->position.x;
		limiteIzq = app->map->mapData.width + 330;
		limiteDer = app->map->mapData.width + 3149;

		//Camara siguiendo al jugador
		if (limiteIzq <= posicion && posicion <= limiteDer) app->render->camera.x = -posicion + 450;

		// Draw map
		app->map->Draw();

		break;

	case app->scene->LOSE:

		app->render->DrawTexture(lose, posicion - 450, app->render->camera.y);
		
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
		{ 
			app->render->camera.y = 0;
			app->render->camera.x = 0;
			scene = app->scene->INTRO;
			break;
		}

		break;

	case app->scene->WIN:
		app->render->DrawTexture(win, 0, 0);
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
		{ 
			scene = app->scene->INTRO; 
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) 
	{
		if (seePath == false) seePath = true;
		else seePath = false;
	}

	if (seePath == true)
	{
		// L12: Get the latest calculated path and draw
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
