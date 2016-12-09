/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <vector>
#include <list>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

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
GUI userInfo = GUI();
MainMenu mainMenu;                  //create mainMenu

/* DEMO ENEMY SHIP - TOBEREMOVED */
list<Enemy*> enemyList;

/* PLAYER SHIP */
Player player = Player(0, -4, -25);
Mesh playerMesh;

/* PLAYER CONTROLS */
bool leftPressed = false;           //left arrow key is held down
bool rightPressed = false;          //right arrow key is held down

/* PROJECTILES */
list<Projectile*> projectiles;      //list of all player projectiles currently on screen
list<Projectile*> enemyProjectiles; //list of all enemy projectiles currently on screen
bool canShoot = true;               //indicates whether projectile can be shot or not (need time in between each projectile)
double multipleOfSpeedBeforeCanShoot = 0;   //keeps track of time before next projectile shoot

/* LIGHTING */
float light0Pos[] = {-5, 3, 0, 1};  //initial light0 position
float light1Pos[] = {5, 3, 0, 1};   //initial light1 positon

/* ANIMATION */
const int speed = 30;               //time between calls of display()


void keyboard(unsigned char key, int x, int y)
{
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
    else if (currentState == Playing)
    {
        switch (key)
        {
        case 32:    //if space is pressed, create a new projectile
            if(canShoot)
            {
                canShoot = false;
                Projectile* p = new Projectile(player.getPosition().at(0),
                                               player.getPosition().at(1),
                                               player.getPosition().at(2));
                projectiles.push_back(p);
            }
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
        case GLUT_KEY_LEFT:
            leftPressed = true;
            break;
        case GLUT_KEY_RIGHT:
            rightPressed = true;
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

void specialUp(int key, int x, int y)
{
    if (currentState == Playing)
    {
        switch(key)
        {
        case GLUT_KEY_LEFT:
            leftPressed = false;
            break;
        case GLUT_KEY_RIGHT:
            rightPressed = false;
            break;
        }
    }
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

void setMeshes()
{
    //Mesh newMesh;
    playerMesh.LoadOBJ("PlayerShip.obj");

    //playerMesh = newMesh;
    player.SetMesh(playerMesh);

    for(list<Enemy*>::iterator i=enemyList.begin(); i!=enemyList.end(); ++i)
    {
        Enemy* enemy = *i;
        enemy->SetMesh(playerMesh);
        cout << "Set enemy mesh" << endl;
    }
}

void setEnemies()
{
    Enemy* enemy = new Enemy(0, 10, -25, 1);
    enemyList.push_back(enemy);
}

void init(void)
{
    setEnemies();
    setMeshes();

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
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //set initial camera position and direction
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);
}

void timer(int value)
{
    switch(currentState)
    {
    case Playing:
        //update positions of player projectiles on screen
        for(auto i=projectiles.begin(); i!=projectiles.end();)
        {
            Projectile* projectileP = *i;
            if (projectileP->getPosition().at(1) >= 40)
            {
                i = projectiles.erase(i);
            }
            else
            {
                projectileP->moveY(0.5);
                ++i;
            }
        }

        //generate enemy projectiles
        for(list<Enemy*>::iterator i=enemyList.begin(); i!=enemyList.end(); ++i)
        {
            Enemy* enemy = *i;
            if (enemy->shouldShoot(10))
            {
                cout << "Enemy shot projectile" << endl;
                Projectile* enemyProj = new Projectile(enemy->getPosition().at(0), enemy->getPosition().at(1), enemy->getPosition().at(2));
                enemyProjectiles.push_back(enemyProj);
            }
        }

        //update positions of enemy projectiles on screen
        for(auto i=enemyProjectiles.begin(); i!=enemyProjectiles.end();)
        {
            Projectile* projectileP = *i;
            if (projectileP->getPosition().at(1) >= 40)
            {
                i = enemyProjectiles.erase(i);
            }
            else
            {
                projectileP->moveY(-0.5);
                ++i;
            }
        }

        if (multipleOfSpeedBeforeCanShoot >= 20)
        {
            multipleOfSpeedBeforeCanShoot = 0;
            canShoot = true;
        }
        else
        {
            multipleOfSpeedBeforeCanShoot++;
        }

        //move ship position if left/right arrow keys are pressed
        if (leftPressed)
        {
            if (player.getPosition().at(0) > -11)
            {
                player.moveX(-0.3);
            }
        }
        if (rightPressed)
        {
            if (player.getPosition().at(0) < 11)
            {
                player.moveX(0.3);
            }
        }
    }

    glutPostRedisplay();    //calls display

    //wait before calling timer() again
    glutTimerFunc(speed, timer, 0);

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
        glFrontFace(GL_CW);
        mainMenu.drawMenu();
        glFrontFace(GL_CCW);
        break;
    case Playing:
        eye[0] = 0;
        eye[1] = -5;
        eye[2] = 0;
        lookAt[0] = 0;
        lookAt[1] = 0;
        lookAt[2] = -10;
        gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);
        player.drawShip();      //draw ship
        glDisable(GL_LIGHTING);
        userInfo.drawScoreAndHP(100);
        glEnable(GL_LIGHTING);

        //draw enemy ships on screen
        for(list<Enemy*>::iterator i=enemyList.begin(); i!=enemyList.end(); ++i)
        {
            Enemy* enemy = *i;
            enemy->drawShip();
        }

        //draw projectiles onto screen
        for(list<Projectile*>::iterator i=projectiles.begin(); i!=projectiles.end(); ++i)
        {
            Projectile* projectileP = *i;
            glColor3f(1, 1, 1);
            projectileP->draw();
        }

        //draw enemy projectiles onto screen
        for(list<Projectile*>::iterator i=enemyProjectiles.begin(); i!=enemyProjectiles.end(); ++i)
        {
            Projectile* projectileP = *i;
            glColor3f(1, 0, 0);
            projectileP->draw();
        }
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
    glutSpecialUpFunc(specialUp);
    glutReshapeFunc(reshape);
    glutTimerFunc(speed, timer, 0);

    glEnable(GL_DEPTH_TEST);
    init();

    glutMainLoop();                     //starts the event loop

    return(0);
}
