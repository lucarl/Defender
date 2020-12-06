#pragma once

class ScannerBehaviourComponent : public Component {

    GameObject *current;
    float xPos, yPos;

public:
    virtual ~ScannerBehaviourComponent() {}

    //point at current object in order to represents a smaller pixel in the scanner
    virtual void Create(AvancezLib *engine, GameObject *go, GameObject *object) {
        current = object;
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        //change the scanner position in the same way as the entity it's pointing to
        go->verticalPosition = current->verticalPosition;
        go->horizontalPosition = current->horizontalPosition;
    }

    virtual void Update(float dt) {
        Move(dt);
        //remove the scanner if the corresponding enemy dies
        if (current->dead) {
            go->dead = true;
        }
    }

    void Move(float dt) {
        //scale down and move up the position of each scanner to create a scanner view
        go->verticalPosition = current->verticalPosition / 10;
        go->horizontalPosition = current->horizontalPosition / 8 + (640 / 2);
    }
};

// the scanner
class Scanner : public GameObject {
public:

    virtual ~Scanner() {}

    virtual void Init() {
        GameObject::Init();
    }

    virtual void Receive(Message m) {
    }
};
