
# Project 1 Report
##### CMPT355 - Jayden Laturnus, Andy Yip, Graham Cooper

AB puzzle solver report in which the search algorithm, heuristic, evaluation function, enhancements, future improvements, and interesting events are discussed.

## Search Algorithm Description

The search algorithm we decided to implement was A*. This was chosen largely because memory was not a constraining factor with regards to the complexity of the puzzle. Because we wrote our program in C, we have more control over how much memory is used. As a result, our search algorithm prioritized speed and low memory usage. 

The state space representation of our puzzle problem represents both the larger and smaller disks in an integer array. A node struct contains values used by A* search in order to find the optimal solution. The contents of said struct are outlined below:

* *current state* 
* *goal state - g(n)* 
* *heuristic (estimated cost from n to reach goal) - h(n)*
* *goal state + heuristic (estimated total cost of path through n to goal) - f(n)*
* *parent node*


## Heuristic

Our heuristic begins with going through the current state, index by index and finding the absolute value between the goal state for each disk. After iterating through the disks, the sum of the absolute values becomes the h(n) for the node. This means that nodes that are closer to the solution will have a smaller sum. This heuristic was chosen simply because it outputted a correct solution. When other heuristics were tried, the correct answer was not found or simply took quite a bit longer to find the optimal solution. Thus, despite knowing our heuristic was sub-optimal we chose it on the basis of speed rather than finding the most optimal solution.

## Evaluation

Our evaluation function checks the current state of each node that is evaluated in the frontier via its heuristic. If the h(n) value is 0, it means the positions of the small disks are aligned with the goal state. The goal function is set for the child state by taking their parent goal function and incrementing it by 1.

## Enhancements

During our initial research phase, our team noticed that most implementations of A* used 2 linked lists for the Frontier and Explored nodes. However, we realized that a hash table would be much faster at checking and accessing the nodes we need without having to traverse the linked list. In the end we settled on making the frontier and explored list into one hash table and kept another frontier as a priority queue. The benefit of this approach was that we can quickly check if a new state already exists or not and we do not have to worry about removing anything from the hash table.

Another smaller enhancement was made to decrease memory usage. We would check the current state to see if it existed in the Frontier or Explored, and if it did, the memory allocated for it would be freed immediately. 


## Interesting things

Some interesting things we did was change our initial heuristic (misplaced numbers) to a heuristic we never covered in class which was the sum of the absolute values of the difference between the goal state and current state. We were surprised to see drastic performance increases. Previously, the puzzle would take over an hour to solve AB17, but with the changes, the puzzle was solved in under a minute. 

The "%" we were familiar with behaves differently in C with negative numbers. In C, % gives the remainder while whereas in a language such as Python, it performs the modulus. Therefore, when we were implementing wrap-around disk swapping, values were not behaving as expected.

We initially used the number of misplaced numbers heuristic, and then switched to Manhattan Distance heuristic which gave us better speeds, and a more optimal "solution". However, we discovered that the Manhattan distance heuristic solution was not correct which surprised us because we believed it to be a "safe" heuristic choice. 

One thing we noticed was that depending on the input provided with the Manhattan distance heuristic and absolute distance heuristic, each heuristic would outperform the other on different inputs.

Absolute distance performed better on:

1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17

17 16 15 14 13 12 11 109 8 7 6 5 4 3 2 0

while Manhattan distance performed better on:

1 2 1 2 3 2 1 2 1 2 1 3 4 1 2 3 4

3 2 3 2 1 0 2 1 1 3 4 1 4 2 4 3 4

## Problems

Our team ran into various problems with our puzzle solver.
1. The hash table for our Frontier nodes were creating duplicate hash values. These duplicate hash values quickly filled our hash table, causing segmentation faults. We realized that this was caused by not filtering duplicates in the Frontier.   

2. We initially used quadratic probing, however, our function was not allocating the nodes properly into the correct locations. This worsened problem number 1 and took a significant amount of time to discover. After switching to linear probing, our hash table worked properly.

3. We had to continuously modify our current heuristic as well as previous heuristics used for the program to solve AB17. Because it took so long to solve the larger puzzle, we did not know if it could actually solve the puzzles. We let the program run for multiple hours with no solution output. This caused confusion among our group because the smaller puzzles could be solved fine. This led us to use a trial and error method with variations of our heuristics until we found our current heuristic to be the most sufficient.   

4. Debugging was also difficult because had to create our own data structures from scratch meaning there were many possibilities for errors to occur in our code. Finding the bugs took a significant amount of time because the program was not operating the way we intended it to.

## Improvements

Some improvements that can be made is to make the program find the goal optimally. While the correct answer is consistently found, the optimal solution is not. After conversing with other groups, we found that the number of steps we needed to find the goal was significantly higher than other groups. We realized this is due to our heuristic being inadmissible. Therefore, given more time, we would have utilized and researched an admissible heuristic without sacrificing speed. We would have attempted to get Manhattan Distance to work properly. This heuristic was faster and admissible, which would have found an optimal solution. Another solution would be to fix our current heuristic (absolute distance) which currently overestimates the path to the goal. For example:

3 3 3 2 2 2 1 1 1 <-- This is a solution not found by the absolute distance heuristic

1 1 1 2 2 2 3 3 3 <-- Goal state of absolute distance heuristic

meaning, there will be unnecessary steps taken to get to the goal state when the solution has been technically found. 

In the future, we would find the index of the first 1 in the current state and have it iterate through the disks from that position. This would have made sure the order of the small disks in our current state is aligned with our goal state.

Another improvement is to add ulimit functionality which would have limited the amount of memory available. After numerous attempts, we concluded that ulimit failed to limit our memory, and failed to halt our program if it exceeded the set value. A limit of multiple vales was set, such as 1 MB, but the program continued to run without crashing. Multiple documents that we read failed to provide a solution. Given more time, we would have read through more documentation to try to get ulimit to work.


