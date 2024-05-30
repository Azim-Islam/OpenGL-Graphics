/*
SPACEBAR - toggle FPS control
W,A,S,D - to move
mouse - look around, inverted mouse
left/right mouse - fly up/down
ESC - quit
*/

#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Camera.h"
#include <string>
#include <cstdlib> 
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cstring>
using namespace std;

void Display();
void Reshape (int w, int h);
void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void MouseMotion(int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Idle();
unsigned int ID;
void Grid();
void initTrack();

Camera g_camera;
bool g_key[256];
bool g_shift_down = false;
bool g_fps_mode = false;
int g_viewport_width = 0;
int g_viewport_height = 0;
bool g_mouse_left_down = false;
bool g_mouse_right_down = false;
float zoomX = 3;
float zoomY = 3;
float zoomZ = 3;
float rot = 0;
static int carPos = 0;
float global_ambient = 1;
float global_diffuse = 1;
float global_ref = 1;
std::vector<int> randomNumbers;
std::vector<std::vector<float>> trackCords;


// Movement settings
const float g_translation_speed = 0.5;
const float g_rotation_speed = M_PI/180*0.2;

struct Element {
    int t;
    unsigned char * data;
};

static std::unordered_map<GLuint, Element> textures;



GLuint LoadTexture(GLuint tex, const char * filename, int width, int height)
{

    // Use std::find to check if the target integer is present in the vector
    std::size_t hashValue = std::hash<std::string>{}(filename);
    
    // Convert the hash value to int
    GLuint tex1 = static_cast<GLuint>(hashValue);
    
    // cout << tex << endl;
    // cout << textures.count() << endl;
    if (textures.count(tex1)>0) {
        glBindTexture(GL_TEXTURE_2D, tex);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, textures[tex1].data);
        return 0;

    } else { //paynai
        //bmp 24 bit
        // cout << "paynai" << endl;

        unsigned char * data;
        unsigned char R,G,B;
        FILE * file;
        
        //open .bmp
        file = fopen(filename, "rb");

        if(file == NULL)return 0;
        //get memory for data
        data =(unsigned char *)malloc(width * height * 3);
        //data skip offset 
        fseek(file,128,0);
        //read file to data
        fread(data, width * height * 3, 1, file);
        //close file
        fclose(file);

        //transpose R,G,B values
        int index;
        for(int i = 0; i < width * height ; ++i)
            {
            index = i*3;
            B = data[index]; G = data[index+1]; R = data[index+2];
            data[index] = G; data[index+1] = R; data[index+2] = B;
            }
            //create a texture
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

        //texture filtering
        glPixelStorei(GL_UNPACK_ALIGNMENT, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        Element obj;
        obj.t = tex;
        obj.data = data;
        textures[tex1] = obj;
        // cout << textures[tex1].t << endl;
        // free(data);
        return 0;

    }
    
}



static GLfloat v_cube[8][3] =
{ //{ x,   y,   z}
    {1.0, 1.0, 0.0}, //point index 7
    {1.0, 1.0, 1.0}, //point index 6
    {0.0, 1.0, 1.0}, //point index 5
    {0.0, 1.0, 0.0}, //point index 4
    {1.0, 0.0, 0.0}, //point index 3
    {1.0, 0.0, 1.0}, //point index 2
    {0.0, 0.0, 1.0}, //point index 1
    {0.0, 0.0, 0.0}, //point index 0
};

static GLubyte p_Indices[6][4] =
{
    // {0, 4, 7, 3}, //6
    // {1, 5, 4, 0}, //5
    // {5, 4, 7, 6}, //4
    // {2, 6, 7, 3}, //3
    // {1, 5, 6, 2}, // indices for drawing the cube plane 2
    // {0, 1, 2, 3}, // indices for drawing the cube plane 1
    {3, 7, 4, 0},
    {0, 4, 5, 1},
    {6, 7, 4, 5},
    {3, 7, 6, 2},
    {2, 6, 5, 1},
    {3, 2, 1, 0},

};

void genRandomNumbers(){
    int numElements = 25; 
    for (int i = 0; i < numElements; ++i) {
        int randomNumber = std::rand();
        randomNumbers.push_back(randomNumber%10);
    }
}

void light()
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = { 0.5*global_ambient, 0.5*global_ambient, 0.5*global_ambient, 1.0};
    GLfloat light_diffuse[]  = { 1*global_diffuse, 1*global_diffuse, 1*global_diffuse, 1.0 };
    GLfloat light_specular[] = { 1.0*global_ref, 1.0*global_ref, 1.0*global_ref, 1.0 };
    GLfloat light_position[] = { 20.0, 12.0, 10.0, 1.0 };
    
    glEnable( GL_LIGHT0 );
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    

}
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


