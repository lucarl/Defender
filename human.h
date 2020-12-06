#pragma once

class HumanBehaviourComponent : public Component {
    float velocity = HUMAN_SPEED;
    float abductPos;

public:
    virtual ~HumanBehaviourComponent() {}

    virtual void Create(AvancezLib *engine, GameObject *go, std::set<GameObject *> *game_objects) {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        int randomDir = rand() % 2;
        if (randomDir) {
            go->dir = 1;
        } else {
            go->dir = -1;
        }
        go->horizontalPosition = go->xPos;
        go->verticalPosition = go->yPos;
    }

    virtual void Update(float dt) {
        //disable render if outside viewport
        go->enabled = !(go->horizontalPosition < -32 || go->horizontalPosition > 640);
        //check if alien has been killed and start falling
        if (go->abducted && abductPos == go->verticalPosition && !go->saved) {
            go->horizontalPosition = go->xPos - CAMERA_POSITION;
            go->verticalPosition += GRAVITY * dt * game_speed;
            go->falling = true;
            //check if human is on the ground
            if (go->verticalPosition > 470)
                go->abducted = false;
        }
        //if not abducted, move human on the ground
        if (!go->abducted)
            Move(dt * velocity);

        abductPos = go->verticalPosition;
    }

    // param move depends on the time, so the player moves always at the same speed on any computer
    void Move(float dt) {
        //check if human has reached end of the world and change direction
        if (go->xPos < 0 || go->xPos > 6400 - 32) {
            if (go->xPos <= 0 || go->xPos >= 6400 - 32) {
                if (go->dir > 0) {
                    go->dir = -1;
                } else {
                    go->dir = 1;
                }
            }
        }
        //walking of human
        go->xPos += ((velocity * dt * game_speed * go->dir));
        //makes human stay in position in relation to viewport
        go->horizontalPosition = go->xPos - CAMERA_POSITION;
    }
};


// the human
class Human : public GameObject {
public:

    virtual ~Human() {}

    virtual void Init(float xPos) {
        this->xPos = xPos;
        yPos = 470;
        GameObject::Init();
    }

    virtual void Receive(Message m) {
        if (m == HIT) {
            Send(HUMAN_HIT);
            dead = true;
        }
    }
};
