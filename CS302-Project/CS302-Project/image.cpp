#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "RGB.h"

using namespace std;

#ifdef IMAGE_H

template<class PT>
ImageType<PT>::ImageType()
{
 N = 0;
 M = 0;
 Q = 0;

 pixelValue = NULL;
 temp = NULL;
}

template<class PT>
ImageType<PT>::ImageType(int tmpN, int tmpM, int tmpQ)
{
 int i, j;

 N = tmpN;
 M = tmpM;
 Q = tmpQ;

 pixelValue = new PT* [N];
 temp = new PT* [N];
 for(i=0; i<N; i++) {
   pixelValue[i] = new PT[M];
   temp[i] = new PT[M];
   for(j=0; j<M; j++){
     pixelValue[i][j] = 0;
	 temp[i][j] = 0;
   }
 }
}

template<class PT>
ImageType<PT>::~ImageType(){
	// ImageType Destructor
	// Matt
		
	for( int i = 0; i < N; i++ ){
		delete[] pixelValue[i];
		delete[] temp[i];
	}
	delete[] pixelValue;
	delete[] temp;
}


template<class PT>
void ImageType<PT>::getImageInfo(int& rows, int& cols, int& levels)
{
 rows = N;
 cols = M;
 levels = Q;
} 

template<class PT>
void ImageType<PT>::setImageInfo(int rows, int cols, int levels)
{
 N= rows;
 M= cols;
 Q= levels;
} 

template<class PT>
void ImageType<PT>::setPixelVal(int i, int j, PT val)
{
 pixelValue[i][j] = val;
}

template<class PT>
void ImageType<PT>::getPixelVal(int i, int j, PT& val)
{
 val = pixelValue[i][j];
}

template<class PT>
PT ImageType<PT>::getPixelVal(int i, int j)
{
 return  pixelValue[i][j]; 
}

template<class PT>
PT ImageType<PT>::meanValue(){
	// compute the mean (avg) pixel value of the image
	// Matt

	int numPixels = N * M;
	PT totalGray = 0;
	
	// loop through all pixels
	for( int i = 0; i < N; i++)
		for( int j = 0; j < M; j++ )
			totalGray += pixelValue[i][j];
	
	// divide the sum of all gray values by total pixels
	return(totalGray / numPixels );
}

template<class PT>
void ImageType<PT>::clearTemp(){
	// set all the values in the temp array to black
	// Matt

    for(int i = 0; i < N; i++)
		for(int j = 0; j < M; j++)
			temp[i][j] = 0;
}

template<class PT>
void ImageType<PT>::clearPV(){
	// set all the values in the pixel array to black
	// Matt

	for( int i = 0; i < N; i++ )
		for( int j = 0; j < M; j++)
			pixelValue[i][j] = 0;
}
template<class PT>
void ImageType<PT>::tempToPV(){
	// store all the values in the temp array to the
	// pixelValue (main) array
	// Matt

	for( int i = 0; i < N; i++)
		for( int j = 0; j < M; j++)
			pixelValue[i][j] = temp[i][j];
}

template<class PT>
void ImageType<PT>::rotate( double angle){
	// Rotate an image by an angle theta..
	// Matt

	// cartesian and polar coords
	int x, y;
	double r, polarTheta = 0.0;
	const double PI = 3.1415926535;
	angle = (angle * PI) / 180;

    // get image dimensions
	int height = N, width = M;	
	
	clearTemp();

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
					// center of the image so rotation isn't necessary
					temp[i][j] = pixelValue[i][j];
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
				temp[i][j] = pixelValue[y][x];
			}
		
		}
	}
	// clear the main array, copy temp to it and black out the temp array
	clearPV();
	tempToPV();
	clearTemp();
	return;
}

