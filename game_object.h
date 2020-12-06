#pragma once

// GameObject represents objects which moves are drawn
#include <vector>

enum Message { HIT, ALIEN_HIT, GAME_OVER, HUMAN_HIT, MOTHER_SHIP_HIT, LEVEL_WIN, NO_MSG, QUIT, CHANGE };

class Component;

class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

public:
	//global position for entities
	float xPos;
	float yPos;
	//local position for entities
    float horizontalPosition;
	float verticalPosition;
	float position;
	float radians;
    float dir;

	bool enabled;
	bool abducted = false;
	bool dead;
	bool mutate;
	bool player;
	bool human;
    bool falling;
    bool saved;
	bool reverse;
	bool mother_ship_killed;

	virtual ~GameObject();
	virtual void Create();
	virtual void AddComponent(Component * component);
	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);
};