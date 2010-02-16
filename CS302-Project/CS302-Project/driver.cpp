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
	double rot;
	bool type;
	int choice;	
	char *in, *out;
	in = new char[256];
	out = new char[256];
    ImageType *iptr = NULL,
		      *img = NULL;

    // Menu constants
	enum { LOAD, SAVE, ROTATEB, ROTATE, INFO, GETVAL, SETVAL, SUBIMG,
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

			case ROTATEB:
				// rotate an image by an angle.. with bilinear interpolation
				cout << "Enter the degree value to rotate by, expressed as a float: ";
				cin >> rot;
				iptr->rotateBilinear(rot);
				break;

			case ROTATE:
				// rotate an image by an angle
				cout << "Enter the degree value to rotate by, expressed as a float: ";
				cin >> rot;
				iptr->rotate(rot);
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
					cin >> brx >> bry;
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
				cout << "Enter the positive integer factor by which you'd like to enlarge: ";
				cin >> i;
				// make sure it's positive
				i = ( i > 0 ) ? i : 0;
				iptr->enlarge( i );
				break;

			case SHRNK:
				// shrink an image by a factor s
				cout << "Enter the positive integer factor by which you'd like to shrink: ";
				cin >> i;
				// make sure it's positive
				i = ( i > 0 ) ? i : 0;
				iptr->shrink( i );
				break;

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
				*iptr = *iptr + *img;
				break;

			case DIFF:
				// compute the difference between two images
				cout << endl << "Enter the filename of the image you want to subtract: ";
				cin >> in;
				readImageHeader( in, N, M, Q, type);
				if( img ) delete img;
				img = new ImageType( N, M, Q);
				readImage(in, *img);
				*iptr = *iptr - *img;
				break;

			case NEG:
				// negate an image
				iptr->negate();
				break;

			case RFLCT: 
				// reflect an image
				cout << endl << "Enter 1 to reflect along the horizon, or 2 to reflect vertically: ";
				cin >> i;
				while( i > 2 || i < 0 ){
					cout << endl << "Not a valid option. Please enter again: ";
					cin >> i;
				}
				if( i == 1 )
					iptr->reflectH();
				else
					iptr->reflectV();
				break;

			case TRANS:
				// translate an img
				iptr->getImageInfo(N , M, Q);
				cout << endl << "Enter the translation offsets for the x and y separated by a space. ";
				cout << "Values must be less than " << M << " and " << N << " respectively: ";
				cin >> j >> i;
				while( i > N || j > M || i < 0 || j < 0 ){
					cout << endl << "Those values are out of bounds. Enter again: ";
					cin >> j >> i;
				}
				iptr->translate(j , i );
				break;
		
			default:
				cout << "That is not a valid choice!" << endl; 
				break;
		}
		choice = menu();
	}// End of main program loop

	// The user has chosen to end the program
	// Deallocate memory for the image
	if( iptr ) delete iptr;
	if( img ) delete img;
	cout << endl << endl << "Goodbye!" << endl << endl << endl;
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
	cout << "3 - Rotate Image w/ Bilinear Interpolation." << endl;
	cout << "4 - Rotate an Image." << endl;
	cout << "5 - Get Image info. " << endl;
	cout << "6 - Get a pixel value." << endl;
	cout << "7 - Set a pixel value." << endl;
	cout << "8 - Get a sub image. " << endl;
	cout << "9 - See Average Gray Level. " << endl;
	cout << "10 - Enlarge an image." << endl;
	cout << "11 - Shrink an image." << endl;
	cout << "12 - Add two images together. " << endl;
	cout << "13 - Compute Difference of two Images. " << endl;
	cout << "14 - Negate an Image." << endl;
	cout << "15 - Reflect Image." << endl;
	cout << "16 - Translate Image." << endl;
	cout << "17 - Exit the program." << endl;
	cout << endl << "Enter your choice: ";
	cin >> choice;
	cout << endl;
	return choice - 1;
}// END MENU



