#include <string>
#include "LogicTree.hpp"
#include <stack>
#include <iostream>

bool parseSymbol( char c ) {
	if ( c != '&' && c != '|' && c != '^' && c != '>' && c != '!' && c != '=' ) {
		std::cerr << "[error] Symbol : " << c << " unreconnized." << std::endl;
		return false;
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
			if ( !parseSymbol( formula[i] ) )
				exit( 1 );
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

int main( int argc, char **argv ){

	if ( argc != 2 ){
		std::cerr << "Wrong argument count." << std::endl;
		exit( 1 );
	}
	std::string formula( argv[1] );
	std::cout << "formula : " << eval_formula( formula ) << std::endl;
	return 0;
}