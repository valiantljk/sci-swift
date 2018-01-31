#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hdf5.h"
#include "../src/python_vol.h"
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION
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

int main(int argc, char **argv) {
        const char file_name[]="large_dataset.h5";
	const char group_name[]="/bowling";
	const char dataset_name[]="scale";
	/*if(argc !=2){
		printf("Input: plugin name, e.g., python\n");
		return 0;
	}*/
	char plugin_name[7]="python";
	printf("Start testing\n");
	//strcpy(plugin_name,argv[1]);
	char fullpath[500];
	hid_t file_id;
	hid_t group_id;
	hid_t dataspaceId;
        hid_t datasetId;
        hid_t acc_tpl;
        hid_t under_fapl;
        hid_t vol_id, vol_id2;
        hid_t int_id;
        hid_t attr;
        hid_t space;
	const unsigned int nelem=60;
	int *data;//, *data_in;
	unsigned int i;
	hsize_t dims[2];
        ssize_t len;
        char name[25];
        static hsize_t      ds_size[2] = {10, 20};
	Py_Initialize();
	import_array();
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
        //TODO: H5Pinsert2(pid, PROP1_NAME, sizeof(int), 0, ...); test/tgenprop.c
        //setup python vol plugin property, e.g., h5py, root, or swift, etc
	size_t PROP1_SIZE=sizeof(int);
 	char pyplugin_name[5]="py";
	int PROP1_DEF_VALUE=0;
	//property name, py
        //property value, 0: h5py
	//		  1: swift
	//		  2: root
	//		  3: fit
	H5Pinsert2(acc_tpl, pyplugin_name, PROP1_SIZE, &PROP1_DEF_VALUE, NULL, NULL, NULL, NULL, NULL, NULL);
        H5Pset_vol(acc_tpl, vol_id, &under_fapl);
	//H5Pset_vol(acc_tpl, vol_id, NULL);
        //Test file create
	//TODO:	acc_tpl taking specific python vol info
	printf("VOL setup OK\n");
	file_id = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, acc_tpl);
        len = H5VLget_plugin_name(file_id, name, 25);
	printf("File Create Test OK\n");
//        printf ("FILE VOL name = %s  %ld\n", name, len);
        // Test group create
	group_id = H5Gcreate2(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        len = H5VLget_plugin_name(group_id, name, 50);
//        printf ("GROUP VOL name = %s  %ld\n", name, len);
	printf("Group Create Test OK\n");
/*        int_id = H5Tcopy(H5T_NATIVE_INT);
	printf("H5Tcopy\n");
        H5Tcommit2(file_id, "int", int_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        printf("H5VLget start\n");
        len = H5VLget_plugin_name(int_id, name, 50);
//        printf ("DT COMMIT name = %s  %ld\n", name, len);
        printf("H5VLget end\n");
        H5Tclose(int_id);
	printf("H5Tcommit\n");
        int_id = H5Topen2(file_id, "int", H5P_DEFAULT);
        len = H5VLget_plugin_name(int_id, name, 50);
//        printf ("DT OPEN name = %s  %ld\n", name, len);
        H5Tclose(int_id);

        int_id = H5Oopen(file_id,"int",H5P_DEFAULT);
        len = H5VLget_plugin_name(int_id, name, 50);
//        printf ("DT OOPEN name = %s  %ld\n", name, len);
*/

//        len = H5Fget_name(file_id, name, 50);
//        printf("name = %ld  %s\n", len, name);

	data = malloc (sizeof(int)*nelem);
        int * data_in = malloc (sizeof(int)*nelem);
	for(i=0;i<nelem;++i)
	  data[i]=i;

	dims [0] =60;
        dims [1] =2;
	dataspaceId = H5Screate_simple(2, dims, NULL); 
        //space = H5Screate_simple (2, ds_size, ds_size);
	sprintf(fullpath,"%s/%s",group_name,dataset_name);
	printf("H5Dcreate2 starts:\n");
	printf("In test python vol: CHECK: Dataset ID: %ld\n",datasetId);
        //test the dataset space id
        printf("In test python vol: CHECK: Data Space ID: %ld\n", dataspaceId);
	datasetId = H5Dcreate2(file_id,fullpath,H5T_NATIVE_INT,dataspaceId,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
        //hid_t temp_spaceid=0;
        //H5Pget ( H5P_DEFAULT , " dataset_space_id " , & temp_spaceid);
        //printf("In test python vol: CHECK: Temp Data Space ID: %ld\n", temp_spaceid);
        if(H5Sget_select_type(dataspaceId)==H5S_SEL_ALL){
          printf("In test python vol: select type is H5S_SEL_ALL\n");
        }else {
          printf("In test python vol: select type is not ALL");
        }
	H5Sclose(dataspaceId);

        len = H5VLget_plugin_name(datasetId, name, 50);

        printf ("DSET name = %s  %ld\n", dataset_name, len);
        printf("In test python vol: CHECK: H5S_ALL:%ld\n",H5S_ALL);	
	H5Dwrite(datasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
/*        H5Dread (datasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_in);
	H5Dclose(datasetId);

        H5Ovisit(file_id, H5_INDEX_NAME, H5_ITER_NATIVE, visit_cb, NULL);

	free (data);
        H5Oclose(int_id);
        H5Sclose (space);

	H5Gclose(group_id);
*/
//*/
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

	H5Fclose(file_id);
//	printf("end of H5Fclose\n");
/*
        H5Pclose(acc_tpl);
        H5Pclose(under_fapl);

        H5VLclose(native_plugin_id);
        H5VLterminate(vol_id, H5P_DEFAULT);
        H5VLunregister (vol_id);
        assert(H5VLis_registered(plugin_name) == 0);
*/  
        printf("Begin python finalize\n");
        //Py_Finalize();
	printf("End python finalize\n");
	return 0;
}

