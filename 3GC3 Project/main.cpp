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
#include "GUI.h"
#include "Mesh.h"
#include "Vector3.h"

/* CAMERA */
float eye[] = {0, -5, 0};           //initial camera location
float lookAt[] = {0, 0, -10};       //point camera is looking at

/* GAME STATE */
float unitPosition[] = {0, 0, 0};
enum GameState { Menu, SelectDifficulty, InstructionMenu, Playing, Paused, GameOver };  //current game state enum
enum ButtonType { Item1, Item2, Item3, Item4 };
GameState currentState = Menu;      //initially in start menu
Player player = Player(6, 3, -25);
GUI userInfo = GUI();
MainMenu mainMenu;                  //create mainMenu

/* LIGHTING */
float light0Pos[] = {-5, 3, 0, 1};  //initial light0 position
float light1Pos[] = {5, 3, 0, 1};   //initial light1 positon


void keyboard(unsigned char key, int x, int y)
{

    //***********************************Main Menu Navigiation***********************************
    if(currentState == Menu)
    {
        switch (key)
        {
        case 13:        //if enter key pressed, check which button is currently highlighted
            switch(mainMenu.getCurrentButton())
            {
            case Item1: //if start button is currently highlighted, switch game state to playing game
                currentState = Playing;
                break;
            case Item2:
                currentState = SelectDifficulty;
                break;
            case Item3:
                //TODO
                break;
            case Item4: //if exit is currently highlighted, exit game
                exit (0);
                break;
            }
            break;
        }
    }
    else if(currentState == SelectDifficulty)
    {
        switch (key)
        {
        case 13:        //if enter key pressed, check which button is currently highlighted
            switch(mainMenu.getCurrentButton())
            {
            case Item1:
                //TODO
                break;
            case Item2:
                //TODO
                break;
            case Item3:
                //TODO
                break;
            case Item4: //if return clicked, return to main menu
                currentState = Menu;
                break;
            }
            break;
        }
    }

    glutPostRedisplay();    //call display again after keyboard input
}

void special(int key, int x, int y)
{
    if(currentState == Menu)
    {
        switch(key)
        {

        case GLUT_KEY_UP:
            mainMenu.goUp();        //scroll up menu
            break;
        case GLUT_KEY_DOWN:
            mainMenu.goDown();      //scroll down menu
            break;
        }

    }
    else if (currentState == Playing)
    {
        switch(key)
        {

        case GLUT_KEY_UP:
            player.moveY(1);
            break;

        case GLUT_KEY_DOWN:
            player.moveY(-1);
            break;

        case GLUT_KEY_LEFT:
            player.moveX(-1);
            break;

        case GLUT_KEY_RIGHT:
            player.moveX(1);
            break;
        }

    }
    else if (currentState == SelectDifficulty)
    {
        switch(key)
        {

        case GLUT_KEY_UP:
            mainMenu.goUp();        //scroll up menu
            break;
        case GLUT_KEY_DOWN:
            mainMenu.goDown();      //scroll down menu
            break;
        }

    }

    glutPostRedisplay();
}


void reshape(int w, int h)
{
    //Windoresizing stuff
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)((w + 0.0f) / h), 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

/* Adds lights to the scene */
void addLights()
{
    //set light colours
    float diff0[4] = {0, 0.2, 1, 1};            //blue light (light0)
    float diff1[4] = {1, 0.6, 0, 1};            //amber light (light1)
    float amb0[4] = {0.2f, 0.2f, 0.2f, 1};
    float amb1[4] = {0.2f, 0.2f, 0.2f, 1};
    float spec0[4] = {0.2f, 0.2f, 0.2f, 1};
    float spec1[4] = {0.2f, 0.2f, 0.2f, 1};

    //set light position and properties
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);

    //draw sphere for light0
    glColor3f(0, 0.2, 1);
    glPushMatrix();
    glTranslatef(light0Pos[0], light0Pos[1], light0Pos[2]);
    glutWireSphere(0.1, 16, 16);
    glPopMatrix();

    //draw sphere for light1
    glColor3f(1, 0.6, 0);
    glPushMatrix();
    glTranslatef(light1Pos[0], light1Pos[1], light1Pos[2]);
    glutWireSphere(0.1, 16, 16);
    glPopMatrix();
}

void init(void)
{
    glClearColor(0, 0, 0, 0);       //black background
    glEnable(GL_COLOR_MATERIAL);    //enable colour material

    //enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
    glEnable(GL_DEPTH_TEST);    //enables z buffer

    //enable backface culling
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //set initial camera position and direction
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);
}

//display method to be recalled upon any changes
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    addLights();        //add lights

    //displays accordingly to what game state
    switch(currentState)
    {
    case Menu:
        mainMenu.drawMenu();
        break;
    case Playing:
        eye[0] = 0;
        eye[1] = -5;
        eye[2] = 0;
        lookAt[0] = 0;
        lookAt[1] = 0;
        lookAt[2] = -10;
        gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);
        player.drawShip();				//draw ship
        glDisable(GL_LIGHTING);
        userInfo.drawScoreAndHP(100);
        glEnable(GL_LIGHTING);
        break;

    case SelectDifficulty:
        mainMenu.drawDifficulty();
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
    glutReshapeFunc(reshape);

    glEnable(GL_DEPTH_TEST);
    init();

    glutMainLoop();                     //starts the event loop

    return(0);
}
