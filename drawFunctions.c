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
    initRenderingObjects(&gs); 
}

void drawSun(){
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(20, 10, -30);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();
}

void drawRoad(const struct Road road){
    
    glPushMatrix();
        glEnable(GL_DEPTH_TEST);
        
        glScalef(road.roadScale.x, road.roadScale.y, road.roadScale.z);
        glRotatef(road.roadRotation.x, 1, 0, 0);
        glRotatef(road.roadRotation.y, 0, 1, 0);
        glRotatef(road.roadRotation.z, 0, 0, 1);
        glTranslatef(road.roadTranslation.x, road.roadTranslation.y, road.roadTranslation.z);
        
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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable( GL_DEPTH_TEST );
        glScalef(tank.tankScale.x, tank.tankScale.y, tank.tankScale.z);
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
        glScalef(car.carScale.x, car.carScale.y, car.carScale.z);
        glTranslatef(car.carPosition.x, car.carPosition.y, car.carPosition.z);
        //glRotatef(car.carRotate.x, 1, 0, 0);
        //glRotatef(car.carRotate.y, 0, 1, 0);
        //glRotatef(car.carRotate.z, 0, 0, 1);
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
void initRenderingObjects(){
    
    gs.road.roadScale.x = 6; // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    gs.road.roadScale.y = 1; // road will be 1m thick - no reason to change
    gs.road.roadScale.z = 1000; //? road length will be 1000m - prone to change // its not rly 1000 right now, it is but its not.
    //Issue with road scale is that its just from -100 to +100 on z axis. Is this because of matrix order?
    //if i put roadscale.z on 200, it will shrink road a lot.

    gs.road.roadRotation.x = 90; //angle
    gs.road.roadRotation.y = 0;  //angle
    gs.road.roadRotation.z = 0;  //angle

    gs.road.roadTranslation.x = 0;
    gs.road.roadTranslation.y = 0;
    gs.road.roadTranslation.z = 0;

    gs.tankMainPlayer.tankTranslate.x = 0;
    gs.tankMainPlayer.tankTranslate.y = 0;
    gs.tankMainPlayer.tankTranslate.z = 31.25; //? This number somewhat positions tank on start of road, in current setup(10.11.2018)

    gs.tankMainPlayer.tankScale.x = 1;
    gs.tankMainPlayer.tankScale.y = 1;
    gs.tankMainPlayer.tankScale.z = 3;

    gs.tankMainPlayer.tankSpeed = 1; //? same as for car speed for now idea is to use this * times of clicking W and to get speed
    
    // Init cars
    gs.car.numOfCars = 1;

    srand(time(NULL));
    gs.car.setOfCarXPositionsAllowedValues[0] = -3.33;
    gs.car.setOfCarXPositionsAllowedValues[1] = 0;
    gs.car.setOfCarXPositionsAllowedValues[2] = 3.33;
    gs.car.ZSpawnPoint = -130;

    for(int i = 0; i < MAX_CARS_ALLOWED; i++){
        gs.carArray[i].carSpeed = 1; //? not sure when this is gonna be needed
        
        gs.carArray[i].carScale.x = 1;
        gs.carArray[i].carScale.y = 1;
        gs.carArray[i].carScale.z = 1;
        
        gs.carArray[i].carRotate.x = 0;
        gs.carArray[i].carRotate.y = 0;
        gs.carArray[i].carRotate.z = 0;

        gs.carArray[i].carPosition.x = gs.car.setOfCarXPositionsAllowedValues[rand()%3];
        gs.carArray[i].carPosition.y = 0;
        gs.carArray[i].carPosition.z = gs.car.ZSpawnPoint;
    }
    //Timers for callback onTimer function
    gs.timeInMS = 20; // 20 ms
    gs.car.timeCarSpawn = 1000;   // 1 sec
}
