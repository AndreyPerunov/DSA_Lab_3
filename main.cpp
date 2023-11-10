#include <iostream>
#include <fstream>
#include <string>
#include <vector>  

int main() {
  // Initialize Data Structure
  

  // Read Data
  std::ifstream file("movies.data");

  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }

  std::string line;

  while (getline(file, line)) {
    try {
      // Add line to data structure

    } catch (const std::invalid_argument& e) {
      std::cerr << "Error reading line number." << std::endl;
    }
  }

  file.close();

  // Process Data

  return 0;
}