template<class PT>
void ImageType<PT>::rotateBilinear( double angle ){
	// Rotate an image by an angle theta.. bilinear 
	// interpolation implemented..
	// Matt

	// cartesian and polar coords
	double x, y;
	double r, polarTheta = 0.0;
	const double PI = 3.1415926535;
	angle = (angle * PI) / 180;

	// interpolation variables
	int xU, yU, xL, yL;
	double tX, tY;
	PT lowerGray, upperGray, newGray;

	// get image dimensions
	int height = N, width = M;
	
	clearTemp();

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
					// center of the image so rotation isn't necessary
					temp[i][j] = pixelValue[i][j];
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
			x = r * cos( polarTheta );
			y = r * sin( polarTheta );

			// then back to raster
			x = x + width/2;
			y = height/2 - y;

			// calculate lower and upper bounds for both x and y
			xL = (int)floor( x );
			yL = (int)floor( y );
			xU = (int)ceil( x );
			yU = (int)ceil( y );

			// find the value t for linear interpolation
			// f(x) = (1 - t)*Po(value) + t*P1(value)
			// in both x and y directions
			tX = x - xL;
			tY = y - yL;		

			// make sure pixels are in bounds
			if( xL >= 0 && xU >= 0 && xL < width && xU < width &&
				yL >= 0 && yU >= 0 && yL < height && yU < height){			
				
					// interpolate top two pixels on x axis
					upperGray = pixelValue[yL][xL] * (1 - tX) + pixelValue[yL][xU] * tX;
					
					// lower two pixels on x axis
					lowerGray = pixelValue[yL][xU] * (1 - tX) + pixelValue[yU][xU] * tX;

					// interpolate between the two values for the y axis and round to get a valid 
					// gray value
					newGray = floor( (upperGray * (1 - tY) + lowerGray * tY) + 0.5);

					// Force pixel to be a proper color 0 - 255
					//newGray = ( newGray > 255 ) ? 255 : newGray;
					//newGray = ( newGray < 0 ) ? 0 : newGray;

					// assign the color value to the temp array
					temp[i][j] = newGray;
					
			}
		
		}
	}
	
	// clear the main array, copy temp to it and black out the temp array
	clearPV();
	tempToPV();
	clearTemp();
	return;
}



template <class PT>
void ImageType<PT>::shrink(int s){
	// Shrink an image by a factor s
	// Matt

	int newI = 0,
		newJ = 0,
		oldHeight = N,
		oldWidth = M;

	if( temp ){
		for( int i = 0; i < N; i++ )
			delete[] temp[i];
		delete[] temp;
	}

	// obtain new image dimensions
	N = (int)floor( (double)(N / s ) + 0.5);
	M = (int)floor( (double)(M / s) + 0.5);

	// allocate new memory for temp
	temp = new PT*[N];
	for( int i = 0; i < N; i++)
		temp[i] = new PT[M];

	// clear the temp array
	clearTemp();

	
	//add every Sth pixel from the PV to the temp array
	for( int i = 0; i < N; i++){
		for( int j = 0; j < M; j++ ){
			newI = ((i + 1) * s) - 1;
			newJ = ((j + 1) * s) - 1;

			temp[i][j] = pixelValue[newI][newJ];
			
		}
	}

	if( pixelValue ){
		for( int i = 0; i < oldHeight; i++ )
			delete[] pixelValue[i];
		delete[] pixelValue;
	}

	pixelValue = new PT*[N];
	for( int i = 0; i < N; i++)
		pixelValue[i] = new PT[M];

	clearPV();
	tempToPV();
	clearTemp();
}


template<class PT>
void ImageType<PT>::enlarge( int s ){
	// Scale an image by a factor s
	// Matt

	int oldJ = 0,
		oldI = 0,
		oldHeight = N,
		oldWidth = M;

	// De-allocate the temp array
	
	if( temp ){
		for( int i = 0; i < N; i++ )
			delete[] temp[i];
		delete[] temp;
	}
	// determine new image size
	M = M * s,
	N = N * s;

	// Allocate memory for temp with new size
	temp = new PT*[N];
	for( int i = 0; i < N; i++ ){
		temp[i] = new PT[M];
	}
	
	
	// Fill it with blank values
	clearTemp();
	
	// loop through and expand
	for( int i = 0; i < N; i++ ){

		// the i position changes on multiples s
		if( i % s == 0 && i > 1)
			oldI++;

		for( int j = 0; j < M; j++ ){

			// the j position we want from the old array changes on multiples of s
			if( j > 1 && j % s == 0)
				oldJ++;
			// construct the new image
			if( oldJ != oldWidth )
				temp[i][j] = pixelValue[oldI][oldJ];
		}
		
		// start back at the beginning column to fill the new row
		oldJ = 0;
	}

	// de-allocate old image array
	if( pixelValue ){
		for( int i = 0; i < oldHeight; i++ )
			delete[] pixelValue[i];
		delete[] pixelValue;
	}

	// Allocate memory for newly sized image
	pixelValue = new PT*[N];
	for( int i = 0; i < N; i++ ){
			pixelValue[i] = new PT[M];
	}
	

	// copy contents of the temp to the new array and erase temp
	tempToPV();
	clearTemp();

	return;
}


