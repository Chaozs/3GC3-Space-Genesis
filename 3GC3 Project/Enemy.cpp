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
    row = Row;
    setAlive(true);
}

void Enemy::setSpeed(float Speed)
{
    speed = Speed;
}

void Enemy::setAlive(bool IsAlive)
{
    isAlive = IsAlive;
}

bool Enemy::getAlive()
{
    return isAlive;
}

void Enemy::setBottomTrue()
{
    isBottom=true;
}

bool Enemy::isBottomTrue()
{
    return isBottom;
}

int Enemy::getRow()
{
    return row;
}

bool Enemy::shouldShoot(int range)
{
    if(!isAlive){
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

void Enemy::drawShip()
{
    if(isAlive){
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

int Enemy::getMultipleOfSpeedBeforeCanShoot()
{
    return multipleOfSpeedBeforeCanShoot;
}

void Enemy::setMultipleOfSpeedBeforeCanShoot(int multiple)
{
    multipleOfSpeedBeforeCanShoot = multiple;
}

bool Enemy::isHit(float x, float y, float z)
{
    if(!isAlive){
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

//    if (position[2] < z-1 || position[0] > z+1)
//    {
//        return false;
//    }

    return true;
}
