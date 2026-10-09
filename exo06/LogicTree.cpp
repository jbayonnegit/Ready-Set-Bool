#include "LogicTree.hpp"
#include <cstdlib>
#include "LogicOperation.hpp"
#include <iostream>

LogicNode::LogicNode() : _connected_nodes( NULL ) , _isLeaf( false ), _symbole(0), _negation(0), _proposal(0), _variable( NULL ), _nbBranches(0) {}

LogicNode::~LogicNode() {
	if ( this->_connected_nodes ) delete []this->_connected_nodes;
	if ( _variable ) delete _variable;
}

	
LogicNode::LogicNode( const LogicNode & copy ) : _isLeaf( copy._isLeaf), _symbole( copy._symbole ), _negation( copy._negation ), _proposal( copy._proposal ), _nbBranches( copy._nbBranches) {

	if ( copy._connected_nodes == NULL ) 
		_connected_nodes = NULL;
	else {
		_connected_nodes = new LogicNode[ copy._nbBranches ];

	
		for ( size_t i = 0; i < copy._nbBranches; i++ ) {
			_connected_nodes[i] = copy._connected_nodes[i];
		}

	}
	if (copy._variable == NULL ) 
		_variable = NULL;
	else {
		_variable = new Variable( copy._variable->getState(), copy._variable->getName() );
	}
}


LogicNode& LogicNode::operator=(const LogicNode& other) {

	_isLeaf = other._isLeaf;
	_symbole = other._symbole;
	_negation = other._negation;
	_proposal = other._proposal;

	if (other._variable == NULL ) 
		_variable = NULL;
	else {
		_variable = new Variable( other._variable->getState(), other._variable->getName() );
	}    
	_nbBranches = other._nbBranches;

	if ( other._connected_nodes == NULL ) 
		_connected_nodes = NULL;
	else {
		_connected_nodes = new LogicNode[ other._nbBranches ];
		for ( size_t i = 0; i < other._nbBranches; i++ ) {
			_connected_nodes[i] = other._connected_nodes[i];
		}
	}
	return *this;
}

void LogicNode::initializeProposal( bool proposal ) {
	this->_proposal = proposal;
	this->_isLeaf   = true;
	this->_negation = 0;
}

void LogicNode::initializeSymbole( char symbole ) {
	this->_symbole  = symbole;
	this->_isLeaf   = false;
	this->_negation = 0;
}

LogicNode   *LogicNode::addBranches(  LogicNode *newNode, LogicNode *toAdd, size_t n ) {
	
	LogicNode *it_prev = toAdd;
	LogicNode *it_old = newNode->_connected_nodes;
	size_t     old_size = newNode->_nbBranches;

	newNode->_connected_nodes = new LogicNode[ n + old_size ];
	LogicNode *it_new = newNode->_connected_nodes;
					
	if ( it_old  ) {
		 size_t i = 0;
		for (; i < old_size; i++ ) {
			it_new[i] = LogicNode( it_old[i] );
		}
		it_new += i;
	}
	for ( size_t i = 0; i < n; i++ ) {
		*it_new = LogicNode( *it_prev );
		it_new = it_new + 1; 
		it_prev++;
	}
	newNode->_nbBranches += n; 
	delete []it_old;
	return newNode;
}

void LogicNode::setNegationMark() {
	this->_negation += 1;
}

void LogicNode::setIsLeaf() {
	this->_isLeaf = true;
}

bool LogicNode::compute( const bool a, const bool b) const {
	
	switch ( _symbole ) {

		case '^' :
			return a ^ b;
		case '>' :
			return Logic<bool>::implication(a , b);
		case '|' :
			return a | b;
		case '&' :
			return a & b;
		case '=' :
			return Logic<bool>::equivalence(a, b);
	}
	return false;
}

void LogicNode::setVariable( char c ) {
	_variable = new Variable(false, c);
}

bool LogicNode::solve() const {
	
	if ( _isLeaf ) {
		bool proposal = _proposal;
		if ( _negation & 1 ) proposal = _proposal ? false : true;
		return proposal;
	}
	
	bool proposal = compute( _left->solve(), _right->solve() );
	if ( _negation & 1 ) proposal = proposal ? false : true;
	return proposal;
}

