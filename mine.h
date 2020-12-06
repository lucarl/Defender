#pragma once


class MineBehaviourComponent : public Component
{
public:
    void Update(float dt)
    {
        //makes mine stay in position in relation to viewport
        go->horizontalPosition = go->position - CAMERA_POSITION;
    }
};

class Mine : public GameObject
{

public:

    virtual void Init(float xPos, float yPos)
    {
        GameObject::Init();
        dead = false;
        position = xPos;
        verticalPosition = yPos;
    }

    virtual void Receive(Message m)
    {
        if (!enabled)
            return;

        if (m == HIT)
        {
            enabled = false;
            dead = true;
        }
    }
};