#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <iostream>
#include "Variable.hpp"

class Sequence : public std::vector<Variable> {

	public :

		Sequence();
		Sequence( uint32_t gray_code, const std::string variable_set );

		bool result;

		void		print() const;
		const std::vector<Variable>   order() const;
		std::string	to_binary_formula( const std::string &formula );
};