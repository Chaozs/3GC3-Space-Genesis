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
public:
    MainMenu();                     //constructor
    void drawMenu();                //draw all buttons and text
    void drawDifficulty();          //draws the difficulty menu
    void drawGameOver();            //draws the gameover screen
    void drawWin();                 //drwas the win screen
    enum ButtonType { Item1, Item2, Item3, Item4 }; //enum for button types
    void goDown();                  //cycle down buttons
    void goUp();                    //cycle up buttons
    ButtonType getCurrentButton();  //get current button hovered

private:
    void drawBox(std::string, float, float);//draw button box - takes string (button text), the position (for offset from other buttons), and highlight, which is added to glColor to "highlight"
    void drawText(std::string, float);      //draws the text to the button
    ButtonType currentHovering = Item1;     //the current button being hovered
};

#endif
