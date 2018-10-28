#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CARS_ALLOWED 512

//struct for rotation, scale, translate
struct Vector3f{
    float x;
    float y;
    float z;
};

//structure instead global variables
struct Car{
    struct Vector3f carPosition;
    float carSpeed;
};

struct Tank{
    float x;
    float y;
    float z;
};

struct gameState{
    struct Car carNumber[MAX_CARS_ALLOWED];
    struct Tank tankMainPlayer;
    int WindowWidth;
    int WindowHeight;
    int yi;
    int actionOnGoing;
};

struct gameState gs;
struct Vector3f roadScale;
struct Vector3f roadRotation;
struct Vector3f roadTranslation;

// callback functions
void onDisplay(void);
void onReshape(int w, int h);
void onKeyboardInput(unsigned char key, int x, int y);
void onTimer(int timer);

void init(void);
void initRenderingObjects(struct Vector3f *aScale, struct Vector3f *aRotation, struct Vector3f *aTranslation,struct gameState *gs);
void drawRoad(const struct Vector3f aScale,const struct Vector3f aRotation,const struct Vector3f aTranslation);
void drawTank(const struct Tank tank);
void drawCar(const struct Car cars);

#define NOT_USED_VAR(X) (void)(X)
// timer callback func variablesw
#define timerID (0)
#define timeInMS (500)

//end of timer callback func variables

int main(int argc, char** argv){
    
    //initialization of glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    //initialize Window
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(2000, 100); // edit this to width/2, height/2
    glutCreateWindow("Crush them!");

    //callback functions
    glutDisplayFunc(onDisplay);
    glutReshapeFunc(onReshape);
    glutKeyboardFunc(onKeyboardInput);

    // initialize stuff
    init();

    // start program
    glutMainLoop(); 
    return 0;
}
void init(void){
    glClearColor(0.0, 0.0 , 0.0, 0.0);
    glClearDepth(1.0);
    glLineWidth(1);
    glShadeModel(GL_FLAT);
}

void drawRoad(const struct Vector3f aScale,const struct Vector3f aRotation,const struct Vector3f aTranslation){
    
    glPushMatrix();
        glEnable(GL_DEPTH_TEST);
        glScalef(aScale.x, aScale.y, aScale.z);
        glRotatef(aRotation.x, 1, 0, 0);
        glRotatef(aRotation.y, 0, 1, 0);
        glRotatef(aRotation.z, 0, 0, 1);
        glTranslatef(aTranslation.x, aTranslation.y, aTranslation.z);
        glLineWidth(1);

        GLfloat width = 1;
        GLfloat height = 1;
        GLfloat depth = 1;

        glBegin(GL_QUADS);
            glColor3f(1.0, 0, 0);
            glVertex3f(-width,-height, depth);//bottom left
            glColor3f(1.0, 1.0, 0);
            glVertex3f(width, -height, depth);//bottom right
            glColor3f(1.0, 0, 1.0);
            glVertex3f(width, height, depth);//top right
            glColor3f(0, 1.0, 1.0);
            glVertex3f(-width, height, depth);//top left
        glEnd();
        //draw lanes on road
        
        glEnable(GL_LINE_STIPPLE);
        glLineStipple (3, 0xF00F); // dashed lines
        glLineWidth(2);
        glBegin(GL_LINES);
                glColor3f(1, 1, 1);
                //left line
                glVertex3f(-width/3, -height, depth/2); 
                glVertex3f(-width/3, height, depth/2);
                //right line
                glVertex3f(width/3, -height, depth/2); 
                glVertex3f(width/3, height, depth/2);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        
        glDisable(GL_DEPTH_TEST);
    glPopMatrix();
}

