#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>

const int width = 1280;
const int height = 720;
int refreshMillis = 16;      // Refresh period in milliseconds
float rot = 0;
float zoomX = 20;
float zoomY = 12;
float zoomZ = 16;



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


static GLfloat colors[6][3] =
{
    {0.46, 0.25, 0.2}, //color for point index 0 Floor Color
    {0.5, 0.0, 1.0}, //color for point index 1
    {0.0, 1.0, 0.0}, //color for point index 2
    {0.0, 1.0, 1.0}, //color for point index 3
    {0.65, 0.41, 0.35}, //color for point index 5 walll Color 2
    {0.69, 0.44, 0.38}, //color for point index 4 Wall Color 1
};


void drawcube(GLfloat r, GLfloat  g, GLfloat  b)
{   
    for (GLint i = 0; i <6; i++)
        {
        glBegin(GL_QUADS);
        glVertex3fv(&v_cube[p_Indices[i][0]][0]);
        glColor3f(r, g, b);
        glVertex3fv(&v_cube[p_Indices[i][1]][0]);
        glColor3f(r, g, b);
        glVertex3fv(&v_cube[p_Indices[i][2]][0]);
        glColor3f(r, g, b);
        glVertex3fv(&v_cube[p_Indices[i][3]][0]);
            
        glEnd();
        }
}

void drawroom()
{
    for (GLint i = 0; i <6; i++){
        if (i == 0 || i == 4 || i == 5)
            {
            glBegin(GL_QUADS);
            glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][0]][0]);
            glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][1]][0]);
            glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][2]][0]);
            glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][3]][0]);   
            glEnd();
            }
    }
}

void drawFloorMatt(GLfloat r, GLfloat  g, GLfloat  b){
    //draw floor matt
    glPushMatrix();
    glScalef(0.3, 0.02, 0.3);
    glTranslatef(0.5, 0, 2);
    drawcube(r, g, b);
    glPopMatrix();
}

void drawWindow(GLfloat r, GLfloat  g, GLfloat  b){
    //window tiles
    //tile 1
    glPushMatrix();
    glScalef(0.01, 0.15, 0.15);
    glTranslatef(0, 2+0.5, 3+0.5);
    drawcube(r, g, b);
    glPopMatrix();

    //tile 2
    glPushMatrix();
    glScalef(0.01, 0.15, 0.15);
    glTranslatef(0, 3.1+0.5, 4.1+0.5);
    drawcube(r, g, b);
    glPopMatrix();

    //tile 3
    glPushMatrix();
    glScalef(0.01, 0.15, 0.15);
    glTranslatef(0, 2+0.5, 4.1+0.5);
    drawcube(r, g, b);
    glPopMatrix();

    //tile 4
    glPushMatrix();
    glScalef(0.01, 0.15, 0.15);
    glTranslatef(0, 3.1+0.5, 3+0.5);
    drawcube(r, g, b);
    glPopMatrix();

}


void drawBalish(GLfloat r, GLfloat  g, GLfloat  b){
    //Draw blaish
    glPushMatrix();
    glScalef(0.05, 0.1, 0.2);
    glTranslatef(0.6, 2, 0.2);
    drawcube(1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.05, 0.1, 0.2);
    glTranslatef(0.6, 2, 1.3);
    drawcube(1, 1, 1);
    glPopMatrix();
}

void drawBed(){
    //Draw bed matress
    glPushMatrix();
    glScalef(0.5, 0.2, 0.5);
    drawcube(0.94, 0.77, 0.53);
    glPopMatrix();
    
    //draw bed floor
    glPushMatrix();
    glScalef(0.51, 0.05, 0.51);
    drawcube(0.73, 0.49, 0.27);
    glPopMatrix();
    
    //draw bed 
    glPushMatrix();
    glScalef(0.35, 0.07, 0.51);
    glTranslatef(0.4, 2, 0);
    drawcube(1, 1, 1);
    glPopMatrix();

}

