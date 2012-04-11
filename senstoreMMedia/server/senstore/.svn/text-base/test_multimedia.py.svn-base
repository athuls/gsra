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
    logging.info('Connected to SenStore!')
    
    # file = open("/Users/bippy/Documents/Personal/clamChowder.png", "rb")
    file = open("/Users/bippy/Desktop/stuff/clamChowdah.png", "rb")
    
    bytes = file.read()
    #logging.info("Num bytes: %s" % type(bytes))
    
    mngr.uploadFile("bippy.png", "png", "A cool file!", "Longer file", bytes)
    
    # mngr.saveFile("filename.png", open("/Users/bippy/Desktop/stuff/clamChowdah.png", "rb").read())

    logging.info("Saving file...")

    # All done
    client.close()
