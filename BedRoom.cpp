#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>

const int width = 1280;
const int height = 720;
int refreshMillis = 16;      // Refresh period in milliseconds
float rot = 0;
float rotKata = 0;
float zoomX = 20;
float zoomY = 12;
float zoomZ = 16;
float eyeX = 70;
float eyeY = 40;
float eyeZ = 0;
float centerX = 10;
float centerY = 10;
float centerZ = 10;
float viewX = 0;
float viewY = 0; 


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
    {0.65, 0.41, 0.35}, //color for point index 1
    {0.69, 0.44, 0.38}, //color for point index 2
    {0.46, 0.25, 0.2}, //color for point index 3
    {0.65, 0.41, 0.35}, //color for point index 5 walll Color 2
    {0.69, 0.44, 0.38}, //color for point index 4 Wall Color 1
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




void drawcube(GLfloat r, GLfloat  g, GLfloat  b)
{   
    for (GLint i = 0; i <6; i++)
        {
        glBegin(GL_QUADS);
        glVertex3fv(&v_cube[p_Indices[i][0]][0]);
        // glColor3f(r, g, b);
        glVertex3fv(&v_cube[p_Indices[i][1]][0]);
        // glColor3f(r, g, b);
        glVertex3fv(&v_cube[p_Indices[i][2]][0]);
        // glColor3f(r, g, b);
        glVertex3fv(&v_cube[p_Indices[i][3]][0]);
            
        glEnd();
        }
}
void light0on(){
    glEnable(GL_LIGHT0);
}

void light0off(){
    glDisable(GL_LIGHT0);
}

void light()
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = { 0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 20.0, 12.0, 10.0, 1.0 };

    glEnable( GL_LIGHT0);
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    

}

void mLight(GLfloat r, GLfloat g, GLfloat b, GLfloat shiny){
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 0.5 };
    GLfloat mat_diffuse[] = { r, g, b, 0.5 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shiny};

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}
void drawroom()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.65, 0.41, 0.35, 1.0 };
    GLfloat mat_diffuse[] = { 0.65, 0.41, 0.35, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {50};

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


    for (GLint i = 0; i <6; i++){
        if (1)
            {
            glBegin(GL_QUADS);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][0]][0]);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][1]][0]);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][2]][0]);
            // glColor3fv(&colors[i][0]);
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
    mLight(r, g, b, 50);
    drawcube(r, g, b);
    glPopMatrix();
}

void drawWindow(GLfloat r, GLfloat  g, GLfloat  b){
    //window tiles
    //tile 1
    glPushMatrix();
    glScalef(0.01, 0.15, 0.15);
    glTranslatef(0, 2+0.5, 3+0.5);
    mLight(r, g, b, 50);
    drawcube(r, g, b);
    glPopMatrix();

    //tile 2
    glPushMatrix();
    glScalef(0.01, 0.15, 0.15);
    glTranslatef(0, 3.1+0.5, 4.1+0.5);
    mLight(r, g, b, 50);
    drawcube(r, g, b);
    glPopMatrix();

    //tile 3
    glPushMatrix();
    glScalef(0.01, 0.15, 0.15);
    glTranslatef(0, 2+0.5, 4.1+0.5);
    mLight(r, g, b, 50);
    drawcube(r, g, b);
    glPopMatrix();

    //tile 4
    glPushMatrix();
    glScalef(0.01, 0.15, 0.15);
    glTranslatef(0, 3.1+0.5, 3+0.5);
    mLight(r, g, b, 50);
    drawcube(r, g, b);
    glPopMatrix();

}

void drawBalish(GLfloat r, GLfloat  g, GLfloat  b){
    //Draw blaish
    glPushMatrix();
    glScalef(0.05, 0.1, 0.2);
    glTranslatef(0.6, 2, 0.2);
    mLight(1, 1, 1, 50);
    drawcube(1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.05, 0.1, 0.2);
    glTranslatef(0.6, 2, 1.3);
    mLight(1, 1, 1, 50);
    drawcube(1, 1, 1);
    glPopMatrix();
}

void drawBed(){


    //Draw bed matress
    mLight(0.94, 0.77, 0.53, 50);
    glPushMatrix();
    glScalef(0.5, 0.2, 0.5);
    drawcube(0.94, 0.77, 0.53);
    glPopMatrix();
    
    //draw bed floor
    mLight(0.73, 0.49, 0.27, 50);
    glPushMatrix();
    glScalef(0.51, 0.05, 0.51);
    drawcube(0.73, 0.49, 0.27);
    glPopMatrix();
    
    //draw bed 
    mLight(1, 1, 1, 50);
    glPushMatrix();
    glScalef(0.35, 0.07, 0.51);
    glTranslatef(0.4, 2, 0);
    drawcube(1, 1, 1);
    glPopMatrix();

}

