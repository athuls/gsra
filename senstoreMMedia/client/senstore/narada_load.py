# Gwendolyn van der Linden
# Copyright (c) 2009 SC Solutions, Inc.
# All rights reserved
# $Id: narada_load.py,v 1.1 2010/09/03 22:29:09 glinden Exp $
import sys
import os.path
import time
import logging
import numpy
import ConfigParser

from scshm.database.client.SenStoreClient import SenStoreClient, SenStore

## \file narada_load.py Narada sensor data extractor example script.

## \package narada_load Narada sensor data extractor example script.

## Extracts the data from a Narada channel to text format.
def main():
    # Setup logging
    FORMAT = "%(asctime)-15s:%(levelname)s %(message)s"
    logging.basicConfig(level=logging.DEBUG,format=FORMAT)

    progname = os.path.basename(sys.argv[0])
    if len(sys.argv) < 3 :
        print 'Usage: %s <unit> <channel> [<start date/time> [<end date/time> [<dt>]]]' % progname
        print 'Gets the signal data from the requested unit/channel from the database.'
        print 'The first column is time [s], since 1970-01-01 Epoch UTC'
        print 'The second column is the Narada unit channel data'
        print ''
        print 'Example: %s 115 2 "2010-06-10 12:00:00" "2010-06-12 13:00:00" 0.1 > u115_ch2.txt' % progname
        print '  gets data at an interval of at most 100ms between the two dates'
        print 'Example: %s 115 2 "2010-06-10 12:00:00" "2010-06-12 13:00:00" > u115_ch2.txt' % progname
        print '  gets all data between the two dates'
        print 'Example: %s 115 2 "2010-06-10 12:00:00" > u115_ch2.txt' % progname
        print '  gets all data since the given date'
        print 'Example: %s 115 2 > u115_ch2.txt' % progname
        print '  gets all data'
        sys.exit(1)
    reqUnitId = sys.argv[1]
    reqChannelNumber= int(sys.argv[2])

    # Load configuration
    config = ConfigParser.ConfigParser()
    config.read(['config/client/SenStore.cfg'])
    
    #
    # Connect to server and get manager proxy
    #
    client = SenStoreClient(config, 'Narada Data Loader Python client')
    mngr = client.getManager()
    logging.info('Connected to SenStore')

    # Get unit object ID
    unitFields = SenStore.DaqUnitFields()
    unitFields.mIdentifier = reqUnitId
    ids = mngr.findEqualDaqUnit(unitFields, ['Identifier'])
    if len(ids) == 0:
        logging.error('Unit %d not found' % reqUnitNumber)
        client.close()
        sys.exit(1)
    unitOID = ids[-1]

    # Get channel object ID
    channelFields = SenStore.DaqUnitChannelFields()
    channelFields.mNumber = reqChannelNumber
    channelFields.mUnit = unitOID
    ids = mngr.findEqualDaqUnitChannel(channelFields, ['Number','Unit'])
    if len(ids) == 0:
        logging.error('Channel %d not found' % reqChannelNumber)
        client.close()
        sys.exit(1)
    channelOID = ids[-1]
    
    # Get channel signal proxy
    signalFields = SenStore.DaqUnitChannelDataFields()
    signalFields.mChannel = channelOID
    ids = mngr.findEqualDaqUnitChannelData(signalFields, ['Channel'])
    if len(ids) == 0:
        logging.error('Signal %d:%d not found' % (reqUnitNumber,reqChannelNumber))
        client.close()
        sys.exit(1)
    signalOID = ids[-1]
    signalPrx = mngr.getDaqUnitChannelData(signalOID)

    #
    # Get signal data
    #
    logging.info('Getting signal data')
    timeInfo = signalPrx.getTimeAxisInfo([])
    if len(timeInfo.t) == 0:
        logging.warn('Signal %d:%d is empty' % (reqUnitNumber,reqChannelNumber))
        client.close()
        sys.exit(0)
    # Set up defaults: all data
    t0 = timeInfo.t[0]
    t1 = timeInfo.t[-1]
    dt = 0.0
    # Override defaults with user provided arguments
    if len(sys.argv) >= 4:
        t0 = time.mktime(time.strptime(sys.argv[3], '%Y-%m-%d %H:%M:%S'))
    if len(sys.argv) >= 5:
        t1 = time.mktime(time.strptime(sys.argv[4], '%Y-%m-%d %H:%M:%S'))
    if len(sys.argv) >= 6:
        dt = float(sys.argv[5])
    if dt > 0.0:
        # Get data no closer than specified sampling interval
        result = signalPrx.getSignalData(t0, dt, t1, [])
        t = numpy.array(result.t)
        y = numpy.array(result.data).reshape(result.shape)
        for i in range(len(t)):
            print t[i], y[i][0]
    else:
        # Get all data, chunked to avoid message size overflow
        timeInfo = signalPrx.getTimeAxisInfo([t0,t1])
        idxStart = timeInfo.idx[0]
        idxStep = 10000
        while True:
            idxEnd = idxStart + idxStep
            result = signalPrx.getSignalDataByIndexRange(idxStart, 1, idxEnd, [])
            t = numpy.array(result.t)
            y = numpy.array(result.data).reshape(result.shape)
            for i in range(len(t)):
                print t[i], y[i][0]
            idxStart = idxEnd
            if idxStart > timeInfo.idx[-1]+1:
                break
            
    # All done
    client.close()

if __name__ == '__main__':
    main()
