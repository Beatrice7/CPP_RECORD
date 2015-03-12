/*左键调出菜单栏：包括画线段，三角形，矩形，圆*/


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/* globals */
GLsizei wh = 500, ww = 500; /* initial window size */
float xm, ym, xmm, ymm;
int first = 0;
float triVertex[3][2];
int ver = 0;
float PI = 3.14159;

/* XOR写入模式
 • 缺省的写入模式是复制模式(copy)或替换模式 (replacement),直接用源像素取代目标像素,即 d’=s
 – 用这种方法不能绘制一条临时直线,因为我们不能用快速简 单的方法恢复在临时直线下面的内容
 • 异或操作(XOR):d’=ds – 相同值为0,不同值为1
 – (ds)s=d
 – 因此如果应用XOR模式画一条直线,那么只要在原地再画 一遍这条直线就可以删除这条直线
	*/


/*选择逻辑操作类型
 • glLogicOp(GL_XOR)
 • glLogicOp(GL_COPY) (默认值)
 */
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
        glColor3f(0.0, 1.0, 0.0);
        glLogicOp(GL_COPY);//直接用源像素取代目标像素
        glRectf(xm, ym, xmm, ymm);
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
        glColor3f(0.0, 0.5, 1.0);
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
        glFlush();
    }
    
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
        exit(0);
}

void move_line(int x, int y)
{
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


//triangle
void mouse_triangle(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        triVertex[ver][0] = x;
        triVertex[ver][1] = wh-y;
        ver++;
        if(ver == 3)
        {
            glColor3f(0.0, 0.0, 0.5);
            glLogicOp(GL_COPY);//直接用源像素取代目标像素
            glBegin(GL_TRIANGLES);
            glVertex2f(triVertex[0][0], triVertex[0][1]);
             glVertex2f(triVertex[1][0], triVertex[1][1]);
             glVertex2f(triVertex[2][0], triVertex[2][1]);
            glEnd();
            glFlush();
            ver = 0;
        }
        
    }
    
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
        exit(0);
}

//circle
void mouse_circle(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        
        xm = x;
        ym = wh-y;
        glColor3f(0.0, 0.5, 1.0);
        glLogicOp(GL_XOR);
        first = 0;
    }
    
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        xmm = x;
        ymm = wh-y;
        float r = sqrt(pow((xm-xmm),2)+pow((ym-ymm),2));
        if (first == 1)
        {
            
            glBegin(GL_TRIANGLE_FAN);
            for(int i = 0; i< 1000; ++i)
            {
                 glVertex2f(xm+r*cos(2*PI/1000*i), ym+r*sin(2*PI/1000*i));
            }
            glEnd();
            glFlush();
        }
        
        
        glColor3f(0.0, 0.5, 1.0);
        glLogicOp(GL_COPY);
        glBegin(GL_TRIANGLE_FAN);
        for(int i = 0; i< 1000; ++i)
        {
            glVertex2f(xm+r*cos(2*PI/1000*i), ym+r*sin(2*PI/1000*i));
        }
        glEnd();
        glFlush();
    }
    
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
        exit(0);
}

void move_circle(int x, int y)
{
    xmm = x;
    ymm = wh-y;
    float r = sqrt(pow((xm-xmm),2)+pow((ym-ymm),2));
    if(first == 1)
    {
        glBegin(GL_TRIANGLE_FAN);
        for(int i = 0; i< 1000; ++i)
        {
            glVertex2f(xm+r*cos(2*PI/1000*i), ym+r*sin(2*PI/1000*i));
        }
        glEnd();
        glFlush();
    }
    
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i< 1000; ++i)
    {
        glVertex2f(xm+r*cos(2*PI/1000*i), ym+r*sin(2*PI/1000*i));
    }
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
   	glClearColor(0.0, 0.0, 0.0, 1.0);
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
            glutMouseFunc(mouse_rect);
            glutMotionFunc(move_rect);
            break;
        case 2:
            glutMouseFunc(mouse_triangle);
            break;
        case 3:
            glutMouseFunc(mouse_circle);
            glutMotionFunc(move_circle);
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
    
    glutCreateWindow("rubber banding");
    
   	init();
   	glutReshapeFunc(reshape);
   	glutMouseFunc(mouse_line);
   	glutMotionFunc(move_line);
    glutDisplayFunc(display);
    
    glutCreateMenu(menu);
    glutAddMenuEntry("line", 0);
    glutAddMenuEntry("square", 1);
    glutAddMenuEntry("triangle", 2);
    glutAddMenuEntry("circle", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    
   	glutMainLoop();
}
