#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hdf5.h"
#include "../src/python_vol.h"
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION
#include <math.h>
#include "timer.h"
#define NDSET 25304802
#define NAMELEN 48

struct timeval start_time[3];
float elapse[3];

void print_usage() {
    printf("Usage: srun -n n_proc ./h5bossio dset_list.txt output n_dsets(max:25304802)\n");
}
//int main(int argc, const char *argv[])
int main(int argc, char* argv[])
{
    int size, rank;
    char *in_filename, *out_filename;
    char **all_dset_names = NULL, *all_dset_names_1d = NULL;
    char tmp_input[256];
    hsize_t dims[1];
    int *all_dset_sizes = NULL;
    int i, j, n_write = NDSET, my_n_write = 0, my_write_start;
    hid_t file_id,fapl,acc_tpl;
    hid_t * dset_ids = NULL;
    hid_t * dcpl_id = NULL;
    hid_t * plist_id = NULL;
    hid_t * filespace = NULL;
    hid_t * memspace = NULL;
    const char plugin_name[7]="python";
    MPI_Info info  = MPI_INFO_NULL;

    hsize_t tsize=0;
    if(argc!=4) {
	print_usage();
	//ERROR;
    }
    in_filename  = argv[1];
    out_filename = argv[2];
    n_write = atoi(argv[3]);
//Swfit VOL Related Code Change
    //MPI Routine
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank==0) printf("number of ranks:[%d]\n",size);

    //Initialize Python and Numpy Routine
    Py_Initialize();
    import_array();

    //Setup Swift Vol
    acc_tpl = H5Pcreate (H5P_FILE_ACCESS);
    H5Pset_fapl_swift(acc_tpl,plugin_name, MPI_COMM_WORLD, MPI_INFO_NULL);

