# Gwendolyn van der Linden
# Copyright (c) 2009 SC Solutions, Inc.
# All rights reserved
# $Id: narada_save.py,v 1.1 2010/09/03 22:29:09 glinden Exp $
import sys
import time
import logging
import numpy
import ConfigParser

# Include client API.
# SenStoreClient is a small wrapper around the client API
# for connection management.  SenStore is the actual client API.
from scshm.database.client.SenStoreClient import SenStoreClient, SenStore

## \file narada_save.py Saves random data to a Narada sensor signal object.

## \package narada_save Saves random data to a Narada sensor signal object.

## Saves random data to a Narada sensor signal object.
#
# The Narada unit and channel number are hard-coded.
# This script is provided as an example, not for actual use.
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
    client = SenStoreClient(config, 'Narada Sensor Data Saver Python client')
    mngr = client.getManager()
    logging.info('Connected to SenStore')

    # (unit, channel) -> signal object ID mapping
    signalPrxTable = {}

    # Get defined signals
    ids = mngr.getDaqUnitChannelDataIds()
    signalPrxs = mngr.getDaqUnitChannelDataList(ids)
    for signalPrx in signalPrxs:
        channelPrx = signalPrx.getChannel()
        unitPrx = channelPrx.getUnit()
        unitNumber = int(unitPrx.getFields().mIdentifier)
        channelNumber = channelPrx.getFields().mNumber
        key = (unitNumber,channelNumber)
        logging.info('Obtained proxy for narada channel signal %d:%d' % key)
        signalPrxTable[key] = signalPrx

    #
    # Example
    #
    t0 = time.time()
    dt = 0.01
    n = 1000
    # Create time axis
    t = numpy.zeros(n)
    for i in range(len(t)):
        t[i] = t0 + i*dt
    # Create dummy data
    data = numpy.random.random_integers(0,2**16-1,n)
    key = (21,2)
    signalPrx = signalPrxTable[key]
    signalPrx.appendToSignal(t.tolist(), data.tolist())
    logging.info('Stored random data in Narada channel signal %d:%d' % key)

    # All done
    client.close()

if __name__ == '__main__':
    main()
