#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited
    const int NO_CONNECTION = 999999;

    for(int i=0; i< MAX_SIZE; i++) {
        for(int j = 0; j<MAX_SIZE; j++) {
            distanceMatrix[i][j] = NO_CONNECTION;
        }
    }
    for (bool & i : visited) {
        i = false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    std::string line;
    int row = 0;

    while (std::getline(file, line) && row < 81) {
        std::stringstream ss(line);
        std::string number;
        int col = 0;

        while (std::getline(ss, number, ',') && col < 81) {
            distanceMatrix[row][col] = std::stoi(number); // Convert string to integer
            col++;
        }

        row++;
    }

    file.close();

}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
  	if(getDistance(provinceA,provinceB) <= maxDistance){ //eşittir işareti olacak mı????
          return true;
    }
    return false; 
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    for (bool & i : visited) {
        i = false;
    }

}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int count = 0;
    for(bool i : visited) {
        if(i) {
            count++;
        }
    }

    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    return distanceMatrix[provinceA][provinceB];
}
