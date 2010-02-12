// Driver.cpp

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"

using namespace std;

void readImageHeader(char[], int&, int&, int&, bool&);
void readImage(char[], ImageType&);
void writeImage(char[], ImageType&);
int menu();


int main(){
 
	int M, N, Q, i, j, newVal;
	int ulx, uly, brx, bry;
	bool type;
	int choice;	
	char *in, *out;
	in = new char[256];
	out = new char[256];
    ImageType *iptr = NULL,
		      *img = NULL;

    // Menu constants
	enum { LOAD, SAVE, ROTATE, INFO, GETVAL, SETVAL, SUBIMG,
			AVGGL, ENLG, SHRNK, ADD, DIFF, NEG, RFLCT, TRANS, QUIT}; 
	
	// load an initial image to be manipulated
	cout << endl << "Enter the filename of the image you want to load: ";
	cin >> in;

	// read image header
	readImageHeader(in, N, M, Q, type);

	// allocate memory for the image array
	iptr = new ImageType( N, M, Q );

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
				//iptr->rotate(45);
				//writeImage("pN.pgm" , *iptr);
				iptr->rotateBilinear(45);
				writeImage( "pI.pgm" , *iptr );
				break;
			
			case INFO:
				// INFO logic
				iptr->getImageInfo( N , M , Q );
				cout << endl << "This image is " << M << " x " << N << " pixels and has, ";
				cout << Q << " levels of gray. " << endl;
				break;

			case GETVAL:
				// get the value of a particular pixel
				iptr->getImageInfo( N , M , Q );
				cout << endl << "Please enter the coordinates of desired pixel separated by a space.. " << endl;
				cout << "Note that values must be less than " << M << " and " << N << " repsectively: ";
				cin >> j >> i;
				// check bounds
				i = ( i < N && i >= 0 ) ? i : 0;
				j = ( j < M && j >= 0 ) ? j : 0;
				cout << "The value at that pixel is: " << iptr->getPixelVal( i , j ) << endl;
				break;

			case SETVAL:
				// Set the value of a particular pixel
				iptr->getImageInfo( N , M , Q );
				cout << endl << "Please enter the coordinates of your pixel and the " << endl;
				cout << "desired value separated by a space (ie 0 0 255.." << endl;
				cout << "Note that values must be less than " << M << " , " << N << " and " << Q + 1 << " respectively : ";
				cin >> j >> i >> newVal;
				// check bounds
				i = ( i < N && i >= 0 ) ? i : 0;
				j = ( j < M && j >= 0 ) ? j : 0;
				newVal = ( newVal <= Q && newVal >= 0 ) ? newVal : Q;
				// set new pixel value
				iptr->setPixelVal( i , j, newVal );
				break;

			case SUBIMG:
				// subimg logic
				iptr->getImageInfo( N , M, Q );
				cout << "Enter the top left x and y coordinates separated by ";
				cout << "a space: ";
				cin >> ulx >> uly;
				while( ulx < 0 || uly < 0 || ulx > M || uly > N ){
					cout << "Those values are out of bounds.. enter again: ";
					cin >> ulx >> uly;
				}

				cout << "Enter the bottom right x and y coordinates separated by ";
				cout << "a space: ";
				cin >> brx >> bry;
				while( brx < 0 || bry < 0 || brx > M || bry > N || bry < uly || brx < ulx){
					cout << "Those values are out of bounds.. enter again: ";
					cin >> ulx >> uly;
				}

				iptr->subImg( ulx, uly, brx, bry );
				
				break;

			case AVGGL:
				// Print average gray level
				cout << endl << "The average gray level for your image is: ";
				cout << iptr->getMeanGray() << endl;
				break;

			
			case ENLG:
				// enlarge an image by a factor S
				iptr->enlarge( 5 );
				writeImage( "enI.pgm" , *iptr );
				break;

			case SHRNK:
				// shrink an image by a factor s
				iptr->shrink( 2 );
				writeImage("shrink.pgm", *iptr);

			case ADD:
				// add two images together
				// load an image to add
				cout << endl << "Enter the filename of the image you want to add: ";
				cin >> in;
				// read image header
				readImageHeader(in, N, M, Q, type);
				// allocate memory for the image array
				if( img ) delete img;
				img = new ImageType( N, M, Q);
				// read image
				readImage(in, *img);				
				// I think this might be a memory leak..?
				*iptr = *iptr + *img;
				writeImage( "added.pgm" , *iptr );
				break;

			case DIFF:
				// compute the difference between two images

			case NEG:
				// negate an image
				iptr->negate();
				writeImage( "negated.pgm" , *iptr );
				break;

			case RFLCT: 
				// reflect an image
				break;

			case TRANS:
				// translate an img
				break;
		
			default:
				cout << "That is not a valid choice!" << endl; 
				break;
		}
		choice = menu();
	}// End of main program loop

	// The user has chosen to end the program
	// Deallocate memory for the image
	if( iptr ) delete[] iptr;
	if( img ) delete[] img;
	cout << endl << endl << "Goodbye!" << endl;
	system("PAUSE");

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
	cout << "4 - Get Image info. " << endl;
	cout << "5 - Get a pixel value." << endl;
	cout << "6 - Set a pixel value." << endl;
	cout << "7 - Get a sub image. " << endl;
	cout << "8 - See Average Gray Level. " << endl;
	cout << "9 - Enlarge an image." << endl;
	cout << "10 - Shrink an image." << endl;
	cout << "11 - Add two images together. " << endl;
	cout << "12 - Compute Difference of two Images. " << endl;
	cout << "13 - Negate an Image." << endl;
	cout << "14 - Reflect Image." << endl;
	cout << "15 - Translate Image." << endl;
	cout << "16 - Exit the program." << endl;
	cout << endl << "Enter your choice: ";
	cin >> choice;
	cout << endl;
	return choice - 1;
}// END MENU



