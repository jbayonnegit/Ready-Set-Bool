
class LogicNode {

	private : 
		
		LogicNode				*_right;
		LogicNode				*_left;
		bool					_isLeaf;
			
		char					_symbole;
		bool					_proposal;
		unsigned int			_negation;

	public :

		LogicNode();
		~LogicNode();

		void initializeProposal( bool proposal );
		void initializeSymbole( char symbole );
		void setBranches( LogicNode *right, LogicNode *left );
		void setNegation();
		
		bool compute( const bool a, const bool b ) const ;
		bool solve() const;
};