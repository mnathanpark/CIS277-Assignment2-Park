#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include "PacketQueue.h"
#include "Queue.h"

int main()
{
    std::cout << "CIS-277 Assignment 2 Demonstration" << std::endl << std::endl;

    // Testing queue functionality with integers
    Queue<int> intQueue(2);
    int intVal = 0;

    assert(intQueue.empty() == true);
    assert(intQueue.front(intVal) == false);

    assert(intQueue.enqueue(100) == true);
    assert(intQueue.enqueue(200) == true);
    assert(intQueue.full() == true);
    assert(intQueue.enqueue(300) == false);

    assert(intQueue.front(intVal) == true && intVal == 100);
    assert(intQueue.size() == 2);

    assert(intQueue.dequeue(intVal) == true && intVal == 100);
    assert(intQueue.front(intVal) == true && intVal == 200);
    assert(intQueue.dequeue(intVal) == true && intVal == 200);
    assert(intQueue.empty() == true);


    // Testing queue functionality with strings
    Queue<std::string> stringQueue(2);
    std::string stringVal = "";

    assert(stringQueue.empty() == true);
    assert(stringQueue.front(stringVal) == false);

    assert(stringQueue.enqueue("Alpha") == true);
    assert(stringQueue.enqueue("Beta") == true);
    assert(stringQueue.full() == true);
    assert(stringQueue.enqueue("Gamma") == false);

    assert(stringQueue.front(stringVal) == true && stringVal == "Alpha");
    assert(stringQueue.size() == 2);

    assert(stringQueue.dequeue(stringVal) == true && stringVal == "Alpha");
    assert(stringQueue.front(stringVal) == true && stringVal == "Beta");
    assert(stringQueue.dequeue(stringVal) == true && stringVal == "Beta");
    assert(stringQueue.empty() == true);

    // Testing queue wraparound
    Queue<char> charQueue(3);
    char charVal = '\0';

    charQueue.enqueue('A');
    charQueue.enqueue('B');
    charQueue.enqueue('C');
    charQueue.dequeue(charVal);
    charQueue.enqueue('D');

    assert(charQueue.size() == 3);
    charQueue.dequeue(charVal); 
    assert(charVal == 'B');
    charQueue.dequeue(charVal); 
    assert(charVal == 'C');
    charQueue.dequeue(charVal); 
    assert(charVal == 'D');
    assert(charQueue.empty() == true);

    // Testing different packet sizes
    PacketQueue testPQ(16, 3);
    std::size_t idOut = 0;
    std::vector<unsigned char> dataOut;

    // Test length of 1
    unsigned char tinyPacket = 0xAB;
    bool r1 = testPQ.receive(1, &tinyPacket, 1);
    assert(r1 == true);

    // Test length of exactly blockSize
    std::vector<unsigned char> exactPacket(16, 0x55);
    bool r2 = testPQ.receive(2, exactPacket.data(), 16);
    assert(r2 == true);

    // Test packet that is too large (length > blockSize)
    std::vector<unsigned char> oversizedPacket(16 + 1, 0xFF);
    bool r3 = testPQ.receive(3, oversizedPacket.data(), oversizedPacket.size());
    assert(r3 == false);

    // Test null pointer/zero length rejection
    assert(testPQ.receive(4, nullptr, 5) == false);
    assert(testPQ.receive(5, exactPacket.data(), 0) == false);

    // Drain and verify round trip
    testPQ.processNext(idOut, dataOut);
    assert(idOut == 1 && dataOut.size() == 1 && dataOut[0] == 0xAB);

    testPQ.processNext(idOut, dataOut);
    assert(idOut == 2 && dataOut.size() == 16 && dataOut[0] == 0x55);




    // Initialize PacketQueue
    PacketQueue pq(64, 3);

    unsigned char packet1[] = {0x00, 0x01, 0x02, 0x3C};
    unsigned char packet2[] = {0xFF, 0xEE, 0xDD};
    unsigned char packet3[] = {0x11, 0x22, 0x33, 0x44};

    std::cout << "Receiving 3 packets" << std::endl;
    pq.receive(101, packet1, sizeof(packet1));
    pq.receive(102, packet2, sizeof(packet2));
    pq.receive(103, packet3, sizeof(packet3));

    std::cout << "Pending packets: " << pq.pending() << std::endl;
    std::cout << "Available blocks: " << pq.availableBlocks() << std::endl << std::endl;

    std::size_t id = 0;
    std::vector<unsigned char> outBytes;

    std::cout << "Processing one packet" << std::endl;
    pq.processNext(id, outBytes);
    std::cout << "Processed packet ID: " << id << " (Expected: 101)" << std::endl;

    std::cout << "Receiving packet 104 after processing one..." << std::endl;
    pq.receive(104, packet1, sizeof(packet1));
    std::cout << "Pending packets: " << pq.pending() << std::endl;
    std::cout << "Available blocks: " << pq.availableBlocks() << std::endl << std::endl;

    std::cout << "Now dequeuing until empty to show that FIFO behavior is maintained." << std::endl;
    pq.processNext(id, outBytes);
    std::cout << "Processed packet ID: " << id << " (Expected: 102)" << std::endl;
    pq.processNext(id, outBytes);
    std::cout << "Processed packet ID: " << id << " (Expected: 103)" << std::endl;
    pq.processNext(id, outBytes);
    std::cout << "Processed packet ID: " << id << " (Expected: 104)" << std::endl << std::endl;

    std::cout << "Filling queue with 3 more packets with ids 105, 106, 107." << std::endl;
    pq.receive(105, packet1, sizeof(packet1));
    pq.receive(106, packet2, sizeof(packet2));
    pq.receive(107, packet3, sizeof(packet3));
    std::cout << "Pending packets: " << pq.pending() << std::endl;
    std::cout << "Available blocks: " << pq.availableBlocks() << std::endl << std::endl;
    std::cout << "Queue now full, attempting to recieve one more." << std::endl;
    bool received = pq.receive(108, packet1, sizeof(packet1));
    std::cout << "Receive status: " << (received ? "true" : "false") << std::endl << std::endl;

    std::cout << "Draining queue" << std::endl;
    pq.processNext(id, outBytes);
    pq.processNext(id, outBytes);
    pq.processNext(id, outBytes);
    std::cout << "Pending packets: " << pq.pending() << std::endl;
    std::cout << "Available blocks: " << pq.availableBlocks() << std::endl << std::endl;
    std::cout << "Queue drained, attempting to process one more" << std::endl;
    bool processed = pq.processNext(id, outBytes);
    std::cout << "Process status: " << (processed ? "true" : "false") << std::endl << std::endl;

    unsigned char roundTripPacket[] = {0x00, 0xFF};
    std::cout << "Receiving packet containing 0x00 and 0xFF and immediately processing it" << std::endl;
    pq.receive(109, roundTripPacket, sizeof(roundTripPacket));
    pq.processNext(id, outBytes);
    std::cout << "Printing contents of the processed packet" << std::endl;
    for (unsigned long i = 0; i < sizeof(roundTripPacket); i++)
    {
        std::cout << "0x" << std::hex << static_cast<int>(roundTripPacket[i]) << std::endl;
    }

    return 0;
}