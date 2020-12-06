#pragma once


class BombBehaviourComponent : public Component {
public:
    void Update(float dt) {
        go->verticalPosition += BOMB_SPEED * dt * sin(go->radians);
        go->position += BOMB_SPEED * dt * cos(go->radians);
        //makes bomb stay in position in relation to viewport
        go->horizontalPosition = go->position - CAMERA_POSITION;

        //disable rendering and update of object when outside viewport
        if (go->verticalPosition > 550 || go->verticalPosition < 70 || go->horizontalPosition > 640 ||
            go->horizontalPosition < 0) {
            go->enabled = false;
            go->dead = true;
        }
    }
};

// the bomb for aliens and baiters
class Bomb : public GameObject {

public:

    virtual void Init(float xPos, float yPos, float direction) {
        GameObject::Init();
        dead = false;
        position = xPos;
        verticalPosition = yPos;
        radians = direction;
    }

    virtual void Receive(Message m) {
        if (!enabled)
            return;

        //disable render and update when player has been hit
        if (m == HIT) {
            enabled = false;
            dead = true;
        }
    }
};