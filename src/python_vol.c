//#ifndef H5_VOL_H
//#define H5_VOL_H
#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hdf5.h"
#include "H5private.h"          /* Generic Functions                    */

#include "H5Dprivate.h"         /* Datasets                             */
//#include "H5Eprivate.h"         /* Error handling                       */
#include "H5Fprivate.h"         /* Files                                */
#include "H5FDprivate.h"        /* File drivers                         */
//#include "H5FFprivate.h"        /* Fast Forward                         */
#include "H5Iprivate.h"         /* IDs                                  */
//#include "H5Mprivate.h"         /* Maps                                 */
//#include "H5MMprivate.h"        /* Memory management                    */
//#include "H5Opkg.h"             /* Objects                              */
#include "H5Pprivate.h"         /* Property lists                       */
#include "H5Sprivate.h"         /* Dataspaces                           */
//#include "H5TRprivate.h"        /* Transactions                         */
#include "H5VLprivate.h"        /* VOL plugins                          */

#include "python_vol.h"
#include "inttypes.h"
#define PYTHON 502
#define NPY_NO_DEPRECATED_API NPY_1_13_API_VERSION
typedef struct H5VL_python_fapl_t {
    MPI_Comm    comm; /*communicator*/
    MPI_Info    info; /*file information*/
}H5VL_python_fapl_t;

/* Stack allocation sizes */
#define H5VL_PYTHON_FOI_BUF_SIZE 1024
#define H5VL_PYTHON_LINK_VAL_BUF_SIZE 256
#define H5VL_PYTHON_GINFO_BUF_SIZE 256
#define H5VL_PYTHON_DINFO_BUF_SIZE 1024
#define H5VL_PYTHON_SEQ_LIST_LEN 128

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
//static herr_t H5VL_python_dataset_get(void * obj, H5VL_dataset_get_t get_type, hid_t dxpl_id, void H5_ATTR_UNUSED **req, va_list arguments);
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
    void  *under_object;
    char * file_name;
    size_t file_name_len;
    unsigned flags;
    hid_t fcpl_id;
    hid_t fapl_id;
    MPI_Comm comm;
    MPI_Info info;
    int my_rank;
    int num_nprocs;
    hid_t type_id;
    hid_t space_id;
    hid_t dcpl_id;
    hid_t dapl_id;
} H5VL_python_t;
typedef struct H5VL_DT {
    int  ndims;	  // Acquired with H5Sget_simple_extent_ndims
    hsize_t * dims;      // Acquired with H5Sget_simple_extent_dims 
    hsize_t * maxdims;   // Acquired with H5Sget_simple_extent_dims
    int  py_type;        // Acquired with H5T_get_class, Support H5T_INTEGER, 16/32, FLOAT 32/64 for now. 
} H5VL_DT;


PyObject * pInstance=NULL;

/* H5Pset_fapl_swift
 * update file access property list with MPI info

*/
herr_t H5Pset_fapl_swift(hid_t fapl, const char plugin, MPI_Comm comm, MPI_Info info){

   H5VL_python_fapl_t fa;
   
   //register swift vol first
   hid_t vol_id = H5VLregister (&H5VL_python_g);
   //verify vol registered 
   assert(H5VLis_registered(plugin) ==1 );
   //setup MPI info
   fa.comm = comm;
   fa.info = info; 
   return H5Pset_vol(fapl, vol_id, &fa); 
}
/* File callbacks Implementation*/
static void *
H5VL_python_file_create(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id, hid_t dxpl_id, void **req)
{
    //hid_t under_fapl;
    H5VL_python_t *file=NULL;
    H5VL_python_fapl_t *fa=NULL;
    H5P_genplist_t *plist = NULL;      /* Property list pointer */
    //Allocate memory for file object
    file = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));
    file->fcpl_id = FAIL;
    file->fapl_id = FAIL;

    /*Get information from fapl*/
    /* Get information from the FAPL */
    if(NULL == (plist = H5P_object_verify(fapl_id, H5P_FILE_ACCESS)))
        //HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, NULL, "not a file access property list")
	printf("not a file access property list\n");
    if(NULL == (fa = (H5VL_python_fapl_t *)H5P_get_vol_info(plist)))
        //HGOTO_ERROR(H5E_FILE, H5E_CANTGET, NULL, "can't get RADOS info struct")
	printf("can't get swift info struct\n");
    //Fill in info
    file->file_name = HDstrdup(name);
    //printf("inside vol, file name got:[%s]\n",file->file_name);
    file->file_name_len = HDstrlen(name);
    file->flags = flags;
    file->fcpl_id = H5Pcopy(fcpl_id);
    file->fapl_id = H5Pcopy(fapl_id);

    /*Duplicate communicator and Info object*/
    H5FD_mpi_comm_info_dup(fa->comm, fa->info, &file->comm, &file->info);
    /* Obtain the process rank and size from the communicator attached to the
     * fapl ID */
    MPI_Comm_rank(fa->comm, &file->my_rank);
    MPI_Comm_size(fa->comm, &file->num_nprocs);
    int ipvol=0; //default is using swift for python vol
    char pvol_name[3]="py"; 
    if( H5Pexist(fapl_id, pvol_name)>0){
      H5Pget(fapl_id, pvol_name, &ipvol);
    }
    //TODO: need to write the meta into disk
    //TODO: need to create the root group
    PyObject *pModule, *pClass;
    PyObject *pValue=NULL; 
    const char module_name[ ] = "python_vol";
    char class_name[] = "swift";
    char method_name[]= "H5VL_python_file_create";
    pModule = PyImport_ImportModule(module_name); 
    // Instantiate an object
    PyErr_Print();    
    
    if(pModule != NULL){
       pInstance = PyObject_CallMethod(pModule, class_name,NULL,NULL);
       PyErr_Print();
    }
    else{
       printf("Failed to get non-null file class\n");
    }
    if(pInstance == NULL)
       printf("New File instance failed\n");
    else{
       pValue = PyObject_CallMethod(pInstance, method_name, "sllllll", name, flags, fcpl_id, fapl_id, dxpl_id, 0, 0);
       PyErr_Print();
       if (pValue != NULL) {
	     //printf("------- Result of H5Fopen from python: %ld\n", PyLong_AsLong(pValue));
             PyObject * rt=PyLong_AsVoidPtr(pValue);
             void * rt_py = rt;
	     if (rt_py==NULL) fprintf(stderr, "File create, returned pointer from python is NULL\n");
             file->under_object = rt_py;
 	     return (void *) file;
        }
        else {
             fprintf(stderr,"Call failed in H5VL File Create\n");
             return NULL;
        }	
    }
    //printf("------- PYTHON H5Fcreate\n");
    return NULL;
}

