#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

private:

	//L02: DONE 1: Declare player parameters
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

	bool OnAir = false;
	bool abletoimpulse = false;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	int pickCoinFxId;
};

#endif // __PLAYER_H__