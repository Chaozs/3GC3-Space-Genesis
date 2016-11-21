/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Player.h"

//constructor for player; requires an initial position
Player::Player(float positionX, float positionY, float positionZ)
{
    hp = 100;   //intially set hp of the player to 100
}

int getHp()
{
    return hp;
}

//decreases hp of player by the specified amount
void decreaseHp(int damage)
{
    hp -= damage;
}
