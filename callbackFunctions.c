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
    gluLookAt(  0, 3, 100, // camera position 
                0, 0, 0, // camera looks at this spot
                0, 1, 0  // normal vector 
            ); 
    
    //Rendering section
    drawRoad(roadScale, roadRotation, roadTranslation);
    drawCubeTank(gs.tankMainPlayer);
    for(int i = 0; i < gs.numOfCars; i++){
        drawCar(gs.carNumber[i]); 
    }
    drawSun();
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
    gluPerspective(60, gs.WindowWidth/(GLfloat)gs.WindowHeight, 1.0, 200.0); // angle, ratio, near clip, far clip
}

void onKeyboardInput(unsigned char key, int x, int y){
    NOT_USED_VAR(x);
    NOT_USED_VAR(y);
    //TODO : Speed could/should be adjusted, to match spawns and movement, realistically, not big deal rn tho
    switch(key){
        case 27: // ESC BUTTON
            exit(0);
            break;
        case 'g': // GO
        case 'G':
            if(gs.actionOnGoing == 0){
                glutTimerFunc(gs.timeCarSpawn, onTimer, timerID1);
                glutTimerFunc(gs.timeInMS, onTimer, timerID);
                gs.actionOnGoing = 1;
            }
            break;
        case 'p': // PAUSE
        case 'P':
            if(gs.actionOnGoing == 1){
                gs.actionOnGoing = 0;
            }
            break;
        case 's': // slow down car
        case 'S':
            break;
        // left
        case 'a': 
        case 'A': 
            if(gs.tankMainPlayer.tankTranslate.x - 3 >= -4) // left wall is not hit yet
                gs.tankMainPlayer.tankTranslate.x -= 3;
            glutPostRedisplay();
            break;
        // right
        case 'd':
        case 'D':
            if(gs.tankMainPlayer.tankTranslate.x + 3 <= 4) // right wall is not hit yet
                gs.tankMainPlayer.tankTranslate.x += 3;
            glutPostRedisplay();
            break;
        case 'w':
        case 'W': // can be used to speed up 
            break;
    }    
    glutPostRedisplay();
}

void onTimer(int timer){  
    int setOfCarXPositionsAllowedValues[3] = {-3.33,0,3.33};
    if(timer == timerID){
        //this timer moves cars
        for(int i = 0; i < gs.numOfCars; i++){
            gs.carNumber[i].carPosition.z += 1;
            //translate * 3 is because of how tank is made(scale<-translate),EDIT THERE WHEN NEEDED
            if(gs.carNumber[i].carPosition.z >= tank.tankTranslate.z*3){ 
                gs.carNumber[i].carPosition.x = setOfCarXPositionsAllowedValues[rand()%3];
                gs.carNumber[i].carPosition.z = -130;
            }
        }
    }else if(timer == timerID1){
        //This timer makes cars
        if(gs.numOfCars < MAX_CARS_ALLOWED)
            gs.numOfCars++;
        else
            return;
    }else return;

    if(gs.actionOnGoing){
        glutPostRedisplay();
        if(timer == timerID)
            glutTimerFunc(gs.timeInMS, onTimer, timerID);
        else if(timer == timerID1)
            glutTimerFunc(gs.timeCarSpawn, onTimer, timerID1);
    }
}

