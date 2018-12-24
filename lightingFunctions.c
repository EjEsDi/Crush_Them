#include <GL/glut.h>
#include <GL/gl.h>
#include "lightingFunctions.h"
#include "mathFunctions.h"

void light(){
    //Light coeffs

    glPushMatrix();
        glLoadIdentity();
        glRotatef(gs.sun.sunRotate.x, 1, 0, 0);
        glRotatef(gs.sun.sunRotate.y, 0, 1, 0);
        glRotatef(gs.sun.sunRotate.z, 0, 0, 1);
        glTranslatef(gs.sun.sunTranslate.x, gs.sun.sunTranslate.y, gs.sun.sunTranslate.z + gs.tankMainPlayer.tankPosition.z);
        GLfloat sunPositionMatrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, sunPositionMatrix);
    glPopMatrix();

    gs.sun.sunPosition.x = sunPositionMatrix[12];
    gs.sun.sunPosition.y = sunPositionMatrix[13];
    gs.sun.sunPosition.z = sunPositionMatrix[14];

    struct Vector3f direction = getDirection(gs.tankMainPlayer.tankPosition, gs.sun.sunPosition);
    gs.sun.lightDirection.x = direction.x;
    gs.sun.lightDirection.y = direction.y;
    gs.sun.lightDirection.z = direction.z;
    GLfloat light_position[] = {gs.sun.lightDirection.x,gs.sun.lightDirection.y, gs.sun.lightDirection.z, 0};
    GLfloat light_ambient[] = {.3f, .3f, .3f, 1};
    GLfloat light_diffuse[] = {.8f, .8f, .8f, 1};
    GLfloat light_specular[] = {1, 1, 1, 1};
    
    // Light parameters
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
void lightForSun(){
    //Light coeffs
    GLfloat light_position[] = {-gs.sun.lightCoef.x, 0, gs.sun.lightCoef.z, 0};
    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1};
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1};

    // Light parameters

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
}
void setVertexColor(float R, float G, float B){
    GLfloat diffuse[] = {R + gs.lightModifier, G + gs.lightModifier, B + gs.lightModifier, 1};
    GLfloat ambient[] = {R - gs.lightModifier, G - gs.lightModifier, B - gs.lightModifier, 1};
    GLfloat specular[] = {0, 0, 0, 1};
    GLfloat shininess = 20;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
