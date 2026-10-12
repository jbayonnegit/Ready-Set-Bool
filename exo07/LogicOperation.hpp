
template <typename T>
class Logic {

    public :

        static bool implication( T a, T b ) {
            // a => b
            if ( !a ) return true;
            else if ( a & b ) return true;
            else return false ;
        }

        static bool equivalence( T a, T b ) {
            // a => b & b => a
            if ( implication(a, b) & implication(b, a) ) return true;
            else return false;        
        }
};