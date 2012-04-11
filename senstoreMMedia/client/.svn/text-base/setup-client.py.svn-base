# Gwendolyn van der Linden
# (C) Copyright 2010 SC Solutions, Inc.
# All Rights Reserved.
# $Id: setup-client.py,v 1.3 2010/09/03 22:29:09 glinden Exp $
import sys
import glob
from distutils.core import setup

setup(name='SenStoreClient',
      version='0.1.2',
      description='SenStore Structural Health Monitoring System Client',
      author='SC Solutions, Inc.',
      author_email='info@scsolutions.com',
      license='See LICENSE.txt', 
      url='http://www.scsolutions.com/',
      packages = ['senstore.scshm',
                  'senstore.scshm.database',
                  'senstore.scshm.database.client',
                  ],
      py_modules = ['senstore.sevenbeambridge',
                    'senstore.narada_create',
                    'senstore.narada_save',
                    'senstore.narada_load',
                    'senstore.narada_query',
                    'setup-client'
                    ],
      data_files = [('Lib/site-packages/senstore/config/client',
                     ['senstore/config/client/SenStore.cfg']),
                    ('Lib/site-packages/senstore',
                     ['LICENSE.txt', 'README.txt', 'senstore/sevenbeambridge.svg',
                      'senstore/narada.cfg']),
                    ('Lib/site-packages/senstore/scshm/database', 
                     ['senstore/scshm/database/SenStore.ice'])
                    ]
      )
