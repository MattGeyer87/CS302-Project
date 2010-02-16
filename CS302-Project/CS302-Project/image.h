#ifndef IMAGE_H
#define IMAGE_H

class ImageType {
 public:
    ImageType();
    ImageType(int, int, int);
	//~ImageType();
	ImageType operator + (ImageType&);
	ImageType operator - (ImageType&);
	ImageType operator = (ImageType&);
    void getImageInfo(int&, int&, int&);
    void setImageInfo(int, int, int);
    void setPixelVal(int, int, int);
    void getPixelVal(int, int, int&);
	int getPixelVal(int, int);
    double getMeanGray();
	void rotate(double);
	void rotateBilinear(double);
	void shrink(int);
	void enlarge(int);
	void negate();
	void subImg(int, int, int, int);
	void reflectH();
	void reflectV();
	void translate(int, int);

 private:
	void clearTemp();
	void clearPV();
	void tempToPV();
	int N, M, Q;
	int **pixelValue;
	int **temp;
};

#endif
