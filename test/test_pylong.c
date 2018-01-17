#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
Py_Initialize();
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
Py_Finalize();
return 0;
}
