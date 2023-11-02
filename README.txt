#READ.ME

Project: Producer-Consumer Thread Synchronization
by Tyler Edwards and Jack de Bruyn
Added November 2023

Description: Solving the Prodcuer-Consumer problem of addressing thread synchronization using semaphores, mutexes, and a bounded buffer. 

Language: C 

Reason: For CPSC 380 (Operating Systems)

Successes: Past experience in class with threads, allowed me to focus on implementing the new concepts.

Setbacks: Figuring out when to use a mutex lock versus semaphores proved to be tricky.

Learned: how to use semaphores, how to use mutex locks, how to use a bounded buffer


/*
Commands:
gcc prodcon.c -o prodcon
./prodcon [DELAY] [# OF PRODUCER THREADS] [# OF CONSUMER THREADS]

Output:
Producer produced [CHECKSUM]
Producer produced [CHECKSUM]
Consumer consumed [CHECKSUM]
...
*/

- Started from in-class base code