void drawCubeTank(const struct Tank tank){
    NOT_USED_VAR(tank);
    //For now I don't actually have Tank tank struct
    //All I did was just initialize drawing
    //TODO: Decide do I need tank as an struct or no 
    glPushMatrix();
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable( GL_DEPTH_TEST );
        glScalef(1, 1, 3);
        glTranslatef(0.0, 0.0, +2);
        glBegin(GL_QUADS);
            /* blue - green - red - light blue - */

            //front and back
            //blue - front
            glColor3f(0, 0, 1);
            glVertex3f(-0.5, -0.5, -0.5);
            glVertex3f(-0.5, 0.5, -0.5);
            glVertex3f(0.5, 0.5, -0.5);
            glVertex3f(0.5, -0.5, -0.5); 

            //green - back
            glColor3f(0, 1, 0);
            glVertex3f(-0.5, -0.5, +0.5);
            glVertex3f(-0.5, 0.5, +0.5);
            glVertex3f(0.5, 0.5, 0.5);
            glVertex3f(0.5, -0.5, 0.5); 
            
            //sides
            // dark green - right
            glColor3f(0, 1, 1);
            glVertex3f(0.5, -0.5, -0.5);
            glVertex3f(0.5, 0.5, -0.5);
            glVertex3f(0.5, 0.5, +0.5);
            glVertex3f(0.5, -0.5, +0.5); 

            //lblue - left
            glColor3f(0, 1, 1);
            glVertex3f(-0.5, -0.5, -0.5);
            glVertex3f(-0.5, 0.5, -0.5);
            glVertex3f(-0.5, 0.5, +0.5);
            glVertex3f(-0.5, -0.5, +0.5); 

            // top and bottom
            //white
            glColor3f(1, 1, 1);
            glVertex3f(-0.5, -0.5, +0.5);
            glVertex3f(+0.5, -0.5, +0.5);
            glVertex3f(+0.5, -0.5, -0.5);
            glVertex3f(-0.5, -0.5, -0.5);

            //yellow
            glColor3f(1, 1, 0);
            glVertex3f(0.5, +0.5, +0.5);
            glVertex3f(-0.5, +0.5, +0.5);
            glVertex3f(-0.5, +0.5, -0.5);
            glVertex3f(0.5, +0.5, -0.5);
        glDisable( GL_DEPTH_TEST );
        glEnd();
        
    glPopMatrix();
}
void drawCar(const struct Car car){
    
    glPushMatrix();
        //TODO check glRotate, when you want it to rotate?
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glScalef(1, 1, 1);
        //glRotatef(aRotation.x, 1, 0, 0);
        //glRotatef(aRotation.y, 0, 1, 0);
        //glRotatef(aRotation.z, 0, 0, 1);
        glTranslatef(car.carPosition.x, car.carPosition.y, car.carPosition.z);

        glBegin(GL_QUADS);
            /* blue - green - red - light blue - */

            //front and back
            //blue - front
            glColor3f(0, 0, 1);
            glVertex3f(-0.5, -0.5, -0.5);
            glVertex3f(-0.5, 0.5, -0.5);
            glVertex3f(0.5, 0.5, -0.5);
            glVertex3f(0.5, -0.5, -0.5); 

            //purple - back
            glColor3f(1, 0, 1);
            glVertex3f(-0.5, -0.5, +0.5);
            glVertex3f(-0.5, 0.5, +0.5);
            glVertex3f(0.5, 0.5, 0.5);
            glVertex3f(0.5, -0.5, 0.5); 
            
            //sides
            //red+blue - right
            glColor3f(1, 0, 1);
            glVertex3f(0.5, -0.5, -0.5);
            glVertex3f(0.5, 0.5, -0.5);
            glVertex3f(0.5, 0.5, +0.5);
            glVertex3f(0.5, -0.5, +0.5); 

            //lblue - left
            glColor3f(0, 1, 1);
            glVertex3f(-0.5, -0.5, -0.5);
            glVertex3f(-0.5, 0.5, -0.5);
            glVertex3f(-0.5, 0.5, +0.5);
            glVertex3f(-0.5, -0.5, +0.5); 

            // top and bottom
            //white
            glColor3f(1, 1, 1);
            glVertex3f(-0.5, -0.5, +0.5);
            glVertex3f(+0.5, -0.5, +0.5);
            glVertex3f(+0.5, -0.5, -0.5);
            glVertex3f(-0.5, -0.5, -0.5);

            //yellow
            glColor3f(1, 1, 0);
            glVertex3f(0.5, +0.5, +0.5);
            glVertex3f(-0.5, +0.5, +0.5);
            glVertex3f(-0.5, +0.5, -0.5);
            glVertex3f(0.5, +0.5, -0.5);
        glDisable( GL_DEPTH_TEST );
        glEnd();
    glPopMatrix();
    glFlush();
}

