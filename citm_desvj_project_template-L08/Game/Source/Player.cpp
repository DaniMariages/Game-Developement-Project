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
	rightAnimation.PushBack({ 19, 146, 11, 15 });
	rightAnimation.PushBack({ 34, 145, 13, 15 });
	rightAnimation.PushBack({ 51, 146, 11, 14 });
	rightAnimation.speed = 0.05f;

	//Idle animation
	idleAnimation.PushBack({ 19,115,10,13 });
	idleAnimation.PushBack({ 35,114,10,14 });
	idleAnimation.PushBack({ 51,115,10,13 });
	idleAnimation.PushBack({ 67,115,10,13 });
	idleAnimation.speed = 0.03f;

	//left animation
	leftAnimation.PushBack({ 52,166,9,13 });
	leftAnimation.PushBack({ 35,165,11,13 });
	leftAnimation.PushBack({ 20,166,9,13 });
	leftAnimation.speed = 0.05f;

	//Jump animation
	jumpAnimation.PushBack({ 17, 194, 14, 13 });

	//Jump animation left
	jumpleftAnimation.PushBack({ 64, 194, 14, 13 });

	//Punch animation
	punchAnimation.PushBack({ 19,259,13,13});
	punchAnimation.PushBack({ 35,259,13,13 });
	punchAnimation.PushBack({ 83,259,13,13 });
	punchAnimation.PushBack({ 51,259,12,13 });
	punchAnimation.PushBack({ 67,259,10,13 });
	punchAnimation.speed = 0.05f;

	//Punch left aniamtion
	punchleftAnimation.PushBack({ 202,287,13,13 });
	punchleftAnimation.PushBack({ 186,287,13,13 });
	punchleftAnimation.PushBack({ 138,287,13,13 });
	punchleftAnimation.PushBack({ 171,287,12,13 });
	punchleftAnimation.PushBack({ 157,287,10,13 });
	punchleftAnimation.speed = 0.05f;

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
	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC);
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
		currentAnimation = &leftAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed, -GRAVITY_Y);
		currentAnimation = &rightAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) && app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(-speed, speed + GRAVITY_Y);
		currentAnimation = &jumpleftAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) && app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(speed, speed + GRAVITY_Y);
		currentAnimation = &jumpAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
		currentAnimation = &punchAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		currentAnimation = &punchleftAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		currentAnimation = &punchAnimation;
	}

	//VOLVER AL IDLE ANIMATION
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {
		currentAnimation = &idleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		currentAnimation = &idleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		currentAnimation = &idleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_UP) {
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