void mLight(GLfloat r, GLfloat g, GLfloat b, GLfloat shiny){
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r*0.5*global_ambient, g*0.5*global_ambient, b*0.5*global_ambient, 1 };
    GLfloat mat_diffuse[] = { r*global_diffuse, g *global_diffuse, b*global_diffuse, 0.5 };
    GLfloat mat_specular[] = { 1.0*global_ref, 1.0*global_ref, 1.0*global_ref, 1.0 };
    GLfloat mat_shininess[] = {shiny};

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void drawBuilding(int i, std::string j, int w, int h)
{
    mLight(1, 1, 1, 80);
    glEnable(GL_TEXTURE_2D);
    LoadTexture(i, j.c_str(), w, h);
    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++){
        if (1)
            {
            getNormal3p(v_cube[p_Indices[i][0]][0], v_cube[p_Indices[i][0]][1], v_cube[p_Indices[i][0]][2],
                    v_cube[p_Indices[i][1]][0], v_cube[p_Indices[i][1]][1], v_cube[p_Indices[i][1]][2],
                    v_cube[p_Indices[i][2]][0], v_cube[p_Indices[i][2]][1], v_cube[p_Indices[i][2]][2]);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][0]][0]);glTexCoord2f(1,0);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][1]][0]);glTexCoord2f(0,0);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][2]][0]);glTexCoord2f(0,1);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][3]][0]);glTexCoord2f(1,1);
            }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

int binomialCoefficient(int n, int k) {
    if (k > n - k) {
        k = n - k;
    }

    int result = 1;
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }

    return result;
}

void drawTrack(){
    mLight(105/255, 105/255, 105/255, 1);
    glPushMatrix();
    glTranslatef(8.5, 0, 5);
    // glDisable(GL_LIGHT0);
    std::vector<std::pair<float, float>> control_points = {
    {-6.0f, 6.0f},
    {-3.0f, -6.0f},
    {3.0f, -6.0f},
    {6.0f, 6.0f}
    };

    // Number of subdivisions for the curve
    int subdivisions = 1000;
    float wd = 0.1;
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(10.0f);
    int n = control_points.size() - 1;
    glBegin(GL_QUADS);
    for (int i = 0; i <= subdivisions; ++i) {
        float t = static_cast<float>(i) / subdivisions;
        float x = 0.0f, y = 0.0f;

        for (int j = 0; j <= n; ++j) {
            float b = static_cast<float>(binomialCoefficient(n, j) * pow(t, j) * pow(1 - t, n - j));
            x += b * control_points[j].first;
            y += b * control_points[j].second;
        }
        glVertex3f(x+wd, 0.12, y+wd);glTexCoord2f(1,0);
        glVertex3f(x+wd, 0.12, y-wd);glTexCoord2f(0,0);
        glVertex3f(x-wd, 0.12, y-wd);glTexCoord2f(0,1);
        glVertex3f(x-wd, 0.12, y+wd);glTexCoord2f(1,1);

    }
    glEnd();
    glLineWidth(2.0f);
    glPopMatrix();


}

void genBuilding(int i, int j){
        int v = randomNumbers[(i+j)%25]+1;
        for(int k=0; k<=v; k++){
            glPushMatrix();
            std::string str1 = "/home/oggy/Desktop/Computer-Graphics-Project/texture/building";
            std::string str2 = std::to_string(randomNumbers[i%7]+1);
            std::string str3 = ".bmp";
            std::string str34 = str1+str2+str3;
            glTranslatef(i/5, 0.09+k, -j*3);
            // glScalef(1, randomNumbers[(i+j)%25]+1, 1);
            
            drawBuilding(i%7+2, str34, 512, 512);
            glPopMatrix();
        }


}

