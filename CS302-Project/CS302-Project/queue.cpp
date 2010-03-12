#include <iostream>
#include "pixel.h"
using namespace std;

#ifdef QUEUE_H


template<class PT>
queue<PT>::queue(){
	max_items = front = rear = 0;
	items = NULL;
}

template<class PT>
queue<PT>::queue( int size ){
	max_items = size + 1;
	front = rear = size;
	items = new PT[max_items];
}

template<class PT>
queue<PT>::~queue(){
	if( items ) delete[] items;
}

template<class PT>
void queue<PT>::enqueue( PT item ){
	// enqueue an item
	rear = (rear + 1) % max_items;
	items[rear] = item;
}

template<class PT>
void queue<PT>::dequeue( PT& item ){
	// dequeue an item
	front = (front + 1) % max_items;
	item = items[front];
}
	

#endif