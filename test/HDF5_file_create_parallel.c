#include <mpi.h>
#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hdf5.h"
#include "timer.h"
#include "../src/python_vol.h"
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION
struct timeval start_time[3];
float elapse[3];
int main(int argc, char **argv) {
	const char plugin_name[7]="python";
    	char file_name[100]="large_dataset.h5";
	hid_t file_id, acc_tpl, under_fapl, vol_id, vol_id2, int_id, attr, space;
	int i;
	if(argc<3)//at least 2 parameters: python_vol fname
	{
		printf("./python_vol_HDF5_File_create filename number_of_files\n");
		printf("Example:\n./HDF5_File_create rocket.h5 10\n");
		return 0;
	}
	else{
		strcpy(file_name,argv[1]);
		MPI_Init(&argc, &argv);
		int my_rank, num_procs;
		MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
		printf("number of ranks:[%d]\n",num_procs);
		//Initialize Python and Numpy Routine
		
		Py_Initialize();
		import_array();
		
        	printf("Start testing\n");

		/* replace the following functions with one 
		  H5Pset_fapl_swift(hid_t fapl, MPI_Comm, MPI_Info)
		*/
		/*
		//Test VOL Plugin Setup
        	under_fapl = H5Pcreate (H5P_FILE_ACCESS);
        	//H5Pset_fapl_native(under_fapl);
        	//assert(H5VLis_registered("native") == 1);
        	vol_id = H5VLregister (&H5VL_python_g);
        	assert(vol_id > 0);
        	assert(H5VLis_registered(plugin_name) == 1);
        	vol_id2 = H5VLget_plugin_id(plugin_name);
        	H5VLinitialize(vol_id2, H5P_DEFAULT);
        	H5VLclose(vol_id2);
        	//native_plugin_id = H5VLget_plugin_id("native");
        	//assert(native_plugin_id > 0);
        	acc_tpl = H5Pcreate (H5P_FILE_ACCESS);
		//size_t prop_size=sizeof(int);
 		//char pyplugin_name[5]="py";
		//int prop_def_value=0;
		//H5Pinsert2(acc_tpl, pyplugin_name, prop_size, &prop_def_value, NULL, NULL, NULL, NULL, NULL, NULL);
        	H5Pset_vol(acc_tpl, vol_id, &under_fapl);
		*/
		acc_tpl = H5Pcreate (H5P_FILE_ACCESS);
		if (acc_tpl < 0) printf("H5Pcreate failed\n");
		H5Pset_fapl_swift(acc_tpl,plugin_name, MPI_COMM_WORLD, MPI_INFO_NULL);
		timer_on (0);	
		//Test HDF5 File Create
		int ifile=0;
		char newfile_name[100];
		for(ifile=0;ifile<atoi(argv[2]);ifile++){
			sprintf(newfile_name,  "%s_%d",file_name,ifile);	
			//printf("creating %s\n",newfile_name);
			file_id = H5Fcreate(newfile_name, H5F_ACC_TRUNC, H5P_DEFAULT, acc_tpl);	
			//H5Fclose(file_id);
		}
		timer_off (0);
		/*
		//Test HDF5 File Close
		H5Fclose(file_id);
        	Py_Finalize();
		printf("Testing Complete\n");
		timer_msg (0);
		*/
		return 0;
	}
}
