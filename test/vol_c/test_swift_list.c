#include <Python.h>
#include "../../src/vol_c/swift.h"
int
main(int argc, char *argv[]){
    PyObject * pValue=NULL;
    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }
    pValue = swift_list(argv); //argv: pythonfile funcname [args]
    if(pValue != NULL){
	printf("In test: Result of call: %ld\n", PyInt_AsLong(pValue));
    }

    return 0;

}

