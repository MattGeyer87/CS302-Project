#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

using namespace std;

#include "image.h"

ImageType::ImageType()
{
 N = 0;
 M = 0;
 Q = 0;

 pixelValue = NULL;
 temp = NULL;
}

ImageType::ImageType(int tmpN, int tmpM, int tmpQ)
{
 int i, j;

 N = tmpN;
 M = tmpM;
 Q = tmpQ;

 pixelValue = new int* [N];
 temp = new int* [N];
 for(i=0; i<N; i++) {
   pixelValue[i] = new int[M];
   temp[i] = new int[M];
   for(j=0; j<M; j++){
     pixelValue[i][j] = 0;
	 temp[i][j] = 0;
   }
 }
}

void ImageType::getImageInfo(int& rows, int& cols, int& levels)
{
 rows = N;
 cols = M;
 levels = Q;
} 

void ImageType::setImageInfo(int rows, int cols, int levels)
{
 N= rows;
 M= cols;
 Q= levels;
} 

void ImageType::setPixelVal(int i, int j, int val)
{
 pixelValue[i][j] = val;
}

void ImageType::getPixelVal(int i, int j, int& val)
{
 val = pixelValue[i][j];
}

double ImageType::getMeanGray(){
	// compute the mean (avg) pixel value of the image
	// Matt

	int numPixels = N * M;
	int totalGray = 0;
	
	// loop through all pixels
	for( int i = 0; i < N; i++)
		for( int j = 0; j < M; j++ )
			totalGray += pixelValue[i][j];
	
	// divide the sum of all gray values by total pixels
	return(totalGray / numPixels );
}

void ImageType::clearTemp(){
	// set all the values in the temp array to black
	// Matt

    for(int i = 0; i < N; i++)
		for(int j = 0; j < M; j++)
			temp[i][j] = 0;
}

void ImageType::clearPV(){
	// set all the values in the pixel array to black
	// Matt

	for( int i = 0; i < N; i++ )
		for( int j = 0; j < M; j++)
			pixelValue[i][j] = 0;
}

void ImageType::tempToPV(){
	// store all the values in the temp array to the
	// pixelValue (main) array
	// Matt

	for( int i = 0; i < N; i++)
		for( int j = 0; j < M; j++)
			pixelValue[i][j] = temp[i][j];
}


void ImageType::rotate( double angle){
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
	tempToPV();
	return;
}