LogicNode *LogicNode::tree_negation() {

	if ( _isLeaf ) {
		this->setNegationMark();
		return this;
	}

	switch ( _symbole ) {

		case '^' : {
			_symbole = '&';
			LogicNode   *it = this->_connected_nodes;
			for ( size_t i = 0; i < _nbBranches; i++ ) {
				it->tree_negation();
				it++;
			}
			break ;
		}
		case '>' : {
			_symbole = '&';
			LogicNode   *it = this->_connected_nodes;
			it = it + 1;
			it->tree_negation();
			break;
		}
		case '|' : {
			_symbole = '&';
			LogicNode   *it = this->_connected_nodes;
			for ( size_t i = 0; i < _nbBranches; i++ ) {
				it->tree_negation();
				it++;
			}
			break;
		}
		case '&' : {
			_symbole = '|';
			LogicNode   *it = this->_connected_nodes;
			for ( size_t i = 0; i < _nbBranches; i++ ) {
				it->tree_negation();
				it++;
			}
			break;
		}
		case '=' : {
			_symbole = '&';
			LogicNode   *it = this->_connected_nodes;
			for ( size_t i = 0; i < _nbBranches; i++ ) {
				it->tree_negation();
				it++;
			}
			break;
		}
	}
	return this;
}

LogicNode   *LogicNode::transformEquivalence() {

	LogicNode *newRight = new LogicNode();

	newRight->_connected_nodes = new LogicNode[2];
	LogicNode *connected_node_right_tmp = newRight->_connected_nodes;

	connected_node_right_tmp = this->_left;
	connected_node_right_tmp = connected_node_right_tmp + 1;
	connected_node_right_tmp = this->_right;
	newRight->_symbole = '>';

	LogicNode *newLeft = new LogicNode();

	newLeft->_connected_nodes = new LogicNode[2];
	LogicNode *connected_node_left_tmp = newLeft->_connected_nodes;

	connected_node_left_tmp = new LogicNode( *this->_right );
	connected_node_left_tmp = connected_node_left_tmp + 1;
	connected_node_left_tmp = new LogicNode( *this->_left);
	newLeft->_symbole = '>';

	this->_symbole = '&';
	this->_right = newRight;
	this->_left = newLeft;
	return this;
}

LogicNode   *LogicNode::transformImplication()  {
	
	this->_connected_nodes = this->_connected_nodes->tree_negation();
	this->_symbole = '|';
	return this;
}

LogicNode   *LogicNode::negative_normal_form() {

	if ( this->_isLeaf ) {
		return this;
	}
	if ( _negation & 1 && _symbole != 0 ) {
		this->_negation = 0;
		this->tree_negation();
	}
	LogicNode   *it = this->_connected_nodes;
	for ( size_t i = 0; i < _nbBranches; i++ ) {
		it->negative_normal_form();
		it++;
	}
	return this;
}

LogicNode   *LogicNode::distribute( LogicNode *toDistribute, LogicNode *nodes, size_t n, char symbole ) {

	LogicNode   *newNode = new LogicNode();
	newNode->_symbole = symbole;
	newNode = newNode->addBranches( newNode, toDistribute, 1);
	newNode = newNode->addBranches( newNode, nodes, n);
	return newNode;
}

