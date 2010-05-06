
typedef struct vectorstruct
{
	double x;
	double y;
	double z;
}vector;

vector &add( vector &out, const vector &a, const vector &b );
vector &subtract( vector &out, const vector &a, const vector &b );
vector &cross( vector &out, const vector &a, const vector &b );
vector &scale( vector &out, double n );
vector &normalize( vector &out, double n );
double scalarproduct( const vector &a, const vector &b);