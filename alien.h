#pragma once

class AlienBehaviourComponent : public Component {
    ObjectPool<Bomb> *bomb_pool;
    ObjectPool<Human> *humans_pool;
    Human *abductee;
    Player *player;

    float time_fire_pressed;    // time from the last time the fire button was pressed
    bool abduct;
    bool mutant;
    bool abducted;
    float velocity = ALIEN_SPEED;
    float mutant_velocity = MUTANT_SPEED;
    int randomNumber;
    int random_placement;
    float radians;

public:
    virtual ~AlienBehaviourComponent() {}

    virtual void Create(AvancezLib *engine, GameObject *go, Player *player, std::set<GameObject *> *game_objects,
                        ObjectPool<Bomb> *bomb_pool, ObjectPool<Human> *humans_pool) {
        //uses position of player and humans, and shooting bombs from the pool
        this->player = player;
        Component::Create(engine, go, game_objects);
        this->bomb_pool = bomb_pool;
        this->humans_pool = humans_pool;
    }

    virtual void Init() {
        //init with different positioning and direction
        int random_dir = rand() % 2;
        random_placement = rand() % 2;
        if (random_dir) {
            go->dir = 1;
        } else {
            go->dir = -1;
        }
        go->verticalPosition = go->yPos;
        time_fire_pressed = -10000.f;
    }

    virtual void Update(float dt) {
        //enable alien rendering if in position of cameras viewport
        go->enabled = !(go->horizontalPosition < -32 || go->horizontalPosition > 640);
        //check if alien is above human and start abduction of human
        for (auto human = humans_pool->pool.begin(); human != humans_pool->pool.end(); human++) {
            if (go->xPos > (*human)->xPos - 1 && go->xPos < (*human)->xPos + 1 && !(*human)->abducted) {
                abductee = (*human);
                abduct = true;
            }
        }
        randomNumber = rand() % 100;
        //moves differently depending on if the alien has mutated or not
        if (!mutant) {
            Move(dt * velocity);
        } else {
            MoveMutant(dt * velocity);
        }
        if (CanFire()) {
            //shoot at random intervals
            if (randomNumber >= 90) {
                // fetches a bomb from the pool and use it in game_objects
                Bomb *bomb = bomb_pool->FirstAvailable();
                if (bomb != NULL)    // bomb is NULL is the object pool can not provide an object
                {
                    //shoot at the position of the player
                    radians = atan2(player->verticalPosition - go->verticalPosition,
                                    player->horizontalPosition - go->horizontalPosition);
                    bomb->Init(go->xPos, go->verticalPosition, radians);
                    game_objects->insert(bomb);
                }
            }
        }
    }

    //movement of the alien
    void Move(float dt) {
        //change direction if moving to the end of the world
        if (go->xPos < 0 || go->xPos > 6400 - 32) {
            if (go->xPos < 0 || go->xPos >= 6400 - 32) {
                if (go->dir > 0) {
                    go->dir = -1;
                } else {
                    go->dir = 1;
                }
            }
        }
        //follows a human if the alien is above it
        if (abduct) {
            //check if alien is at humans position
            if (go->verticalPosition > abductee->verticalPosition - 32) {
                go->verticalPosition = abductee->verticalPosition - 32;
                abducted = true;
            }
            //check if human has been brought to the top of the screen and mutate
            if (abducted) {
                abductee->abducted = true;
                go->verticalPosition -= ((velocity * dt * game_speed));
                go->horizontalPosition = go->xPos - CAMERA_POSITION;
                abductee->verticalPosition -= ((velocity * dt * game_speed));
                abductee->horizontalPosition = go->xPos - CAMERA_POSITION;
                if (go->verticalPosition < 70) {
                    abductee->dead = true;
                    mutant = true;
                    go->mutate = true;
                }
                //follow the humans position
            } else {
                radians = atan2(abductee->verticalPosition - go->verticalPosition,
                                abductee->horizontalPosition - go->horizontalPosition);
                go->verticalPosition += ((velocity * dt * game_speed));
                go->xPos += dt * cos(radians) * game_speed;
                go->horizontalPosition = go->xPos - CAMERA_POSITION;
            }
            //moving the alien across the world in a fixed position
        } else {
            if (go->verticalPosition > 350) {
                go->verticalPosition = 350;
            }
            go->verticalPosition += ((velocity * dt * game_speed));
            go->xPos += ((velocity * dt * game_speed * go->dir));
            go->horizontalPosition = go->xPos - CAMERA_POSITION;
        }
    }

    //movement of mutant
    void MoveMutant(float dt) {
        //random placement of mutant
        if (random_placement) {
            if (go->verticalPosition > 100) {
                go->verticalPosition = 100;
            }
        } else {
            if (go->verticalPosition > 380) {
                go->verticalPosition = 380;
            }
        }
        //move mutant to players position
        radians = atan2(player->verticalPosition - go->verticalPosition,
                        player->horizontalPosition - go->horizontalPosition);
        go->verticalPosition += ((mutant_velocity * dt * game_speed));
        go->xPos += dt * cos(radians) * game_speed;
        //makes alien stay in position in relation to viewport
        go->horizontalPosition = go->xPos - CAMERA_POSITION;
    }

    bool CanFire() {
        if ((engine->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed) ||
            (go->horizontalPosition < -32 || go->horizontalPosition > 640))
            return false;
        time_fire_pressed = engine->getElapsedTime();
        return true;
    }
};


// the alien enemy
class Alien : public GameObject {
public:

    virtual ~Alien() {}

    virtual void Init(float xPos) {
        this->xPos = xPos;
        this->yPos = 102;
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
