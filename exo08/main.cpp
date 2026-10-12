#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

#include <vector>

std::vector< std::vector<int> > powerset(std::vector<int> A) {
    std::vector< std::vector<int> > powerSet;
    powerSet.push_back(std::vector<int>()); 

    for (size_t i = 0; i < A.size(); i++) {
        size_t current_size = powerSet.size();        
        for (size_t j = 0; j < current_size; j++) {
            std::vector<int> newSubset = powerSet[j];
            newSubset.push_back(A[i]);
            powerSet.push_back(newSubset);
        }
    }

    return powerSet;
}

int main(int argc, char **argv) {
    
    if ( argc != 2 ) exit(1);
    std::string str( argv[1] );

    std::vector< int > set;
    for ( size_t i = 0; i < str.size(); i++ ) {
        if ( (str[i] < 48 || str[i] > 57 ) && str[i] != ' ') {
            std::cerr << "Wrong format." << std::endl;
            exit(1); 
        }
        if (str[i] >= 48 && str[i] <= 57 )
            set.push_back( std::atoi( &str[i] ));
    }
    std::vector< std::vector<int> > Pset = powerset( set );
    std::cout << "P=(";
    for ( size_t i = 0; i < Pset.size(); i++ ) {
        std::cout << "{";
        for ( size_t j = 0; j < Pset[i].size(); j ++ )
        {
            std::cout << Pset[i][j];
            if ( j != Pset[i].size() - 1 )
                std::cout << ",";
        }
        std::cout << "}";
        if ( i != Pset.size() - 1)
            std::cout << ",";
    }
    std::cout << ")" << std::endl;
}