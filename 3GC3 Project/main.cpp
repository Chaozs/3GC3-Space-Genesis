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
enum GameState { Menu, SelectDifficulty, InstructionMenu, Playing, Paused, GameOver, Win };  //current game state enum
enum ButtonType { Item1, Item2, Item3, Item4 };
GameState currentState = Menu;      //initially in start menu
GUI userInfo = GUI();
MainMenu mainMenu;                  //create mainMenu
bool gamePaused = false;            //indicates whether or not game is currently paused
std::string difficultyString = "Medium";

/* ENEMY SHIP */
vector<Enemy*> enemyRow1;
vector<Enemy*> enemyRow2;
vector<Enemy*> enemyRow3;
vector<Enemy*> enemyRow4;
vector<Enemy*> enemyRow5;
int enemyCounter = 55;
Enemy flyingEnemy = Enemy(-25, 26, -25, 1);
bool flyingEnemyIsFlying = false;
//int enemyCounter = enemyRow1.size() + enemyRow2.size() + enemyRow3.size() + enemyRow4.size() + enemyRow5.size();

float enemyMovement = 0.06f; //enemy x movement speed
float enemyDifficulty = -0.0015; //enemy downwards movement speed
int indexCounter=0;

/* PLAYER SHIP */
Player player = Player(0, -4, -25);
Mesh playerMesh;
Mesh cubeMesh;

/* PLAYER CONTROLS */
bool leftPressed = false;           //left arrow key is held down
bool rightPressed = false;          //right arrow key is held down

/* BARRIERS */
list<Barrier*> barriers;
int barrierCounter = 10;

/* PROJECTILES */
list<Projectile*> projectiles;      //list of all player projectiles currently on screen
list<Projectile*> enemyProjectiles; //list of all enemy projectiles currently on screen
bool canShoot = true;               //indicates whether projectile can be shot or not (need time in between each projectile)
double multipleOfSpeedBeforeCanShoot = 0;   //keeps track of time before next projectile shoot

/* LIGHTING */
float light0Pos[] = {-5, 3, -25, 1};  //initial light0 position
float light1Pos[] = {5, 3, -25, 1};   //initial light1 positon

/* ANIMATION */
const int speed = 17;               //time between calls of display()

GLubyte* img_data; 					//how to play image
/* TEXTURE */
GLubyte* image;
GLubyte* image2;
int widthA, heightA, maxA;
int width = 0, height = 0, max = 0;
GLuint myTex[2];

GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;

    /* first open file and check if it's an ASCII PPM (indicated by P3 at the start) */
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        exit(0);
    }
    fscanf(fd, "%c",&c);

    /* next, skip past the comments - any line starting with #*/
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);

    /* now get the dimensions and max colour value from the image */
    fscanf(fd, "%d %d %d", &n, &m, &k);

    /* calculate number of pixels and allocate storage for this */
    nm = n*m;
    img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    s=255.0/k;

    /* for every pixel, grab the read green and blue values, storing them in the image data array */
    for(i=0; i<nm; i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }

    /* finally, set the "return parameters" (width, height, max) and return the image array */
    *width = n;
    *height = m;
    *max = k;

    return img;
}

