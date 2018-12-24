//
// Created by aleksandar on 24.12.18..
//

#include "mathFunctions.h"
#include "drawFunctions.h"
#include <stdbool.h>
#include <GL/glut.h>
#include <math.h>

bool collisionCheck(struct Vector3f a, struct Vector3f b, struct Vector3f aSize, struct Vector3f bSize){
    // Collision x-axis
    bool collisionX = a.x + aSize.x >= b.x && b.x + bSize.x >= a.x;
    // Collision y-axis
    bool collisionY = a.y + aSize.y >= b.y && b.y + bSize.y >= a.y;
    //Collision z-axis
    bool collisionZ = a.z + aSize.z >= b.z && b.z + bSize.z >= a.z;
    // Collision only if on all three axes

    return collisionX && collisionY && collisionZ;
}

struct Vector3f getDirection(struct Vector3f a, struct Vector3f b)
{
    struct Vector3f result = b;
    result.x -= a.x;
    result.y -= a.y;
    result.z -= a.z;
    return normalize(result);
}

struct Vector3f normalize(struct Vector3f a){
    float len = (float)(sqrt((a.x*a.x)+(a.y*a.y)+(a.z+a.z)));
    struct Vector3f result;
    if(len == 0){
        result.x = 0;
        result.y = 0;
        result.z = 0;
    }
    result.x = a.x / len;
    result.y = a.y / len;
    result.z = a.z / len;
    return result;
}

void setTankTurretMatrix(void)
{
    // Start matrix at the position of the tank
    glTranslatef(gs.tankMainPlayer.tankPosition.x, gs.tankMainPlayer.tankPosition.y, gs.tankMainPlayer.tankPosition.z);

    // Move the matrix up by the tank height, so we get the turret ontop of the tank
    glTranslatef(0, gs.tankMainPlayer.tankScale.y, 0);

    // This will rotate the turret and gun
    glRotatef(gs.tankMainPlayer.turretRotate.x, 0, 1, 0);
    struct Vector3f turretSize;
    turretSize.x = (float)(gs.tankMainPlayer.tankScale.x / 1.2f);
    turretSize.y = (float)(gs.tankMainPlayer.tankScale.y / 1.2f);
    turretSize.z = (float)(gs.tankMainPlayer.tankScale.z / 1.2f);

    float barrelLength = 1.5f;

    // Start by moving the barrel origin to the edge of the turret
    float barrelZPosition = (float)(turretSize.z * 0.5f);
    // Then move the barrel by half its lenght, so that we push the rest of the barrel out of the turret
    barrelZPosition += barrelLength * 0.5f;

    // Move the barrel UP by half the turret-size, so that its centered on the turret
    float barrelYPosition = (float)(turretSize.y * 0.5f);
    glTranslatef(0, barrelYPosition, -barrelZPosition);
}