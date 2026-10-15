#include <cstdint>
#include <cstdlib>

class Math {

    public :

        static uint32_t adder_no_retained( uint32_t a, uint32_t b) 
        {
            uint32_t result = 0;
            uint32_t index = 1;

            for ( size_t i = 0; i < 32; i++ ) {
                result |=  (( a & index ) ^ ( b & index )); 
                index <<= 1;
            }
            return result;
        }

        static uint32_t adder( uint32_t a, uint32_t b) 
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

        static uint32_t multiplier( uint32_t a, uint32_t b ) {

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

        static uint32_t exponent( uint32_t n, uint32_t e ) {

            uint32_t result = n;

            for ( size_t i = 0; i < e - 1; i++ ) {
                result = multiplier(result, n);
            }
            return result;
        }

        static uint32_t gray_code( uint32_t n ) {
            return adder_no_retained(n , n << 1) >> 1;
        }
};
