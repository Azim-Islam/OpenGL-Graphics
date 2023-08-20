#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>

const int width = 1000;
const int height = 1000;

float rot = 0;

static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0}, //point index 0
    {0.0, 0.0, 2.0}, //point index 1
    {2.0, 0.0, 2.0}, //point index 2
    {2.0, 0.0, 0.0}, //point index 3
    {1.0, 4.0, 1.0} //point index 4
};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2}, // indices for drawing the triangle plane 1
    {4, 2, 3}, // indices for drawing the triangle plane 2
    {4, 3, 0}, // indices for drawing the triangle plane 3
    {4, 0, 1} // indices for drawing the triangle plane 4
};

static GLubyte quadIndices[1][4] =
{
    {0, 3, 2, 1}
}; // indeces for drawing the quad plane

static GLfloat colors[5][3] =
{
    {0.0, 0.0, 1.0}, //color for point index 0
    {0.5, 0.0, 1.0}, //color for point index 1
    {0.0, 1.0, 0.0}, //color for point index 2
    {0.0, 1.0, 1.0}, //color for point index 3
    {0.8, 0.0, 0.0} //color for point index 4
};

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}


void drawpyramid(GLfloat R = 1.0, GLfloat G = 0.0, GLfloat B = 0.0)
{

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { R*0.4, G*0.4, B*0.4, 1.0 };
    GLfloat mat_diffuse[] = { R, G, B, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <4; i++)
    {
        for (GLint j = 0; j < 3; j++)
        glVertex3fv(&v_pyramid[p_Indices[i][j]][0]);
    }
    glEnd();
    glBegin(GL_QUADS);
    for (GLint i = 0; i < 1; i++)
    {
        for (GLint j = 0; j < 4; j++) glVertex3fv(&v_pyramid[quadIndices[i][j]][0]);
    }
    glEnd();
}


void light()
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = { 0.3, 0.3, 0.3, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 2.0, 10.0, 2.0, 1.0 };

    glEnable( GL_LIGHT0);
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);

}


static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5, 5, -5, 5, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0, 5, 0, 0, 0, 0,1,0);
    glViewport(0, 0, width, height);

    glPushMatrix();
    glTranslatef(0, -2, -2);
    glRotatef(rot, 0, 1, 0);
    glScalef(1.5, 1.5, 1.5);
    drawpyramid(1.0, 0.0, 1.0);
    //glutSolidSphere(3, 100, 100);
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

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    light();

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    //glutIdleFunc(idle);



    glutMainLoop();

    return EXIT_SUCCESS;
}
