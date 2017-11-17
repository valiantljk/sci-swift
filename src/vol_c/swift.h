#ifndef SWIFT_H
#define SWIFT_H
PyObject * swift_list (char * args[]);
PyObject * swift_put  (char * args[]);
PyObject * swift_get  (char * args[]);
typedef double real;

typedef struct arglist arglist;

struct arglist {
        int n;                  /* number of args */
        int *at;                /* argument types -- see DISCUSSION below */
        real *ra;               /* pure real args (IN, OUT, and INOUT) */
        real *derivs;           /* for partial derivatives (if nonzero) */
        real *hes;              /* for second partials (if nonzero) */
};
#endif 
