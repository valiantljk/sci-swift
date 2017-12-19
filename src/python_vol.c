//#ifndef H5_VOL_H
//#define H5_VOL_H
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hdf5.h"
#include "python_vol.h"
#define PYTHON 502
//static herr_t H5VL_python_init(hid_t vipl_id);
//static herr_t H5VL_python_term(hid_t vtpl_id);
/* Datatype callbacks */
static void *H5VL_python_datatype_commit(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t type_id, hid_t lcpl_id, hid_t tcpl_id, hid_t tapl_id, hid_t dxpl_id, void **req);
static void *H5VL_python_datatype_open(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t tapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_python_datatype_get(void *dt, H5VL_datatype_get_t get_type, hid_t dxpl_id, void **req, va_list arguments);
static herr_t H5VL_python_datatype_close(void *dt, hid_t dxpl_id, void **req);

/* Dataset callbacks */
static void *H5VL_python_dataset_create(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id, void **req);
static void *H5VL_python_dataset_open(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_python_dataset_read(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                                    hid_t file_space_id, hid_t plist_id, void *buf, void **req);
static herr_t H5VL_python_dataset_write(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                                     hid_t file_space_id, hid_t plist_id, const void *buf, void **req);
static herr_t H5VL_python_dataset_close(void *dset, hid_t dxpl_id, void **req);

/* File callbacks */
static void *H5VL_python_file_create(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id, hid_t dxpl_id, void **req);
static void *H5VL_python_file_open(const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_python_file_get(void *file, H5VL_file_get_t get_type, hid_t dxpl_id, void **req, va_list arguments);
static herr_t H5VL_python_file_close(void *file, hid_t dxpl_id, void **req);

/* Group callbacks */
static void *H5VL_python_group_create(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_python_group_close(void *grp, hid_t dxpl_id, void **req);

/* Link callbacks */

/* Object callbacks */
static void *H5VL_python_object_open(void *obj, H5VL_loc_params_t loc_params, H5I_type_t *opened_type, hid_t dxpl_id, void **req);
static herr_t H5VL_python_object_specific(void *obj, H5VL_loc_params_t loc_params, H5VL_object_specific_t specific_type, hid_t dxpl_id, void **req, va_list arguments);

hid_t native_plugin_id = -1;

const H5VL_class_t H5VL_python_g = {
    0,							/* version */
    PYTHON,						/* value */
    "python",                                      	/* name */
    NULL, //    H5VL_python_init,                        /* initialize */
    NULL, //    H5VL_python_term,                        /* terminate */
    sizeof(hid_t),
    NULL,
    NULL,
    {                                           /* attribute_cls */
        NULL, //H5VL_python_attr_create,                /* create */
        NULL, //H5VL_python_attr_open,                  /* open */
        NULL, //H5VL_python_attr_read,                  /* read */
        NULL, //H5VL_python_attr_write,                 /* write */
        NULL, //H5VL_python_attr_get,                   /* get */
        NULL, //H5VL_python_attr_specific,              /* specific */
        NULL, //H5VL_python_attr_optional,              /* optional */
        NULL  //H5VL_python_attr_close                  /* close */
    },
    {                                           /* dataset_cls */
        H5VL_python_dataset_create,                    /* create */
        H5VL_python_dataset_open,                      /* open */
        H5VL_python_dataset_read,                      /* read */
        H5VL_python_dataset_write,                     /* write */
        NULL, //H5VL_python_dataset_get,               /* get */
        NULL, //H5VL_python_dataset_specific,          /* specific */
        NULL, //H5VL_python_dataset_optional,          /* optional */
        H5VL_python_dataset_close                      /* close */
    },
    {                                               /* datatype_cls */
        H5VL_python_datatype_commit,                   /* commit */
        H5VL_python_datatype_open,                     /* open */
        H5VL_python_datatype_get,                      /* get_size */
        NULL, //H5VL_python_datatype_specific,         /* specific */
        NULL, //H5VL_python_datatype_optional,         /* optional */
        H5VL_python_datatype_close                     /* close */
    },
    {                                           /* file_cls */
        H5VL_python_file_create,                      /* create */
        H5VL_python_file_open,                        /* open */
        H5VL_python_file_get,                         /* get */
        NULL, //H5VL_python_file_specific,            /* specific */
        NULL, //H5VL_python_file_optional,            /* optional */
        H5VL_python_file_close                        /* close */
    },
    {                                           /* group_cls */
        H5VL_python_group_create,                     /* create */
        NULL, //H5VL_python_group_open,               /* open */
        NULL, //H5VL_python_group_get,                /* get */
        NULL, //H5VL_python_group_specific,           /* specific */
        NULL, //H5VL_python_group_optional,           /* optional */
        H5VL_python_group_close                       /* close */
    },
    {                                           /* link_cls */
        NULL, //H5VL_python_link_create,                /* create */
        NULL, //H5VL_python_link_copy,                  /* copy */
        NULL, //H5VL_python_link_move,                  /* move */
        NULL, //H5VL_python_link_get,                   /* get */
        NULL, //H5VL_python_link_specific,              /* specific */
        NULL, //H5VL_python_link_optional,              /* optional */
    },
    {                                           /* object_cls */
        H5VL_python_object_open,                        /* open */
        NULL, //H5VL_python_object_copy,                /* copy */
        NULL, //H5VL_python_object_get,                 /* get */
        H5VL_python_object_specific,                    /* specific */
        NULL, //H5VL_python_object_optional,            /* optional */
    },
    {
        NULL,
        NULL,
        NULL
    },
    NULL
};

typedef struct H5VL_python_t {
    void   *under_object;
} H5VL_python_t;


/* File callbacks Implementation*/
static void *
H5VL_python_file_create(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id, hid_t dxpl_id, void **req)
{
    hid_t under_fapl;
    H5VL_python_t *file;
    
    file = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));

    under_fapl = *((hid_t *)H5Pget_vol_info(fapl_id));
    int ipvol=0; //default is using h5py for python vol
    char pvol_name[3]="py"; 
    if( H5Pexist(fapl_id, pvol_name)>0){
      H5Pget(fapl_id, pvol_name, &ipvol);
      //printf ("%s vol exists: %d\n",pvol_name,ipvol);
    }
   
    //file->under_object = H5VLfile_create(name, flags, fcpl_id, under_fapl, dxpl_id, req);
    PyObject * vol_cls =NULL;  // This layer will figure out which python vol to be called, based on fapl_id, similar to line 564 in H5F.c
    //TODO: Remind myself, Dec 10 2017, For now, just figure out, how to return a swift file object, in future, figure out returning a generic python object
    //pexist in tgenprop.c TODO
    //pget, don't forget to store in a pytho vol field, H5VL_python_t; 
    //TODO: Figure out which python vol to call 
    //line 604, H5F.c    if(NULL == (vol_cls = (H5VL_class_t *)H5I_object_verify(plugin_prop.plugin_id, H5I_VOL)))
    //    			HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "not a VOL plugin ID")	 
    //pass the vol_cls to the python_vol.py, from where the dispatching happens, and the actual vol, e.g., swift, is called.   
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_file_create"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
	pArgs = PyTuple_New(7);
        PyTuple_SetItem(pArgs, 0, PyString_FromString(name));
	PyTuple_SetItem(pArgs, 1, PyLong_FromLong(flags));
	PyTuple_SetItem(pArgs, 2, PyLong_FromLong(fcpl_id));
	PyTuple_SetItem(pArgs, 3, PyLong_FromLong(under_fapl));
	PyTuple_SetItem(pArgs, 4, PyLong_FromLong(dxpl_id));
	if(req!=NULL)
 	 PyTuple_SetItem(pArgs, 5, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
	 PyTuple_SetItem(pArgs, 5, PyString_FromString("None")); 
	PyTuple_SetItem(pArgs, 6, PyLong_FromLong(ipvol));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
		//printf("------- Result of H5Fcreate from python: %ld\n", PyInt_AsLong(pValue));
		void * rt_py = PyLong_AsVoidPtr(pValue);
		if (rt_py==NULL) printf("returned pointer from python is NULL\n");
                file->under_object = rt_py;
	 	//printf("file obj: %p,file->under_object:%p\n",file,rt_py);
		return (void *) file;
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
	        Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return NULL;
        }
	Py_XDECREF(pArgs);
     }
     else {
     	fprintf(stderr, "------- PYTHON H5Fcreate Failed\n");
	return NULL;
     }
    }
    else {
        fprintf(stderr, "Python module :%s is not available\n",args[0]);
    }
    //printf("------- PYTHON H5Fcreate\n");
    return (void *)file;
}

