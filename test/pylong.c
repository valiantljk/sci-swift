#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
Py_Initialize();
import_array();
long x =1;
PyObject * pylong = PyLong_FromLong(x);
PyObject * rta = PyLong_AsVoidPtr(pylong);
void * rt = (void *) rta;
if(rt==NULL) {
 printf("convert fail, rt is NULL\n");
 exit(-1);
}
PyObject * pylong2=PyLong_FromVoidPtr(rt);
long y=PyLong_AsLong(pylong2);
printf("before:%ld,after:%ld\n",x, y);
npy_intp m[1];
m[0] = 2;
int dims_test[2]={6,10};
PyObject * py_dims = PyArray_SimpleNewFromData(1, m, NPY_INT ,(void *)dims_test);
PyErr_Print();
Py_Finalize();
return 0;
}
