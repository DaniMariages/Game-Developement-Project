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
#include "EntityManager.h"
#include "Enemy.h"
#include "PathFinding.h"


Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");

	//Right animation
	rightAnimation.PushBack({ 32, 292, 18, 26 });
	rightAnimation.PushBack({ 62, 290, 22, 26 });
	rightAnimation.PushBack({ 96, 292, 18, 26 });
	rightAnimation.speed = 0.12f;

	//Idle animation
	idleAnimation.PushBack({ 30,228,20,26 });
	idleAnimation.PushBack({ 62,226,20,28 });
	idleAnimation.PushBack({ 94,228,20,26 });
	idleAnimation.PushBack({ 126,228,20,26 });
	idleAnimation.speed = 0.05f;

	//left animation
	leftAnimation.PushBack({ 96,330,18,26 });
	leftAnimation.PushBack({ 62,328,22,26 });
	leftAnimation.PushBack({ 32,330,18,26 });
	leftAnimation.speed = 0.12f;

	//Jump animation
	jumpAnimation.PushBack({ 26, 386, 28, 26 });

	//Jump animation left
	jumpleftAnimation.PushBack({ 120, 386, 28, 26 });

	//Punch animation
	punchAnimation.PushBack({ 30,516,26,26 });
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

	//Win animation
	winAnimation.PushBack({ 124, 34, 26, 29 });
	winAnimation.PushBack({ 156, 34, 26, 29 });
	winAnimation.PushBack({ 124, 34, 26, 29 });
	winAnimation.PushBack({ 156, 34, 26, 29 });
	winAnimation.PushBack({ 124, 34, 26, 29 });
	winAnimation.PushBack({ 156, 34, 26, 29 });
	winAnimation.speed = 0.2f;
}

Enemy::~Enemy() {

}

bool Enemy::Awake()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Enemy::Start() 
{
	//Textura del enemigo
	texture = app->tex->Load(texturePath);

	//Body del enemigo
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);

	//Asignacion de la clase al cuerpo
	pbody->listener = this;

	//Asignacion de tipo de collider
	pbody->ctype = ColliderType::SPIKES; //Se le asigna SPIKES porque cumple su misma funcion

	//Animacion del enemigo
	currentAnimation = &idleAnimation;

	return true;
}

bool Enemy::Update()
{
	bool PlayerIsAlive = app->scene->player->spawn;
	
	if(PlayerIsAlive == false)
	{
		FollowPlayer();
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 10;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10;

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	
	return true;
}

bool Enemy::CleanUp() 
{
	return true;
}

void Enemy::FollowPlayer()
{
	//Encontrar la posicion del jugador
	iPoint playerPosition = app->scene->player->position;

	//Verifica a que posiciones puede andar
	app->pathfinding->IsWalkable(position);

	//Teniendo la posicion del jugador como objetivo 
	app->pathfinding->CreatePath(position, playerPosition);
	
	position.x += 2;
	
}



