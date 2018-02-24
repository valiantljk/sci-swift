try:
    from h5pvol import H5PVol
except Exception as e:
    print ("Python HDF5 Vol is not implemented")

try:
    from h5svol import H5PVol
except Exception as e:
    print ("Python Swift Vol is not implemented")
    pass

class h5py:
    return H5PVol

class swift:
    return H5PVol
