#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "robot.h"
#include "destination.h"

#define MAX_NAME_LENGTH 20
#define SQUARE(x) ((x) * (x))

/*Function to check whether there is enough space left */
static void *checkPointer (void *pointer)
{
    if (pointer == NULL)
    {
        printf ("Error: out of memory!\n");
        exit (1);
    }
    return pointer;
}

/* Safely allocate memory by checking the pointer */
void *safeMalloc (size_t size)
{
    return checkPointer (malloc (size));
}

/* Safely duplicate a string by safe memory allocation */
char *safeStringDuplicate (const char *string)
{
    return strcpy (safeMalloc (strlen (string) + 1), string);
}

/* Check whether the input is negative or zero */
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

/*Function to add a robot to the map and return this map */
static robotData *addRobotToMap (FILE *inputData, robotData *map)
{
    char robotName[MAX_NAME_LENGTH+1];
    double location_x, location_y, speed;
    robotData *robot, *finder;

    /* Scan the file for the robot data */
    fscanf (inputData, "%s", robotName);
    fscanf (inputData, "%lf%lf%lf", &location_x, &location_y, &speed);
    checkInputError(speed);

    /* Add a new robot structure */
    robot = newRobot (safeStringDuplicate (robotName), location_x, location_y, speed);

    /* Add the robot as next in the list */
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

    /* Return the first robot as map */
    return map;
}

static robotData *createRobotMap (FILE *inputData)
{
    int amountOfRobots;
    int currentRobot;

    robotData *map = NULL;

    /* Scan the amount of robots */
    fscanf (inputData, "%d", &amountOfRobots);

    /* Scan for all robots the input data and add it to the list (map) */
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

/* Function to add a destination to the map and return this map */
static destinationData *addDestinationToMap (FILE *inputData, destinationData *map)
{
    double location_x, location_y;
    destinationData *destination, *finder;

    /* Input of the coordinates of the map */
    fscanf (inputData, "%lf%lf", &location_x, &location_y);

    destination = newDestination (location_x, location_y);

    /* Assign this destination as next in the list */
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

    /* Scan the amount of destinations */
    fscanf (inputData, "%d", &amountOfDestinations);

    /* For every destination take the input values and add them to the list */
    for (currentDestination = 0; currentDestination < amountOfDestinations; currentDestination++)
        map = addDestinationToMap (inputData, map);

    return map;
}

/* Function to calculate the new time value for a given robot and destination */
double calculateTimePassed (robotData *robot, destinationData *destination)
{
    double  newTime;

    double  robot_x = robot -> location_x,
            robot_y = robot -> location_y,
            speed   = robot -> speed,
            time    = robot -> time,
            destination_x = destination -> location_x,
            destination_y = destination -> location_y;

    /* Equation: time = distance / speed */
    newTime = time + (sqrt (SQUARE (destination_x - robot_x) + SQUARE (destination_y - robot_y)))/speed;

    return newTime;
}

/* Function to return the first available robot */
static robotData *firstAvailableRobot (robotData *robotMap)
{
    robotData *firstAvailableRobot = robotMap;
    double shortestTime = robotMap -> time;

    while (robotMap)
    {
        /* Check for all robots which is first available */
        if (robotMap -> time < shortestTime)
        {
            shortestTime = robotMap -> time;
            firstAvailableRobot = robotMap;
        }
        robotMap = robotMap -> next;
    }

    /* Return the pointer to this first available robot */
    return firstAvailableRobot;
}

static void printMovement (robotData *robot)
{
    /* The result that will be printed for each robot movement */
    printf ("\n%s moves to location (%.2f, %.2f) and arrives at %.2f\n", robot -> name, robot -> location_x, robot -> location_y, robot -> time);
}

/* Function that calculates the next closest destination for the first available robot */
destinationData *calculateClosestDestination (robotData *robotMap, destinationData *destinationMap)
{
    robotData *robot = firstAvailableRobot (robotMap);
    /* The first destination is input for the variables */
    destinationData *closestDestination = destinationMap, *previousDestination = NULL, *finder = destinationMap;
    double timePassed, shortestTimePassed = calculateTimePassed (robot, closestDestination);

    /* Find the closest destination for this robot */
    while (finder -> next)
    {
        timePassed = calculateTimePassed (robot, finder -> next);
        if (timePassed < shortestTimePassed)
        {
            /* Assign these values if this destination is closer */
            shortestTimePassed = timePassed;
            closestDestination = finder -> next;
            previousDestination = finder;
        }
        finder = finder -> next;
    }

    /* Assign the values to the robot for this given destination */
    robot -> location_x = closestDestination -> location_x;
    robot -> location_y = closestDestination -> location_y;
    robot -> time       = shortestTimePassed;

    /* Print the result */
    printMovement (robot);

    /* Delete the destination from the list */
    if (previousDestination == NULL)
        destinationMap = destinationMap -> next;
    else
        previousDestination -> next = previousDestination -> next -> next;
    free (closestDestination);

    return destinationMap;
}

/* Calculate the shortest paths while there are still destinations left */
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

    /* Create the robot and destination maps */
    robotMap = createRobotMap (inputData);
    destinationMap = createdDestinationMap (inputData);

    /* Calculate and output the shortest paths */
    calculateShortestPaths (robotMap, destinationMap);

    /* Close file */
    if (inputData)
        fclose (inputData);

    return EXIT_SUCCESS;
}
