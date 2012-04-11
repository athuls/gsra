# Gwendolyn van der Linden
# (C) Copyright 2010 SC Solutions, Inc.
# All Rights Reserved.
# $Id: setup-server.py,v 1.4 2010/09/03 01:20:38 glinden Exp $
import sys
import glob
from distutils.core import setup

setup(name='SenStoreServer',
      version='0.1.2',
      description='SenStore Structural Health Monitoring System Server',
      author='SC Solutions, Inc.',
      author_email='info@scsolutions.com',
      license='See LICENSE.txt', 
      url='http://www.scsolutions.com/',
      packages = ['senstore.scshm',
                  'senstore.scshm.database',
                  'senstore.scshm.database.server',
                  'senstore.scshm.database.client',
                  ],
      py_modules = ['senstore.scshm_create',
                    'senstore.scshm_start',
                    'senstore.scshm_backup',
                    'senstore.scshm_restore'
                    ],
      data_files = [('Lib/site-packages/senstore/config/server',
                     ['senstore/config/server/SenStore.cfg']),
                    ('Lib/site-packages/senstore/config/client',
                     ['senstore/config/client/SenStore.cfg']),
                    ('Lib/site-packages/senstore/install',
                     ['senstore/install/database_create.sql',
                      'senstore/install/tables_create.sql']),
                    ('Lib/site-packages/senstore',
                     ['LICENSE.txt', 'README.txt']),
                    ('Lib/site-packages/senstore/scshm/database',
                     ['senstore/scshm/database/SenStore.ice'])
                    ]
      )
