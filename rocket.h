#pragma once


class RocketBehaviourComponent : public Component {
public:

    void Update(float dt) {
        //change direction of rocket depending on what direction player is shooting at
        if (go->reverse) {
            go->horizontalPosition -= ROCKET_SPEED * dt; // rocket_speed * time
        } else {
            go->horizontalPosition += ROCKET_SPEED * dt;
        }
        //disable rendering and update if outside viewport
        if (go->horizontalPosition >= 640 || go->horizontalPosition <= 0) {
            go->enabled = false;
            go->dead = true;
        }
    }
};


// rockets are shot by the player towards the aliens
class Rocket : public GameObject {

public:
    virtual void Init(double xPos, double yPos, bool direction) {
        GameObject::Init();
        dead = false;
        horizontalPosition = xPos;
        verticalPosition = yPos;
        reverse = direction;
    }

    virtual void Receive(Message m) {
        if (!enabled)
            return;

        if (m == HIT) {
            enabled = false;
            dead = true;
        }
    }
};