void drawWardobe(){
    //Draw Wardrobe
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glTranslatef(3, 0, 0);
    drawcube(0.43, 0.17, 0.01);
        //Draw Left Strip
        glPushMatrix();
        glScalef(0.05, 1, 0.01);
        glTranslatef(0, 0, 100);
        drawcube(0.26, 0.1, 0);
        glPopMatrix();
        //Draw Middle Strip
        glPushMatrix();
        glScalef(0.05, 1, 0.01);
        glTranslatef(10, 0, 100);
        drawcube(0.26, 0.1, 0);
        glPopMatrix();
        //Draw Right Strip
        glPushMatrix();
        glScalef(0.05, 1, 0.01);
        glTranslatef(20, 0, 100);
        drawcube(0.26, 0.1, 0);
        glPopMatrix();
    glPopMatrix();

    //Draw Box
    glPushMatrix();
    glScalef(0.08, 0.15, 0.15);
    glTranslatef(9, 5.3, 0);
    drawcube(0.9, 0.84, 0.46);
    glPopMatrix();

    //Draw Bottom Strip
    glPushMatrix();
    glScalef(0.23, 0.03, 0.23);
    glTranslatef(2.55, 0, 0);
    drawcube(0.26, 0.1, 0);
    glPopMatrix();

    //Draw Top Strip
    glPushMatrix();
    glScalef(0.23, 0.03, 0.23);
    glTranslatef(2.55, 26, 0);
    drawcube(0.26, 0.1, 0);
    glPopMatrix();
}

void drawSideTable(){
        //Draw sidetable
    glPushMatrix();
    glScalef(0.05, 0.3, 0.2);
    glTranslatef(0, 0, 2.5);
        //draw bottom strip
        glPushMatrix();
        glScalef(1.1, 0.1, 1.1);
        glTranslated(0, 0, -0.05);
        drawcube(0.73, 0.49, 0.27);
        glPopMatrix();

        //draw middle strip
        glPushMatrix();
        glScalef(0.1, 0.1, 1);
        glTranslated(9.5, 4, 0);
        drawcube(0.73, 0.49, 0.27);
        glPopMatrix();

        //draw top strip
        glPushMatrix();
        glScalef(0.1, 0.1, 1);
        glTranslated(9.5, 8, 0);
        drawcube(0.73, 0.49, 0.27);
        glPopMatrix();

        //draw phone
        glPushMatrix();
        glScalef(0.25, 0.05, 0.1);
        glTranslatef(2, 20, 5);
        drawcube(0, 0, 0);
        glPopMatrix();

        //draw bottle
        glPushMatrix();
        glScalef(0.2, 0.3, 0.1);
        glTranslatef(2, 3.3, 3);
        drawcube(0.26, 0.33, 0.93);
        glPopMatrix();


    drawcube(0.94, 0.77, 0.53);
    glPopMatrix();

}

void drawTubeLight(){
    //draw tube light
    glPushMatrix();
    glScalef(0.2, 0.07, 0.01);
    glTranslatef(1, 11, 0.2);
    glPushMatrix();
    glScalef(1.1, 1.1, 0.1);
    drawcube(0.87, 0.87, 0.87);
    glPopMatrix();
    drawcube(1, 1, 1);
    glPopMatrix();

}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glFrustum(-10, 10, -10, 10, 1.0, 100.0);
    gluPerspective(45.0, 16/9.0, 1.0, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 100, 0, 0, 0, 0, 1, 0);
    glViewport(0, 0, width, height);

    glPushMatrix();
    glRotatef(-0, 0, 1, 0); //initial rotation
    glRotatef(rot, 0, 1, 0);
    glTranslatef(-40, -20, 0);

    glScalef(zoomX, zoomY, zoomZ);

    drawroom();
    drawWindow(0.87, 0.92, 0.97);
    drawFloorMatt(0.13, 0.47, 0.53);
    drawBalish(1, 1, 1);
    drawBed();
    drawWardobe();
    drawSideTable();
    drawTubeLight();


    glPopMatrix();
    




    glutSwapBuffers();
}

void Timer(int value) {
   glutPostRedisplay();    // Post a paint request to activate display()
   glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
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
        rot++;
        rot++;
        break;
    case '.':
        rot--;
        rot--;
        rot--;
        break;
    case '+':
        zoomX *= 2;
        zoomY *= 2;
        zoomZ *= 2;
        break;
    case '_':
        zoomX /= 2;
        zoomY /= 2;
        zoomZ /= 2;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    // rot+=0.005;
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

//    glClearColor(1,1,1,1);

    glEnable(GL_DEPTH_TEST);
    glShadeModel( GL_SMOOTH ); //???
    glEnable(GL_NORMALIZE); //???
    glEnable(GL_BLEND); //???

    glutMainLoop();

    return EXIT_SUCCESS;
}