template<class PT>
ImageType<PT>& ImageType<PT>::operator = (ImageType<PT>& rhs ){
	// overloaded = operator
	// Matt
	
		int rhsN, rhsM, rhsQ;
		rhs.getImageInfo( rhsN, rhsM, rhsQ );

		if( rhsN != N || rhsM != M || rhsQ != Q ){
			N = rhsN;
			M = rhsM;
			Q = rhsQ;

			if( pixelValue ){
			for( int i = 0; i < N; i++ )
				delete[] pixelValue[i];
			delete[] pixelValue;
			}

			if( temp ){
			for( int i = 0; i < N; i++ )
				delete[] temp[i];
			delete[] temp;
			}

			pixelValue = new PT*[N];
			temp = new PT*[N];
			for( int i = 0; i < N; i++ ){
				pixelValue[i] = new PT[M];
				temp[i] = new PT[M];
			}
			clearTemp();
			clearPV();
		}

		for( int i = 0; i < N; i++ )
			for( int j = 0; j < M; j++ )
				pixelValue[i][j] = rhs.getPixelVal(i, j);
	
	return *this;

}
	
template<class PT>
ImageType<PT>& ImageType<PT>::operator + ( ImageType<PT>& rhs ){
	// add two images together
	// Matt

	int rhsM, rhsN, rhsQ;
	rhs.getImageInfo( rhsN, rhsM, rhsQ );

	int nM, nN, nQ;
	PT newVal;

	// use the largest of each dimension
	nM = ( rhsM > M ) ? rhsM : M;
	nN = ( rhsN > N ) ? rhsN : N;
	nQ = ( rhsQ > Q ) ? rhsQ : Q;

	// instantiate a image object to be returned
	ImageType *sum = new ImageType<PT>( nN , nM , nQ );

	// loop through and assign values to the new array
	for( int i = 0; i < nN; i++ ){
		for( int j = 0; j < nM; j++){

			// check bounds
			if( i >= N ) sum->setPixelVal( i, j, rhs.getPixelVal( i, j ) );
			else if( i >= rhsN ) sum->setPixelVal( i , j, pixelValue[i][j]  );
			else if( j >= M ) sum->setPixelVal( i, j , rhs.getPixelVal( i, j ) );
			else if( j >= rhsM ) sum->setPixelVal( i, j, pixelValue[i][j]  );
			else{
				// Determine new pixel value
				newVal = floor( ( ( rhs.getPixelVal( i , j ) * 0.5 ) + ( pixelValue[i][j] * 0.5 ) ) + 0.5 );
				//newVal = (pixelValue[i][j] * 0.5) * 0.5;
				
				// store new pixel into the array
				sum->setPixelVal( i , j , newVal );
			}
		}
	}

	return *sum;
}


template<class PT>
ImageType<PT>& ImageType<PT>::operator-( ImageType<PT>& rhs ){
	// compute the difference of two images
	// Matt

	int rhsM, rhsN, rhsQ;
	rhs.getImageInfo( rhsN, rhsM, rhsQ );

	int nM, nN, nQ;
	PT newVal;

	// use the largest of each dimension
	nM = ( rhsM > M ) ? rhsM : M;
	nN = ( rhsN > N ) ? rhsN : N;
	nQ = ( rhsQ > Q ) ? rhsQ : Q;

	// instantiate a image object to be returned
	ImageType *diff = new ImageType<PT>( nN , nM , nQ );

	// loop through and assign values to the new array
	for( int i = 0; i < nN; i++ ){
		for( int j = 0; j < nM; j++){

			// check bounds
			if( i >= N ) diff->setPixelVal( i, j, rhs.getPixelVal( i, j ) );
			else if( i >= rhsN ) diff->setPixelVal( i , j, pixelValue[i][j]  );
			else if( j >= M ) diff->setPixelVal( i, j , rhs.getPixelVal( i, j ) );
			else if( j >= rhsM ) diff->setPixelVal( i, j, pixelValue[i][j]  );
			else{
				// Determine new pixel value
				newVal = abs( pixelValue[i][j] - rhs.getPixelVal(i , j) );
				
				// store new pixel into the array
				diff->setPixelVal( i , j , newVal );
			}
		}
	}

	return *diff;
}


template<class PT>
void ImageType<PT>::negate(){
	// negate an image
	// Matt

	for( int i = 0; i < N; i++ )
		for( int j = 0; j < M; j++)
			pixelValue[i][j] = (pixelValue[i][j] * -1 ) + Q;
}

