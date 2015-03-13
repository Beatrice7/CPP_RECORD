#include <GLUT/glut.h>
#include <stdlib.h>

/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init(void)
{
    GLfloat mat_specular[] = { 1.0, 0.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; //最后一个参数为0表示该光源是directional的
    //  GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 }; //最后一个参数非0表示该光源是positional的
    
    
    //光的颜色照到物体上,即物体呈现出的颜色
    GLfloat light_ambient[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 1.0, 1.0 };
    
    //环境光与光源无关，当关闭LIGHT0时，显现环境光颜色
    //GLfloat Light_Model_Ambient[] = { 0.0 , 0.0 , 0.0 , 1.0 }; //全局环境光设为黑色
    //GLfloat Light_Model_Ambient[] = { 1.0 ,1.0 , 1.0 , 1.0 };//这时物体呈现灰色，也即是环境光的颜色（当LIGHT0关闭时）
    GLfloat Light_Model_Ambient[] = { 0.2 ,0.2 , 0.2 , 1.0 }; //默认的全局环境光
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT , Light_Model_Ambient ); //设置全局环境光的方法
    
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); //把全局环境光设置为黑色,然后注释掉本句可以发现"黑屏"
    glDepthFunc(GL_LESS); //glDepthFunc()来设置深度测试函数
    glEnable(GL_DEPTH_TEST);
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSolidSphere (1.0, 20, 16);
    glFlush ();
}
void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
                 1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
        glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
                 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

