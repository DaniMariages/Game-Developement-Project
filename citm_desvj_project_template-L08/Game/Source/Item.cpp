#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");

	idleAnimation.PushBack({ 96, 80, 16, 16 });
	idleAnimation.PushBack({ 112, 80, 16, 16 });
	idleAnimation.PushBack({ 128, 80, 16, 16 });
	idleAnimation.PushBack({ 144, 80, 16, 16 });
	idleAnimation.loop = true;
	idleAnimation.speed = 0.2f;

}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Item::Start() {


	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 10, bodyType::DYNAMIC);

	//Collider type
	pbody->ctype = ColliderType::ITEM;

	currentAnimation = &idleAnimation;
	

	return true;
}

bool Item::Update()
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	if (app->scene->player->win == true) 
	{
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 7;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 7;

		//app->render->DrawTexture(texture, position.x, position.y);

		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);

		
	}
	

	return true;
}

bool Item::CleanUp()
{
	return true;
}