static void *
H5VL_python_file_open(const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req)
{
    hid_t under_fapl;
    H5VL_python_t *file;

    file = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));

    under_fapl = *((hid_t *)H5Pget_vol_info(fapl_id));
    file->under_object = H5VLfile_open(name, flags, under_fapl, dxpl_id, req);
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_file_open"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(5);
        PyTuple_SetItem(pArgs, 0, PyString_FromString(name));
        PyTuple_SetItem(pArgs, 1, PyLong_FromLong(flags));
        PyTuple_SetItem(pArgs, 2, PyLong_FromLong(fapl_id));
        PyTuple_SetItem(pArgs, 3, PyLong_FromLong(dxpl_id));
	if(req!=NULL)
         PyTuple_SetItem(pArgs, 4, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 4, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
		printf("------- Result of H5Fopen from python: %ld\n", PyLong_AsLong(pValue));
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return NULL;
        }
        Py_XDECREF(pArgs);
     }
     else {
     	fprintf(stderr, "------- PYTHON H5Fopen Failed\n");
	return NULL;
     }
    }
    else {
	fprintf(stderr, "------- Python module :%s is not available\n",args[0]);	
    }

    //printf("------- PYTHON H5Fopen\n");
    return (void *)file;
}

static herr_t 
H5VL_python_file_get(void *file, H5VL_file_get_t get_type, hid_t dxpl_id, void **req, va_list arguments)
{
    H5VL_python_t *f = (H5VL_python_t *)file;

    H5VLfile_get(f->under_object, native_plugin_id, get_type, dxpl_id, req, arguments);

    //printf("------- PYTHON H5Fget %d\n", get_type);
    return 1;
}
static herr_t 
H5VL_python_file_close(void *file, hid_t dxpl_id, void **req)
{
    H5VL_python_t *f = (H5VL_python_t *)file;
    PyObject * plong = PyLong_FromVoidPtr(f->under_object);
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_file_close"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(3);
        PyTuple_SetItem(pArgs, 0,plong);
        PyTuple_SetItem(pArgs, 1, PyLong_FromLong(dxpl_id));
	if(req!=NULL)
         PyTuple_SetItem(pArgs, 2, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 2, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
		//printf("------- Result of H5Fclose from python: %ld\n", PyLong_AsLong(pValue));
		printf ("------- PYTHON H5Fclose\n");	
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return -1;
        }
        Py_XDECREF(pArgs);
	Py_XDECREF(pValue);
	Py_XDECREF(pModule);
	Py_XDECREF(pFunc);
     }
     else {
      	fprintf(stderr, "------- PYTHON H5Fclose failed\n");
      	return -1;
     }
    }
    else {
	fprintf(stderr, "------- Python module :%s is not available\n",args[0]);
	return -1;
    }
    free(f);
    return 1;
}
/* Group callbacks Implementation*/
static void *
H5VL_python_group_create(void *obj, H5VL_loc_params_t loc_params, const char *name, 
                      hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id, void **req)
{
    H5VL_python_t *group;
    H5VL_python_t *o = (H5VL_python_t *)obj;
    PyObject * plong = PyLong_FromVoidPtr(o); // get the id of py object, and pass into python layer to form a py obj
    PyObject * plong_under = PyLong_FromVoidPtr(o->under_object);
    group = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_group_create"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(7);
	//TODO: struct pointer
	//PyTuple_SetItem(pArgs, 0, PyCapsule_New(obj, "obj", NULL));
	PyTuple_SetItem(pArgs, 0,plong_under);
        //TODO: struct
	PyTuple_SetItem(pArgs, 1, PyCapsule_New(&loc_params, "loc", NULL));
        PyTuple_SetItem(pArgs, 2, PyString_FromString(name));
        PyTuple_SetItem(pArgs, 3, PyLong_FromLong(gcpl_id));
        PyTuple_SetItem(pArgs, 4, PyLong_FromLong(gapl_id));
        PyTuple_SetItem(pArgs, 5, PyLong_FromLong(dxpl_id)); 
        if(req!=NULL)
         PyTuple_SetItem(pArgs, 6, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 6, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
//                printf("------- Result of H5Gcreate from python: %ld\n", PyInt_AsLong(pValue));
                void * rt_py = PyLong_AsVoidPtr(pValue);
                if (rt_py==NULL) printf("returned pointer from python is NULL\n");
                group->under_object = rt_py;
                return (void *) group;		
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return NULL;
        }
        Py_XDECREF(pArgs);
     }
     else {
	PyErr_Print();
     	fprintf(stderr, "------- PYTHON H5Gcreate failed\n");
	return NULL;
     }
    }
    else {
	PyErr_Print();
        fprintf(stderr, "------- Python module :%s is not available\n",args[0]);
    }
    //printf ("------- PYTHON H5Gcreate\n");
    return (void *) group;
}

