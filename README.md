# Maze Solver

Maze Solver involves a robot agent with multiple range sensors that successfully navigates a maze to reach the exit. A static maze is constructed with the robot in an initial location on the left end. The robot makes its way through the maze solely by conditional forward/rotational movements based on the sensor values at any given instant.


## Approach

The approach used to navigate the static maze involves identifying openings in the general direction of the exit and moving appropriately. The wanderer is initialized as a state machine with the initial state "moving forward", which transitions to either of the following two states:

- Rotate (i.e. rotate away) if approaching an object in the general forward direction
- Rotate toward, if an an opening is identified in the general direction of the exit

## Key Challenges

The following were the key challenges faced in implementing this project:

- Identifying the appropriate number of realistic sensors, such that enough information was available without too much overhead. A generic sensor orientation was chosen (5 sensors in the W, NW, N, NE and E directions), which is similar to an actual physical robot that I programmed, equipped with sonar sensors, to do similar navigation
- Defining the "general direction of the exit" proved to be quite difficult, given that the orientation of the robot could be any angle. This was solved by implementing a conditional that checked the magnitude of the current robot angle and turning accordingly
- I initially tried to incorporate other components (a bitmap of the traversed regions, another user-manipulated robot that attempted to sabotage the maze solver), but was not able to get these to work. In the case of the bit map, adequately determining the current location and next steps proved more challenging than I'd initially planned, and I abandoned this approach. The user-manipulated robot caused the compiled code to seg fault for reasons I was unable to determine, so this was also abandoned.

## Installation

1. Clone this repository to using the following command: 
```bash
git clone https://github.com/nirajsuresh/Maze-Solver.git
```
2. Open a command window in the Maze-Solver folder and run the following docker command:
```bash
docker run -p80:80 -p8765:8765 -v %cd%:/source -it klavins/enviro:v1.6
```
## Execution
1. Open a web browser and navigate to localhost
2. To compile and run the code, run the following commands:
```bash
esm start
```
```bash
make
```
```bash
enviro
```
Maze Solver will now appear in the web browser.

## Acknowledgments
This project makes extensive use of the **enviro** library developed by klavinslab (© 2019 University of Washington and Eric Klavins) and material provided as part of the University of Washington course *EE P 520: Software Engineering for Embedded Applications*.