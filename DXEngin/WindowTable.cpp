#include "WindowTable.h"

template <class T>
WindowTable<T>::WindowTable() {}

template <class T>
WindowTable<T>::~WindowTable() {}

template <class T>
int WindowTable<T>::operator += (const T*)
{
	return 0;
}

template <class T>
void WindowTable<T>::operator -= (size_t index)
{

}

template <class T>
T* WindowTable<T>::operator [] (const size_t)
{
	return nullptr;
}