static herr_t 
H5VL_python_group_close(void *grp, hid_t dxpl_id, void **req)
{
    H5VL_python_t *g = (H5VL_python_t *)grp;

    //H5VLgroup_close(g->under_object, native_plugin_id, dxpl_id, req);
    free(g);
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_group_close"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(3);
        //TODO: struct pointer
        PyTuple_SetItem(pArgs, 0, PyCapsule_New(grp, "grp", NULL));
        PyTuple_SetItem(pArgs, 1, PyLong_FromLong(dxpl_id));
        //req can be NULL
        if(req!=NULL)
         PyTuple_SetItem(pArgs, 2, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 2, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
                printf("------- Result of H5Gclose from python: %ld\n", PyInt_AsLong(pValue));
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return -1;
        }
        Py_XDECREF(pArgs);
     }
     else {
        fprintf(stderr, "------- PYTHON H5Gclose failed\n");
        return -1;
     }
    }
    else {
        fprintf(stderr, "------- Python module :%s is not available\n",args[0]);
    }
    //printf ("------- PYTHON H5Gclose\n");
    return 1;
}
/* Datatypes callbacks Implementation*/
static void *
H5VL_python_datatype_commit(void *obj, H5VL_loc_params_t loc_params, const char *name, 
                         hid_t type_id, hid_t lcpl_id, hid_t tcpl_id, hid_t tapl_id, hid_t dxpl_id, void **req)
{
    H5VL_python_t *dt;
    H5VL_python_t *o = (H5VL_python_t *)obj;

    dt = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));
    
    dt->under_object = H5VLdatatype_commit(o->under_object, loc_params, native_plugin_id, name, 
                                           type_id, lcpl_id, tcpl_id, tapl_id, dxpl_id, req);

    printf("------- PYTHON H5Tcommit\n");
    return dt;
}
static void *
H5VL_python_datatype_open(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t tapl_id, hid_t dxpl_id, void **req)
{
    H5VL_python_t *dt;
    H5VL_python_t *o = (H5VL_python_t *)obj;  

    dt = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));

    dt->under_object = H5VLdatatype_open(o->under_object, loc_params, native_plugin_id, name, tapl_id, dxpl_id, req);

    //printf("------- PYTHON H5Topen\n");
    return (void *)dt;
}
static herr_t 
H5VL_python_datatype_get(void *dt, H5VL_datatype_get_t get_type, hid_t dxpl_id, void **req, va_list arguments)
{
    H5VL_python_t *o = (H5VL_python_t *)dt;
    herr_t ret_value;

    ret_value = H5VLdatatype_get(o->under_object, native_plugin_id, get_type, dxpl_id, req, arguments);

    //printf("------- PYTHON datatype get\n");
    return ret_value;
}

