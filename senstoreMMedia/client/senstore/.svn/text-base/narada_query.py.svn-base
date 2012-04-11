# Gwendolyn van der Linden
# Copyright (c) 2009 SC Solutions, Inc.
# All rights reserved
# $Id: narada_query.py,v 1.1 2010/09/03 22:29:10 glinden Exp $
import sys
import time
import logging
import numpy
import ConfigParser

# Include client API.
# SenStoreClient is a small wrapper around the client API
# for connection management.  SenStore is the actual client API.
from scshm.database.client.SenStoreClient import SenStoreClient, SenStore

## \file narada_query.py Reports which Narada channels have data.

## \package narada_save Reports which Narada channels have data.

## Reports which Narada channels have data.
#
# This script gets all Narada channels from the server, and shows
# information on the signal data, if any is available.
def main():
    # Setup logging
    FORMAT = "%(asctime)-15s:%(levelname)s %(message)s"
    logging.basicConfig(level=logging.DEBUG,format=FORMAT)

    # Load configuration
    config = ConfigParser.ConfigParser()
    config.read(['config/client/SenStore.cfg'])
    
    #
    # Connect to server and get manager proxy
    #
    client = SenStoreClient(config,'Narada Query Python client')
    mngr = client.getManager()
    logging.info('Connected to SenStore')

    # Get defined signals
    ids = mngr.getDaqUnitChannelDataIds()
    signalPrxs = mngr.getDaqUnitChannelDataList(ids)
    for signalPrx in signalPrxs:
        # Get Narada channel object proxy
        channelPrx = signalPrx.getChannel()
        # Get Narada unit object proxy
        unitPrx = channelPrx.getUnit()
        # Get unit and channel number, which form the key to the signal proxy
        unitNumber = int(unitPrx.getFields().mIdentifier)
        channelNumber = channelPrx.getFields().mNumber
        key = (unitNumber,channelNumber)
        # Get time range of available signal data
        timeInfo = signalPrx.getTimeAxisInfo([])
        if len(timeInfo.t) > 0:
            t0 = timeInfo.t[0]
            t1 = timeInfo.t[-1]
            print 'Narada unit %d channel %d info:' % key
            s0 = time.strftime("%Y-%b-%d %H:%M:%S (%Z)", time.localtime(t0))
            s1 = time.strftime("%Y-%b-%d %H:%M:%S (%Z)", time.localtime(t1))
            print '  time: %s to %s' % (s0, s1)
            print '  sample index: %d to %d' % (timeInfo.idx[0], timeInfo.idx[-1])
            dimInfo = signalPrx.getSignalSize()
            print "  size: %s" % 'x'.join(['%d' % dim for dim in dimInfo])

    # All done
    client.close()

if __name__ == '__main__':
    main()