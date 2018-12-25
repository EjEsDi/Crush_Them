//
// Created by aleksandar on 24.12.18..
//

#include "drawInitFunctions.h"
#include "mathFunctions.h"
#include "image.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <time.h>

void init(void){
    initRenderingObjects();
    srand(time(NULL));
    glClearColor(gs.sky.skyColor.x, gs.sky.skyColor.y, gs.sky.skyColor.z, 0);
    glClearDepth(1.0f);
    glLineWidth(1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void tankInit(){
    gs.tankMainPlayer.tankPosition.x = 0;
    gs.tankMainPlayer.tankPosition.y = -1;
    gs.tankMainPlayer.tankPosition.z = 280;

    gs.tankMainPlayer.tankScale.x = 1;
    gs.tankMainPlayer.tankScale.y = 1;
    gs.tankMainPlayer.tankScale.z = 3;

    gs.tankMainPlayer.turretRotate.x = 0;

    gs.tankMainPlayer.tankSpeed = 30;
    gs.tankMainPlayer.currDir = 0;
    gs.tankMainPlayer.prevDir = 0;

    gs.lastMouseX = gs.WindowHeight/2;

    gs.tankMainPlayer.shoot = false;
}

void roadInit(){
    gs.road.roadScale.x = 6; // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    gs.road.roadScale.y = 1; //
    gs.road.roadScale.z = 300; // road is 300m long.
    //Road is starting in 0,0,300 and goes until 0,0,-300

    gs.road.roadRotation.x = 90; //angle
    gs.road.roadRotation.y = 0;  //angle
    gs.road.roadRotation.z = 0;  //angle

    gs.road.roadPosition.x = 0;
    gs.road.roadPosition.y = 0;
    gs.road.roadPosition.z = 0;

    gs.road2.roadScale.x = 6; // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    gs.road2.roadScale.y = 1; //
    gs.road2.roadScale.z = 300; // road2 is 300m long.
    //Road is starting in 0,0,-300 and goes until 0,0,-900
    gs.road2.roadRotation.x = 90; //angle
    gs.road2.roadRotation.y = 0;  //angle
    gs.road2.roadRotation.z = 0;  //angle

    //Road 3 is 300m long and is starting on 0,0,-300 goes until 0,0,-900
    gs.road2.roadPosition.x = 0;
    gs.road2.roadPosition.y = 0;
    gs.road2.roadPosition.z = -600;

    gs.road3.roadScale.x = 6; // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    gs.road3.roadScale.y = 1; //
    gs.road3.roadScale.z = 300; // road3 is 300m long

    gs.road3.roadRotation.x = 90; //angle
    gs.road3.roadRotation.y = 0;  //angle
    gs.road3.roadRotation.z = 0;  //angle

    //Road 3 is 300m long and is starting on 0,0,-900 goes until 0,0,-1500
    gs.road3.roadPosition.x = 0;
    gs.road3.roadPosition.y = 0;
    gs.road3.roadPosition.z = -1200;
}

void bulletInit(){

    glPushMatrix();
    glLoadIdentity();
        setTankTurretMatrix();
    glPopMatrix();

    gs.bullet.needToResetBullet = false;
    gs.bullet.Charging = 0;

    gs.bullet.scale.x = 1;
    gs.bullet.scale.y = 1;
    gs.bullet.scale.z = 1;
}

void carsInit(){
    // Init cars
    gs.car.numOfCars = 1; // used for drawing cars.

    gs.car.setOfCarXPositionsAllowedValues[0] = -3.2;
    gs.car.setOfCarXPositionsAllowedValues[1] = 0;
    gs.car.setOfCarXPositionsAllowedValues[2] = 3.2;
    gs.car.ZSpawnPoint = 300; // How far away from tank, cars should spawn
    gs.car.carSpeed = 30;
    for(int i = 0; i < MAX_CARS_ALLOWED; i++){
        gs.carArray[i].carScale.x = 1;
        gs.carArray[i].carScale.y = 1;
        gs.carArray[i].carScale.z = 1;

        gs.carArray[i].carRotate.x = 0;
        gs.carArray[i].carRotate.y = 180; //cars need to go forward
        gs.carArray[i].carRotate.z = 0;

        gs.carArray[i].carPosition.x = gs.car.setOfCarXPositionsAllowedValues[i % 3];
        gs.carArray[i].carPosition.y = -1;
        gs.carArray[i].carPosition.z = gs.car.ZSpawnPoint + i*20;

        gs.car.showShield = 0;
        gs.car.shieldOpacity = 0;
    }
    //Timers for callback onTimer function
    gs.car.timeCarSpawn = 1000;   // 1 sec
    gs.car.lastZPoint = -200;
}
void rightSideRoadInit(){
    //same as road, move it to side(left and right), different collor
    gs.rightSideRoad.roadScale.x = 18;   // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    gs.rightSideRoad.roadScale.y = 1;   //
    gs.rightSideRoad.roadScale.z = 300; // road is 300m long.
    //Road is starting in 0,0,300 and goes until 0,0,-300

    gs.rightSideRoad.roadRotation.x = 90; //angle
    gs.rightSideRoad.roadRotation.y = 0;  //angle
    gs.rightSideRoad.roadRotation.z = 0;  //angle

    gs.rightSideRoad.roadPosition.x = gs.rightSideRoad.roadScale.x + gs.road.roadScale.x;
    gs.rightSideRoad.roadPosition.y = 0;
    gs.rightSideRoad.roadPosition.z = 0;

    gs.rightSideRoad2.roadScale.x = 18;   // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    gs.rightSideRoad2.roadScale.y = 1;   //
    gs.rightSideRoad2.roadScale.z = 300; // road2 is 300m long.
    //Road is starting in 0,0,-300 and goes until 0,0,-900
    gs.rightSideRoad2.roadRotation.x = 90; //angle
    gs.rightSideRoad2.roadRotation.y = 0;  //angle
    gs.rightSideRoad2.roadRotation.z = 0;  //angle

    //Road 3 is 300m long and is starting on 0,0,-300 goes until 0,0,-900
    gs.rightSideRoad2.roadPosition.x = gs.rightSideRoad2.roadScale.x + gs.road2.roadScale.x;
    gs.rightSideRoad2.roadPosition.y = 0;
    gs.rightSideRoad2.roadPosition.z = -600;

    gs.rightSideRoad3.roadScale.x = 18;   // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    gs.rightSideRoad3.roadScale.y = 1;   //
    gs.rightSideRoad3.roadScale.z = 300; // road3 is 300m long

    gs.rightSideRoad3.roadRotation.x = 90; //angle
    gs.rightSideRoad3.roadRotation.y = 0;  //angle
    gs.rightSideRoad3.roadRotation.z = 0;  //angle

    //Road 3 is 300m long and is starting on 0,0,-900 goes until 0,0,-1500
    gs.rightSideRoad3.roadPosition.x = gs.rightSideRoad3.roadScale.x + gs.road3.roadScale.x;
    gs.rightSideRoad3.roadPosition.y = 0;
    gs.rightSideRoad3.roadPosition.z = -1200;
}
void leftSideRoadInit()
{
    //same as road, move it to side(left and right), different collor
    gs.leftSideRoad.roadScale.x = 18; // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and right
    gs.leftSideRoad.roadScale.y = 1; //
    gs.leftSideRoad.roadScale.z = 300; // road is 300m long.
    //Road is starting in 0,0,300 and goes until 0,0,-300

    gs.leftSideRoad.roadRotation.x = 90; //angle
    gs.leftSideRoad.roadRotation.y = 0;  //angle
    gs.leftSideRoad.roadRotation.z = 0;  //angle

    gs.leftSideRoad.roadPosition.x = -gs.leftSideRoad.roadScale.x - gs.road.roadScale.x;
    gs.leftSideRoad.roadPosition.y = 0;
    gs.leftSideRoad.roadPosition.z = 0;

    gs.leftSideRoad2.roadScale.x = 18; // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and left
    gs.leftSideRoad2.roadScale.y = 1; //
    gs.leftSideRoad2.roadScale.z = 300; // road2 is 300m long.
    //Road is starting in 0,0,-300 and goes until 0,0,-900
    gs.leftSideRoad2.roadRotation.x = 90; //angle
    gs.leftSideRoad2.roadRotation.y = 0;  //angle
    gs.leftSideRoad2.roadRotation.z = 0;  //angle

    //Road 3 is 300m long and is starting on 0,0,-300 goes until 0,0,-900
    gs.leftSideRoad2.roadPosition.x = -gs.leftSideRoad.roadScale.x - gs.road.roadScale.x;
    gs.leftSideRoad2.roadPosition.y = 0;
    gs.leftSideRoad2.roadPosition.z = -600;

    gs.leftSideRoad3.roadScale.x = 18; // road width will be 6m - prone to change -- if it changes, need to account change with car positions and how much tank can move to left and left
    gs.leftSideRoad3.roadScale.y = 1; //
    gs.leftSideRoad3.roadScale.z = 300; // road3 is 300m long

    gs.leftSideRoad3.roadRotation.x = 90; //angle
    gs.leftSideRoad3.roadRotation.y = 0;  //angle
    gs.leftSideRoad3.roadRotation.z = 0;  //angle

    //Road 3 is 300m long and is starting on 0,0,-900 goes until 0,0,-1500
    gs.leftSideRoad3.roadPosition.x = -gs.leftSideRoad.roadScale.x - gs.road.roadScale.x;
    gs.leftSideRoad3.roadPosition.y = 0;
    gs.leftSideRoad3.roadPosition.z = -1200;
}
void skyInit(){
    gs.sky.skyColor.x = 0.6f;
    gs.sky.skyColor.y = 0.8f;
    gs.sky.skyColor.z = 1;
    gs.sky.dayTimer = 30;
    gs.sky.flag = 0; // while its 0 its peak of day, when 1 its peak of night
}
void sunInit(){
    gs.sun.sunRotate.x = 0;
    gs.sun.sunRotate.y = 0;
    gs.sun.sunRotate.z = 0;
    gs.sun.sunTranslate.x = 0;
    gs.sun.sunTranslate.y = 45; // it doesnt go trough road with this value
    gs.sun.sunTranslate.z = -50;
}

void imageInit(){
    //Code taken from class and edited for own needs.
    Image * image;
    image = image_init(0,0);
    image_read(image, "/home/aleksandar/Desktop/Crush_Them/sand.bmp");
    glGenTextures(2, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    image_done(image);
}
void initRenderingObjects(){
    roadInit();
    carsInit();
    leftSideRoadInit();
    rightSideRoadInit();
    tankInit();
    skyInit();
    sunInit();
    bulletInit();
    imageInit();
    gs.cameraMovement = 0;
    gs.numberOfCrushes = 0;
    gs.gameover = false;
}
