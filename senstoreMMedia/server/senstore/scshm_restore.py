# Copyright (c) 2010 SC Solutions, Inc.
# See LICENCE.TXT for licensing terms.
# $Id: scshm_restore.py,v 1.1 2010/09/03 01:20:39 glinden Exp $
import sys
import logging
import os.path
import Ice
from scshm.database.client.backup import *

## \file scshm_restore.py  Database restore script.

## \package scshm_restore  Database restore script.

## Database restore application entry point.
#
# Restores the given backup file to the database.
# @see scshm_backup.main()
# @see scshm.database.client.backup.restore()
def main():
    if len(sys.argv) != 2:
        print 'SenStore database restore'
        print 'Usage: %s <backupfile>' % os.path.basename(sys.argv[0])
    # Load configuration
    config = ConfigParser.ConfigParser()
    config.read(['config/client/SenStore.cfg'])
    # Setup logging
    try:
        level = config.get('Logging', 'LEVEL').upper()
        loglevel = logging.__dict__[level]
    except:
        loglevel = logging.INFO
    FORMAT = "%(asctime)-15s:%(levelname)s %(message)s"
    logging.basicConfig(level=loglevel,format=FORMAT)
    restore(config, sys.argv[1])

if __name__ == '__main__':
    main()