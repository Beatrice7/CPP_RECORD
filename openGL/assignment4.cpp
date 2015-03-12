/*左键调出菜单栏：包括画线段，矩形*/
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#define LEFT 1   //0001
#define RIGHT 2  //0010
#define BOTTOM 4 //0100
#define TOP 8    //1000
using namespace std;

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat xm, ym, xmm, ymm;
GLfloat XL, XR, YB, YT;//窗口边界
int first = 0;

void displayline(float x1, float y1, float x2, float y2)
{
    cout << "in displine" << endl;
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glFlush();
}

void rectEdge(float xm, float ym, float xmm, float ymm)
{
    if(xm < xmm){
        XL = xm;
        XR = xmm;
    }else{
        XL = xmm;
        XR = xm;
    }
    
    if(ym < ymm){
        YB = ym;
        YT = ymm;
    }else{
        YB = ymm;
        YT = ym;
    }
    cout << "in rect Edge" << endl;
}

int encode(float x, float y)
{
    int c = 0;
    if(x < XL) c |= LEFT;
    if(x > XR) c |= RIGHT;
    if(y < YB) c |= BOTTOM;
    if(y > YT) c |= TOP;
    return c;
}

void lineClip(float x1, float y1, float x2, float y2)
{
    int code, x, y;
    int code1 = encode(x1, y1);
    int code2 = encode(x2, y2);
    while(code1 != 0 || code2 != 0){
        if((code1 & code2) != 0) return; //两个端点同在窗口的上方、下方、左方或右方
        code = code1;
        if(code1 == 0) code = code2;
        if((LEFT & code) != 0){
            x = XL;
            y = y1 + (XL - x1) * (y2 - y1) / (x2 - x1);
        }else if((RIGHT & code) != 0){
            x = XR;
            y = y1 + (XR - x1) * (y2 - y1) / (x2 - x1);
        }else if((BOTTOM & code) != 0){
            y = YB;
            x = x1 + (x2 - x1) * (YB - y1) / (y2 - y1);
        }else if ((TOP & code) != 0)
        {
            y = YT;
            x = x1 + (x2 - x1) * (YT - y1) / (y2 - y1);
        }
        
        if(code == code1){
            x1 = x;
            y1 = y;
            code1 = encode(x, y);
        }else{
            x2 = x;
            y2 = y;
            code2 = encode(x, y);
        }
    }
    
    displayline(x1, y1, x2, y2);
}


void mouse_rect(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        xm = x;
        ym = wh-y;
        glColor3f(0.0, 0.0, 1.0);
        glLogicOp(GL_XOR);
        first = 0;
    }
    
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        if (first == 1)
        {
            glRectf(xm, ym, xmm, ymm);
            glFlush();
        }
        
        xmm = x;
        ymm = wh-y;
        glColor3f(0.0, 0.0, 0.5);
        glLogicOp(GL_COPY);//直接用源像素取代目标像素
        glRectf(xm, ym, xmm, ymm);
        rectEdge(xm, ym, xmm, ymm);
        glFlush();
    }
    
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
        exit(0);
}

void move_rect(int x, int y)
{
    if(first == 1)
    {
        glRectf(xm, ym, xmm, ymm);
        glFlush();
    }
    
    xmm = x;
    ymm = wh-y;
    glRectf(xm, ym, xmm, ymm);
    glFlush();
    first = 1;
}

//line
void mouse_line(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        
        xm = x;
        ym = wh-y;
        glColor3f(1.0, 0.5, 0.0);
        glLogicOp(GL_XOR);
        first = 0;
    }
    
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        if (first == 1)
        {
            glBegin(GL_LINES);
            glVertex2f(xm, ym);
            glVertex2f(xmm, ymm);
            glEnd();
            glFlush();
        }
        
        xmm = x;
        ymm = wh-y;
        glColor3f(0.0, 0.5, 0.5);
        glLogicOp(GL_COPY);//直接用源像素取代目标像素
        glBegin(GL_LINES);
        glVertex2f(xm, ym);
        glVertex2f(xmm, ymm);
        glEnd();
        lineClip(xm, ym, xmm, ymm);
        glFlush();
    }
    
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
        exit(0);
}

void move_line(int x, int y)
{
    glLineWidth(3.0);
    if(first == 1)
    {
        glBegin(GL_LINES);
        glVertex2f(xm, ym);
        glVertex2f(xmm, ymm);
        glEnd();
        glFlush();
    }
    
    xmm = x;
    ymm = wh-y;
    glBegin(GL_LINES);
    glVertex2f(xm, ym);
    glVertex2f(xmm, ymm);
    glEnd();
    glFlush();
    first = 1;
}

void reshape(GLsizei w, GLsizei h)
{
    
    /* adjust clipping box */
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,(GLdouble)w, 0.0,(GLdouble)h, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /* adjust viewport and clear */
    
    glViewport(0,0,w,h);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    
    /* set global size for use by drawing routine */
    
    ww = w;
    wh = h;
}

void init(void)
{
    glViewport(0,0,ww,wh);
    
    /* Pick 2D clipping window to match size of screen window
     This choice avoids having to scale object coordinates
     each time window is resized */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,(GLdouble) ww , 0.0,(GLdouble) wh , -1.0, 1.0);
    
    /* set clear color to black and clear window */
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    
    glEnable(GL_COLOR_LOGIC_OP);
}


/* display callback required by GLUT 3.0 */
void display(void)
{}

void menu(int id)
{
    switch (id) {
        case 0:
            glutMouseFunc(mouse_line);
            glutMotionFunc(move_line);
            break;
        case 1:
            init();
            glutMouseFunc(mouse_rect);
            glutMotionFunc(move_rect);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(ww, wh);
    
    glutCreateWindow("Cohen-Sutherland");
    
    init();
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_rect);
    glutMotionFunc(move_rect);
    glutDisplayFunc(display);
    
    glutCreateMenu(menu);
    glutAddMenuEntry("line", 0);
    glutAddMenuEntry("square", 1);
    
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    
    glutMainLoop();
}
