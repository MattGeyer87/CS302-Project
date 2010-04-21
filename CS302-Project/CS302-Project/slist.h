#ifndef SLIST_H
#define SLIST_H

#include "node.cpp"

template<class PT>
class slist{
public:
	slist();
	~slist();
	void makeEmpty();
	bool isFull();
	int getLength();
	void retrieveItem( PT& , bool& );
	void insertItem( PT& );
	void deleteItem( PT );
	void resetList();
	bool isLastItem();
	void getNextItem( PT& );
private:
	int length;
	node<PT>* data;
	node<PT>* cpos;
};

#include "slist.cpp"

#endif