#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Square(x) ((x) * (x))
#define TOTAL_NUMB_ROBOTS 11

/* This is the data structure that gives for each robot a location on the x- and y-axis */
struct robot_location
{
    double x_axis, y_axis;
};

typedef struct
{
    /* Place to add more variables to this structure */
    struct robot_location   location;
} robot_data;

/* Declaration of global variables */
robot_data robot[TOTAL_NUMB_ROBOTS];
double **destinations_coords;
int total_numb_destinations;

void input ()
{
    int robot_numb, destination_numb;

    /* Input of the starting location of all the robots */
    printf ("Give for all %d robots the starting location coordinates:\n", TOTAL_NUMB_ROBOTS);
    for (robot_numb = 0; robot_numb < TOTAL_NUMB_ROBOTS; robot_numb++)
    {
        scanf ("%lf%lf", &robot[robot_numb].location.x_axis, &robot[robot_numb].location.y_axis);
    }

    /* Input of the amount of destinations where the robots need to travel to */
    printf ("\nGive the amount of destinations: ");
    scanf ("%d", &total_numb_destinations);
    destinations_coords = calloc (total_numb_destinations, sizeof(double));

    /*  Input of those coördinates in a matrix:
        1st column: x-axis of the destination
        2nd column: y-axis of the destination
        3rd column: whether or not the destination has been visited yet */
    printf ("\nGive for %d destinations the coordinates:\n", total_numb_destinations);
    for (destination_numb = 0; destination_numb < total_numb_destinations; destination_numb++)
    {
        destinations_coords[destination_numb] = calloc(3, sizeof(double));
        scanf ("%lf%lf", &destinations_coords[destination_numb][0], &destinations_coords[destination_numb][1]);
    }
}

double calc_distance (int robot_numb, int destination_numb)
{
    double distance, diff_x_axis, diff_y_axis;

    diff_x_axis = robot[robot_numb].location.x_axis - destinations_coords[destination_numb][0];
    diff_y_axis = robot[robot_numb].location.y_axis - destinations_coords[destination_numb][1];
    distance = sqrt (Square(diff_x_axis) + Square(diff_y_axis));

    return distance;
}

/* Output of the program and setting the location variables of the robot to its new location */
void print_movement (int robot_numb, int target_destination, double minimum_distance)
{
    robot[robot_numb].location.x_axis = destinations_coords[target_destination][0];
    robot[robot_numb].location.y_axis = destinations_coords[target_destination][1];
    destinations_coords[target_destination][2] = 1; /* Makes it visible that this destination has been visited */

    printf ("Robot %d moves a distance of %.2f to location (%.2f, %.2f)\n", robot_numb+1, minimum_distance,
                robot[robot_numb].location.x_axis, robot[robot_numb].location.y_axis);
}

/* Function that calculates for each robot the closest destination */
void shortest_paths (int round)
{
    int robot_numb, destination_numb, target_destination;
    double minimum_distance, distance;

    printf ("\nRound %d\n", round);

    for (robot_numb = 0; robot_numb < TOTAL_NUMB_ROBOTS; robot_numb++)
    {
        minimum_distance = 1E100;
        target_destination = -1;

        for (destination_numb = 0; destination_numb < total_numb_destinations; destination_numb++)
        {
            if (destinations_coords[destination_numb][2] != 1)
            {
                distance = calc_distance (robot_numb, destination_numb);

                if(distance < minimum_distance)
                {
                    minimum_distance = distance;
                    target_destination = destination_numb;
                }
            }
        }

        if (target_destination < 0)
        {
            /* This only occurs when there are no destinations left that are still unvisited */
            printf ("Robot %d does not move\n", robot_numb+1);
        }
        else
        {
            print_movement (robot_numb, target_destination, minimum_distance);
        }
    }
}

int main()
{
    int round = 0;

    input();

    /* You only have to run the shortest_paths function until all destinations are reached */
    while (TOTAL_NUMB_ROBOTS*round < total_numb_destinations)
    {
        shortest_paths (round+1);
        round++;
    }

    return 0;
}
