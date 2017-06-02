# Programming masterclass
Files for the robotics masterclass about clean code

For the clean code assignment I decided to make an algorithm in C that calculates the shortest paths for a swarm of robots to travel to reach all the given destinations.

My first version of this algorithm was a relatively simple algorithm for 11 robots (one for each group member) but it had quite some flaws. It only looked to distance and not time.

So in version 2 I changed the robot and destination mapping to linear linked lists, while also taking the speed and passed time of robots into account.

This results in the second version where all the data is inputted from the data file (as project argument). Also the two headers "robot.h" and "destination.h" are used for version 2 of the algorithm. The second algorithm is a lot better, however still not perfect, but improving it would require much more processing power, while the current algorithm is very fast and could be done by the robots themselves.