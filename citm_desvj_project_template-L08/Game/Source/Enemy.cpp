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
#include "Map.h"


Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");

	//Right animation
	rightAnimation.PushBack({ 377,122,32,25 });
	rightAnimation.PushBack({ 347,121,29,25 });
	rightAnimation.PushBack({ 314,122,30,26 });
	rightAnimation.PushBack({ 281,122,32,26 });
	rightAnimation.PushBack({ 249,120,32,25 });
	rightAnimation.PushBack({ 217,122,32,25 });
	rightAnimation.speed = 0.12f;

	//Idle animation
	leftAnimation.PushBack({ 217,26,32,24 });
	leftAnimation.PushBack({ 249,26,33,24 });
	idleAnimation.speed = 0.05f;

	//left animation
	leftAnimation.PushBack({ 217,26,32,24 });
	leftAnimation.PushBack({ 251,55,29,26 });
	leftAnimation.PushBack({ 282,57,30,26 });
	leftAnimation.PushBack({ 312,57,34,26 });
	leftAnimation.PushBack({ 345,56,33,25 });
	leftAnimation.PushBack({ 377,58,32,25 });
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
	leftidleAnimation.PushBack({ 345,56,33,25 });
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
	flying = parameters.attribute("flying").as_bool();
	speed = parameters.attribute("speed").as_float();
	range = parameters.attribute("range").as_int();
	
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
	pbody->ctype = ColliderType::ENEMY; //Se le asigna SPIKES porque cumple su misma funcion

	//Asignar si el enemigo es volador
	if (flying) pbody->body->SetGravityScale(0);
	else pbody->body->SetGravityScale(-GRAVITY_Y);

	//Animacion del enemigo
	currentAnimation = &idleAnimation;

	return true;
}

bool Enemy::Update()
{
	bool PlayerIsAlive = app->scene->player->spawn;

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 10;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10;

	enemyPos = app->map->WorldToMap(position.x, position.y);
	playerPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	app->pathfinding->CreatePath(enemyPos, playerPos, flying);

	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	switch (state)
	{
	case IDLE:

		
		if (flying) pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		else 
		{
			pbody->body->SetLinearVelocity(b2Vec2(directionX * speed, 0));
			//if (!app->pathfinding->IsWalkable(iPoint(enemyPos.x + directionX, enemyPos.y), flying))(directionX *= -1); ;
		}

		//Calcular la distancia al jugador, si esta cerca cambia a CHASE
		if (position.DistanceTo(app->scene->player->position) < range) state = Enemy::CHASE;

		break;

	case CHASE:

		//Si el jugador se acerca deja de seguirlo y cambia a IDLE
		if (position.DistanceTo(app->scene->player->position) > range) state = Enemy::IDLE;

		//Algoritmo del enemigo para seguir al jugador
		if (app->pathfinding->GetLastPath()->Count() > 0)
		{
			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

			//Calcula el siguiente tile al que puede caminar
			iPoint pos = app->map->MapToWorld(path->At(1)->x, path->At(1)->y);

			//El enemigo se moverá hacia la derecha
			if (pos.x > position.x)
			{
				directionX = 1;
				currentAnimation = &rightAnimation;
			}

			//El enemigo se moverá hacia la izquierda
			if (pos.x < position.x)
			{
				directionX = -1;
				currentAnimation = &leftAnimation;
			}
		}


		pbody->body->SetLinearVelocity(b2Vec2(directionX * 2 * speed, -GRAVITY_Y));
		break;

	case DEAD:
		break;
	}
	

	

	return true;
}

bool Enemy::CleanUp() 
{
	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:

		LOG("Collision ITEM");
		break;

	case ColliderType::PLATFORM:

		LOG("Collision PLATFORM");
		break;

	case ColliderType::WALL:

		LOG("Collision WALL");

		if (directionX == 1)
		{
			directionX = -1;
			currentAnimation = &leftAnimation;
		}
		else 
		{
			directionX = 1;
			currentAnimation = &rightAnimation;
		}

		break;

	case ColliderType::SPIKES:

		LOG("Collision SPIKES");
		break;

	case ColliderType::UNKNOWN:

		LOG("Collision UNKNOWN");
		break;

	case ColliderType::WIN:

		LOG("Collision WIN");
		break;

	case ColliderType::ENEMY:

		LOG("Collision SPIKES");
		break;

	}
}



