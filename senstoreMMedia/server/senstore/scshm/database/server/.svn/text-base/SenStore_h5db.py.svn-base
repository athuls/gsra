# Copyright (c) 2010 SC Solutions, Inc.
# See LICENCE.TXT for licensing terms.
# $Id: h5db-tpl.py,v 1.13 2010/09/03 22:29:10 glinden Exp $
import sys
import traceback
import logging
from tables import *
import numpy
from find import *

## \package scshm.database.server.SenStore_h5db
# SenStore HDF5 server implementation.
#
# This module implements the SenStore HDF5 database server.
#
# The Database class provides the main database implementation.  The
# <tt><<i>name</i>>Props</tt> classes can be used to define new
# objects that can be stored in the database, using the
# <tt>Database::add<<i>name</i>>()</tt> member functions.  Objects can
# be retrieved using the <tt>Database::get<<i>name</i>>()</tt> member
# functions.


## PyTables column definition for the MappingMatrix table.
class MappingMatrixCols(IsDescription):
    ## Object ID.
    id = Int64Col()


## PyTables column definition for the DaqUnitChannelData table.
class DaqUnitChannelDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()


## PyTables column definition for the AccelMeas table.
class AccelMeasCols(IsDescription):
    ## Object ID.
    id = Int64Col()


## Checks if an HDF5 group has a child with the given name.
#
# @param group HDF5 group object
# @param name child name
def nodeExists(group, name):
    return group._v_children.has_key(name)


## Gets the largest ID in use at the given table
#
# @param table HDF5 table with an 'id' field
def getMaxId(table):
    maxId = 0
    for row in table.iterrows():
        if maxId < row['id']:
            maxId = row['id']
    return maxId


