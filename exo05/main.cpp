#include <string>
#include "LogicTree.hpp"
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

std::string	negation_normal_form( std::string &formula ){
	if (!parseFormula( formula )) exit( 1 );

	std::string	negative_form;

	LogicNode	*root = LogicNode::createTree( formula );
	root->transformTree();
	root = root->negative_normal_form();
	negative_form = root->treeToString();
	return negative_form;
}

int main( int argc, char **argv ){

	if ( argc != 2 ){
		std::cerr << "Wrong argument count." << std::endl;
		exit( 1 );
	}
	std::string formula( argv[1] );
	std::cout << negation_normal_form( formula ) << std::endl;
	return 0;
}
