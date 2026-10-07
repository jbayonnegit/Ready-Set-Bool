#include "Variable.hpp"

Variable::Variable() : _state( false ), _name(0) {}
Variable::Variable( const Variable &copy ) : _state( copy._state ), _name( copy._name ) {}
Variable::Variable( bool state ,char c ) : _state( state ), _name( c ) {}
Variable::~Variable(){}

bool Variable::getState() const {
    return _state;
}

char Variable::getName() const {
    return _name;
}


std::ostream& operator<<(std::ostream &flux, Variable const &variable) {
    flux << variable.getState();
    return flux;
}
