#include <GL/glut.h>
#include <GL/gl.h>
//idea right now is to have car, tank, road..

void light(){

    //Light coeffs
    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 0.1};
    GLfloat light_diffuse[] = {0.0, 0.0, 0.0, 1};
    GLfloat light_specular[] = {1, 1, 1, 1};
    //GLfloat model_ambient[] = {1, 1, 1, 1};

    // Light parameters
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
void material()
{
    // Coeffs for material
    GLfloat ambient_coeffs[] = {0.3, 0.3, 0.3, 1};
    GLfloat diffuse_coeffs[] = {0.3, 0.3, 0.3, 1};
    GLfloat specular_coeffs[] = {1, 1, 1, 1};
    //GLfloat emission_coeffs[] = {0.1, 0.1, 0.1, 1};
    GLfloat shininess = 2;
    // Material parameters.
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    //glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
}

void setVertexColor(float R, float G, float B)
{
    GLfloat diffuse[] = {R, G, B};
    GLfloat ambient[] = {R, G, B};
    GLfloat specular[] = {R, G, B};
    GLfloat shininess = 100;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
