#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int decodeBase(const std::string& value, int base) {
    int result = 0;
    int power = 1;
    
    // Loop through the string backwards to convert from base to decimal
    for (int i = value.size() - 1; i >= 0; --i) {
        char digit = value[i];
        int numericValue = (isdigit(digit)) ? (digit - '0') : (digit - 'A' + 10);
        result += numericValue * power;
        power *= base;
    }
    
    return result;
}

// Function to parse the JSON structure
std::vector<std::pair<int, int>> parseInput(const json& data) {
    std::vector<std::pair<int, int>> points;
    
    for (auto& element : data.items()) {
        std::string key = element.key();
        if (std::isdigit(key[0])) { // Check if the key is numeric
            int x = std::stoi(key);
            int base = std::stoi(element.value()["base"]);
            std::string value = element.value()["value"];
            int y = decodeBase(value, base); // Decode y based on the base
            points.push_back({x, y});
        }
    }
    
    return points;
}

// Lagrange interpolation to find the constant term (f(0))
double lagrangeInterpolation(const std::vector<std::pair<int, int>>& points, int x_value = 0) {
    double result = 0.0;
    int n = points.size();
    
    for (int i = 0; i < n; ++i) {
        double term = points[i].second;  // yi
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                term *= (x_value - points[j].first) / static_cast<double>(points[i].first - points[j].first);
            }
        }
        result += term;
    }
    
    return result;
}

int main() {
    // Prompt the user for JSON input
    std::cout << "Enter JSON input: " << std::endl;
    
    std::string jsonString;
    std::getline(std::cin, jsonString);
    
    // Parse the JSON input
    json inputData;
    try {
        inputData = json::parse(jsonString);
    } catch (const json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return 1;
    }

  
    std::vector<std::pair<int, int>> points = parseInput(inputData);

    double constantTerm = lagrangeInterpolation(points, 0);
 
    std::cout << "The constant term (c) is: " << constantTerm << std::endl;