void keyboard(unsigned char key, int x, int y)
{
    if(currentState == Menu)
    {
        switch (key)
        {
        case 32:        //if space key pressed, check which button is currently highlighted
            switch(mainMenu.getCurrentButton())
            {
            case Item1: //if start button is currently highlighted, switch game state to playing game
                currentState = Playing;
                break;
            case Item2:
                currentState = SelectDifficulty;
                break;
            case Item3:
                currentState = InstructionMenu;
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
        case 32:        //if space key pressed, check which button is currently highlighted
            switch(mainMenu.getCurrentButton())
            {
            case Item1: //easy difficulty
                enemyDifficulty = -0.001;
                difficultyString = "Easy";
                currentState = Menu;
                break;
            case Item2: //medium difficulty
                enemyDifficulty = -0.0015;
                difficultyString = "Medium";
                currentState = Menu;
                break;
            case Item3: //hard difficulty
                enemyDifficulty = -0.002;
                difficultyString = "Hard";
                currentState = Menu;
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
            if (gamePaused)
            {
                break;
            }
            if(canShoot)
            {
                canShoot = false;
                Projectile* p = new Projectile(player.getPosition().at(0),
                                               player.getPosition().at(1),
                                               player.getPosition().at(2));
                projectiles.push_back(p);
            }
            break;
        case 'p':
            gamePaused = !gamePaused;   //toggles whether or not game is paused
            break;
        }
    }
    else if (currentState == GameOver)
    {
        switch (key)
        {
        case 32:
            switch(mainMenu.getCurrentButton())
            {
            case Item1: //if start button is currently highlighted, switch game state to playing game
                currentState = Menu;
                break;
            case Item2:
                currentState = Playing;
                break;
            }
            break;
        }
    }
    else if (currentState == Win)
    {
        switch (key)
        {
        case 32:
            switch(mainMenu.getCurrentButton())
            {
            case Item1: //if start button is currently highlighted, switch game state to playing game
                currentState = Menu;
                break;
            case Item2:
                currentState = Playing;
                break;
            }
            break;
        }
    }
    else if(currentState == InstructionMenu)
    {
        switch (key)
        {
        case 32:
            currentState = Menu;
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
        if (gamePaused)
        {
            return;
        }
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
    else if (currentState == GameOver)
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
    else if (currentState == Win)
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
        if (gamePaused)
        {
            return;
        }
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

//reshape for accounting for window size
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
    float diff0[4] = {1, 1, 1, 1};
    float diff1[4] = {1, 1, 1, 1};
    float amb0[4] = {0.2f, 0.2f, 0.2f, 1};
    float amb1[4] = {0.2f, 0.2f, 0.2f, 1};
    float spec0[4] = {0.2f, 0.2f, 0.2f, 1};
    float spec1[4] = {0.2f, 0.2f, 0.2f, 1};

    //set light position and properties
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
    // glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    // glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);

}

//initialize enemies
void setEnemies()
{
    float xIncrement = 1.4f;
    for(int i=0; i<11; i++)
    {
        Enemy* enemy = new Enemy(-7+xIncrement*i, 23, -25, 1);
        enemyRow1.push_back(enemy);
    }

    for(int i=0; i<11; i++)
    {
        Enemy* enemy = new Enemy(-7+xIncrement*i, 20, -25, 1);
        enemyRow2.push_back(enemy);
    }

    for(int i=0; i<11; i++)
    {
        Enemy* enemy = new Enemy(-7+xIncrement*i, 17, -25, 1);
        enemyRow3.push_back(enemy);
    }

    for(int i=0; i<11; i++)
    {
        Enemy* enemy = new Enemy(-7+xIncrement*i, 14, -25, 1);
        enemyRow4.push_back(enemy);
    }

    for(int i=0; i<11; i++)
    {
        Enemy* enemy = new Enemy(-7+xIncrement*i, 11, -25, 1);
        enemy -> setBottomTrue();
        enemyRow5.push_back(enemy);
    }
}

//initialize barriers
void setBarriers()
{
    //Big barrier 1
    //bottom layer
    Barrier* barrier = new Barrier(-9.5f, 0, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-8, 0, -25);
    barriers.push_back(barrier);
    //middle layer
    barrier = new Barrier(-9.5f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-9, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-8.5f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-8, 0.5f, -25);
    barriers.push_back(barrier);
    //top layer
    barrier = new Barrier(-9.5f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-9, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-8.5f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-8, 1, -25);
    barriers.push_back(barrier);

    //Big barrier 2
    //bottom layer
    barrier = new Barrier(-3.833f, 0, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-2.333f, 0, -25);
    barriers.push_back(barrier);
    //Middle layer
    barrier = new Barrier(-3.833f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-3.333f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-2.833f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-2.333f, 0.5f, -25);
    barriers.push_back(barrier);
    //Top layer
    barrier = new Barrier(-3.833f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-3.333f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-2.833f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(-2.333f, 1, -25);
    barriers.push_back(barrier);

    //Big barrier 3
    //bottom layer
    barrier = new Barrier(2.334f, 0, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(3.834f, 0, -25);
    barriers.push_back(barrier);
    //Middle layer
    barrier = new Barrier(2.334f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(2.834f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(3.334f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(3.834f, 0.5f, -25);
    barriers.push_back(barrier);
    //Top layer
    barrier = new Barrier(2.334f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(2.834f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(3.334f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(3.834f, 1, -25);
    barriers.push_back(barrier);

    //Big barrier 3
    //bottom layer
    barrier = new Barrier(8, 0, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(9.5f, 0, -25);
    barriers.push_back(barrier);
    //Middle layer
    barrier = new Barrier(8, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(8.5f, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(9, 0.5f, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(9.5f, 0.5f, -25);
    barriers.push_back(barrier);
    //Bottom layer
    barrier = new Barrier(8, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(8.5f, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(9, 1, -25);
    barriers.push_back(barrier);
    barrier = new Barrier(9.5f, 1, -25);
    barriers.push_back(barrier);
}

void bindTextures()
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(2, myTex);

    glBindTexture(GL_TEXTURE_2D, myTex[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*Get and save image*/
    image = LoadPPM("PlayerShip_DIFFUSE.ppm",&widthA, &heightA, &maxA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthA, heightA, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, myTex[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*Get and save image*/
    image2 = LoadPPM("Null_texture.ppm",&widthA, &heightA, &maxA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthA, heightA, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image2);


    glMatrixMode(GL_TEXTURE);
}

void resetGame()
{
    flyingEnemyIsFlying = false;
    flyingEnemy.moveX(-50);
    player = Player(0, -4, -25);
    enemyRow1.clear();
    enemyRow2.clear();
    enemyRow3.clear();
    enemyRow4.clear();
    enemyRow5.clear();
    barriers.clear();
    projectiles.clear();
    enemyProjectiles.clear();
    gamePaused = false;
    leftPressed = false;
    rightPressed = false;
    canShoot = true;
    multipleOfSpeedBeforeCanShoot = 0;
    setEnemies();
    setBarriers();
    userInfo = GUI();
    glutPostRedisplay();
}

//initialize
void init(void)
{
    bindTextures();
    playerMesh.LoadOBJ("PlayerShip.obj");
    cubeMesh.LoadOBJ("EnemyShip.obj");
    setEnemies();
    setBarriers();
    glBindTexture(GL_TEXTURE_2D, 0);

    glClearColor(0.1, 0.1, 0.1, 0);       //black background

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

//for updating which enemy unit is bottom for row 2
void updateRow2(int i)
{
    if((enemyRow5.at(i) -> getAlive()==false) &&
            (enemyRow4.at(i) -> getAlive() == false) &&
            (enemyRow3.at(i) -> getAlive() == false))
    {
        enemyRow1.at(i)->setBottomTrue();
    }
}

//for updating which enemy unit is bottom for row 3
void updateRow3(int i)
{
    if((enemyRow5.at(i) -> getAlive()==false) &&
            (enemyRow4.at(i) -> getAlive() == false))
    {
        if(enemyRow2.at(i)->getAlive())
        {
            enemyRow2.at(indexCounter)->setBottomTrue();
        }
        else if(enemyRow1.at(i)->getAlive())
        {
            enemyRow1.at(indexCounter)->setBottomTrue();
        }
    }
}

//for updating which enemy unit is bottom for row 4
void updateRow4(int i)
{
    if(enemyRow5.at(i) -> getAlive()==false)
    {
        if(enemyRow3.at(i)->getAlive())
        {
            enemyRow3.at(indexCounter)->setBottomTrue();
        }
        else if(enemyRow2.at(i)->getAlive())
        {
            enemyRow2.at(indexCounter)->setBottomTrue();
        }
        else if(enemyRow1.at(i)->getAlive())
        {
            enemyRow1.at(indexCounter)->setBottomTrue();
        }
    }
}

//for updating which enemy unit is bottom for row 5
void updateRow5(int i)
{
    if(enemyRow4.at(i)->getAlive())
    {
        enemyRow4.at(indexCounter)->setBottomTrue();
    }
    else if(enemyRow3.at(i)->getAlive())
    {
        enemyRow3.at(indexCounter)->setBottomTrue();
    }
    else if(enemyRow2.at(i)->getAlive())
    {
        enemyRow2.at(indexCounter)->setBottomTrue();
    }
    else if(enemyRow1.at(i)->getAlive())
    {
        enemyRow1.at(indexCounter)->setBottomTrue();
    }
}

//timer for gameloop
void timer(int value)
{
    switch(currentState)
    {
    case Playing:
        if (!gamePaused)
        {
            //update player projectiles on screen
            for(auto i=projectiles.begin(); i!=projectiles.end();)
            {
                Projectile* projectileP = *i;
                if (projectileP->getPosition().at(1) >= 40)
                {
                    i = projectiles.erase(i);
                }
                else
                {
                    //check if projectile hits a barrier
                    for(list<Barrier*>::iterator b=barriers.begin(); b!=barriers.end(); ++b)
                    {
                        Barrier* barrier = *b;
                        if (barrier->isHit(projectileP->getPosition().at(0),
                                           projectileP->getPosition().at(1),
                                           projectileP->getPosition().at(2)))
                        {
                            i = projectiles.erase(i);
                            barrier->decreaseHp();
                        }
                    }

                    //check if projectile hits an enemy
                    for(auto j=enemyRow1.begin(); j!=enemyRow1.end();)
                    {
                        Enemy* enemy = *j;
                        if (enemy->isHit(projectileP->getPosition().at(0),
                                         projectileP->getPosition().at(1),
                                         projectileP->getPosition().at(2)))
                        {
                            i = projectiles.erase(i);
                            enemy->setAlive(false);
                            ++j;
                            userInfo.incScoreBy(150);
                            enemyCounter--;
                        }
                        else
                        {
                            ++j;
                        }
                    }

                    //check if projectile hits an enemy
                    indexCounter=0;
                    for(auto j=enemyRow2.begin(); j!=enemyRow2.end();)
                    {
                        Enemy* enemy = *j;
                        if (enemy->isHit(projectileP->getPosition().at(0),
                                         projectileP->getPosition().at(1),
                                         projectileP->getPosition().at(2)))
                        {
                            i = projectiles.erase(i);
                            enemy->setAlive(false);
                            updateRow2(indexCounter);
                            indexCounter++;
                            ++j;
                            userInfo.incScoreBy(150);
                            enemyCounter--;
                        }
                        else
                        {
                            indexCounter++;
                            ++j;
                        }
                    }

                    //check if projectile hits an enemy
                    indexCounter=0;
                    for(auto j=enemyRow3.begin(); j!=enemyRow3.end();)
                    {
                        Enemy* enemy = *j;
                        if (enemy->isHit(projectileP->getPosition().at(0),
                                         projectileP->getPosition().at(1),
                                         projectileP->getPosition().at(2)))
                        {
                            i = projectiles.erase(i);
                            enemy->setAlive(false);
                            updateRow3(indexCounter);
                            indexCounter++;
                            ++j;
                            userInfo.incScoreBy(150);
                            enemyCounter--;
                        }
                        else
                        {
                            indexCounter++;
                            ++j;
                        }
                    }

                    //check if projectile hits an enemy
                    indexCounter=0;
                    for(auto j=enemyRow4.begin(); j!=enemyRow4.end();)
                    {
                        Enemy* enemy = *j;
                        if (enemy->isHit(projectileP->getPosition().at(0),
                                         projectileP->getPosition().at(1),
                                         projectileP->getPosition().at(2)))
                        {
                            i = projectiles.erase(i);
                            enemy->setAlive(false);
                            updateRow4(indexCounter);
                            indexCounter++;
                            ++j;
                            userInfo.incScoreBy(150);
                            enemyCounter--;
                        }
                        else
                        {
                            indexCounter++;
                            ++j;
                        }
                    }

                    //check if projectile hits an enemy
                    indexCounter=0;
                    for(auto j=enemyRow5.begin(); j!=enemyRow5.end();)
                    {
                        Enemy* enemy = *j;
                        if (enemy->isHit(projectileP->getPosition().at(0),
                                         projectileP->getPosition().at(1),
                                         projectileP->getPosition().at(2)))
                        {
                            i = projectiles.erase(i);
                            enemy->setAlive(false);
                            updateRow5(indexCounter);
                            indexCounter++;
                            ++j;
                            userInfo.incScoreBy(150);
                            enemyCounter--;
                        }
                        else
                        {
                            indexCounter++;
                            ++j;
                        }
                    }

                    if (flyingEnemy.isHit(projectileP->getPosition().at(0),
                                          projectileP->getPosition().at(1),
                                          projectileP->getPosition().at(2)))
                    {
                        i = projectiles.erase(i);
                        flyingEnemy.moveX(-50);
                        userInfo.incScoreBy(500);
                        flyingEnemyIsFlying = false;
                    }

                    //update positions of player projectiles on screen
                    projectileP->moveY(0.5);
                    ++i;
                }
            }

            if(enemyRow1.back()->getX()>=11)
            {
                enemyMovement = -0.06f;
            }
            else if(enemyRow1.front()->getX()<=-11)
            {
                enemyMovement = 0.06f;
            }

            //generate enemy projectiles
            for(std::vector<Enemy*>::iterator i = enemyRow1.begin(); i != enemyRow1.end(); ++i)
            {
                Enemy* enemy = *i;
                if (enemy->shouldShoot(200) && enemy->isBottomTrue() && enemy->getMultipleOfSpeedBeforeCanShoot() >= 10)
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(0);
                    Projectile* enemyProj = new Projectile(enemy->getPosition().at(0), enemy->getPosition().at(1), enemy->getPosition().at(2));
                    enemyProjectiles.push_back(enemyProj);
                }
                else
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(enemy->getMultipleOfSpeedBeforeCanShoot()+1);
                }
                enemy->moveX(enemyMovement);
                enemy->moveY(enemyDifficulty);
            }

            //generate enemy projectiles
            for(std::vector<Enemy*>::iterator i = enemyRow2.begin(); i != enemyRow2.end(); ++i)
            {
                Enemy* enemy = *i;
                if (enemy->shouldShoot(200) && enemy->isBottomTrue() && enemy->getMultipleOfSpeedBeforeCanShoot() >= 10)
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(0);
                    Projectile* enemyProj = new Projectile(enemy->getPosition().at(0), enemy->getPosition().at(1), enemy->getPosition().at(2));
                    enemyProjectiles.push_back(enemyProj);
                }
                else
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(enemy->getMultipleOfSpeedBeforeCanShoot()+1);
                }
                enemy->moveX(enemyMovement);
                enemy->moveY(enemyDifficulty);
            }

            //generate enemy projectiles
            for(std::vector<Enemy*>::iterator i = enemyRow3.begin(); i != enemyRow3.end(); ++i)
            {
                Enemy* enemy = *i;
                if (enemy->shouldShoot(200) && enemy->isBottomTrue() && enemy->getMultipleOfSpeedBeforeCanShoot() >= 10)
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(0);
                    Projectile* enemyProj = new Projectile(enemy->getPosition().at(0), enemy->getPosition().at(1), enemy->getPosition().at(2));
                    enemyProjectiles.push_back(enemyProj);
                }
                else
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(enemy->getMultipleOfSpeedBeforeCanShoot()+1);
                }
                enemy->moveX(enemyMovement);
                enemy->moveY(enemyDifficulty);
            }

            //generate enemy projectiles
            for(std::vector<Enemy*>::iterator i = enemyRow4.begin(); i != enemyRow4.end(); ++i)
            {
                Enemy* enemy = *i;
                if (enemy->shouldShoot(200) && enemy->isBottomTrue() && enemy->getMultipleOfSpeedBeforeCanShoot() >= 10)
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(0);
                    Projectile* enemyProj = new Projectile(enemy->getPosition().at(0), enemy->getPosition().at(1), enemy->getPosition().at(2));
                    enemyProjectiles.push_back(enemyProj);
                }
                else
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(enemy->getMultipleOfSpeedBeforeCanShoot()+1);
                }
                enemy->moveX(enemyMovement);
                enemy->moveY(enemyDifficulty);
            }

            //generate enemy projectiles
            for(std::vector<Enemy*>::iterator i = enemyRow5.begin(); i != enemyRow5.end(); ++i)
            {
                Enemy* enemy = *i;
                if (enemy->shouldShoot(200) && enemy->isBottomTrue() && enemy->getMultipleOfSpeedBeforeCanShoot() >= 10)
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(0);
                    Projectile* enemyProj = new Projectile(enemy->getPosition().at(0), enemy->getPosition().at(1), enemy->getPosition().at(2));
                    enemyProjectiles.push_back(enemyProj);
                }
                else
                {
                    enemy->setMultipleOfSpeedBeforeCanShoot(enemy->getMultipleOfSpeedBeforeCanShoot()+1);
                }
                enemy->moveX(enemyMovement);
                enemy->moveY(enemyDifficulty);
            }

            //move flying enemy if it is flying across screen
            if (flyingEnemyIsFlying)
            {
                if (flyingEnemy.getPosition().at(0) >= 25)
                {
                    flyingEnemyIsFlying = false;
                    flyingEnemy.moveX(-50);
                }
                else
                {
                    //determine if flying enemy should shoot
                    if (flyingEnemy.shouldShoot(200) && flyingEnemy.getMultipleOfSpeedBeforeCanShoot() >= 10)
                    {
                        flyingEnemy.setMultipleOfSpeedBeforeCanShoot(0);
                        Projectile* enemyProj = new Projectile(flyingEnemy.getPosition().at(0), flyingEnemy.getPosition().at(1), flyingEnemy.getPosition().at(2));
                        enemyProjectiles.push_back(enemyProj);
                    }
                    else
                    {
                        flyingEnemy.setMultipleOfSpeedBeforeCanShoot(flyingEnemy.getMultipleOfSpeedBeforeCanShoot()+1);
                    }
                    flyingEnemy.moveX(0.1); //moves flying enemy across screen
                }
            }

            //determine if flying enemy will start to fly by
            if (flyingEnemy.shouldShoot(200) && !flyingEnemyIsFlying)
            {
                flyingEnemyIsFlying = true;
            }

            //update positions of enemy projectiles on screen
            for(auto i=enemyProjectiles.begin(); i!=enemyProjectiles.end();)
            {
                Projectile* projectileP = *i;
                if (projectileP->getPosition().at(1) <= -7)
                {
                    i = enemyProjectiles.erase(i);
                }
                //check if enemy projectile hits player
                else if (player.isHit(projectileP->getPosition().at(0), projectileP->getPosition().at(1), projectileP->getPosition().at(2)))
                {
                    player.decreaseHp(50);
                    i = enemyProjectiles.erase(i);
                }
                else
                {
                    //check if projectile hits a barrier
                    for(list<Barrier*>::iterator b=barriers.begin(); b!=barriers.end(); ++b)
                    {
                        Barrier* barrier = *b;
                        if (barrier->isHit(projectileP->getPosition().at(0),
                                           projectileP->getPosition().at(1),
                                           projectileP->getPosition().at(2)))
                        {
                            i = enemyProjectiles.erase(i);
                            barrier->decreaseHp();
                        }
                    }

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

            glutPostRedisplay();    //calls display
        }

        if(player.getHp() == 0)
        {
            currentState = GameOver;
            resetGame();
        }

        if(enemyCounter == 0 || barrierCounter == 0)
        {
            currentState = Win;
            resetGame();
        }
    }
    //wait before calling timer() again
    glutTimerFunc(speed, timer, 0);
}

//display method to be recalled upon any changes
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    reshape(800,800);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    addLights();        //add lights

    //displays accordingly to what game state
    glBindTexture(GL_TEXTURE_2D, myTex[1]);
    switch(currentState)
    {
    case Menu:
        glFrontFace(GL_CW);
        mainMenu.drawMenu();
        glFrontFace(GL_CCW);
        break;
    case Playing:
        //enable lighting
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);

        if(gamePaused)
        {
            glDisable(GL_LIGHTING);
            glBindTexture(GL_TEXTURE_2D, 0);
            userInfo.drawPause();
            glEnable(GL_LIGHTING);
            glBindTexture(GL_TEXTURE_2D, myTex[0]);
        }
        eye[0] = 0;
        eye[1] = -5;
        eye[2] = 0;
        lookAt[0] = 0;
        lookAt[1] = 0;
        lookAt[2] = -10;

        glBindTexture(GL_TEXTURE_2D, myTex[0]);
        gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);

        player.drawShip(playerMesh);      //draw ship
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, 0);
        userInfo.drawScoreAndHP(player.getHp());
        userInfo.drawDifficulty(difficultyString);
        glEnable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, myTex[0]);

        for(list<Barrier*>::iterator i=barriers.begin(); i!=barriers.end(); ++i)
        {
            Barrier* barrier = *i;
            barrier->drawBarrier();
        }

        glBindTexture(GL_TEXTURE_2D, myTex[1]);
        //draw enemy ships on screen
        for(std::vector<Enemy*>::iterator i = enemyRow1.begin(); i != enemyRow1.end(); ++i)
        {
            Enemy* enemy = *i;
            enemy->drawShip(cubeMesh);
        }

        //draw enemy ships on screen
        for(std::vector<Enemy*>::iterator i = enemyRow2.begin(); i != enemyRow2.end(); ++i)
        {
            Enemy* enemy = *i;
            enemy->drawShip(cubeMesh);
        }

        //draw enemy ships on screen
        for(std::vector<Enemy*>::iterator i = enemyRow3.begin(); i != enemyRow3.end(); ++i)
        {
            Enemy* enemy = *i;
            enemy->drawShip(cubeMesh);
        }

        //draw enemy ships on screen
        for(std::vector<Enemy*>::iterator i = enemyRow4.begin(); i != enemyRow4.end(); ++i)
        {
            Enemy* enemy = *i;
            enemy->drawShip(cubeMesh);
        }

        //draw enemy ships on screen
        for(std::vector<Enemy*>::iterator i = enemyRow5.begin(); i != enemyRow5.end(); ++i)
        {
            Enemy* enemy = *i;
            enemy->drawShip(cubeMesh);
        }

        //draw flying enemy ship on screen
        flyingEnemy.drawShip(cubeMesh);

        //draw projectiles onto screen
        for(list<Projectile*>::iterator i=projectiles.begin(); i!=projectiles.end(); ++i)
        {
            Projectile* projectileP = *i;
            //glColor3f(0.7, 0.7, 0.7);
            projectileP->draw();
        }

        //draw enemy projectiles onto screen
        for(list<Projectile*>::iterator i=enemyProjectiles.begin(); i!=enemyProjectiles.end(); ++i)
        {
            Projectile* projectileP = *i;
            projectileP->draw();
        }
        break;
    case SelectDifficulty:
        glFrontFace(GL_CW);
        mainMenu.drawDifficulty();
        glFrontFace(GL_CCW);
        break;
    case InstructionMenu:
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 800);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRasterPos2i(600,250);
        glPixelZoom(-1, 1);
        glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, img_data);

        glRasterPos2i(205,150);
        std::string ScoreLabel = "Press SPACE to go back and resume your mission.";
        for(int i=0; i<ScoreLabel.size(); i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ScoreLabel[i]);
        }
        glFlush();
        break;
    }
    case GameOver:
        glDisable(GL_LIGHTING);
        glFrontFace(GL_CW);
        mainMenu.drawGameOver();
        glFrontFace(GL_CCW);
        break;
    case Win:
        glDisable(GL_LIGHTING);
        glFrontFace(GL_CW);
        mainMenu.drawWin();
        glFrontFace(GL_CCW);
        break;
    }

    glutSwapBuffers();
}

//main method
int main(int argc, char** argv)
{
    glutInit(&argc, argv);              //starts up GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int k;
    img_data = LoadPPM("instructions.ppm", &width, &height, &k);
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
