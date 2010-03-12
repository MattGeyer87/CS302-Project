#include "stack.h"
#include "queue.h"
#include "RGB.h"
#include "pixel.h"
#include "image.h"

using namespace std;


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

void findComponentBFS( ImageType<int>& input , ImageType<int>& output , pixel seed, int label ){
	// find the connected components of the image
	// using a queue

	int i, j, N, M, Q;
	input.getImageInfo( N , M, Q );

	queue<pixel> pixQ( N * M);

	pixQ.makeEmpty();

	pixQ.enqueue(seed);

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
							if( !pixQ.isFull() ) pixQ.enqueue( seed );
						}						
					}
				}
			}
	}
	return;
}

int computeComponents( ImageType<int>& input , ImageType<int>& output ){
	// find the number of components in the image
	int numComps = 0, label = 0;
	int N, M, Q;

	pixel seed;

	input.getImageInfo( N, M, Q);
	
	output.setWhite();

	for( int i = 0; i < N; i++){
		for( int j = 0; j < M; j++ ){
			if( input.getPixelVal(i,j) == 255 && output.getPixelVal(i,j) == 255 ){
				numComps++;
				label = numComps;
				seed.setPixelVals( i , j );
				findComponentDFS( input , output, seed, label );
				findComponentBFS( input, output, seed, label );
			}
		}
	}
	return numComps;
}
	

