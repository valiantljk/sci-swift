#include <Python.h>
#include "swift.h"
PyObject * swift_list(int n, char *args[]){
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * settings [] ={"swiftclient.service","SwiftService", "list","swift"};
    pName = PyString_FromString(settings[0]);
    pModule = PyImport_ImportModule(settings[0]);
    Py_DECREF(pName);
    if(pModule == NULL){
      printf("Importing swiftclient.service failed\n");
      PyErr_Print();
      return NULL;
    }
    else {
     printf("Importing swiftclient.service ok\n");
     pFunc = PyObject_GetAttrString(pModule, settings[1]);
     if(pFunc && PyCallable_Check(pFunc)){
        printf("SwiftService is callable\n");
     }
     else {
        printf ("SwiftService is not callable\n");
        PyErr_Print();
     }
    }
    return pValue;
}

PyObject * basic_check(int n, char *args[])
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    pName = PyString_FromString(args[0]);
    /* Error checking of pName left out */
    long num = strtol(args[2], NULL, 10);
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, args[1]);
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

PyObject * operator_digest(int n, char *args[]){
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * settings [] ={"operator","_compare_digest","add"};
    pName = PyString_FromString(settings[0]);
    //pModule = PyImport_Import(pName);
    pModule = PyImport_ImportModule(settings[0]);
    Py_DECREF(pName);
    if(pModule == NULL){
      printf("Importing operator module failed\n");
      PyErr_Print();
    }
    else {
     printf ("Importing operator module ok\n");
     PyObject * pattr= PyString_FromString(settings[1]);
     PyObject * pattr_add = PyString_FromString(settings[2]);
     pFunc = PyObject_GetAttrString(pModule,settings[1]);
     /*
     // CallMethod
     PyObject * pcall= PyObject_CallMethod(pModule,settings[1],NULL);
     if(pcall == NULL) printf("calling _compare_digest in operator failed\n");
     // GetAttr
     pFunc = PyObject_GenericGetAttr(pModule, pattr);
     if (pFunc == NULL) printf("GenericGet _compare_digest failed\n");
     else printf("GenericGet _compare_digest ok\n");
     PyObject * pFunc2 = PyObject_GenericGetAttr(pModule, pattr_add);
     if (pFunc2 == NULL) printf ("GenericGet add failed\n");
     else printf("GenericGet add ok\n");
     // GetDict
     PyObject * plist= PyModule_GetDict(pModule);
     if (plist == NULL) printf("GetDict(operator) failed\n");
     else {
	printf("GetDict(operator) ok\n");
	Py_ssize_t lendict= PyDict_Size(plist);
	printf("Size of dict(operator):%zd\n",lendict);
	PyObject * pkeys = PyDict_Keys(plist);
	if (pkeys == NULL) printf("Dick_Keys(operator) failed\n");
	else {
		Py_ssize_t isz=0;
		for (isz=0; isz<lendict; isz++){
		     PyObject * pit = PyList_GetItem(pkeys,isz);
		     if(pit == NULL) printf("GetItem %zd returns NULL\n",isz);
		     else {
				//printf item
				PyObject * pstr= PyObject_Str(pit);
				printf("%zd item is %s\n",isz,PyString_AsString(pstr));
			  }
		}
	}
     }
     */
     // Callable_Check
     printf("\nhere\n");
     if(pFunc && PyCallable_Check(pFunc)){
    	printf("_compare_digest is callable\n");
     }
     else {
    	printf ("_compare_digest is not callable\n");
	PyErr_Print();
     }
     printf("here\n");
    }
    return pValue;
}

