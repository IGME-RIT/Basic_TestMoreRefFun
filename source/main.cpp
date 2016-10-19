/*
TestMoreRefFun
(c) 2016
original authors: David I. Schwartz
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	This tutorial demonstrates experiments, mostly of which are bad (but see how we delete from test3() and pointers in test4()).
*	You can run into trouble trying to return references, though there are times you must (see later with OOP and the STL)
*/



#include <iostream>
using namespace std;
#pragma region Leak Detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG
#pragma endregion

int& test1() {	// declare function test1()
    int x = 1;
    return x;	// warning: returning local variable
}

int& test2(int a) {
    return a;	// warning: returning temporary
}

int& test3() {	//declare function test3()
    int* i = new int;							// allocate memory in heap
    *i = 10;									// dereference of pointer i gets 10
    cout << "Value of *i:    " << *i << endl;	// output: Value of *i:    10
    cout << "Where i points: " << i << endl;	// output: Where i points: 0114E8B0
    return *i;
}

int* test4() {
    return new int;
}

int main() {
    
    // bad uses of returning references!
    // adapted from http://stackoverflow.com/questions/752658/is-the-practice-of-returning-a-c-reference-variable-evil
    cout << test1() << endl;  // generates warning
    cout << test2(2) << endl; // generates warning

    int& j = test3();         // refer to return value of test3
    cout << "Value of j:     " << j << endl;	// output: Value of j:     10
    cout << "Address of j:   " << &j << endl;	// output: Address of j:   00CECDF8
    delete &j;                // j refers to where 10 is stored on heap...see below:

    /**********************************************************************************************
     *     i ---> 10    i is deallocated when test3() done                                        *
     *             ^                                                                              *
     *             |                                                                              *
     *     j ------+    j refers to the 10, and so, &j and i store the same address values        *
     *                                                                                            *
     *   stack    heap                                                                            *
     **********************************************************************************************/
	
     int goodInt = test3();		// what happens if you don't use an &? ( test3 refers to a temporary int which will be delete after calling the fucntion )
    // delete &goodInt;         // won't work. goodInt is a copy of the allocated int, which is now lost forever; not sending the address of the value, sending back the value pointed by i
	

    // Ways to avoid problems with references and avoid leaks: 
    int* a = test4();  // has to be a pointer
    int& b = *test4(); // can refer to dereferenced return of test4 (long winded, but OK)
    delete a;          // a is a pointer
    delete &b;         // b refers to same variable returned by test4

    _CrtDumpMemoryLeaks();

}