void genCity(){
    for(int j=1; j < 5; j++){
        for(int i=10; i <= 70; i += 10){
            glPushMatrix();
            std::string str1 = "/home/oggy/Desktop/Computer-Graphics-Project/texture/building";
            std::string str2 = std::to_string(i/10);
            std::string str3 = ".bmp";
            std::string str34 = str1+str2+str3;
            glTranslatef(i/5, 0.09, j+10);
            glScalef(1, randomNumbers[i%25], 1);
            drawBuilding(i%7+2, str34, 512, 512);
            glPopMatrix();
        }

    }
    for(int j=1; j < 5; j++){
        for(int i=10; i <= 70; i += 10){
            genBuilding(i, j);
        }
    }

}

int main (int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("FPS by Azim Islam - Hit SPACEBAR to toggle FPS mode");
    initTrack();
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    light();
    glShadeModel( GL_SMOOTH );
    glutIgnoreKeyRepeat(1);
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(Display);
    glutIdleFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MouseMotion);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutIdleFunc(Idle);
    genRandomNumbers();
    glutTimerFunc(1, Timer, 0);
    glutMainLoop();

    return 0;
}

void Grid()
{
    glPushMatrix();
    glColor3f(1,1,1);

    for(int i=-50; i < 50; i++) {
        glBegin(GL_LINES);
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);
        glEnd();
    }

    for(int i=-50; i < 50; i++) {
        glBegin(GL_LINES);
        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
        glEnd();
    }

    glPopMatrix();
}

void drawGrass(float x, float y, float z){
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x-10, y, z);
    glScalef(10, 0.01, 10);
    std::string str1 = "/home/oggy/Desktop/Computer-Graphics-Project/texture/grass";
    std::string str2 = std::to_string(1000);
    std::string str3 = ".bmp";
    std::string str34 = str1+str2+str3;
    drawBuilding(1000, str34.c_str(), 512, 512);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawMati(){
    glPushMatrix();
    mLight(1, 1, 1, 80);
    for(int i = 0; i<25; i+= 1){
        drawGrass(i, 0.1, 0);
    }
    for(int i = 0; i<25; i+= 1){
        drawGrass(i, 0.1, 10);
    }

    glPopMatrix();

    glPushMatrix();
    // mLight(1, 1, 1, 80);
    // for(int i = 0; i<25; i+= 1){
    //     drawGrass(i, 0.1, 0);
    // }
    for(int i = 0; i<25; i+= 1){
        drawGrass(i, 0.1, -10);
    }
        for(int i = 0; i<25; i+= 1){
        drawGrass(i, 0.1, -15);
    }


    glPopMatrix();


}

void renderText(const char* string, float x, float y) {
    int j = strlen( string );

    glColor3f( 1, 1, 1 );
    glRasterPos2f( x, y );
    for( int i = 0; i < j; i++ ) {
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, string[i] );
    }
}

void draw_cylinder(GLfloat radius,
                   GLfloat height,
                   GLubyte R,
                   GLubyte G,
                   GLubyte B)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glColor3ub(R-40,G-40,B-40);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*3.14159 ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glEnable(GL_TEXTURE_2D);
    std::string str1 = "/home/oggy/Desktop/Computer-Graphics-Project/texture/tire_rim.bmp";
    GLint i = 222;
    LoadTexture(i, str1.c_str(), 512, 512);
    glColor3ub(R,G,B);
    glBegin(GL_POLYGON);
    GLfloat tx, ty;
    angle = 0.0;
        while( angle < 2*3.14159 ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            tx = (x/radius + 1)*0.5;
            ty = (y/radius + 1)*0.5;
            glVertex3f(x, y , height);
            glTexCoord2f(tx, ty);
            angle = angle + angle_stepsize;
        }
        tx = (radius/radius + 1)*0.5;
        ty = (0.0/radius + 1)*0.5;
        glVertex3f(radius, 0.0, height);
        glTexCoord2f(tx, ty);
        
    glEnd();
    glDisable(GL_TEXTURE_2D);


    /** Draw the circle on bottom of cylinder */
    glEnable(GL_TEXTURE_2D);
    LoadTexture(i, str1.c_str(), 512, 512);
    glColor3ub(R,G,B);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*3.14159 ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            tx = (x/radius + 1)*0.5;
            ty = (y/radius + 1)*0.5;
            glVertex3f(x, y , 0.0);
            glTexCoord2f(tx, ty);
            angle = angle + angle_stepsize;
        }
        tx = (radius/radius + 1)*0.5;
        ty = (0.0/radius + 1)*0.5;
        glVertex3f(radius, 0.0, 0.0);
        glTexCoord2f(tx, ty);
    glEnd();
    glDisable(GL_TEXTURE_2D);


}

