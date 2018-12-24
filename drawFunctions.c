#include "drawFunctions.h"
#include "lightingFunctions.h"
#include "drawInitFunctions.h"
#include "image.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>

/************************************
    Functions definitions start here
*************************************/

void drawSun(){
    glPushMatrix();
        glLoadIdentity();
        glRotatef(gs.sun.sunRotate.x, 1, 0, 0);
        glRotatef(gs.sun.sunRotate.y, 0, 1, 0);
        glRotatef(gs.sun.sunRotate.z, 0, 0, 1);
        glTranslatef(gs.sun.sunTranslate.x, gs.sun.sunTranslate.y, gs.sun.sunTranslate.z);
        setVertexColor(1, 1, 0);
        glutSolidSphere(1, 50, 50);
    glPopMatrix();
}
void skyChangeFunction(){
    if (gs.sun.sunRotate.z >= 360){ // instead of moduo 360
        gs.sun.sunRotate.z = 0;
    }
    gs.sun.sunRotate.z += 0.30;   //Math done on paper for this number
    //Decrement until first reaches 0, that will be night color
    if (gs.sky.flag == 0)
    {
        gs.sky.skyColor.x = (float)(gs.sky.skyColor.x - 0.001);
        gs.sky.skyColor.y = (float)(gs.sky.skyColor.y - 0.001);
        gs.sky.skyColor.z = (float)(gs.sky.skyColor.z - 0.001);
        if (gs.lightModifier < 0.4)
            gs.lightModifier += 0.001;
        if (gs.sky.skyColor.x < 0)
            gs.sky.flag = 1;
    }
    if (gs.sky.flag == 1)
    {
        gs.sky.skyColor.x = (float)(gs.sky.skyColor.x + 0.001);
        gs.sky.skyColor.y = (float)(gs.sky.skyColor.y + 0.001);
        gs.sky.skyColor.z = (float)(gs.sky.skyColor.z + 0.001);
        if (gs.lightModifier > 0)
            gs.lightModifier -= 0.001;
        if (gs.sky.skyColor.x > 0.6)
            gs.sky.flag = 0;
    }
    //Increment until u reach day.. (0.6, 0.8, 1, 0) <- day color
    glClearColor(gs.sky.skyColor.x, gs.sky.skyColor.y, gs.sky.skyColor.z, 0);
    

    if (gs.sun.quadrant == 1)
    {
        gs.sun.lightCoef.x -= gs.sun.mod;
        if (gs.sun.lightCoef.x <= -1)
        {
            gs.sun.lightCoef.x = -1;
            gs.sun.quadrant = 2;
        }
    }
    else if (gs.sun.quadrant == 2)
    {
        gs.sun.lightCoef.x += gs.sun.mod;
        if (gs.sun.lightCoef.x >= 0)
        {
            gs.sun.lightCoef.x = 0;
            gs.sun.quadrant = 3;
        }
    }
    else if (gs.sun.quadrant == 3)
    {
        gs.sun.lightCoef.x += gs.sun.mod;
        if (gs.sun.lightCoef.x >= 1)
        {
            gs.sun.lightCoef.x = 1;
            gs.sun.quadrant = 4;
        }
    }
    else if (gs.sun.quadrant == 4)
    {
        gs.sun.lightCoef.x -= gs.sun.mod;
        if (gs.sun.lightCoef.x <= 0)
        {
            gs.sun.lightCoef.x = 0;
            gs.sun.quadrant = 1;
        }
    }
}
void drawSquare(){
        struct Vector3f v3f = {.5, .5, .5};
        glBegin(GL_QUADS);
            //blue - front
            glNormal3f(0, 1, 0);
            setVertexColor(0, 0, 1);
            glVertex3f(-v3f.x, 0, -v3f.z);
            glVertex3f(-v3f.x, 1, -v3f.z);
            glVertex3f(v3f.x, 1, -v3f.z);
            glVertex3f(v3f.x, 0, -v3f.z);

            //green - back
            glNormal3f(0, -1, 0);
            setVertexColor(0, 1, 0);
            glVertex3f(-v3f.x, 0, v3f.z);
            glVertex3f(-v3f.x, 1, v3f.z);
            glVertex3f(v3f.x, 1, v3f.z);
            glVertex3f(v3f.x, 0, v3f.z);

            //sides
            // purple - right
            glNormal3f(1.0f, 0.0, 0.0);
            setVertexColor(1, 0, 1);
            glVertex3f(v3f.x, 0, -v3f.z);
            glVertex3f(v3f.x, 1, -v3f.z);
            glVertex3f(v3f.x, 1, +v3f.z);
            glVertex3f(v3f.x, 0, +v3f.z);

            //brown- left
            glNormal3f(-1.0f, 0.0, 0.0);
            setVertexColor(1, 0.6, 0);
            glVertex3f(-v3f.x, 0, -v3f.z);
            glVertex3f(-v3f.x, 1, -v3f.z);
            glVertex3f(-v3f.x, 1, +v3f.z);
            glVertex3f(-v3f.x, 0, +v3f.z);

            // top and bottom
            //white - bottom
            glNormal3f(0, -1, 0);
            setVertexColor(1, 1, 1);
            glVertex3f(-v3f.x, 0, v3f.z);
            glVertex3f(+v3f.x, 0, v3f.z);
            glVertex3f(+v3f.x, 0, -v3f.z);
            glVertex3f(-v3f.x, 0, -v3f.z);

            //yellow - top
            glNormal3f(0.0, 1.0f, 0.0);
            setVertexColor(1, 1, 0);
            glVertex3f(v3f.x, 1, +v3f.z);
            glVertex3f(-v3f.x, 1, +v3f.z);
            glVertex3f(-v3f.x, 1, -v3f.z);
            glVertex3f(v3f.x, 1, -v3f.z);
        glEnd();
}
void drawRoad(struct Road road){
    glPushMatrix();
        struct Vector3f v3f = {.5, .5, .5};

        glTranslatef(road.roadPosition.x, road.roadPosition.y-1.5f, road.roadPosition.z);
        glScalef(road.roadScale.x * 2.f, road.roadScale.y, road.roadScale.z * 2.f);
        glRotatef(road.roadRotation.x, 1, 0, 0);
        glRotatef(road.roadRotation.y, 0, 1, 0);
        glRotatef(road.roadRotation.z, 0, 0, 1);

        setVertexColor(0.3, 0.3, 0.3);
        glutSolidCube(1);
        
        glLineWidth(4);
        glBegin(GL_LINES);
                setVertexColor(1, 1, 1);
                //left line
                glNormal3f(0, 1, 0);
                glVertex3f(-v3f.x/3, -v3f.y, (float)(v3f.z-1.01)); // z-axis, -1.01 is just so we see lines
                glVertex3f(-v3f.x/3, v3f.y, (float)(v3f.z-1.01));  // they are just tiny bit above road it self
                //right line                                // better solution?
                glNormal3f(0, 1, 0);
                glVertex3f(v3f.x/3, -v3f.y, (float)(v3f.z-1.01));
                glVertex3f(v3f.x/3, v3f.y, (float)(v3f.z-1.01));
        glEnd();
    glPopMatrix();
}
void drawScore(){
    glColor3f(1, 0 ,0);
    glMatrixMode(GL_PROJECTION); // Take current project matrix
    glPushMatrix();  // Push so we work with new "copy" matrix
        glLoadIdentity(); // Identity
        glMatrixMode(GL_MODELVIEW); // Add modelMatrix to it
            glPushMatrix(); // push so we work with new "copy" matrix
                glLoadIdentity(); // identity
                gluOrtho2D(0.0, gs.WindowWidth, 0.0, gs.WindowHeight); // work within window
                    //add score drawing with bitmap
                    char scoreAsString[32];
                    int a = sprintf(scoreAsString,"Crushed: %d",gs.numberOfCrushes);
                    if(a < 0)
                        exit(1);
                    //set the position of the text in the window using the x and y coordinates
                    glRasterPos2i(gs.WindowWidth-(13*12),gs.WindowHeight-30); // 13*12 is:13 letters if number of crushed vehicles is > 1000, 12 is size of ~chars
                    //get the length of the string to display
                    int len = (int) strlen(scoreAsString);
                    //loop to display character by character
                    for (int i = 0; i < len; i++){
                        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,scoreAsString[i]);
                    }
                glMatrixMode(GL_PROJECTION); // put Projection matrix back in
            glPopMatrix(); // Pop 2nd copy matrix
        glMatrixMode(GL_MODELVIEW); // Put Model back in
    glPopMatrix(); // Pop 1st copy matrix
    glutPostRedisplay(); // print all on screen
}
void drawEndGame(){
    glColor3f(1, 0 ,0);
    glMatrixMode(GL_PROJECTION); // Take current project matrix
    glPushMatrix();  // Push so we work with new "copy" matrix
        glLoadIdentity(); // Identity
        glMatrixMode(GL_MODELVIEW); // Add modelMatrix to it
            glPushMatrix(); // push so we work with new "copy" matrix
                glLoadIdentity(); // identity
                gluOrtho2D(0.0, gs.WindowWidth, 0.0, gs.WindowHeight); // work within window
                    //add score drawing with bitmap
                    char gameOver[11] = "Game over!";
                    //set the position of the text in the window using the x and y coordinates
                    glRasterPos2i(gs.WindowWidth/2 - 60,gs.WindowHeight/2);
                    //get the length of the string to display
                    int len = (int)strlen(gameOver);
                    //loop to display character by character
                    for (int i = 0; i < len; i++){
                        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOver[i]);
                    }
                glMatrixMode(GL_PROJECTION); // put Projection matrix back in
            glPopMatrix(); // Pop 2nd copy matrix
        glMatrixMode(GL_MODELVIEW); // Put Model back in
    glPopMatrix(); // Pop 1st copy matrix
    glutPostRedisplay(); // print all on screen
}
void drawCubeTank(struct Tank tank){
    glPushMatrix();
        // Start matrix at the position of the tank
        glTranslatef(tank.tankPosition.x, tank.tankPosition.y, tank.tankPosition.z);
        // Save the tank-scale, we dont want that to apply to the turret and gun
        glPushMatrix();
            glScalef(tank.tankScale.x, tank.tankScale.y, tank.tankScale.z);
            drawSquare();
        glPopMatrix();
        // Move the matrix up by the tank height, so we get the turret ontop of the tank
        glTranslatef(0, tank.tankScale.y, 0);
        // This will rotate the turret and gun
        glRotatef(gs.tankMainPlayer.turretRotate.x, 0, 1, 0);
        struct Vector3f turretSize;
        turretSize.x = (float)(tank.tankScale.x / 1.2);
        turretSize.y = (float)(tank.tankScale.y / 1.2);
        turretSize.z = (float)(tank.tankScale.z / 1.2);
        // Save the turret-scale, we dont want that to apply to the gun
        glPushMatrix();
            glScalef(turretSize.x, turretSize.y, turretSize.z);
            drawSquare();
        glPopMatrix();

        // Dont really need to save matrix here, since this is the last transformations we do anyway
        glPushMatrix();
            float barrelLenght = 1.5;

            // Start by moving the barrel origin to the edge of the turret
            float barrelZPosition = (float)(turretSize.z * 0.5);
            // Then move the barrel by half its lenght, so that we push the rest of the barrel out of the turret
            barrelZPosition += barrelLenght * 0.5;
            
            // Move the barrel UP by half the turret-size, so that its centered on the turret
            float barrelYPosition = (float)(turretSize.y * 0.5);

            glTranslatef(0, barrelYPosition, -barrelZPosition);
            glScalef(0.2, 0.2, barrelLenght);
            glutSolidSphere(1, 20, 20);
        glPopMatrix();
    glPopMatrix();
}
void drawBullet(){  
    glPushMatrix();
        glTranslatef(gs.bullet.bulletPosition.x, gs.bullet.bulletPosition.y, gs.bullet.bulletPosition.z);
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(.3, .3, .3);
        glutSolidSphere(0.3, 10, 10);
        glDisable(GL_LIGHT1);
        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHT0);
    glPopMatrix();
}
void drawCar(struct Car cars){
    glPushMatrix();
    glTranslatef(cars.carPosition.x, cars.carPosition.y, cars.carPosition.z); // use same Translate for Car and for Shield
    glPushMatrix();
        glScalef(cars.carScale.x, cars.carScale.y, cars.carScale.z);
        glRotatef(cars.carRotate.x, 1, 0, 0);
        glRotatef(cars.carRotate.y, 0, 1, 0);
        glRotatef(cars.carRotate.z, 0, 0, 1);
        drawSquare();
    glPopMatrix();
    if(cars.showShield == 1){
        glPushMatrix();
                glEnable(GL_BLEND);
                glEnable(GL_COLOR_MATERIAL);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(1, 0, 0, cars.shieldOpacity);
                glutSolidSphere(cars.carScale.y + 0.3, 10, 10); // Spawns shield around car.
                glDisable(GL_BLEND);
                glDisable(GL_COLOR_MATERIAL);
        glPopMatrix();
    }
    glPopMatrix();
}
void drawSideRoad(struct Road road){
    glPushMatrix();
        glTranslatef(road.roadPosition.x, road.roadPosition.y, road.roadPosition.z);
        glScalef(road.roadScale.x, road.roadScale.y, road.roadScale.z);
        glRotatef(road.roadRotation.x, 1, 0, 0);
        glRotatef(road.roadRotation.y, 0, 1, 0);
        glRotatef(road.roadRotation.z, 0, 0, 1);
        
        glLineWidth(1);
        struct Vector3f v3f = {1, 1, 1};
        glBindTexture(GL_TEXTURE_2D, names[0]);
        setVertexColor(1,1,1);
        glNormal3f(0, 1, 0);
        glBegin(GL_QUADS);
            glTexCoord2f(-1, -1);
            glVertex3f(-v3f.x,-v3f.y, v3f.z);//bottom left
            glTexCoord2f(1, -1);
            glVertex3f(v3f.x, -v3f.y, v3f.z);//bottom right
            glTexCoord2f(1, 1);
            glVertex3f(v3f.x, v3f.y, v3f.z);//top right
            glTexCoord2f(-1, 1);
            glVertex3f(-v3f.x, v3f.y, v3f.z);//top left
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}




