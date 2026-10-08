#include <sstream>

class Variable {

	private :

		bool	_state;
		char	_name;

	public :

		Variable();
		Variable( bool state, char c );
		~Variable();
		Variable& operator=(const Variable& other);
		

		bool	getState() const;
		char	getName() const;

};

std::ostream& operator<<(std::ostream &flux, Variable const &variable);