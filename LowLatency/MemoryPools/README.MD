# Memory Pools

Memory pools are used to avoid Dynamic Memory allocation.
A useful low latency application cannot be built without creating and deleting many objects at runtime.

## Definitions

### Stack Memory Allocation

The allocation happens on contiguous blocks of memory. It is called stack memory allocation because it happens happens in the function call stack. The size of memory to be allocated is known to the compiler.
Whenever a function is called, its variables get memory allocated on the stack. 
Whenever the function call is over, the memory for the variables is de-allocated. 
This all happens automatically, a programmer does not have to worry about memory allocation and de-allocation of stack variables. Also known as Temporary memory allocation because as soon as the method finishes its execution all the data belonging to that method flushes out from the stack automatically. This means any value stored in the stack memory scheme is accessible as long as the method hasn’t completed its execution and is currently in a running state.

- It’s a temporary memory allocation scheme where the data members are accessible only if the method( ) that contained them is currently running.
- It allocates or de-allocates the memory automatically as soon as the corresponding method completes its execution.
- Stack memory allocation is considered safer as compared to heap memory allocation because the data stored can only be accessed by the owner thread.
- Memory allocation and de-allocation are faster as compared to Heap-memory allocation.
- Stack memory has less storage space as compared to Heap-memory.
  
### Heap Memory Allocation

The memory is allocated during the execution of instructions. It is called a heap because it is a pile of memory space available to allocate and de-allocate. Every time we make an object it is always created in Heap-space and the referencing information is always stored in Stack-memory. 
Heap memory allocation isn’t as safe as Stack memory allocation because the data stored in this space is accessible or visible to all threads. If a programmer does not handle this memory well, a memory leak can happen in the program.

- No automatic de-allocation feature is provided. We need to use a Garbage collector to remove the old unused objects in order to use the memory efficiently.
- The processing time(Accessing time) of this memory is quite slow as compared to Stack-memory.
- Heap memory is also not as threaded-safe as Stack-memory because data stored in Heap-memory are visible to all threads.
- The size of the Heap-memory is quite larger as compared to the Stack-memory.
- Heap memory is accessible or exists as long as the whole application runs.

### Memory Pool

A memory pool is defined as anything from which we can request additional memory or objects and return free memory or objects to. By building our own custom memory pool, we can leverage the usage patterns and control the allocation and deallocation mechanisms for optimal performance.

We will use the memory pool to describe a 
pool of objects of a certain type. We will use the memory pool to allocate and deallocate objects that we cannot predict.

## Solution

Pre-allocate large blocks of memory at startup and serve out required amounts at runtime.
Do the memory allocation and deallocation. Improvements to performance:
- Limit the memory pool usage to certain components in our system instead of all processes running on the server. 
- Control the memory storage and allocation and deallocation algorithms, tuning them to perform optimally for our specific application.