#pragma once

class BomberBehaviourComponent : public Component {
    float time_fire_pressed;    // time from the last time the fire button was pressed
    ObjectPool<Mine> *mines_pool;
    bool up;
    float velocity = BOMBER_SPEED;
    int randomNumber;

public:
    virtual ~BomberBehaviourComponent() {}

    virtual void Create(AvancezLib *engine, GameObject *go, std::set<GameObject *> *game_objects,
                        ObjectPool<Mine> *mines_pool) {
        Component::Create(engine, go, game_objects);
        this->mines_pool = mines_pool;
    }

    virtual void Init() {
        //random direction of bomber on init
        int random_dir = rand() % 2;
        if (random_dir) {
            go->dir = 1;
        } else {
            go->dir = -1;
        }
        go->horizontalPosition = go->xPos;
        go->verticalPosition = go->yPos;
        time_fire_pressed = -10000.f;
    }

    virtual void Update(float dt) {
        //enable rendering if in viewport
        go->enabled = !(go->horizontalPosition < -32 || go->horizontalPosition > 640);
        randomNumber = rand() % 100;
        Move(dt * velocity);
        if (CanFire()) {
            if (randomNumber >= 90) {
                // fetches a mine from the pool and use it in game_objects
                Mine *mine = mines_pool->FirstAvailable();
                if (mine != NULL)    // mine is NULL is the object pool can not provide an object
                {
                    mine->Init(go->xPos, go->verticalPosition);
                    game_objects->insert(mine);
                }
            }
        }
    }

    void Move(float dt) {
        //change direction if it goes to the end of the world
        if (go->xPos < 0 || go->xPos > 6400 - 32) {
            if (go->xPos < 0 || go->xPos >= 6400 - 32) {
                if (go->dir > 0) {
                    go->dir = -1;
                } else {
                    go->dir = 1;
                }
            }
        }
        //create the moving pattern
        if (go->verticalPosition >= 350) {
            up = true;
        }
        if (go->verticalPosition <= 300) {
            up = false;
        }
        if (up) {
            go->verticalPosition -= ((velocity * dt * game_speed));
            go->xPos += ((velocity * dt * game_speed * go->dir));
            go->horizontalPosition = go->xPos - CAMERA_POSITION;
        } else {
            go->verticalPosition += ((velocity * dt * game_speed));
            go->xPos += ((velocity * dt * game_speed * go->dir));
            //makes bomber stay in position in relation to viewport
            go->horizontalPosition = go->xPos - CAMERA_POSITION;
        }
    }

    bool CanFire() {
        if ((engine->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed) ||
            (go->horizontalPosition < -32 || go->horizontalPosition > 640))
            return false;

        time_fire_pressed = engine->getElapsedTime();

        return true;
    }
};


// the main bomber enemy
class Bomber : public GameObject {
public:

    virtual ~Bomber() {}

    virtual void Init(float xPos) {
        this->xPos = xPos;
        yPos = 325;
        GameObject::Init();
    }

    virtual void Receive(Message m) {
        if (m == HIT) {
            Send(ALIEN_HIT);
            enabled = false;
            dead = true;
        }
    }
};
