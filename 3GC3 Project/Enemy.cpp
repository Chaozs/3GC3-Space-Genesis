/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Enemy.h"

//constructor for enemy; requires an initial position
Enemy::Enemy(float x, float y, float z, int Row, int Column) : GameObject(x,y,z)
{
	row = Row;
	column = Column;
}

void Enemy::setSpeed(float Speed){
	speed = Speed;
}

void Enemy::setAlive(bool IsAlive){
	isAlive = IsAlive;
}

bool Enemy::getAlive(){
	return isAlive;
}

void Enemy::setBottomTrue(){
	isBottom=true;
}

bool Enemy::isBottomTrue(){
	return isBottom;
}

int Enemy::getRow(){
	return row;
}

int Enemy::getColumn(){
	return column;
}