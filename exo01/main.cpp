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

uint32_t adder( uint32_t a, uint32_t b) 
{
    uint32_t result = 0;
    uint32_t index = 1;
    uint32_t retained = 0;
    
    for ( size_t i = 0; i < 32; i++ ) {
        retained <<= 1;
        // bit turn to one if ( a XOR b XOR retained ) OR if a AND b AND retained
        result |=  (( a & index ) ^ ( b & index ) ^ retained ) | (( a & index ) & ( b & index ) & retained); 
        // retained turn to one if ()
        retained = (( a & index ) & ( b & index )) | ( retained & (( a & index ) | ( b & index )));
        index <<= 1;
    }
    return result;
}

uint32_t multiplier( uint32_t a, uint32_t b ) {

    uint32_t index = 1;
    uint32_t result = 0;
    uint32_t mul = 0;

    for ( size_t i = 0; i < 32; i++ ) {
        mul = (a & index);  
        if ( mul != 0)
            result = adder( result, b <<  i );  
        index <<= 1;
    }
    return result;
}

int main( int ac, char **av ) {

    if ( ac != 3 ) exit(1);
        uint32_t a = std::atoi( av[1] );
    uint32_t b = std::atoi( av[2] );
    std::cout << "a      : "; printBinary(a); std::cout << std::endl;
    std::cout << "b      : "; printBinary(b); std::cout << std::endl;
    uint32_t result = multiplier(a, b);
    std::cout << "result : "; printBinary(result); std::cout << std::endl;
    std::cout << "result : " << result << std::endl;
    return 0;
}