void    LogicNode::conjunctive_normal_form() {
	
	if ( _isLeaf ) {
		return ;
	}

	std::stack<LogicNode *> connected_nodes_distributed;

	for ( size_t i = 0; i < _nbBranches ; i++ ) {

		LogicNode   *child = &_connected_nodes[i];

		// Do i need to distribute ?
		if ( child->_symbole == '&' ) {

			// yes, what have I to distribute ?
			LogicNode   *to_distribute = child->_connected_nodes;
			// Ok, now how many ?

			for ( size_t k = 0; k < child->_nbBranches; k++ ) {
				// on what ?

				for ( size_t j = 0; j < _nbBranches ; j++ )
				{
					// not on me
					if ( i != j ) {

						// distribute on child of my sisters
						LogicNode *distributeOn;
						size_t  n;
						if ( _connected_nodes[j]._isLeaf ) {
							distributeOn = &_connected_nodes[j];
							n = 1;
						}
						else {
							distributeOn = _connected_nodes[j]._connected_nodes;
							n = _connected_nodes[j]._nbBranches;   
						}
						LogicNode *distributed = distribute( &to_distribute[k], distributeOn, n,'|' );
						connected_nodes_distributed.push( distributed );
					}
				}
			}
		}
	}
	if ( connected_nodes_distributed.size() == 0 ) {
		for ( size_t i = 0; i < _nbBranches; i++ ) {
			_connected_nodes[i].conjunctive_normal_form();
		}        
		return ;
	}
	_symbole = '&';
	_nbBranches = connected_nodes_distributed.size();
	LogicNode *_newConnected = new LogicNode[ connected_nodes_distributed.size() ];
	for ( size_t i = 0; i < _nbBranches ; i++ ) {
		LogicNode *distributed = connected_nodes_distributed.top(); 
		connected_nodes_distributed.pop();
		_newConnected[i] = LogicNode(*distributed);
		delete distributed;
	}
	delete []_connected_nodes;
	_connected_nodes = _newConnected;

	for ( size_t i = 0; i < _nbBranches; i++ ) {
		_connected_nodes[i].conjunctive_normal_form();
	}
}

LogicNode   *LogicNode::transformTree() {

	if ( this->_isLeaf ) {
		return this;
	}
	if ( _symbole == '=') this->transformEquivalence();
	if ( _symbole == '>') this->transformImplication();

	LogicNode   *it = this->_connected_nodes;
	for ( size_t i = 0; i < _nbBranches; i++ ) {
		it->transformTree();
		it++;
	}
	return this;
}

void	LogicNode::reduce_tree() {

	if ( _isLeaf ) {
		return;
	}
	std::vector<LogicNode *> _newConnecetd_vector;

	for ( size_t i = 0; i < _nbBranches; i++ ) {

		if ( _connected_nodes[i]._symbole == this->_symbole ) {
			LogicNode	*newConnectedNodes = _connected_nodes[i]._connected_nodes;
			for ( size_t j = 0; j < _connected_nodes[i]._nbBranches; j++ ) {
				LogicNode *node = new LogicNode( newConnectedNodes[j] );
				_newConnecetd_vector.push_back( node );
			}
		}
		else {
			LogicNode *node = new LogicNode( _connected_nodes[i] );
			_newConnecetd_vector.push_back( node );
		}
	}
	delete []_connected_nodes;
	_connected_nodes = new LogicNode[ _newConnecetd_vector.size() ];
	_nbBranches = _newConnecetd_vector.size();
	for ( size_t i = 0; i < _newConnecetd_vector.size(); i++ ) {
		LogicNode *node = _newConnecetd_vector[i];
		_connected_nodes[i] = *node;
		delete node; 
	}
	for ( size_t i = 0; i < _nbBranches; i++ ) {
		_connected_nodes[i].reduce_tree();
	}
	return ;
}


std::ostringstream *LogicNode::nodeToString( std::ostringstream *stream ) const {

	if ( stream == NULL )
		stream = new std::ostringstream();
	if ( _isLeaf ) {
		*stream << _variable->getName();
	if ( _negation & 1 ) *stream << '!';
		return stream;
	}
	LogicNode   *it = this->_connected_nodes;
	it += _nbBranches - 1;
	for ( size_t i = _nbBranches; i > 0; i-- ) {
		it->nodeToString( stream );
		it--;
	}
	if ( _nbBranches != 0 ) {
		for ( size_t i = 0; i < _nbBranches - 1; i++ ) {
			*stream << this->_symbole;
		}
	}
	if ( _negation & 1 ) *stream << '!';
	return stream;
}

std::string LogicNode::treeToString() const {
	
	std::string formula;
	std::ostringstream * stream = NULL;
	
	stream = this->nodeToString( stream );
	formula = stream->str();
	delete stream;
	return formula;
}