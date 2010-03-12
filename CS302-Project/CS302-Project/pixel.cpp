#include "pixel.h"
#include <iostream>
using namespace std;

pixel::pixel(){
	// defualt constructor
	i = j = 0;
}


pixel::pixel(int y, int x){
	i = y;
	j = x;
}


void pixel::getPixelVals( int& y, int& x) const{
	y = i;
	x = j;
}


void pixel::setPixelVals( int y, int x){
	i = y;
	j = x;
}

pixel& pixel::operator = ( const pixel& rhs ){
	// assignment operator
	int x, y;

	if( this != &rhs ){
		rhs.getPixelVals( y , x );
		i = y;
		j = x;

	}	
	return *this;
}


