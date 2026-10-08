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

LogicNode   *LogicNode::addBranches(  LogicNode *newNode, LogicNode *previousNode ) {
	
    LogicNode *it_prev = previousNode->_connected_nodes;
	LogicNode *it_old = newNode->_connected_nodes;
    size_t     old_size = newNode->_nbBranches;

	newNode->_connected_nodes = new LogicNode[ previousNode->_nbBranches + old_size ];
	LogicNode *it_new = newNode->_connected_nodes;
					
    if ( it_old  ) {
	     size_t i = 0;
        for (; i < old_size; i++ ) {
	    	it_new[i] = LogicNode( it_old[i] );
	    }
        it_new += i;
    }
	for ( size_t i = 0; i < previousNode->_nbBranches; i++ ) {
		*it_new = LogicNode( *it_prev );
		it_new = it_new + 1; 
		it_prev++;
	}
	newNode->_nbBranches += previousNode->_nbBranches; 
    delete previousNode;
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

LogicNode   *LogicNode::conjunctive_normal_form() {
    return nullptr;
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