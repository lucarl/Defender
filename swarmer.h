#pragma once

class SwarmerBehaviourComponent : public Component {
    float velocity = ALIEN_SPEED;
    Player *player;
    float radians;

public:
    virtual ~SwarmerBehaviourComponent() {}

    virtual void
    Create(AvancezLib *engine, GameObject *go, Player *player, std::set<GameObject *> *game_objects) {
        //needs position of player
        this->player = player;
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {}

    virtual void Update(float dt) {
        //disable rendering if outside viewport
        go->enabled = !(go->horizontalPosition < -32 || go->horizontalPosition > 640);
        Move(dt * velocity);
    }

    // param move depends on the time, so the player moves always at the same speed on any computer
    void Move(float dt) {
        float horizontal_position;
        //change y position in regards to what way the player is moving
        if (player->reverse) {
            horizontal_position = player->horizontalPosition + 50;
        } else {
            horizontal_position = player->horizontalPosition - 50;
        }
        //move towards players position
        radians = atan2(player->verticalPosition - go->verticalPosition,
                        horizontal_position - go->horizontalPosition);
        go->xPos += dt * cos(radians) * game_speed;
        //makes swarmer stay in position in relation to viewport
        go->horizontalPosition = go->xPos - CAMERA_POSITION;
    }
};

// the swarmer
class Swarmer : public GameObject {
public:

    virtual ~Swarmer() {}

    virtual void Init(float xPos, float yPos) {
        this->xPos = xPos;
        verticalPosition = yPos;
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
