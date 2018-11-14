#include "callbackFunctions.h"
#include "drawFunctions.h"
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

/************************************
    Functions definitions start here
*************************************/

void onDisplay(void){
    //remove old content
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //setting camera position and where it looks at
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  0, 3, gs.tankMainPlayer.tankTranslate.z+7, // camera position 
                0, 0, gs.cameraMovement-20, // camera looks at this spot
                0, 1, 0  // normal vector 
            ); 
    glutPostRedisplay();
    //Rendering section
    drawRoad(gs.road);
    drawCubeTank(gs.tankMainPlayer);
    
    for(int i = 0; i < gs.car.numOfCars; i++){
        drawCar(gs.carArray[i]); 
    }
    
    //drawSun(); // TODO fix sun :)
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
            exit(0);
            break;
        case 'g': // GO
        case 'G':
            if(gs.actionOnGoing == 0){
                glutTimerFunc(gs.car.timeCarSpawn, onTimer, timerID1);
                glutTimerFunc(gs.timeInMS, onTimer, timerID); //TODO delta movement
                glutTimerFunc(50, onTimer, timerID2); // todo figure good timer
                glutTimerFunc(50, onTimer, timerID3); // same. Fix in onTimer function as well
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
        case 's': // slow down tank
        case 'S':
            break;
        // left
        case 'a': 
        case 'A': //TODO delta movement
            if(gs.tankMainPlayer.tankTranslate.x - 3 >= -4) // left wall is not hit yet
                gs.tankMainPlayer.tankTranslate.x -= 3;
            glutPostRedisplay();
            break;
        // right
        case 'd':
        case 'D'://TODO delta movement
            if(gs.tankMainPlayer.tankTranslate.x + 3 <= 4) // right wall is not hit yet
                gs.tankMainPlayer.tankTranslate.x += 3;
            glutPostRedisplay();
            break;
        case 'w':
        case 'W': // can be used to speed up tank
            break;
    }    
    glutPostRedisplay();
}

void onTimer(int timer){  
    
    if(timer == timerID){
        //this timer moves cars
        for(int i = 0; i < gs.car.numOfCars; i++){
            gs.carArray[i].carPosition.z += 1;
            if(gs.carArray[i].carPosition.z - 10 >= gs.tankMainPlayer.tankTranslate.z + 10){ // car and tank move towards each other on Z
                gs.carArray[i].carPosition.x = gs.car.setOfCarXPositionsAllowedValues[rand()%3];
                gs.carArray[i].carPosition.z = gs.car.ZSpawnPoint;
            }
        }
    }else if(timer == timerID1){
        //This timer makes cars spawn in proper timers, by increasing number of cars that can be spawned.
        if(gs.car.numOfCars < MAX_CARS_ALLOWED)
            gs.car.numOfCars++;
        else
            return;
    }else if(timer == timerID2){
        if(gs.actionOnGoing){
            gs.tankMainPlayer.tankTranslate.z -= 1;
            gs.cameraMovement -= 1;
            
        }
    }else if(timer == timerID3){
        gs.road.roadTranslation.z += 1;
    }else
        return;

    if(gs.actionOnGoing){
        glutPostRedisplay();
        if(timer == timerID)
            glutTimerFunc(gs.timeInMS, onTimer, timerID);
        else if(timer == timerID1)
            glutTimerFunc(gs.car.timeCarSpawn, onTimer, timerID1);
        else if(timer == timerID2)
            glutTimerFunc(50, onTimer, timerID2);
        else if(timer == timerID3)
            glutTimerFunc(50, onTimer, timerID3);
    }
}