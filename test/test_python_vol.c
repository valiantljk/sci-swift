#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hdf5.h"
#include "../src/python_vol.h"
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION

int main(int argc, char **argv) {
        const char file_name[]="large_dataset.h5";
	const char group_name[]="/bowling";
	const char plugin_name[7]="python";
	char fullpath_int32[100];
        char fullpath_int16[100];
        char fullpath_float32[100];
        char fullpath_float64[100];
        sprintf(fullpath_int32,  "%s/%s",group_name,"dt_int32");
        sprintf(fullpath_int16,  "%s/%s",group_name,"dt_int16");
        sprintf(fullpath_float32,"%s/%s",group_name,"dt_float32");
        sprintf(fullpath_float64,"%s/%s",group_name,"dt_float64");
	hid_t file_id, group_id, dataspaceId, datasetId, acc_tpl, under_fapl, vol_id, vol_id2, int_id, attr, space;
	int i;
	hsize_t dims[2]={6,10};
        if(argc==3){
            dims[0] = strtol(argv[1], NULL, 10);
            dims[1] = strtol(argv[2], NULL, 10);
        }
	hsize_t nelem = dims[0] * dims[1];

        //Create Data
        int       * data_int32   = malloc(sizeof(int)      *nelem);
        short int * data_int16   = malloc(sizeof(short int)*nelem);
        float     * data_float32 = malloc(sizeof(float)    *nelem);
        double    * data_float64 = malloc(sizeof(double)   *nelem);
        for(i=0;i<nelem;++i){
          data_int32[i]   = i;
          data_int16[i]   = 1;
          data_float32[i] = 2.0;
          data_float64[i] = 3.14;
        }
        //Create Data Space
        dataspaceId = H5Screate_simple(2, dims, NULL);

	//Initialize Python and Numpy Routine
	Py_Initialize();
	import_array();

        printf("Start testing\n");

	//Test VOL Plugin Setup
        under_fapl = H5Pcreate (H5P_FILE_ACCESS);
        H5Pset_fapl_native(under_fapl);
        assert(H5VLis_registered("native") == 1);
        vol_id = H5VLregister (&H5VL_python_g);
        assert(vol_id > 0);
        assert(H5VLis_registered(plugin_name) == 1);
        vol_id2 = H5VLget_plugin_id(plugin_name);
        H5VLinitialize(vol_id2, H5P_DEFAULT);
        H5VLclose(vol_id2);
        native_plugin_id = H5VLget_plugin_id("native");
        assert(native_plugin_id > 0);
        acc_tpl = H5Pcreate (H5P_FILE_ACCESS);
	size_t prop_size=sizeof(int);
 	char pyplugin_name[5]="py";
	int prop_def_value=0;
	H5Pinsert2(acc_tpl, pyplugin_name, prop_size, &prop_def_value, NULL, NULL, NULL, NULL, NULL, NULL);
        H5Pset_vol(acc_tpl, vol_id, &under_fapl);

	//Test HDF5 File Create
	file_id = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, acc_tpl);

        //Test HDF5 Group Create
	group_id = H5Gcreate2(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

 	//Test HDF5 Dataset Create
	hid_t datasetId_int32   = H5Dcreate2(file_id,fullpath_int32,  H5T_NATIVE_INT,   dataspaceId,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
        hid_t datasetId_int16   = H5Dcreate2(file_id,fullpath_int16,  H5T_NATIVE_SHORT, dataspaceId,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
        hid_t datasetId_float32 = H5Dcreate2(file_id,fullpath_float32,H5T_NATIVE_FLOAT, dataspaceId,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
	hid_t datasetId_float64 = H5Dcreate2(file_id,fullpath_float64,H5T_NATIVE_DOUBLE,dataspaceId,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT); 
	H5Sclose(dataspaceId);

	//Test HDF5 Dataset Write
	H5Dwrite(datasetId_int32,   H5T_NATIVE_INT,    H5S_ALL, H5S_ALL, H5P_DEFAULT, data_int32);
        H5Dwrite(datasetId_int16,   H5T_NATIVE_SHORT,  H5S_ALL, H5S_ALL, H5P_DEFAULT, data_int16);
	H5Dwrite(datasetId_float32, H5T_NATIVE_FLOAT,  H5S_ALL, H5S_ALL, H5P_DEFAULT, data_float32);
	H5Dwrite(datasetId_float64, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_float64);


        //Test HDF5 Dataset Read
	//int * data_in = malloc(sizeof(int)*nelem);
        //H5Dread (datasetId_int32, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_in);

	free (data_int16);
      	free (data_int32);
	free (data_float32);
	free (data_float64);


	//Test HDF5 Dataset Close
	H5Dclose(datasetId_int16);
	H5Dclose(datasetId_int32);
	H5Dclose(datasetId_float32);
	H5Dclose(datasetId_float64);


	//Test HDF5 Group Close
	H5Gclose(group_id);


	//Test HDF5 File Close
	H5Fclose(file_id);

        //Py_Finalize();
	return 0;
}






/*************************CLEAN UP CODES*******************************/

/*
        H5Pclose(acc_tpl);
        H5Pclose(under_fapl);

        H5VLclose(native_plugin_id);
        H5VLterminate(vol_id, H5P_DEFAULT);
        H5VLunregister (vol_id);
        assert(H5VLis_registered(plugin_name) == 0);
*/



        //len = H5VLget_plugin_name(group_id, name, 50);
        //printf ("GROUP VOL name = %s  %ld\n", name, len);
        //printf("Group Create Test OK\n");
        
        /*
        int_id = H5Tcopy(H5T_NATIVE_INT);
        printf("H5Tcopy\n");
        H5Tcommit2(file_id, "int", int_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        printf("H5VLget start\n");
        len = H5VLget_plugin_name(int_id, name, 50);
        printf ("DT COMMIT name = %s  %ld\n", name, len);
        printf("H5VLget end\n");
        H5Tclose(int_id);
        printf("H5Tcommit\n");
        int_id = H5Topen2(file_id, "int", H5P_DEFAULT);
        len = H5VLget_plugin_name(int_id, name, 50);
        printf ("DT OPEN name = %s  %ld\n", name, len);
        H5Tclose(int_id);
        int_id = H5Oopen(file_id,"int",H5P_DEFAULT);
        len = H5VLget_plugin_name(int_id, name, 50);
        printf ("DT OOPEN name = %s  %ld\n", name, len);
        len = H5Fget_name(file_id, name, 50);
        printf("name = %ld  %s\n", len, name);
        */


/*static herr_t
visit_cb(hid_t oid, const char *name,
         const H5O_info_t *oinfo, void *udata)
{
    char n[25];
    if(H5Iget_type(oid) == H5I_GROUP) {
        size_t len = H5VLget_plugin_name(oid, n, 50);
        //printf ("Visiting GROUP VOL name = %s  %ld\n", n, len);
    }
    if(H5Iget_type(oid) == H5I_DATASET) 
        //printf("visiting dataset\n");
    if(H5Iget_type(oid) == H5I_DATATYPE) 
        //printf("visiting datatype\n");

    return 1;
}*/
 /* end h5_verify_cached_stabs_cb() */


        /*if(argc !=2){
                printf("Input: plugin name, e.g., python\n");
                return 0;
        }*/


        //strcpy(plugin_name,argv[1]);


        //len = H5VLget_plugin_name(file_id, name, 25);
        //printf("File Create Test OK\n");
        //printf ("FILE VOL name = %s  %ld\n", name, len);


        //TODO: H5Pinsert2(pid, PROP1_NAME, sizeof(int), 0, ...); test/tgenprop.c
        //setup python vol plugin property, e.g., h5py, root, or swift, etc
        //property name, py
        //property value, 0: h5py
        //                1: swift
        //                2: root
        // 
        //H5Pset_vol(acc_tpl, vol_id, NULL);
        //Test file create
        //TODO: acc_tpl taking specific python vol info
        //printf("VOL Plugin Setup OK\n");   


        //printf("In test python vol: CHECK: Dataset ID: %ld\n",datasetId);
        //test the dataset space id
        //printf("In test python vol: CHECK: Data Space ID: %ld\n", dataspaceId);      


 /*       if(H5Sget_select_type(dataspaceId)==H5S_SEL_ALL){
          printf("In test python vol: select type is H5S_SEL_ALL, OK:\n");
        }else {
          printf("In test python vol: select type is not ALL");
        }
*/
/*
#if 0

        attr = H5Acreate2(group_id, "attr1", int_id, space, H5P_DEFAULT, H5P_DEFAULT);
        int_id = H5Aget_type(attr);
        len = H5VLget_plugin_name(int_id, name, 50);
        printf ("DT OPEN name = %s  %d\n", name, len);

        H5Aclose (attr);

        int_id = H5Oopen(file_id,"int",H5P_DEFAULT);
        len = H5VLget_plugin_name(int_id, name, 50);
        printf ("DT OOPEN name = %s  %d\n", name, len);
        H5Oclose(int_id);


        H5Fclose(file_id);
        file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT);//acc_tpl);
        H5Fflush(file_id, H5F_SCOPE_GLOBAL);
#endif
*/
