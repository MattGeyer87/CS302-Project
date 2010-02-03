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


//Entered a comment in the test branch..

// Test lol

void testFunction(){
	cout << "Just testing how function changes the main file.." << endl;
	cout << "This change i made inside of the master branch..";
	cout << "I guess changing the master changes the other branch?";
	cout << "But it doesn't work the other way around?";
	return;
}

//Added a comment in the test branch.a
void testBranchFunction(){
	cout << "Made this change from inside mattDev branch";
	cout << "Testing how it affects changes when you",
	"branch";
	}

#endif
