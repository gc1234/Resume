# AB Puzzle Solver
Implementation of an AB puzzle solver, project was done for CMPT355 at Macewan University, implemented by Graham Cooper, Andy Yip, Jayden Laturnus

## Getting Started

To run this project and be using a Linux or MacOS operating system. From your command line:

```bash
 # Extract the zip folder and navigate to the project directory
$ cd ab-puzzle-solver

 # Create the executable and object file(s)
$ make

 # Run the shell
$ ./AB 10

 # Input large disks, followed by small disks as such
$ 1 2 3 4 1 2 3 1 2 3
$ 1 1 1 3 2 2 3 3 0 2

 # Output as such would be
$ Solution given:
$ 1 1 1 3 2 2 3 3 0 2
$ 1 1 1 3 2 2 0 3 3 2
$ 1 1 1 0 2 2 3 3 3 2
$ 1 1 1 2 2 2 3 3 3 0
```

## Notes

On large puzzle sizes the program may take some time to solve the puzzle and output a solution, on average given size 17 the puzzle solved it within a few seconds. Sizes greater than this may exceed multiple 10s of seconds and could even take as long as multiple minutes.