/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __MAINMENU_H__
#define __MAINMENU_H__

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

using namespace std;

class MainMenu
{
private:
    void drawBox(std::string text, float position, float highLight);    //draw button box - takes string (button text), the position (for offset from other buttons), and highlight, which is added to glColor to "highlight"
    void drawText(std::string text, float position);                    //draws the text to the button

public:
    MainMenu();                     //constructor
    void draw();                    //draw all buttons and text
    enum ButtonType { Start, Difficulty, HowToPlay, Exit }; //enum for button types
    void goDown();                  //cycle down buttons
    void goUp();                    //cycle up buttons
    ButtonType getCurrentButton();  //get current button hovered
    ButtonType currentHovering;     //the current button being hovered

};

#endif
