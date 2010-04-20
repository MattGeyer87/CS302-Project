
#include "ulist.h"
#include "pixel.h"

class region{
public:
	region();
	~region();
	region& operator = ( const region& );
	bool operator <= ( region& );
	bool operator >= ( region& );
	bool operator < ( region& );
	bool operator > ( region& );
	void setEccentricity( int );
	int getEccentricity();
	void getEccentricity( int& );
	void setSize( int );
	int getSize();
	void getSize( int& );
	void setOrientation( double );
	double getOrientation();
	void getOrientation( double& );
	void setIntensity( int );
	int getIntensity();
	void getIntensity( int& );
	ulist<pixel>* getPixelList();
	void setPixelList( ulist<pixel>* );
private:
	int size, eccentricity, intensity;
	double orientation;
	ulist<pixel> *pixels;
};