static herr_t 
H5VL_python_datatype_close(void *dt, hid_t dxpl_id, void **req)
{
    H5VL_python_t *type = (H5VL_python_t *)dt;

    assert(type->under_object);

    H5VLdatatype_close(type->under_object, native_plugin_id, dxpl_id, req);
    free(type);

    //printf("------- PYTHON H5Tclose\n");
    return 1;
}
/* Object callbacks Implementation*/
static void *
H5VL_python_object_open(void *obj, H5VL_loc_params_t loc_params, H5I_type_t *opened_type, hid_t dxpl_id, void **req)
{
    H5VL_python_t *new_obj;
    H5VL_python_t *o = (H5VL_python_t *)obj;

    new_obj = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));
    
    new_obj->under_object = H5VLobject_open(o->under_object, loc_params, native_plugin_id, opened_type, dxpl_id, req);

    //printf("------- PYTHON H5Oopen\n");
    return (void *)new_obj;
}

static herr_t 
H5VL_python_object_specific(void *obj, H5VL_loc_params_t loc_params, H5VL_object_specific_t specific_type, 
                         hid_t dxpl_id, void **req, va_list arguments)
{
    H5VL_python_t *o = (H5VL_python_t *)obj;

    H5VLobject_specific(o->under_object, loc_params, native_plugin_id, specific_type, dxpl_id, req, arguments);

    //printf("------- PYTHON Object specific\n");
    return 1;
}
/* Dataset callbacks Implementation*/
static void *
H5VL_python_dataset_create(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id, void **req) 
{
    H5VL_python_t *dset;
    H5VL_python_t *o = (H5VL_python_t *)obj;

    dset = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));

    dset->under_object = H5VLdataset_create(o->under_object, loc_params, native_plugin_id, name, dcpl_id,  dapl_id, dxpl_id, req);
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_dataset_create"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(7);
	//TODO: struct pointer
	PyTuple_SetItem(pArgs, 0, PyCapsule_New(obj, "obj", NULL));
	//TODO: struct 
	PyTuple_SetItem(pArgs, 1, PyCapsule_New(&loc_params, "loc", NULL));
	PyTuple_SetItem(pArgs, 2, PyString_FromString(name));
        PyTuple_SetItem(pArgs, 3, PyLong_FromLong(dcpl_id));
        PyTuple_SetItem(pArgs, 4, PyLong_FromLong(dapl_id));
        PyTuple_SetItem(pArgs, 5, PyLong_FromLong(dxpl_id));
	if(req!=NULL)
         PyTuple_SetItem(pArgs, 6, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 6, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
                printf("------- Result of H5Dcreate from python: %ld\n", PyInt_AsLong(pValue));
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return NULL;
        }
        Py_XDECREF(pArgs);
     }
     else {
        fprintf(stderr, "------- PYTHON H5Dcreate failed\n");
        return NULL;
     }
    }
    else {
        fprintf(stderr, "------- Python module :%s is not available\n",args[0]);
    }
    //printf ("------- PYTHON H5Dcreate\n");
    //TODO: figureout the file object
    return (void *) dset;
}

