#include<iostream>
using namespace std;


#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

template<class T>

class Dynamic_array {
private:
	T* data;
	int len;
	int cap;

	// Doubles the current capacity when needed
	void double_capacity() {
		if (cap == 0) {
			cap++;
		}
		cap *= 2;
		T* temp = new T[cap];
		for (int i = 0; i < len; i++) {
			temp[i] = data[i];
		}
		delete[] data;
		data = temp;
	}

public:
	// Constructors & Destructor
	Dynamic_array() :data(nullptr), len(0), cap(0) {} // Default constructor

	Dynamic_array(int count, T* value)       // Fill constructor
	{
		len = count;
		cap = count;
		data = new T[cap];
		for (int i = 0; i < len; i++) {
			data[i] = value[i];
		}

	}
	Dynamic_array(const Dynamic_array& other) // Copy constructor
	{
		if (this != &other) {
			clear();
			this->len = other.len;
			this->cap = other.cap;
			this->data = new T[cap];
			for (int i = 0; i < other.len; i++) {
				this->data[i] = other.data[i];
			}
		}
	}
	~Dynamic_array()                          // Destructor
	{
		delete[]data;
	}
	// Operators
	Dynamic_array& operator=(const Dynamic_array& other) // Copy assignment
	{
		if (this != &other) {
			clear();
			this->len = other.len;
			this->cap = other.cap;
			this->data = new T[cap];
			for (int i = 0; i < other.len; i++) {
				this->data[i] = other.data[i];
			}
		}
		return*this;
	}
	T& operator[](int index)                          // Subscript operator
	{
		if (index < len && index >= 0) {
			return data[index];
		}
		else {
			cout << "\nWrong index";

			return data[len - 1];
		}
	}
	const T& operator[](int index) const            // Const subscript operator
	{
		if (index < len && index >= 0) {
			return data[index];
		}
		else {
			cout << "\nWrong index";

			return data[len - 1];
		}
	}
	bool operator==(const Dynamic_array& other) const // Equality comparison
	{
		if (this->len != other.len) {
			return false;
		}
		for (int i = 0; i < len; i++) {
			if (data[i] != other.data[i]) {
				return false;
			}
		}
		return true;

	}
	bool operator!=(const Dynamic_array& other) const // Inequality comparison
	{
		if (*this == other) {
			return false;
		}
		return true;
	}
	Dynamic_array operator+(const Dynamic_array& other) const // Concatenation
	{
		Dynamic_array result;
		result.len = len + other.len;
		result.cap = cap + other.cap;
		result.data = new T[result.cap];

		for (int i = 0; i < len; i++) {
			result.data[i] = data[i];
		}
		for (int i = 0; i < other.len; i++) {
			result.data[len + i] = other.data[i];
		}
		return result;
	}
	Dynamic_array& operator+=(const Dynamic_array& other)     // Append
	{
		*this = *this + other;
		return *this;
	}

	// Modifiers
	void push(T value)                 // Adds an element to the end
	{
		if (len == cap) {
			double_capacity();
		}
		data[len] = value;
		len++;
	}
	void pop()                           // Removes the last element
	{
		if (len > 0) {
			len--;
		}
	}
	void insert_at(int index, T value) // Inserts value at index
	{
		if (index <= 0 || index > len) {
			return;
		}
		if (len == cap) {
			double_capacity();
		}
		for (int i = len; i > index; i--) {
			data[i] = data[i - 1];
		}

		data[index] = value;
		len++;
	}
	void delete_at(int index)            // Deletes value at index
	{
		if (index <= 0 || index >= len) {
			return;
		}
		len--;
		for (int i = index; i < len; i++) {
			data[i] = data[i + 1];
		}
	}
	void reserve(int new_capacity)       // Reserves a new capacity
	{
		if (new_capacity > cap) {
			cap = new_capacity;
			int* temp = new int[cap];
			for (int i = 0; i < len; i++) {
				temp[i] = data[i];
			}
			delete[] data;
			data = temp;
		}
		else {
			cout << "\nGiven capacity is smaller";
		}
	}
	void clear()                         // Clears all elements
	{
		delete[]data;
		data = nullptr;
		this->len = 0;
		this->cap = 0;
	}
	void swap(Dynamic_array& other)      // Swaps with another array
	{
		int* temp_data = data;
		int temp_len = len;
		int temp_cap = cap;

		data = other.data;
		len = other.len;
		cap = other.cap;

		other.data = temp_data;
		other.len = temp_len;
		other.cap = temp_cap;
	}
	void shrink_to_fit()                 // Reduces capacity to match size
	{
		reserve(len);
	}
	void reverse()                       // Reverses the array elements
	{
		int temp = data[0];
		for (int i = 0; i < len / 2; i++) {
			temp = data[i];
			data[i] = data[len - 1 - i];
			data[len - 1 - i] = temp;
		}
	}
	// Accessors
	bool empty() const // Checks if array is empty
	{
		if (len == 0) {
			return true;
		}
		return false;
	}
	int size() const   // Returns current size
	{
		return len;
	}
	T& at(int index) // Bounds-checked element access
	{
		if (index < len && index >= 0) {
			return data[index];
		}
		else {
			cout << "\nWrong index";
			return data[len - 1];
		}
	}
	const T& at(int index) const
	{
		if (index < len && index >= 0) {
			return data[index];
		}
		else {
			cout << "\nWrong index";
			return data[len - 1];
		}
	}
	T& front() // Returns first element
	{
		return data[0];
	}
	const T& front() const
	{
		return data[0];
	}
	T& back() // Returns last element
	{
		return data[len - 1];
	}
	const T& back() const
	{
		return data[len - 1];
	}
	int find(T value) const // Return index of first instance, or -1 if non-existent
	{
		for (int i = 0; i < len; i++) {
			if (data[i] == value) {
				return i;
			}
		}
		return -1;
	}

};

#endif // DYNAMIC_ARRAY_H