#include <Python.h>
#include "../src/swift.h"
#include <stdio.h>
#include <assert.h>
int test_list(){
     // swift_list(1, 'container') should return all objects in container
     //PyObject * pValue;
     int argc = 1;
     char * argv []= {"scratch"};
     PyObject * pValue =  swift_list(argc, argv); // I need to know the returned type of swift list
     printf("swift_list called\n");
     Py_ssize_t x = PyDict_Size(pValue);
     //int count = (int) PyList_Size(x);
     assert(x>2);  
     printf("x:%zd \n",x);
     return 0; 
}

int test_operator(){
     int argc = 1;
     char * argv []= {"scratch"};
     PyObject * pValue =  operator_digest(argc, argv);
     printf("operator_digest called\n");
     return 0;
}


int main(){
    Py_Initialize();

    //Test 1: basic python c wrapper
    PyObject * pValue=NULL;
    int n =  3; 
    char * args []={"test", "f", "10"};
    printf("\nTesting basic c wrapper\n");
    printf ("number of args: %d\n",n);
    int i=0;
    printf("Input args list:");
    for(i=0;i<n;i++)
     printf ("%s\t",args[i]);
    printf("\n");
    pValue = basic_check(n, args); //argv: pythonfile funcname [args]
    if(pValue != NULL){
	printf("In test: Result of call: %ld\n", PyInt_AsLong(pValue));
    }
    printf("\n");
    //Test 2: swift list 
    printf("\nTesting swift list\n");
    printf ("number of args: %d\n",n);
    printf("Input args list:");
    for(i=0;i<n;i++)
     printf ("%s\t",args[i]);
    printf("\n");
    //test_list();
    printf("\n");
    printf("\n");
    test_operator();
    printf("\n");
    Py_Finalize();
    return 0;

}
