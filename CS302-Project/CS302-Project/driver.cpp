// Driver.cpp

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>


using namespace std;

#include "image.h"


// Prototypes for Dr Bebis' functions
void readImageHeader(char[], int&, int&, int&, bool&);
void readImage(char[], ImageType&);
void writeImage(char[], ImageType&);

// Our function protoypes
int menu();



int main(){
 
	int M, N, Q;
	bool type;
	int choice;	
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
				iptr->rotate(120);
				writeImage( "outie2.pgm" , *iptr );

			case INFO:
				// INFO logic
				break;

			case AVGGL:
				// Print average gray level
				cout << endl << "The average gray level for your image is: ";
				cout << iptr->getMeanGray() << endl;
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
	cout << "8 - See Average Gray Level. " << endl;
	cout << "11 - Exit the program." << endl;
	cout << endl << "Enter your choice: ";
	cin >> choice;
	return choice - 1;
}// END MENU



