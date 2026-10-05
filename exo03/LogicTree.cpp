#include "LogicTree.hpp"
#include <cstdlib>
#include "LogicOperation.hpp"
#include <iostream>

LogicNode::LogicNode() : _right(NULL), _left(NULL), _isLeaf( false ), _symbole(0), _proposal(0), _negation(false) {}

LogicNode::~LogicNode() {
    if ( _left ) delete _left;
    if ( _right ) delete _right;
}

void LogicNode::initializeProposal( bool proposal ) {
    this->_proposal = proposal;
    this->_isLeaf   = true;
    this->_negation = false;
}

void LogicNode::initializeSymbole( char symbole ) {
    this->_symbole  = symbole;
    this->_isLeaf   = false;
    this->_negation = false;
}

void LogicNode::setBranches( LogicNode *right, LogicNode *left ) {
    _right = right;
    _left = left;
}

void LogicNode::setNegation() {
    this->_negation = true;
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

bool LogicNode::solve() const {
    
    if ( _isLeaf ) return _proposal;
    
    bool proposal = compute( _left->solve(), _right->solve() );
    if ( _negation ) proposal = proposal ? false : true;
    return proposal;
}