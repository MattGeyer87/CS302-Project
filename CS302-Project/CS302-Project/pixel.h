#ifndef PIXEL_H
#define PIXEL_H


class pixel{
public:
	pixel();
	pixel& operator = ( const pixel& );
	pixel(int,int);
	void setPixelVals( int, int);
	void getPixelVals( int&, int&) const;
private:
	int i, j;

};



#endif