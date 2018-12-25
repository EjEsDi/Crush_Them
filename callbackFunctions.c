#include "callbackFunctions.h"
#include "drawFunctions.h"
#include "lightingFunctions.h"
#include "mathFunctions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

/************************************
    Functions definitions start here
*************************************/

void onDisplay(void){
    //remove old content
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //setting camera position and where it looks at
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  0, 3, gs.tankMainPlayer.tankPosition.z+8, // camera position
                0, 0, gs.cameraMovement-10, // camera looks at this spot
                0, 1, 0  // normal vector 
            );

    light();

    //Rendering section

    drawRoad(gs.road);
    drawRoad(gs.road2);
    drawRoad(gs.road3);
    drawSideRoad(gs.rightSideRoad);
    drawSideRoad(gs.rightSideRoad2);
    drawSideRoad(gs.rightSideRoad3);
    drawSideRoad(gs.leftSideRoad);
    drawSideRoad(gs.leftSideRoad2);
    drawSideRoad(gs.leftSideRoad3);

    glDisable(GL_LIGHTING);
    drawScore();
    if(gs.gameover == true && gs.actionOnGoing == 0){
        drawEndGame();
        glutSwapBuffers();
        sleep(3);
        exit(0);
    }

    glEnable(GL_LIGHTING);

    glDisable(GL_LIGHT0);
    lightForSun();
    drawSun();
    glDisable(GL_LIGHT1);
    glEnable(GL_LIGHT0);
    
    for (int i = 0; i < gs.car.numOfCars; i++){
        drawCar(gs.carArray[i]); 
    }
    drawCubeTank(gs.tankMainPlayer);

    if (gs.tankMainPlayer.shoot){
        drawBullet();
    }
    
    // draw all on main buffer
    glutSwapBuffers();
}
void onReshape(int w, int h){
    gs.WindowWidth = w;
    gs.WindowHeight = h;
    glViewport(0,0, gs.WindowWidth, gs.WindowHeight); //width and height of screen where stuff is drawn in end
    //projection of what camera sees
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, gs.WindowWidth/(GLfloat)gs.WindowHeight, 1, 300.0f); // angle, ratio, near clip, far clip
}
void onKeyboardInput(unsigned char key, int x, int y){
    NOT_USED_VAR(x);
    NOT_USED_VAR(y);
    switch(key){
        case 27: // ESC BUTTON
            glDeleteTextures(2, names);
            exit(0);
        case 'g': // GO
        case 'G':
            if(gs.actionOnGoing == 0){
                glutTimerFunc((unsigned)gs.car.timeCarSpawn, onTimer, carSpeedTimer);
                glutTimerFunc((unsigned)gs.car.carSpeed, onTimer, carSpawnTimer);
                glutTimerFunc((unsigned)gs.tankMainPlayer.tankSpeed, onTimer, tankMovementTimer);
                glutTimerFunc((unsigned)gs.sky.dayTimer, onTimer, skyColorTimer);
                glutPostRedisplay();
                gs.actionOnGoing = 1;
            }
            break;
        case 'p': // PAUSE
        case 'P':
            if(gs.actionOnGoing == 1){
                gs.actionOnGoing = 0;
            }
            break;
        // left
        case 'a': 
        case 'A':
            gs.tankMainPlayer.prevDir = gs.tankMainPlayer.currDir;
            gs.tankMainPlayer.currDir = -1;
            gs.tankMainPlayer.prevDir = 0;
            break;
        // right
        case 'd':
        case 'D':
            gs.tankMainPlayer.prevDir = gs.tankMainPlayer.currDir;
            gs.tankMainPlayer.currDir = 1;
            gs.tankMainPlayer.prevDir = 0;
            break;
        default:
            break;
    }    
    glutPostRedisplay();
}
void onKeyboardUp(unsigned char key, int x, int y){
    NOT_USED_VAR(x);
    NOT_USED_VAR(y);
    switch(key){
        // left
        case 'a': 
        case 'A':
            if(gs.tankMainPlayer.currDir == -1){
                gs.tankMainPlayer.currDir = gs.tankMainPlayer.prevDir;
                gs.tankMainPlayer.prevDir = 0;
            }
            break;
        // right
        case 'd':
        case 'D':
            if(gs.tankMainPlayer.currDir == 1){
                gs.tankMainPlayer.currDir = gs.tankMainPlayer.prevDir;
                gs.tankMainPlayer.prevDir = 0;
            }
            break;
        default:
            break;
    }    
    glutPostRedisplay();
}
void tankShoot(int button, int state, int x, int y){
    NOT_USED_VAR(x);
    NOT_USED_VAR(y);
    if (gs.actionOnGoing == 1)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                gs.tankMainPlayer.shoot = true;
                if(gs.bullet.Charging == 0 && gs.tankMainPlayer.shoot){
                glPushMatrix();
                    glLoadIdentity();
                    setTankTurretMatrix();
                    
                    setBulletMatrix();
                    
                glPopMatrix();
                }
            }
        }
    }
}
void onMousePassive(int x, int y){
    NOT_USED_VAR(y);
    if(gs.actionOnGoing == 0)
        return;
    gs.tankMainPlayer.turretRotate.x = 0.1f * (gs.lastMouseX - x);
    gs.lastMouseX = x;
    gs.lastMouseX = gs.WindowWidth/2;
}
void onTimer(int timer){
    if (timer == carSpeedTimer){
        // this timer moves cars
        if(gs.actionOnGoing){
            if (gs.tankMainPlayer.shoot)
            {
                gs.bullet.bulletPosition.x -= gs.bullet.bulletDirection.x;
                gs.bullet.bulletPosition.y -= 0.03f;
                gs.bullet.bulletPosition.z -= gs.bullet.bulletDirection.z;
                gs.bullet.Charging++;
                gs.bullet.needToResetBullet = false;

                if (gs.bullet.bulletPosition.y <= -1){ // -1 on Y is when bomb reaches floor
                    gs.bullet.needToResetBullet = true;
                    gs.bullet.Charging = 0;
                }
            }
                for (int i = 0; i < gs.car.numOfCars; i++)
                {
                    if (!gs.bullet.needToResetBullet && collisionCheck(gs.bullet.bulletPosition, gs.carArray[i].carPosition, gs.bullet.scale, gs.carArray[i].carScale)){
                        gs.bullet.needToResetBullet = true;
                        gs.bullet.Charging = 0;
                        if (gs.carArray[i].showShield == 0)
                        {
                            gs.carArray[i].carPosition.x = gs.car.setOfCarXPositionsAllowedValues[rand() % 3];
                            gs.carArray[i].carPosition.z = gs.car.lastZPoint - 10; // reposition car
                            gs.car.lastZPoint = (int)gs.carArray[i].carPosition.z; // last car spawn point.
                            gs.numberOfCrushes++;
                        }
                        if (gs.carArray[i].shieldOpacity > 0)
                        {
                            gs.carArray[i].shieldOpacity -= 0.5f;
                            if (gs.carArray[i].shieldOpacity == 0)
                                gs.carArray[i].showShield = 0;
                        }
                        break;
                    }
                    
                    gs.carArray[i].carPosition.z += 1;
                    if (gs.carArray[i].carPosition.z - 10 >= gs.tankMainPlayer.tankPosition.z + 10){
                        if (gs.numberOfCrushes >= 10)
                        {
                            if (gs.carArray[i].shieldOpacity < 1)
                            {
                                gs.carArray[i].shieldOpacity += 0.5f;
                                gs.carArray[i].showShield = 1;
                            }
                        }
                        gs.carArray[i].carPosition.x = gs.car.setOfCarXPositionsAllowedValues[rand() % 3];
                        gs.carArray[i].carPosition.z = gs.car.lastZPoint - 10; // reposition car
                        gs.car.lastZPoint = (int)gs.carArray[i].carPosition.z;  // last car spawn point.
                    }
                    else if (collisionCheck(gs.tankMainPlayer.tankPosition, gs.carArray[i].carPosition,
                            gs.tankMainPlayer.tankScale, gs.carArray[i].carScale)){
                        if (gs.carArray[i].showShield == 1 && gs.carArray[i].shieldOpacity == 1)
                        { // If there is collision with car with full shield. Game over.
                            gs.gameover = true;
                            gs.actionOnGoing = 0;
                        }
                        else
                        {
                            if (gs.carArray[i].shieldOpacity > 0)
                                gs.carArray[i].shieldOpacity -= 0.5f;
                            if (gs.carArray[i].shieldOpacity == 0)
                                gs.carArray[i].showShield = 0;
                        }
                        gs.numberOfCrushes++;
                        gs.carArray[i].carPosition.x = gs.car.setOfCarXPositionsAllowedValues[rand() % 3];
                        gs.carArray[i].carPosition.z = gs.car.lastZPoint - 10; // reposition car
                        gs.car.lastZPoint = (int)gs.carArray[i].carPosition.z; // last car spawn point.
                    }
            }
            if (gs.tankMainPlayer.shoot && gs.bullet.needToResetBullet)
            {
                gs.tankMainPlayer.shoot = false;
            }
        }
    }else if (timer == carSpawnTimer){
        //This timer makes cars spawn in proper timers
        if(gs.car.numOfCars < MAX_CARS_ALLOWED)
            gs.car.numOfCars++;
        else
            return;
    }else if (timer == tankMovementTimer){
        if(gs.actionOnGoing){
            if (gs.tankMainPlayer.shoot)
            {                                              
                gs.bullet.bulletPosition.x -= gs.bullet.bulletDirection.x;
                gs.bullet.bulletPosition.y -= 0.03f;
                gs.bullet.bulletPosition.z -= gs.bullet.bulletDirection.z;
                gs.bullet.Charging++;
                gs.bullet.needToResetBullet = false;
                if(gs.bullet.bulletPosition.y <= -1){ // -1 on Y is when bomb reaches floor
                    gs.bullet.needToResetBullet = true;
                    gs.bullet.Charging = 0;
                }
                for (int i = 0; i < gs.car.numOfCars; i++)
                {
                    if (collisionCheck(gs.bullet.bulletPosition, gs.carArray[i].carPosition,
                            gs.bullet.scale, gs.carArray[i].carScale)){
                        gs.bullet.Charging = 0;
                        if (gs.carArray[i].showShield == 0)
                        {
                            gs.carArray[i].carPosition.x = gs.car.setOfCarXPositionsAllowedValues[rand() % 3];
                            gs.carArray[i].carPosition.z = gs.car.lastZPoint - 10; // reposition car
                            gs.car.lastZPoint = (int)gs.carArray[i].carPosition.z; // last car spawn point.
                            gs.numberOfCrushes++;
                        }
                        if (gs.carArray[i].shieldOpacity > 0)
                        {                                
                            gs.carArray[i].shieldOpacity -= 0.5f;
                            if (gs.carArray[i].shieldOpacity == 0)
                                gs.carArray[i].showShield = 0;
                        }
                        gs.bullet.needToResetBullet = true;
                        break;
                    }
                }
                if(gs.bullet.needToResetBullet){
                    gs.tankMainPlayer.shoot = false;
                }
            }

            // moves tank left and right when keyboard input comes in.
            if (gs.tankMainPlayer.currDir == -1){
                if(gs.tankMainPlayer.tankPosition.x >= -3.8f)
                    gs.tankMainPlayer.tankPosition.x -= 0.2f;
            }else if (gs.tankMainPlayer.currDir == 1){
                if(gs.tankMainPlayer.tankPosition.x <= 3.8f)
                    gs.tankMainPlayer.tankPosition.x += 0.2f;
            }
            gs.tankMainPlayer.tankPosition.z -= 1; // keeps moving tank forward
            gs.cameraMovement -= 1; // keeps moving camera with tank
            //makes road infinite
            if(gs.tankMainPlayer.tankPosition.z == gs.road2.roadPosition.z){
                // Road 1 should be copied in back, behind road 3
                gs.road.roadPosition.z = gs.road3.roadPosition.z - 2*gs.road.roadScale.z;
                gs.leftSideRoad.roadPosition.z = gs.leftSideRoad3.roadPosition.z - 2 * gs.leftSideRoad.roadScale.z;
                gs.rightSideRoad.roadPosition.z = gs.rightSideRoad3.roadPosition.z - 2 * gs.rightSideRoad.roadScale.z;
            }else if(gs.tankMainPlayer.tankPosition.z == gs.road3.roadPosition.z){
                // Road 2 should be copied in back, behind road 1
                gs.road2.roadPosition.z = gs.road.roadPosition.z - 2*gs.road2.roadScale.z;
                gs.leftSideRoad2.roadPosition.z = gs.leftSideRoad.roadPosition.z - 2 * gs.leftSideRoad2.roadScale.z;
                gs.rightSideRoad2.roadPosition.z = gs.rightSideRoad.roadPosition.z - 2 * gs.rightSideRoad2.roadScale.z;
            }else if(gs.tankMainPlayer.tankPosition.z == gs.road.roadPosition.z){
                // Road 3 should be copied in back, behind road 2
                gs.road3.roadPosition.z = gs.road2.roadPosition.z - 2*gs.road3.roadScale.z;
                gs.leftSideRoad3.roadPosition.z = gs.leftSideRoad2.roadPosition.z - 2 * gs.leftSideRoad3.roadScale.z;
                gs.rightSideRoad3.roadPosition.z = gs.rightSideRoad2.roadPosition.z - 2 * gs.rightSideRoad3.roadScale.z;
            }
        }
    }else if(timer == skyColorTimer){
        if(gs.actionOnGoing)
            skyChangeFunction();
    }else
        return;
    if(gs.actionOnGoing){
        glutPostRedisplay();
        if (timer == carSpeedTimer)
            glutTimerFunc((unsigned)gs.car.carSpeed, onTimer, carSpeedTimer);
        else if (timer == carSpawnTimer)
            glutTimerFunc((unsigned)gs.car.timeCarSpawn, onTimer, carSpawnTimer);
        else if(timer == tankMovementTimer)
            glutTimerFunc((unsigned)gs.tankMainPlayer.tankSpeed, onTimer, tankMovementTimer);
        else if(timer == skyColorTimer)
            glutTimerFunc((unsigned)gs.sky.dayTimer, onTimer, skyColorTimer);
    }
}
