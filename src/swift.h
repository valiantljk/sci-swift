#ifndef SWIFT_H
#define SWIFT_H
PyObject * swift_list (int n, char * args[]);
PyObject * swift_put  (int n, char * args[]);
PyObject * swift_get  (int n, char * args[]);
PyObject * basic_check (int n, char * args[]);
PyObject * pyvol_check (int n, char * args[]);
// some other fun check
PyObject * operator_digest (int n, char * args[]);
#endif 
