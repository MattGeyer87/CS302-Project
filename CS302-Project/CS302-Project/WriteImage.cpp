#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#include "image.h"
#include "RGB.h"

void writeImage(char fname[], ImageType<int>& image)
{
 int i, j;
 int N, M, Q;
 unsigned char *charImage;
 ofstream ofp;

 image.getImageInfo(N, M, Q);

 charImage = (unsigned char *) new unsigned char [M*N];

 // convert the integer values to unsigned char

 int val;

 for(i=0; i<N; i++)
   for(j=0; j<M; j++) {
     image.getPixelVal(i, j, val);
     charImage[i*M+j]=(unsigned char)val;
   }

 ofp.open(fname, ios::out | ios::binary);

 if (!ofp) {
   cout << "Can't open file: " << fname << endl;
   exit(1);
 }

 ofp << "P5" << endl;
 ofp << M << " " << N << endl;
 ofp << Q << endl;

 ofp.write( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

 if (ofp.fail()) {
   cout << "Can't write image " << fname << endl;
   exit(0);
 }

 ofp.close();

 delete [] charImage;

}
void writeImage(char fname[], ImageType<RGB>& image)
{
 int i, j;
 int r, g, b;
 int N, M, Q;
 unsigned char *charImage;
 ofstream ofp;

 image.getImageInfo(N, M, Q);

 charImage = (unsigned char *) new unsigned char [3*M*N];

 RGB val;

 for(i=0; i<N; i++)
   for(j=0; j<3*M; j+=3) {
     image.getPixelVal(i, j/3, val);
	 val.getRGB( r, g, b);
     charImage[i*3*M+j]=(unsigned char)r;
     charImage[i*3*M+j+1]=(unsigned char)g;
     charImage[i*3*M+j+2]=(unsigned char)b;
   }

 ofp.open(fname, ios::out | ios::binary);

 if (!ofp) {
   cout << "Can't open file: " << fname << endl;
   exit(1);
 }

 ofp << "P6" << endl;
 ofp << M << " " << N << endl;
 ofp << Q << endl;

 ofp.write( reinterpret_cast<char *>(charImage), (3*M*N)*sizeof(unsigned char));

 if (ofp.fail()) {
   cout << "Can't write image " << fname << endl;
   exit(0);
 }

 ofp.close();

 delete [] charImage;
}
