#pragma once

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();

	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	SDL_Texture* texture;
	const char* texturePath;

	Animation* currentAnimation;
	Animation idleAnimation;
	Animation leftidleAnimation;
	Animation rightAnimation;
	Animation leftAnimation;
	Animation jumpAnimation;
	Animation jumpleftAnimation;
	Animation punchAnimation;
	Animation punchleftAnimation;
	Animation winAnimation;

	PhysBody* pbody;
	iPoint position;

	iPoint enemyPos;
	iPoint playerPos;

	int directionX = 1;
	int directionY = 1;
	float speed = 0.3;
	bool flying = false;
	int range;
	bool enemySpawn = false;

	enum estado
	{
		IDLE,
		CHASE,
		DEAD
	};

	int state = 1;

};

#endif // __PLAYER_H__
