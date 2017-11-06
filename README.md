# Sci-Swift
Scientific Object Store based on Openstack Swift


# How to use

some basic python scripts are ready to use in examples
```
python list.py 
python upload.py
```


![scibox](https://user-images.githubusercontent.com/1396867/32252259-d83a685e-be51-11e7-8a2b-e5f30c1cf1ee.png)


# SciSwift QuickStart 
(Step 1-3 building time ~ 2mins 35 sec)

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
  ./sciswift 
```
