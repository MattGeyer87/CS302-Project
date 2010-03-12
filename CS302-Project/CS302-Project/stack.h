#ifndef STACK_H
#define STACK_H

template<class PT>
class stack{
public:
	stack();
	stack(int);
	~stack();
	void makeEmpty() { top = -1; }
	bool isEmpty(){	return ( top == -1); }
	bool isFull() { return ( top == max_items - 1 ); }
	void push(PT);
	void pop(PT&);
private:
	int top, max_items;
	PT *items;
};

#include "stack.cpp"

#endif