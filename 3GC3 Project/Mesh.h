/*
3GC3 Final Project - Space Genesis

David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __MESH_H__
#define __MESH_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <vector>
#include <stdio.h>

#include "Vector3.h"


using namespace std;

class Mesh
{
public:
    //Vertices
    std::vector<Vector3> vertices;
    std::vector<int> vIndex;
    std::vector<Vector3> normals;
    std::vector<int> nIndex;
    std::vector<Vector3> uvs;

    //Faces
    void LoadOBJ(const char * path);
    void Draw();
    Mesh();
};

#endif