void drawTire(){
    glPushMatrix();
    glTranslatef(0, 5, 0);
    draw_cylinder(2, 2, 1, 1, 1);
    glPopMatrix();

}

void carBox(GLfloat r, GLfloat g, GLfloat b, GLfloat s){

    mLight(r, g, b, s);

    std::string str1 = "/home/oggy/Desktop/Computer-Graphics-Project/texture/tire_rim.bmp";
    GLint i = 225;
    // LoadTexture(i, str1.c_str(), 512, 512);

    // glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++){
        if (1)
            {
            getNormal3p(v_cube[p_Indices[i][0]][0], v_cube[p_Indices[i][0]][1], v_cube[p_Indices[i][0]][2],
                    v_cube[p_Indices[i][1]][0], v_cube[p_Indices[i][1]][1], v_cube[p_Indices[i][1]][2],
                    v_cube[p_Indices[i][2]][0], v_cube[p_Indices[i][2]][1], v_cube[p_Indices[i][2]][2]);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][0]][0]);glTexCoord2f(1,0);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][1]][0]);glTexCoord2f(0,0);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][2]][0]);glTexCoord2f(0,1);
            // glColor3fv(&colors[i][0]);
            glVertex3fv(&v_cube[p_Indices[i][3]][0]);glTexCoord2f(1,1);
            }
    }
    glEnd();
    // glDisable(GL_TEXTURE_2D);
}

void drawCarBody1(){
    glPushMatrix();
    glTranslatef(-3, 6, -1.5);
        glPushMatrix();
        glScalef(22, 5, 15);
        carBox(0.1, 0.5, 0.3, 50);
        glPopMatrix();
    glPopMatrix();
}

void drawCarBody2(){
    glPushMatrix();
    glTranslatef(0, 10, -1.5);
        glPushMatrix();
        glScalef(22/2, 6, 15);
        carBox(0.1, 0.2, 0.3, 50);
        glPopMatrix();
    glPopMatrix();

}

void drawCarBody(){
    glPushMatrix();
    drawTire();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 10);
    drawTire();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 0, 0);
    drawTire();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 0, 10);
    drawTire();
    glPopMatrix();


    glPushMatrix();
    glPopMatrix();

    drawCarBody1();
    drawCarBody2();

}
void drawCar(float x, float y, float z){
    glPushMatrix();
    glTranslatef(x+8.45, y, z+5);
        glPushMatrix();
        float c = trackCords.size()-carPos%trackCords.size();
        float r = -60 + 110*(c)/trackCords.size();
        glRotatef(r, 0, 1, 0);
            glPushMatrix();
            glScalef(0.01, 0.01, 0.01);
            drawCarBody();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

}

void initTrack(){
    // glTranslatef(8.5, 0, 5);
    std::vector<std::pair<float, float>> control_points = {
    {-6.0f, 6.0f},
    {-3.0f, -6.0f},
    {3.0f, -6.0f},
    {6.0f, 6.0f}
    };

    // Number of subdivisions for the curve
    int subdivisions = 10000;
    float wd = 0.1;
    int n = control_points.size() - 1;
    for (int i = 0; i <= subdivisions; ++i) {
        float t = static_cast<float>(i) / subdivisions;
        float x = 0.0f, y = 0.0f;

        for (int j = 0; j <= n; ++j) {
            float b = static_cast<float>(binomialCoefficient(n, j) * pow(t, j) * pow(1 - t, n - j));
            x += b * control_points[j].first;
            y += b * control_points[j].second;
        }
        trackCords.push_back({x, 0.09, y});
    }

}


