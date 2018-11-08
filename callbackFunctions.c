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
                0, 2, 0  // normal vector 
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
    //TODO when first car passes tank(-100 on z-axis for now)
    // when counter = 0?
    // when to do it, when first car is of screen would be good
    //todo what to do when numofcars > max cars allowed, exit game for now?
    srand(time(NULL));
    if(timer == timerID){
        //this timer moves cars
        for(int i = 0; i < gs.numOfCars; i++){
            gs.carNumber[i].carPosition.z += 1;
        }
    }else if(timer == timerID1){
        //This timer makes cars
        int setOfCarXPositionsAllowedValues[9] = {-3.33,0,3.33,0,-3.33, 3.33, 3.33,-3.33,0};
        int setOfCarZPositionsAllowedValues[4] = {-100,-110,-120,-130};
        gs.carNumber[gs.numOfCars].carPosition.x = setOfCarXPositionsAllowedValues[rand()%9];
        gs.carNumber[gs.numOfCars].carPosition.y = 0; // puts car on the road
        gs.carNumber[gs.numOfCars].carPosition.z = setOfCarZPositionsAllowedValues[rand()%4];
        gs.numOfCars++;
        gs.carNumber[gs.numOfCars].carPosition.x = setOfCarXPositionsAllowedValues[rand()%9];
        gs.carNumber[gs.numOfCars].carPosition.y = 0; // puts car on the road
        gs.carNumber[gs.numOfCars].carPosition.z = setOfCarZPositionsAllowedValues[rand()%4];
        gs.numOfCars++;
        gs.carNumber[gs.numOfCars].carPosition.x = setOfCarXPositionsAllowedValues[rand()%9];
        gs.carNumber[gs.numOfCars].carPosition.y = 0; // puts car on the road
        gs.carNumber[gs.numOfCars].carPosition.z = setOfCarZPositionsAllowedValues[rand()%4];
        gs.numOfCars++;
        if((gs.numOfCars >= MAX_CARS_ALLOWED)){
            gs.actionOnGoing = 0;
            return;
        }
    }else return;

    if(gs.actionOnGoing){
        glutPostRedisplay();
        if(timer == timerID)
            glutTimerFunc(gs.timeInMS, onTimer, timerID);
        else if(timer == timerID1)
            glutTimerFunc(gs.timeCarSpawn, onTimer, timerID1);
    }
}

