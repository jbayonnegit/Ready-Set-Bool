#include <cstdlib>
#include <iostream>
#include <cstdint>

void    printBinary( uint32_t a ) {

    uint32_t result = 0;
    int32_t tmp = 32768 * 32768;

    for ( size_t i = 0; i < 32; i++ ) {
        result = a & tmp ? 1 : 0;
        tmp >>= 1;
        std::cout << result ;
    }
}

uint32_t adder_no_retained( uint32_t a, uint32_t b) 
{
    uint32_t result = 0;
    uint32_t index = 1;
    
    for ( size_t i = 0; i < 32; i++ ) {
        result |=  (( a & index ) ^ ( b & index )); 
        index <<= 1;
    }
    return result;
}

uint32_t gray_code( uint32_t n ) {
    return adder_no_retained(n , n << 1) >> 1;
}

int main( int ac, char **av ) {

    if ( ac != 2 ) exit(1);
        uint32_t n = std::atoi( av[1] );
    std::cout << "n            : "; printBinary(n); std::cout << std::endl;
    uint32_t result = gray_code(n);
    std::cout << "gray_code(n) : "; printBinary(result); std::cout << std::endl;
    std::cout << "result       : " << result << std::endl;
    return 0;
}