void Display (void) {
    carPos += 10;
    glClearColor (0.0,0.0,0.0,1.0); //clear the screen to black
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glLoadIdentity();

	g_camera.Refresh();

    glColor3f(0,1,0);

    // glutWireTeapot(0.5);
    glPushMatrix();
    glRotatef(rot, 0, 1, 0);
    glTranslatef(-20, 0, 0);


    glScalef(zoomX, zoomY, zoomZ);
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(20, 12, 10);
    glutSolidSphere (1.0, 50, 50);
    glPopMatrix();
    drawMati();
    drawTrack();
    drawCar(trackCords[carPos%trackCords.size()][0], trackCords[carPos%trackCords.size()][1], trackCords[carPos%trackCords.size()][2]);
    genCity();
    glPopMatrix();
    Grid();
    

    glutSwapBuffers(); //swap the buffers
}

void Reshape (int w, int h) {
    g_viewport_width = w;
    g_viewport_height = h;

    glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION); //set the matrix to projection

    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1 , 100.0); //set the perspective (angle of sight, width, height, ,depth)
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
}

void Keyboard(unsigned char key, int x, int y)
{
    if(key == 27) {
        exit(0);
    }

    if(key == ' ') {
        g_fps_mode = !g_fps_mode;

        if(g_fps_mode) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(g_viewport_width/2, g_viewport_height/2);
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    if(key == '1'){
    glEnable(GL_LIGHT0);
    }
    if(key == '2'){
    glDisable(GL_LIGHT0);
    }
    if(key== '3'){
        global_ambient = global_ambient == 1 ? 0:1;
    }
    if(key== '4'){
        global_diffuse = global_diffuse == 1 ? 0:1;
    }
    if(key== '5'){
        global_ref = global_ref == 1 ? 0:1;
    }

    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        g_shift_down = true;
    }
    else {
        g_shift_down = false;
    }

    g_key[key] = true;
}

void KeyboardUp(unsigned char key, int x, int y)
{
    g_key[key] = false;
}

void Timer(int value)
{
    if(g_fps_mode) {
        if(g_key['w'] || g_key['W']) {
            g_camera.Move(g_translation_speed);
        }
        else if(g_key['s'] || g_key['S']) {
            g_camera.Move(-g_translation_speed);
        }
        else if(g_key['a'] || g_key['A']) {
            g_camera.Strafe(g_translation_speed);
        }
        else if(g_key['d'] || g_key['D']) {
            g_camera.Strafe(-g_translation_speed);
        }
        else if(g_mouse_left_down) {
            g_camera.Fly(-g_translation_speed);
        }
        else if(g_mouse_right_down) {
            g_camera.Fly(g_translation_speed);
        }
    }

    glutTimerFunc(1, Timer, 0);
}

void Idle()
{
    Display();
}

void Mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN) {
        if(button == GLUT_LEFT_BUTTON) {
            g_mouse_left_down = true;
        }
        else if(button == GLUT_RIGHT_BUTTON) {
            g_mouse_right_down = true;
        }
    }
    else if(state == GLUT_UP) {
        if(button == GLUT_LEFT_BUTTON) {
            g_mouse_left_down = false;
        }
        else if(button == GLUT_RIGHT_BUTTON) {
            g_mouse_right_down = false;
        }
    }
}

void MouseMotion(int x, int y)
{
    // This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
    // This avoids it being called recursively and hanging up the event loop
    static bool just_warped = false;

    if(just_warped) {
        just_warped = false;
        return;
    }

    if(g_fps_mode) {
        int dx = x - g_viewport_width/2;
        int dy = y - g_viewport_height/2;

        if(dx) {
            g_camera.RotateYaw(g_rotation_speed*dx);
        }

        if(dy) {
            g_camera.RotatePitch(g_rotation_speed*dy);
        }

        glutWarpPointer(g_viewport_width/2, g_viewport_height/2);

        just_warped = true;
    }
}
