#ifndef DESTINATION_H_INCLUDED
#define DESTINATION_H_INCLUDED

/* Structure for the destinations of version 2 of the robot swarm algorithm */

typedef struct destination destinationData;

struct destination
{
    double              location_x, location_y;
    destinationData    *next;
};

#endif /* DESTINATION_H_INCLUDED */