static void *
H5VL_python_file_open(const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req)

{

    PyObject *pModule=NULL, *pClass=NULL;
    PyObject *pValue=NULL;
    //printf("in H5VL file open\n");
    H5VL_python_fapl_t *fa = NULL;
    H5P_genplist_t *plist = NULL;      /* Property list pointer */
    //H5VL_python_file_t *file = NULL;
    char foi_buf_static[H5VL_PYTHON_FOI_BUF_SIZE];
    char *foi_buf_dyn = NULL;
    char *foi_buf = foi_buf_static;
    void *gcpl_buf = NULL;
    uint64_t gcpl_len;
    uint64_t root_grp_oid;
    hbool_t must_bcast = FALSE;
    uint8_t *p;
    int ret;
    void *ret_value = NULL;
    hid_t under_fapl;
    H5VL_python_t *file =NULL;
   //Allocate file object 
    if(NULL==(file = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t))))
      //HGOTO_ERROR(H5E_FILE, H5E_CANTALLOC, NULL, "can't allocate RADOS file struct");
	printf("can't allocate swift file struct\n");
    //under_fapl = *((hid_t *)H5Pget_vol_info(fapl_id));
    //printf("Testing H5VL file open\n");
    //const char class_name[ ] = "H5PVol";
    int ipvol=0; //default is using swift for python vol
    char pvol_name[3]="py";
   /* Get information from the FAPL */
    if(NULL == (plist = H5P_object_verify(fapl_id, H5P_FILE_ACCESS)))
        //HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, NULL, "not a file access property list")
	printf("not a file access property list\n");
    if(NULL == (fa = (H5VL_python_fapl_t *)H5P_get_vol_info(plist))) //get mpi infor from fapl id
        //HGOTO_ERROR(H5E_SYM, H5E_CANTGET, NULL, "can't get python info struct")
	printf("can't get python info struct\n");
    if(NULL == (file->file_name = HDstrdup(name)))
        //HGOTO_ERROR(H5E_RESOURCE, H5E_CANTALLOC, NULL, "can't copy file name")
	printf("can't copy file name\n");
    file->file_name_len = HDstrlen(name);
    file->flags = flags;
    if((file->fapl_id = H5Pcopy(fapl_id)) < 0) // keep the fapl in memory, for later reference
        //HGOTO_ERROR(H5E_FILE, H5E_CANTCOPY, NULL, "failed to copy fapl")
	printf("failed to copy fapl\n");
    /* Duplicate communicator and Info object. */
    if(FAIL == H5FD_mpi_comm_info_dup(fa->comm, fa->info, &file->comm, &file->info)) //copy from fa to file
        //HGOTO_ERROR(H5E_INTERNAL, H5E_CANTCOPY, NULL, "Communicator/Info duplicate failed")
	printf("Communicator/Info duplicate failed\n");
    /* Obtain the process rank and size from the communicator attached to the
     * fapl ID */
    MPI_Comm_rank(fa->comm, &file->my_rank);
    MPI_Comm_size(fa->comm, &file->num_nprocs);
    //printf("number of processes: %d\n",file->num_nprocs);
    if( H5Pexist(fapl_id, pvol_name)>0){
      H5Pget(fapl_id, pvol_name, &ipvol);
    }
    char class_name[] = "swift";
    const char module_name[]= "python_vol";
    pModule = PyImport_ImportModule(module_name);
    if(pModule != NULL){
      pInstance = PyObject_CallMethod(pModule, class_name,NULL,NULL);
      PyErr_Print();
    }
    else{
       printf("Failed to get non-null file class\n");
    }
   //file->under_object = H5VLfile_open(name, flags, under_fapl, dxpl_id, req);
    char method_name[]= "H5VL_python_file_open";
    if(pInstance == NULL)
       fprintf(stderr, "New File instance failed\n");
    else{
       pValue = PyObject_CallMethod(pInstance, method_name, "slllll", name, flags, fapl_id, dxpl_id, 0, 0);
       //PyErr_Print();
       if (pValue != NULL) {
             //printf("------- Result of H5Fopen from python: %ld\n", PyLong_AsLong(pValue));
             PyObject * rt=PyLong_AsVoidPtr(pValue);
             void * rt_py = rt;
             if (rt_py==NULL) fprintf(stderr, "File open, returned pointer from python is NULL\n");
             file->under_object = rt_py;
             return (void *) file;
        }
        else {
             fprintf(stderr,"Call failed in H5VL File Open\n");
             return NULL;
        }
    }
        printf("%s:%u\n",__func__,__LINE__);
        fflush(stdout);
    //printf("------- PYTHON H5Fopen\n");
    return NULL;	
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
    if(f==NULL || f->under_object==NULL) {
	return 1;
    }
    PyObject * plong_under = PyLong_FromVoidPtr(f->under_object);
    PyObject *pValue=NULL;
    char method_name[]= "H5VL_python_file_close";
    if(pInstance==NULL){
      printf("pInstance is NULL in file close\n");
      return -1;
    }else{
      pValue = PyObject_CallMethod(pInstance, method_name, "lll", PyLong_AsLong(plong_under), dxpl_id, 0);
      PyErr_Print();
      if(pValue !=NULL){
        free(f);
	Py_DECREF(pInstance);
	//printf("cleaning pInstance\n");
	return 1;
      }
    }
    return -1;
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
    H5FD_mpi_comm_info_dup(o->comm, o->info, &group->comm, &group->info) ; 
    //printf("number of processes inside pyvol group create:%d\n",o->num_nprocs);  
    PyObject *pArgs, *pValue=NULL;
    char method_name[]= "H5VL_python_group_create";
    if(pInstance==NULL){
      printf("pInstance is NULL in group create\n");
      return NULL; 
    }else{
      pValue = PyObject_CallMethod(pInstance, method_name, "llsllll", PyLong_AsLong(plong_under), 0, name, gcpl_id, gapl_id, dxpl_id, 0);
      if(pValue !=NULL){
	//printf("------- Result of H5Fcreate from python: %ld\n", PyLong_AsLong(pValue));	
	void * rt_py = PyLong_AsVoidPtr(pValue);
        if (rt_py==NULL) fprintf(stderr, "File create, returned pointer from python is NULL\n");
	group->under_object = rt_py;
	return (void *) group;
      }	      
    } 
    //printf ("------- PYTHON H5Gcreate\n");
    return (void *) group;
}

