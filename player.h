#pragma once

#include "background.h"

class PlayerBehaviourComponent : public Component {
    float time_fire_pressed;    // time from the last time the fire button was pressed
    float time_hit;
    float time_reverse_pressed;
    ObjectPool<Rocket> *rockets_pool;
    ObjectPool<Human> *humans_pool;
    Human *victim;
    float thrust = 0;
    float move = 0.5;
    bool move_camera_left;
    bool move_camera_right;

public:
    virtual ~PlayerBehaviourComponent() {}

    //needs humans position in order to grab it, did not have time to finish this implementation
    virtual void
    Create(AvancezLib *engine, GameObject *go, std::set<GameObject *> *game_objects, ObjectPool<Rocket> *rockets_pool,
           ObjectPool<Human> *humans_pool) {
        Component::Create(engine, go, game_objects);
        this->rockets_pool = rockets_pool;
        this->humans_pool = humans_pool;
    }

    virtual void Init() {
        go->horizontalPosition = 100;
        go->verticalPosition = 240;
        //set global position to local position
        go->xPos = go->horizontalPosition;
        go->yPos = go->verticalPosition;
        thrust = 0;
        time_fire_pressed = -10000.f;
        time_reverse_pressed = -10000.f;
    }

    virtual void Update(float dt) {
        //not done(used to move position of player)
        /*if (go->saved) {
            victim->horizontalPosition = 100;
        }*/
        AvancezLib::KeyStatus keys;
        //change the viewport in regards to players position
        go->xPos = CAMERA_POSITION + go->horizontalPosition;
        go->yPos = go->verticalPosition;
        engine->getKeyStatus(keys);
        //the mapping for the key presses
        if (keys.up)
            Move(-dt * move);
        if (keys.down)
            Move(dt * move);
        if (keys.thrust)
            Thrust(dt * thrust, dt);
        if (!keys.thrust) {
            Idle(dt * thrust);
        }
        if (keys.fire) {
            if (CanFire()) {
                // fetches a rocket from the pool and use it in game_objects
                Rocket *rocket = rockets_pool->FirstAvailable();
                if (rocket != NULL)    // rocket is NULL is the object pool can not provide an object
                {
                    rocket->Init(go->horizontalPosition, go->verticalPosition, go->reverse);
                    game_objects->insert(rocket);
                }
            }
        }
        moveCamera(dt);
        //reverse to movement of player
        if (keys.reverse) {
            if (CanReverse()) {
                go->reverse = !go->reverse;
            }
        }
        //checking if human has been saved(not done)
        for (auto human = humans_pool->pool.begin(); human != humans_pool->pool.end(); human++) {
            if ((*human)->falling && go->horizontalPosition > (*human)->horizontalPosition - 50 &&
                go->horizontalPosition<(*human)->horizontalPosition + 50 && go->verticalPosition>(
                        *human)->verticalPosition - 50 &&
                go->verticalPosition < (*human)->verticalPosition + 50
                    ) {
                victim->saved = true;
            }
        }
        //makes player stay in position of the world
        float temp_pos = go->horizontalPosition;
        if (go->xPos <= 0) {
            CAMERA_POSITION = go->xPos - go->horizontalPosition;
            go->horizontalPosition += 5;
            thrust = 0;
        }
        if (go->xPos >= 6400) {
            CAMERA_POSITION = go->xPos - go->horizontalPosition;
            go->horizontalPosition -= 5;
            thrust = 0;
        }
    }

    //moving the camera in regards to the players movement
    void moveCamera(float dt) {
        if (go->reverse) {
            move_camera_right = false;
            move_camera_left = true;
            go->horizontalPosition += dt * CAMERA_SPEED;
            if (go->horizontalPosition > 540) {
                move_camera_left = false;
                go->horizontalPosition = 540;
            }
        }
        if (!go->reverse) {
            go->horizontalPosition -= dt * CAMERA_SPEED;
            move_camera_left = false;
            move_camera_right = true;
            if (go->horizontalPosition < 100) {
                move_camera_right = false;
                go->horizontalPosition = 100;
            }
        }
        //create an illusion that the player increases/decreases thrust
        if (move_camera_left)
            CAMERA_POSITION -= dt * CAMERA_SPEED - thrust;
        if (move_camera_right)
            CAMERA_POSITION += dt * CAMERA_SPEED + thrust;
    }

    //gradually makes the ship go faster until it reaches the speed limit
    void Thrust(float speed, float dt) {
        if (thrust > PLAYER_SPEED) {
            thrust = PLAYER_SPEED;
        }
        if (thrust < -PLAYER_SPEED) {
            thrust = -PLAYER_SPEED;
        }
        if (go->reverse) {
            thrust -= 0.005;
            if (go->horizontalPosition <= 540 && go->horizontalPosition >= 490) {
                go->horizontalPosition -= dt * 0.45f;
                float tempPos = go->horizontalPosition;
                if (go->horizontalPosition <= 490) {
                    go->horizontalPosition = tempPos;
                }
            }
        } else if (!go->reverse) {
            thrust += 0.005;
            if (go->horizontalPosition >= 100 && go->horizontalPosition <= 150) {
                go->horizontalPosition += dt * 0.45f;

                float tempPos = go->horizontalPosition;
                if (go->horizontalPosition >= 150) {
                    go->horizontalPosition = tempPos;
                }
            }
        }
        CAMERA_POSITION += speed;
    }

    //slow down ship if thrust is not pressed
    void Idle(float speed) {
        if ((go->reverse && speed < 0) || (!go->reverse && speed < 0)) {

            thrust += 0.002;
            if (thrust > 0) {
                thrust = 0;
            }
        }
        if ((go->reverse && speed > 0) || (!go->reverse && speed > 0)) {
            thrust -= 0.002;
            if (thrust < 0) {
                thrust = 0;
            }
        }
        CAMERA_POSITION += speed;
    }

    // move the player up or down
    // param move depends on the time, so the player moves always at the same speed on any computer
    void Move(float move) {
        go->verticalPosition += move;
        if (go->verticalPosition > (550 - 32))
            go->verticalPosition = 550 - 32;

        if (go->verticalPosition < 70)
            go->verticalPosition = 70;
    }

    // return true if enough time has passed from the previous rocket
    bool CanFire() {
        // shoot just if enough time passed by
        if ((engine->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed))
            return false;

        time_fire_pressed = engine->getElapsedTime();

        return true;
    }

    bool CanReverse() {
        // reverse just if enough time passed by
        if ((engine->getElapsedTime() - time_reverse_pressed) < (REVERSE_TIME_INTERVAL / game_speed))
            return false;

        time_reverse_pressed = engine->getElapsedTime();

        return true;
    }
};

// the main player
class Player : public GameObject {
public:

    int lives;    // it's game over when goes below zero

    virtual ~Player() {}

    virtual void Init() {
        GameObject::Init();
        lives = NUM_LIVES;
    }

    virtual void Receive(Message m) {
        if (m == HIT) {
            Send(HIT);
            RemoveLife();

            if (lives < 0)
                Send(GAME_OVER);
        }
    }

    void RemoveLife() {
        lives--;
    }
};
