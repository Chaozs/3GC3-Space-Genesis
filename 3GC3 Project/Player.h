/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __PLAYER_H__
#define __PLAYER_H__

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

class Player : public GameObject
{
    public:
        Player(float x, float y, float z) : GameObject(x, y, z){hp = 100;}    //constructor for player; requires an initial position
        int getHp();                    //returns hp of the player
        void decreaseHp(int);           //decreases the hp of the player by the specified amount

    private:
        int hp;                         //hp of the player
};

#endif
