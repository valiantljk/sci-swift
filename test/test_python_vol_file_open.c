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
        char file_name[100];
	char group_name[100];
        char dataset_name[100];
	hid_t file_id, group_id, dataspaceId, datasetId, acc_tpl, under_fapl, vol_id, vol_id2, int_id, attr, space;
	int i;
	hsize_t ndims=0, *dims=NULL,nelem=1;
        if(argc!=4)//4 parameters: python_vol fname groupname dname 
	{
           printf("./python_vol filename groupname datasetname\n");
	   printf("Example:\n./python_vol rocket.h5 spacex falcon\n");
	   return 0;
        }
        else{
	   strcpy(file_name,argv[1]);
	   strcpy(group_name, argv[2]);
	   strcpy(dataset_name,argv[3]);
	}
        char dset_name[100];
        sprintf(dset_name,  "%s/%s",group_name, dataset_name); 
        //Create Data Space
        //dataspaceId = H5Screate_simple(ndims, dims, NULL);

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

	//Test HDF5 File Open 
	file_id = H5Fopen(file_name, H5F_ACC_RDONLY,acc_tpl);
        printf("file_id:%ld\n",file_id);
        //Test HDF5 Dataset Open

        datasetId = H5Dopen(file_id, dset_name,H5P_DEFAULT);
        printf("datasetid:%ld\n",datasetId);
        dataspaceId = H5Dget_space(datasetId); 
        printf("dataspaceId:%ld\n",dataspaceId);
        ndims = H5Sget_simple_extent_ndims(dataspaceId);
        H5Sget_simple_extent_dims(dataspaceId, dims, NULL); 	
        printf("ndims:%d\n",ndims);
        for(i=0;i<ndims;i++){
            nelem*=dims[i];
        }	 
       //Test HDF5 Dataset Read
	int       * data_in   = malloc(sizeof(int)      *nelem);
	H5Dread (datasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_in);
	free (data_in);

	//Test HDF5 Dataset Close
	H5Dclose(datasetId);
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
