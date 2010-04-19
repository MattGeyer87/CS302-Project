#ifdef ULIST_H

#include <iostream>

using namespace std;

template<class PT>
ulist<PT>::ulist(){
	// constructor
	length = 0;
	data = NULL;
	cpos = NULL;
}

template<class PT>
void ulist<PT>::makeEmpty(){
	// delete all items in the list
	node<PT> *temp = NULL;
	while(data != NULL){
		temp = data;
		data = data->next;
		delete temp;
	}
	length = 0;
}

template<class PT>
ulist<PT>::~ulist(){
	// destructor
	makeEmpty();
}

template<class PT>
bool ulist<PT>::isFull(){
	// check if the list is full
	node<PT> *temp = new node<PT>;
	if( !temp )
		return true;
	else{
		delete temp;
		return false;
	}
}

template<class PT>
int ulist<PT>::getLength(){
	// return the length of the list
	return length;
}

template<class PT>
void ulist<PT>::resetList(){
	// reset the list
	cpos = data;
}


template<class PT>
void ulist<PT>::retrieveItem( PT& item , bool& found ){
	// find an item in the list
	
	node<PT>* location = data;
	found = false;

	while( (location != NULL) && !found ){

		if( location->info > item )
			location = location->next;
		else if( item == location->info ){
			found = true;
			item = location->info;
		}
		else{
			location = NULL;
		}
	}
	return;
}

template<class PT>
void ulist<PT>::deleteItem( PT item ){
	// delete an item in the list
	node<PT> *tempLoc, *location = data;

	if( item == data->info ){
		tempLoc = data;
		data = data->next;
	}
	else{
		while(!(location->next>info == item) )
			location = location->next;

		tempLoc = location->next;
		location->next = tempLoc->next;
	}
	delete tempLoc;
	length--;
	return;
}

template<class PT>
void ulist<PT>::getNextItem( PT& item ){
	// get the next item in the list
	item = cpos->info;
	cpos = cpos->next;
}

template<class PT>
bool ulist<PT>::isLastItem(){
	// return if it is the last item in the list
	return( cpos == NULL );
}

template<class PT>
void ulist<PT>::insertItem( PT item ){
	// insert item 
	node<PT> *location = new node<PT>;
	location->info = item;
	location->next = data;
	data = location;
	length++;
}







#endif