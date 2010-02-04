// Driver.cpp

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#include "image.h"


// Prototypes for Dr Bebis' functions
void readImageHeader(char[], int&, int&, int&, bool&);
void readImage(char[], ImageType&);
void writeImage(char[], ImageType&);

// Our function protoypes
int menu();
/*
void rastToCart(int, int, int&, int&);
void cartToPolar(int, int, double&, double&);
void polarToCart(double, double, int&, int&);
void cartToRast(int, int, int&, int&);
*/
void rotateImage( ImageType&, ImageType&, double);


int main(){
 
	int M, N, Q;
	bool type;
	int val, choice;	
	char *in, *out;
	in = new char[256];
	out = new char[256];
    ImageType *iptr = NULL;

    // Menu constants
	enum { LOAD, SAVE, ROTATE, INFO, GETVAL, SETVAL, SUBIMG,
			AVGGL, ENLG, SHRNK, QUIT}; 
	
	// load an initial image to be manipulated
	cout << endl << "Enter the filename of the image you want to load: ";
	cin >> in;
	// read image header
	readImageHeader(in, N, M, Q, type);
	// allocate memory for the image array
	iptr = new ImageType( N, M, Q );
	ImageType *img = new ImageType( N, M, Q);
	// read image
	readImage(in, *iptr);


	// Show user the menu
	choice = menu();

	// Main program loop.. Goes until the user chooses to quit
	while( choice != QUIT ){
		switch(choice){
			case LOAD:
				// load an image to be manipulated
				cout << endl << "Enter the filename of the image you want to load: ";
				cin >> in;
				// read image header
				readImageHeader(in, N, M, Q, type);
				// de-allocate memory from old image
				if( iptr ) delete iptr;
				// allocate memory for the image array
				iptr = new ImageType(N, M, Q);
				// read image
				readImage(in, *iptr);
				break;

			case SAVE:
				// save the image
				cout << endl << "Enter a filename to save your image: ";
				cin >> out;
				writeImage(out, *iptr);
				break;

			case ROTATE:
				// rotate an image by an angle
				rotateImage( *iptr, *img, 12 );
				writeImage( "outie.pgm" , *img );

			case INFO:
				// INFO logic
				break;

			case QUIT:
				// The user has chosen to end the program
				// Deallocate memory for the image
				if( iptr ) delete[] iptr;
				cout << endl << endl << "Goodbye!" << endl;
				system("PAUSE");
				break;

			default:
				cout << "That is not a valid choice!" << endl; 
				break;
		}
		choice = menu();
	}// End of main program loop

	return 0;
} // END MAIN

int menu(){
	// Present menu and return user choice
	int choice = 0;
	
	cout << endl << "Main Menu.. " << endl;
	cout << endl << endl;
	cout << "1 - Load a new image." << endl;
	cout << "2 - Save image to file." << endl;
	cout << "3 - Rotate Image." << endl;
	cout << "4 - Get a pixel value." << endl;
	cout << "5 - Set a pixel value." << endl;
	cout << "11 - Exit the program." << endl;
	cout << endl << "Enter your choice: ";
	cin >> choice;
	return choice - 1;
}// END MENU

void rotateImage( ImageType& source, ImageType& dest, double angle){
	// Rotate an image by an angle theta..
	// Matt

	int newVal;

	// cartesian and polar coords
	int x, y;
	double r, polarTheta = 0.0;
	const double PI = 3.1415926535;
	angle = (angle * PI) / 180;

    // get image dimensions
	int height, width, levels;
	source.getImageInfo( height, width, levels );
	
	
	for(int i = 0; i < height; i++ ){
		for( int j = 0; j < width; j++ ){
			dest.setPixelVal( i , j , 0 );
		}
	}
	
	// loop through all pixels in the destination image
	for( int i = 0; i < height; i++ ){
		for( int j = 0; j < width; j++ ){

			// convert from raster to cartesian
			x = j - width/2;
			y = height/2 - i;

			// next convert from cartesian to polar so that we can rotate
			r = sqrt((double)( x * x + y * y ));

			if( x == 0 ){
				if( y == 0 ){
					source.getPixelVal(j , i, newVal);
					dest.setPixelVal( j , i, newVal );
				}
				else if( y > 0 )
					polarTheta = 0.5 * PI;
				
				else 
					polarTheta = 1.5 * PI;
			}
			else
				polarTheta = atan2((double)y , (double)x );

			// rotate the pixel
			polarTheta -= angle;

			// convert back to cartesian
			x = (int)(floor(r * cos( polarTheta ) + 0.5 ));
			y = (int)(floor(r * sin( polarTheta ) + 0.5 ));

			// then back to raster
			x = x + width/2;
			y = height/2 - y;

			// check bounds
			if( x >= 0 && x < width && y >= 0 && y < height){			
				source.getPixelVal(y , x, newVal );
				dest.setPixelVal( i, j, newVal );
			}
		
		}
	}
	return;
}

