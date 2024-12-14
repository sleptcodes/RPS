# RPS
RPS Cellular Automota (use terminal size 215 X 64 when running)

A collection of toy programs I used years ago to familiarize myself with multithreading

3 programs that do the same thing:

rps: basic rock paper scizzors cellular automata (when a cell is surrounded by 3+ of its predators, converts to its predator)

threaded: Each row gets updated in its own thread and race conditions are avoided. 

threadedandlocked: Each row gets updated in its own thread and race conditions are solved via mutex locks.
