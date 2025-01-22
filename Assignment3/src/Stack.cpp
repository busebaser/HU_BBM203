#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack(): data{} {
    top = -1;
}

// Adds a province to the top of the stack
void Stack::push(int province) {
    if(top >= MAX_SIZE -1){
        std::cerr << "Stack is full! " << std::endl;
        return;
    }
    data[++top] = province;
}


// Removes and returns the top province from the stack
int Stack::pop() {
    if(isEmpty()) {
        std::cerr << "Stack underflow!" << std::endl;
        return -1;

    }
    return data[top--];
}

// Returns the top province without removing it
int Stack::peek() const {
    if(isEmpty()) {
        std::cerr << "Stack is empty!" << std::endl;
        return -1;
    }
    return data[top];
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    return top == -1;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    if(isEmpty()) {
        return 0;
    }
    return top+1;
}
