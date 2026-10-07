#include "LogicTree.hpp"
#include <cstdlib>
#include "LogicOperation.hpp"
#include <iostream>

LogicNode::LogicNode() : _right(NULL), _left(NULL), _isLeaf( false ), _symbole(0), _negation(0), _proposal(0), _variable( NULL ) {}

LogicNode::~LogicNode() {
    if ( _left ) delete _left;
    if ( _right ) delete _right;
    if ( _variable ) delete _variable;
}

		
LogicNode::LogicNode( const LogicNode & copy ) : _isLeaf( copy._isLeaf), _symbole( copy._symbole ), _negation( copy._negation ), _proposal( copy._proposal ) {

    if ( copy._right == NULL ) 
        _right = NULL;
    else 
        _right = new LogicNode( *copy._right );

    if (copy._left == NULL ) 
        _left = NULL;
    else 
        _left = new LogicNode( *copy._left );
    
    if (copy._variable == NULL ) 
        _variable = NULL;
    else 
        _variable = new Variable( *copy._variable );
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

void LogicNode::setBranches( LogicNode *right, LogicNode *left ) {
    _right = right;
    _left = left;
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
    _variable = new Variable( false, c );
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
            _left = _left->tree_negation();
            _right = _right->tree_negation();
            break ;
        }
        case '>' : {
            _symbole = '&';
            _right = _right->tree_negation();
            break;
        }
        case '|' : {
            _symbole = '&';
            _left = _left->tree_negation();
            _right = _right->tree_negation();
            break;
        }
        case '&' : {
            _symbole = '|';
            _left = _left->tree_negation();
            _right = _right->tree_negation();
            break;
        }
        case '=' : {
            _symbole = '&';
            _left = _left->tree_negation();
            _right = _right->tree_negation();
            break;
        }
    }
    return this;
}

LogicNode   *LogicNode::transformEquivalence() {

    LogicNode *newRight = new LogicNode();
    LogicNode *newLeft = new LogicNode();

    newRight->_right = this->_right;
    newRight->_left = this->_left;
    newRight->_symbole = '>';

    newLeft->_right = new LogicNode( *this->_left);
    newLeft->_left = new LogicNode( *this->_right );
    newLeft->_symbole = '>';

    this->_symbole = '&';
    this->_right = newRight;
    this->_left = newLeft;
    return this;
}

LogicNode   *LogicNode::transformImplication()  {
    
    _left = _left->tree_negation();
    this->_symbole = '|';
    return this;
}

LogicNode   *LogicNode::negative_normal_form() {

    if ( this->_isLeaf ) {
        return this;
    }
    if ( _symbole == '=') {
        this->transformEquivalence();
        _left->transformImplication();
        _right->transformImplication();
    }
    if ( _symbole == '>') this->transformImplication();
    if ( _negation & 1 && _symbole != 0 ) {
        this->_negation = 0;
        this->tree_negation();
    }
    _left->negative_normal_form();
    _right->negative_normal_form();
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
    _left->nodeToString( stream );
    _right->nodeToString( stream );
    *stream << this->_symbole;
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