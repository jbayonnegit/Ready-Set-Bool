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

std::vector<char>	extractVariable( std::string formula ) {
	std::vector<char> var;

	for ( size_t i = 0; i < formula.size(); i++ ) {
		if ( (formula[i] >= 65 && formula[i] <= 90)  ) {
			bool include = false;
			for ( std::vector<char>::iterator it = var.begin(); it != var.end(); it++ ) {
				if ( *it == formula[i] )
					include = true;
			}
			if (!include) {
				var.push_back( formula[i] );
			}
		}
	}
	return var;
}

void	assignSetsToVariable( std::map< char, std::vector<int > > &sets_map, std::vector< std::vector<int> > sets, std::vector<char> var ) {
	for ( size_t i = 0; i < var.size(); i++ ) {
		sets_map[ var[i] ] = sets[i];
	}
}

std::vector<int>	create_universe( std::vector< std::vector<int> > sets ) {

	std::vector<int> universe;
	bool include;

	for ( size_t i = 0; i < sets.size(); i++ ) {
		for ( size_t j = 0; j < sets[i].size(); j++ ) {
			include = false;
			for ( std::vector<int>::iterator it = universe.begin(); it != universe.end(); it++ ) {

				if ( *it == sets[i][j] ) {
					include = true;
					break;
				}
			}
			if (!include) {
				universe.push_back(sets[i][j]);
			}
		}
	}

	return universe;
}

std::vector<int>	eval_set( std::string &formula, std::vector< std::vector<int> > sets ){
	if (!parseFormula( formula )) exit( 1 );
	
	std::vector<int>	result;
	std::vector<char> var = extractVariable( formula );
	std::map< char, std::vector<int > > sets_map;
	std::vector<int>	universe;

	if ( sets.size() != var.size()) {
		std::cerr << "[error] Incorrect number of set." << std::endl;	
		exit( 1 );
	}


	assignSetsToVariable( sets_map, sets, var );
	
	universe = create_universe( sets );
	LogicNode	*root;
	root = LogicNode::createTree( formula );
	root->fillTreeWithSet( sets_map, &universe );
	root->transformTree();
	root->negative_normal_form();
	root->reduce_tree();
	result = root->solve( result );
	delete root;
	return result;
}

int main( int argc, char **argv ){

	if ( argc < 2 ){
		std::cerr << "Wrong argument count." << std::endl;
		exit( 1 );
	}
	std::string formula( argv[1] );
	std::vector< std::vector<int> > sets;

	for ( int i = 2; i < argc; i++ ) {
		std::vector<int>	set;
		std::string str( argv[i] );
		for ( size_t i = 0; i < str.size(); i++ ) {
        if ( (str[i] < 48 || str[i] > 57 ) && str[i] != ' ') {
            std::cerr << "Wrong format." << std::endl;
            exit(1); 
        }
        if (str[i] >= 48 && str[i] <= 57 )
            set.push_back( std::atoi( &str[i] ));
			while (str[i] >= 48 && str[i] <= 57) i++;
    	}
		sets.push_back( set );
	}



	std::vector<int> Pset = eval_set( formula, sets );
    std::cout << "{";
    for ( size_t j = 0; j < Pset.size(); j ++ )
    {
        std::cout << Pset[j];
        if ( j != Pset.size() - 1 )
            std::cout << ",";
    }
    std::cout << "}" << std::endl;
    
	return 0;
}