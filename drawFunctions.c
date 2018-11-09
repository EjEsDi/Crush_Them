#include "drawFunctions.h"
#include "callbackFunctions.h"
#include <GL/glut.h>
#include <time.h>

/************************************
    Functions definitions start here
*************************************/
//TODO: There is something wrong in drawing or Makefile. 
//TODO: sometimes(after doing make) tank shows up in rly weird spot
//TODO: make sure that make is working then rest

void init(void){
    glClearColor(0.6, 0.8 , 1, 0.0);
    glClearDepth(1.0);
    glLineWidth(1);
    glShadeModel(GL_FLAT);
    gs.timeInMS = 20;
    gs.timeCarSpawn = 1000;
    initRenderingObjects(&roadScale, &roadRotation, &roadTranslation, &gs); 
}

void drawSun(){
    glPushMatrix();
    glLoadIdentity();
        glTranslatef(20, 10, -30);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();
}

void drawRoad(const struct Vector3f aScale,const struct Vector3f aRotation,const struct Vector3f aTranslation){
    
    glPushMatrix();
        glEnable(GL_DEPTH_TEST);
        glScalef(aScale.x, aScale.y, aScale.z);
        glRotatef(aRotation.x, 1, 0, 0);
        glRotatef(aRotation.y, 0, 1, 0);
        glRotatef(aRotation.z, 0, 0, 1);
        glTranslatef(aTranslation.x, aTranslation.y, aTranslation.z);
        glLineWidth(1);


        GLfloat width = 1; // 1m wide * scale
        GLfloat height = 0.1; // road is just above 0, because lookAt points at 0 on y-axis
        GLfloat depth = 0; 

        glColor3f(0.3, 0.3, 0.3);
        glBegin(GL_POLYGON);
            glVertex3f(-width,-height, depth);//bottom left
            glVertex3f(width, -height, depth);//bottom right
            glVertex3f(width, height, depth);//top right
            glVertex3f(-width, height, depth);//top left
        glEnd();

        //draw lanes on road
        glEnable(GL_LINE_STIPPLE);
        glLineStipple (3, 0xF00F); // dashed lines
        glLineWidth(5);
        glBegin(GL_LINES);
                glColor3f(1, 1, 1);
                //left line
                glVertex3f(-width/3, -height, depth-0.001); // z-axis, -0.001 is just so we see lines
                glVertex3f(-width/3, height, depth-0.001);  // they are just tiny bit above road it self
                //right line                                // better solution?
                glVertex3f(width/3, -height, depth-0.001); 
                glVertex3f(width/3, height, depth-0.001);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        glDisable(GL_DEPTH_TEST);
    glPopMatrix();
}
void drawCubeTank(const struct Tank tank){
    
    glPushMatrix();
        //TODO: Because I want to know where my tank is, I will need Scalef and translatef to be sent to here.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable( GL_DEPTH_TEST );
        glScalef(1, 1, 3); // size of tank, adjustable only here, change that? 
        glTranslatef(tank.tankTranslate.x, tank.tankTranslate.y, tank.tankTranslate.z);
        glBegin(GL_QUADS);
            /* blue - green - red - light blue - */

            //blue
            glColor3f(0, 0, 1);
            glVertex3f(-0.5, 0, -0.5);
            glVertex3f(-0.5, 1, -0.5);
            glVertex3f(0.5, 1, -0.5);
            glVertex3f(0.5, 0, -0.5); 

            //green - back
            glColor3f(0, 1, 0);
            glVertex3f(-0.5, 0, +0.5);
            glVertex3f(-0.5, 1, +0.5);
            glVertex3f(0.5, 1, 0.5);
            glVertex3f(0.5, 0, 0.5); 
            
            //sides
            // purple - right
            glColor3f(1, 0, 1);
            glVertex3f(0.5, 0, -0.5);
            glVertex3f(0.5, 1, -0.5);
            glVertex3f(0.5, 1, +0.5);
            glVertex3f(0.5, 0, +0.5); 

            //purple - left
            glColor3f(1, 0, 1);
            glVertex3f(-0.5, 0, -0.5);
            glVertex3f(-0.5, 1, -0.5);
            glVertex3f(-0.5, 1, +0.5);
            glVertex3f(-0.5, 0, +0.5); 

            // top and bottom
            //white
            glColor3f(1, 1, 1);
            glVertex3f(-0.5, 0, +0.5);
            glVertex3f(+0.5, 0, +0.5);
            glVertex3f(+0.5, 0, -0.5);
            glVertex3f(-0.5, 0, -0.5);

            //yellow
            glColor3f(1, 1, 0);
            glVertex3f(0.5, 1, +0.5);
            glVertex3f(-0.5, 1, +0.5);
            glVertex3f(-0.5, 1, -0.5);
            glVertex3f(0.5, 1, -0.5);
            
        glEnd();
        
    glPopMatrix();
}
void drawCar(const struct Car car){
    
    glPushMatrix();
        //TODO check glRotate, when you want it to rotate? during crush or smth, will need Struct instead hardcode numbers for rotation
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glScalef(1, 1, 1);
        glTranslatef(car.carPosition.x, car.carPosition.y, car.carPosition.z);
        // glRotatef(0, 1, 0, 0);
        // glRotatef(0, 0, 1, 0);
        // glRotatef(60, 0, 0, 1);
        glBegin(GL_QUADS);
            /* blue - green - red - light blue - */

            //front and back
            //blue - front
            glColor3f(0, 0, 1);
            glVertex3f(-0.5, 0, -0.5);
            glVertex3f(-0.5, 1, -0.5);
            glVertex3f(0.5, 1, -0.5);
            glVertex3f(0.5, 0, -0.5); 

            //purple - back
            glColor3f(1, 0, 1);
            glVertex3f(-0.5, 0, +0.5);
            glVertex3f(-0.5, 1, +0.5);
            glVertex3f(0.5, 1, 0.5);
            glVertex3f(0.5, 0, 0.5); 
            
            //sides
            //purple - right
            glColor3f(1, 0, 1);
            glVertex3f(0.5, 0, -0.5);
            glVertex3f(0.5, 1, -0.5);
            glVertex3f(0.5, 1, +0.5);
            glVertex3f(0.5, 0, +0.5); 

            //lblue - left
            glColor3f(1, 0, 0);
            glVertex3f(-0.5, 0, -0.5);
            glVertex3f(-0.5, 1, -0.5);
            glVertex3f(-0.5, 1, +0.5);
            glVertex3f(-0.5, 0, +0.5); 

            // top and bottom
            //white
            glColor3f(1, 1, 1);
            glVertex3f(-0.5, 0, +0.5);
            glVertex3f(+0.5, 0, +0.5);
            glVertex3f(+0.5, 0, -0.5);
            glVertex3f(-0.5, 0, -0.5);

            //yellow
            glColor3f(1, 1, 0);
            glVertex3f(0.5, 1, +0.5);
            glVertex3f(-0.5, 1, +0.5);
            glVertex3f(-0.5, 1, -0.5);
            glVertex3f(0.5, 1, -0.5);
        glDisable( GL_DEPTH_TEST );
        glEnd();
    glPopMatrix();
}
void initRenderingObjects(struct Vector3f *aScale, struct Vector3f *aRotation, struct Vector3f *aTranslation,struct gameState *gamestate){
    //Should I put Scale,Rotation and Trasnaltion in init() function?
    //Road should be initialized on very start, should I mix it with those funcs or keep here?
    //Road should be initialized once only, do I need to keep checking on it? Future versions of game?
    //roadScalef
    aScale->x = 6; // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    aScale->y = 1; // road will be 1m thick - no reason to change
    aScale->z = 1000; // road length will be 1000m - prone to change // its not rly 1000, its 200m long cuz of matrices order
    //roadRotation
    aRotation->x = 90;  //angle
    aRotation->y = 0;  //angle
    aRotation->z = 0;  //angle
    //roadTranslation
    aTranslation->x = 0;
    aTranslation->y = 0;
    aTranslation->z = 0;
    // tank translate
    tank.tankTranslate.x = 0; // its 0 on start, it changes during gameplay
    tank.tankTranslate.y = 0; // should always stay 0
    tank.tankTranslate.z = 31.5; // TODO: can be changed, for now it forces tank on begining of screen(NOT BEGINING OF ROAD!)
    gamestate->tankMainPlayer = tank;
    gs.carSpeed = 1; //TODO: is this needed? idea is to use this * times of clicking W and to get move speed, but not sure if needed
    gs.numOfCars = 1;
    // Initialize first car.
    srand(time(NULL));
    int setOfCarXPositionsAllowedValues[3] = {-3.33,0,3.33};
    int setOfCarZPositionsAllowedValues[6] = {-150,-140,-130,-100,-120,-110};
    for(int i = 0; i < MAX_CARS_ALLOWED; i++){
        gs.carNumber[i].carPosition.x = setOfCarXPositionsAllowedValues[rand()%3];
        gs.carNumber[i].carPosition.y = 0;
        gs.carNumber[i].carPosition.z = setOfCarZPositionsAllowedValues[rand()%6];
    }
    
}
