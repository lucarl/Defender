#pragma once

class BackgroundBehaviourComponent : public Component {

public:
    virtual ~BackgroundBehaviourComponent() {}

    virtual void
    Create(AvancezLib *engine, GameObject *go, std::set<GameObject *> *game_objects) {
        Component::Create(engine, go, game_objects);
    }

    virtual void Init() {
        go->horizontalPosition = 0;
        go->verticalPosition = 70;
    }

    virtual void Update(float dt) {
        MoveCamera();
    }

    // moving the background in relation to the position of the camera
    void MoveCamera() {
        go->horizontalPosition = -CAMERA_POSITION;
    }
};

// the background
class Background : public GameObject {
public:

    virtual ~Background() {}

    virtual void Init() {
        GameObject::Init();
    }

    virtual void Receive(Message m) {
    }
};