void drawWardobe(){
    //Draw Wardrobe
    mLight(0.43, 0.17, 0.01, 50);
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glTranslatef(3, 0, 0);
    drawcube(0.43, 0.17, 0.01);
        //Draw Left Strip
        glPushMatrix();
        glScalef(0.05, 1, 0.01);
        glTranslatef(0, 0, 100);
        mLight(0.26, 0.1, 0, 50);
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
    mLight(0.9, 0.84, 0.46, 50);
    glPushMatrix();
    glScalef(0.08, 0.15, 0.15);
    glTranslatef(9, 5.3, 0);
    drawcube(0.9, 0.84, 0.46);
    glPopMatrix();

    //Draw Bottom Strip
    mLight(0.26, 0.1, 0, 50);
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
        mLight(0.73, 0.49, 0.27, 50);
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
        mLight(0, 0, 0, 50);
        drawcube(0, 0, 0);
        glPopMatrix();

        //draw bottle
        glPushMatrix();
        glScalef(0.2, 0.3, 0.1);
        glTranslatef(2, 3.3, 3);
        mLight(0.26, 0.33, 0.93, 50);
        drawcube(0.26, 0.33, 0.93);
        glPopMatrix();

    mLight(0.94, 0.77, 0.53, 50);
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
    mLight(0.87, 0.87, 0.87, 50);
    drawcube(0.87, 0.87, 0.87);
    glPopMatrix();
    mLight(1, 1, 1, 50);
    drawcube(1, 1, 1);
    glPopMatrix();

}

void drawWallClock(){
    //draw a wall clock
    glPushMatrix();
    glScalef(0.01, 0.3, 0.3);
    glTranslatef(1, 1+0.8, 1-0.5);
        glPushMatrix();
        glTranslatef(0.2, 0, 0);
        glScalef(1, 0.1, 0.1);
        glTranslatef(0, 4.5, 4.5);
            //ghorir kata.exe
            glPushMatrix();
            glScalef(1, 1, 3);
            glTranslatef(0, 0, 0.5);
            mLight(0, 0, 0, 50);
            drawcube(0, 0, 0);
            glPopMatrix();
            //ghorir kata.exe
            glPushMatrix();
            glScalef(1, 3, 0.4);
            glTranslatef(0, 0.5, 0);
                glPushMatrix();
                glTranslatef(1, 1, 1);
                glRotatef(rotKata, 1, 0, 0);
                glTranslatef(-1, -1, -1);
                mLight(0, 0, 0, 50);
                drawcube(0, 0, 0);
                glPopMatrix();
            glPopMatrix();
        mLight(0, 0, 1, 50);
        drawcube(0, 0, 1);
        glPopMatrix();
    mLight(1, 1, 1, 50);
    drawcube(1, 1, 1);
    glPopMatrix();

    glPopMatrix();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glFrustum(-10, 10, -10, 10, 1.0, 100.0);
    gluPerspective(90.0, 16/9.0, 1.0, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
    glViewport(0, 0, width, height);

    glPushMatrix();
    glRotatef(-45, 0, 1, 0); //initial rotation
    glRotatef(rot, 0, 1, 0);
    glTranslatef(-20, -10, 0);


    glScalef(zoomX, zoomY, zoomZ);

    drawroom();
    drawWindow(0.87, 0.92, 0.97);
    drawFloorMatt(0.13, 0.47, 0.53);
    drawBalish(1, 1, 1);
    drawBed();
    drawWardobe();
    drawSideTable();
    drawTubeLight();
    drawWallClock();
    




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
        zoomX *= 1.2;
        zoomY *= 1.2;
        zoomZ *= 1.2;
        break;
    case '_':
        zoomX /= 1.2;
        zoomY /= 1.2;
        zoomZ /= 1.2;
        break;
    case 'w':
        eyeY += 2;
        break;
    case 's':
        eyeY -= 2;
        break;
    case 'a':
        eyeX += 2;
        break;
    case 'd':
        eyeX -= 2;
        break;
    case 'q':
        eyeZ += 2;
        break;
    case 'e':
        eyeZ -= 2;
        break;
    case 'u':
        centerY += 2;
        break;
    case 'j':
        centerY -= 2;
        break;
    case 'h':
        centerX += 2;
        break;
    case 'k':
        centerX -= 2;
        break;
    case 'y':
        centerZ += 2;
        break;
    case 'i':
        centerX -= 2;
        break;
    case '1':
        light0off();
        break;
    case '2':
        light0on();
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
    glEnable(GL_LIGHTING);
    light();


    glutMainLoop();

    return EXIT_SUCCESS;
}