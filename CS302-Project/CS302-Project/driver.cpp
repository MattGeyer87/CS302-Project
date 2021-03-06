// Driver.cpp

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "RGB.h"
#include "pixel.h"
#include <time.h>
#include "slist.h"
#include "region.h"
#include "ulist.h"

using namespace std;

void readImageHeader(char[], int&, int&, int&, bool&);
void readImage(char[], ImageType<int>&);
void writeImage(char[], ImageType<int>&);
void readImage(char[], ImageType<RGB>&);
void writeImage(char[], ImageType<RGB>&);
int computeComponents( ImageType<int>&, ImageType<int>& , ImageType<int>&, slist<region>& );
void computeRegionProperties( ImageType<int>& , region& );
void classifyRegions( ImageType<int>& , ImageType<int>& , slist<region>& );
RGB abs( RGB );
RGB floor( RGB );

int menu();


int main(){
 
	int M, N, Q, i, j, newVal;
	int ulx, uly, brx, bry, comps;
	double rot;
	bool type, demo;
	int choice;	
	long timer;

	char *c_in, *g_in, *c_img_in, *g_img_in, *out, *in;
	in = new char[256];
	c_in = new char[256];
	out = new char[256];
    c_img_in = new char[256];
	g_in = new char[256];
	g_img_in = new char[256];
    ImageType<RGB> *c_iptr = NULL,
		           *c_img = NULL;
				   //*iptr = NULL,
				   //*img = NULL;
	ImageType<int> *g_iptr = NULL,
		           *g_img = NULL,
				   *iptr = NULL,
				   *img = NULL,
				   *src = NULL,
				   *classout = NULL;

	ulist<pixel> *ul, *ul2;
	slist<region> *regions = new slist<region>;
	region *t_reg, *b_reg;
	pixel *pix, *pix2;

	// Menu constants
	enum { LOAD, SAVE, ROTATEB, ROTATE, INFO, GETVAL, SETVAL, SUBIMG,
			AVGGL, ENLG, SHRNK, ADD, DIFF, NEG, RFLCT, TRANS, AUTOTHRESH, 
			THRESH, DILATE, ERODE, CONCOMP, CCTT, QUIT}; 

	//cout << "Would you like to run in Demo mode? Enter 1 - yes , 2 - no: ";
	//cin >> choice;
   
	
	//if( choice == 1 )
    //		demo = true;
    //else
		demo = false;

	if( demo ){
		// load an initial image to be manipulated
		cout << endl << "Enter the filename of the first color image you want to load: ";
		cin >> c_in;

		// read image header
		readImageHeader(c_in, N, M, Q, type);

		// allocate memory for the image array
		c_iptr = new ImageType<RGB>( N, M, Q );

		// read image
		readImage(c_in, *c_iptr);

		cout << endl << "Enter the filename of the second color image you want to load: ";
		cin >> c_img_in;
		readImageHeader(c_img_in, N, M, Q, type);
		c_img = new ImageType<RGB>( N, M, Q );
		readImage(c_img_in, *c_img);

		// again for the grayscale image
		cout << endl << "Enter the filename of the first grayscale image you want to load: ";
		cin >> g_in;
		readImageHeader(g_in, N, M, Q, type);
		g_iptr = new ImageType<int>( N, M, Q );
		readImage(g_in, *g_iptr);

		cout << endl << "Enter the filename of the second grayscale image you want to load: ";
		cin >> g_img_in;
		readImageHeader(g_img_in, N, M, Q, type);
		g_img = new ImageType<int>( N, M, Q );
		readImage(g_img_in, *g_img);
	}
	else{
		cout << endl << "Enter the filename of the image you want to load: ";
		cin >> in;
		// read image header
		readImageHeader(in, N, M, Q, type);
		// de-allocate memory from old image
		if( iptr ) delete iptr;
		if( src ) delete src;
		// allocate memory for the image array
		iptr = new ImageType<int>( N , M , Q );
		src = new ImageType<int>( N , M , Q );
		classout = new ImageType<int>( N , M , Q );
		classout->setWhite();
		// read image
		readImage(in, *iptr);
		*src = *iptr;
	
	}

	// Show user the menu
	choice = menu();
	
	// Main program loop.. Goes until the user chooses to quit
	while( choice != QUIT ){
		switch(choice){
			
			case LOAD:
				system("cls");
				// load an image to be manipulated
				cout << endl << "Enter the filename of the image you want to load: ";
				cin >> in;
				// read image header
				readImageHeader(in, N, M, Q, type);
				// de-allocate memory from old image
				if( iptr ) delete iptr;
				if( src ) delete src;
				if( classout ) delete classout;
				// allocate memory for the image array
				iptr = new ImageType<int>(N, M, Q);
				src = new ImageType<int>( N , M , Q );
				classout = new ImageType<int>( N , M , Q );
				classout->setWhite();
				// read image
				readImage(in, *iptr);
				*src = *iptr;
				
				break;

			case SAVE:
				// save the image
				system("cls");
				cout << endl << "Enter a filename to save your image: ";
				cin >> out;
				writeImage(out, *iptr);
				break;
		
				
			case ROTATEB:
				// rotate an image by an angle.. with bilinear interpolation
				system("cls");
				cout << "Enter the degree value to rotate by, expressed as a float: ";
				cin >> rot;
				if(demo){
					c_iptr->rotateBilinear(rot);
					g_iptr->rotateBilinear(rot);
				}
				else
					iptr->rotateBilinear(rot);
				break;
			
			case ROTATE:
				// rotate an image by an angle
				system("cls");
				cout << "Enter the degree value to rotate by, expressed as a float: ";
				cin >> rot;
				if(demo){
					c_iptr->rotate(rot);
					g_iptr->rotate(rot);
				}
				else
					iptr->rotate(rot);
				break;
			
			
			case INFO:
				// INFO logic
				system("cls");
				if( demo ){
					c_iptr->getImageInfo( N , M , Q );
					cout << endl << "The first image is " << M << " x " << N << " pixels and has, ";
					cout << Q << " levels. " << endl;

					g_iptr->getImageInfo( N , M , Q );
					cout << endl << "The second image is " << M << " x " << N << " pixels and has, ";
					cout << Q << " levels. " << endl;
				}
				else{
					iptr->getImageInfo( N , M , Q );
					cout << endl << "This image is " << M << " x " << N << " pixels and has, ";
					cout << Q << " levels. " << endl;
					cout << endl;
				}
				system("PAUSE");
				break;

			case GETVAL:
				// get the value of a particular pixel
				system("cls");
				if( demo )
					c_iptr->getImageInfo( N , M , Q );
				else
					iptr->getImageInfo( N , M , Q );

				cout << endl << "Please enter the coordinates of desired pixel separated by a space.. " << endl;
				cout << "Note that values must be less than " << M << " and " << N << " repsectively: ";
				cin >> j >> i;
				// check bounds
				i = ( i < N && i >= 0 ) ? i : 0;
				j = ( j < M && j >= 0 ) ? j : 0;
				if( demo ){
					cout << "The value at that pixel in the first is: " << c_iptr->getPixelVal( i , j ) << endl;
					cout << "The value at that pixel in the second is: " << g_iptr->getPixelVal( i , j ) << endl;
				}
				else
					cout << "The value at that pixel is: " << iptr->getPixelVal( i , j ) << endl;
				system("PAUSE");
				break;
		
			case SETVAL:
				// Set the value of a particular pixel
				system("cls");
				if( demo )
					c_iptr->getImageInfo( N , M , Q );
				else
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
				if( demo ){
					RGB val = newVal;
					c_iptr->setPixelVal(i,j, val );
					g_iptr->setPixelVal(i,j, newVal);
				}
				else
					iptr->setPixelVal( i , j, newVal );
				break;
			
			case SUBIMG:
				// subimg logic
				system("cls");
				if( demo )
					c_iptr->getImageInfo( N , M , Q );
				else
					iptr->getImageInfo( N , M , Q );
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
				if( demo ){
					c_iptr->subImg( ulx, uly, brx, bry );
					g_iptr->subImg( ulx, uly, brx, bry );
				}
				else
					iptr->subImg( ulx, uly, brx, bry );
				
				break;

			case AVGGL:
				// Print average gray level
				system("cls");
				cout << endl << "The average color level for your images are: ";
				if( demo ){
					cout << "Color: " << c_iptr->meanValue() << endl;
					cout << "Graylevel: " << g_iptr->meanValue() << endl;
				}
				else
					cout << iptr->meanValue() << endl << endl;
				system("PAUSE");
				break;
				
			
			case ENLG:
				system("cls");
				// enlarge an image by a factor S
				cout << "Enter the positive integer factor by which you'd like to enlarge: ";
				cin >> i;
				// make sure it's positive
				i = ( i > 0 ) ? i : 0;
				if( demo ){
					c_iptr->enlarge( i );
					g_iptr->enlarge( i );
				}
				else
					iptr->enlarge( i );
				break;
			
			case SHRNK:
				// shrink an image by a factor s
				system("cls");
				cout << "Enter the positive integer factor by which you'd like to shrink: ";
				cin >> i;
				// make sure it's positive
				i = ( i > 0 ) ? i : 0;
				if( demo ){
					c_iptr->shrink( i );
					g_iptr->shrink( i );
				}
				else
					iptr->shrink( i );
				break;
			
			case ADD:
				system("cls");
				if( demo ){
					*c_iptr = *c_iptr + *c_img;
					*g_iptr = *g_iptr + *g_img;
				}
				else{
					// add two images together
					// load an image to add
					cout << endl << "Enter the filename of the image you want to add: ";
					cin >> in;
					// read image header
					readImageHeader(in, N, M, Q, type);
					// allocate memory for the image array
					if( img ) delete img;
					img = new ImageType<int>( N, M, Q);
					// read image
					readImage(in, *img);				
					*iptr = *iptr + *img;
				}
				break;

			
			case DIFF:
				system("cls");
				if( demo ){
					*c_iptr = *c_iptr - *c_img;
					*g_iptr = *g_iptr - *g_img;
				}
				else{
					// compute the difference between two images
					cout << endl << "Enter the filename of the image you want to subtract: ";
					cin >> in;
					readImageHeader( in, N, M, Q, type);
					if( img ) delete img;
					img = new ImageType<int>( N, M, Q);
					readImage(in, *img);
					*iptr = *iptr - *img;
				}
				break;
	
			case NEG:
				// negate an image
				system("cls");
				if( demo ){
					c_iptr->negate();
					g_iptr->negate();
				}
				else
					iptr->negate();
				break;

			case RFLCT: 
				// reflect an image
				system("cls");
				cout << endl << "Enter 1 to reflect along the horizon, or 2 to reflect vertically: ";
				cin >> i;
				while( i > 2 || i < 0 ){
					cout << endl << "Not a valid option. Please enter again: ";
					cin >> i;
				}
				if( i == 1 )
					if( demo ){
						c_iptr->reflectH();
						g_iptr->reflectH();
					}
					else
						iptr->reflectH();
				else{
					if( demo ){
						c_iptr->reflectV();
						g_iptr->reflectV();
					}
					else
						iptr->reflectV();
				}
				break;

			case TRANS:
				// translate an img
				system("cls");
				if( demo )
					c_iptr->getImageInfo( N , M , Q );
				else
					iptr->getImageInfo( N , M , Q );
				cout << endl << "Enter the translation offsets for the x and y separated by a space. ";
				cout << "Values must be less than " << M << " and " << N << " respectively: ";
				cin >> j >> i;
				while( i > N || j > M || i < 0 || j < 0 ){
					cout << endl << "Those values are out of bounds. Enter again: ";
					cin >> j >> i;
				}
				if( demo ){
					c_iptr->translate(j , i );
					g_iptr->translate(j , i );
				}
				else
					iptr->translate(j , i );
				break;		
			
			case AUTOTHRESH:
				system("cls");
				iptr->AutoThreshold( 128 );
				break;
			
			case THRESH:
				system("cls");
				iptr->threshold( 128 );
				break;

			case DILATE:
				system("cls");
				iptr->dilate();
				break;

			case ERODE:
				system("cls");
				iptr->erode();
				break;

			case CONCOMP:
				system("cls");
				regions->makeEmpty();
				iptr->getImageInfo( N , M, Q );
				if ( img ) delete img;
				img = new ImageType<int>( N , M , Q );
				timer = clock();
				comps = computeComponents( *iptr , *img , *src,  *regions);
				cout <<  (double)(( clock() - timer ) / CLOCKS_PER_SEC) << endl;
				*iptr = *img;
				cout << "Number of components: " << comps << endl << endl;
				cout << "Number of components in the list: " << regions->getLength() << endl;
				system("PAUSE");
				break;

			case CCTT:
				system("cls");
				cout << "************** Connected Components *************" << endl << endl;
				regions->makeEmpty();
				iptr->getImageInfo( N , M , Q );
				if( img ) delete img;
				img = new ImageType<int>(N , M , Q );
				iptr->AutoThreshold(100);
				iptr->dilate();
				iptr->erode();
				iptr->erode();
				comps = computeComponents(*iptr , *img , *src , *regions);
				*iptr = *img;
				cout << "Number of connected components in the list: " << regions->getLength() << endl;
				
				// once the list is created the properties of each region in the list 
				// are computed so the regions can be classified
				
				cout << endl << endl;

				t_reg = new region;
				regions->resetList();
				cout << endl << endl;

				
				for( int k = 0; k < regions->getLength(); k++ ){
					if(! regions->isLastItem() ){
						regions->getNextItem( *t_reg );
					}
					
					// print summary to the screen
					cout << "Region: " << k + 1 << endl;
					cout << "Region Size: " << t_reg->getSize() << endl;
					cout << "Orientation: " << t_reg->getOrientation() << " degrees. " << endl;
					cout << "Eccentricity: " << t_reg->getEccentricity() << endl;
					cout << "Intensity: " << t_reg->getIntensity() << endl;
					cout << endl << endl;
				}

				// allow the user to classify the regions they want to see
				classifyRegions( *classout , *src , *regions );
				break;
		
			default:
				system("cls");
				cout << "That is not a valid choice!" << endl; 
				break;
		}
	
		if( demo ){
			writeImage( "demoColor.ppm" , *c_iptr );
			writeImage( "demoGray.pgm" , *g_iptr );
		}
		else
			writeImage( "default.pgm", *iptr );

		system("cls");
		choice = menu();
	}// End of main program loop

	// The user has chosen to end the program
	// Deallocate memory for the image
	if( iptr ) delete iptr;
	if( img ) delete img;
	system("cls");
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
	cout << "17 - Auto-Threshold." << endl;
	cout << "18 - Threshold." << endl;
	cout << "19 - Dilate. " << endl;
	cout << "20 - Erode." << endl;
	cout << "21 - Connected Components." << endl;
	cout << "22 - Multiple Step Compute Components." << endl;
	cout << "23 - Exit the program." << endl;
	cout << endl << "Enter your choice: ";
	cin >> choice;
	cout << endl;
	return choice - 1;
}// END MENU

