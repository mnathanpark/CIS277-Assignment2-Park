# CIS-277 Assignment 2: Network Packet Arrival Queue

## Student
Nathan Park

## Design
The queue class uses a fixed capacity circular queue. The underlying data structure is a vector. The front index is tracked, and only updated when dequeue is called. The rear index is calculated using modular arithmetic when enqueue is called to correctly place the new packet in the right index.
The memory pool handles the memory of the raw binary bytes in the packets with LIFO behavior. The underlying data structure is a stack. The queue handles the data about the packets with FIFO behavior. The queue is made of PacketDescriptor structures, which contain an id, pointer to the block of memory, and the length of the packet.

## How to Compile
g++ -std=c++23 -Wall -Wextra -pedantic main.cpp PacketQueue.cpp MemoryPool.cpp -o main

## How to Run
./main

## Tests and Results
Using assert, I tested the queue functionality with a queue of integers and strings for the empty function, full function, front function both when empty and containing something, enqueue function, and dequeue function. The expected and actual values all match. 
Using assert, I tested the wraparound functionality of a fixed capacity circular queue by enqueueing until full, dequeueing once, and enqueueing once more. The expected and actual values all match.
Using assert, I tested the ability of the queue to accept packages of different sizes. One was a packet of length 1, another was a packet the exact size of the block, and the last was a packet too big for the block size. The expected and actual values all match.

## Analysis Questions
Why must packet service be FIFO even though free-block reuse is LIFO?
Packet service must be FIFO so that each packet can eventually be processed. If it were LIFO, then given a steady stream of new packets, theoretically the very first packet received would never be processed and would always be skipped in favor of the newest received packet, which is not fair. Free-block reusue if LIFO because it is only cares where the memory is stored, not in what order it is stored. There is no concept of fairness in this case.

How does your circular array wrap without shifting all waiting packets?
By keeping track of the front index and how many items the queue currently has, it can wrap around the fixed capacity using modular arithmetic to find the tail index. Each enqueue increases the count, which updates the tail index. Each dequeue decreases the count and updates the front index. 

What happens at queue overflow and underflow, and why must those attempts leave state unchanged?
Both enqueue and dequeue will return false when the queue is overflowing or underflowing. Because this is implemented with a fixed capacity queue, overflowing and underflowing are undefined behavior. Changing any state of the queue when attempting these undefined behaviors will mess up the queue, and thus must be unchanged to maintain validity.

What are the time complexities of queue operations and of receive/processNext when the packet has length bytes?
Queue operations all run in O(1) time. The receive and processNext functions operate in O(length) time, because they must both copy however many bytes. 

Why must processNext copy bytes out before returning the block to the pool?
If the block returned to the pool without the bytes being copied, the stack will treat that memory block as free to be allocated. If that block were to be allocated before we copy those bytes, it could be overwritten with new data and that preexisting data would be lost forever, which is not what we want. 

## AI Use 
I used Gemini to help with the assert test cases at the beginning of main.cpp. I checked the result to make sure the assert was working properly and that each assert was checking for the correct thing.