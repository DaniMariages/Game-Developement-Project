#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	//Right animation
	rightAnimation.PushBack({ 36, 1, 34, 26 });
	rightAnimation.PushBack({ 71, 1, 34, 26 });
	rightAnimation.PushBack({ 106, 1, 34, 26 });
	rightAnimation.speed = 0.05f;

	//Idle animation
	idleAnimation.PushBack({ 36,1,34,26 });

	//Jump animation
	jumpAnimation.PushBack({ 351, 28, 34, 26 });

}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnimation;

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 16, bodyType::DYNAMIC);
	return true;
}

bool Player::Update()
{

	// L07 DONE 5: Add physics to the player - updated player position using physics

	int speed = 1; 
	float impulse = pbody->body->GetMass() * 10;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y); 

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(0, speed+GRAVITY_Y);
		currentAnimation = &jumpAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed, -GRAVITY_Y);
		currentAnimation = &rightAnimation;
	}

	//VOLVER AL IDLE ANIMATION
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {
		currentAnimation = &idleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		currentAnimation = &idleAnimation;
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);


	return true;
}

bool Player::CleanUp()
{

	return true;
}