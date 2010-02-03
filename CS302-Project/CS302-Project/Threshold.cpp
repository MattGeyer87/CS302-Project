#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#include "image.h"

void readImageHeader(char[], int&, int&, int&, bool&);
void readImage(char[], ImageType&);
void writeImage(char[], ImageType&);
int not_main();

int not_main()
{
 int i, j; 
 int M, N, Q;
 bool type;
 int val;
 int thresh;

 char *in, *out;
 in = new char[256];
 out = new char[256];

 cout << "Enter the input and output file separated by a space: " << endl;
 cin >> in >> out;

 // read image header
 readImageHeader(in, N, M, Q, type);

 // allocate memory for the image array

 ImageType image(N, M, Q);

 // read image
 readImage(in, image);
 
 cout << "Enter threshold: ";
 cin >> thresh;

 // threshold image 

 for(i=0; i<N; i++)
   for(j=0; j<M; j++) {
     image.getPixelVal(i, j, val);
     if(val < thresh) 
       image.setPixelVal(i, j, 255);
     else
       image.setPixelVal(i, j, 0);
    }

 // write image
 writeImage(out, image);
 
 system("PAUSE");
 return (1);
}
