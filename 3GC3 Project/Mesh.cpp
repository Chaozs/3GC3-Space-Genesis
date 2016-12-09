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
    if( file == NULL )
    {
        printf("Impossible to open the file !\n");
        return;
    }

    while(true)
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
        {
            break; // EOF = End Of File. Quit the loop.
        }
        // else : parse lineHeader
        //VERTICES
        float input1;
        float input2;
        float input3;
        if ( strcmp( lineHeader, "v" ) == 0 )
        {
            fscanf(file, "%f %f %f\n", &input1, &input2, &input3);
            Vector3 v = Vector3(input1, input2, input3);
            vertices.push_back(v);
        }
        else if ( strcmp( lineHeader, "vn" ) == 0 )
        {
            fscanf(file, "%f %f %f\n", &input1, &input2, &input3);
            Vector3 v = Vector3(input1, input2, input3);
            normals.push_back(v);
        }

        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%i/%i/%i %i/%i/%i %i/%i/%i\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9)
            {
                printf("\n Get rid of the N-gons, moron.\n");
                return;
            }
            else
            {
                vIndex.push_back(vertexIndex[0]);
                vIndex.push_back(vertexIndex[1]);
                vIndex.push_back(vertexIndex[2]);
                nIndex.push_back(normalIndex[0]);
                nIndex.push_back(normalIndex[1]);
                nIndex.push_back(normalIndex[2]);
            }
        }
    }
    /*
      printf("%i\n", vIndex.size());
       for(int i = 0; i < vertices.size(); i++)
      {
          //if(i%3 == 0)
          //{
               printf("\n");
          //}
          printf("%f", vertices[i].x);
      }
      printf("\n");
      printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
      for(int i = 0; i < vIndex.size(); i++)
      {
          if(i%3 == 0)
          {
               printf("\n");
          }
          printf("%i - ", vIndex[i]);
      }*/

}

bool fired = false;

void Mesh::Draw()
{
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_TRIANGLES);
    int i = 0;
    while(i < vIndex.size())
    {        
        glNormal3f(normals[nIndex[i]-1].x, normals[nIndex[i]-1].y, normals[nIndex[i]-1].z);
        glVertex3f(vertices[vIndex[i]-1].x, vertices[vIndex[i]-1].y, vertices[vIndex[i]-1].z);      
        i++;
        glNormal3f(normals[nIndex[i]-1].x, normals[nIndex[i]-1].y, normals[nIndex[i]-1].z);
        glVertex3f(vertices[vIndex[i]-1].x, vertices[vIndex[i]-1].y, vertices[vIndex[i]-1].z);
        i++;
        glNormal3f(normals[nIndex[i]-1].x, normals[nIndex[i]-1].y, normals[nIndex[i]-1].z);
        glVertex3f(vertices[vIndex[i]-1].x, vertices[vIndex[i]-1].y, vertices[vIndex[i]-1].z);
        i++;
        
    }
    glEnd();
    /*glBegin(GL_POINTS);
    for(int i = 0; i < vertices.size(); i++)
    {
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }*/
    //glEnd();
    fired = true;
}
