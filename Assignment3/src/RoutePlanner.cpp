#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <string>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    // Load map data from file
    // Mark all provinces as unvisited initially

    // Load priority provinces
    // Load restricted provinces
    map.loadDistanceData(distance_data);
    map.resetVisited();

    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    int index = 0;

    while (std::getline(file, line)) {
        if (index >= MAX_PRIORITY_PROVINCES) {
            std::cerr << "Warning: Number of provinces exceeds maximum limit." << std::endl;
            break;
        }

        // Find the position of '(' and ')' in the line
        size_t openParen = line.find('(');
        size_t closeParen = line.find(')');

        // Check if both parentheses exist and are in the correct order
        if (openParen != std::string::npos && closeParen != std::string::npos && openParen < closeParen) {
            // Extract the number between the parentheses
            std::string numberStr = line.substr(openParen + 1, closeParen - openParen - 1);
            int cityNumber = std::stoi(numberStr);

            // Add the number to the array
            priorityProvinces[index++] = cityNumber;
            numPriorityProvinces++;
        }
    }
    file.close();
}




// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    int index = 0;

    while (std::getline(file, line)) {
        if (index >= MAX_WEATHER_RESTRICTED_PROVINCES) {
            std::cerr << "Warning: Number of weather restricted provinces exceeds maximum limit." << std::endl;
            break;
        }

        // Find the position of '(' and ')' in the line
        size_t openParen = line.find('(');
        size_t closeParen = line.find(')');

        // Check if both parentheses exist and are in the correct order
        if (openParen != std::string::npos && closeParen != std::string::npos && openParen < closeParen) {
            // Extract the number between the parentheses
            std::string numberStr = line.substr(openParen + 1, closeParen - openParen - 1);
            int cityNumber = std::stoi(numberStr);

            // Add the number to the array
            weatherRestrictedProvinces[index++] = cityNumber;
            numWeatherRestrictedProvinces++;
        }
    }
    file.close();
}


// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int i = 0; i < numWeatherRestrictedProvinces; i++) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    stack.push(startingCity);
    route.push_back(startingCity);
    map.markAsVisited(startingCity);

    while(!isExplorationComplete()) {
        exploreFromProvince(startingCity);
        backtrack();

        if (!stack.isEmpty()) {
            startingCity = stack.peek();

        } else {
            break; // Exit the loop if stack is empty
        }
    }
    displayResults();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    if(queue.isEmpty()) {
        enqueueNeighbors(province);
    }

    while(!queue.isEmpty()) {
        int province_value = queue.dequeue();
        if(!isWeatherRestricted(province_value)){ // hava durumu bozuk değilse buraya gideceğiz.
            if(map.isWithinRange(stack.peek(),province_value,maxDistance) && !map.isVisited(province_value)) {
                totalDistanceCovered+= map.getDistance(stack.peek(), province_value);

                stack.push(province_value);
                map.markAsVisited(province_value);
                route.push_back(province_value); // route a ekledim
                enqueueNeighbors(province_value);

            }
        }
        else {
            std::cout << "Province " << cities[province_value] << " is weather-restricted. Skipping." << std::endl;
        }
    }
}

void RoutePlanner::enqueueNeighbors(int province) {
    for(int i = 0; i < 81 ; i++) {
        if(!map.isVisited(i) && map.isWithinRange(province,i,maxDistance) && i != province) {
            if(queue.size == MAX_QUEUE_SIZE) {
                std::cerr << "Queue overflow: Cannot enqueue province " << cities[i] << ". Queue is full." << std::endl;
                continue;
            }
            if(isPriorityProvince(i)) {
                queue.enqueuePriority(i);
            } else {
                queue.enqueue(i);
            }
        }
    }
}

void RoutePlanner::backtrack() {
    // If you reach a dead-end province 
    if (!stack.isEmpty()) {
        int backtracked_province = stack.pop();

        //map.visited[backtracked_province] = false;

    }
}

bool RoutePlanner::isExplorationComplete() const {
    if(stack.isEmpty() && queue.isEmpty()) {
        return true;
    }
    return false;
}

std::string RoutePlanner::visited_or_not(int priority_province) const {
    if(map.isVisited(priority_province)) {
        return "Visited";
    }
    return "Not Visited";
}

void RoutePlanner::displayResults() const {
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary

    int count = 0;
    for(bool i : map.visited) {
        if(i) {
            count++;
        }
    }

    int count_priority = 0;
    for(int i = 0; i < numPriorityProvinces; i++) {

        if(map.visited[priorityProvinces[i]]) {
            count_priority++;
        }
    }

    std::cout << "----------------------------" << std::endl;
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Total Number of Provinces Visited: " << count <<  std::endl;
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;
    std::cout << "Route Taken:" << std::endl;

    for (int province : route) {
        std::cout << cities[province] << " -> ";
    }
    std::cout << "End" << std::endl;

    std::cout << "Priority Provinces Status:" << std::endl;
    for(int i = 0; i < numPriorityProvinces; i++) {
        std::cout << "- " << cities[priorityProvinces[i]] << " (" << visited_or_not(priorityProvinces[i])  << ")" << std::endl;
    }

    std::cout << "Total Priority Provinces Visited: " << count_priority << " out of " << numPriorityProvinces << std::endl;

    if(numPriorityProvinces == count_priority) {
        std::cout << "Success: All priority provinces were visited." << std::endl;
    }else {
        std::cout << "Warning: Not all priority provinces were visited." << std::endl;
    }
}


