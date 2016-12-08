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

float eye[] = {0,-5,0};        //initial camera location
float lookAt[] = {0,0,-10};         //point camera is looking at
float unitPosition[] = {0,0,0};
enum GameState { Menu, SelectDifficulty, InstructionMenu, Playing, Paused, GameOver };    //current game state enum
enum ButtonType { Item1, Item2, Item3, Item4 };
GameState currentState = Menu;  //initially in start menu

MainMenu mainMenu;              //create mainMenu

void keyboard(unsigned char key, int x, int y){

    //***********************************Main Menu Navigiation***********************************
    if(currentState == Menu){
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
            case Item4:  //if exit is currently highlighted, exit game
                exit (0);
                break;
            }
            break;
        }
    }else if(currentState == SelectDifficulty){
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
            case Item4:  //if return clicked, return to main menu
                currentState = Menu;
                break;
            }
            break;
        }
    }



    glutPostRedisplay();    //call display again after keyboard input
}

void special(int key, int x, int y){


    if(currentState == Menu){
        switch(key){

        case GLUT_KEY_UP:
            mainMenu.goUp();        //scroll up menu
            break;
        case GLUT_KEY_DOWN:
            mainMenu.goDown();      //scroll down menu
            break;
        }

    } else if (currentState == Playing){
        switch(key){
        
        case GLUT_KEY_UP:
            unitPosition[1]++;
            break;

        case GLUT_KEY_DOWN:
            unitPosition[1]--;
            break;

        case GLUT_KEY_LEFT:
            unitPosition[0]--;
            break;

        case GLUT_KEY_RIGHT:
            unitPosition[0]++;
            break;


        }
    } else if (currentState == SelectDifficulty){
        switch(key){

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


void init(void){
    glClearColor(0, 0, 0, 0);       //black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
    //set initial camera position and direction
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);
}

//display method to be recalled upon any changes
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


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
        glPushMatrix();
            glTranslatef(6, 3, -25);
            glTranslatef(unitPosition[0], unitPosition[1], unitPosition[2]);
            //cout << unitPosition[2] << ", " << unitPosition[1] << endl;
            glutWireTeapot(1);
        glPopMatrix();
        break;

    case SelectDifficulty:
        mainMenu.drawDifficulty();
        break;
    }

    glutSwapBuffers();
}


//main method
int main(int argc, char** argv){
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
