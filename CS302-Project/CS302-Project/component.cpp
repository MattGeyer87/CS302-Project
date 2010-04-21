#include "stack.h"
#include "queue.h"
#include "RGB.h"
#include "pixel.h"
#include "image.h"
#include "slist.h"
#include "region.h"
#include <math.h>

using namespace std;


double moments( double p , double q , ulist<pixel>& rp ){
	// compute the moments of a region

	double moment = 0;
	int i, j;
	pixel pix;

	rp.resetList();
	
	// loop through every pixel in the region
	for( int k = 0; k < rp.getLength(); k++ ){
		// retrieve pixel from the list
		rp.getNextItem( pix );
		pix.getPixelVals( i , j );
		// sum for the moment
		moment += ( pow( i , p ) * pow( j , q ) );
	}
	return moment;
}

double centralMoments( double p , double q, double xbar , double ybar , ulist<pixel>& rp ){
	// compute central moments of a region once centroid has been determined

	double c_moment = 0;
	int i, j;
	pixel pix;

	rp.resetList();

	// loop through every pixel in the region
	for( int k = 0; k < rp.getLength(); k++ ){
		// retrieve pixel from the list
		rp.getNextItem( pix );
		pix.getPixelVals( i , j );
		// sum for the central moment
		c_moment += ( pow( i - xbar , p ) * pow( j - ybar, q ) );
	}
	return c_moment;
}

void computeRegionProperties( ImageType<int>& source , region& reg ){
	// compute the properties of a region

	const double PI = 3.14159265;

	double xbar, ybar, pmax , pmin;
	double orient, rad;

	int i = 0, j = 0, mean = 0, size = 0;
	
	pixel pix;
	ulist<pixel> *ulptr;
	region *rptr = NULL;

	rptr = &reg;

	ulptr = reg.getPixelList();

	// determine centroid
	xbar = floor( moments( 1 , 0 , *ulptr ) / moments( 0 , 0 , *ulptr ) );
	ybar = floor( moments( 0 , 1 , *ulptr ) / moments( 0 , 0 , *ulptr ) );

	// determine principal moments
	pmax = ( centralMoments( 2 , 0 , xbar , ybar , *ulptr) + centralMoments( 0 , 2 , xbar , ybar , *ulptr ) ) / 2;
	pmax += sqrt( pow( centralMoments( 2 , 0 , xbar , ybar , *ulptr ) , 2 ) +
	     	      pow( centralMoments( 0 , 2 , xbar , ybar , *ulptr ) , 2 ) - 
				  2 * ( centralMoments( 0 , 2 , xbar , ybar , *ulptr ) * centralMoments( 2 , 0 , xbar , ybar , *ulptr ) ) +
				  4 * ( pow( centralMoments( 1 , 1 , xbar , ybar , *ulptr ) , 2 ) ) );

	pmin = ( centralMoments( 2 , 0 , xbar , ybar , *ulptr) + centralMoments( 0 , 2 , xbar , ybar , *ulptr ) ) / 2;
	pmin -= sqrt( pow( centralMoments( 2 , 0 , xbar , ybar , *ulptr ) , 2 ) +
			      pow( centralMoments( 0 , 2 , xbar , ybar , *ulptr ) , 2 ) - 
     			  2 * ( centralMoments( 0 , 2 , xbar , ybar , *ulptr ) * centralMoments( 2 , 0 , xbar , ybar , *ulptr ) ) +
				  4 * ( pow( centralMoments( 1 , 1 , xbar , ybar , *ulptr ) , 2 ) ) );

			
	// determine orientation
	rad = atan2( pmax - centralMoments( 2 , 0 , xbar , ybar , *ulptr ) , centralMoments( 1 , 1 , xbar , ybar , *ulptr ) );

	// convert to degrees and set
	orient = ( rad * 180 ) / PI;
	rptr->setOrientation( orient );
					
	// set eccentricity
	rptr->setEccentricity( int( sqrt( pmax / pmin ) ) );

	// determine intensity
	ulptr->resetList();
	for( int k = 0; k < ulptr->getLength(); k++ ){
		ulptr->getNextItem( pix );
		pix.getPixelVals( i , j );
		mean += source.getPixelVal( i , j );
	}
    size = ulptr->getLength();
	
	if(size > 0 ){
		mean = int( mean / size );
		rptr->setIntensity( mean );
	}	

	return;
}

	

