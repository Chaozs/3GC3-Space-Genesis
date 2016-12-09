/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <vector>

#include "Mesh.h"
#include "Vector3.h"

using namespace std;

//TODO: *hold mesh of object (+getter/setter for mesh)
//      *detect collision

class GameObject
{
public:
    GameObject(float, float, float);    //constructor for game object; requires an initial position
    vector<float> getPosition();        //returns object's position
    void moveX(float);                  //moves object's X position; positive float = increases X position & vice versa
    void moveY(float);                  //moves object's Y position; positive float = increases Y position & vice versa
    void moveZ(float);                  //moves object's Z position; positive float = increases Z position & vice versa
    void SetMesh(Mesh& m);
    Mesh getMesh();

protected:
    float position[3];                  //position of object
    Mesh mesh;
};

#endif
