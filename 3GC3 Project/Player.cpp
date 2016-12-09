/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Player.h"


/*
//constructor for player; requires an initial position
Player::Player(float x, float y, float z)
{
    //sets initial position
    position[0] = x;
    position[1] = y;
    position[2] = z;
    hp = 100;   //intially set hp of the player to 100
}*/

int Player::getHp()
{
    return hp;
}

//decreases hp of player by the specified amount
void Player::decreaseHp(int damage)
{
    hp -= damage;
}
