#include <iostream>
#include "math.h"
#include "RGB.h"

using namespace std;

RGB::RGB(){
	// Default constructor
	setRGB( 255 , 255, 255 );
}

RGB::RGB( int r, int g, int b ){
	// constructor with 3 values passed
	setRGB( r , g , b );
}

RGB::RGB( int val ){
	// constructor with one value passed
	setRGB( val, val, val );
}
	

RGB::RGB( const RGB& copy ){
	// copy constructor 
	int rd, gr, bl;
	copy.getRGB( rd, gr, bl );
	r = rd;
	g = gr;
	b = bl;
}

void RGB::setRGB(int rd, int gr, int bl ){
	// set the RGB values of the image
	// precondition - the values are 0 - 255
	r = rd;
	g = gr;
	b = bl;

	// check values
	r = ( r <= 255 ) ? r : 255;
	g = ( g <= 255 ) ? g : 255;
	b = ( b <= 255 ) ? b : 255;

	r = ( r >= 0 ) ? r : 0;
	g = ( g >= 0 ) ? g : 0;
	b = ( b >= 0 ) ? b : 0;

}

void RGB::getRGB( int& rd, int& gr, int& bl ) const{
	// get the RGB values
	rd = r;
	gr = g;
	bl = b;
}

//**************************************//
//		Assignment Operators	        //
//**************************************//  

RGB& RGB::operator = (const RGB& rhs ){
	// the overloaded assignment operator for 
	// other RGB objects
	int rd, gr, bl;
	if( this != &rhs ){
		rhs.getRGB( rd, gr, bl );
		r = rd;
		g = gr;
		b = bl;
	}
	return *this;
}

RGB& RGB::operator = (const int& rhs ){
	// the overloaded assignment operator for integer 
	// assignments
	// pre-condition, the value is a valid intensity 0 - 255 

	int val;

	// force bounds
	val = ( rhs <= 255 ) ? rhs : 255;
	val = ( rhs >= 0 ) ? rhs : 0;
	
	// assign values
	r = val;
	g = val;
	b = val;

	return *this;
}

//**************************************//
//		Addition Operators	            //
//**************************************//  
	

RGB& RGB::operator+=( const RGB& rhs ){
	// overloaded += operator
	int rd, gr, bl;
	rhs.getRGB( rd, gr, bl );
	
	// add values
	r += rd;
	g += gr;
	b += bl;

	return *this;
}

RGB RGB::operator + ( const RGB& rhs ){
	// overloaded addition operator
	// create a new RGB object with copy constructor
	// then use overloaded += to add them
	return RGB(*this) += rhs;
}

RGB& RGB::operator += (const int& rhs){
	// overloaded += for integers
	
	r += rhs;
	g += rhs;
	b += rhs;

	return *this;
}

RGB RGB::operator + (const int& rhs){
	// overloaded + for integers
	return RGB(*this) += rhs;
}


RGB& RGB::operator += (const double& rhs){
	// overloaded += for doubles
	
	r += (int)rhs;
	g += (int)rhs;
	b += (int)rhs;

	return *this;
}

RGB RGB::operator + (const double& rhs){
	// overloaded + for doubles
	return RGB(*this) += rhs;
}


//**************************************//
//		Subtraction Operators           //
//**************************************//  

RGB& RGB::operator -= ( const RGB& rhs ){
	// overloaded -= operator
	int rd, gr, bl;
	rhs.getRGB( rd, gr, bl );

	// subtract values
	r -= rd;
	g -= gr;
	b -= bl;

	return *this;
}

const RGB RGB::operator - ( const RGB& rhs ){
	// overloaded addition operator
	// works the same as +
	return RGB(*this) -= rhs;
}


//**************************************//
//		Division Operators              //
//**************************************//  

RGB& RGB::operator /= ( const int& val ){
	// division by an integer
    r = (int)(r / val);
	g = (int)(g / val);
	b = (int)(b / val);

	return *this;
}

const RGB RGB::operator / ( const int& val ){
	// * operator for ints
	return RGB(*this) /= val;
}


//**************************************//
//		Multiplication Operators        //
//**************************************//  


RGB& RGB::operator *= (const int& val ){
	// multiplication by an integer
	r *= val;
	g *= val;
	b *= val;

	return *this;
}

RGB RGB::operator * ( const int& val ){
	// * operator for ints
	return RGB(*this) *= val;
}

RGB& RGB::operator *= (const double& val ){
	// multiplication by a double
	r = (int)(r * val);
	g = (int)(g * val);
	b = (int)(b * val);

	return *this;
}

RGB RGB::operator * ( const double& val ){
	// * operator for double
	return RGB(*this) *= val;
}


ostream& operator << ( ostream& out, const RGB& pixel ){
	// overloaded << operator 
	int r, g, b;
	pixel.getRGB( r, g, b);

	out << "R: " << r << " ";
	out << "G: " << g << " ";
	out << "B: " << b << " ";

	return out;
}

