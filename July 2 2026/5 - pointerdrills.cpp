#include <iostream>
using namespace std;

// This is me testing many of the questions on this webpage, which is great for practicing pointers:
// https://cpointerdrills.github.io/





// (This function is also part of Q5)
int placeholderValue = 0;
int* c = &placeholderValue;
void weirdPointerFunction(int* parameter) {
	*parameter = 3;

	cout << *parameter << endl;
}




// (This stuff is also part of Q10)
int intA = 5;
int* p_intA = &intA;
int** p_p_intA = &p_intA;

void pointerToPointerFunction(int** p_p_intA_Alias) {
	//**p_p_intA_Alias = 2;
	cout << **p_p_intA_Alias << endl;
};


// Q11
void change_int_ptr(int **p_p_intA) {

	*p_p_intA = (int*)malloc(sizeof(int));

	cout << *p_p_intA << endl;
}



int main() {


	// Q1 Define an integer a  and initialize it to 42.
	int a = 42;

	// Q2
	int* p_a = &a;

	// Q3
	*p_a = 43;

	// Q4
	int b = 44;
	p_a = &b;

	// Q5
	weirdPointerFunction(c);

	// Q6
	weirdPointerFunction(&a);

	// Q7
	weirdPointerFunction(p_a);

	// Q8
	int** p_p_a;

	// Q9
	p_p_a = &p_a;

	// Q10
	pointerToPointerFunction(p_p_intA);

	// Q11
	change_int_ptr(p_p_intA);


	return 0;
}