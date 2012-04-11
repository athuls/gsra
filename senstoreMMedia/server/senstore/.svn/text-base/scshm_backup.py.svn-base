# Copyright (c) 2010 SC Solutions, Inc.
# See LICENCE.TXT for licensing terms.
# $Id: scshm_backup.py,v 1.1 2010/09/03 01:20:39 glinden Exp $
import sys
import time
import logging
import Ice
from scshm.database.client.backup import *

## \file scshm_backup.py  Database backup script.

## \package scshm_backup  Database backup script.

## Connects to the database with an exclusive-access session and
# backups all data in an HDF5 file.  The name of the backup file
# is automatically generated with a current time stamp.
# @see scshm_restore.main()
# @see scshm.database.client.backup.backup()
def main():
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
    t = time.time()
    tstamp = time.strftime('%Y-%m-%d_%H%M', time.localtime(t))
    filename = 'backup-senstore-%s.h5' % tstamp
    backup(config, filename)

if __name__ == '__main__':
    main()