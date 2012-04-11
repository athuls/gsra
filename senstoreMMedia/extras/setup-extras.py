# Gwendolyn van der Linden
# (C) Copyright 2010 SC Solutions, Inc.
# All Rights Reserved.
# $Id: setup-extras.py,v 1.1 2010/05/21 15:48:05 glinden Exp $
import sys
import glob
from distutils.core import setup

setup(name='SenStoreExtras',
      version='0.1.2',
      description='SenStore Structural Health Monitoring System Server',
      author='SC Solutions, Inc.',
      author_email='info@scsolutions.com',
      license='See LICENSE.txt', 
      url='http://www.scsolutions.com/',
      packages = ['senstore.buildtools'],
      data_files = [('Lib/site-packages/senstore',
                     ['LICENSE.txt', 'README.txt'])]
      )
