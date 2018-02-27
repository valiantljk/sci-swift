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
	const char plugin_name[7]="python";
        char file_name[100]="large_dataset.h5";
	char group_name[100]="bowling";
        char dset_name[100]="falcon";
	hid_t file_id, group_id, dataspaceId, datasetId, acc_tpl, under_fapl, vol_id, vol_id2, int_id, attr, space;
	int i;
	hsize_t ndims=0, *dims=NULL,nelem=1;
        if(argc<6)//at least 5 parameters: python_vol fname dname ndims dim0
	{
           printf("./python_vol filename groupname datasetname ndims dims0 dims1 ...\n");
	   printf("Example:\n./python_vol rocket.h5 spacex falcon 3 100 20 30\n");
	   return 0;
        }
        else{
	   strcpy(file_name,argv[1]);
	   strcpy(group_name, argv[2]);
	   strcpy(dset_name,argv[3]);
	   ndims = strtol(argv[4], NULL, 10);
	   dims = malloc(sizeof(hsize_t)*ndims);
	   if(argc!=ndims+5){
		printf("ndims is:%d, but number of dims provided is only %d\n",(int)ndims,argc-(int)ndims);
		return 0;
	   }
	   for(i=0;i<ndims;i++){
	     dims[i]=strtol(argv[5+i], NULL, 10);
	     nelem*=dims[i];
	   }
        }
        printf("Creating four types of data\n");
        //Create Data with 4 different types
        int       * data_int32   = malloc(sizeof(int)      *nelem);
        short int * data_int16   = malloc(sizeof(short int)*nelem);
        float     * data_float32 = malloc(sizeof(float)    *nelem);
        double    * data_float64 = malloc(sizeof(double)   *nelem);
        char fullpath_int32[100];
        char fullpath_int16[100];
        char fullpath_float32[100];
        char fullpath_float64[100];
        sprintf(fullpath_int32,  "%s/%s%s",group_name, dset_name,"_int32");
        sprintf(fullpath_int16,  "%s/%s%s",group_name, dset_name,"_int16");
        sprintf(fullpath_float32,"%s/%s%s",group_name, dset_name,"_float32");
        sprintf(fullpath_float64,"%s/%s%s",group_name, dset_name,"_float64");
        //Fill in fake data
        for(i=0;i<nelem;++i){
          data_int32[i]   = i;
          data_int16[i]   = 1;
          data_float32[i] = 2.0;
          data_float64[i] = 3.14;
        }
        printf("Data is ready\n");
        //Create Data Space
        dataspaceId = H5Screate_simple(ndims, dims, NULL);

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
	//Test HDF5 Dataset Write
	H5Dwrite(datasetId_int32,   H5T_NATIVE_INT,    H5S_ALL, H5S_ALL, H5P_DEFAULT, data_int32);
        H5Dwrite(datasetId_int16,   H5T_NATIVE_SHORT,  H5S_ALL, H5S_ALL, H5P_DEFAULT, data_int16);
	H5Dwrite(datasetId_float32, H5T_NATIVE_FLOAT,  H5S_ALL, H5S_ALL, H5P_DEFAULT, data_float32);
	H5Dwrite(datasetId_float64, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_float64);
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

        Py_Finalize();
	printf("Testing Complete\n");
	return 0;
}