//Swift VOL Related Code Change


    if (rank == 0) 
        printf("Writing %d datasets.\n", n_write);
    
    all_dset_names    = (char**)calloc(n_write, sizeof(char*));
    all_dset_names_1d = (char*)calloc(n_write*NAMELEN, sizeof(char));
    all_dset_sizes = (int*)calloc(n_write, sizeof(int));
    dset_ids =(hid_t *) calloc(n_write, sizeof(hsize_t));
    filespace =(hid_t *) calloc(n_write, sizeof(hsize_t));
    memspace =(hid_t *) calloc(n_write, sizeof(hsize_t));
    dcpl_id =(hid_t *) calloc(n_write, sizeof(hsize_t));
    plist_id =(hid_t *) calloc(n_write, sizeof(hsize_t));
    for (i = 0; i < n_write; i++) 
        all_dset_names[i] = all_dset_names_1d + i*NAMELEN;

    // Rank 0 read from input file
    if (rank == 0) {
        printf("Reading from %s\n", in_filename);
        FILE *pm_file = fopen(in_filename, "r");
        if (NULL == pm_file) {
            fprintf(stderr, "Error opening file: %s\n", in_filename);
            return (-1);
        }

        // /3523/55144/1/coadd, 1D: 4619, 147808
        i = 0;
        char dset_ndim[4];
        int  dset_nelem;
        while (fgets(tmp_input, 255, pm_file) != NULL ) {
      	   // printf("Reading %d,%d line\n",i,n_write);
	    if (i >= n_write) 
                break;
            sscanf(tmp_input+1, "%[^,], %[^:]: %d, %d", 
                               all_dset_names[i], dset_ndim, &dset_nelem, &all_dset_sizes[i]);
            if (strcmp(dset_ndim, "1D") != 0) {
                printf("Dimension is %s!\n", dset_ndim);
                continue;
            }
            for (j = 0; j < strlen(all_dset_names[i]); j++) {
                if (all_dset_names[i][j] == '/') 
                    all_dset_names[i][j] = '-';
                
            }
            i++;
        }

        fclose(pm_file);
        n_write = i;
	
         for (i = 0; i < n_write; i++) {
             tsize+= all_dset_sizes[i]; 
         }
	 printf("Total size is %ld bytes\n",tsize);
    }

    //fflush(stdout);
    my_n_write  = n_write;
    MPI_Bcast( &n_write, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast( all_dset_names_1d, n_write*NAMELEN, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast( all_dset_sizes, n_write, MPI_INT, 0, MPI_COMM_WORLD);
   // fflush(stdout);
    // Distribute work evenly
    // Last rank may have extra work
    my_n_write = n_write / size;
    my_write_start = rank * my_n_write;
    if (rank == size - 1) 
        my_n_write += n_write % size;
    printf("rank:%d,start:%d,ndsets:%d\n",rank,my_write_start,my_n_write);
    file_id = H5Fcreate(out_filename, H5F_ACC_TRUNC, H5P_DEFAULT, acc_tpl);
    fflush(stdout); 
    if (file_id < 0) {
        printf("Error creating a file [%s]\n", out_filename);
        //ERROR;
    }
    else {
	if (rank==0){
	   printf("file creation ok\n");	
	}
    }
    /* H5Fclose(file_id); */
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    timer_on(1);
    for (i = 0; i < n_write; i++) {
        dims[0] = all_dset_sizes[i]/4;
        //printf("rank:%d,my write in bytes %d\n",rank,dims[0]);	
        filespace[i] = H5Screate_simple(1, dims, NULL); 
        memspace[i] = H5Screate_simple(1, dims, NULL);
	dcpl_id[i] =  H5Pcreate(H5P_DATASET_CREATE);
	plist_id[i] = H5Pcreate(H5P_DATASET_XFER);
	H5Pset_dxpl_mpio(plist_id[i], H5FD_MPIO_COLLECTIVE);
//	if (rank==0) printf("dset:%d,size:%d\n",i,all_dset_sizes[i]);
        dset_ids[i] = H5Dcreate(file_id, all_dset_names[i], H5T_NATIVE_INT, filespace[i], 
                            H5P_DEFAULT,  dcpl_id[i], H5P_DEFAULT);
        //H5Sclose(filespace);
        //H5Dclose(dset_id);
	//printf("rank:%d, creating dset:%d\n",rank,i);
    }
    //H5Fclose(file_id);
    MPI_Barrier (MPI_COMM_WORLD);
    timer_off(1);
    //file_id=H5Fopen(out_filename, H5F_ACC_RDWR, acc_tpl);
    if(file_id <0){
	if(rank==0)  printf("Error opening a file [%s]\n", out_filename);
        //ERROR;
    }else{
    	if(rank==0)  printf("Ok opening existing file[%s]\n",out_filename);
    }

    //fflush(stdout);

    //ERROR;
    hsize_t count = 0;
    //char *buf = NULL; 
    herr_t ret;
    //char * buf=(char*)malloc(sizeof(char)*all_dset_sizes[my_write_start]);
    int ** buf = (int **) malloc (sizeof(int *)*my_n_write);
    for (i=0; i< my_n_write;i++){
	buf[i]=(int *) malloc (sizeof(int)*all_dset_sizes[my_write_start+i]/4);
	memset(buf[i],1,sizeof(int)*all_dset_sizes[my_write_start+i]/4);
    }
    MPI_Barrier (MPI_COMM_WORLD);
    timer_on (0);
    int adjust_i=0;
    for (i = 0; i < my_n_write; i++) {
	
	adjust_i=i+my_write_start;
        if (dset_ids[adjust_i] < 0) {
            printf("Error opening the dataset [%s]\n", all_dset_names[adjust_i]);
            continue;
        }
        ret = H5Dwrite(dset_ids[adjust_i], H5T_NATIVE_INT, memspace[adjust_i], filespace[adjust_i], plist_id[adjust_i], buf[i]);
        if (ret < 0) {
            printf("Error writing to the dataset [%s]\n", all_dset_names[adjust_i]);
            H5Dclose(dset_ids[adjust_i]);
            continue;
        }
    }
    fflush(stdout);
    printf("rank:%d, done with dset write\n",rank);
    MPI_Barrier (MPI_COMM_WORLD);
    free(buf);
    free(all_dset_names_1d);// = (char*)calloc(n_write*NAMELEN, sizeof(char));
    free(all_dset_sizes);// = (int*)calloc(n_write, sizeof(int));
    free(dset_ids);	// =(hid_t *) calloc(n_write, sizeof(hsize_t));
    free(filespace);	// =(hid_t *) calloc(n_write, sizeof(hsize_t));
    free(memspace);	// =(hid_t *) calloc(n_write, sizeof(hsize_t));
    free(dcpl_id);	// =(hid_t *) calloc(n_write, sizeof(hsize_t));

    timer_off (0);
    fflush(stdout);
    
    if (rank == 0)
    {
	printf("first timer is dset creation cost, second is dset write cost\n");
	timer_msg(1);//dset creation cost
	timer_msg(0);//dset write cost
	printf("%ld KB or %ld MB\n",tsize/1024, tsize/1024/1024);
    }
    fflush(stdout);
//    H5Fclose(file_id);
    H5Pclose(acc_tpl);

    (void)MPI_Finalize();

    return 0;
}
