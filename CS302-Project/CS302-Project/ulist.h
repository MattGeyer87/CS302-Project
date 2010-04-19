#ifndef ULIST_H
#define ULIST_H

#include "node.cpp"

template<class PT>
class ulist{
public:
	ulist();
	~ulist();
	void makeEmpty();
	bool isFull();
	int getLength();
	void retrieveItem( PT& , bool& );
	void insertItem( PT );
	void deleteItem( PT );
	void resetList();
	bool isLastItem();
	void getNextItem( PT& );

private:
	int length;
	node<PT> *data, *cpos;
};





#include "ulist.cpp"

#endif