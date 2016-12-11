/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __ENEMY_H__
#define __ENEMY_H__

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
#include <time.h>
#include <cstdlib>

using namespace std;

//Enemy Class
class Enemy : public GameObject
{
public:
    Enemy(float, float, float, int);    //constructor for enemy; requires an initial position
    void setAlive(bool);                //set enemy to alive or dead
    bool getAlive();                    //gets if enemy is alive or dead
    void setBottomTrue();               //sets enemy to being bottom unit
    bool isBottomTrue();                //gets if enemy is bottom unit
    bool shouldShoot(int);              //uses random to check if enemy should shoot
    void drawParticle();                //draws enemy's particles for when dead
    void drawShip(Mesh&);               //draws ship
    int getMultipleOfSpeedBeforeCanShoot();     //gets tracker for if enemy can shoot again
    void setMultipleOfSpeedBeforeCanShoot(int); //sets tracker for if enemy can shoot again
    bool isHit(float, float, float);    //whether or not the given coordinates is within enemy's hit box
    float getParticleSpeed();

private:
    bool isAlive;                       //whether the enemy unit is alive
    bool isBottom = false;              //whether or not unit is the bottom one
    int multipleOfSpeedBeforeCanShoot = 0;  //when reaches 5, enemy can shoot projectile again; increases by 1 upon every timer() call
    float particleSpeed = 0.0f;         //particle speed
    float alphaParticle = 1.0f;           //the alpha channel for the particle
};

#endif
