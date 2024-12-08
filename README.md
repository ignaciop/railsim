# railsim

Metro simulation using POSIX threads (pthreads) API.

Based on a project from the COMP304 course (Spring 2018) at Koc University (more details in original pdf inside docs folder).

Some slight modifications I made:

*The simulation will represent the number of times the tunnel is cleared as rounds.*

*Each round takes aproximately 1 minute to finish.*

*Note: the original project uses simulated time, but I feel it is more consistent if the tunnel ends each round in a clear state.*

*Instead of -s, the command line argument -r is used to indicate the total number of rounds (e.g. -r 2).*

The rest is the same as the original.

Build: **make release**

Usage: **build/release/railsim -p X -r N**

X: Probability of trains arriving at each section every second (0.0 <= X <= 1.0)

N: Number of tunnel cleared rounds (N >= 1)

Example run with standard values: ***build/release/railsim -p 0.5 -r 2***
