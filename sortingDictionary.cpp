#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>

// Function to check if a string contains only letters
bool isOnlyLetters(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isalpha(c); });
}

// Function to convert a string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string lowerCaseStr = str;
    std::transform(lowerCaseStr.begin(), lowerCaseStr.end(), lowerCaseStr.begin(), [](unsigned char c) { return std::tolower(c); });
    return lowerCaseStr;
}

// Comparison function for sorting by length and alphabetically
bool compareByLengthAndAlphabetically(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) {
        return a.size() < b.size();
    }
    return a < b;
}

int main() {
    std::ifstream inputFile("words.txt");
    std::ofstream outputFile("dictionary.txt");

    if (!inputFile) {
        std::cerr << "Unable to open input file." << std::endl;
        return 1;
    }

    std::vector<std::string> validLines;
    std::string line;

    while (std::getline(inputFile, line)) {
        // Check if the line contains only letters and has a length of 3 or more
        if (isOnlyLetters(line) && line.size() >= 3) {
            validLines.push_back(toLowerCase(line));
        }
    }

    // Sort the valid lines by length and alphabetically
    std::sort(validLines.begin(), validLines.end(), compareByLengthAndAlphabetically);

    // Write the sorted lines to the output file
    for (const auto& sortedLine : validLines) {
        outputFile << sortedLine << std::endl;
    }

    std::cout << "File sorted by string length and alphabetically successfully!" << std::endl;

    return 0;
}
