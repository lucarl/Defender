#pragma once

class BaiterBehaviourComponent : public Component {
    float time_fire_pressed;    // time from the last time the fire button was pressed
    ObjectPool<Bomb> *bomb_pool;
    float velocity = BAITER_SPEED;
    int randomNumber;

    Player *player;
    float radians;

public:
    virtual ~BaiterBehaviourComponent() {}

    virtual void Create(AvancezLib *engine, GameObject *go, Player *player, std::set<GameObject *> *game_objects,
                        ObjectPool<Bomb> *bomb_pool, float x, float y) {
        go->xPos = x;
        go->yPos = y;
        this->player = player;
        Component::Create(engine, go, game_objects);
        this->bomb_pool = bomb_pool;
    }

    virtual void Init() {
        go->horizontalPosition = go->xPos;
        go->verticalPosition = go->yPos;
        time_fire_pressed = -10000.f;
    }

    virtual void Update(float dt) {
        //enable baiter endering if in position of viewport
        go->enabled = !(go->horizontalPosition < -32 || go->horizontalPosition > 640);
        randomNumber = rand() % 100;
        Move(dt * velocity);

        if (CanFire()) {
            if (randomNumber >= 90) {
                // fetches a rocket from the pool and use it in game_objects
                Bomb *bomb = bomb_pool->FirstAvailable();
                if (bomb != NULL)    // rocket is NULL is the object pool can not provide an object
                {
                    //shoot at player position
                    radians = atan2(player->verticalPosition - go->verticalPosition,
                                    player->horizontalPosition - go->horizontalPosition);
                    bomb->Init(go->xPos, go->verticalPosition, radians);
                    game_objects->insert(bomb);
                }
            }
        }
    }

    void Move(float dt) {
        float horizontal_position;

        //keep the x-position at certain value
        if (go->verticalPosition > 180)
            go->verticalPosition = 180;

        //change placement near player depending on what direction the player is going
        if (player->reverse) {
            horizontal_position = player->horizontalPosition - 232;
        } else {
            horizontal_position = player->horizontalPosition + 100;
        }

        //follow players position
        radians = atan2(player->verticalPosition - go->verticalPosition,
                        (horizontal_position + 100) - go->horizontalPosition);
        go->verticalPosition += ((0.1f * dt * game_speed));
        go->xPos += dt * cos(radians) * game_speed;
        //makes baiter stay in position in relation to viewport
        go->horizontalPosition = (go->xPos - CAMERA_POSITION);


    }

    bool CanFire() {
        if ((engine->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed) ||
            (go->horizontalPosition < -32 || go->horizontalPosition > 640))
            return false;
        time_fire_pressed = engine->getElapsedTime();

        return true;
    }
};


// the baiter enemy
class Baiter : public GameObject {
public:

    virtual ~Baiter() {}

    virtual void Init(float xPos) {
        this->xPos = xPos;
        yPos = 32;
        GameObject::Init();
    }

    virtual void Receive(Message m) {
        if (m == HIT) {
            Send(ALIEN_HIT);
            dead = true;
            enabled = false;
        }
    }
};
