# Sci-Swift
Scientific Object Store based on Openstack Swift

# Why Sci-Swift

* No code change required for existing HDF5 applications to use object store (A promising HPC storage for replacing current PFS) 
* Enable HDF5 to work with any kind of formats/storage using your favourite _python_ library.
  e.g, FITs, Parquet, ROOT, TensorFlow, etc

# Design

![sciswift2](https://user-images.githubusercontent.com/1396867/33396383-2f32fdd0-d4fd-11e7-9038-a87dcc501d2d.png)

# QuickStart 
(Step 1-3 building time ~ 2mins 35 sec)

Python 2.7.13 :: Anaconda 2.3.0 (64-bit)

**Step 1**. Create a data only docker container
 ```
  docker run -v /srv --name SWIFT_DATA busybox
```
**Step 2**. Create a openstack object store (with volume from the existing data container, binding host port 12345 with container port 8080)
```
  docker run -d -p 12345:8080 --volumes-from SWIFT_DATA -t valiantljk/sciswift:v3 # need 'docker login' first
```

**Step 3**. Start openstack swift
```
  docker ps # to get the containerID of the current running sciswift container 
  docker exec -it containerID bash # launch a shell for mornitoring purpose, i.e., you will see the openstack swift logs
  ./usr/local/bin/startmain.sh      # start the openstack swift server, i.e., launching account/object/proxy server process 
```
**Step 4**. Test openstack swift and sciswift

   * Test connection to openstack swift from host
```
  swift -A http://127.0.0.1:12345/auth/v1.0 -U test:tester -K testing stat # can observe the previous mornitoring shell for logs
```
   * Test connection to openstack swift within sciswift container 
```
  docker exec -it containerID bash # launch an interactive shell 
  swift -A http://127.0.0.1:8080/auth/v1.0 -U test:tester -K testing stat 
```
   * Test with python-swiftclient
```
  cd /build/sci-swift/examples   # within the same container launched in last step
  source sourceme
  python upload.py swift 1.h5 wind 100 100  # create a 100*100 array with h5py and upload this file to swift store
  python list.py swift # list all objects in 'swift' container
```
   * Test with HDF5 Swift VOL, the real SciSwift feature 
```
  cd /build/sci-swift/vol_plugins/test
  make
  ./sciswift swift
```


# Code Structure
```
.
|-- LICENSE
|-- README.md
|-- examples
|   |-- list.py
|   |-- sourceme
|   `-- upload.py
|-- src
|   |-- Makefile
|   |-- __init__.py
|   |-- swift.c
|   |-- swift.h
|   |-- swift.py
|   |-- swift_vol.c
|   `-- swift_vol.h
`-- test
    |-- Makefile
    |-- __init__.py
    |-- readme
    |-- test.py
    |-- test_swift.c
    |-- test_swift.py
    `-- test_swift_vol.c
```
