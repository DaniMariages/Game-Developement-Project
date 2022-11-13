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
#include "Item.h"

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

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	currentAnimation = &idleAnimation;

	return true;
}

bool Player::Update()
{

	// L07 DONE 5: Add physics to the player - updated player position using physics

	int jump = 15; 
	int speed = 10;
	int jumponmovement = 20;
	float dump = 300;
	
	pbody->body->SetAngularDamping(dump);
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y); 
	b2Vec2 velcero = b2Vec2(0, 0);

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	
	//SALTO
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && OnAir == false) {

		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		OnAir = true;

		pbody->body->ApplyLinearImpulse(b2Vec2(0, -jump), pbody->body->GetPosition(), true);
		currentAnimation = &jumpAnimation;

	}

	//MOVIMIENTO A LA IZQUIERDA ESTANDO EN EL SUELO
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && OnAir == false) {
		vel = b2Vec2(-speed, jump);
		pbody->body->SetLinearVelocity(vel);
		currentAnimation = &leftAnimation;
	}
	//MOVIMIENTO A LA IZQUIERDA ESTANDO EN EL AIRE
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && OnAir == true && abletoimpulse == true)
	{
		pbody->body->ApplyLinearImpulse(b2Vec2(-4, 0), pbody->body->GetPosition(), true);
		abletoimpulse = false;
	}


	//MOVIMIENTO A LA DERECHA ESTANDO EN EL SUELO
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && OnAir == false) {
		vel = b2Vec2(speed, jump);
		pbody->body->SetLinearVelocity(vel);
		currentAnimation = &rightAnimation;
	}
	//MOVIMIENTO A LA DERECHA ESTANDO EN EL AIRE
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && OnAir == true && abletoimpulse == true)
	{
		pbody->body->ApplyLinearImpulse(b2Vec2(4, 0), pbody->body->GetPosition(), true);
		abletoimpulse = false;
	}


	//SALTO DIAGONAL HACIA LA IZQUIERDA
	if ((app->input->GetKey(SDL_SCANCODE_A) && app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)) {
		vel = b2Vec2(-6, -jumponmovement);
		pbody->body->SetLinearVelocity(vel);
		currentAnimation = &jumpleftAnimation;
	}
	//SALTO DIAGONAL HACIA LA DERECHA
	if ((app->input->GetKey(SDL_SCANCODE_D) && app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)) {
		vel = b2Vec2(6, -jumponmovement);
		pbody->body->SetLinearVelocity(vel);
		currentAnimation = &jumpAnimation;
	}

	//MOVIMIENTO Y GOLPE
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

	//QUEDARSE QUIETO AL DEJAR DE PULSAR EN EL AIRE
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP && OnAir == true) {
		
		pbody->body->SetLinearVelocity(pbody->body->GetLinearVelocity());
		currentAnimation = &idleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP && OnAir == true) {
		pbody->body->SetLinearVelocity(pbody->body->GetLinearVelocity());
		currentAnimation = &leftidleAnimation;
	}

	//VOLVER A LA ANIMACION NORMAL CUANDO ACABAS DE PEGAR
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_UP) {
		currentAnimation = &idleAnimation;
	}

	//QUEDARSE QUIETO AL DEJAR DE PULSAR EN EL SUELO
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP && OnAir == false) {
		pbody->body->SetLinearVelocity(velcero);
		currentAnimation = &idleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP && OnAir == false) {
		pbody->body->SetLinearVelocity(velcero);
		currentAnimation = &leftidleAnimation;
	}

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

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");

		app->audio->PlayFx(pickCoinFxId);
		
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");

		OnAir = false;
		abletoimpulse = true;
		
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");


		break;
	case ColliderType::SPIKES:
		LOG("Collision SPIKES");


		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}
