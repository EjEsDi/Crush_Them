#include "callbackFunctions.h"
#include "drawFunctions.h"
#include <GL/glut.h>
#include <stdio.h>

/************************************
    Functions definitions start here
*************************************/


void onDisplay(void){
    //remove old content
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //setting camera position and where it looks at
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();// TODO:not best position of camera, if i need to put on begining of road
    gluLookAt(  0, 3, 20, // camera position 
                0, 0, 0, // camera looks at this spot
                0, 1, 0  // normal vector 
            ); 
    
    //Rendering section
    drawRoad(roadScale, roadRotation, roadTranslation);
    drawCubeTank(gs.tankMainPlayer);
    for(int i = 0; i < MAX_CARS_ALLOWED; i++)
        drawCar(gs.carNumber[i]);
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
    gluPerspective(60, gs.WindowWidth/(GLfloat)gs.WindowHeight, 1.0, 2000.0); // angle, ratio, near clip, far clip
}

void onKeyboardInput(unsigned char key, int x, int y){
    NOT_USED_VAR(x);
    NOT_USED_VAR(y);
    //TODO : Check if speed is properly working and if it works as intended
    switch(key){
        case 27: // ESC BUTTON
            exit(0);
            break;
        case 'g': // GO
        case 'G':
            if(gs.actionOnGoing == 0){
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
        case 's': // slow down instead of stop
        case 'S':
            if(gs.actionOnGoing){
                if(!gs.timeInMS == 500)
                    gs.timeInMS += 100;
            }               
            break;
        // left
        case 'a': 
        case 'A': 
            if(gs.tankMainPlayer.tankTranslate.x - 3 > -4) // left wall is not hit yet
                gs.tankMainPlayer.tankTranslate.x -= 3;
            break;
        // right
        case 'd':
        case 'D':
            if(gs.tankMainPlayer.tankTranslate.x + 3 < 4) // right wall is not hit yet
                gs.tankMainPlayer.tankTranslate.x += 3;
            break;
        case 'w':
        case 'W': // can be used to speed up
            if(gs.actionOnGoing){
                if(!gs.timeInMS == 100)
                    gs.timeInMS -= 100;
                glutTimerFunc(gs.timeInMS, onTimer, timerID);
            }  
            break;
    }    
    glutPostRedisplay();
}

void onTimer(int timer){
    //TODO for now just preset cars are moving.
    //Wait for other funcs to be fixed, then change here accordingly.
    
    if(timer == timerID){
        for(int i = 0; i < MAX_CARS_ALLOWED; i++){
            gs.carNumber[i].carPosition.z++;
        }
    }else return;
    
    glutPostRedisplay();

    if(gs.actionOnGoing){
        glutTimerFunc(gs.timeInMS, onTimer, timerID);
    }
}

