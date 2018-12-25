#ifndef DRAWFUNCTIONS_H
#define DRAWFUNCTIONS_H
#include <GL/gl.h>
#include <stdbool.h>

/************************************
    Macro definitions start here
*************************************/

#define MAX_CARS_ALLOWED 10
#define NOT_USED_VAR(X) (void)(X)


//Create game state.
struct gameState gs;
GLuint names[1]; // used for texture

/************************************
    Structs declarations start here
*************************************/

struct Vector3f{
    float x;
    float y;
    float z;
};
struct Car{
    float carSpeed;
    int setOfCarXPositionsAllowedValues[3];
    int ZSpawnPoint;
    int numOfCars;
    int timeCarSpawn;
    struct Vector3f carScale;
    struct Vector3f carPosition;
    struct Vector3f carRotate;
    int showShield;
    float shieldOpacity;
    long long lastZPoint;
};
struct Tank{
    struct Vector3f tankPosition;
    struct Vector3f tankScale;
    struct Vector3f turretRotate; //can be moved to turret struct if it gets made
    float tankSpeed;
    int prevDir;
    int currDir;
    bool shoot;
};
struct Road{
    struct Vector3f roadScale;
    struct Vector3f roadRotation;
    struct Vector3f roadPosition;
};
struct Sky{
    struct Vector3f skyColor;
    int dayTimer;
    int flag;
};
struct Sun{
    struct Vector3f sunRotate;
    struct Vector3f sunTranslate; // its initializing suns position
    struct Vector3f sunPosition; // its used in lighting function, after calculating direction
    struct Vector3f lightDirection;
};
struct Bullet{
    struct Vector3f bulletPosition;
    struct Vector3f bulletDirection;
    struct Vector3f scale;
    bool needToResetBullet;
    int Charging;
};
struct gameState{
    struct Car carArray[MAX_CARS_ALLOWED];
    struct Car car;
    struct Sun sun;
    struct Road road, road2, road3;
    struct Sky sky;
    struct Road rightSideRoad, rightSideRoad2, rightSideRoad3;
    struct Road leftSideRoad, leftSideRoad2, leftSideRoad3;
    struct Tank tankMainPlayer;
    struct Bullet bullet;
    int WindowWidth;
    int WindowHeight;
    int actionOnGoing;
    float cameraMovement;
    int numberOfCrushes;
    int lastMouseX;
    bool gameover;
};


/************************************
    Functionss declarations start here
*************************************/

void drawBullet(void);
void drawSquare(void);
void drawRoad(struct Road road);
void drawSideRoad(struct Road road);
void drawCubeTank(struct Tank tank);
void drawCar(struct Car cars);
void drawSun(void);
void drawScore(void);
void drawEndGame(void);
void skyChangeFunction(void);

#endif
