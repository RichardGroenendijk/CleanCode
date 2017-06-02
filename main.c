#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "robot.h"
#include "destination.h"

#define MAX_NAME_LENGTH 20
#define SQUARE(x) ((x) * (x))

static void *checkPointer (void *pointer)
{
    if (pointer == NULL)
    {
        printf ("Error: out of memory!\n");
        exit (1);
    }
    return pointer;
}

void *safeMalloc (size_t size)
{
    return checkPointer (malloc (size));
}

char *safeStringDuplicate (const char *string)
{
    return strcpy (safeMalloc (strlen (string) + 1), string);
}

static void checkInputError (double input)
{
    if (input <= 0)
    {
        printf ("Error: negative or zero input");
        exit (EXIT_FAILURE);
    }
}

robotData *newRobot (char *name, double location_x, double location_y, double speed)
{
    robotData *newRobot = safeMalloc (sizeof(robotData));

    newRobot -> name       = name;
    newRobot -> location_x = location_x;
    newRobot -> location_y = location_y;
    newRobot -> time       = 0;
    newRobot -> speed      = speed;
    newRobot -> next       = NULL;

    return newRobot;
}

static robotData *addRobotToMap (FILE *inputData, robotData *map)
{
    char robotName[MAX_NAME_LENGTH+1];
    double location_x, location_y, speed;
    robotData *robot, *finder;

    fscanf (inputData, "%s", robotName);
    fscanf (inputData, "%lf%lf%lf", &location_x, &location_y, &speed);
    checkInputError(speed);

    robot = newRobot (safeStringDuplicate (robotName), location_x, location_y, speed);

    if (map == NULL)
    {
        map = robot;
    }
    else
    {
        finder = map;
        while (finder -> next)
        {
            finder = finder -> next;
        }
        finder -> next = robot;
    }

    return map;
}

static robotData *createRobotMap (FILE *inputData)
{
    int amountOfRobots;
    int currentRobot;

    robotData *map = NULL;

    fscanf (inputData, "%d", &amountOfRobots);

    for (currentRobot = 0; currentRobot < amountOfRobots; currentRobot++)
        map = addRobotToMap (inputData, map);

    return map;
}

destinationData *newDestination (double location_x, double location_y)
{
    destinationData *newDestination = safeMalloc (sizeof(destinationData));

    newDestination -> location_x   = location_x;
    newDestination -> location_y   = location_y;
    newDestination -> next         = NULL;

    return newDestination;
}

static destinationData *addDestinationToMap (FILE *inputData, destinationData *map)
{
    double location_x, location_y;
    destinationData *destination, *finder;

    fscanf (inputData, "%lf%lf", &location_x, &location_y);

    destination = newDestination (location_x, location_y);

    if (map == NULL)
    {
        map = destination;
    }
    else
    {
        finder = map;
        while (finder -> next)
        {
            finder = finder -> next;
        }
        finder -> next = destination;
    }

    return map;
}

static destinationData *createdDestinationMap (FILE *inputData)
{
    int amountOfDestinations;
    int currentDestination;

    destinationData *map = NULL;

    fscanf (inputData, "%d", &amountOfDestinations);

    for (currentDestination = 0; currentDestination < amountOfDestinations; currentDestination++)
        map = addDestinationToMap (inputData, map);

    return map;
}

double calculateTimePassed (robotData *robot, destinationData *destination)
{
    double  robot_x = robot -> location_x,
            robot_y = robot -> location_y,
            speed   = robot -> speed,
            time    = robot -> time,
            destination_x = destination -> location_x,
            destination_y = destination -> location_y;

    time += (sqrt (SQUARE (destination_x - robot_x) + SQUARE (destination_y - robot_y)))/speed;

    return time;
}

static robotData *firstAvailableRobot (robotData *robotMap)
{
    robotData *firstAvailableRobot = robotMap;
    double shortestTime = robotMap -> time;

    while (robotMap)
    {
        if (robotMap -> time < shortestTime)
        {
            shortestTime = robotMap -> time;
            firstAvailableRobot = robotMap;
        }
        robotMap = robotMap -> next;
    }

    return firstAvailableRobot;
}

static void printMovement (robotData *robot)
{
    printf ("\n%s moves to location (%.2f, %.2f) and arrives at %.2f\n", robot -> name, robot -> location_x, robot -> location_y, robot -> time);
}

destinationData *calculateClosestDestination (robotData *robotMap, destinationData *destinationMap)
{
    robotData *robot = firstAvailableRobot (robotMap);
    destinationData *closestDestination = destinationMap, *previousDestination = NULL, *finder = destinationMap;
    double timePassed, shortestTimePassed = robot -> time + calculateTimePassed (robot, closestDestination);

    while (finder -> next)
    {
        timePassed = robot -> time + calculateTimePassed (robot, finder -> next);
        if (timePassed < shortestTimePassed)
        {
            shortestTimePassed = timePassed;
            closestDestination = finder -> next;
            previousDestination = finder;
        }
        finder = finder -> next;
    }

    robot -> location_x = closestDestination -> location_x;
    robot -> location_y = closestDestination -> location_y;
    robot -> time       = shortestTimePassed;

    printMovement (robot);

    if (previousDestination == NULL)
        destinationMap = destinationMap -> next;
    else
        previousDestination -> next = previousDestination -> next -> next;
    free (closestDestination);

    return destinationMap;
}

static void calculateShortestPaths (robotData *robotMap, destinationData *destinationMap)
{
    while (destinationMap)
    {
        destinationMap = calculateClosestDestination (robotMap, destinationMap);
    }
}

int main (int argc, char *argv[])
{
    FILE *inputData = NULL;
    robotData *robotMap = NULL;
    destinationData *destinationMap = NULL;

    /* Check project arguments */
    if (argc != 2)
    {
        fprintf (stderr, "Usage: %s <datafile>\n", argv[0]);
        exit (EXIT_FAILURE);
    }

    /* Open file */
    if ((inputData = fopen (argv[1], "r")) == NULL)
    {
        fprintf (stderr, "Error opening file <%s>\n", argv[1]);
        exit (EXIT_FAILURE);
    }

    robotMap = createRobotMap (inputData);
    destinationMap = createdDestinationMap (inputData);

    calculateShortestPaths (robotMap, destinationMap);

    /* Close file */
    if (inputData)
        fclose (inputData);

    return EXIT_SUCCESS;
}
