#ifndef QUEUE_H
#define QUEUE_H

template<class PT>
class queue{
public:
	queue();
	queue(int);
	~queue();
	void makeEmpty(){ front = rear = max_items - 1; }
	bool isEmpty(){ return( front == rear ); }
	bool isFull(){ return ( (rear + 1) % max_items == front ); }
	void enqueue(PT);
	void dequeue(PT&);
private:
	int front, rear, max_items;
	PT *items;
};

#include "queue.cpp"

#endif