void findComponentBFS( ImageType<int>& input , ImageType<int>& output , ulist<pixel>& pixUL, pixel seed, int label ){
	// find the connected components of the image
	// using a queue

	int i, j, N, M, Q;
	input.getImageInfo( N , M, Q );

	queue<pixel> pixQ( N * M);

	pixQ.makeEmpty();

	pixQ.enqueue( seed );
	pixUL.insertItem( seed );

	while( !pixQ.isEmpty() ){
		pixQ.dequeue( seed );
		seed.getPixelVals( i , j);
		output.setPixelVal(i , j, label * 10);

		// start checking the 8 neighbors
			for( int a = -1; a < 2; a++ ){
				for( int b = -1; b < 2; b++ ){
					// make sure it's in bounds
					if( a == 0 && b == 0) continue;
					if( i + a >= 0 && i + a < N && j + b >= 0 && j + b < M ){
						if( input.getPixelVal(i + a, j + b) == 255 && output.getPixelVal( i + a, j + b ) == 255 ){
							output.setPixelVal(i + a , j + b, -1);
							seed.setPixelVals(i + a, j + b );
							// insert the pixel into the queue and pixel list
							if( !pixQ.isFull() ) pixQ.enqueue( seed );
							if( !pixUL.isFull() ) pixUL.insertItem( seed );
						}						
					}
				}
			}
	}
	return;
}

int computeComponents( ImageType<int>& input , ImageType<int>& output , ImageType<int>& src, slist<region>& rList ){
	// find the number of components in the image
	int numComps = 0, label = 0;
	int N, M, Q, k, p;
	
	ulist<pixel> *ulptr;
	region *rptr = new region;
	pixel seed;
	
	input.getImageInfo( N, M, Q);
	output.setWhite();

	rList.resetList();

	for( int i = 0; i < N; i++){
		for( int j = 0; j < M; j++ ){
			if( input.getPixelVal(i,j) == 255 && output.getPixelVal(i,j) == 255 ){
				numComps++;
				label = numComps;
				seed.setPixelVals( i , j );
				
		
				ulptr = rptr->getPixelList();
								
				// populate the pixel list and label the output image
				findComponentBFS( input, output, *ulptr, seed, label );
			

				//set the size of the region
				rptr->setSize( ulptr->getLength() );
				
				
				// threshold region and perform classification steps
				if( rptr->getSize() > 5 ){
					computeRegionProperties( src , *rptr );
					// insert into the list of regions
					rList.insertItem( *rptr );
				}

				ulptr->makeEmpty();

			}
		}
	}

	return numComps;
}
	


// Copies of the Region Search Function with Different Implementations //

void findComponentRecur( ImageType<int>& input , ImageType<int>& output,  pixel& seed, int& label ){
	// find the connected components of the image
	// using recursion

	int i, j, N, M, Q;
	input.getImageInfo(N, M, Q);

	// label the pixel that was passed to the function
	seed.getPixelVals( i , j);
	output.setPixelVal(i , j, label * 10);

	// loop through the pixel's neighbors
	for( int a = -1; a < 2; a++ ){
		for( int b = -1; b < 2; b++ ){
			
			// make sure we are checking a pixel that is in bounds
			// and we don't want to check the current pixel
			if( a == 0 && b == 0) continue;
			if( i + a >= 0 && i + a < N && j + b >= 0 && j + b < M ){
				// if there is an unmarked neighbor call the function recursively to mark it's neighbors
				if( input.getPixelVal(i + a, j + b) == 255 && output.getPixelVal( i + a, j + b ) == 255 ){
					seed.setPixelVals(i + a, j + b );
					findComponentRecur( input , output, seed, label );
				}						
			}
		}
	}

	return;
}

void findComponentDFS( ImageType<int>& input , ImageType<int>& output, pixel seed, int label ){
	// find the connected components of the image
	// using a stack

	int i, j, N, M, Q;
	input.getImageInfo(N, M, Q);
	stack<pixel> pixS( N * M );
	pixS.makeEmpty();

	pixS.push( seed );

	while( !pixS.isEmpty() ){
		pixS.pop( seed );
		seed.getPixelVals( i , j);
		output.setPixelVal(i , j, label * 10);

		// start checking the 8 neighbors
			for( int a = -1; a < 2; a++ ){
				for( int b = -1; b < 2; b++ ){
					// make sure it's in bounds
					if( a == 0 && b == 0) continue;
					if( i + a >= 0 && i + a < N && j + b >= 0 && j + b < M ){
						if( input.getPixelVal(i + a, j + b) == 255 && output.getPixelVal( i + a, j + b ) == 255 ){
							output.setPixelVal(i + a , j + b, -1);
							seed.setPixelVals(i + a, j + b );
							if( !pixS.isFull() ) pixS.push( seed );
						}						
					}
				}
			}
	}
}