static void *
H5VL_python_dataset_open(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dapl_id, hid_t dxpl_id, void **req)
{
    H5VL_python_t *dset;
    H5VL_python_t *o = (H5VL_python_t *)obj;

    dset = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));

    dset->under_object = H5VLdataset_open(o->under_object, loc_params, native_plugin_id, name, dapl_id, dxpl_id, req);
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_dataset_open"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(6);
        //TODO: struct pointer
        PyTuple_SetItem(pArgs, 0, PyCapsule_New(obj, "obj", NULL));
        //TODO: struct 
        PyTuple_SetItem(pArgs, 1, PyCapsule_New(&loc_params, "loc", NULL));
        PyTuple_SetItem(pArgs, 2, PyString_FromString(name));
        PyTuple_SetItem(pArgs, 3, PyLong_FromLong(dapl_id));
        PyTuple_SetItem(pArgs, 4, PyLong_FromLong(dxpl_id));
        if(req!=NULL)
         PyTuple_SetItem(pArgs, 5, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 5, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
                printf("------- Result of H5Dopen from python: %ld\n", PyInt_AsLong(pValue));
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return NULL;
        }
        Py_XDECREF(pArgs);
     } 
     else {
        fprintf(stderr, "------- PYTHON H5Dcreate failed\n");
        return NULL;
     }  
    }   
    else {
        fprintf(stderr, "------- Python module :%s is not available\n",args[0]);
    }   
    //printf ("------- PYTHON H5Dcreate\n");
    //TODO: figureout the file object
    return (void *) dset;
}

