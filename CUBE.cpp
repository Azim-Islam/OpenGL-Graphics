#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>

const int width = 500;
const int height = 500;

float rot = 0;

static GLfloat v_cube[8][3] =
{ //{ x,   y,   z}
    {0.0, 0.0, 0.0}, //point index 0
    {0.0, 0.0, 1.0}, //point index 1
    {1.0, 0.0, 1.0}, //point index 2
    {1.0, 0.0, 0.0}, //point index 3
    {0.0, 1.0, 0.0}, //point index 4
    {0.0, 1.0, 1.0}, //point index 5
    {1.0, 1.0, 1.0}, //point index 6
    {1.0, 1.0, 0.0}, //point index 7
};

static GLubyte p_Indices[6][4] =
{
    {0, 1, 2, 3}, // indices for drawing the cube plane 1
    {1, 5, 6, 2}, // indices for drawing the cube plane 2
    {2, 6, 7, 3},
    {5, 4, 7, 6},
    {1, 5, 4, 0},
    {0, 4, 7, 3},

};


static GLfloat colors[5][3] =
{
    {0.0, 0.0, 1.0}, //color for point index 0
    {0.5, 0.0, 1.0}, //color for point index 1
    {0.0, 1.0, 0.0}, //color for point index 2
    {0.0, 1.0, 1.0}, //color for point index 3
    {0.8, 0.0, 0.0} //color for point index 4
};


void drawcube()
{
    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        glColor3f(0,0,1);
        glVertex3fv(&v_cube[p_Indices[i][0]][0]);
        glColor3f(1,1,0);
        glVertex3fv(&v_cube[p_Indices[i][1]][0]);
        glColor3f(1,0.5,1);
        glVertex3fv(&v_cube[p_Indices[i][2]][0]);
        glColor3f(0.5,0,1);
        glVertex3fv(&v_cube[p_Indices[i][3]][0]);
    }
    glEnd();
}





static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5, 5, -5, 5, 2.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 5, 0, 0, 0, 0, 1, 0);
    glViewport(0, 0, width, height);

    glPushMatrix();
    glRotatef(rot, 0, 1, 0);
    glTranslatef(-2,-2,-2);
    glScalef(2,2,2);
    drawcube();
    glPopMatrix();

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;

    case ',':
        rot++;
        break;
    case '.':
        rot--;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    rot+=0.005;
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

//    glClearColor(1,1,1,1);

    glEnable(GL_DEPTH_TEST);
    // glShadeModel( GL_SMOOTH ); ???
    // glEnable(GL_NORMALIZE); ???
    // glEnable(GL_BLEND); ???


    glutMainLoop();

    return EXIT_SUCCESS;
}