#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//#include <windows.h>
#include <GL/glut.h>
#include<vector>
#define pi (2*acos(0.0))
using namespace std;
void drawWheel(int radius,int height);
inline double degreeToRadian(int degree){
    return degree * pi/180;
}

int movedFw,turnedRight;
double cameraHeight,angle;
double cameraAngle,WheelTranslationUnit,wheelRotationUnit;
int drawgrid,drawaxes,movementUnit,rotateUnit,cubeSide,wheelRadius,wheelHeight;

struct point{
    double x,y,z;
};

typedef struct point Point;
Point pos,l,r,u;
Point points[132];

void moveWheelForward(int unit){
    movedFw = (movedFw+1)%8;
    pos.x+= (unit*u.x);
    pos.y+= (unit*u.y);
    pos.z+= (unit*u.z);
}
void moveWheelBackward(int unit){
    movedFw = (movedFw-1)%8;
    pos.x-= (unit*u.x);
    pos.y-= (unit*u.y);
    pos.z-= (unit*u.z);
}
void turnWheelLeft(int unit){
    turnedRight = (turnedRight+1)%8 ;
    double cosA = cos(degreeToRadian(unit));
    double sinA = sin(degreeToRadian(unit));
    l.x = (l.x * cosA) + (u.x * sinA) ;
    l.y = (l.y * cosA) + (u.y * sinA) ;
    l.z = (l.z * cosA) + (u.z * sinA) ;
    u.x = r.y * l.z - l.y * r.z;
    u.y = l.x * r.z - r.x * l.z;
    u.z = r.x * l.y - l.x * r.y;
}
void turnWheelRight(int unit){
    turnedRight = (turnedRight-1)%8 ;
    double cosA = cos(degreeToRadian(unit));
    double sinA = sin(degreeToRadian(unit));
    u.x = (u.x * cosA) + (l.x * sinA) ;
    u.y = (u.y * cosA) + (l.y * sinA) ;
    u.z = (u.z * cosA) + (l.z * sinA) ;
    l.x = u.y * r.z - r.y * u.z;
    l.y = r.x * u.z - u.x * r.z;
    l.z = u.x * r.y - r.x * u.y;
}

void drawAxes(){
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}
void drawGrid(){
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void generateRimPoints(GLfloat radius,GLfloat height){
    int i;
    float x,y;
    int segments = 20;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = pi/2;
    double heightStepsize= (height*1.0)/segments;
    for(i=0; i<4; i++)
    {
        x = radius * cos(angle);
        y = radius * sin(angle);
        points[i].x=x;
        points[i].y=y;
        points[i].z=-height/2;
        angle+=angle_stepsize;
    }
}
void generateWheelPoints(GLfloat radius,GLfloat height){
    generateRimPoints(radius,height);
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.3;
    angle = 0.0;
    int i=4;
    while( angle < 2*pi )
    {
        points[i].x = radius * cos(angle);
        points[i].y = radius * sin(angle);
        points[i].z = height/2;
        points[i+1].x = points[i].x ;
        points[i+1].y = points[i].y ;
        points[i+1].z = -height/2;
        angle = angle + angle_stepsize;
        i+=2;
    }
    points[i].x = radius ;
    points[i].y = 0.0 ;
    points[i].z = height/2;
    points[i+1].x = radius ;
    points[i+1].y = 0.0;
    points[i+1].z = -height/2;
}
void drawWheel(int radius,int height){
    int segments=20;
    double heightStepsize= (height*1.0)/segments;
    glTranslatef(0,0,20);
    glRotatef(90,1,0,0);

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(turnedRight * wheelRotationUnit, 0, 1, 0);
    glRotatef(movedFw * wheelRotationUnit,0,0,1);

    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_QUAD_STRIP);
    int j=0;
    double rimHeight=-height/2;
    while(j<segments){
            glVertex3f(points[0].x, points[0].y, rimHeight);
            glVertex3f(points[2].x, points[2].y, rimHeight);
            rimHeight = rimHeight + heightStepsize;
            j++;
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    j=0;
    rimHeight=-height/2;
    while(j<segments){
            glVertex3f(points[1].x, points[1].y, rimHeight);
            glVertex3f(points[3].x, points[3].y, rimHeight);
            rimHeight = rimHeight + heightStepsize;
            j++;
    }
    glEnd();

    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.3;

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    int i=4;
    while( angle < 2*pi )
    {
        glVertex3f(points[i].x, points[i].y, points[i].z);
        glVertex3f(points[i+1].x, points[i+1].y, points[i+1].z);
        angle = angle + angle_stepsize;
        i+=2;
    }
    glVertex3f(points[i].x, points[i].y, points[i].z);
    glVertex3f(points[i+1].x, points[i+1].y, points[i+1].z);
    glEnd();
}

void keyboardListener(unsigned char key, int x,int y){
    switch(key)
    {
        case 'w':
        moveWheelForward(WheelTranslationUnit);
        break;
    case 's':
        moveWheelBackward(WheelTranslationUnit);
        break;
    case 'a':
        turnWheelLeft(wheelRotationUnit);
        break;
    case 'd':
        turnWheelRight(wheelRotationUnit);
        break;
    default:
        break;
    }
}
void specialKeyListener(int key, int x,int y){
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        //moveCameraBackward(movementUnit);
        cameraHeight -= 3.0;
        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;
        //moveCameraForward(movementUnit);
        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        //moveCameraRight(movementUnit);
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        //moveCameraLeft(movementUnit);
        break;

    case GLUT_KEY_PAGE_UP:
        //moveCameraUp(movementUnit);
        break;
    case GLUT_KEY_PAGE_DOWN:
        //moveCameraDown(movementUnit);
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}
void mouseListener(int button, int state, int x, int y){
    //x, y is the x-y of the screen (2D)
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}

void display(){
    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);
    //initialize the matrix
    glLoadIdentity();
    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?
    gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);
    /****************************
    / Add your objects from here
    ****************************/
    //add objects
    drawGrid();
    drawWheel(wheelRadius,wheelHeight);
    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}
void animate(){
    //angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}
void init(){
    //codes for initialization
    drawgrid=1;
    drawaxes=0;
    cameraHeight=75.0;
    cameraAngle=1.0;
    angle=0;
    movementUnit = 2*5;
    rotateUnit=3*5;
    pos.x = 0; pos.y = 0; pos.z = 0;
    //u==x axis
    u.x = 1; u.y = 0; u.z = 0;
    //r==y axis
    r.x = 0; r.y = 1; r.z = 0;
    //l==z axis
    l.x = 0; l.y = 0; l.z = 1;
    cubeSide = 10;
    wheelRadius = 15;
    wheelHeight = 10;
    movedFw = 0,turnedRight=0;
    WheelTranslationUnit = (2*pi*wheelRadius)/8;
    wheelRotationUnit = 360/8;
    generateWheelPoints(wheelRadius,wheelHeight);
    //clear the screen
    glClearColor(0,0,0,0);
    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);
    //initialize the matrix
    glLoadIdentity();
    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}
int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
