/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "GameObject.h"

//constructor for game object; requires an initial position
GameObject::GameObject(float x, float y, float z)
{
    //sets initial position
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

//returns object's position in the form of a vector
vector<float> GameObject::getPosition()
{
    vector<float> vPosition(3);
    vPosition[0] = position[0];
    vPosition[1] = position[1];
    vPosition[2] = position[2];
    return vPosition;
}

void GameObject::setMesh(Mesh& m)
{
    mesh = m;
}

Mesh GameObject::getMesh()
{
    return mesh;
}

//moves object's X position by movement
void GameObject::moveX(float movement)
{
    position[0] += movement;
}

//moves object's Y position by movement
void GameObject::moveY(float movement)
{
    position[1] += movement;
}

//moves object's Z position by movement
void GameObject::moveZ(float movement)
{
    position[2] += movement;
}
