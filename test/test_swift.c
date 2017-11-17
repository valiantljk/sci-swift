#include <Python.h>
#include "../src/swift.h"
int
main(int argc, char *argv[]){
    Py_Initialize();
    PyObject * pValue=NULL;
    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }
    pValue = swift_list(argc, argv); //argv: pythonfile funcname [args]
    if(pValue != NULL){
	printf("In test: Result of call: %ld\n", PyInt_AsLong(pValue));
    }
    Py_Finalize();
    return 0;

}
