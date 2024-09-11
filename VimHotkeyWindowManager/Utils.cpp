#include "Utils.h"

std::string trim(const std::string& s) {
    if (s.empty()) {
        return "";
    }

    auto start = s.begin();
    // Skip leading whitespace characters
    while (start != s.end() && std::isspace(static_cast<unsigned char>(*start))) {
        start++;
    }

    auto end = s.end();
    // Skip trailing whitespace characters
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(static_cast<unsigned char>(*end)));

    // Return the substring from the start position to the end position
    return std::string(start, end + 1);
}

// Function to check if a string contains only whitespace and newline characters
bool isWhitespaceOnly(const std::string& s) {
    for (char c : s) {
        // Check if the character is not a whitespace character
        if (!std::isspace(static_cast<unsigned char>(c))) {
            return false; // If any character is not a whitespace, return false
        }
    }
    return true; // All characters are whitespace
}