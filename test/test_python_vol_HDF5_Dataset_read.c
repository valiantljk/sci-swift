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
	hid_t file_id, group_id, datasetId, acc_tpl, under_fapl, vol_id, vol_id2, int_id, attr, space;
	int i;
	hsize_t ndims=0, *dims=NULL,nelem=1;
        if(argc<5)//at least 5 parameters: python_vol fname dname len
	{
           printf("./python_vol filename groupname datasetname, size(h5dget_space not working, sorry)\n");
	   printf("Example:\n./HDF5_Dataset_read rocket.h5 spacex falcon_int32 3000\n");
	   return 0;
        }
        else{
	   strcpy(file_name,argv[1]);
	   strcpy(group_name, argv[2]);
	   strcpy(dset_name,argv[3]);
      	   nelem=strtol(argv[4], NULL, 10);
	}	
        char dset_path[100];
        sprintf(dset_path,  "%s/%s",group_name, dset_name);
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
	//HDF5 File Open
	
	file_id = H5Fopen(file_name, H5F_ACC_RDONLY,acc_tpl);
	//HDF5 Dataset Open
	
	datasetId = H5Dopen(file_id, dset_path,H5P_DEFAULT);	
  	hid_t dataspace;
  	size_t      size;                  /* size of data*/    
  	int rank;
  	//dataspace = H5Dget_space(datasetId);    /* dataspace handle */
  	//rank      = H5Sget_simple_extent_ndims(dataspace);
  	//hsize_t     dims_out[rank];
  	//H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
	//nelem=1;
	//for(i=0;i<rank;i++){
	//    nelem*=dims_out[i];
	//}
	//printf("number of elements:%ld,number of dims:%d, datasetId:%ld,dataspace:%ld\n",(long)nelem,rank,datasetId,dataspace);
       
        //Test HDF5 Dataset Read
	int check = 0;
	void *data_in = NULL;
	if(strstr(dset_name, "int32")!=NULL){
		//printf("datapath:%s, dataset name:%s\n",dset_path, dset_name);
		data_in   = malloc(sizeof(int)      *nelem);
		H5Dread (datasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_in);
		if(data_in!=NULL){
		 printf("Printing first 10 numbers\n");
		 for(check=0; check<10; check++) 
		    printf("%d ",((int *)data_in)[check]);
		 printf("Should be 0-9\n");
		}
	}else if(strstr(dset_name, "int16")!=NULL){
		data_in   =  malloc(sizeof(short int)*nelem);
		H5Dread (datasetId, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_in);
		printf("Printing first 10 numbers\n");
		for(check=0; check<10; check++) 
		    printf("%u ",((short int *) data_in)[check]);
		printf("Should be 1\n");
	}else if(strstr(dset_name, "float32")!=NULL){
		data_in   = malloc(sizeof(float)    *nelem);
		H5Dread (datasetId, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_in);
		printf("Printing first 10 numbers\n");
		for(check=0; check<10; check++) 
		    printf("%f ",((float *)data_in)[check]);
		printf("Should be 2.0\n");
	}else if(strstr(dset_name, "float64")!=NULL){
		data_in   = malloc(sizeof(double)   *nelem);
		H5Dread (datasetId, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_in);
		printf("Printing first 10 numbers\n");
		for(check=0; check<10; check++) 
		    printf("%lf ",((double *)data_in)[check]);
		printf("Should be 3.14\n");
	}
	else {
		printf("dataset specified error\n");
	}
	//if(data_in!=NULL ) free (data_in);
	
	//Test HDF5 Dataset Close
	H5Dclose(datasetId);

	//Test HDF5 Group Close
	//H5Gclose(group_id);


	//Test HDF5 File Close
	H5Fclose(file_id);

        Py_Finalize();
	printf("\nTesting Complete\n");
	return 0;
}



