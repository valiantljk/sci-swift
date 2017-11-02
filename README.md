# Sci-Swift
Scientific Object Store based on Openstack Swift


# How to use

some basic python scripts are ready to use in examples
```
python list.py 
python upload.py
```


![scibox](https://user-images.githubusercontent.com/1396867/32252259-d83a685e-be51-11e7-8a2b-e5f30c1cf1ee.png)


# Use Sci-Swift in Docker Container

Create a data only container
 ```
docker run -v /srv --name SWIFT_DATA busybox
```
Create a openstack object store with volume from the existing data container
```
ID=$(docker run -d -p 12345:8080 --volumes-from SWIFT_DATA -t sciswift)
```

