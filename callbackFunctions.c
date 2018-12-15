#include "callbackFunctions.h"
#include "drawFunctions.h"
#include "lightingFunctions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "image.h"
#include <unistd.h>

/************************************
    Functions definitions start here
*************************************/

void onDisplay(void){
    //remove old content
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //setting camera position and where it looks at
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  0, 3, gs.tankMainPlayer.tankTranslate.z+8, // camera position 
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
    gluPerspective(60, gs.WindowWidth/(GLfloat)gs.WindowHeight, 1, 300.0); // angle, ratio, near clip, far clip
}
void onKeyboardInput(unsigned char key, int x, int y){
    NOT_USED_VAR(x);
    NOT_USED_VAR(y);
    switch(key){
        case 27: // ESC BUTTON
            glDeleteTextures(2, names);
            exit(0);
            break;
        case 'g': // GO
        case 'G':
            if(gs.actionOnGoing == 0){
                glutTimerFunc(gs.car.timeCarSpawn, onTimer, carSpeedTimer);
                glutTimerFunc(gs.car.carSpeed, onTimer, carSpawnTimer);
                glutTimerFunc(gs.tankMainPlayer.tankSpeed, onTimer, tankMovementTimer);
                glutTimerFunc(gs.sky.dayTimer, onTimer, skyColorTimer);
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
    }    
    glutPostRedisplay();
}

void tankShoot(int button, int state, int x, int y){
    NOT_USED_VAR(x);
    NOT_USED_VAR(y);
    if(gs.actionOnGoing == 1){
        if (button == GLUT_LEFT_BUTTON){
            if (state == GLUT_DOWN){
                gs.leftMouseDown = true;
                gs.tankMainPlayer.shoot = true;
            }
            else{
                gs.leftMouseDown = false;
            }
        }
    }
}

void onMousePassive(int x, int y){
    NOT_USED_VAR(y);
    if(gs.actionOnGoing == 0)
        return;
    gs.tankMainPlayer.rotateTurret.x = 0.1 * (gs.lastMouseX - x);
    gs.lastMouseX = x;
    gs.lastMouseX = gs.WindowWidth/2;
}

void onTimer(int timer){
    if (timer == carSpeedTimer){
        // 14.12.2018 , did some change again , still had issues with few cars on same spot. Looking for better solution.
        // right now if car passes u, it will go in same lane, over and over.
        // this timer moves cars
        for(int i = 0; i < gs.car.numOfCars; i++){
            gs.carArray[i].carTranslate.z += 1;
            
            if(gs.carArray[i].carTranslate.z - 10 >= gs.tankMainPlayer.tankTranslate.z + 10){ // respawn car
                if (gs.numberOfCrushes > 3){ // after 3 crushes, cars start getting shields
                    if (gs.carArray[i].shieldOpacity < 1){
                        gs.carArray[i].shieldOpacity += 0.5;
                        gs.carArray[i].showShield = 1;
                    }
                }
                
                if (gs.car.lastZPoint != gs.tankMainPlayer.tankTranslate.z - gs.car.ZSpawnPoint - 30)
                    gs.carArray[i].carTranslate.x = gs.car.setOfCarXPositionsAllowedValues[rand() % 3];
                else{
                    if(gs.carArray[gs.car.lastCar].carTranslate.x == gs.car.setOfCarXPositionsAllowedValues[2]){
                        gs.carArray[i].carTranslate.x = gs.car.setOfCarXPositionsAllowedValues[1];
                    }else if(gs.carArray[gs.car.lastCar].carTranslate.x == gs.car.setOfCarXPositionsAllowedValues[1]){
                        gs.carArray[i].carTranslate.x = gs.car.setOfCarXPositionsAllowedValues[0];
                    }else{
                        gs.carArray[i].carTranslate.x = gs.car.setOfCarXPositionsAllowedValues[2];
                    }
                }
                gs.carArray[i].carTranslate.z = gs.tankMainPlayer.tankTranslate.z - gs.car.ZSpawnPoint;
                gs.car.lastZPoint = gs.carArray[i].carTranslate.z;
            }else if(collisionCheck(gs.tankMainPlayer.tankTranslate, gs.carArray[i].carTranslate, gs.tankMainPlayer.tankScale, gs.carArray[i].carScale)){
                if(gs.carArray[i].showShield == 1 && gs.carArray[i].shieldOpacity == 1){ // If there is collision with car with full shield. Game over.
                    gs.gameover = true;
                    gs.actionOnGoing = 0;
                }else{
                    if(gs.carArray[i].shieldOpacity > 0)
                        gs.carArray[i].shieldOpacity -= 0.5;
                    if (gs.carArray[i].shieldOpacity == 0)
                        gs.carArray[i].showShield = 0;
                }
                gs.numberOfCrushes++;
                
                if (gs.car.lastZPoint != gs.tankMainPlayer.tankTranslate.z - gs.car.ZSpawnPoint){
                    gs.carArray[i].carTranslate.x = gs.car.setOfCarXPositionsAllowedValues[rand() % 3];
                    gs.carArray[i].carTranslate.z = gs.tankMainPlayer.tankTranslate.z - gs.car.ZSpawnPoint;
                }else{
                    if(gs.carArray[gs.car.lastCar].carTranslate.x == gs.car.setOfCarXPositionsAllowedValues[2]){
                        gs.carArray[i].carTranslate.x = gs.car.setOfCarXPositionsAllowedValues[1];
                    }else if(gs.carArray[gs.car.lastCar].carTranslate.x == gs.car.setOfCarXPositionsAllowedValues[1]){
                        gs.carArray[i].carTranslate.x = gs.car.setOfCarXPositionsAllowedValues[0];
                    }else{
                        gs.carArray[i].carTranslate.x = gs.car.setOfCarXPositionsAllowedValues[2];
                    }
                    gs.carArray[i].carTranslate.z = gs.tankMainPlayer.tankTranslate.z - gs.car.ZSpawnPoint - 30;
                }
                gs.car.lastZPoint = gs.carArray[i].carTranslate.z;
            }
            gs.car.lastCar = i;
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
                if (gs.bullet.movement.y >= -1){                                                  
                    //-1 is when bomb reaches floor, one other case to reset is collision
                    gs.bullet.movement.x -= gs.bullet.direction.x; //TODO this aint right
                    gs.bullet.movement.y -= 0.03;
                    gs.bullet.movement.z -= 1;
                }
                /*else if (collisionCheck(gs.bullet.position, gs.carArray[i].carTranslate, gs.bullet.scale, gs.carArray[i].carScale))
                {
                    if (gs.carArray[i].shieldOpacity > 0)
                    {
                        gs.carArray[i].shieldOpacity -= 0.5;
                        if (gs.carArray[i].shieldOpacity == 0)
                            gs.carArray[i].showShield = 0;
                    }
                }*/
                else{
                    gs.tankMainPlayer.shoot = false;
                    gs.bullet.movement.x = 0;
                    gs.bullet.movement.y = 0;
                    gs.bullet.movement.z = 0;
                }
            }

            // moves tank left and right when keyboard input comes in.
            if (gs.tankMainPlayer.currDir == -1){
                if(gs.tankMainPlayer.tankTranslate.x >= -3.8)
                    gs.tankMainPlayer.tankTranslate.x -= 0.2;
            }else if (gs.tankMainPlayer.currDir == 1){
                if(gs.tankMainPlayer.tankTranslate.x <= 3.8)
                    gs.tankMainPlayer.tankTranslate.x += 0.2;
            }
            gs.tankMainPlayer.tankTranslate.z -= 1; // keeps moving tank forward
            gs.cameraMovement -= 1; // keeps moving camera with tank
            //makes road infinite
            if(gs.tankMainPlayer.tankTranslate.z == gs.road2.roadTranslation.z){
                // Road 1 should be copied in back, behind road 3
                gs.road.roadTranslation.z = gs.road3.roadTranslation.z - 2*gs.road.roadScale.z;
                gs.leftSideRoad.roadTranslation.z = gs.leftSideRoad3.roadTranslation.z - 2 * gs.leftSideRoad.roadScale.z;
                gs.rightSideRoad.roadTranslation.z = gs.rightSideRoad3.roadTranslation.z - 2 * gs.rightSideRoad.roadScale.z;
            }else if(gs.tankMainPlayer.tankTranslate.z == gs.road3.roadTranslation.z){
                // Road 2 should be copied in back, behind road 1
                gs.road2.roadTranslation.z = gs.road.roadTranslation.z - 2*gs.road2.roadScale.z;
                gs.leftSideRoad2.roadTranslation.z = gs.leftSideRoad.roadTranslation.z - 2 * gs.leftSideRoad2.roadScale.z;
                gs.rightSideRoad2.roadTranslation.z = gs.rightSideRoad.roadTranslation.z - 2 * gs.rightSideRoad2.roadScale.z;
            }else if(gs.tankMainPlayer.tankTranslate.z == gs.road.roadTranslation.z){
                // Road 3 should be copied in back, behind road 2
                gs.road3.roadTranslation.z = gs.road2.roadTranslation.z - 2*gs.road3.roadScale.z;
                gs.leftSideRoad3.roadTranslation.z = gs.leftSideRoad2.roadTranslation.z - 2 * gs.leftSideRoad3.roadScale.z;
                gs.rightSideRoad3.roadTranslation.z = gs.rightSideRoad2.roadTranslation.z - 2 * gs.rightSideRoad3.roadScale.z;
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
            glutTimerFunc(gs.car.carSpeed, onTimer, carSpeedTimer);
        else if (timer == carSpawnTimer)
            glutTimerFunc(gs.car.timeCarSpawn, onTimer, carSpawnTimer);
        else if(timer == tankMovementTimer)
            glutTimerFunc(gs.tankMainPlayer.tankSpeed, onTimer, tankMovementTimer);
        else if (timer == skyColorTimer)
            glutTimerFunc(gs.sky.dayTimer, onTimer, skyColorTimer);
    }
}