template<class PT>
void ImageType<PT>::subImg(int tlx, int tly, int brx, int bry){
	// get a sub image from the main image
	// precondition - the values passed are in bounds
	// matt

	int oldN = N, oldM = M;

	// prepare the temp array
	if( temp ){
		for( int i = 0; i < N; i++ )
			delete[] temp[i];
		delete[] temp;
	}

	// Force coords to be in bounds if the precondition is not met
	tlx = ( tlx > 0 && tlx < M && tlx <= brx ) ? tlx : 0;
	tly = ( tly > 0 && tly < N && tly <= bry ) ? tly : 0;
	brx = ( brx > 0 && brx < M && brx >= tlx ) ? brx : (M - 1);
	bry = ( bry > 0 && bry < N && bry >= tly ) ? bry : (N - 1);

	// get dimensions of the subimg
	N = bry - tly;
	M = brx - tlx;

    temp = new PT *[N];
	for( int i = 0; i < N; i++ )
		temp[i] = new PT[M];

	clearTemp();

	// loop through and fill the temp array
	for( int i = tly; i < bry; i++ ){
		for( int j = tlx; j < brx; j++ ){
			// store values from the source
			temp[i - tly][j - tlx] = pixelValue[i][j];
		}
	}

	// save results to the main array
	if( pixelValue ){
		for( int i = 0; i < oldN; i++ )
			delete[] pixelValue[i];
		delete[] pixelValue;
	}
    
	pixelValue = new PT*[N];
	for( int i = 0; i < N; i++ )
		pixelValue[i] = new PT[M];

	tempToPV();
	return;
}

template<class PT>
void ImageType<PT>::reflectH(){
	// reflect an image along the horizon
	// matt
	clearTemp();

	for( int i = 0; i < N; i++ )
		for( int j = 0; j < M; j++ )
			temp[i][j] = pixelValue[i][M - j - 1];

	tempToPV();
	return;
}

template<class PT>
void ImageType<PT>::reflectV(){
	// reflect an image vertically
	// matt
	clearTemp();

	for( int i = 0; i < N; i++ )
		for( int j = 0; j < M; j++ )
			temp[i][j] = pixelValue[N - i - 1][j];

	tempToPV();
	return;
}

template<class PT>
void ImageType<PT>::translate( int offsetX , int offsetY ){
	// translate an image in the x and y by the given offsets
	// precondition - the offsets aren't larger than the original image..
	// if they aren't then there is no offset in that direction

	int ofx, ofy;
	// make sure offsets are in bounds
	ofx = ( offsetX < M ) ? offsetX : 0;
	ofy = ( offsetY < N ) ? offsetY : 0;
	
	// prepare the temp array
	clearTemp();

	for( int i = 0; i < N; i++){
		for( int j = 0; j < M; j++ ){
			// check bounds
			if( (i + ofy < N) && (j + ofx < M) )
				// place the values from the source
				// into offset pixel coords on the temp array
				temp[i + ofy][j + ofx] = pixelValue[i][j];
		}
	}
	// push changes to the main array
	tempToPV();
}

template<class PT>
void ImageType<PT>::threshold( int t ){
	// Threshold an image

	// Fix later to be efficient
	PT *objectPixels = new PT[N * M];
	PT *bgPixels = new PT[N * M];
    PT objAvg = 0, bgAvg = 0, objSum = 0, bgSum = 0;
	int thresh = t,
		prevThresh = 0;
    int k = 0,
		l = 0;

	while( thresh != prevThresh ){
		for( int i = 0; i < N; i++ ){
			for( int j = 0; j < M; j++ ){
				if( pixelValue[i][j] > thresh ){
					objectPixels[k] = pixelValue[i][j];
					k++;
				}
				else{
					bgPixels[l] = pixelValue[i][j];
					l++;
				}
			}
		}
		prevThresh = thresh;

		// find average value of pixel in each set
		for( int i = 0; i < k; i++ )
			objSum += objectPixels[i];

		for( int i = 0; i < l; i++)
			bgSum += bgPixels[i];

		objAvg = objSum / k;
		bgAvg = bgSum / l;

		// new threshold 
		thresh = (objAvg + bgAvg) / 2;

		// reset values
		k = 0;
		l = 0;
		objSum = 0;
		bgSum = 0;
		objAvg = 0;
		bgAvg = 0;

	}

	cout << thresh << " " << prevThresh << " " << t;
}
		
		


    

	

#endif

