/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Enemy.h"
#include <iostream>

//constructor for enemy; requires an initial position
Enemy::Enemy(float x, float y, float z, int Row) : GameObject(x,y,z)
{
    setAlive(true);
}

//set whether enemy is currently alive
void Enemy::setAlive(bool IsAlive)
{
    isAlive = IsAlive;
}

//get whether enemy is currently alive
bool Enemy::getAlive()
{
    return isAlive;
}

//set unit to be bottom (only bottom enemy unit can shoot)
void Enemy::setBottomTrue()
{
    isBottom = true;
}

//gets if unit is bottom
bool Enemy::isBottomTrue()
{
    return isBottom;
}

//method that uses random to determine if unit should shoot
bool Enemy::shouldShoot(int range)
{
    if(!isAlive)
    {
        return false;
    }
#define uniform() (rand()/(RAND_MAX + 1.0))
    int randomInt = (int) (uniform() * (range+1));

    //cout << randomInt << endl;
    if(randomInt==1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//draws particles. Particles are hidden behind ship until ship dies
void Enemy::drawParticle()
{

    glColor4f(1.0, 1.0, 1.0, alphaParticle);
    glBegin(GL_QUADS);

    float partSize = 0.1f;

    //up
    glVertex3f(position[0]-partSize*0.7, position[1]-partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize*0.7, position[1]-partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize*0.7, position[1]+partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]-partSize*0.7, position[1]+partSize+particleSpeed*0.7, position[2]);

    //up-right
    glVertex3f(position[0]-partSize+particleSpeed*0.7, position[1]-partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize+particleSpeed*0.7, position[1]-partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize+particleSpeed*0.7, position[1]+partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]-partSize+particleSpeed*0.7, position[1]+partSize+particleSpeed*0.7, position[2]);

    //right
    glVertex3f(position[0]-partSize+particleSpeed, position[1]-partSize, position[2]);
    glVertex3f(position[0]+partSize+particleSpeed, position[1]-partSize, position[2]);
    glVertex3f(position[0]+partSize+particleSpeed, position[1]+partSize, position[2]);
    glVertex3f(position[0]-partSize+particleSpeed, position[1]+partSize, position[2]);

    //down-right
    glVertex3f(position[0]-partSize+particleSpeed*0.7, position[1]-partSize-particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize+particleSpeed*0.7, position[1]-partSize-particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize+particleSpeed*0.7, position[1]+partSize-particleSpeed*0.7, position[2]);
    glVertex3f(position[0]-partSize+particleSpeed*0.7, position[1]+partSize-particleSpeed*0.7, position[2]);

    //down
    glVertex3f(position[0]-partSize, position[1]-partSize-particleSpeed, position[2]);
    glVertex3f(position[0]+partSize, position[1]-partSize-particleSpeed, position[2]);
    glVertex3f(position[0]+partSize, position[1]+partSize-particleSpeed, position[2]);
    glVertex3f(position[0]-partSize, position[1]+partSize-particleSpeed, position[2]);

    //down-left
    glVertex3f(position[0]-partSize-particleSpeed*0.7, position[1]-partSize-particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize-particleSpeed*0.7, position[1]-partSize-particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize-particleSpeed*0.7, position[1]+partSize-particleSpeed*0.7, position[2]);
    glVertex3f(position[0]-partSize-particleSpeed*0.7, position[1]+partSize-particleSpeed*0.7, position[2]);

    //left
    glVertex3f(position[0]-partSize-particleSpeed, position[1]-partSize, position[2]);
    glVertex3f(position[0]+partSize-particleSpeed, position[1]-partSize, position[2]);
    glVertex3f(position[0]+partSize-particleSpeed, position[1]+partSize, position[2]);
    glVertex3f(position[0]-partSize-particleSpeed, position[1]+partSize, position[2]);

    //up-left
    glVertex3f(position[0]-partSize-particleSpeed*0.7, position[1]-partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize-particleSpeed*0.7, position[1]-partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]+partSize-particleSpeed*0.7, position[1]+partSize+particleSpeed*0.7, position[2]);
    glVertex3f(position[0]-partSize-particleSpeed*0.7, position[1]+partSize+particleSpeed*0.7, position[2]);

    glColor4f(1.0, 1.0, 1.0, 1.0);
    glEnd();
}

float Enemy::getParticleSpeed()
{
    return particleSpeed;
}

//draws ship
void Enemy::drawShip(Mesh& mesh)
{
    if((!isAlive)&&(particleSpeed<1.0f))
    {
        particleSpeed = particleSpeed + 0.02f;
        alphaParticle = alphaParticle - 0.02f;
    }
    //only draw particle until it reaches that point
    if(particleSpeed<1.0f)
    {
        drawParticle();
    }
    if(isAlive)
    {
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glPushMatrix();
        glScalef(0.15f, 0.15f, 0.15f);
        glPushMatrix();
        glRotatef(180, 0, 1, 1);
        mesh.Draw();
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }
}

//get counter for tracking if enemy can shoot again
int Enemy::getMultipleOfSpeedBeforeCanShoot()
{
    return multipleOfSpeedBeforeCanShoot;
}

//set counter for tracking if enemy can shoot again
void Enemy::setMultipleOfSpeedBeforeCanShoot(int multiple)
{
    multipleOfSpeedBeforeCanShoot = multiple;
}

//hit detection for if enemy was hit by projectile
bool Enemy::isHit(float x, float y, float z)
{
    if(!isAlive)
    {
        return false;
    }
    if (position[0] < x-0.5 || position[0] > x+0.5)
    {
        return false;
    }
    if (position[1] < y-1 || position[1] > y+1)
    {
        return false;
    }
    return true;
}