static herr_t 
H5VL_python_dataset_read(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                      hid_t file_space_id, hid_t plist_id, void *buf, void **req)
{
    H5VL_python_t *d = (H5VL_python_t *)dset;

    H5VLdataset_read(d->under_object, native_plugin_id, mem_type_id, mem_space_id, file_space_id, 
                     plist_id, buf, req);
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_dataset_read"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(7);
	//TODO: struct pointer
        PyTuple_SetItem(pArgs, 0, PyCapsule_New(dset, "dset", NULL));
        PyTuple_SetItem(pArgs, 1, PyLong_FromLong(mem_type_id));
        PyTuple_SetItem(pArgs, 2, PyLong_FromLong(mem_space_id));
        PyTuple_SetItem(pArgs, 3, PyLong_FromLong(file_space_id));
        PyTuple_SetItem(pArgs, 4, PyLong_FromLong(plist_id));
	//TODO: buf must not be NULL, right?
        PyTuple_SetItem(pArgs, 5, PyCapsule_New(buf, "buf", NULL));
        //req can be NULL
        if(req!=NULL)
         PyTuple_SetItem(pArgs, 6, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 6, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
                printf("------- Result of H5Dread from python: %ld\n", PyInt_AsLong(pValue));
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return -1;
        }
        Py_XDECREF(pArgs);
     } 
     else {
        fprintf(stderr, "------- PYTHON H5Dread failed\n");
        return -1;
     }
    }
    else {
        fprintf(stderr, "------- Python module :%s is not available\n",args[0]);
    }
    //printf ("------- PYTHON H5Dread\n");
    return 1;     
}
static herr_t 
H5VL_python_dataset_write(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                       hid_t file_space_id, hid_t plist_id, const void *buf, void **req)
{
    H5VL_python_t *d = (H5VL_python_t *)dset;

    H5VLdataset_write(d->under_object, native_plugin_id, mem_type_id, mem_space_id, file_space_id, 
                     plist_id, buf, req);
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_dataset_write"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(7);
        //TODO: struct pointer
        PyTuple_SetItem(pArgs, 0, PyCapsule_New(dset, "dset", NULL));
        PyTuple_SetItem(pArgs, 1, PyLong_FromLong(mem_type_id));
        PyTuple_SetItem(pArgs, 2, PyLong_FromLong(mem_space_id));
        PyTuple_SetItem(pArgs, 3, PyLong_FromLong(file_space_id));
        PyTuple_SetItem(pArgs, 4, PyLong_FromLong(plist_id));
        //TODO: buf must not be NULL, right?
        PyTuple_SetItem(pArgs, 5, PyCapsule_New((void *)buf, "buf", NULL));
        //req can be NULL
        if(req!=NULL)
         PyTuple_SetItem(pArgs, 6, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 6, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
                printf("------- Result of H5Dwrite from python: %ld\n", PyInt_AsLong(pValue));
        }
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return -1;
        }
        Py_XDECREF(pArgs);
     } 
     else {
        fprintf(stderr, "------- PYTHON H5Dwrite failed\n");
        return -1;
     }  
    }   
    else {
        fprintf(stderr, "------- Python module :%s is not available\n",args[0]);
    }   
    //printf ("------- PYTHON H5Dwrite\n");
    return 1;     

}
static herr_t 
H5VL_python_dataset_close(void *dset, hid_t dxpl_id, void **req)
{
    H5VL_python_t *d = (H5VL_python_t *)dset;

    H5VLdataset_close(d->under_object, native_plugin_id, dxpl_id, req);
    free(d);
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue=NULL;
    char * args [] ={"python_vol","H5VL_python_dataset_close"};
    pModule = PyImport_ImportModule(args[0]);
    if (pModule != NULL) {
     pFunc = PyObject_GetAttrString(pModule, args[1]);
     if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(3);
        //TODO: struct pointer
        PyTuple_SetItem(pArgs, 0, PyCapsule_New(dset, "dset", NULL));
        PyTuple_SetItem(pArgs, 1, PyLong_FromLong(dxpl_id));
        //req can be NULL
        if(req!=NULL)
         PyTuple_SetItem(pArgs, 2, Py_BuildValue("O",PyCapsule_New(req, "req", NULL)));
        else
         PyTuple_SetItem(pArgs, 2, PyString_FromString("None"));
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
                printf("------- Result of H5Dclose from python: %ld\n", PyInt_AsLong(pValue));
	}
        else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return -1;
        }
        Py_XDECREF(pArgs);
     }
     else {
        fprintf(stderr, "------- PYTHON H5Dclose failed\n");
        return -1;
     }
    }
    else {
        fprintf(stderr, "------- Python module :%s is not available\n",args[0]);
    }
    //printf ("------- PYTHON H5Dclose\n");
    return 1;
}
#if 0
static void *H5VL_python_attr_create(void *obj, H5VL_loc_params_t loc_params, const char *attr_name, hid_t acpl_id, hid_t aapl_id, hid_t dxpl_id, void **req){    
static herr_t H5VL_python_attr_close(void *attr, hid_t dxpl_id, void **req){

/* Datatype callbacks */


/* Dataset callbacks */
static void *H5VL_python_dataset_create(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id, void **req){
static herr_t H5VL_python_dataset_close(void *dset, hid_t dxpl_id, void **req){

/* File callbacks */

    
static void *H5VL_python_file_open(const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req){


/* Group callbacks */

static void *H5VL_python_group_open(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t gapl_id, hid_t dxpl_id, void **req){
static herr_t H5VL_python_group_get(void *obj, H5VL_group_get_t get_type, hid_t dxpl_id, void **req, va_list arguments){


/* Link callbacks */

/* Object callbacks */


#endif

//#endif
