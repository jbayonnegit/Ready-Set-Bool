#include "Sequence.hpp"

Sequence::Sequence(){}

Sequence::Sequence( uint32_t gray_code, std::string variables_set ) {
    uint32_t index = 1;
    for ( size_t i = 0; i < variables_set.size(); i++ ) {
        bool state = index & gray_code;
        Variable variable = Variable( state ,variables_set[i]);
        gray_code >>= 1;
		this->push_back( variable );
    }
}

std::string Sequence::to_binary_formula( const std::string &formula ) {

    std::ostringstream stream;

    for ( size_t i = 0; i < formula.size(); i++ ) {
        if ( formula[i] >= 65 && formula[i] <= 90 ) {
            for ( typename std::vector<Variable>::const_iterator it = this->begin(); it != this->end(); it++ ) {
                if ( formula[i] == it->getName() ) {
                    stream << *it;
                    break;
                }
            }
        }
        else {
            stream << formula[i];
        }
    }
    std::string binary_formula( stream.str() );
    return binary_formula;
}

void    Sequence::print() const {
    std::cout << "|";
    for ( typename std::vector<Variable>::const_iterator it = this->begin(); it != this->end(); it++ )
        std::cout << " " << *it << " |";
    std::cout << " " << result << " |" << std::endl;
}