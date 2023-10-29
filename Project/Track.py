from collections import defaultdict
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from Scene import draw_scene

width = 1280;
height = 720;
refreshMillis = 16;      # Refresh period in milliseconds
rot = 0;
rotKata = 0;
zoomX = 20;
zoomY = 12;
zoomZ = 16;
eyeX = 70;
eyeY = 40;
eyeZ = 0;
centerX = 10;
centerY = 10;
centerZ = 10;
viewX = 0;
viewY = 0; 

w,h= 500, 500

ctrl = defaultdict(int)



def display():
    global rot, zoomX, zoomY, zoomZ, eyeX, eyeY, eyeZ, ctrl
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); # type: ignore
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    # glFrustum(-10, 10, -10, 10, 1.0, 100.0);
    gluPerspective(90.0, 16/9.0, 1.0, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
    glViewport(0, 0, width, height);

    glPushMatrix();
    glRotatef(-30, 0, 1, 0); # initial rotation
    glRotatef(rot, 0, 1, 0);
    glTranslatef(-20, -10, 0);


    glScalef(zoomX, zoomY, zoomZ);

    draw_scene(ctrl)
    glPopMatrix()
    glutSwapBuffers()

def animate():
    pass

def key(key:str, x, y):
    global rot, zoomX, zoomY, zoomZ, eyeX, eyeY, eyeZ
    key = key.decode('utf-8') # type: ignore
    if key == ",":
        rot += 3
    elif key == ".":
        rot -= 3
    elif key == "+":
        zoomX *= 1.2
        zoomY *= 1.2
        zoomZ *= 1.2
    elif key == "_":
        zoomX /= 1.2;
        zoomY /= 1.2;
        zoomZ /= 1.2;
    elif key == "w":
        eyeY += 2
    elif key == "s":
        eyeY -= 2
    elif key == "a":
        eyeX += 2
    elif key == "d":
        eyeX -= 2
    elif key == 'q':
        eyeZ += 2
    elif key == 'e':
        eyeZ -= 2
    elif key == "u":
        ctrl['Z'] += 1
    elif key == "j":
        ctrl['Z'] -= 1
    elif key == "h":
        ctrl['X'] -= 1
    elif key == "k":
        ctrl['X'] += 1
    print(ctrl)


    # glutPostRedisplay()

def idle():
    glutPostRedisplay()

def Timer(value):
   glutPostRedisplay();    # Post a paint request to activate display()
   glutTimerFunc(refreshMillis, Timer, 0); # subsequent timer call at milliseconds


glutInit()
glutInitWindowSize(width, height)
glutInitWindowPosition(10, 10)
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) # type: ignore
glutCreateWindow("3D Racing Track.")
glutDisplayFunc(display)
glutIdleFunc(display)
glutKeyboardFunc(key)
glutMainLoop()