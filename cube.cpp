#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<math.h>
#include <GL/glut.h>
#define pi (2*acos(0.0))
using namespace std;
inline double degreeToRadian(int degree){
    return degree * pi/180;
}

double cameraHeight,cameraAngle,angle;
int drawgrid,drawaxes,movementUnit,rotateUnit,cubeSide,maxCubeSide,sphereRadius,maxSphereRadius;
int cylinderRadius,cylinderMaxRadius,cylinderHeight,cylinderMaxHeight;
struct point{
    double x,y,z;
};
typedef struct point Point;
Point pos,l,r,u;

//camera movement functions
void moveCameraForward(int unit){
    pos.x+= (unit*l.x);
    pos.y+= (unit*l.y);
    pos.z+= (unit*l.z);
}
void moveCameraBackward(int unit){
    pos.x-= (unit*l.x);
    pos.y-= (unit*l.y);
    pos.z-= (unit*l.z);
}
void moveCameraRight(int unit){
    pos.x+= (unit*r.x);
    pos.y+= (unit*r.y);
    pos.z+= (unit*r.z);
}
void moveCameraLeft(int unit){
    pos.x-= (unit*r.x);
    pos.y-= (unit*r.y);
    pos.z-= (unit*r.z);
}
void moveCameraUp(int unit){
    pos.x+= (unit*u.x);
    pos.y+= (unit*u.y);
    pos.z+= (unit*u.z);
}
void moveCameraDown(int unit){
    pos.x-= (unit*u.x);
    pos.y-= (unit*u.y);
    pos.z-= (unit*u.z);
}
//camera rotation functions
void rotateCameraLeft(int unit){
    double cosA = cos(degreeToRadian(unit));
    double sinA = sin(degreeToRadian(unit));
    r.x = (r.x * cosA) + (l.x * sinA) ;
    r.y = (r.y * cosA) + (l.y * sinA) ;
    r.z = (r.z * cosA) + (l.z * sinA) ;
    l.x = u.y * r.z - r.y * u.z;
    l.y = r.x * u.z - u.x * r.z;
    l.z = u.x * r.y - r.x * u.y;
}
void rotateCameraRight(int unit){
    double cosA = cos(degreeToRadian(unit));
    double sinA = sin(degreeToRadian(unit));
    l.x = (l.x * cosA) + (r.x * sinA) ;
    l.y = (l.y * cosA) + (r.y * sinA) ;
    l.z = (l.z * cosA) + (r.z * sinA) ;
    r.x = l.y * u.z - u.y * l.z;
    r.y = u.x * l.z - l.x * u.z;
    r.z = l.x * u.y - u.x * l.y;
}
void rotateCameraUp(int unit){
    double cosA = cos(degreeToRadian(unit));
    double sinA = sin(degreeToRadian(unit));
    l.x = (l.x * cosA) + (u.x * sinA) ;
    l.y = (l.y * cosA) + (u.y * sinA) ;
    l.z = (l.z * cosA) + (u.z * sinA) ;
    u.x = r.y * l.z - l.y * r.z;
    u.y = l.x * r.z - r.x * l.z;
    u.z = r.x * l.y - l.x * r.y;
}
void rotateCameraDown(int unit){
    double cosA = cos(degreeToRadian(unit));
    double sinA = sin(degreeToRadian(unit));
    u.x = (u.x * cosA) + (l.x * sinA) ;
    u.y = (u.y * cosA) + (l.y * sinA) ;
    u.z = (u.z * cosA) + (l.z * sinA) ;
    l.x = u.y * r.z - r.y * u.z;
    l.y = r.x * u.z - u.x * r.z;
    l.z = u.x * r.y - r.x * u.y;
}
void tiltCameraClockwise(int unit){
    double cosA = cos(degreeToRadian(unit));
    double sinA = sin(degreeToRadian(unit));
    u.x = (u.x * cosA) + (r.x * sinA) ;
    u.y = (u.y * cosA) + (r.y * sinA) ;
    u.z = (u.z * cosA) + (r.z * sinA) ;
    r.x = l.y * u.z - u.y * l.z;
    r.y = u.x * l.z - l.x * u.z;
    r.z = l.x * u.y - u.x * l.y;
}
void tiltCameraAnticlockwise(int unit){
    double cosA = cos(degreeToRadian(unit));
    double sinA = sin(degreeToRadian(unit));
    r.x = (r.x * cosA) + (u.x * sinA) ;
    r.y = (r.y * cosA) + (u.y * sinA) ;
    r.z = (r.z * cosA) + (u.z * sinA) ;
    u.x = r.y * l.z - l.y * r.z;
    u.y = l.x * r.z - r.x * l.z;
    u.z = r.x * l.y - l.x * r.y;
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
void drawSquare(double a){
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( 20, a, a );
        glVertex3f( 20, a,-a );
        glVertex3f( 20,-a,-a );
        glVertex3f( 20,-a, a );
    }
    glEnd();
}
void drawOneEighthSphere(double radius,int slices,int stacks){
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices/4; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glColor3f(1,0,0);
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
            }
            glEnd();
        }
    }
}
void drawOneFourthCylinder(GLfloat radius,GLfloat height){
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    /** Draw the tube */
    glColor3f(0,1,0);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while( angle < pi/2 )
    {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height/2);
        glVertex3f(x, y, -height/2);
        angle = angle + angle_stepsize;
    }
    x = radius * cos(angle);
    y = radius * sin(angle);
    //glVertex3f(radius, 0.0, height/2);
    //glVertex3f(radius, 0.0, -height/2);
    glVertex3f(x, y, height/2);
    glVertex3f(x, y, -height/2);
    glEnd();
}
void drawFillingCylinders(GLfloat radius,GLfloat height){
    glPushMatrix();{
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(90,0,0,1);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(-90,0,0,1);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,0,0,1);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(90,0,1,0);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(-90,0,1,0);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(90,1,0,0);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(-90,1,0,0);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,1,0,0);
        glRotatef(90,0,1,0);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,1,0,0);
        glRotatef(-90,0,1,0);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,0,1,0);
        glRotatef(90,1,0,0);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,0,1,0);
        glRotatef(-90,1,0,0);
        glTranslatef(cubeSide,cubeSide,0);
        drawOneFourthCylinder(radius,height);
    }
    glPopMatrix();
}
void drawSphereFromParts(double radius){
    glPushMatrix();{
        glTranslatef(cubeSide,cubeSide,cubeSide);
        drawOneEighthSphere(radius,24,20);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(90,0,0,1);
        glTranslatef(cubeSide,cubeSide,cubeSide);
        drawOneEighthSphere(radius,24,20);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(-90,0,0,1);
        glTranslatef(cubeSide,cubeSide,cubeSide);
        drawOneEighthSphere(radius,24,20);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(2*90,0,0,1);
        glTranslatef(cubeSide,cubeSide,cubeSide);
        drawOneEighthSphere(radius,24,20);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,1,0,0);
        glTranslatef(cubeSide,cubeSide,cubeSide);
        drawOneEighthSphere(radius,24,20);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,1,0,0);
        glRotatef(90,0,0,1);
        glTranslatef(cubeSide,cubeSide,cubeSide);
        drawOneEighthSphere(radius,24,20);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,1,0,0);
        glRotatef(-90,0,0,1);
        glTranslatef(cubeSide,cubeSide,cubeSide);
        drawOneEighthSphere(radius,24,20);
    }glPopMatrix();
    glPushMatrix();{
        glRotatef(180,1,0,0);
        glRotatef(2*90,0,0,1);
        glTranslatef(cubeSide,cubeSide,cubeSide);
        drawOneEighthSphere(radius,24,20);
    }glPopMatrix();
}
void drawCube(int a){
    if(a>maxCubeSide) a = maxCubeSide;
    if(a<0) a = 0 ;
    glColor3f(1,1,1);
    glPushMatrix();{
        drawSquare(a);
        glTranslatef(-maxCubeSide*2,0,0);
        drawSquare(a);
        glTranslatef(maxCubeSide*2,0,0);
        glRotatef(90,0,0,1);
        drawSquare(a);
        glTranslatef(-maxCubeSide*2,0,0);
        drawSquare(a);
        glTranslatef(maxCubeSide*2,0,0);
        glRotatef(90,0,1,0);
        drawSquare(a);
        glTranslatef(-maxCubeSide*2,0,0);
        drawSquare(a);
    }
    glPopMatrix();
}