## HDF5 database implementation
#
# Provides direct access to an HDF5 database.
#
# The HDF5 file has the following layout:
# - <tt>/objects/<<i>classname</i>>/</tt> contains class objects
# - <tt>/arrays/<<i>classname</i>>/id<<i>id</i>>/data</tt> contains array data
# - <tt>/arrays/<<i>classname</i>>/id<<i>id</i>>/<<i>axis</i>></tt>
#   contains array axis definition
# - <tt>/signals/<<i>classname</i>>/id<<i>id</i>>/data</tt> contains signal data
# - <tt>/signals/<<i>classname</i>>/id<<i>id</i>>/<<i>axis</i>></tt>
#   contains signal axis definition
class Database:
    ## Opens an HDF5 file and creates any missing tables.
    #
    # If the HDF5 file does not yet exist it is created.
    #
    # @param filename HDF5 file name
    # @param title    HDF5 file title (will be stored in file on creation)
    def __init__(self, filename, title = ''):
        self.h5 = openFile(filename, mode = "a", title = title)
        #
        # Create missing tables
        #
        if not nodeExists(self.h5.root, 'objects'):
            objs = self.h5.createGroup('/', 'objects', 'Objects')
        else:
            objs = self.h5.root.objects
        if not nodeExists(self.h5.root, 'arrays'):
            arrays = self.h5.createGroup('/', 'arrays', 'Array data')
        else:
            arrays = self.h5.root.arrays
        if not nodeExists(self.h5.root, 'signals'):
            signals = self.h5.createGroup('/', 'signals', 'Signal data')
        else:
            signals = self.h5.root.signals
        if not nodeExists(objs, 'MappingMatrix'):
            # Create MappingMatrix object table
            self.tblMappingMatrix = self.h5.createTable(objs,
                'MappingMatrix', MappingMatrixCols,
                'Linear mapping between two node-related quantities. ')
            logging.info('Creating HDF5 table for MappingMatrix objects')
        else:
            self.tblMappingMatrix = objs.MappingMatrix
        if not nodeExists(arrays, 'MappingMatrix'):
            # Create MappingMatrix array data group
            self.grpMappingMatrix = self.h5.createGroup(arrays,
                'MappingMatrix', 'Linear mapping between two node-related quantities. ')
            logging.info('Creating HDF5 group for MappingMatrix array data')
        else:
            self.grpMappingMatrix = self.h5.getNode(arrays, 'MappingMatrix')
        self.h5.flush()
        if not nodeExists(objs, 'DaqUnitChannelData'):
            # Create DaqUnitChannelData object table
            self.tblDaqUnitChannelData = self.h5.createTable(objs,
                'DaqUnitChannelData', DaqUnitChannelDataCols,
                'Data acquisition unit raw channel data. ')
            logging.info('Creating HDF5 table for DaqUnitChannelData objects')
        else:
            self.tblDaqUnitChannelData = objs.DaqUnitChannelData
        if not nodeExists(signals, 'DaqUnitChannelData'):
            # Create DaqUnitChannelData signal data group
            self.grpDaqUnitChannelData = self.h5.createGroup(signals,
                'DaqUnitChannelData', 'Data acquisition unit raw channel data. ')
            logging.info('Creating HDF5 group for DaqUnitChannelData signal data')
        else:
            self.grpDaqUnitChannelData = self.h5.getNode(signals, 'DaqUnitChannelData')
        self.h5.flush()
        if not nodeExists(objs, 'AccelMeas'):
            # Create AccelMeas object table
            self.tblAccelMeas = self.h5.createTable(objs,
                'AccelMeas', AccelMeasCols,
                'Multi-axis accelerometer measurement data. ')
            logging.info('Creating HDF5 table for AccelMeas objects')
        else:
            self.tblAccelMeas = objs.AccelMeas
        if not nodeExists(signals, 'AccelMeas'):
            # Create AccelMeas signal data group
            self.grpAccelMeas = self.h5.createGroup(signals,
                'AccelMeas', 'Multi-axis accelerometer measurement data. ')
            logging.info('Creating HDF5 group for AccelMeas signal data')
        else:
            self.grpAccelMeas = self.h5.getNode(signals, 'AccelMeas')
        self.h5.flush()

    ## Gets the HDF5 fields members of an MappingMatrix fields object.
    #
    # The fields object that is passed must have a valid id field
    # value.  The object fields will be updated (merged) with the
    # retrieved HDF5 fields.
    #
    # @param fields existing MappingMatrixFields object
    # @see MappingMatrixProps
    def getMappingMatrix(self, fields):
        # TODO: smarter and faster lookup?
        for row in self.tblMappingMatrix.where('id==%d' % fields.id):
            break
        else:
            return

    ## Gets the MappingMatrix array dimensions.
    #
    # @param id  MappingMatrix object ID
    # @return array dimensions
    def getMappingMatrixArraySize(self, id):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            # No data has been set yet
            return []
        array = self.h5.getNode(grp, 'data')
        return [long(x) for x in array.shape]

    ## Gets the MappingMatrix OutputNode axis by MappingMatrix id.
    #
    # @param id ID of existing MappingMatrix object
    # @return OutputNode axis
    def getMappingMatrixOutputNodeAxis(self, id):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'OutputNodeAxis'):
            # No axis data has been set yet
            return []
        # Get array axes
        return grp.OutputNodeAxis.read()

    ## Gets the MappingMatrix InputNode axis by MappingMatrix id.
    #
    # @param id ID of existing MappingMatrix object
    # @return InputNode axis
    def getMappingMatrixInputNodeAxis(self, id):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'InputNodeAxis'):
            # No axis data has been set yet
            return []
        # Get array axes
        return grp.InputNodeAxis.read()

    ## Gets a slice of the MappingMatrix array data.
    # 
    # @param id      MappingMatrix object ID
    # @param slices  list of slices for each dimension (empty is all)
    # @return (shape, data)
    def getMappingMatrixArrayData(self, id, slices):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        array = self.h5.getNode(grp, 'data')
        dataSlices = [Ellipsis]*len(array.shape)
        dataSlices[0:len(slices)] = slices
        for i,slc in enumerate(dataSlices):
            if slc == Ellipsis:
                # PyTables does not support more than one Ellipsis
                dataSlices[i] = slice(0,array.shape[i],1)
        # Get array data
        dOut = array[tuple(dataSlices)]
        # Compute total number of elements
        n = reduce(int.__mul__, dOut.shape)
        return dOut.shape, dOut.reshape((n,)).tolist()

    ## Updates the HDF5 fields of an MappingMatrix object.
    #
    # The fields object that is passed must have a valid id field
    # value.  The object fields will be updated with the retrieved
    # HDF5 fields.
    #
    # @param fields      existing MappingMatrix object
    # @param fieldNames  list of names of fields to be updated (empty for all)
    # @see MappingMatrixProps
    def setMappingMatrix(self, fields, fieldNames):
        # TODO: smarter and faster lookup?
        for row in self.tblMappingMatrix.where('id==%d' % fields.id):
            break
        else:
            return

    ## Sets the MappingMatrix OutputNode axis by MappingMatrix id.
    #
    # @param id    ID of existing MappingMatrix object
    # @param data  axis data
    # @return OutputNode axis
    def setMappingMatrixOutputNodeAxis(self, id, data):
        # Check for existence of data and compatible axis length
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            raise RuntimeError('Data must be created before setting axis')
        array = self.h5.getNode(grp, 'data')
        dim = array.shape[0]
        if dim != len(data):
            raise ValueError('Axis length must be %d' % dim)
        # Set or update axis
        if nodeExists(grp, 'OutputNodeAxis'):
            # Delete existing array data
            self.h5.removeNode(grp, 'OutputNodeAxis')
        # Store array data
        a = self.h5.createArray(grp, 'OutputNodeAxis', data, 'OutputNode axis')
        a.flush()

    ## Sets the MappingMatrix InputNode axis by MappingMatrix id.
    #
    # @param id    ID of existing MappingMatrix object
    # @param data  axis data
    # @return InputNode axis
    def setMappingMatrixInputNodeAxis(self, id, data):
        # Check for existence of data and compatible axis length
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            raise RuntimeError('Data must be created before setting axis')
        array = self.h5.getNode(grp, 'data')
        dim = array.shape[1]
        if dim != len(data):
            raise ValueError('Axis length must be %d' % dim)
        # Set or update axis
        if nodeExists(grp, 'InputNodeAxis'):
            # Delete existing array data
            self.h5.removeNode(grp, 'InputNodeAxis')
        # Store array data
        a = self.h5.createArray(grp, 'InputNodeAxis', data, 'InputNode axis')
        a.flush()

    ## Sets the MappingMatrix array dimensions.
    #
    # Note that deletes any existing array data.
    #
    # @param id  MappingMatrix object ID
    # @param dims array dimensions
    def createMappingMatrixArray(self, id, dims):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if nodeExists(grp, 'data'):
            # Delete existing array data
            self.h5.removeNode(grp, 'data')
        # Create new array
        atom = Float64Atom()
        shape = tuple(dims)
        a = self.h5.createCArray(grp, 'data', atom, shape)
        a.flush()

    ## Sets the MappingMatrix array data of the i'th dimension, at the
    ## given indices.
    #
    # @param id      MappingMatrix object ID
    # @param slices  list of slices for each dimension (empty is all)
    # @param data    vectorized array data
    def setMappingMatrixArrayData(self, id, slices, data):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        array = self.h5.getNode(grp, 'data')
        data = numpy.array(data)
        dataShape = array.shape[:]
        dataSlices = [Ellipsis]*len(array.shape)
        dataSlices[0:len(slices)] = slices
        for i,slc in enumerate(dataSlices):
            if slc == Ellipsis:
                # PyTables does not support more than one Ellipsis
                dataSlices[i] = slice(0,array.shape[i],1)
            else:
                dataShape[i] = len(range(slc.start,slc.stop,slc.step))
        data = data.reshape(tuple(dataShape))
        array.__setitem__(tuple(dataSlices), data)
        array.flush()

    ## Adds the HDF5 fields of an MappingMatrix object.
    #
    # The ID field of the given MappingMatrix properties must be set to
    # the SQL object ID.
    #
    # @param fields  MappingMatrix object fields
    # @see getMappingMatrix()
    def addMappingMatrix(self, fields):
        row = self.tblMappingMatrix.row
        row['id'] = fields.id
        row.append()
        self.tblMappingMatrix.flush()
        # Create group to hold arrays-specific properties
        grp = self.h5.createGroup(self.grpMappingMatrix, 'id%d' % fields.id)

    ## Adds a MappingMatrix set.
    #
    # The ID field of each MappingMatrix property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of MappingMatrixFields objects
    # @see MappingMatrixProps
    # @see getMappingMatrix()
    def addMappingMatrixList(self, fieldsList):
        row = self.tblMappingMatrix.row
        for fields in fieldsList:
            row[name] = fields.id
            row.append()
        self.tblMappingMatrix.flush()

    ## Gets the HDF5 fields members of an DaqUnitChannelData fields object.
    #
    # The fields object that is passed must have a valid id field
    # value.  The object fields will be updated (merged) with the
    # retrieved HDF5 fields.
    #
    # @param fields existing DaqUnitChannelDataFields object
    # @see DaqUnitChannelDataProps
    def getDaqUnitChannelData(self, fields):
        # TODO: smarter and faster lookup?
        for row in self.tblDaqUnitChannelData.where('id==%d' % fields.id):
            break
        else:
            return

    ## Gets the DaqUnitChannelData array dimensions.
    #
    # @param id  DaqUnitChannelData object ID
    # @return array dimensions
    def getDaqUnitChannelDataArraySize(self, id):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            # No data has been set yet
            return []
        array = self.h5.getNode(grp, 'data')
        return [long(x) for x in array.shape]

    ## Gets the DaqUnitChannelData SubChannels axis by DaqUnitChannelData id.
    #
    # @param id ID of existing DaqUnitChannelData object
    # @return SubChannels axis
    def getDaqUnitChannelDataSubChannelsAxis(self, id):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if not nodeExists(grp, 'SubChannelsAxis'):
            # No axis data has been set yet
            return []
        # Get array axes
        return grp.SubChannelsAxis.read()
    ## Gets the DaqUnitChannelData signal time axis information.
    #
    # If the time stamp list is empty, the beginning and end
    # of the list are returned.
    #
    # @param id     DaqUnitChannelData object ID
    # @param tList  list of time stamps [s]
    # @return signal time axis information
    def getDaqUnitChannelDataTimeAxisInfo(self, id, tList):
        # TODO: faster algorithm for searching
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        tAxis = self.h5.getNode(grp, 'time')
        nAxis = len(tAxis)
        if nAxis == 0:
            return [],[]
        if len(tList) == 0:
            return ([tAxis[0],tAxis[-1]],[0,nAxis-1])
        # Find index of first matching timestamp in each interval
        tActual = []
        idxlist = []
        idx1 = 0
        for t in tList:
            idx2 = findNextClosest(tAxis, t, idx1)
            if idx2 == None:
                idx2 = nAxis - 1
                tActual.append(tAxis[idx2])
                idxlist.append(idx2)
                break
            if idx1 != idx2 or not idx2 in idxlist:
                tActual.append(tAxis[idx2])
                idxlist.append(idx2)
                idx1 = idx2
        return tActual, idxlist

    ## Gets the signal data.
    #
    # The returned signal data will have time stamps tStart <= t <=
    # tEnd, with no two time stamps closer than tDelta.
    #
    # @param id      DaqUnitChannelData object ID
    # @param tStart  start time 
    # @param tStep   minimum time interval
    # @param tEnd    end time 
    # @param slices  list of slices for each dimension (empty is all)
    # @return (shape, data, t) 
    def getDaqUnitChannelDataSignalData(self, id, tStart, tStep, tEnd, slices):
        # TODO: faster algorithm for searching
        idxlist = []
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        data = self.h5.getNode(grp, 'data')
        tAxis = self.h5.getNode(grp, 'time')
        #
        # Find time stamps to retrieve data for
        #
        t = tStart
        i = 0
        while True:
            i = findNextClosest(tAxis, t, i)
            if i == None:
                break
            if tStep >= 0.0:
                t = tAxis[i] + tStep
            else:
                t = tAxis[i]
            if t <= tEnd:
                idxlist.append(i)
            if t >= tEnd:
                break
            i += 1
        if len(idxlist) == 0:
            return (0,), [], []
        #
        # Build data slice
        #
        dataSlices = [Ellipsis]*len(data.shape)
        # Caller-defined slices
        dataSlices[1:1+len(slices)] = slices
        # Other dimensions
        for i,s in enumerate(dataSlices):
            if i == 0:
                # Time axis slice
                dataSlices[0] = idxlist
                continue
            if s == Ellipsis:
                # PyTables does not support more than one Ellipsis
                dataSlices[i] = slice(0,data.shape[i],1)
        try:
            # Try extended slicing (requires PyTables >= 2.2)
            dOut = data[tuple(dataSlices)]
            tOut = tAxis[idxlist]
            # Compute total number of elements
            return dOut.shape, dOut.flatten().tolist(), tOut.tolist()
        except:
            pass
        # Fall back to standard slicing
        tOut = []
        dOut = []
        for idx in idxlist:
            tOut.append(tAxis[idx])
            tmp = data[tuple([idx]+dataSlices[1:])]
            shape = tmp.shape
            dOut += tmp.flatten().tolist()
        return [len(idxlist)]+list(shape), dOut, tOut

    ## Gets the signal data by sample index range.
    #
    # The returned signal data will contain time stamps at
    # [idxStart,  idxStart+idxStep, idxStart+2*idxStep, ...],
    # where idx < idxEnd.
    #
    # @param id        DaqUnitChannelData object ID
    # @param idxStart  start index (included in data)
    # @param idxStep   index step size
    # @param idxEnd    end index (not included in data)
    # @param slices    list of slices for each dimension (empty is all)
    # @return (shape, data, t) 
    def getDaqUnitChannelDataSignalDataByIndexRange(self, id, idxStart, idxStep, idxEnd, slices):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        data = self.h5.getNode(grp, 'data')
        tAxis = self.h5.getNode(grp, 'time')
        tOut = tAxis[idxStart:idxEnd:idxStep]
        if len(tOut) == 0:
            return (0,), [], []
        #
        # Build data slice
        #
        dataSlices = [Ellipsis]*len(data.shape)
        # Caller-defined slices
        dataSlices[1:1+len(slices)] = slices
        # Other dimensions
        for i,s in enumerate(dataSlices):
            if i == 0:
                # Time axis slice
                dataSlices[0] = slice(idxStart,idxEnd,idxStep)
                continue
            if s == Ellipsis:
                # PyTables does not support more than one Ellipsis
                dataSlices[i] = slice(0,data.shape[i],1)
            else:
                dataSlices[i] = slice(s.start,s.end,s.step)
        dOut = data[tuple(dataSlices)]
        # Compute total number of elements
        n = reduce(int.__mul__, dOut.shape)
        return dOut.shape, dOut.reshape((n,)).tolist(), tOut.tolist()

    ## Updates the HDF5 fields of an DaqUnitChannelData object.
    #
    # The fields object that is passed must have a valid id field
    # value.  The object fields will be updated with the retrieved
    # HDF5 fields.
    #
    # @param fields      existing DaqUnitChannelData object
    # @param fieldNames  list of names of fields to be updated (empty for all)
    # @see DaqUnitChannelDataProps
    def setDaqUnitChannelData(self, fields, fieldNames):
        # TODO: smarter and faster lookup?
        for row in self.tblDaqUnitChannelData.where('id==%d' % fields.id):
            break
        else:
            return

    ## Sets the DaqUnitChannelData SubChannels axis by DaqUnitChannelData id.
    #
    # @param id    ID of existing DaqUnitChannelData object
    # @param data  axis data
    # @return SubChannels axis
    def setDaqUnitChannelDataSubChannelsAxis(self, id, data):
        # Check for existence of data and compatible axis length
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            raise RuntimeError('Data must be created before setting axis')
        array = self.h5.getNode(grp, 'data')
        dim = array.shape[1]
        if dim != len(data):
            raise ValueError('Axis length must be %d' % dim)
        # Set or update axis
        if nodeExists(grp, 'SubChannelsAxis'):
            # Delete existing array data
            self.h5.removeNode(grp, 'SubChannelsAxis')
        # Store array data
        a = self.h5.createArray(grp, 'SubChannelsAxis', data, 'SubChannels axis')
        a.flush()

    ## Sets the DaqUnitChannelData signal array dimensions.
    #
    # Note that the array size reflects the size of each axis, not
    # including the time axis.  The signal will grow along the time
    # axis as arrays are added.
    #
    # This method must be called before signal arrays can be added.
    # Any existing data is removed without warning.
    #
    # @param id    DaqUnitChannelData object ID
    # @param dims  signal array dimensions
    def createDaqUnitChannelDataSignal(self, id, dims):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if nodeExists(grp, 'data'):
            # Delete existing array data
            self.h5.removeNode(grp, 'data')
        if nodeExists(grp, 'time'):
            # Delete existing time data
            self.h5.removeNode(grp, 'time')
        # Create new signal data array
        atom = Int32Atom()
        shape = tuple([0] + dims)
        a = self.h5.createEArray(grp, 'data', atom, shape)
        a.flush()
        # Create new signal time array
        atom = Time64Atom()
        shape = (0,)
        a = self.h5.createEArray(grp, 'time', atom, shape)
        a.flush()

    ## Appends a data point to the DaqUnitChannelData signal.
    #
    # The data must be supplied as a vector, which is interpreted as a
    # multi-dimensional array.
    #
    # @param id    DaqUnitChannelData object ID
    # @param t     list of timestamps
    # @param data  vectorized signal data
    def appendToDaqUnitChannelDataSignal(self, id, t, data):
        # TODO: keeping time axis sorted
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        # Add signal data
        array = self.h5.getNode(grp, 'data')
        data = numpy.array(data)
        data = data.reshape(tuple([len(t)] + list(array.shape[1:])))
        array.append(data)
        array.flush()
        # Add time data
        array = self.h5.getNode(grp, 'time')
        array.append(t)
        array.flush()

    ## Adds the HDF5 fields of an DaqUnitChannelData object.
    #
    # The ID field of the given DaqUnitChannelData properties must be set to
    # the SQL object ID.
    #
    # @param fields  DaqUnitChannelData object fields
    # @see getDaqUnitChannelData()
    def addDaqUnitChannelData(self, fields):
        row = self.tblDaqUnitChannelData.row
        row['id'] = fields.id
        row.append()
        self.tblDaqUnitChannelData.flush()
        # Create group to hold signal-specific properties
        grp = self.h5.createGroup(self.grpDaqUnitChannelData, 'id%d' % fields.id)

    ## Adds a DaqUnitChannelData set.
    #
    # The ID field of each DaqUnitChannelData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of DaqUnitChannelDataFields objects
    # @see DaqUnitChannelDataProps
    # @see getDaqUnitChannelData()
    def addDaqUnitChannelDataList(self, fieldsList):
        row = self.tblDaqUnitChannelData.row
        for fields in fieldsList:
            row[name] = fields.id
            row.append()
        self.tblDaqUnitChannelData.flush()

    ## Gets the HDF5 fields members of an AccelMeas fields object.
    #
    # The fields object that is passed must have a valid id field
    # value.  The object fields will be updated (merged) with the
    # retrieved HDF5 fields.
    #
    # @param fields existing AccelMeasFields object
    # @see AccelMeasProps
    def getAccelMeas(self, fields):
        # TODO: smarter and faster lookup?
        for row in self.tblAccelMeas.where('id==%d' % fields.id):
            break
        else:
            return

    ## Gets the AccelMeas array dimensions.
    #
    # @param id  AccelMeas object ID
    # @return array dimensions
    def getAccelMeasArraySize(self, id):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            # No data has been set yet
            return []
        array = self.h5.getNode(grp, 'data')
        return [long(x) for x in array.shape]

    ## Gets the AccelMeas Direction axis by AccelMeas id.
    #
    # @param id ID of existing AccelMeas object
    # @return Direction axis
    def getAccelMeasDirectionAxis(self, id):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if not nodeExists(grp, 'DirectionAxis'):
            # No axis data has been set yet
            return []
        # Get array axes
        return grp.DirectionAxis.read()
    ## Gets the AccelMeas signal time axis information.
    #
    # If the time stamp list is empty, the beginning and end
    # of the list are returned.
    #
    # @param id     AccelMeas object ID
    # @param tList  list of time stamps [s]
    # @return signal time axis information
    def getAccelMeasTimeAxisInfo(self, id, tList):
        # TODO: faster algorithm for searching
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        tAxis = self.h5.getNode(grp, 'time')
        nAxis = len(tAxis)
        if nAxis == 0:
            return [],[]
        if len(tList) == 0:
            return ([tAxis[0],tAxis[-1]],[0,nAxis-1])
        # Find index of first matching timestamp in each interval
        tActual = []
        idxlist = []
        idx1 = 0
        for t in tList:
            idx2 = findNextClosest(tAxis, t, idx1)
            if idx2 == None:
                idx2 = nAxis - 1
                tActual.append(tAxis[idx2])
                idxlist.append(idx2)
                break
            if idx1 != idx2 or not idx2 in idxlist:
                tActual.append(tAxis[idx2])
                idxlist.append(idx2)
                idx1 = idx2
        return tActual, idxlist

    ## Gets the signal data.
    #
    # The returned signal data will have time stamps tStart <= t <=
    # tEnd, with no two time stamps closer than tDelta.
    #
    # @param id      AccelMeas object ID
    # @param tStart  start time 
    # @param tStep   minimum time interval
    # @param tEnd    end time 
    # @param slices  list of slices for each dimension (empty is all)
    # @return (shape, data, t) 
    def getAccelMeasSignalData(self, id, tStart, tStep, tEnd, slices):
        # TODO: faster algorithm for searching
        idxlist = []
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        data = self.h5.getNode(grp, 'data')
        tAxis = self.h5.getNode(grp, 'time')
        #
        # Find time stamps to retrieve data for
        #
        t = tStart
        i = 0
        while True:
            i = findNextClosest(tAxis, t, i)
            if i == None:
                break
            if tStep >= 0.0:
                t = tAxis[i] + tStep
            else:
                t = tAxis[i]
            if t <= tEnd:
                idxlist.append(i)
            if t >= tEnd:
                break
            i += 1
        if len(idxlist) == 0:
            return (0,), [], []
        #
        # Build data slice
        #
        dataSlices = [Ellipsis]*len(data.shape)
        # Caller-defined slices
        dataSlices[1:1+len(slices)] = slices
        # Other dimensions
        for i,s in enumerate(dataSlices):
            if i == 0:
                # Time axis slice
                dataSlices[0] = idxlist
                continue
            if s == Ellipsis:
                # PyTables does not support more than one Ellipsis
                dataSlices[i] = slice(0,data.shape[i],1)
        try:
            # Try extended slicing (requires PyTables >= 2.2)
            dOut = data[tuple(dataSlices)]
            tOut = tAxis[idxlist]
            # Compute total number of elements
            return dOut.shape, dOut.flatten().tolist(), tOut.tolist()
        except:
            pass
        # Fall back to standard slicing
        tOut = []
        dOut = []
        for idx in idxlist:
            tOut.append(tAxis[idx])
            tmp = data[tuple([idx]+dataSlices[1:])]
            shape = tmp.shape
            dOut += tmp.flatten().tolist()
        return [len(idxlist)]+list(shape), dOut, tOut

    ## Gets the signal data by sample index range.
    #
    # The returned signal data will contain time stamps at
    # [idxStart,  idxStart+idxStep, idxStart+2*idxStep, ...],
    # where idx < idxEnd.
    #
    # @param id        AccelMeas object ID
    # @param idxStart  start index (included in data)
    # @param idxStep   index step size
    # @param idxEnd    end index (not included in data)
    # @param slices    list of slices for each dimension (empty is all)
    # @return (shape, data, t) 
    def getAccelMeasSignalDataByIndexRange(self, id, idxStart, idxStep, idxEnd, slices):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        data = self.h5.getNode(grp, 'data')
        tAxis = self.h5.getNode(grp, 'time')
        tOut = tAxis[idxStart:idxEnd:idxStep]
        if len(tOut) == 0:
            return (0,), [], []
        #
        # Build data slice
        #
        dataSlices = [Ellipsis]*len(data.shape)
        # Caller-defined slices
        dataSlices[1:1+len(slices)] = slices
        # Other dimensions
        for i,s in enumerate(dataSlices):
            if i == 0:
                # Time axis slice
                dataSlices[0] = slice(idxStart,idxEnd,idxStep)
                continue
            if s == Ellipsis:
                # PyTables does not support more than one Ellipsis
                dataSlices[i] = slice(0,data.shape[i],1)
            else:
                dataSlices[i] = slice(s.start,s.end,s.step)
        dOut = data[tuple(dataSlices)]
        # Compute total number of elements
        n = reduce(int.__mul__, dOut.shape)
        return dOut.shape, dOut.reshape((n,)).tolist(), tOut.tolist()

    ## Updates the HDF5 fields of an AccelMeas object.
    #
    # The fields object that is passed must have a valid id field
    # value.  The object fields will be updated with the retrieved
    # HDF5 fields.
    #
    # @param fields      existing AccelMeas object
    # @param fieldNames  list of names of fields to be updated (empty for all)
    # @see AccelMeasProps
    def setAccelMeas(self, fields, fieldNames):
        # TODO: smarter and faster lookup?
        for row in self.tblAccelMeas.where('id==%d' % fields.id):
            break
        else:
            return

    ## Sets the AccelMeas Direction axis by AccelMeas id.
    #
    # @param id    ID of existing AccelMeas object
    # @param data  axis data
    # @return Direction axis
    def setAccelMeasDirectionAxis(self, id, data):
        # Check for existence of data and compatible axis length
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            raise RuntimeError('Data must be created before setting axis')
        array = self.h5.getNode(grp, 'data')
        dim = array.shape[1]
        if dim != len(data):
            raise ValueError('Axis length must be %d' % dim)
        # Set or update axis
        if nodeExists(grp, 'DirectionAxis'):
            # Delete existing array data
            self.h5.removeNode(grp, 'DirectionAxis')
        # Store array data
        a = self.h5.createArray(grp, 'DirectionAxis', data, 'Direction axis')
        a.flush()

    ## Sets the AccelMeas signal array dimensions.
    #
    # Note that the array size reflects the size of each axis, not
    # including the time axis.  The signal will grow along the time
    # axis as arrays are added.
    #
    # This method must be called before signal arrays can be added.
    # Any existing data is removed without warning.
    #
    # @param id    AccelMeas object ID
    # @param dims  signal array dimensions
    def createAccelMeasSignal(self, id, dims):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if nodeExists(grp, 'data'):
            # Delete existing array data
            self.h5.removeNode(grp, 'data')
        if nodeExists(grp, 'time'):
            # Delete existing time data
            self.h5.removeNode(grp, 'time')
        # Create new signal data array
        atom = Float32Atom()
        shape = tuple([0] + dims)
        a = self.h5.createEArray(grp, 'data', atom, shape)
        a.flush()
        # Create new signal time array
        atom = Time64Atom()
        shape = (0,)
        a = self.h5.createEArray(grp, 'time', atom, shape)
        a.flush()

    ## Appends a data point to the AccelMeas signal.
    #
    # The data must be supplied as a vector, which is interpreted as a
    # multi-dimensional array.
    #
    # @param id    AccelMeas object ID
    # @param t     list of timestamps
    # @param data  vectorized signal data
    def appendToAccelMeasSignal(self, id, t, data):
        # TODO: keeping time axis sorted
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        # Add signal data
        array = self.h5.getNode(grp, 'data')
        data = numpy.array(data)
        data = data.reshape(tuple([len(t)] + list(array.shape[1:])))
        array.append(data)
        array.flush()
        # Add time data
        array = self.h5.getNode(grp, 'time')
        array.append(t)
        array.flush()

    ## Adds the HDF5 fields of an AccelMeas object.
    #
    # The ID field of the given AccelMeas properties must be set to
    # the SQL object ID.
    #
    # @param fields  AccelMeas object fields
    # @see getAccelMeas()
    def addAccelMeas(self, fields):
        row = self.tblAccelMeas.row
        row['id'] = fields.id
        row.append()
        self.tblAccelMeas.flush()
        # Create group to hold signal-specific properties
        grp = self.h5.createGroup(self.grpAccelMeas, 'id%d' % fields.id)

    ## Adds a AccelMeas set.
    #
    # The ID field of each AccelMeas property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of AccelMeasFields objects
    # @see AccelMeasProps
    # @see getAccelMeas()
    def addAccelMeasList(self, fieldsList):
        row = self.tblAccelMeas.row
        for fields in fieldsList:
            row[name] = fields.id
            row.append()
        self.tblAccelMeas.flush()

    ## Closes the HDF5 file.
    def close(self):
        self.h5.close()
