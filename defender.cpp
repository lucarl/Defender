#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"

#include <set>



const unsigned int	NUM_LIVES = 2;
const unsigned int	POINTS_PER_ALIEN = 100;

const float			FIRE_TIME_INTERVAL = 150.f;
const float         REVERSE_TIME_INTERVAL = 300.f;
const float			PLAYER_SPEED = 0.8f;
const float			ROCKET_SPEED = 0.5f;
const float			BOMB_SPEED = 0.15f;
const float			ALIEN_SPEED = 0.2f;
const float			BAITER_SPEED = 0.9f;
const float			BOMBER_SPEED = 0.4f;
const float			MUTANT_SPEED = 0.75f;
const float			GRAVITY = 0.1f;
const float			HUMAN_SPEED = 0.1f;
const float         CAMERA_SPEED = 0.4f;

float  CAMERA_POSITION = 0;

float game_speed = 1.f;		// speed of the game



#include "component.h"
#include "game_object.h"
#include "background_object.h"

#include "bomb.h"
#include "mine.h"
#include "rocket.h"
#include "human.h"
#include "player.h"
#include "alien.h"
#include "mother_ship.h"
#include "bomber.h"
#include "baiter.h"
#include "swarmer.h"
#include "scanner.h"
#include "background.h"
#include "game.h"

int main(int argc, char** argv)
{
	AvancezLib engine;

	engine.init(640, 550);

	Game game;
	game.Create(&engine);
	game.Init();

	float lastTime = engine.getElapsedTime();
	while (true)
	{
		float newTime = engine.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		engine.processInput();
		game.Update(dt);
		game.Draw();
	}
}



