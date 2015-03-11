/*左键调出菜单栏：包括 1.平凡算法 2.DDA 3.Bresenham */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

GLsizei wh = 500, ww = 500; /* initial window size */
GLint x1, y11, x2, y2;
GLint SELECT = 0;

void write_pixel(int ix, int iy){
    glPointSize(3.0);
    glBegin(GL_POINTS);
    glVertex2i(ix, iy);
    glEnd();
}

int myround(float a){
    return (int)(a + 0.5);
}

void swap(int x, int y){
    int temp;
    temp = x;
    x = y;
    y = temp;
}

void oridinaryLine(){
    int x;
    float y;
    GLfloat m = (float)(y2 - y11) / (x2 - x1);
    if(x1 > x2) swap(x1, x2), swap(y11, y2);
    for(x = x1; x <= x2; x++){
        y = m * (x - x1) + y11;
        write_pixel(x, myround(y));
    }
    cout << "oridinary" << endl;
}

void DDA(){
    int x;
    float y = y11;//y一定要记得初始化为y11
    GLfloat m = (float)(y2 - y11) / (x2 - x1);
    //if(fabs(m) > 1) swap(x1, y11), swap(x2, y2);
    if(x1 > x2) swap(x1, x2), swap(y11, y2);
    for(x = x1; x <= x2; x++){
        y += m;
        write_pixel(x, myround(y));
    }
    cout << "DDA" << endl;
    
}

void Bresenham()
{
    cout  << "Bresenham" << endl;
    int dx, dy, i, e;
    int incx, incy, inc1, inc2;
    int x,y;
    
    dx = x2 - x1;
    dy = y2 - y11;
    
    if(dx < 0) dx = -dx;
    if(dy < 0) dy = -dy;
    incx = 1;
    if(x2 < x1) incx = -1;
    incy = 1;
    if(y2 < y11) incy = -1;
    x=x1;
    y=y11;
    
    if(dx > dy)
    {
        write_pixel(x,y);
        e = 2*dy - dx;
        inc1 = 2*( dy -dx);
        inc2 = 2*dy;
        for(i = 0; i < dx; i++)
        {
            if(e >= 0)
            {
                y += incy;
                e += inc1;
            }
            else e += inc2;
            x += incx;
            write_pixel(x,y);
        }
    }
    else
    {
        write_pixel(x,y);
        e = 2*dx - dy;
        inc1 = 2*( dx - dy);
        inc2 = 2*dx;
        for(i = 0; i < dy; i++)
        {
            if(e >= 0)
            {
                x += incx;
                e += inc1;
            }
            else e += inc2;
            y += incy;
            write_pixel(x,y);
        }
    }
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



void mouse_line(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        
        x1 = x;
        y11 = wh-y;
        glLogicOp(GL_XOR);
    }
    
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        //glLogicOp(GL_COPY);
        x2 = x;
        y2 = wh-y;
        switch(SELECT){
            case 0:
                glColor3f(1.0, 0.0, 0.0);
                oridinaryLine();
                break;
            case 1:
                glColor3f(1.0, 0.0, 0.0);
                DDA();
                break;
            case 2:
                glColor3f(1.0, 0.0, 0.0);
                Bresenham();
                break;
            default:
                break;
        }
        glFlush();
        
        if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
            exit(0);
    }
}

void move_line(int x, int y)
{
    x2 = x;
    y2 = wh-y;
    glClear(GL_COLOR_BUFFER_BIT);
    switch(SELECT){
        case 0:
            glColor3f(0.0, 1.0, 0.0);//mac: red, pink ,yellow???
            oridinaryLine();
            break;
        case 1:
            glColor3f(0.0, 0.0, 1.0);
            DDA();
            break;
        case 2:
            glColor3f(1.0, 1.0, 0.0);
            Bresenham();
            break;
        default:
            break;
    }
    glFlush();
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



/* display callback required by GLUT 3.0 */
void display(void)
{}

void menu(int id)
{
    switch (id){
        case 0:
            SELECT = 0;
            glutMouseFunc(mouse_line);
            glutMotionFunc(move_line);
            break;
        case 1:
            SELECT = 1;
            init();
            glutMouseFunc(mouse_line);
            glutMotionFunc(move_line);
            break;
        case 2:
            SELECT = 2;
            glutMouseFunc(mouse_line);
            glutMotionFunc(move_line);
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
    
    glutCreateWindow("三种画线方法");
    
    init();
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_line);
    glutMotionFunc(move_line);
    glutDisplayFunc(display);
    
    glutCreateMenu(menu);
    glutAddMenuEntry("oridinaryLine", 0);
    glutAddMenuEntry("DDA", 1);
    glutAddMenuEntry("Bresenham", 2);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutMainLoop();
}
