# Heap-Memory-Management-Simulator


The project is a memory management system that provides functionality for allocating and deallocating memory blocks in a heap data structure. It implements various algorithms and features to efficiently manage memory and optimize resource utilization.

### Functionality:

1.	Memory Allocation: The system allows users to allocate memory blocks dynamically based on their requirements.
2.	Memory Deallocation: Users can deallocate memory blocks when they are no longer needed, freeing up resources for future allocation.
3.	Free Space Management: The system efficiently manages free memory blocks to minimize fragmentation and maximize available memory.
4.	Memory Block Tracking: The system keeps track of allocated memory blocks, allowing users to perform operations on specific blocks using their names.
5.	Merge Free Blocks: Whenever possible, adjacent free memory blocks are merged together to create larger contiguous free blocks, improving memory utilization.


### Algorithms Used:

•	First-Fit Algorithm: The system employs the first-fit algorithm to find a suitable free memory block for allocation. It searches the free list sequentially and selects the first block that satisfies the size requirement.\
•	Best-Fit Algorithm: In addition to the first-fit algorithm, the system also utilizes the best-fit algorithm. It searches the free list for the smallest available block that can accommodate the requested size. This algorithm aims to minimize fragmentation and allocate memory more efficiently.



### Features:

1.	Heap Data Structure: The project uses a heap data structure to manage the memory blocks efficiently.
2.	Dynamic Memory Allocation: Users can dynamically allocate memory blocks of varying sizes as per their needs.
3.	Memory Deallocation: The system allows users to deallocate memory blocks, freeing them up for future allocations.
4.	Memory Block Tracking: Each allocated memory block is associated with a name, allowing users to perform operations on specific blocks by referring to their names.
5.	Efficient Free Space Management: The system maintains a free list that tracks available memory blocks, enabling quick allocation and merging of free blocks.
6.	Memory Block Merging: Adjacent free memory blocks are merged to create larger free blocks, reducing fragmentation and improving memory utilization
7.	Error Handling: The system handles scenarios where memory allocation or deallocation fails and provides appropriate error codes or messages.
8.	Memory Usage Statistics: The system keeps track of the overall allocated and free memory sizes, providing information about memory utilization.

\
\
Overall, this project provides a robust memory management system with features like dynamic memory allocation, efficient free space management, and memory block tracking. By utilizing algorithms like the first-fit and best-fit algorithms, it optimizes memory utilization, minimizes fragmentation, and ensures efficient management of memory resources.