void keyboardListener(unsigned char key, int x,int y){
    switch(key)
    {
    case '1':
        rotateCameraLeft(rotateUnit);
        break;
    case '2':
        rotateCameraRight(rotateUnit);
        break;
    case '3':
        rotateCameraUp(rotateUnit);
        break;
    case '4':
        rotateCameraDown(rotateUnit);
        break;
    case '5':
        tiltCameraClockwise(rotateUnit);
        break;
    case '6':
        tiltCameraAnticlockwise(rotateUnit);
        break;
    default:
        break;
    }
}
void specialKeyListener(int key, int x,int y){
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        moveCameraBackward(movementUnit);
        //cameraHeight -= 3.0;
        break;
    case GLUT_KEY_UP:		// up arrow key
        //cameraHeight += 3.0;
        moveCameraForward(movementUnit);
        break;

    case GLUT_KEY_RIGHT:
        //cameraAngle += 0.03;
        moveCameraRight(movementUnit);
        break;
    case GLUT_KEY_LEFT:
        //cameraAngle -= 0.03;
        moveCameraLeft(movementUnit);
        break;

    case GLUT_KEY_PAGE_UP:
        moveCameraUp(movementUnit);
        break;
    case GLUT_KEY_PAGE_DOWN:
        moveCameraDown(movementUnit);
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        if(cubeSide<maxCubeSide)
            cubeSide++;
        if(sphereRadius>0)
            sphereRadius--;
        if(cylinderHeight<cylinderMaxHeight && cylinderRadius)
            cylinderHeight++;
        if(cylinderRadius>0 && cylinderHeight<cylinderMaxHeight)
            cylinderRadius--;
        break;
    case GLUT_KEY_END:
        if(cubeSide>0)
            cubeSide--;
        if(sphereRadius<maxSphereRadius )
            sphereRadius++;
        if(cylinderHeight>0 && cylinderRadius<cylinderMaxRadius )
            cylinderHeight--;
        if(cylinderRadius<cylinderMaxRadius && cylinderHeight)
            cylinderRadius++;
        break;

    default:
        break;
    }
}
void mouseListener(int button, int state, int x, int y){
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
    gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    drawAxes();
    drawCube(cubeSide);
    drawSphereFromParts(sphereRadius);
    drawFillingCylinders(sphereRadius,2*cubeSide);


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
    drawgrid=0;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;
    movementUnit = 2*5;
    rotateUnit=3*5;
    pos.x = 100;
    pos.y = 100;
    pos.z = 0;
    u.x = 0;
    u.y = 0;
    u.z=1;
    r.x = -1/sqrt(2);
    r.y = -r.x;
    r.z=0;
    l.x = r.x;
    l.y = l.x;
    l.z =0;
    cubeSide = 10; maxCubeSide = 20;
    sphereRadius = 10; maxSphereRadius = 20;
    cylinderHeight = 10; cylinderMaxHeight = 20 ;
    cylinderRadius = 10; cylinderMaxRadius = 20 ;
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
int  main(int argc, char **argv){
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
