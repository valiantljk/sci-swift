#include <Python.h>
#include "swift.h"
PyObject * swift_list(int n, char *args[])
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    pName = PyString_FromString(args[1]);
    /* Error checking of pName left out */
    long num=0;
    if (n>3){ 
     num = strtol(args[3], NULL, 10);
    }
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, args[2]);
        /* pFunc is a new reference */
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, PyInt_FromLong(num));
            pValue = PyObject_CallObject(pFunc, pArgs);
            printf("called\n");
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
		Py_DECREF(PyTuple_GetItem(pArgs, 1));
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return NULL;
            }
            /* decref arrays in object */
	    //Py_DECREF(PyTuple_GetItem(pArgs, 1));
            Py_XDECREF(pArgs);
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", args[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", args[1]);
        return NULL;
    }
    return pValue;
}

