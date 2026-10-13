#include <cstdint>
#include <iostream> 
#include <cstdlib> 

// Injective : 

// Let f be a function, E and F be set. An application is injective if each distinct elements of
// E admit a distinct image in F. In other words :
// Let x and x' elementss of E, f(x) != f(x')

// Surjective :

// Let f be a function, E and F be set. An application is surjective if each element of F have a pre-image
// in E. In other words : 

// For all y element's of F, there is an x in E, such as f(x) = y

// Bijective : Injective + Surjective

void    printBinary( uint32_t a ) {

    uint32_t result = 0;
    int32_t tmp = 32768 * 32768;

    for ( size_t i = 0; i < 32; i++ ) {
        result = a & tmp ? 1 : 0;
        tmp >>= 1;
        std::cout <<  result ;
    }
}

double  map( uint16_t x, uint16_t y) {

    uint32_t result = 0;
    uint32_t index_x = 1;
    uint32_t index_y = 2;

    uint32_t ytmp = static_cast<uint32_t>(y) << 1;    
    for ( size_t i = 0; i < 32; i++ ) {
        if ( i % 2 == 0 ) {
            result |= ( x & index_x ) << i / 2;
            index_x <<= 1; 
            result |= ( ytmp & index_y ) << i / 2;
            index_y <<= 1; 
        }
    }
    double res = result / 4294967295.0;
    return res;
}   

int main( int argc, char **argv ) {

    if ( argc != 3 ) {
		std::cerr << "Wrong argument count." << std::endl;
        exit( 1 );
    }
    for ( size_t i = 0; argv[1][i]; i++ ) {
        if ( !isdigit(argv[1][i]) ) {
		    std::cerr << "Argument must be digit." << std::endl;
            exit ( 1 );
        }
    }
    for ( size_t i = 0; argv[2][i]; i++ ) {
        if ( !isdigit(argv[2][i]) ) {
		    std::cerr << "Argument must be digit." << std::endl;
            exit ( 1 );
        }
    }

    int x = std::atoi( argv[1] );
    int y = std::atoi( argv[2] );
    if ( x > 65535 || y > 65535) {
        std::cerr << "Out of range." << std::endl;
        exit ( 1 );
    }
    std::cout << map( static_cast<uint16_t>( x ), static_cast<uint16_t>( y )) << std::endl;
    return 0;
}