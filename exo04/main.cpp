#include <string>
#include "LogicTree.hpp"
#include "Sequence.hpp"
#include "Math.hpp"
#include <stack>
#include <iostream>
#include <sstream>
#include <cstring>

bool parseSymbol( char c ) {
	if ( c != '&' && c != '|' && c != '^' && c != '>' && c != '!' && c != '=' ) {
		return false;
	}
	return true;
}

bool parseFormula( std::string &formula ) {
	for ( size_t i = 0; i < formula.size(); i++ ) {
		if (!parseSymbol(formula[i]) && (formula[i] < 65 || formula[i] > 90)) {
			std::cerr << "[error] Symbol : " << formula[i] << " unreconnized." << std::endl;
			return false;
		}
	}
	return true;
}

bool eval_formula( std::string &formula ) {

	std::stack<LogicNode *> nodes;

	for ( size_t i = 0; i < formula.size() ; i++ ){


		if ( formula[i] == '1' || formula[i] == '0' ) {
			
			LogicNode *node = new LogicNode();

			bool proposal = formula[i] == '1' ? true : false;
			node->initializeProposal( proposal );
			nodes.push( node );
		} else {
			if ( formula[i] == '!' ) {
				nodes.top()->setNegation();
				continue ;
			}
			LogicNode *node = new LogicNode();
			if ( nodes.size() < 2 ) {
				std::cerr << "[error] Wrong expression format." << std::endl;
				exit( 1 );            
			}
			LogicNode *right = nodes.top(); 
			nodes.pop();
			LogicNode *left  = nodes.top(); 
			nodes.pop();

			char symbole = formula[i];
			node->initializeSymbole( symbole );
			node->setBranches( right, left );
			nodes.push ( node );
		}
	}
	if ( nodes.size() != 1 ) {
		std::cerr << "[error] Wrong expression format." << std::endl;
		exit( 1 );
	}
	LogicNode *root = nodes.top();
	bool result = root->solve();
	delete root; 
	return ( result );
}

void print_truth_table( std::string &formula ) {

	std::ostringstream stream;
	char buff[90];

	if (!parseFormula( formula )) exit( 1 );
	std::memset(&buff, 0, 90);

	for ( size_t i = 0; i < formula.size() ; i++ ) {
		if ( formula[i] >= 65 && formula[i] <= 90 && buff[static_cast<int>(formula.c_str()[i])] == 0 ){ 
			stream << formula[i];
			buff[ static_cast<int>(formula.c_str()[i]) ] = formula.c_str()[i];
		}; 
	}

	const std::string	variables_set( stream.str() );	
	const uint32_t 	table_size = Math::exponent(2, variables_set.size() );
	
	std::cout << "table size : " << table_size << std::endl;
	std::cout << "|";
	for ( size_t i = 0; i < variables_set.size(); i++ ) {
		std::cout << " " << variables_set[i] << " |";
	}
	std::cout << " = |" << std::endl;
	std::cout << "|";
	for ( size_t i = 0; i < variables_set.size() + 1; i++ ) {
		std::cout << "---|";
	}
	std::cout << std::endl; 
	for ( size_t i = 0; i < table_size; i++ ) {
		Sequence _sequence( Math::gray_code(i), variables_set );
		std::string binary_formula =  _sequence.to_binary_formula( formula );
		_sequence.result = eval_formula( binary_formula );
		_sequence.print();
	}
}

int main( int argc, char **argv ){

	if ( argc != 2 ){
		std::cerr << "Wrong argument count." << std::endl;
		exit( 1 );
	}
	std::string formula( argv[1] );
	print_truth_table( formula );
	return 0;
}