#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

typedef struct robot robotData;

struct robot
{
    char        *name;
    double      location_x, location_y;
    double      time, speed;
    robotData  *next;
};

#endif /* ROBOT_H_INCLUDED */

