minishell.c creates a custom shell that works for all Bash commands. It can also run other programs and process keyboard signals. This gave me good practice using the exec and fork
commands in c, as well as utilizing an atomic variable with a sighandler to handle the keyboard signals.

tiling.rkt is a program written in scheme that plays the tiling the courtyard game. This game allows the user to define the dimensions of a square board comprised of many smaller
squares. First the program places one random square somewhere in the courtyard. Then, my program fills the courtyard with L's. an L is comprised of three individual squares that
form an L shape. Since scheme is a purely recursive language, this program gave me a lot of good practice with complicated recursion, specifically divide and conquer recursion.

waterjug.cpp is a program written in c++ that plays the waterjug game. There are three waterjugs, each full of water in some start state. They then pour water into eachother by a
strict set of rules. the user defines the start state, along with an end state. My program determines if the end state is reachable from the start state. From this I learned how to 
work with pointers in c++.
