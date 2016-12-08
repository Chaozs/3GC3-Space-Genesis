/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __VECTOR3_H__
#define __VECTOR3_H__
/*
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <vector>*/

using namespace std;

//TODO: *hold mesh of object (+getter/setter for mesh)
//      *detect collision

class Vector3
{
    public:
        float x;
        float y;
        float z;
        Vector3(float, float, float);  
    private:
        float position[3];                   //position of object
};

#endif
