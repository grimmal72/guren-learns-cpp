#include <iostream>
using namespace std;


int main() {

	int x = 10;
	int y = 20;

	int* ptr1 = &x;  // ptr1 now points to x
	int* ptr2 = &y;


	cout << ptr1 << endl;
	cout << ptr2 << endl;
	
	ptr1 = (int *) x;
	ptr2 = (int *) y;

	cout << ptr1 << endl;
	cout << ptr2 << endl;


	return 0;
};