#ifndef IMAGE_H
#define IMAGE_H

class ImageType {
 public:
   ImageType();
   ImageType(int, int, int);
   void getImageInfo(int&, int&, int&);
   void setImageInfo(int, int, int);
   void setPixelVal(int, int, int);
   void getPixelVal(int, int, int&);
 private:
   int N, M, Q;
   int **pixelValue;
};
//Added a comment in the test branch.
void testBranchFunction(){
	cout << "Testing how it affects changes when you,
	branch";
	}
#endif
