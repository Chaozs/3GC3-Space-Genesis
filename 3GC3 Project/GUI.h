/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __GUI_H__
#define __GUI_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <utility>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string> 

//this class prints out the player score and HP
class GUI
{
public:
    GUI();
    void incScoreBy(int);		//increase current score by input
    void drawScoreAndHP(int);	//draws the score and HP given by input
    void drawPause();
    void drawDifficulty(std::string);

private:
    int score;					//the current score
};

#endif
