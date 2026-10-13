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

typedef struct s_point {
    uint16_t x;
    uint16_t y;
}               t_point;


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

t_point reverse_map( double n ) {

    uint32_t image = n * 4294967295.0;
    uint32_t index_x = 1;
    uint32_t index_y = 2;
    t_point p;

    uint32_t x = 0;
    uint32_t y = 0;

    for ( size_t i = 0; i < 32; i++ ) {
        x |= ( image & index_x ) >> i;
        index_x <<= 2; 
        y |= ( image & index_y ) >> i;
        index_y <<= 2; 
    
    }
    y >>= 1;
    p.x = x;
    p.y = y;
    std::cout << "x : " << p.x << " y : " << p.y << std::endl;
    return p;
}

int main( int argc, char **argv ) {

    if ( argc != 2 ) {
		std::cerr << "Wrong argument count." << std::endl;
        exit( 1 );
    }
    for ( size_t i = 0; argv[1][i]; i++ ) {
        if ( !isdigit(argv[1][i]) && argv[1][i] != '.') {
		    std::cerr << "Argument must be digit." << std::endl;
            exit ( 1 );
        }
    }


    double n = std::atof( argv[1] );
    if ( n > 1 || n < 0 ) {
        std::cerr << "Out of range." << std::endl;
        exit ( 1 );
    }
    reverse_map( n );
    return 0;
}