#pragma once

class MotherShipBehaviourComponent : public Component {
    float velocity = ALIEN_SPEED;
    Player *player;

public:
    virtual ~MotherShipBehaviourComponent() {}

    virtual void Create(AvancezLib *engine, GameObject *go,  std::set<GameObject *> *game_objects) {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        //random direction of mother ship
        int random_dir = rand() % 2;
        if (random_dir) {
            go->dir = 1;
        } else {
            go->dir = -1;
        }
        go->horizontalPosition = go->xPos;
        go->verticalPosition = go->yPos;
    }

    virtual void Update(float dt) {
        go->enabled = !(go->horizontalPosition < -32 || go->horizontalPosition > 640);
        Move(dt * velocity);
    }

    void Move(float dt) {
        //makes mother ship change direction if end of world
        if (go->xPos < 0 || go->xPos > 6400 - 32) {
            if (go->xPos < 0 || go->xPos >= 6400 - 32) {
                if (go->dir > 0) {
                    go->dir = -1;
                } else {
                    go->dir = 1;
                }
            }
        }
        //have a set yPos for mother ship
        if (go->verticalPosition > 170) {
            go->verticalPosition = 170;
        }
        go->verticalPosition += ((
                velocity * dt
                * game_speed));
        go->xPos += ((velocity * dt * game_speed * go->dir));
        //makes mother ship stay in position in relation to viewport
        go->horizontalPosition = go->xPos - CAMERA_POSITION;
    }
};


// the mother ship
class MotherShip : public GameObject {
public:

    virtual ~MotherShip() {}

    virtual void Init(float xPos) {
        this->xPos = xPos;
        yPos = 170;
        GameObject::Init();
    }

    virtual void Receive(Message m) {
        if (m == HIT) {
            Send(ALIEN_HIT);
            mother_ship_killed = true;
            dead = true;
            enabled = false;
        }
    }
};
