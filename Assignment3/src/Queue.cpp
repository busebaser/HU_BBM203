#include "Queue.h"
#include <iostream>

#include "Map.h"

// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here
    front = -1;
    rear = -1;
    size = 0;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // Implement circular structure 
    // Add the province
    if(size==MAX_QUEUE_SIZE) {
        std::cerr << "Queue is full!"  << std::endl;
        return;
    }
    if(isEmpty()) {
        front = 0;
        rear = 0;
    }else {
        rear = (rear + 1) % MAX_QUEUE_SIZE;
    }
    data[rear] = province;
    size++;

}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if(isEmpty()) {
        std::cerr << "Queue is empty!" << std::endl;
        return -1;
    }
    int value = data[front];
    front = (front + 1) % MAX_QUEUE_SIZE;
    size--;

    if(size ==0) {
        front = rear = -1;
    }

    return value;
}

// Returns the front province without removing it
int Queue::peek() const {
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    return size==0;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    if (size == MAX_QUEUE_SIZE){
        std::cerr << "Queue is full! Cannot enqueue the priority!" << std::endl;
        return;
    }

    if (isEmpty()) {
        front = 0;
        rear = 0;
        data[rear] = province;
        size++;
        return;
    }

    front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    data[front] = province;
    size++;

}