static herr_t 
H5VL_python_group_close(void *grp, hid_t dxpl_id, void **req)
{
    H5VL_python_t *g = (H5VL_python_t *)grp;
    PyObject * plong_under = PyLong_FromVoidPtr(g->under_object);
    PyObject *pValue=NULL;
    char method_name[]= "H5VL_python_group_close";
    if(pInstance==NULL){
      fprintf(stderr, "pInstance is NULL in group close\n");
      return 1; 
    }else{
      pValue = PyObject_CallMethod(pInstance, method_name, "lll", PyLong_AsLong(plong_under), dxpl_id, 0);
      PyErr_Print();
      if(pValue !=NULL){
        //printf("------- Result of H5Gclose from python: %ld\n", PyLong_AsLong(pValue));
        free(g);
        return 1;
      }
    }
   return -1;
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

    //printf("------- PYTHON H5Tcommit\n");
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


void helper_dt (hid_t dcpl_id, H5VL_DT * dt){
     hid_t space_id, type_id;
     H5Pget (dcpl_id, "dataset_space_id", &space_id); 
     H5Pget (dcpl_id, "dataset_type_id", &type_id);

     dt->ndims = H5Sget_simple_extent_ndims (space_id);
     //printf("Checking dt ndims:%d\n",dt->ndims);
     dt->dims = malloc(sizeof(unsigned long long int )* dt->ndims);
     dt->maxdims = malloc(sizeof(unsigned long long int)* dt->ndims);
     H5Sget_simple_extent_dims(space_id,dt->dims, dt->maxdims);
     hid_t atomic_type = H5Tget_class(type_id); // return type, see H5Tpublic.h
     size_t type_size = H5Tget_size(type_id); // return number of bytes, e.g., if int, it could be 4, 8, 16, 32, etc
     int py_type=0; // default is int16 

     if(atomic_type==0||atomic_type==1){
       int nbits = type_size*8; 
       if(nbits==16&&(int)atomic_type==0)      py_type=0; // int16
       else if(nbits==32&&(int)atomic_type==0) py_type=1; // int32
       else if(nbits==32&&(int)atomic_type==1) py_type=2; // float32
       else if(nbits==64&&(int)atomic_type==1) py_type=3; // float64
       else{fprintf(stderr, "type is not supported, only support int16, int32, float32, double\n");}
     }
     else fprintf(stderr, "type is not supported, only support int and float\n");
     dt->py_type=py_type;
}
//check vol layer, figure out what is calling this callback. 
static void *
H5VL_python_dataset_create(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id, void **req) 
{
    H5VL_python_t *dset;
    H5VL_python_t *o = (H5VL_python_t *)obj;
    PyObject * plong_under = PyLong_FromVoidPtr(o->under_object);
    dset = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));
    H5FD_mpi_comm_info_dup(o->comm, o->info, &dset->comm, &dset->info);
    //printf("number of processes inside pyvol dataset_create:%d\n",o->num_nprocs);
    PyObject *pValue=NULL;
    char method_name[] = "H5VL_python_dataset_create";
    size_t type_size = 0;
    size_t space_size = 0;
    hid_t type_id, space_id;
    void *type_buf = NULL;
    void *space_buf = NULL;
    H5P_genplist_t *plist = NULL;      /* Property list pointer */


    plist = (H5P_genplist_t *)H5I_object(dcpl_id);
  
    import_array();
    H5VL_DT * dt= malloc(sizeof(H5VL_DT)); //get the dataset size, type
    helper_dt (dcpl_id, dt);
    //printf("Testing H5VL_DT,%d\n",dt->py_type); 
    npy_intp dtm=dt->ndims; 
    PyObject * py_dims = PyArray_SimpleNewFromData(1, &dtm, NPY_INTP,dt->dims);//TODO: 64 bits, vs 32 bit 
    PyObject * py_maxdims = PyArray_SimpleNewFromData(1, &dtm, NPY_INTP,dt->maxdims );  
    /* Get the dcpl plist structure */
    //plist = (H5P_genplist_t *)H5I_object(dcpl_id);
    //printf("[][][][]dcpl_id:[%lu][%llx]\n",dcpl_id,dcpl_id);
    /*
    if(NULL == (plist = (H5P_genplist_t *)H5I_object(dcpl_id)))
        HGOTO_ERROR(H5E_ATOM, H5E_BADATOM, NULL, "can't find object for ID")
    */
    /* get creation properties */
    H5P_get(plist, H5VL_PROP_DSET_TYPE_ID, &type_id);
    H5P_get(plist, H5VL_PROP_DSET_SPACE_ID, &space_id);
    printf("type_id:%u,space_id:%u\n",type_id,space_id);
    fflush(stdout);
    //printf("Get again,type id:[%lu][%llx],space_id:[%lu][%llx]\n",type_id,type_id,space_id,space_id);
    /*
    if(H5P_get(plist, H5VL_PROP_DSET_TYPE_ID, &type_id) < 0)
        HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, NULL, "can't get property value for datatype id")
    if(H5P_get(plist, H5VL_PROP_DSET_SPACE_ID, &space_id) < 0)
        HGOTO_ERROR(H5E_PLIST, H5E_CANTGET, NULL, "can't get property value for space id")
    */
    fflush(stdout);
    if(pInstance==NULL){
      fprintf(stderr, "pInstance is NULL in group create\n");
      return NULL; 
    }else{
	pValue = PyObject_CallMethod(pInstance, method_name, "llsllllilOO", 
		PyLong_AsLong(plong_under), 0, name, dcpl_id, dapl_id, 
		dxpl_id, 0,dt->ndims,dt->py_type,py_dims, py_maxdims);
     if(pValue !=NULL){
        //printf("------- Result of H5Dcreate from python: %ld\n", PyLong_AsLong(pValue));
        void * rt_py = PyLong_AsVoidPtr(pValue);
        if (rt_py==NULL) fprintf(stderr, "Dataset create, returned pointer from python is NULL\n");
        dset->under_object = rt_py;

	/*
        dset->space_id=H5Scopy(space_id);//space_id;
	printf("Finally[][][]dset->space_id:[%llx],[%llx]\n",dset->space_id,space_id);
	fflush(stdout);
	printf("do h5scopy now, line 681\n");
	dset->type_id=H5Scopy(type_id);
	fflush(stdout);
	*/
	//dset->type_id = H5Tcopy(type_id);
	//dset->space_id = H5Tcopy(space_id);
	dset->space_id =space_id;
	dset->type_id = type_id;
	return (void *) dset;
      }      
    } 
    return NULL;

}
static void *
H5VL_python_dataset_open(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dapl_id, hid_t dxpl_id, void **req)
{
    H5VL_python_t *dset;
    H5VL_python_t *o = (H5VL_python_t *)obj;
    PyObject * plong_under = PyLong_FromVoidPtr(o->under_object);
    dset = (H5VL_python_t *)calloc(1, sizeof(H5VL_python_t));
    H5FD_mpi_comm_info_dup(o->comm, o->info, &dset->comm, &dset->info);
    //retrieve metadata and fill in memory buffer for later using. 
    PyObject *pValue=NULL;
    /* H5P_genplist_t * plist = NULL;
    hid_t type_id, space_id;
    plist = (H5P_genplist_t *)H5I_object(dapl_id);
    // get creation properties 
    H5P_get(plist, H5VL_PROP_DSET_TYPE_ID, &type_id);
    H5P_get(plist, H5VL_PROP_DSET_SPACE_ID, &space_id);
    printf("rank:%d, dataset open: dapl_id:%u, space_id:%u\n",o->my_rank,dapl_id,space_id);
    */    char method_name[] = "H5VL_python_dataset_open";
    if(pInstance==NULL){
      fprintf(stderr, "pInstance is NULL in dataset open\n");
      return NULL;
    }else{
      pValue = PyObject_CallMethod(pInstance, method_name, "llslll", PyLong_AsLong(plong_under), 0, name, dapl_id, dxpl_id,0 );
      if(pValue !=NULL){
        //printf("------- Result of H5Dopen from python: %ld\n", PyLong_AsLong(pValue));
        void * rt_py = PyLong_AsVoidPtr(pValue);
        if (rt_py==NULL) fprintf(stderr, "Dataset open, returned pointer from python is NULL\n");
        dset->under_object = rt_py;
//	dset->space_id = space_id;
//	dset->type_id = type_id;
        return (void *) dset;
      }
    }
    return NULL;
}
PyObject * Data_CPY(long dsetId, void * buf,hsize_t * nelem, int * type_size)
{
    char dt_name[] = "H5VL_python_dt_info";
    npy_intp ndims=0,dtype=0;
    npy_intp *dims=NULL;
    /*if((h5_ndims = H5Sget_simple_extent_ndims(dset->space_id)) < 0)
        HGOTO_ERROR(H5E_DATASET, H5E_CANTGET, FAIL, "can't get number of dimensions")
    if(ndims != H5Sget_simple_extent_dims(dset->space_id, dim, NULL))
        HGOTO_ERROR(H5E_DATASET, H5E_CANTGET, FAIL, "can't get dimensions")
    */
    import_array();
    //retrieve the dataset information based dataset id in python vol layer. 
    if(pInstance!=NULL){
       PyObject * dt_obj= PyObject_CallMethod(pInstance, dt_name, "l",dsetId);
       if(dt_obj==NULL){
        fprintf(stderr, "dt_Obj is null\n");
        return NULL; 
       }
       PyArrayObject * dt_arr=(PyArrayObject *)dt_obj;
       //convert back to c array
       if(dt_arr->descr->type_num>=0){
         npy_intp * dt_if =(npy_intp *) dt_arr->data;
         ndims=dt_if[0];
         dtype=dt_if[1];
         dims=dt_if+2; //pointer starts from 3rd element
       }else{
        printf("dt_arr.type_num:%d is not PyArray_LONG\n",dt_arr->descr->type_num);
       }

    }
    else{
       fprintf(stderr, "pInstance is NULL\n");
       return NULL; 
    }
    //Create pyobject reference to c buffer
    PyObject * pydata;
    if (dtype == 0){//int16 
      pydata = PyArray_SimpleNewFromData(ndims, dims, NPY_INT16, buf );
      *type_size=(int)sizeof(short int);
    }
    else if (dtype == 1){//int32
      pydata = PyArray_SimpleNewFromData(ndims, dims, NPY_INT32, buf );
      *type_size=(int)sizeof(int);
    }
    else if (dtype == 2) {//float32
      pydata = PyArray_SimpleNewFromData(ndims, dims, NPY_FLOAT, buf );
      *type_size=(int)sizeof(float);
    }
    else if (dtype == 3) {//float64
      pydata = PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, buf );
      *type_size=(int)sizeof(double);
    }
    else {
      fprintf(stderr, "Type is not supported for now Jan 31 2018\n");
      return NULL; 
    }
    //convert to C-contiguous array
    PyObject * pydata_c = PyArray_FROM_OF(pydata, NPY_ARRAY_C_CONTIGUOUS);
    npy_intp x=1;
    int i;
    for(i=0;i<ndims;i++) x*=dims[i];
    *nelem = (hsize_t)x;
    return pydata_c;
}
PyObject * Data_CPY2(long dsetId, void * buf, H5VL_python_t * dset)
{
    char dt_name[] = "H5VL_python_dt_info";
    npy_intp ndims=0,dtype=0;
    npy_intp *dims=NULL;
    hsize_t h5_dims[H5S_MAX_RANK];
    //int h5_ndims; 
    //h5_ndims = H5Sget_simple_extent_ndims(dset->space_id);
    //H5Sget_simple_extent_dims(dset->space_id, h5_dims, NULL);
    /*if((h5_dims = H5Sget_simple_extent_ndims(dset->space_id)) < 0)
        HGOTO_ERROR(H5E_DATASET, H5E_CANTGET, FAIL, "can't get number of dimensions")
    if(h5_ndims != H5Sget_simple_extent_dims(dset->space_id, dim, NULL))
        HGOTO_ERROR(H5E_DATASET, H5E_CANTGET, FAIL, "can't get dimensions")
    */
    /*int x=0;
    printf("In Data_CPY2:\n");
    for(x=0;x<h5_ndims;x++){
	printf("%d-th dims is:%lu\n",x,(unsigned long)h5_dims[x]);	
    }
    */
    import_array();
    //retrieve the dataset information based dataset id in python vol layer. 
    if(pInstance!=NULL){
       PyObject * dt_obj= PyObject_CallMethod(pInstance, dt_name, "l",dsetId);
       if(dt_obj==NULL){
        fprintf(stderr, "dt_Obj is null\n");
        return NULL;
       }
       PyArrayObject * dt_arr=(PyArrayObject *)dt_obj;
       //convert back to c array
       if(dt_arr->descr->type_num>=0){
         npy_intp * dt_if =(npy_intp *) dt_arr->data;
         ndims=dt_if[0];
         dtype=dt_if[1];
         dims=dt_if+2; //pointer starts from 3rd element
       }else{
        printf("dt_arr.type_num:%d is not PyArray_LONG\n",dt_arr->descr->type_num);
       }
    }
    else{
       fprintf(stderr, "pInstance is NULL\n");
       return NULL;
    }
    //Create pyobject reference to c buffer

    PyObject * pydata;
    if (dtype == 0){//int16 
      printf("dtype is 0---------------------------------------->\n");
      pydata = PyArray_SimpleNewFromData(ndims, dims, NPY_INT16, buf );
    }
    else if (dtype == 1){//int32
      pydata = PyArray_SimpleNewFromData(ndims, dims, NPY_INT32, buf );
    }
    else if (dtype == 2) {//float32
      pydata = PyArray_SimpleNewFromData(ndims, dims, NPY_FLOAT, buf );
    }
    else if (dtype == 3) {//float64
      pydata = PyArray_SimpleNewFromData(ndims, dims, NPY_DOUBLE, buf );
    }
    else {
      fprintf(stderr, "Type is not supported for now Jan 31 2018\n");
      return NULL;
    }
    //convert to C-contiguous array
    PyObject * pydata_c = PyArray_FROM_OF(pydata, NPY_ARRAY_C_CONTIGUOUS);
    return pydata_c;
}

