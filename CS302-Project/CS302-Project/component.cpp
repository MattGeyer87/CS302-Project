#include "stack.h"
#include "queue.h"
#include "RGB.h"
#include "pixel.h"
#include "image.h"
#include "slist.h"
#include "region.h"

using namespace std;


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

int computeComponents( ImageType<int>& input , ImageType<int>& output , slist<region>& rList ){
	// find the number of components in the image
	int numComps = 0, label = 0;
	int N, M, Q;
	
	ulist<pixel> *ulptr = NULL;
	region *rptr = NULL;
	pixel seed;
	input.getImageInfo( N, M, Q);
	output.setWhite();

	for( int i = 0; i < N; i++){
		for( int j = 0; j < M; j++ ){
			if( input.getPixelVal(i,j) == 255 && output.getPixelVal(i,j) == 255 ){
				numComps++;
				label = numComps;
				seed.setPixelVals( i , j );
				
				// instantiate new region
				rptr = new region;
				// create new pixel list
				ulptr = new ulist<pixel>;
				// set region list to the new list
				rptr->setPixelList( ulptr );
				// populate the pixel list and label the output image
				findComponentBFS( input, output, *ulptr, seed, label );
				//set the size of the region
				rptr->setSize( ulptr->getLength() );
				// threshold the region and insert
				if( rptr->getSize() > 5 )
					rList.insertItem( *rptr );
			}
		}
	}
	return numComps;
}
	

