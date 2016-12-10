/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Projectile.h"

#include <iostream>

//constructor for projectile; requires an initial position
Projectile::Projectile(float x, float y, float z) : GameObject(x, y, z)
{
}

//draw projectile
void Projectile::draw()
{
    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]); //move projectile to its position
    glScalef(0.33, 1, 0.3);  //scale projectile to slim and tall
    glutSolidCube(0.5);
    glPopMatrix();
}
