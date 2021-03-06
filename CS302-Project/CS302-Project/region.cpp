#include "region.h"

region::region(){
	// default constructor
	size = eccentricity = intensity = 0;
	orientation = 0;
	pixels = new ulist<pixel>;
}

region::region( const region& r ){
	// copy constructor
	pixel p;
	size = r.size;
	eccentricity = r.eccentricity;
	orientation = r.orientation;
	intensity = r.intensity;
	r.pixels->resetList();
	pixels->makeEmpty();
	while( !r.pixels->isLastItem() ){
		r.pixels->getNextItem( p );
		pixels->insertItem( p );
	}
}

region::~region(){
	// destructor
	pixels->makeEmpty();
}

//**********************//
//       Setters        //
//**********************//

void region::setSize( int s ){
	size = s;
}

void region::setEccentricity( int e ){
	
	e = ( e < 0 )? 0 : e;
	eccentricity = e;
}

void region::setIntensity( int i ){
	intensity = i;
}

void region::setOrientation( double o ){
	orientation = o;
}

void region::setPixelList( ulist<pixel>* ul ){
	pixels = ul;
}


//**********************//
//		Getters			//
//**********************//

int region::getSize(){
	// return the size of a region
	return size;
}

void region::getSize( int& s ){
	// return by reference getSize
	s = size;
}

int region::getEccentricity(){
	// return a region's eccentricity
	return eccentricity;
}

void region::getEccentricity( int& e ){
	// return by reference getEccentricty
	e = eccentricity;
}

int region::getIntensity(){
	// return intensity
	return intensity;
}

void region::getIntensity( int& i ){
	// pass by ref intensity
	i = intensity;
}

double region::getOrientation(){
	// return orientation
	return orientation;
}

void region::getOrientation( double& o ){
	o = orientation;
}

ulist<pixel>* region::getPixelList(){
	// return a pointer to the region's list of pixels
	return pixels;
}

//**********************//
// Overloaded Operators //
//**********************//

region& region::operator = ( const region& rhs ){
	//overloaded assignment operator
	if( this != &rhs ){
		pixel p;
		size = rhs.size;
		eccentricity = rhs.eccentricity;
		orientation = rhs.orientation;
		intensity = rhs.intensity;
		rhs.pixels->resetList();
		pixels->makeEmpty();
		while( !rhs.pixels->isLastItem() ){
			rhs.pixels->getNextItem( p );
			pixels->insertItem( p );
		}	
	}
	return *this;
}

bool region::operator < ( region& rhs ){
	// overloaded less than operator
	return( size < rhs.getSize() );
}

bool region::operator > ( region& rhs ){
	// overloaded greater than operator
	return( size > rhs.getSize() );
}

bool region::operator >= ( region& rhs ){
	// overloaded greater than or equal to
	return( size >= rhs.getSize() );
}

bool region::operator <= ( region& rhs ){
	// overloaded less than equal to
	return( size <= rhs.getSize() );
}

