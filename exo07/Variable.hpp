#include <sstream>

class Variable {

	private :

		bool	_state;
		char	_name;

	public :

		Variable();
		Variable( const Variable &copy );
		Variable( bool state, char c );
		~Variable();
		

		bool	getState() const;
		char	getName() const;

};

std::ostream& operator<<(std::ostream &flux, Variable const &variable);