void classifyRegions( ImageType<int>& in , ImageType<int>& src , slist<region>& rList ){
	// give the user options to view specific regions

	slist<region> trList;
	ulist<pixel> *tpList;
	region *t_reg = NULL;
	pixel pix;
	int a = 0,
		b = 0,
		i = 0,
		j = 0,
		flag = 0;

	int choice = 0;
	enum { SIZE , ORIENT , ECCENT , INTNSTY , QUIT };

	do{
		cout << endl << endl;
		cout << "Classify Region Menu: " << endl;
		cout << "1 - Regions by Size." << endl;
		cout << "2 - Regions by orientation." << endl;
		cout << "3 - Regions by eccentricity." << endl;
		cout << "4 - Regions by intensity." << endl;
		cout << "5 - Return to main menu." << endl;
		cout << endl << "Enter your choice: ";
		cin >> choice;

		switch(choice - 1){
			case SIZE:
				// threshold by size
				cout << "Enter two sizes to threshold between separated by a space: ";
				cin >> a >> b;
				flag = SIZE;
				break;
			case ORIENT:
				cout << "Enter two orientations to threshold between separated by a space: ";
				cin >> a >> b;
				flag = ORIENT;
				break;
			case ECCENT:
				cout << "Enter two eccentricities to threshold between separated by a space: ";
				cin >> a >> b;
				flag = ECCENT;
				break;
			case INTNSTY:
				cout << "Enter two intesnsities to threshold between separated by a space: ";
				cin >> a >> b;
				flag = INTNSTY;
				break;
			case QUIT:
				return;
				break;
			default:
				break;
		}
		
		rList.resetList();
		trList.makeEmpty();
		in.setWhite();

		t_reg = new region;

		// iterate through the region list and add
		// qualified regions to the temp region list
		while( !rList.isLastItem() ){
			rList.getNextItem( *t_reg );
			switch (flag){
				case SIZE:
					if( t_reg->getSize() > a && t_reg->getSize() < b ) trList.insertItem( *t_reg );
					break;
				case ORIENT:
					if( t_reg->getOrientation() > a && t_reg->getOrientation() < b ) trList.insertItem( *t_reg );
					break;
				case ECCENT:
					if( t_reg->getEccentricity() > a && t_reg->getEccentricity() < b ) trList.insertItem( *t_reg );
					break;
				case INTNSTY:
					if( t_reg->getIntensity() > a && t_reg->getIntensity() < b ) trList.insertItem( *t_reg );
					break;
			}
		}

		trList.resetList();
		rList.resetList();
		// iterate through the temp region list and 
		// add pixels to the output img

		cout << endl << "There are " << trList.getLength() << " regions that match that criterion." << endl;
		
		while( !trList.isLastItem() ){
			trList.getNextItem( *t_reg );
			tpList = t_reg->getPixelList();
			tpList->resetList();
			while( !tpList->isLastItem() ){
				tpList->getNextItem( pix );
				pix.getPixelVals( i , j );
				in.setPixelVal( i , j , src.getPixelVal( i , j ) );
			}
		}
		system("PAUSE");
		
		system("cls");

		writeImage( "cregion.pgm" , in );
	}while( (choice - 1) != QUIT);

	return;
}



RGB abs( RGB pixel ){
	// overloaded abs function
	int rd, gr, bl;
	pixel.getRGB( rd, gr, bl );
	
	rd = abs(rd);
	bl = abs(bl);
	gr = abs(gr);

	pixel.setRGB( rd, gr, bl );

	return pixel;
}

RGB floor( RGB pixel ){
	// overloaded floor function
	int rd, gr, bl;
	pixel.getRGB( rd, gr, bl );

	rd = (int)floor( (double) rd );
	bl = (int)floor( (double) bl );
	gr = (int)floor( (double) gr );
	
	pixel.setRGB( rd, gr, bl );

	return pixel;
}


