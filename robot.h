#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

/* Structure for the robots of version 2 of the robot swarm algorithm */

typedef struct robot robotData;

struct robot
{
    char        *name;
    double      location_x, location_y;
    double      time, speed;
    robotData  *next;
};

#endif /* ROBOT_H_INCLUDED */

