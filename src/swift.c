#include <Python.h>
#include "swift.h"
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
     //int hasatt1=PyObject_HasAttrString(pModule,settings[1]);
     int hasatt1 = PyObject_HasAttr(pModule,pattr);
     if (hasatt1==1) {
 	printf("operator has %s\n",settings[1]);	
     }
     else {
     	printf("operator does not have %s\n",settings[1]);
	PyErr_Print();
	//return NULL;
     }
     printf("here\n");
     hasatt1=PyObject_HasAttrString(pModule,settings[2]);
     if (hasatt1==1) {
        printf("operator has %s\n",settings[2]); 
     }
     else {
        printf("operator does not have %s\n",settings[2]);
        PyErr_Print();
        //return NULL;
     }
     printf("here\n");
     pFunc = PyObject_GetAttrString(pModule,settings[1]);
     if(pFunc && PyCallable_Check(pFunc)){
    	printf("_compare_digest is callable\n");
     }
     else {
    	printf ("_compare_digest is not callable\n");
	PyErr_Print();
     }
    }
    return pValue;
}

PyObject * swift_list(int n, char *args[]){
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * settings [] ={"swiftclient.service","SwiftService", "list","swift"};
    pName = PyString_FromString(settings[0]);
    //pModule = PyImport_ImportModuleNoBlock(pName);
    //pModule = PyImport_ImportModuleEx(settings[1],NULL,NULL,pName);
    pModule = PyImport_ImportModule(settings[0]);
    Py_DECREF(pName);
    if(pModule == NULL){
      printf("Importing swiftclient.service failed\n");
      printf("here 1:\n");
      PyErr_Print();
      printf("here 2:\n");
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
