# Copyright (c) 2010 SC Solutions, Inc.
# See LICENCE.TXT for licensing terms.
# DANGER: wipes out database
import os
import os.path
import time
import logging
import pg
import Ice

## \file scshm_create.py  Database creation script.

## \package scshm_create  Database creation script.

## Creates a PostgreSQL database.
#
# The application properties must include proper settings
# for the PostgreSQL database (see config/server/SenStore.cfg).
#
# @param props   application properties
# @param dbname  name of database
def createDb(props, dbname):
    host = props.getProperty('PostgreSQL.hostname')
    port = props.getProperty('PostgreSQL.port')
    user = props.getProperty('PostgreSQL.username')
    password = props.getProperty('PostgreSQL.password')
    
    db = pg.connect(dbname='template1', host=host, user=user, port=int(port),
                    passwd=password)
    db.query('DROP DATABASE IF EXISTS %s;' % dbname)            
    db.query('CREATE DATABASE %s;' % dbname)

## Executes a PostgreSQL scripts for a given database.
#
# @param props   application properties
# @param dbname  name of database
# @param script  SQL script file name
def execSql(props, dbname, script):
    host = props.getProperty('PostgreSQL.hostname')
    port = props.getProperty('PostgreSQL.port')
    user = props.getProperty('PostgreSQL.username')
    password = props.getProperty('PostgreSQL.password')
    
    db = pg.connect(dbname=dbname, host=host, user=user, port=int(port),
                    passwd=password)
    f = open(script, 'r')
    cmd = ''
    for line in f:
        cmd += line
        if ';' in cmd:
            cmd = cmd.strip()
            print cmd
            db.query(cmd)
            cmd = ''

## Database creation application entry point.
#
# Erases the current PostgreSQL database and HDF5 file if they exist,
# and creates a new empty PostgreSQL database and tables.
#
# @param configFile configuration file name
def main(configFile = 'config/server/SenStore.cfg'):
    # Setup logging
    p = Ice.createProperties()
    p.load(configFile)
    level = p.getProperty('Logging.level').upper()
    try:
        loglevel = logging.__dict__[level]
    except:
       loglevel = logging.INFO
    FORMAT = "%(asctime)-15s:%(levelname)s %(message)s"
    logging.basicConfig(level=loglevel,format=FORMAT)
    
    dbname = p.getProperty('PostgreSQL.database')
    logging.info('Creating new empty SQL database: %s' % dbname)
    createDb(p, dbname)
    
    logging.info('Creating database tables')
    execSql(p, dbname, 'install/tables_create.sql')
    time.sleep(0.1)

    # Remove old HDF5 database
    h5file = p.getProperty('HDF5.database')
    if os.path.exists(h5file):
        logging.info('Removing HDF5 database: %s' % h5file)
        os.unlink(h5file)

if __name__ == '__main__':
    main()