from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

def init():
    ctrlpoints =   [
        [(0, -1.5, 4.0), (0, -1.5, 2.0), (0.5, -1.5, -1.0), (1.5, -1.5, 2.0)], 

        [(0, -0.5, 1.0), (0, -0.5, 3.0), (0.5, -0.5, 0.0), (1.5, -0.5, -1.0)], 
    
        [(0, 0.5, 4.0), (-0.5, 0.5, 0.0), (0.5, 0.5, 3.0), (1.5, 0.5, 4.0)], 

        [(0, 1.5, -2.0), (-0.5, 1.5, -2.0), (0.5, 1.5, 0.0), (1.5, 1.5, -1.0)]
        ]
    
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, ctrlpoints);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

def draw_cube(r:float,  g:float, b:float):
    indices =     [
    (0, 1, 2, 3), 
    (1, 5, 6, 2),
    (2, 6, 7, 3),
    (5, 4, 7, 6),
    (1, 5, 4, 0),
    (0, 4, 7, 3),
    ]
    points = [(0.0, 0.0, 0.0), #point index 0
    (0.0, 0.0, 1.0), #point index 1
    (1.0, 0.0, 1.0), #point index 2
    (1.0, 0.0, 0.0), #point index 3
    (0.0, 1.0, 0.0), #point index 4
    (0.0, 1.0, 1.0), #point index 5
    (1.0, 1.0, 1.0), #point index 6
    (1.0, 1.0, 0.0),] #point index 7
    
    for i in range(6):
        glBegin(GL_QUADS)
        glVertex3fv(points[indices[i][0]]);
        glColor3f(r, g, b);
        glVertex3fv(points[indices[i][1]]);
        glColor3f(r, g, b);
        glVertex3fv(points[indices[i][2]]);
        glColor3f(r, g, b);
        glVertex3fv(points[indices[i][3]]);
        glEnd()


def draw_track():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); # type: ignore
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix ();
    for j in range(9):
        glBegin(GL_LINE_STRIP);
        for i in range(31):
                glEvalCoord2f(i/30.0, j/8.0)
        glEnd();
        glBegin(GL_LINE_STRIP);
        for i in range(31):
                glEvalCoord2f(j/8.0, i/30.0)
        glEnd();
   
   
    glPopMatrix ();
    glFlush();




def draw_scene():
    init()
    draw_track()
    