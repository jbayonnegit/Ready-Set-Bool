#include <stack>
#include "Variable.hpp"
#include <iostream>
#include <vector>
#include <map>

class LogicNode {

	private : 
		
		LogicNode		*_right;
		LogicNode		*_left;
		LogicNode		*_connected_nodes;
		bool			_isLeaf;

		char			_symbole;
		unsigned int	_negation;
		bool			_proposal;
		Variable		*_variable;
		unsigned int	_nbBranches;

	public :

		LogicNode();
		LogicNode( const LogicNode & copy );
		LogicNode& operator=(const LogicNode& other);
		~LogicNode();

		void 				initializeProposal( bool proposal );
		void 				initializeSymbole( char symbole );
		LogicNode			*addBranches(  LogicNode *newNode, LogicNode *toAdd, size_t n );

		void 				setNegationMark();
		bool				isNeg() const ;
		void 				setIsLeaf();
		void 				setSymbolNegation();
		void 				setVariable( char c );

		bool 				compute( const bool a, const bool b ) const ;
		std::vector<int>	compute( std::vector<int> a, std::vector<int> b) const ;
		bool 				solve() const;
		std::vector<int>	solve( std::vector<int> result ) const;
		
		std::ostringstream *nodeToString(  std::ostringstream *stream ) const;
		std::string			treeToString() const;
		
		LogicNode			*tree_negation();
		void				reduce_tree();
		LogicNode			*transformEquivalence();
		LogicNode			*transformImplication();
		LogicNode			*transformTree();
		void				fillTreeWithSet( std::map< char, std::vector<int > > &sets_map );
		LogicNode			*negative_normal_form();
		void				conjunctive_normal_form();
		LogicNode   		*distribute( LogicNode *toDistribute, LogicNode *nodes, size_t n, char symbole ) ;


		static	LogicNode *createTree( std::string &formula ) {

			std::stack<LogicNode *> stack_nodes;

			for ( size_t i = 0; i < formula.size() ; i++ ){
				if (formula[i] >= 65 && formula[i] <= 90) {
					LogicNode *node = new LogicNode();
					node->setVariable( formula[i] );
					node->setIsLeaf();
					stack_nodes.push( node );
				} else {
					if ( formula[i] == '!' ) {
						stack_nodes.top()->setNegationMark();
						continue ;
					}
					LogicNode *newNode = new LogicNode();
					if ( stack_nodes.size() < 2 ) {
						std::cerr << "[error] Wrong expression format." << std::endl;
						exit( 1 );            
					}
					
					char symbole = formula[i];
					newNode->initializeSymbole( symbole );
					for ( size_t j = 0; j < 2; j++ ) {
						LogicNode *previousNode = stack_nodes.top(); 
						stack_nodes.pop();
						
						if ( previousNode->_symbole == symbole ) {
							newNode = newNode->addBranches( newNode, previousNode->_connected_nodes, previousNode->_nbBranches );
    						delete previousNode;
						}
						else {
							if ( !newNode->_connected_nodes )
							{
								newNode->_connected_nodes = new LogicNode[1];
								*newNode->_connected_nodes = LogicNode( *previousNode );
								newNode->_nbBranches += 1;
								delete previousNode;
								continue ;
							}
	
							LogicNode *old_nodes = newNode->_connected_nodes;
							size_t nb_branches = newNode->_nbBranches;
							newNode->_connected_nodes = new LogicNode[ newNode->_nbBranches + 1];
							size_t k = 0;
							while ( k < nb_branches ) 
							{
								newNode->_connected_nodes[k] = old_nodes[k];
								k++;
							}
							newNode->_connected_nodes[k] = LogicNode( *previousNode );
							newNode->_nbBranches += 1;
							delete previousNode;
							delete []old_nodes;
						}
					}

					stack_nodes.push ( newNode );
				}
			}
			if ( stack_nodes.size() != 1 ) {
				std::cerr << "[error] Wrong expression format." << std::endl;
				exit( 1 );
			}
			LogicNode *root = stack_nodes.top();
			return root;
		}
};