PyObject * Data_CPY4(long dsetId, void * buf, size_t len)
{
    char dt_name[] = "H5VL_python_dt_info";
    npy_intp ndims[1];
    npy_intp dtype=2;
    npy_intp dims=len;
    import_array();
    //retrieve the dataset information based dataset id in python vol layer. 
    if(pInstance!=NULL){
       PyObject * dt_obj= NULL; 
       dt_obj = PyObject_CallMethod(pInstance, dt_name, "l",dsetId);
	PyErr_Print();
       if(dt_obj==NULL){
        fprintf(stderr, "dt_Obj is null\n");
        return NULL;
       }
       PyArrayObject * dt_arr=(PyArrayObject *)dt_obj;
       //convert back to c array
       if(dt_arr->descr->type_num>=0){
         npy_intp * dt_if =(npy_intp *) dt_arr->data;
         dtype = dt_if[1];
       }else{
        printf("dt_arr.type_num:%d is not PyArray_LONG\n",dt_arr->descr->type_num);
       }
    }
    else{
       fprintf(stderr, "pInstance is NULL\n");
       return NULL;
    }

    //Create pyobject reference to c buffer
    PyObject * pydata=NULL;
    if (dtype == 0){//int16
	pydata = PyArray_SimpleNewFromData(1, &dims, NPY_INT16, buf );
    }
    else if (dtype == 1){//int32
      pydata = PyArray_SimpleNewFromData(1, &dims, NPY_INT32, buf );
    }
    else if (dtype == 2) {//float32
      pydata = PyArray_SimpleNewFromData(1, &dims, NPY_FLOAT, buf );
    }
    else if (dtype == 3) {//float64
      
      pydata = PyArray_SimpleNewFromData(1, &dims, NPY_DOUBLE, buf );
    }
    else {
      fprintf(stderr, "Type is not supported for now Jan 31 2018\n");
      return NULL;
    }

    //convert to C-contiguous array

    if(pydata==NULL) 
	printf("pydata in CPY4 is null\n");
    else{
    	PyObject * pydata_c = PyArray_FROM_OF(pydata, NPY_ARRAY_C_CONTIGUOUS);
	return pydata_c;		
    }
    return NULL;
}

