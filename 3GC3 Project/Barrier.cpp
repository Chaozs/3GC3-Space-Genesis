/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Barrier.h"

//constructor for barrier; requires an initial position
Barrier::Barrier(float positionX, float positionY, float positionZ)
{
    hp = 4;     //intially set hp of barrier to max hp (which is 4)
}

int getHp()
{
    return hp;
}

//decreases hp of barrier by 1
void decreaseHp()
{
    hp--;
}
