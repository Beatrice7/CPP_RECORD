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
 c:smooth | flat
 */


/*固定光源为LIGHT0, 位置为(1.0, 1.0, 1.0， 0.0), 颜色为亮紫色, 高光为白色*/
/*移动光源为LIGHT1, 位置为(2 * sin(lightspin)), 2 * cos(lightspin), 1.0, 1.0),                                     颜色为亮绿色, 高光为蓝色, 每点击一次鼠标lightspin增加30°*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

GLint flag;
int nVertices, nFaces, nEdges; //712, 1420, 0
GLfloat Vertices[1000][3], VerNormal[1000][3], FaceNormal[1500][3];
GLint Faces[1500][3];

GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},
    {1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0},
    {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

//static GLfloat viewer[] = {0.0, 0.0, 5.0};

static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis;
static GLfloat trans[] = {0.0, 0.0, 0.0};
static GLint direct;
static GLfloat scale[] = {1.0, 1.0, 1.0};
static GLint multiply;
//light
static GLdouble lightspin = 0;

//smooth (default)
GLint shadeMode = 1;
void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    
    GLfloat fixedPos[] = { 1.8, 1.8, 1.8, 0.0 };
    // 指定环境光强度（RGBA）
    GLfloat ambientLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
    // 指定散射光强度（RGBA）
    GLfloat diffuseLight[] = {1.0f, 0.0f, 1.0f, 1.0f};//亮紫色
    // 指定镜面光强度（RGBA）
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};//高光部分为白色
  
    // 设置光源0的环境光成分
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    // 设置光源0的散射光成分
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    // 设置光源0的镜面光成分
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv (GL_LIGHT0, GL_POSITION, fixedPos);
    glEnable(GL_LIGHTING);
    //光源0为固定光，紫色
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

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

void drawPolygon(int v1, int v2, int v3, int i)
{
    glBegin(GL_POLYGON);
    if(shadeMode == 1){//smooth
        //v1
        glColor3fv(colors[v1 % 8]);
        glNormal3fv(VerNormal[v1]);
        glVertex3fv(Vertices[v1]);
        //v2
        glColor3fv(colors[v2 % 8]);
        glNormal3fv(VerNormal[v2]);
        glVertex3fv(Vertices[v2]);
        //v3
        glColor3fv(colors[v3 % 8]);
        glNormal3fv(VerNormal[v2]);
        glVertex3fv(Vertices[v3]);
    }else{//flat
        glNormal3fv(FaceNormal[i]);
        //v1
        glColor3fv(colors[v1 % 8]);
        glVertex3fv(Vertices[v1]);
        //v2
        glColor3fv(colors[v2 % 8]);
        glVertex3fv(Vertices[v2]);
        //v3
        glColor3fv(colors[v3 % 8]);
        glVertex3fv(Vertices[v3]);
    }
    glEnd();
}

void colorObject()
{
    for(int i = 0; i < nFaces; ++i)
    {
        drawPolygon(Faces[i][0], Faces[i][1], Faces[i][2], i);
    }
}

void calFaceNor(int i)
{
    double len;
    double vector01[3] = {
        Vertices[Faces[i][1]][0] - Vertices[Faces[i][0]][0],
        Vertices[Faces[i][1]][1] - Vertices[Faces[i][0]][1],
        Vertices[Faces[i][1]][2] - Vertices[Faces[i][0]][2]
    };
    double vector02[3] = {
        Vertices[Faces[i][2]][0] - Vertices[Faces[i][0]][0],
        Vertices[Faces[i][2]][1] - Vertices[Faces[i][0]][1],
        Vertices[Faces[i][2]][2] - Vertices[Faces[i][0]][2]
    };
    FaceNormal[i][0] = vector01[1] * vector02[2] - vector01[2] * vector02[1];
    FaceNormal[i][1] = vector01[2] * vector02[0] - vector01[0] * vector02[2];
    FaceNormal[i][2] = vector01[0] * vector02[1] - vector01[1] * vector02[0];
    len = sqrt(FaceNormal[i][0] * FaceNormal[i][0] + FaceNormal[i][1] * FaceNormal[i][1] + FaceNormal[i][2] * FaceNormal[i][2]);
    FaceNormal[i][0] /= len;
    FaceNormal[i][1] /= len;
    FaceNormal[i][2] /= len;
}

void calVerNor()
{
    int i, j, k, node;
    double len;
    int f[1000] = { 0 };
    
    for (i = 0; i < nFaces; i++)
    {
        for (j = 0; j < 3; j++)
        {
            node = Faces[i][j];
            if (f[node] == 0)
            {
                for (k = 0; k < nFaces; k++)
                {
                    if (Faces[k][0] == node || Faces[k][1] == node || Faces[k][2] == node)
                    {
                        VerNormal[node][0] += FaceNormal[k][0];
                        VerNormal[node][1] += FaceNormal[k][1];
                        VerNormal[node][2] += FaceNormal[k][2];
                    }
                }
                len = sqrt(VerNormal[node][0] * VerNormal[node][0] + VerNormal[node][1] * VerNormal[node][1] + VerNormal[node][2] * VerNormal[node][2]);
                VerNormal[node][0] /= len;
                VerNormal[node][1] /= len;
                VerNormal[node][2] /= len;
                f[node] = 1;
            }
        }
    }
}

void calNormal(){
    for(int i = 0; i < nFaces; i++)
    {
        calFaceNor(i);
    }
    calVerNor();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glTranslatef(trans[0], trans[1], trans[2]);
    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);
   // glTranslatef(trans[0], trans[1], trans[2]);
    glScaled(scale[0], scale[1], scale[2]);
    
    /*
    if(shadeMode == 1)
        glShadeModel (GL_SMOOTH);
    else
        glShadeModel (GL_FLAT);
    */
    
    
    //光源1为移动光源
    GLfloat movePos[] = { static_cast<GLfloat>(2 * sin(lightspin)), static_cast<GLfloat>(2 * cos(lightspin)), 1.0, 1.0 };
    // 指定环境光强度（RGBA）
    GLfloat ambientLight1[] = {0.2f, 0.2f, 0.2f, 1.0f};
    // 指定散射光强度（RGBA）
    GLfloat diffuseLight1[] = {0.0f, 1.0f, 0.0f, 1.0f};//散射光为绿色
    // 指定镜面光强度（RGBA）
    GLfloat specularLight1[] = {0.0f, 0.0f, 1.0f, 1.0f};//高光部分为蓝色
    glEnable(GL_LIGHT1);
    // 设置光源1的环境光成分
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
    // 设置光源1的散射光成分
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
    // 设置光源1的镜面光成分
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
    glLightfv (GL_LIGHT1, GL_POSITION, movePos);
    
    glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHTING);
    
    
    //glEnable(GL_COLOR_MATERIAL);
    //放在画物体这句的前后效果不同，在前面才有效
    colorObject();
    calNormal();
    
    //glFlush();
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

void mouse(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                lightspin += 30;
                glutPostRedisplay();
            }
            break;
        default:
            break;
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
        case 'c':
            shadeMode = -shadeMode;//don't forget to call Redisplay();
            glutPostRedisplay();
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
    glutCreateWindow("light");
    init();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    
    glutKeyboardFunc(keys);
    glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */
    glutMainLoop();
}



