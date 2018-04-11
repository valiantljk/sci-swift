/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* 
 *  This example illustrates how to write and read data in an existing
 *  dataset.  It is used in the HDF5 Tutorial.
 */

#include "hdf5.h"
#define FILE "rocket.h5"
#include <stdio.h>
#include <stdlib.h>
int main() {

   hid_t       file_id, datasetId;  /* identifiers */
   herr_t      status;
   int         i;

   /* Open an existing file. */
   file_id = H5Fopen(FILE, H5F_ACC_RDWR, H5P_DEFAULT);

   /* Open an existing dataset. */
   datasetId = H5Dopen2(file_id, "spacex/falcon_int32", H5P_DEFAULT);
   hid_t dataspace;
   size_t      size;                  /* size of data*/
   int rank;
   dataspace = H5Dget_space(datasetId);    /* dataspace handle */
   printf("dsetid:%ld,dspce:%ld\n",(long)datasetId, (long)dataspace);
   rank      = H5Sget_simple_extent_ndims(dataspace);
   hsize_t     dims_out[rank];
   H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
   int nelem=1;
   for(i=0;i<rank;i++){
       nelem*=dims_out[i];
   }
   int * dset_data =malloc(sizeof(int)*nelem);

   status = H5Dread(datasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
                    dset_data);

   /* Close the dataset. */
   status = H5Dclose(datasetId);

   /* Close the file. */
   status = H5Fclose(file_id);
}
