from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *



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


k1 = lambda t: (1 - t) * (1 - t) * (1 - t)
k2 = lambda t: 3 * (1 - t) * (1 - t) * t
k3 = lambda t: 3 * (1 - t) * t * t
k4 = lambda t: t * t * t
PT = lambda t, P1, P2, P3, P4: P1 * k1(t) + P2 * k2(t) + P3 * k3(t) + P4 * k4(t);
ctrl_points = [15, 30, 20, 8]

def draw_curve():
    glPushMatrix()

    seg_points = []
    seg = 300
    for i in range(seg):
        t = i/seg
        pt = PT(t, *ctrl_points)
        seg_points.append(pt)

    #We will be fixing Y axis, draw through X axis and iterate throguh Z axis.
    line_points_3fv_1 = [[i/10, 1, seg_points[i]] for i in range(seg)]
    line_points_3fv_2 = [[i/10, 1, seg_points[i]+5] for i in range(seg)]
    # print(line_points_3fv)
    for i in range(0, seg, 1):
        glColor3f(77/255, 85/255, 90/255)
        glBegin(GL_QUADS)
        for p in line_points_3fv_1[i:i+2]:
            glVertex3fv(p)
        for p in line_points_3fv_2[i:i+2][::-1]:
            glVertex3fv(p)
        glEnd()
        
    glPopMatrix()

def draw_building_1(ctrl=None):
    glPushMatrix()
    glTranslatef(10, 1, 28)
    draw_cube(117/255, 75/255, 0/255)
    glPopMatrix()

def draw_track(ctrl=None):
    draw_curve()
    draw_building_1(ctrl)
    




def draw_scene(ctrl=None):
    draw_track(ctrl)
    