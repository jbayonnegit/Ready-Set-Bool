#include <sstream>
#include <vector>
class Variable {

	private :

		bool			 _state;
		char			 _name;
		std::vector<int> *_set;
	public :

		Variable();
		Variable( bool state, char c );
		Variable( const Variable &copy  );
		~Variable();
		Variable& operator=(const Variable& other);
		

		bool	getState() const;
		char	getName() const;
		void	setSet( std::vector<int> set );
		std::vector<int> *getSet() const;

};

std::ostream& operator<<(std::ostream &flux, Variable const &variable);