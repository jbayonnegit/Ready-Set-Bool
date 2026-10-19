#include <stack>
#include "Variable.hpp"
#include <iostream>

class LogicNode {

	private : 
		
		LogicNode		*_right;
		LogicNode		*_left;
		bool			_isLeaf;

		char			_symbole;
		unsigned int	_negation;
		bool			_proposal;
		Variable		*_variable;

	public :

		LogicNode();
		LogicNode( const LogicNode & copy );
		~LogicNode();

		void 				initializeProposal( bool proposal );
		void 				initializeSymbole( char symbole );
		void 				setBranches( LogicNode *right, LogicNode *left );

		void 				setNegationMark();
		void 				setIsLeaf();
		void 				setSymbolNegation();
		void 				setVariable( char c );

		bool 				compute( const bool a, const bool b ) const ;
		bool 				solve() const;
		
		std::ostringstream *nodeToString(  std::ostringstream *stream ) const;
		std::string			treeToString() const;
		
		LogicNode			*tree_negation();
		
		LogicNode			*transformEquivalence();
		LogicNode			*transformImplication();

		LogicNode   		*transformExclusiveDisjunction();
		LogicNode			*transformTree();
		LogicNode			*negative_normal_form();

		static	LogicNode *createTree( std::string &formula ) {

			std::stack<LogicNode *> nodes;

			for ( size_t i = 0; i < formula.size() ; i++ ){
				if (formula[i] >= 65 && formula[i] <= 90) {
					LogicNode *node = new LogicNode();
					node->setVariable( formula[i] );
					node->setIsLeaf();
					nodes.push( node );
				} else {
					if ( formula[i] == '!' ) {
						nodes.top()->setNegationMark();
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
					node->setBranches( left, right );
					
					nodes.push ( node );
				}
			}
			if ( nodes.size() != 1 ) {
				std::cerr << "[error] Wrong expression format." << std::endl;
				exit( 1 );
			}
			LogicNode *root = nodes.top();
			return root;
		}
};