PyObject * Data_CPY3(void * buf, int len, int dtype)
{
    npy_intp dims=len; //TODO: this is something to be fixed later, the limit of PyArray_SimpleNewFromData
    //Create pyobject reference to c buffer
    import_array();
    PyObject * pydata=NULL;
    if (dtype == 0){//int16 
      pydata = PyArray_SimpleNewFromData(1, &dims, NPY_INT16, buf );
    }
    else if (dtype == 1){//int32
      pydata = PyArray_SimpleNewFromData(1, &dims, NPY_INT32, buf );
      PyErr_Print();
   }
    else if (dtype == 2) {//float32
      pydata = PyArray_SimpleNewFromData(1, &dims, NPY_FLOAT, buf );
    }
    else if (dtype == 3) {//float64
      pydata = PyArray_SimpleNewFromData(1, &dims, NPY_DOUBLE, buf );
    }
    else if (dtype == 4) { //unsigned int
      pydata = PyArray_SimpleNewFromData(1, &dims, NPY_UINT, buf );
    }
    else {
      fprintf(stderr, "Type is not supported for now Jan 31 2018\n");
      return NULL;
    }
    //convert to C-contiguous array
    PyObject * pydata_c = PyArray_FROM_OF(pydata, NPY_ARRAY_C_CONTIGUOUS);
    return pydata_c;
}

/*-------------------------------------------------------------------------
 * Function:    H5VL_python_dataset_read
 *
 * Purpose:     Reads raw data from a dataset into a buffer.
 *`
 * Return:      Success:        0
 *              Failure:        -1, dataset not read.
 *
 * Programmer:  Jialin Liu
 *              May, 2018
 *
 *-------------------------------------------------------------------------
 */
