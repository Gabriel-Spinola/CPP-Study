#include <iostream>
#include <string>
#include <array>

int main() {
    const int arraySize = 12;
    int firstArray[arraySize] = { 
        1, 2, 3, 4, 5, 6,
        7, 8, 9, 10, 11, 12
    };

    firstArray[4] = 23;

    for(int i = 0; i < arraySize; i++) {
        std::cout << i << ' ' << firstArray[i] << "\n";
    }

    std::cout << "\n";

    const int stringArraySize = 3;
    std::string stringArray[stringArraySize] = { "None" };

    for(int i = 0; i < stringArraySize; i++) {
        std::cout << i << " Enter a name: ";
        std::getline(std::cin, stringArray[i]);
    }

     for(int i = 0; i < stringArraySize; i++) {
        std::cout << i << ' ' << stringArray[i] << "\n";
    }

    system("PAUSE");
}