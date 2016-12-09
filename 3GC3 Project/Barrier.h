/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __BARRIER_H__
#define __BARRIER_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "GameObject.h"

using namespace std;

class Barrier : public GameObject
{
public:
    Barrier(float, float, float);   //constructor for barrier; requires an initial position
    int getHp();                    //returns hp of the barrier
    void decreaseHp();              //decreases the barrier's hp by 1

private:
    int hp;
};

#endif