static herr_t 
H5VL_python_dataset_read(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                      hid_t file_space_id, hid_t plist_id, void *buf, void **req)
{
    hssize_t num_elem,num_elem_memory,num_elem_file;
    H5S_sel_iter_t mem_sel_iter, file_sel_iter;    /* Selection iteration info */
    hbool_t mem_sel_iter_init = FALSE, file_sel_iter_init = FALSE;      /* Selection iteration info has been initialized */
    hid_t real_file_space_id,real_mem_space_id;
    H5VL_python_t *o = (H5VL_python_t *)dset;
    PyObject * plong_under = PyLong_FromVoidPtr(o->under_object);
    H5S_sel_iter_t sel_iter;    /* Selection iteration info */
    hbool_t sel_iter_init = FALSE;      /* Selection iteration info has been initialized */
    int ndims;
    hsize_t dim[H5S_MAX_RANK];
    MPI_Comm_rank(o->comm, &o->my_rank);
    MPI_Comm_size(o->comm, &o->num_nprocs);

    size_t  type_size = H5Tget_size(mem_type_id);
    //TODO:dataspace info needs to be stored during dataset create, then read from memory into buffer at here
    /*if((ndims = H5Sget_simple_extent_ndims(o->space_id)) < 0)
        printf("can't get number of dimensions\n");
    printf("ndims:%d\n",ndims);
    if(ndims != H5Sget_simple_extent_dims(o->space_id, dim, NULL))
        printf("can't get dimensions\n");
    printf("dims[0]:%d\n",dim[0]);
    printf("space id:%u\n",o->space_id);
   */
    hid_t type_id=0,space_id=0;
    //H5P_get(plist_id, H5VL_PROP_DSET_TYPE_ID, &type_id);
    //H5P_get(plist_id, H5VL_PROP_DSET_SPACE_ID, &space_id);
    //printf("rank:%d, type_id:%u,space_id:%u\n",o->my_rank, type_id,space_id);
    //fflush(stdout);
    //o->space_id = space_id;
    //o->type_id = type_id;
//VERIFY HYPERSLAB SELECTION 
    /* Get "real" file space */
    //if(file_space_id == H5S_ALL)
        //real_file_space_id = o->space_id;
//	real_file_space_id = space_id;
//    else
        real_file_space_id = file_space_id;

    /* Get number of elements in file space selection */
    if((num_elem_file = H5Sget_select_npoints(real_file_space_id)) < 0)
        printf("can't get number of points in file selection\n");

    real_mem_space_id = mem_space_id;

    /* Get number of elements in memory space selection */
    if((num_elem_memory = H5Sget_select_npoints(real_mem_space_id)) < 0)
        printf("can't get number of points in memory selection\n");


    if(num_elem_file != num_elem_memory){
        printf("file selection does not equal memory selection\n");
    }
    // Assuming H5Pget_layout returns H5D_CONTIGUOUS, skip cases of H5D_COMPACT, and H5D_CHUNKED
    // skip type checking
    // Initialize selection iterators
    H5S_t * mem_space_obj = (H5S_t *) H5I_object_verify(real_mem_space_id, H5I_DATASPACE);
    H5S_t * file_space_obj = (H5S_t *) H5I_object_verify(real_file_space_id, H5I_DATASPACE);
    if(H5S_select_iter_init(&mem_sel_iter, mem_space_obj, type_size) < 0)
        printf("unable to initialize selection iterator\n");
    mem_sel_iter_init = TRUE;       /* Selection iteration info has been initialized */

    if(H5S_select_iter_init(&file_sel_iter, file_space_obj, type_size) < 0)
        printf("unable to initialize selection iterator\n");
    file_sel_iter_init = TRUE;       /* Selection iteration info has been initialized */
//GET SELECTIONS

    //calculate file space offset from file space selection 
    size_t mem_i=0, file_i=0;
    size_t tot_len = num_elem_memory * type_size;
    size_t tot_len2=tot_len;
    size_t mem_nseq = 0, file_nseq=0;
    size_t nelem;
    size_t mem_off[128], file_off[128];    
    size_t mem_len[128], file_len[128];
    size_t io_len;
    size_t * meta_offlen= (size_t *) malloc (3*sizeof(size_t));
    size_t cur_metal = 3;
    do {
	if(file_i == file_nseq){
		if(H5S_SELECT_GET_SEQ_LIST(file_space_obj, 0, &file_sel_iter,
			(size_t)128, (size_t)-1, &file_nseq, &nelem, file_off, file_len)<0)
			printf("file sequence generation failed\n");
		meta_offlen=realloc(meta_offlen, (cur_metal + file_nseq*2)*sizeof(size_t));
		memcpy(meta_offlen+cur_metal, file_off,file_nseq*sizeof(size_t));
		//record file offset list, store in meta_offlen at meta_offlen +2
		memcpy(meta_offlen+cur_metal+file_nseq, file_len, file_nseq*sizeof(size_t));
		//record file length list, store after offset list
		cur_metal+=file_nseq*2;
		file_i = 0;
	}
	io_len = file_len[file_i];
	tot_len2 -= io_len;
	file_i++;
    } while(tot_len2 > 0);
    meta_offlen[1] = meta_offlen[3];//min file offset
    meta_offlen[2] = meta_offlen[cur_metal-2]+meta_offlen[cur_metal-1]-1;//max file offset=  last seq's off + last seq's len
    meta_offlen[0] = cur_metal;// total length of this array
    //NOW, meta_offlen has per rank's file offset/length info, and first element tells the total length of this array
    //QUERY GLOBAL METADATA
    npy_intp * gmeta=NULL;
    int gmeta_len=0;
    if(o->my_rank==0){

        //PyObject * pmeta = NULL; //malloc(sizeof(PyObject));
        long pmeta=0;
        PyObject * pValue = NULL;
        char method_name [] = "H5VL_python_dataset_read";
        if(pInstance == NULL){
                printf("pInstance is NULL in dataset read\n");
                return -1;
        } else {
		long req_meta =-2;
                pValue = PyObject_CallMethod(pInstance, method_name, "lllllll", PyLong_AsLong(plong_under), mem_type_id, mem_space_id, file_space_id,plist_id, pmeta,req_meta);
                PyErr_Print();
		PyArrayObject * dt_arr=(PyArrayObject *)pValue;
//		printf("%s:%u\n",__func__,__LINE__);
  //  		fflush(stdout); 
                if(pValue !=NULL){
                        gmeta = (npy_intp *) dt_arr->data;
                        gmeta_len =(int)gmeta[0]+1; //only fill one element, i.e., the length of metadata
//			printf("rank:%d, inside, gmeta[0]:%d\n",o->my_rank, gmeta[0]);
 //    			printf("rank:%d, inside, gmeta_len length:%d\n",o->my_rank, gmeta_len);
                }
        }

    }
    MPI_Bcast(&gmeta_len, 1, MPI_INT, 0, o->comm); // bcast total length of metadata info
    //printf("rank:%d, after bcast, gmeta_len length:%d\n",o->my_rank, gmeta_len);
    int * re_gmeta=(int *) malloc(sizeof(int)*gmeta_len);;
    if(re_gmeta==NULL) printf("rank:%d, re_gmeta is null\n",o->my_rank);
    if(o->my_rank==0){
	int km;
	for(km=0;km<gmeta_len;km++){
	   re_gmeta[km]=gmeta[km];
	}
	printf("\n");
    } 
    //allocate the meta buffer with gmeta_len
    MPI_Barrier(o->comm);
    //printf("rank:%d, %s:%u\n",o->my_rank, __func__,__LINE__);
    //fflush(stdout);
    if(re_gmeta!=NULL)
        MPI_Bcast( re_gmeta,gmeta_len, MPI_INT, 0, o->comm); // bcast metadata info: length, min_off, max_off, off1,len1,off2, len2, ...
    else 
	printf("rank:%d, re_gmeta is NULL\n",o->my_rank);
    re_gmeta=(void *) re_gmeta;
   
    PyObject * py_gmeta =NULL;
    py_gmeta = Data_CPY3(re_gmeta, gmeta_len, 1); //convert into pyobject
    PyObject * py_meta_offlen = NULL;
    py_meta_offlen = Data_CPY3(meta_offlen, gmeta_len, 1);//convert into pyobject
    char method_name_scan[] = "H5VL_python_dstobj_scan";
    PyObject * pValue_cdata =NULL;
    pValue_cdata = PyObject_CallMethod(pInstance, method_name_scan, "lOOl",PyLong_AsLong(plong_under), py_gmeta,py_meta_offlen,0);
    printf("rank:%d read done, %s:%u\n",o->my_rank, __func__,__LINE__);
    MPI_Barrier(o->comm);
    if(pValue_cdata !=NULL){
	 //void * buf1=(void *) (((PyArrayObject *) pValue_cdata)->data);
	 //buf = (int *) buf;
	 //buf1 = (int *) buf1;
	 PyErr_Print();
	 //TODO; H5cast to memory space
	 return 1;
    }
    return 1;     
}
static herr_t 
H5VL_python_dataset_write(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                       hid_t file_space_id, hid_t plist_id, const void *buf, void **req)
{

    hid_t real_file_space_id,real_mem_space_id;
    hssize_t num_elem,num_elem_memory,num_elem_file;
    H5S_sel_iter_t mem_sel_iter, file_sel_iter;    /* Selection iteration info */
    hbool_t mem_sel_iter_init = FALSE, file_sel_iter_init = FALSE;      /* Selection iteration info has been initialized */
    H5VL_python_t *o = (H5VL_python_t *)dset;
    PyObject * plong_under = PyLong_FromVoidPtr(o->under_object);
    int ndims;
    hsize_t dim[H5S_MAX_RANK];
    if((ndims = H5Sget_simple_extent_ndims(o->space_id)) < 0)
        printf("can't get number of dimensions\n");
    if(ndims != H5Sget_simple_extent_dims(o->space_id, dim, NULL))
        printf("can't get dimensions\n");
    
    /* Get "real" file space */
    if(file_space_id == H5S_ALL)
        real_file_space_id = o->space_id;
    else
        real_file_space_id = file_space_id;

    /* Get number of elements in file space selection */
    if((num_elem_file = H5Sget_select_npoints(real_file_space_id)) < 0)
        printf("can't get number of points in file selection\n");

    real_mem_space_id = mem_space_id; 

    /* Get number of elements in memory space selection */
    if((num_elem_memory = H5Sget_select_npoints(real_mem_space_id)) < 0)
        printf("can't get number of points in memory selection\n");


    if(num_elem_file != num_elem_memory){
        printf("file selection does not equal memory selection\n");
    }
    MPI_Comm_rank(o->comm, &o->my_rank);
    MPI_Comm_size(o->comm, &o->num_nprocs); 
    printf("rank:%d %s:%u\n",o->my_rank, __func__,__LINE__);
    fflush(stdout);
    // Assuming H5Pget_layout returns H5D_CONTIGUOUS, skip cases of H5D_COMPACT, and H5D_CHUNKED
    // skip type checking
    size_t  type_size = H5Tget_size(mem_type_id); 
    // Initialize selection iterators
    H5S_t * mem_space_obj = (H5S_t *) H5I_object_verify(real_mem_space_id, H5I_DATASPACE);
    H5S_t * file_space_obj = (H5S_t *) H5I_object_verify(real_file_space_id, H5I_DATASPACE);	
    if(H5S_select_iter_init(&mem_sel_iter, mem_space_obj, type_size) < 0)
        printf("unable to initialize selection iterator\n");
    mem_sel_iter_init = TRUE;       // Selection iteration info has been initialized 

    if(H5S_select_iter_init(&file_sel_iter, file_space_obj, type_size) < 0)
        printf("unable to initialize selection iterator\n");
    file_sel_iter_init = TRUE;       // Selection iteration info has been initialized 
    
    //iterate sequence from the hyperslab
    size_t mem_nseq = 0, file_nseq=0;
    size_t nelem;
    size_t mem_off[128], file_off[128]; //why 128, ask Neil/Quincey, July 19, 2018. Answer: H5S_SELECT_GET_SEQ_LIST just retrieve 128 each time
    size_t  mem_len[128], file_len[128];
    size_t io_len;
    size_t tot_len = num_elem_memory * type_size;
    size_t tot_len2 = tot_len;
    size_t total_length =tot_len;
    size_t mem_i = 0, file_i=0;
    PyObject *pValue=NULL;
    char method_name[] = "H5VL_python_dataset_write";
    //Call dataset write method
    if(pInstance==NULL){
      printf("pInstance is NULL in dataset write\n");
      return -1;
    }
    char * temp_buf=NULL;
    if(tot_len>0){
    	temp_buf = (char *)malloc(tot_len);
    }else{
	printf("total length in dataset write is 0\n");
	return 0;
    }
    if(temp_buf == NULL) {
	printf("error in allocating temp buffer\n");
	return 0;
    }
    int cur_loc = 0;
    int start_offset=0;
    size_t * meta_offlen= (size_t *) malloc (3*sizeof(size_t));
    int no_hyperslab=0;
    int meta_length =3;//first elem is total length of this array, second and third is min and max offsets
    // Generate sequences from the file space until finished 
     
    do {
	no_hyperslab++;
        // Get the sequences of bytes if necessary   // only do this during first itertion of this do-while loop, note by Jialin
        HDassert(mem_i <= mem_nseq);
        if(mem_i == mem_nseq) { //H5S_SELECT_GET_SEQ_LIST(S,FLAGS,ITER,MAXSEQ,MAXBYTES,NSEQ,NBYTES,OFF,LEN)
            if(H5S_SELECT_GET_SEQ_LIST(mem_space_obj, 0, &mem_sel_iter, (size_t)128, (size_t)-1, &mem_nseq, &nelem, mem_off, mem_len) < 0)
            	printf( "memory sequence generation failed\n");
	    mem_i = 0;
        } 
	io_len = mem_len[mem_i];
	tot_len-=io_len;
	//concatenate memory buffer
	memcpy(temp_buf+cur_loc,buf+mem_off[mem_i],mem_len[mem_i]);
	cur_loc += io_len;
        mem_i++;
    } while(tot_len > 0);
   

    printf("rank:%d %s:%u\n",o->my_rank, __func__,__LINE__);
    fflush(stdout);
    if(no_hyperslab==-1){//TODO: figure out how to check whether hyperslab is used, H5Pget_dxpl_mpio
	//do independent I/O for each rank
	PyObject * pydata = NULL; 
	pydata = Data_CPY4(PyLong_AsLong(plong_under), temp_buf, (npy_intp)num_elem_memory);
	PyErr_Print();
	long start_offset_simple=-3;
	//printf("rank:%d, going to do non hyperslab write\n",o->my_rank);
	pValue = PyObject_CallMethod(pInstance, method_name, "lllllOl", PyLong_AsLong(plong_under),  
		mem_type_id, mem_space_id, file_space_id,plist_id, pydata, start_offset_simple);
	if(temp_buf !=NULL)free(temp_buf);	
	if(pydata!=NULL) Py_DECREF(pydata);
	if(pValue == NULL) printf("error in vol\n");
    }
   
    else{
	
    	size_t cur_metal = 3;

    	do {
       		if(file_i == file_nseq){
	   		if(H5S_SELECT_GET_SEQ_LIST(file_space_obj, 0, &file_sel_iter, 
				(size_t)128, (size_t)-1, &file_nseq, &nelem, file_off, file_len)<0)
                   		printf("file sequence generation failed\n");
	   		meta_offlen=realloc(meta_offlen, (cur_metal + file_nseq*2)*sizeof(size_t));
           		memcpy(meta_offlen+cur_metal, file_off,file_nseq*sizeof(size_t)); 
				//record file offset list, store in meta_offlen at meta_offlen +2
           		memcpy(meta_offlen+cur_metal+file_nseq, file_len, file_nseq*sizeof(size_t)); 
				//record file length list, store after offset list
	   		cur_metal+=file_nseq*2;
           		file_i = 0;
       		}
       		io_len = file_len[file_i];
       		tot_len2 -= io_len;
       		file_i++;
    	} while(tot_len2 > 0);

    	int cur_mi=0;
        printf("rank:%d %s:%u\n",o->my_rank, __func__,__LINE__);
        fflush(stdout);
	if(cur_metal <=3){
		printf("cur_metal <=3,error\n");
		return 0;
	}
    	meta_offlen[1] = meta_offlen[3];//min file offset
    	meta_offlen[2] = meta_offlen[cur_metal-2]+meta_offlen[cur_metal-1] -1;//max file offset
    	meta_offlen[0] = cur_metal;// total length of this array
    	start_offset = meta_offlen[1];//append into dset name to form a unique object name: dsetname_start_offset, e.g., data_offset_32
	printf("rank:%d, meta len:%d, meta min:%d, meta max:%d\n",o->my_rank, meta_offlen[0],meta_offlen[1],meta_offlen[2]);
        fflush(stdout);
  	PyObject * pydata = Data_CPY4(PyLong_AsLong(plong_under), temp_buf, num_elem_memory); 
  	pValue = PyObject_CallMethod(pInstance, method_name, "lllllOl", 
		PyLong_AsLong(plong_under),  mem_type_id, mem_space_id, file_space_id,plist_id, pydata, start_offset);
  	// communicate metadata and ask rank 0 to write metadata as one object with name dset_gmeta
    	int elems_cur_rank = meta_offlen[0]; // this includes length of array, min_off, max_off and offset list, thus 1 + 2 + 2*num_elem_file
	//gather size info from other ranks 
    	int * receive_counts = malloc(o->num_nprocs * sizeof(int));
   	MPI_Gather(&elems_cur_rank, 1, MPI_UNSIGNED,receive_counts ,1 , MPI_UNSIGNED, 0, o->comm); // now rank 0 has all rank's metadata size info
	//figure out displs and total length
    	int * displs=NULL;
    	int total_meta_length=0;
    	int * total_meta =NULL;
    	if(o->my_rank==0){
     		displs = malloc(o->num_nprocs * sizeof(int));
     		displs[0] = 0; //rank root
     		total_meta_length += receive_counts[0];
     		int mi=1;
     		for (mi=1;mi< o->num_nprocs; mi++){
			displs[mi] = total_meta_length;
			total_meta_length += receive_counts[mi];
     		}
    		total_meta = malloc((total_meta_length+1) * sizeof(int)); 
    	}
    	//gather all metadata into total_meta
    	MPI_Gatherv(meta_offlen, elems_cur_rank, MPI_INT,
               total_meta+1, receive_counts, displs, MPI_INT,
               0, o->comm);
	//write total_meta into storage
	printf("rank:%d %s:%u\n",o->my_rank, __func__,__LINE__);
    	if(o->my_rank ==0) {
		total_meta[0] = total_meta_length;
    		PyObject * pydata_meta = Data_CPY3((void *)total_meta, total_meta_length+1, 1);
     		long meta_global_mark=-2;
     		PyObject_CallMethod(pInstance, method_name, "lllllOl", 
			PyLong_AsLong(plong_under),  mem_type_id, mem_space_id, 
		file_space_id,plist_id, pydata_meta, meta_global_mark); 
		//	if(total_meta != NULL) free(total_meta);
       		//if(pydata_meta != NULL) Py_DECREF(pydata_meta);
    	}
   	//dset_gmeta: len, len_meta1,min_off_meta1, max_off_meta1,len_meta2,min_off_meta2,max_off_meta2, ....
	//if (temp_buf != NULL ) free(temp_buf);
	//if (meta_offlen != NULL) free(meta_offlen);
	//if(receive_counts != NULL) free(receive_counts);
	//if (pydata != NULL) Py_DECREF(pydata);
    }
    return 1;     
}
static herr_t 
H5VL_python_dataset_close(void *dset, hid_t dxpl_id, void **req)
{
    H5VL_python_t *d = (H5VL_python_t *)dset;
    PyObject * plong_under = PyLong_FromVoidPtr(d->under_object);    
    PyObject *pValue=NULL;
    char method_name [] ="H5VL_python_dataset_close";
    if(pInstance==NULL){
      printf("pInstance is NULL in dataset close\n");
      return 1;
    }else{
      //printf("in C, dataset id is %ld\n",PyLong_AsLong(plong_under));
      pValue = PyObject_CallMethod(pInstance, method_name, "lll", PyLong_AsLong(plong_under), dxpl_id, 0);
      PyErr_Print(); 
      if(pValue !=NULL){
        //printf("------- Result of H5Dclose from python: %ld\n", PyLong_AsLong(pValue));
        return 1;
      }
      else
        return -1;
    }
    //printf ("------- PYTHON H5Dclose\n");
    free(d);
    return 1;
}
