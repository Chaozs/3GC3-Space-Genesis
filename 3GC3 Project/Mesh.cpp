/*
3GC3 Final Project - Space Genesis
David Hobson / hobsondd / 001412317
Jack Snopek / snopekjt / 001408851
Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Mesh.h"

Mesh::Mesh()
{
}

void Mesh::LoadOBJ(const char * path)
{
    FILE * file = fopen(path, "r");

    float input1;
    float input2;
    float input3;
    int vInd1 = 0, vInd2= 0, vInd3= 0, uInd1= 0, uInd2= 0, uInd3= 0, nInd1= 0, nInd2= 0, nInd3 = 0;

    while(true)
    {
        char prefix[128];

        int res = fscanf(file, "%s", prefix);
        if (res == EOF)
        {
            break; 
        }

        
        if ( strcmp("v", prefix) == 0 )
        {
            fscanf(file, "%f %f %f\n", &input1, &input2, &input3);
            vertices.push_back(Vector3(input1, input2, input3));
        }
        else if ( strcmp("vn", prefix) == 0 )
        {
            fscanf(file, "%f %f %f\n", &input1, &input2, &input3);
            normals.push_back(Vector3(input1, input2, input3));
        }
        else if (strcmp("vt", prefix) == 0 )
        {
            fscanf(file, "%f %f \n", &input1, &input2);
            uvs.push_back(Vector3(input1, input2, 0));
        }

        else if (strcmp("f", prefix) == 0 )
        {           
            int matches = fscanf(file, "%i/%i/%i %i/%i/%i %i/%i/%i\n", &vInd1, &uInd1, &nInd1, &vInd2, &uInd2, &nInd1, &vInd3, &uInd3, &nInd1);

            vIndex.push_back(vInd1);
            vIndex.push_back(vInd2);
            vIndex.push_back(vInd3);

            uIndex.push_back(uInd1);
            uIndex.push_back(uInd2);
            uIndex.push_back(uInd3);

            nIndex.push_back(nInd1);
            nIndex.push_back(nInd1);
            nIndex.push_back(nInd1);
        }
    }
}

void Mesh::Draw()
{
    glPolygonMode(GL_FRONT, GL_FILL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glBegin(GL_TRIANGLES);
    int i = 0;
    while(i < vIndex.size())
    {        
        glNormal3f(normals[nIndex[i]-1].x, normals[nIndex[i]-1].y, normals[nIndex[i]-1].z);
        glTexCoord2f(uvs[uIndex[i]-1].x, uvs[uIndex[i]-1].y);
        glVertex3f(vertices[vIndex[i]-1].x, vertices[vIndex[i]-1].y, vertices[vIndex[i]-1].z); 

        i++;
        glNormal3f(normals[nIndex[i]-1].x, normals[nIndex[i]-1].y, normals[nIndex[i]-1].z);
        glTexCoord2f(uvs[uIndex[i]-1].x, uvs[uIndex[i]-1].y);
        glVertex3f(vertices[vIndex[i]-1].x, vertices[vIndex[i]-1].y, vertices[vIndex[i]-1].z);
        i++;
        glNormal3f(normals[nIndex[i]-1].x, normals[nIndex[i]-1].y, normals[nIndex[i]-1].z);
        glTexCoord2f(uvs[uIndex[i]-1].x, uvs[uIndex[i]-1].y);
        glVertex3f(vertices[vIndex[i]-1].x, vertices[vIndex[i]-1].y, vertices[vIndex[i]-1].z);
        i++;
        
    }
    glEnd();
}
