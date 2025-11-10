#include <iostream>
#include "BitArray.h"

int main() {
    BitArray array(10, true);
    std::cout << "Bit array (10 bits with the true value): " << array.to_string() << std::endl;

    array.set(5, false);
    array.set(2, false);
    std::cout << "After changing bits 5 and 2 to the false value: " << array.to_string() << std::endl;

    // 3. Проверка размера и состояния
    std::cout << "Size of array: " << array.size() << std::endl;
    std::cout << "Sum of bits: " << array.count() << std::endl;

    return 0;
}