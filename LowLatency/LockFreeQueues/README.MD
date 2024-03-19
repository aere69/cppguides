# Lock Free Queues

Queues provide a way to communicate/transfer data between threads and processes.

## Use Cases

1.  Concurrent access to shared data and it is important to avoid data corruption.
2.  Reader and writer must have consistent views of the shared data.
3.  Transfer information from one thread to another (or from one process to another).
   
We will create a lock-free queue to facilitate communication between threads without the overhead of locks and context switches. This lock-free queue is only to be used for Single Producer Single Consumer (SPSC) – that is, only one thread writes to the queue and only one thread consumes from the queue. 
