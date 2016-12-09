/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Enemy.h"

//constructor for enemy; requires an initial position
Enemy::Enemy(float x, float y, float z, int Row) : GameObject(x,y,z)
{
    row = Row;
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
    srand (time(NULL));
    int randomInt = rand() % range + 1;
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
