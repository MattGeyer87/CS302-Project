#include <iostream>
using namespace std;

#ifndef RGB_H
#define RGB_H

class RGB{
public:
	RGB();
	RGB( const RGB& );
	RGB( int, int, int );
	RGB( int );
	RGB& operator = (const RGB& );
	RGB& operator = (const int& );

	RGB& operator += (const RGB& );
	RGB& operator += (const int&);
	RGB& operator += (const double&);
	RGB operator + (const RGB& );	
	RGB operator + (const int&);
	RGB operator + (const double&);
	
	RGB& operator -= (const RGB& );
	const RGB operator - (const RGB& );
	
	RGB& operator /= (const int& );
	const RGB operator / (const int& );
	
	RGB& operator *= (const int& );
	RGB& operator *= (const double& );
	RGB operator * (const int& );
	RGB operator * (const double& );
	
	void setRGB(int,int,int);
	void getRGB(int&,int&,int&) const;
	friend ostream& operator<<(ostream& out, const RGB& pixel);
private:
	int r, g, b;


};

#endif