#ifdef STACK_H

template<class PT>
stack<PT>::stack(){
	// default constructor
	top = -1;
	items = NULL;
}

template<class PT>
stack<PT>::stack( int size ){
	max_items = size;
	items = new PT[max_items];
}

template<class PT>
stack<PT>::~stack(){
	if( items ) delete[] items;
}

template<class PT>
void stack<PT>::push( PT item ){
	// push an item onto the stack
	top++;
	items[top] = item;
}

template<class PT>
void stack<PT>::pop( PT& item ){
	// pop an item off the stack
	item = items[top];
	top--;
}


#endif