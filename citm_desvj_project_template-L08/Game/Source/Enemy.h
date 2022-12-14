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

	void FollowPlayer();

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
	int pickCoinFxId;
};

#endif // __PLAYER_H__
