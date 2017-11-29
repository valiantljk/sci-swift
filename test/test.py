def f(n):
    import h5py # swift_vol.py
    fx=h5py.File('test.h5','a')
    import logging
    minimum_size = -0.1
    container='swift'
    from swiftclient.service import SwiftService, SwiftError
    from swiftclient.service import SwiftService
    with SwiftService() as swift:
     try:
        list_parts_gen = swift.list(container=container)
        #print (list_parts_gen)
        for page in list_parts_gen:
            if page["success"]:
                for item in page["listing"]:

                    i_size = int(item["bytes"])
                    if i_size > minimum_size:
                        i_name = item["name"]
                        i_etag = item["hash"]
                        print(
                            "%s [size: %s] [etag: %s]" %
                            (i_name, i_size, i_etag)
                        )
            else:
                print page
                raise page["error"]

     except SwiftError as e:
        pass
    print "Hello from f %d"%n
    return n
