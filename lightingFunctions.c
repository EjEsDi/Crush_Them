#include <GL/glut.h>
#include <GL/gl.h>
//idea right now is to have car, tank, road..

void light(){

    //Light coeffs
    GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1};
    GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};
    GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};
    GLfloat model_ambient[] = {1, 1, 1, 1};

    // Light parameters
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

}

void material(){
    // Coeffs for material
    GLfloat ambient_coeffs[] = {0.5, 0.1, 0.1, 1};
    GLfloat diffuse_coeffs[] = {0.0, 0.0, 0.8, 1};
    GLfloat specular_coeffs[] = {1, 1, 1, 1};
    GLfloat emission_coeffs[] = {0.7, 0.7, 0.7, 1};
    GLfloat shininess = 20;
    // Material parameters.
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
}