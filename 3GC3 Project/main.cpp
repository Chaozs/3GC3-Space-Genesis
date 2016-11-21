/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <math.h>
#include <string>
#include <iostream>

#include "MainMenu.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Barrier.h"

float eye[] = {5,3.5,5};        //initial camera location
float lookAt[] {0,0,0};         //point camera is looking at
enum ButtonType { Start, Difficulty, HowToPlay, Exit };                 //menu buttons enum
enum GameState { Menu, InstructionMenu, Playing, Paused, GameOver };    //current game state enum
GameState currentState = Menu;  //initially in start menu

MainMenu mainMenu;              //create mainMenu

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 13:        //if enter key pressed, check which button is currently highlighted
        switch(mainMenu.getCurrentButton())
        {
        case Start: //if start button is currently highlighted, switch game state to playing game
            currentState = Playing;
            break;
        case Difficulty:
            //TODO
            break;
        case HowToPlay:
            //TODO
            break;
        case Exit:  //if exit is currently highlighted, exit game
            exit (0);
            break;
        }
        break;
    }
    glutPostRedisplay();    //call display again after keyboard input
}

void special(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:

        break;
    case GLUT_KEY_RIGHT:

        break;
    case GLUT_KEY_UP:
        if(currentState==Menu)      //if in main menu
        {
            mainMenu.goUp();        //scroll up menu
        }
        break;
    case GLUT_KEY_DOWN:
        if(currentState==Menu)      //if in main menu
        {
            mainMenu.goDown();      //scroll down menu
        }
        break;
    }
    glutPostRedisplay();
}

void init(void)
{
    glClearColor(0, 0, 0, 0);       //black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
}

//display method to be recalled upon any changes
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //set initial camera position and direction
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);

    //displays accordingly to what game state
    switch(currentState)
    {
    case Menu:
        mainMenu.draw();
        break;
    case Playing:
        break;
    }

    glutSwapBuffers();
}

//main method
int main(int argc, char** argv)
{
    glutInit(&argc, argv);              //starts up GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Space Genesis");  //creates the window with game name

    glutDisplayFunc(display);           //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glEnable(GL_DEPTH_TEST);
    init();

    glutMainLoop();                     //starts the event loop

    return(0);
}
