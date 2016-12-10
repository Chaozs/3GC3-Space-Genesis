/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Player.h"

//constructor for player; requires an initial position
Player::Player(float x, float y, float z) : GameObject(x, y, z)
{
    hp = 100;   //intially set hp of the player to 100
}

int Player::getHp()
{
    return hp;
}

void Player::setHp(int h){
    hp = h;
}

//decreases hp of player by the specified amount
void Player::decreaseHp(int damage)
{
    hp -= damage;
}

//draw player ship
void Player::drawShip()
{
	glPushMatrix();
            glTranslatef(position[0], position[1], position[2]);
            	glPushMatrix();
            		glScalef(0.3f, 0.3f, 0.3f);
            		glPushMatrix();
            			glRotatef(50,1,0,0);
            			mesh.Draw();
            		glPopMatrix();
            	glPopMatrix();
    glPopMatrix();
}

//checks if player is hit
bool Player::isHit(float x, float y, float z)
{
    if (position[0] < x-0.8 || position[0] > x+0.8)
    {
        return false;
    }

    if (position[1] < y-1.3 || position[1] > y+1.3)
    {
        return false;
    }
    return true;
}
