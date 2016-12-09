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
    score=0;
}

//increase score by increqase
void GUI::incScoreBy(int increase)
{
    score = score+increase;
}

//draws the player's current HP and Score
void GUI::drawScoreAndHP(int HP)
{
    glRasterPos3f(-21, 30, -25);
    std::string ScoreLabel = {"Score: "};
    for(int i=0; i<ScoreLabel.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ScoreLabel[i]);
    }

    glRasterPos3f(-17, 30, -25);
    std::string ScoreText = std::to_string(score);
    for(int i=0; i<ScoreText.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ScoreText[i]);
    }

    glRasterPos3f(-12, 30, -25);
    std::string HPLabel = {"HP: "};
    for(int i=0; i<HPLabel.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, HPLabel[i]);
    }

    glRasterPos3f(-9, 30, -25);
    std::string HPText = std::to_string(HP);
    for(int i=0; i<HPText.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, HPText[i]);
    }
}
