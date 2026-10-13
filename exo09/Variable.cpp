#include "Variable.hpp"
#include <iostream>
Variable::Variable() : _state( false ), _name(0), _set( NULL ) {}
Variable::Variable( bool state ,char c ) : _state( state ), _name( c ), _set(NULL) {
}
Variable::Variable( const Variable &copy ) {
    this->_name = copy.getName();
    this->_state = copy.getState();
    if ( copy.getSet() )
        this->_set = new std::vector<int>( *copy.getSet() );
    else
        this->_set = NULL;
}

Variable::~Variable(){
    if ( _set )
        delete _set;
}

bool Variable::getState() const {
    return _state;
}

char Variable::getName() const {
    return _name;
}

Variable& Variable::operator=(const Variable& other) {
    if ( this == &other ) return *this;
    _state = other._state;
    _name = other._name;
    _set = new std::vector<int>( *other._set );
    return *this;
}

void	Variable::setSet( std::vector<int> set ) {
    _set = new std::vector<int>( set );
    return ;
}

std::vector<int> *Variable::getSet() const {
    return _set;
}


std::ostream& operator<<(std::ostream &flux, Variable const &variable) {
    flux << variable.getState();
    return flux;
}
