#ifndef IMAGE_H
#define IMAGE_H

template<class PT>
class ImageType {
 public:
    ImageType();
    ImageType(int, int, int);
	~ImageType();
	ImageType<PT>& operator + (ImageType<PT>&);
	ImageType<PT>& operator - (ImageType<PT>&);
	ImageType<PT>& operator = (ImageType<PT>&);
    void getImageInfo(int&, int&, int&);
    void setImageInfo(int, int, int);
    void setPixelVal(int, int, PT);
    void getPixelVal(int, int, PT&);
	PT getPixelVal(int, int);
    PT meanValue();
	void rotate(double);
	void rotateBilinear(double);
	void shrink(int);
	void enlarge(int);
	void negate();
	void subImg(int, int, int, int);
	void reflectH();
	void reflectV();
	void translate(int, int);
	void AutoThreshold(int);
	void threshold(int);
	void dilate();
	void erode();
	void setWhite();

 private:
	void clearTemp();
	void clearPV();
	void tempToPV();
	int N, M, Q;
	PT **pixelValue;
	PT **temp;
};

#include "image.cpp"

#endif
