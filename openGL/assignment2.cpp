/*
 u:up
 d:down
 l:left
 r:right
 S:larger
 s:smaller
 x:rotate with x_axis
 y:rotate with y_axis
 z:rotate with z_axis
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;

GLint flag;
int nVertices, nFaces, nEdges; //712, 1420, 0
GLfloat Vertices[1000][3];
GLfloat Faces[1500][3];
GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},
    {1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0},
    {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis;
static GLfloat trans[] = {0.0, 0.0, 0.0};
static GLint direct;
static GLfloat scale[] = {1.0, 1.0, 1.0};
static GLint multiply;


void load_off(const string &filename)
{
    ifstream fin;
    fin.open(filename);
    string off;
    getline(fin, off);//读入首行字符串"OFF"
    fin >>   nVertices >> nFaces >> nEdges; //读入点、面、边数目
    for (int i=0; i<nVertices; i++)
    {
        fin>> Vertices[i][0] >> Vertices[i][1] >> Vertices[i][2];
    }
    int n;
    for (int i=0; i<nFaces; i++)
    {
        fin>> n >> Faces[i][0] >> Faces[i][1] >> Faces[i][2];
    }
    
    fin.close();
}

void drawPolygon(int v1, int v2, int v3)
{
    glBegin(GL_POLYGON);
    glColor3fv(colors[v1 % 8]);
    glVertex3fv(Vertices[v1]);
    glColor3fv(colors[v2 % 8]);
    glVertex3fv(Vertices[v2]);
    glColor3fv(colors[v3 % 8]);
    glVertex3fv(Vertices[v3]);
    glEnd();
}

void colorObject()
{
    for(int i = 0; i < nFaces; ++i)
    {
        drawPolygon(Faces[i][0], Faces[i][1], Faces[i][2]);
    }
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    /*平移语句放在旋转之前，否则不是绕着物体自身旋转，而是绕之前着平移后的坐标中心*/
    glTranslatef(trans[0], trans[1], trans[2]);
    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);
   // glTranslatef(trans[0], trans[1], trans[2]);
    glScaled(scale[0], scale[1], scale[2]);
    
    colorObject();
    
    glFlush();
    glutSwapBuffers();
}


void spinObject()
{
    theta[axis] += 5.0;
    if( theta[axis] > 360.0 ) theta[axis] -= 360.0;
    /* display(); */
    
    glutPostRedisplay();
}

void transObject()
{
    
    switch (direct) {
        case 0:
            trans[1] += 0.1;
            break;
        case 1:
            trans[1] -= 0.1;
            break;
        case 2:
            trans[0] += 0.1;
            break;
        case 3:
            trans[0] -= 0.1;
            break;
        default:
            break;
            
    }
    
    glutPostRedisplay();
}

void scaleObject()
{
    
    if(multiply == 0){
        scale[0] *= 0.7;
        scale[1] *= 0.7;
        scale[2] *= 0.7;
        glutPostRedisplay();
    }
    if(multiply == 1){
        scale[0] *= 1.3;
        scale[1] *= 1.3;
        scale[2] *= 1.3;
        glutPostRedisplay();
    }
    
}


void keys(unsigned char key, int x, int y)
{
    switch (key) {
        case 'x':
            axis = 0, spinObject();
            break;
        case 'y':
            axis = 1, spinObject();
            break;
        case 'z':
            axis = 2, spinObject();
            break;
        case 'u':
            direct = 0, transObject();
            break;
        case 'd':
            direct = 1, transObject();
            break;
        case 'r':
            direct = 2, transObject();
            break;
        case 'l':
            direct = 3, transObject();
            break;
        case's':
            multiply = 0, scaleObject();
            break;
        case 'S':
            multiply = 1, scaleObject();
            break;
        default:
            break;
    }
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
                2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
                2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int
main(int argc, char **argv)
{
    load_off("/Users/qiruotong/Documents/my_opengl/project1/project1/horse.off");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("transform");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    
    glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */
    glutMainLoop();
}

