# Gwendolyn van der Linden
# Copyright (c) 2009 SC Solutions, Inc.
# All rights reserved
# $Id: sevenbeambridge.py,v 1.4 2010/08/20 00:49:48 glinden Exp $
import sys
import time
import logging
import numpy
import ConfigParser

# Include client API.
# SenStoreClient is a small wrapper around the client API
# for connection management.  SenStore is the actual client API.
from scshm.database.client.SenStoreClient import SenStoreClient, SenStore

if __name__ == '__main__':
    # Setup logging
    FORMAT = "%(asctime)-15s:%(levelname)s %(message)s"
    logging.basicConfig(level=logging.DEBUG,format=FORMAT)

    # Load configuration
    config = ConfigParser.ConfigParser()
    config.read(['config/client/SenStore.cfg'])

    #
    # Connect to server and get manager proxy
    #
    client = SenStoreClient(config)
    mngr = client.getManager()
    logging.info('Connected to SenStore')

    #
    # Create structure entry
    #
    # Create and fill new Structure object
    struct = SenStore.StructureFields()
    struct.mName = 'Demo bridge'
    struct.mType = 'Suspension bridge'
    struct.mDistanceUnit = SenStore.Unit.UnitFOOT
    struct.mForceUnit = SenStore.Unit.UnitKILOPOUND
    struct.mWeightUnit = SenStore.Unit.UnitKILOSLUG
    # Request server to add new structure, and store the assigned object ID
    struct.id = mngr.addStructure(struct)
    logging.info('Created structure [oid=%d]' % struct.id)

    #
    # Update structure entry (to show how that's done)
    #
    # Get proxy to structure object from server
    structPrx = mngr.getStructure(struct.id)
    # Set fields to be updated (changed fields only)
    struct = SenStore.StructureFields()
    struct.mDescription = 'Dummy bridge for CyberInfrastructure testing'
    # Request server to update indicated fields in database
    structPrx.setFields(struct, ['Description'])
    # Get all structure fields from server
    struct = structPrx.getFields()

    #
    # Create coordinate system
    #
    # Create and fill new FEMCoordSystem object
    coord = SenStore.FEMCoordSystemFields()
    coord.mStructure = struct.id
    coord.mAX = 1.0
    coord.mAY = 0.0
    coord.mAZ = 0.0
    coord.mBX = 0.0
    coord.mBY = 1.0
    coord.mBZ = 0.0
    coord.mDescription = 'Default coordinate system'
    coord.mXorigin = 0.0
    coord.mYorigin = 0.0
    coord.mZorigin = 0.0
    # Request server to add new coordinate system
    coord.id = mngr.addFEMCoordSystem(coord)
    logging.info('Created default coordinate structure [oid=%d]' % coord.id)

    #
    # Create nodes
    #
    #   nodeid,   x,   y,    z
    nodes = [
        (10000,  0.0, 0.0, 100.0), # Auxiliary node
        (10001,  0.0, 0.0,   0.0),
        (10002, 10.0, 0.0,   0.0),
        (10003, 20.0, 0.0,   0.0),
        (10004,  5.0, 0.0,   5.0),
        (10005, 15.0, 0.0,   5.0)
        ]
    nodeId = {} # Keep track of object IDs
    for nodeid,x,y,z in nodes:
        # Create and fill new FEMNode object
        node = SenStore.FEMNodeFields()
        node.mLocalID = nodeid
        node.mX       = x
        node.mY       = y
        node.mZ       = z
        node.mSystem  = coord.id
        # Request server to add new node
        node.id = mngr.addFEMNode(node)
        nodeId[nodeid] = node.id
        logging.info('Created node [oid=%d]' % node.id)

    #
    # Retrieve list of nodes for this coordinate system (for
    # illustrational purposes)
    #
    # Search by providing example fields object
    node = SenStore.FEMNodeFields()
    node.mSystem  = coord.id
    # Retrieve list of IDs of matching objects
    ids = mngr.findEqualFEMNode(node, ['System'])
    # Get the fields objects themselves
    nodeFieldsList = mngr.getFEMNodeFieldsList(ids)
    for fields in nodeFieldsList:
        logging.info('Node[oid=%(id)06d] x=%(mX)7.1f z=%(mZ)7.1f' % \
                         fields.__dict__)

    #
    # Create beam group
    #
    # Note: future version will use object inheritance for group <- beamgroup)
    group = SenStore.FEMGroupFields()
    # Group type is defined as an an enumeration
    group.mGroupType = SenStore.GroupType.GroupTypeBEAM
    group.mStructure = struct.id
    group.mDescription = 'Default beam group'
    group.mLocalID = 10000
    # Request server to add new group
    group.id = mngr.addFEMGroup(group)
    logging.info('Created default beam group [oid=%d]' % group.id)
    
    #
    # Create beams
    #
    #   beamid,    n1,    n2,   aux
    beams = [
        (10001, 10001, 10002, 10000),
        (10002, 10002, 10003, 10000),
        (10003, 10001, 10004, 10000),
        (10004, 10004, 10002, 10000),
        (10005, 10002, 10005, 10000),
        (10006, 10005, 10003, 10000),
        (10007, 10004, 10005, 10000),
        ]
    for beamid, n1, n2, aux in beams:
        beam = SenStore.FEMBeamFields()
        beam.mNode1      = nodeId[n1]
        beam.mNode2      = nodeId[n2]
        beam.mAuxNode    = nodeId[aux]
        beam.mElementID  = beamid
        beam.mGroup      = group.id
        beam.id = mngr.addFEMBeam(beam)
        logging.info('Created beam [oid=%d]' % beam.id)

    #
    # Create compliance matrix
    #
    # Create and fill new ComplianceMatrix fields object
    compl = SenStore.MappingMatrixFields()
    compl.mStructure = struct.id
    compl.mName = 'Kz'
    compl.mDescription = 'Vertical compliance'
    compl.mOutputQuantity = SenStore.Quantity.QuantityZDISPLACEMENT
    compl.mInputQuantity = SenStore.Quantity.QuantityZFORCE
    # Request server to add new MappingMatrix
    compl.id = mngr.addMappingMatrix(compl)
    # Get proxy to ComplianceMatrix object, to get access to its functions
    complPrx = mngr.getMappingMatrix(compl.id)
    logging.info('Created compliance mapping matrix [oid=%d]' % compl.id)
    # Create list of nodes that we have data for
    localNodeIds = [10002,10004,10005]
    n = len(localNodeIds)
    nodeIds = [nodeId[localId] for localId in localNodeIds]
    # Create matrix
    complPrx.createArray([n,n])
    # Set axes
    print nodeIds
    complPrx.setOutputNodeAxis(nodeIds)
    complPrx.setInputNodeAxis(nodeIds)
    logging.info('Created compliance matrix [oid=%d] axis' % compl.id)
    logging.info('Output nodes: %s', complPrx.getOutputNodeAxis())
    logging.info('Input nodes : %s', complPrx.getInputNodeAxis())
    data = [1.0, 0.5, 0.2,\
            0.2, 0.5, 0.3,\
            0.2, 0.5, 1.0]
    # Store matrix
    complPrx.setArrayData([], data)

    #
    # Create sensor
    #
    # Create and fill new Sensor object
    sensor = SenStore.SensorFields()
    sensor.mModel = 'B&K Accelerometer'
    # Request server to add new sensor, and store the assigned object ID
    sensor.id = mngr.addSensor(sensor)
    logging.info('Created sensor [oid=%d]' % sensor.id)

    #
    # Create sensor association
    #
    sensorAssoc = SenStore.SensorTimeDataFields()
    sensorAssoc.mStructure = struct.id
    sensorAssoc.mSensor = sensor.id
    sensorAssoc.mNode = nodeId[10002]
    # Note: Channel is ill defined, because our sensor uses three channels... 
    sensorAssoc.mStartTime = time.time()
    sensorAssoc.mStatus = 'OK'
    sensorAssoc.id = mngr.addSensorTimeData(sensorAssoc)
    logging.info('Created sensor association [oid=%d]' % sensorAssoc.id)

    #
    # Create accelerometer measurement signal
    #
    # Create and fill new AccelMeas object
    meas = SenStore.AccelMeasFields()
    meas.mSensor = sensor.id
    # Request server to add new signal, and store the assigned object ID
    meas.id = mngr.addAccelMeas(meas)
    logging.info('Created AccelMeas [oid=%d]' % meas.id)
    # Get proxy to AccelMeas object, to get access to
    # its functions
    measPrx = mngr.getAccelMeas(meas.id)
    # Define measured directions
    dirs = [SenStore.Quantity.QuantityXDISPLACEMENT,
            SenStore.Quantity.QuantityYDISPLACEMENT,
            SenStore.Quantity.QuantityZDISPLACEMENT]
    # Create data array
    measPrx.createSignal([len(dirs)])
    # Set measured directions
    measPrx.setDirectionAxis(dirs)
    logging.info('Set sensor directions to %s' % \
                     str(measPrx.getDirectionAxis()))
    
    #
    # Create and add random sensor data
    #
    # Note that time is a floating point number, indicating seconds
    # since the epoch (00:00:00 on 1 January 1970), in UTC
    t0 = time.time()
    t = numpy.arange(0.0, 20.0, 0.001) + t0
    data = numpy.random.rand(len(dirs)*len(t))
    # NOTE: when passing numpy arrays, the data type MUST EXACTLY
    # MATCH the interface data type.  Using data.tolist() is safer,
    # but slightly slower.
    measPrx.appendToSignal(t, data.tolist())

    #
    # Get signal properties and data
    #
    print 'Signal size: ', measPrx.getSignalSize()
    t = numpy.arange(t0-10, t0+30,1.0)
    result = measPrx.getTimeAxisInfo(t)
    print 'Time axis info: ', result.t
    result = measPrx.getSignalData(t0+10, 0.5, t0+15, [])
    print numpy.array(result.data).reshape(result.shape)

    # All done
    client.close()