void initRenderingObjects(struct Vector3f *aScale, struct Vector3f *aRotation, struct Vector3f *aTranslation,struct gameState *gamestate){
    //Should I put Scale,Rotation and Trasnaltion in init() function?
    //Road should be initialized on very start, should I mix it with those funcs or keep here?
    //Road should be initialized once only, do I need to keep checking on it? Future versions?
    //TODO rename arguments
    //road Scalef
    aScale->x = 6;
    aScale->y = 1;
    aScale->z = 500;
    //roadRotation
    aRotation->x = 90;
    aRotation->y = 0;
    aRotation->z = 0;
    //roadTranslation
    aTranslation->x = 0;
    aTranslation->y = 0;
    aTranslation->z = 0;
    //TODO make use of tankMainPlayer?
    //Initialize tank and put him in spot for game play
    gamestate->tankMainPlayer.x = 1;
    gamestate->tankMainPlayer.y = 0;
    gamestate->tankMainPlayer.z = 8;
    //TODO Fix gameplay here
    //Initialize cars, should be reedited in future.
    int min_distance = -100;
    for(int i = 0; i < 15; i++){
        min_distance += 10;
        if(i%2 == 0)
            gamestate->carNumber[i].carPosition.x = (rand() % 4);
        else
            gamestate->carNumber[i].carPosition.x = -(rand() % 4);
        gamestate->carNumber[i].carPosition.y = 0;
        gamestate->carNumber[i].carPosition.z = min_distance;
        if (min_distance == -10){
            min_distance = -95;
        }
    }
}
void onDisplay(void){
    //remove old content
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //setting camera position and where it looks at
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  0, 3, 10, // camera position
                0, 0, 2, // camera looks at this spot
                0, 1, 0  // normal vector 
            ); 
    
    //Rendering section
    initRenderingObjects(&roadScale, &roadRotation, &roadTranslation, &gs);
    drawRoad(roadScale, roadRotation, roadTranslation);
    drawCubeTank(gs.tankMainPlayer);
    for(int i = 0; i < 15; i++)
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
    gluPerspective(60, gs.WindowWidth/(GLfloat)gs.WindowHeight, 1.0, 1000.0); // angle, ratio, near clip, far clip
}

void onKeyboardInput(unsigned char key, int x, int y){
    NOT_USED_VAR(x);
    NOT_USED_VAR(y);
    //TODO a lot stuff here
    switch(key){
        case 27: // ESC BUTTON
            exit(0);
            break;
        case 'g': // GO
        case 'G':
            if(gs.actionOnGoing == 0){
                gs.actionOnGoing = 1;
                glutTimerFunc(timeInMS, onTimer, timerID);
            }
            break;
        case 's': // STOP
        case 'S':
            if(gs.actionOnGoing == 1){
                gs.actionOnGoing = 0;
            }
            break;
        // left
        case 'a': 
        case 'A': 
            break;
        // right
        case 'd':
        case 'D':
            break;
        case 'w':
        case 'W':
            break;
    }
    glutPostRedisplay();
}

void onTimer(int timer){
    //TODO a lot stuff here
    NOT_USED_VAR(timer);
    if(timerID == 0){
        // movement of road ID
    }
    glutPostRedisplay();
    glutTimerFunc(timeInMS, onTimer, timerID);
}
