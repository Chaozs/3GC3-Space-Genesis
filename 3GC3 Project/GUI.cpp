/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "GUI.h"

//constructor
GUI::GUI()
{
    score = 0;
}

//increase score by increqase
void GUI::incScoreBy(int increase)
{
    score += increase;
}

//draws the player's current HP and Score
void GUI::drawScoreAndHP(int HP)
{   
    //Score Label
    glRasterPos3f(-21, 30, -25);
    std::string ScoreLabel = "Score:";
    for(int i=0; i<ScoreLabel.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ScoreLabel[i]);
    }

    //Score Number
    glRasterPos3f(-17.5, 30, -25);
    std::string ScoreText = std::to_string(score);
    for(int i=0; i<ScoreText.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ScoreText[i]);
    }

    //HP Label
    glRasterPos3f(-12, 30, -25);
    std::string HPLabel = "HP:";
    for(int i=0; i<HPLabel.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, HPLabel[i]);
    }

    //HP Number
    glRasterPos3f(-10, 30, -25);
    std::string HPText = std::to_string(HP);
    for(int i=0; i<HPText.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, HPText[i]);
    }
}

//Draws the difficulty text to the screen
void GUI::drawDifficulty(std::string difficulty)
{
    glRasterPos3f(10, 30, -25);
    for(int i=0; i<difficulty.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, difficulty[i]);
    }
}

//Draws the pause text to the screen
void GUI::drawPause()
{
    glRasterPos3f(-1, 10, -20);
    std::string ScoreLabel = "Pause";
    for(int i=0; i<ScoreLabel.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ScoreLabel[i]);
    }
}
