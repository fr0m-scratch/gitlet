#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../src/Serialize.hpp"

int main() {
    // Create directory if it doesn't exist
    struct stat st;
    if (stat(".gitlet", &st) == -1) {
        mkdir(".gitlet", 0700);
    }

    // Test serializing an integer
    int x = 42;
    try {
        serialize(x, ".gitlet/test_int_serialize");

        // Test deserializing the integer
        int y = deserialize<int>(".gitlet/test_int_serialize");

        if (x == y) {
            std::cout << "Serialize/Deserialize test for integer passed.\n";
        } else {
            std::cout << "Serialize/Deserialize test for integer failed.\n";
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    // Add more tests for other data types and complex objects

    return 0;
}
