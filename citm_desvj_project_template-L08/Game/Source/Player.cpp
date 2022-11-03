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
	rightAnimation.PushBack({ 32, 292, 18, 26 });
	rightAnimation.PushBack({ 62, 290, 22, 26 });
	rightAnimation.PushBack({ 96, 292, 18, 26 });
	rightAnimation.speed = 0.05f;

	//Idle animation
	idleAnimation.PushBack({ 30,228,20,26 });
	idleAnimation.PushBack({ 62,226,20,28 });
	idleAnimation.PushBack({ 94,228,20,26 });
	idleAnimation.PushBack({ 126,228,20,26 });
	idleAnimation.speed = 0.03f;

	//left animation
	leftAnimation.PushBack({ 96,330,18,26 });
	leftAnimation.PushBack({ 62,328,22,26 });
	leftAnimation.PushBack({ 32,330,18,26 });
	leftAnimation.speed = 0.05f;

	//Jump animation
	jumpAnimation.PushBack({ 26, 386, 28, 26 });

	//Jump animation left
	jumpleftAnimation.PushBack({ 120, 386, 28, 26 });

	//Punch animation
	punchAnimation.PushBack({ 30,516,26,26});
	punchAnimation.PushBack({ 62,516,26,26 });
	punchAnimation.PushBack({ 94,516,24,26 });
	punchAnimation.PushBack({ 126,516,20,26 });
	punchAnimation.speed = 0.05f;

	//Punch left animation
	punchleftAnimation.PushBack({ 396,572,26,26 });
	punchleftAnimation.PushBack({ 364,572,26,26 });
	punchleftAnimation.PushBack({ 334,572,24,26 });
	punchleftAnimation.PushBack({ 306,572,20,26 });
	punchleftAnimation.speed = 0.05f;

	//Left Idle animation
	leftidleAnimation.PushBack({ 183,194,20,26 });
	leftidleAnimation.PushBack({ 151,192,20,28 });
	leftidleAnimation.PushBack({ 119,194,20,26 });
	leftidleAnimation.PushBack({ 87,194,20,26 });
	leftidleAnimation.speed = 0.03f;
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
	/*pbody = app->physics->CreateRectangle(position.x, position.y, 18,28, bodyType::DYNAMIC);*/
	pbody = app->physics->CreateCircle(position.x, position.y,13, bodyType::DYNAMIC);
	return true;
}

bool Player::Update()
{

	// L07 DONE 5: Add physics to the player - updated player position using physics

	int speed = 3; 
	float flo = 10;
	float dump = 13;
	float impulse = pbody->body->GetMass() * 10;
	pbody->body->SetAngularDamping(dump);
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y); 

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(0, +GRAVITY_Y);
		pbody->body->SetLinearVelocity(vel);
		currentAnimation = &jumpAnimation;

	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed, -GRAVITY_Y);
		pbody->body->SetAngularVelocity(-flo);
		/*pbody->body->SetLinearVelocity(vel);*/
		currentAnimation = &leftAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed, -GRAVITY_Y);
		/*pbody->body->SetLinearVelocity(vel);*/
		pbody->body->SetAngularVelocity(flo);
		currentAnimation = &rightAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) && app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(-speed, speed + GRAVITY_Y);
		pbody->body->SetLinearVelocity(vel);
		currentAnimation = &jumpleftAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) && app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(speed, speed + GRAVITY_Y);
		pbody->body->SetLinearVelocity(vel);
		currentAnimation = &jumpAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
		currentAnimation = &punchAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
		vel = b2Vec2(0, 0);
		currentAnimation = &punchleftAnimation;
	}

	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		currentAnimation = &punchAnimation;
	}

	//VOLVER AL IDLE ANIMATION
	//if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {
	//	currentAnimation = &idleAnimation;
	//}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		currentAnimation = &idleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		currentAnimation = &leftidleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_UP) {
		currentAnimation = &idleAnimation;
	}


	//Set the velocity of the pbody of the player
	//float flo = 2;
	//pbody->body->SetAngularVelocity(flo);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 10;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10;

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);


	return true;
}

bool Player::CleanUp()
{

	return true;
}