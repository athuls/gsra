# Gwendolyn van der Linden
# Copyright (c) 2009 SC Solutions, Inc.
# All rights reserved
# $Id: sqlclient-tpl.py,v 1.6 2010/09/03 22:29:10 glinden Exp $
import sys
import os.path
import traceback
import logging
import pgdb
import Ice
Ice.loadSlice(os.path.join(os.path.dirname(__file__),\
                           '..', 'SenStore.ice'))
import SenStore

## \package scshm.database.server.SenStore_pgdb
# SenStore PostgreSQL server access implementation.
#
# This module implements access to the SenStore PostgreSQL database.
# The SenStore server assumes it has sole access to the database.
# No special effort has been made to protect data internal integrity,
# other than defining references as foreign keys in the SQL database.
# Hence, other programs writing to the database may cause data
# inconsistencies in the SenStore server,
# and other programs reading the database may experience
# data inconsistencies when the SenStore server changes

def zeroNone(value):
    if value == None:
        return 0
    return value

## PostgreSQL client implementation
#
# Provides object-oriented access to an PostgreSQL database.
class Database:
    ## Initializes the PostgreSQL database connection.
    #
    # @param username  PostgreSQL database user name
    # @param passwd    PostgreSQL database user password
    # @param hostname  PostgreSQL database host
    # @param port      PostgreSQL database port
    # @param dbname    PostgreSQL database name
    def __init__(self, username, passwd, hostname='localhost',
                 port=5432, dbname='SenStore'):
        host = '%s:%d' % (hostname, int(port))
        self.db = pgdb.connect(database=dbname, host=host,
                               user=username, password=passwd)
        self.cur = self.db.cursor()
        # Load enumeration table
        self.MaterialTypeLabelToId = {}
        self.MaterialTypeIdToEnum = {}
        self.cur.execute("SELECT ObjectID,Label from ENUM_MaterialType")
        rows = self.cur.fetchall()
        for id,label in rows:
            self.MaterialTypeLabelToId['MaterialType%s'%label] = id
        id = self.MaterialTypeLabelToId['MaterialTypeELASTIC']
        self.MaterialTypeIdToEnum[id] = \
                SenStore.MaterialType.MaterialTypeELASTIC
        id = self.MaterialTypeLabelToId['MaterialTypeORTHOTROPIC']
        self.MaterialTypeIdToEnum[id] = \
                SenStore.MaterialType.MaterialTypeORTHOTROPIC
        id = self.MaterialTypeLabelToId['MaterialTypeNONLINEARELASTIC']
        self.MaterialTypeIdToEnum[id] = \
                SenStore.MaterialType.MaterialTypeNONLINEARELASTIC
        id = self.MaterialTypeLabelToId['MaterialTypePLASTICBILINEAR']
        self.MaterialTypeIdToEnum[id] = \
                SenStore.MaterialType.MaterialTypePLASTICBILINEAR
        id = self.MaterialTypeLabelToId['MaterialTypePLASTICMULTILINEAR']
        self.MaterialTypeIdToEnum[id] = \
                SenStore.MaterialType.MaterialTypePLASTICMULTILINEAR
        id = self.MaterialTypeLabelToId['MaterialTypeTHERMOISOTROPIC']
        self.MaterialTypeIdToEnum[id] = \
                SenStore.MaterialType.MaterialTypeTHERMOISOTROPIC
        id = self.MaterialTypeLabelToId['MaterialTypeTHERMOORTHOTROPIC']
        self.MaterialTypeIdToEnum[id] = \
                SenStore.MaterialType.MaterialTypeTHERMOORTHOTROPIC
        # Load enumeration table
        self.UnitLabelToId = {}
        self.UnitIdToEnum = {}
        self.cur.execute("SELECT ObjectID,Label from ENUM_Unit")
        rows = self.cur.fetchall()
        for id,label in rows:
            self.UnitLabelToId['Unit%s'%label] = id
        id = self.UnitLabelToId['UnitMETER']
        self.UnitIdToEnum[id] = \
                SenStore.Unit.UnitMETER
        id = self.UnitLabelToId['UnitKILOGRAM']
        self.UnitIdToEnum[id] = \
                SenStore.Unit.UnitKILOGRAM
        id = self.UnitLabelToId['UnitNEWTON']
        self.UnitIdToEnum[id] = \
                SenStore.Unit.UnitNEWTON
        id = self.UnitLabelToId['UnitFOOT']
        self.UnitIdToEnum[id] = \
                SenStore.Unit.UnitFOOT
        id = self.UnitLabelToId['UnitPOUND']
        self.UnitIdToEnum[id] = \
                SenStore.Unit.UnitPOUND
        id = self.UnitLabelToId['UnitKILOPOUND']
        self.UnitIdToEnum[id] = \
                SenStore.Unit.UnitKILOPOUND
        id = self.UnitLabelToId['UnitSLUG']
        self.UnitIdToEnum[id] = \
                SenStore.Unit.UnitSLUG
        id = self.UnitLabelToId['UnitKILOSLUG']
        self.UnitIdToEnum[id] = \
                SenStore.Unit.UnitKILOSLUG
        # Load enumeration table
        self.QuantityLabelToId = {}
        self.QuantityIdToEnum = {}
        self.cur.execute("SELECT ObjectID,Label from ENUM_Quantity")
        rows = self.cur.fetchall()
        for id,label in rows:
            self.QuantityLabelToId['Quantity%s'%label] = id
        id = self.QuantityLabelToId['QuantityXDISPLACEMENT']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityXDISPLACEMENT
        id = self.QuantityLabelToId['QuantityYDISPLACEMENT']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityYDISPLACEMENT
        id = self.QuantityLabelToId['QuantityZDISPLACEMENT']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityZDISPLACEMENT
        id = self.QuantityLabelToId['QuantityXROTATION']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityXROTATION
        id = self.QuantityLabelToId['QuantityYROTATION']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityYROTATION
        id = self.QuantityLabelToId['QuantityZROTATION']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityZROTATION
        id = self.QuantityLabelToId['QuantityXFORCE']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityXFORCE
        id = self.QuantityLabelToId['QuantityYFORCE']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityYFORCE
        id = self.QuantityLabelToId['QuantityZFORCE']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityZFORCE
        id = self.QuantityLabelToId['QuantityRAW']
        self.QuantityIdToEnum[id] = \
                SenStore.Quantity.QuantityRAW
        # Load enumeration table
        self.CoordinateSystemTypeLabelToId = {}
        self.CoordinateSystemTypeIdToEnum = {}
        self.cur.execute("SELECT ObjectID,Label from ENUM_CoordinateSystemType")
        rows = self.cur.fetchall()
        for id,label in rows:
            self.CoordinateSystemTypeLabelToId['CoordinateSystemType%s'%label] = id
        id = self.CoordinateSystemTypeLabelToId['CoordinateSystemTypeCARTESIAN']
        self.CoordinateSystemTypeIdToEnum[id] = \
                SenStore.CoordinateSystemType.CoordinateSystemTypeCARTESIAN
        id = self.CoordinateSystemTypeLabelToId['CoordinateSystemTypeSPHERICAL']
        self.CoordinateSystemTypeIdToEnum[id] = \
                SenStore.CoordinateSystemType.CoordinateSystemTypeSPHERICAL
        id = self.CoordinateSystemTypeLabelToId['CoordinateSystemTypeCYLINDRICAL']
        self.CoordinateSystemTypeIdToEnum[id] = \
                SenStore.CoordinateSystemType.CoordinateSystemTypeCYLINDRICAL
        # Load enumeration table
        self.BoundaryTypeLabelToId = {}
        self.BoundaryTypeIdToEnum = {}
        self.cur.execute("SELECT ObjectID,Label from ENUM_BoundaryType")
        rows = self.cur.fetchall()
        for id,label in rows:
            self.BoundaryTypeLabelToId['BoundaryType%s'%label] = id
        id = self.BoundaryTypeLabelToId['BoundaryTypeFREE']
        self.BoundaryTypeIdToEnum[id] = \
                SenStore.BoundaryType.BoundaryTypeFREE
        id = self.BoundaryTypeLabelToId['BoundaryTypeFIXED']
        self.BoundaryTypeIdToEnum[id] = \
                SenStore.BoundaryType.BoundaryTypeFIXED
        # Load enumeration table
        self.SectionTypeLabelToId = {}
        self.SectionTypeIdToEnum = {}
        self.cur.execute("SELECT ObjectID,Label from ENUM_SectionType")
        rows = self.cur.fetchall()
        for id,label in rows:
            self.SectionTypeLabelToId['SectionType%s'%label] = id
        id = self.SectionTypeLabelToId['SectionTypeBOX']
        self.SectionTypeIdToEnum[id] = \
                SenStore.SectionType.SectionTypeBOX
        id = self.SectionTypeLabelToId['SectionTypePROPERTIES']
        self.SectionTypeIdToEnum[id] = \
                SenStore.SectionType.SectionTypePROPERTIES
        id = self.SectionTypeLabelToId['SectionTypeRECTANGULAR']
        self.SectionTypeIdToEnum[id] = \
                SenStore.SectionType.SectionTypeRECTANGULAR
        id = self.SectionTypeLabelToId['SectionTypePIPE']
        self.SectionTypeIdToEnum[id] = \
                SenStore.SectionType.SectionTypePIPE
        id = self.SectionTypeLabelToId['SectionTypeI']
        self.SectionTypeIdToEnum[id] = \
                SenStore.SectionType.SectionTypeI
        id = self.SectionTypeLabelToId['SectionTypeL']
        self.SectionTypeIdToEnum[id] = \
                SenStore.SectionType.SectionTypeL
        id = self.SectionTypeLabelToId['SectionTypeU']
        self.SectionTypeIdToEnum[id] = \
                SenStore.SectionType.SectionTypeU
        # Load enumeration table
        self.GroupTypeLabelToId = {}
        self.GroupTypeIdToEnum = {}
        self.cur.execute("SELECT ObjectID,Label from ENUM_GroupType")
        rows = self.cur.fetchall()
        for id,label in rows:
            self.GroupTypeLabelToId['GroupType%s'%label] = id
        id = self.GroupTypeLabelToId['GroupTypeBEAM']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypeBEAM
        id = self.GroupTypeLabelToId['GroupTypeTRUSS']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypeTRUSS
        id = self.GroupTypeLabelToId['GroupTypeGENERAL']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypeGENERAL
        id = self.GroupTypeLabelToId['GroupTypeISOBEAM']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypeISOBEAM
        id = self.GroupTypeLabelToId['GroupTypePLATE']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypePLATE
        id = self.GroupTypeLabelToId['GroupTypeSHELL']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypeSHELL
        id = self.GroupTypeLabelToId['GroupTypeSPRING']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypeSPRING
        id = self.GroupTypeLabelToId['GroupTypeTHREEDSOLID']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypeTHREEDSOLID
        id = self.GroupTypeLabelToId['GroupTypeTWODSOLID']
        self.GroupTypeIdToEnum[id] = \
                SenStore.GroupType.GroupTypeTWODSOLID
        # Load enumeration table
        self.SensorTypeLabelToId = {}
        self.SensorTypeIdToEnum = {}
        self.cur.execute("SELECT ObjectID,Label from ENUM_SensorType")
        rows = self.cur.fetchall()
        for id,label in rows:
            self.SensorTypeLabelToId['SensorType%s'%label] = id
        id = self.SensorTypeLabelToId['SensorTypeACCELEROMETER']
        self.SensorTypeIdToEnum[id] = \
                SenStore.SensorType.SensorTypeACCELEROMETER
        id = self.SensorTypeLabelToId['SensorTypeANEMOMETER']
        self.SensorTypeIdToEnum[id] = \
                SenStore.SensorType.SensorTypeANEMOMETER
        id = self.SensorTypeLabelToId['SensorTypeWINDVANE']
        self.SensorTypeIdToEnum[id] = \
                SenStore.SensorType.SensorTypeWINDVANE
        id = self.SensorTypeLabelToId['SensorTypeSTRAINGAUGE']
        self.SensorTypeIdToEnum[id] = \
                SenStore.SensorType.SensorTypeSTRAINGAUGE

    ## Adds a Structure object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  Structure object fields
    def addStructure(self, fields):
        insertCmd = 'INSERT INTO CLASS_Structure '\
            '(var_Name,var_Description,var_Type,ref_DistanceUnit,ref_ForceUnit,ref_WeightUnit) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mName)
        args.append(fields.mDescription)
        args.append(fields.mType)
        args.append(self.UnitLabelToId[str(fields.mDistanceUnit)])
        args.append(self.UnitLabelToId[str(fields.mForceUnit)])
        args.append(self.UnitLabelToId[str(fields.mWeightUnit)])
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_Structure_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of Structure objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of Structure object fields
    def addStructureList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_Structure '\
            '(var_Name,var_Description,var_Type,ref_DistanceUnit,ref_ForceUnit,ref_WeightUnit) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mName)
            args.append(fields.mDescription)
            args.append(fields.mType)
            args.append(self.UnitLabelToId[str(fields.mDistanceUnit)])
            args.append(self.UnitLabelToId[str(fields.mForceUnit)])
            args.append(self.UnitLabelToId[str(fields.mWeightUnit)])
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_Structure_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a Structure object in the database.
    #
    # @param fields     Structure object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setStructure(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Name' in fieldNames):
            cmds.append("var_Name=%s")
            args.append(fields.mName)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Type' in fieldNames):
            cmds.append("var_Type=%s")
            args.append(fields.mType)
        if allFields or ('DistanceUnit' in fieldNames):
            cmds.append("ref_DistanceUnit=%s")
            args.append(self.UnitLabelToId[str(fields.mDistanceUnit)])
        if allFields or ('ForceUnit' in fieldNames):
            cmds.append("ref_ForceUnit=%s")
            args.append(self.UnitLabelToId[str(fields.mForceUnit)])
        if allFields or ('WeightUnit' in fieldNames):
            cmds.append("ref_WeightUnit=%s")
            args.append(self.UnitLabelToId[str(fields.mWeightUnit)])
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_Structure SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the Structure object IDs from the database.
    def getStructureIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_Structure'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the Structure object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  Structure object fields
    # @return \c True on success, \c False if object not found
    def getStructure(self, fields):
        selectCmd = 'SELECT '\
            'var_Name,var_Description,var_Type,ref_DistanceUnit,ref_ForceUnit,ref_WeightUnit '\
            'FROM CLASS_Structure WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mName = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mType = data.pop(0)
        fields.mDistanceUnit = self.UnitIdToEnum[data.pop(0)]
        fields.mForceUnit = self.UnitIdToEnum[data.pop(0)]
        fields.mWeightUnit = self.UnitIdToEnum[data.pop(0)]
        return True

    ## Gets a list of IDs of all Structure objects matching the given
    # reference data.
    #
    # @param fields Structure object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching Structure objects
    def findEqualStructure(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Name' in fieldNames):
            cmds.append("var_Name=%s")
            args.append(fields.mName)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Type' in fieldNames):
            cmds.append("var_Type=%s")
            args.append(fields.mType)
        if (len(fieldNames)) == 0 or ('DistanceUnit' in fieldNames):
            cmds.append("ref_DistanceUnit=%s")
            args.append(self.UnitLabelToId[str(fields.mDistanceUnit)])
        if (len(fieldNames)) == 0 or ('ForceUnit' in fieldNames):
            cmds.append("ref_ForceUnit=%s")
            args.append(self.UnitLabelToId[str(fields.mForceUnit)])
        if (len(fieldNames)) == 0 or ('WeightUnit' in fieldNames):
            cmds.append("ref_WeightUnit=%s")
            args.append(self.UnitLabelToId[str(fields.mWeightUnit)])
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_Structure WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMDof object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMDof object fields
    def addFEMDof(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMDof '\
            '(ref_Structure,var_LocalId,ref_Node,ref_Direction) VALUES '\
            "(%s,%s,%s,%s)"
        args = []
        if fields.mStructure == 0:
            args.append(None)
        else:
            args.append(fields.mStructure)
        args.append(fields.mLocalId)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(self.QuantityLabelToId[str(fields.mDirection)])
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMDof_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMDof objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMDof object fields
    def addFEMDofList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMDof '\
            '(ref_Structure,var_LocalId,ref_Node,ref_Direction) VALUES '\
            "(%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mStructure == 0:
                args.append(None)
            else:
                args.append(fields.mStructure)
            args.append(fields.mLocalId)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(self.QuantityLabelToId[str(fields.mDirection)])
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMDof_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMDof object in the database.
    #
    # @param fields     FEMDof object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMDof(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if allFields or ('LocalId' in fieldNames):
            cmds.append("var_LocalId=%s")
            args.append(fields.mLocalId)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('Direction' in fieldNames):
            cmds.append("ref_Direction=%s")
            args.append(self.QuantityLabelToId[str(fields.mDirection)])
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMDof SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMDof object IDs from the database.
    def getFEMDofIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMDof'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMDof object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMDof object fields
    # @return \c True on success, \c False if object not found
    def getFEMDof(self, fields):
        selectCmd = 'SELECT '\
            'ref_Structure,var_LocalId,ref_Node,ref_Direction '\
            'FROM CLASS_FEMDof WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mStructure = zeroNone(data.pop(0))
        fields.mLocalId = data.pop(0)
        fields.mNode = zeroNone(data.pop(0))
        fields.mDirection = self.QuantityIdToEnum[data.pop(0)]
        return True

    ## Gets a list of IDs of all FEMDof objects matching the given
    # reference data.
    #
    # @param fields FEMDof object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMDof objects
    def findEqualFEMDof(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if (len(fieldNames)) == 0 or ('LocalId' in fieldNames):
            cmds.append("var_LocalId=%s")
            args.append(fields.mLocalId)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('Direction' in fieldNames):
            cmds.append("ref_Direction=%s")
            args.append(self.QuantityLabelToId[str(fields.mDirection)])
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMDof WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMNodalMass object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMNodalMass object fields
    def addFEMNodalMass(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMNodalMass '\
            '(var_Description,var_Mass1,var_Mass2,var_Mass3,var_Mass4,var_Mass5,var_Mass6,ref_Node) VALUES '\
            "(%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mMass1)
        args.append(fields.mMass2)
        args.append(fields.mMass3)
        args.append(fields.mMass4)
        args.append(fields.mMass5)
        args.append(fields.mMass6)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMNodalMass_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMNodalMass objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMNodalMass object fields
    def addFEMNodalMassList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMNodalMass '\
            '(var_Description,var_Mass1,var_Mass2,var_Mass3,var_Mass4,var_Mass5,var_Mass6,ref_Node) VALUES '\
            "(%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mMass1)
            args.append(fields.mMass2)
            args.append(fields.mMass3)
            args.append(fields.mMass4)
            args.append(fields.mMass5)
            args.append(fields.mMass6)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMNodalMass_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMNodalMass object in the database.
    #
    # @param fields     FEMNodalMass object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMNodalMass(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Mass1' in fieldNames):
            cmds.append("var_Mass1=%0.16g")
            args.append(fields.mMass1)
        if allFields or ('Mass2' in fieldNames):
            cmds.append("var_Mass2=%0.16g")
            args.append(fields.mMass2)
        if allFields or ('Mass3' in fieldNames):
            cmds.append("var_Mass3=%0.16g")
            args.append(fields.mMass3)
        if allFields or ('Mass4' in fieldNames):
            cmds.append("var_Mass4=%0.16g")
            args.append(fields.mMass4)
        if allFields or ('Mass5' in fieldNames):
            cmds.append("var_Mass5=%0.16g")
            args.append(fields.mMass5)
        if allFields or ('Mass6' in fieldNames):
            cmds.append("var_Mass6=%0.16g")
            args.append(fields.mMass6)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMNodalMass SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMNodalMass object IDs from the database.
    def getFEMNodalMassIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNodalMass'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMNodalMass object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMNodalMass object fields
    # @return \c True on success, \c False if object not found
    def getFEMNodalMass(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_Mass1,var_Mass2,var_Mass3,var_Mass4,var_Mass5,var_Mass6,ref_Node '\
            'FROM CLASS_FEMNodalMass WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mMass1 = data.pop(0)
        fields.mMass2 = data.pop(0)
        fields.mMass3 = data.pop(0)
        fields.mMass4 = data.pop(0)
        fields.mMass5 = data.pop(0)
        fields.mMass6 = data.pop(0)
        fields.mNode = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all FEMNodalMass objects matching the given
    # reference data.
    #
    # @param fields FEMNodalMass object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMNodalMass objects
    def findEqualFEMNodalMass(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Mass1' in fieldNames):
            cmds.append("var_Mass1=%0.16g")
            args.append(fields.mMass1)
        if (len(fieldNames)) == 0 or ('Mass2' in fieldNames):
            cmds.append("var_Mass2=%0.16g")
            args.append(fields.mMass2)
        if (len(fieldNames)) == 0 or ('Mass3' in fieldNames):
            cmds.append("var_Mass3=%0.16g")
            args.append(fields.mMass3)
        if (len(fieldNames)) == 0 or ('Mass4' in fieldNames):
            cmds.append("var_Mass4=%0.16g")
            args.append(fields.mMass4)
        if (len(fieldNames)) == 0 or ('Mass5' in fieldNames):
            cmds.append("var_Mass5=%0.16g")
            args.append(fields.mMass5)
        if (len(fieldNames)) == 0 or ('Mass6' in fieldNames):
            cmds.append("var_Mass6=%0.16g")
            args.append(fields.mMass6)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNodalMass WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMNLElasticStrainStress object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMNLElasticStrainStress object fields
    def addFEMNLElasticStrainStress(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMNLElasticStrainStress '\
            '(var_MaterialID,var_RecordNumber,var_Strain,var_Stress) VALUES '\
            "(%s,%s,%0.16g,%0.16g)"
        args = []
        args.append(fields.mMaterialID)
        args.append(fields.mRecordNumber)
        args.append(fields.mStrain)
        args.append(fields.mStress)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMNLElasticStrainStress_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMNLElasticStrainStress objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMNLElasticStrainStress object fields
    def addFEMNLElasticStrainStressList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMNLElasticStrainStress '\
            '(var_MaterialID,var_RecordNumber,var_Strain,var_Stress) VALUES '\
            "(%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mMaterialID)
            args.append(fields.mRecordNumber)
            args.append(fields.mStrain)
            args.append(fields.mStress)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMNLElasticStrainStress_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMNLElasticStrainStress object in the database.
    #
    # @param fields     FEMNLElasticStrainStress object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMNLElasticStrainStress(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('RecordNumber' in fieldNames):
            cmds.append("var_RecordNumber=%s")
            args.append(fields.mRecordNumber)
        if allFields or ('Strain' in fieldNames):
            cmds.append("var_Strain=%0.16g")
            args.append(fields.mStrain)
        if allFields or ('Stress' in fieldNames):
            cmds.append("var_Stress=%0.16g")
            args.append(fields.mStress)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMNLElasticStrainStress SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMNLElasticStrainStress object IDs from the database.
    def getFEMNLElasticStrainStressIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNLElasticStrainStress'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMNLElasticStrainStress object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMNLElasticStrainStress object fields
    # @return \c True on success, \c False if object not found
    def getFEMNLElasticStrainStress(self, fields):
        selectCmd = 'SELECT '\
            'var_MaterialID,var_RecordNumber,var_Strain,var_Stress '\
            'FROM CLASS_FEMNLElasticStrainStress WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mMaterialID = data.pop(0)
        fields.mRecordNumber = data.pop(0)
        fields.mStrain = data.pop(0)
        fields.mStress = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMNLElasticStrainStress objects matching the given
    # reference data.
    #
    # @param fields FEMNLElasticStrainStress object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMNLElasticStrainStress objects
    def findEqualFEMNLElasticStrainStress(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('RecordNumber' in fieldNames):
            cmds.append("var_RecordNumber=%s")
            args.append(fields.mRecordNumber)
        if (len(fieldNames)) == 0 or ('Strain' in fieldNames):
            cmds.append("var_Strain=%0.16g")
            args.append(fields.mStrain)
        if (len(fieldNames)) == 0 or ('Stress' in fieldNames):
            cmds.append("var_Stress=%0.16g")
            args.append(fields.mStress)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNLElasticStrainStress WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMBoundary object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMBoundary object fields
    def addFEMBoundary(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMBoundary '\
            '(ref_Node,ref_Ovalization,ref_Phi,ref_Rx,ref_Ry,ref_Rz,ref_Ux,ref_Uy,ref_Uz,var_Warping) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        args = []
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(self.BoundaryTypeLabelToId[str(fields.mOvalization)])
        args.append(self.BoundaryTypeLabelToId[str(fields.mPhi)])
        args.append(self.BoundaryTypeLabelToId[str(fields.mRx)])
        args.append(self.BoundaryTypeLabelToId[str(fields.mRy)])
        args.append(self.BoundaryTypeLabelToId[str(fields.mRz)])
        args.append(self.BoundaryTypeLabelToId[str(fields.mUx)])
        args.append(self.BoundaryTypeLabelToId[str(fields.mUy)])
        args.append(self.BoundaryTypeLabelToId[str(fields.mUz)])
        args.append(fields.mWarping)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMBoundary_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMBoundary objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMBoundary object fields
    def addFEMBoundaryList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMBoundary '\
            '(ref_Node,ref_Ovalization,ref_Phi,ref_Rx,ref_Ry,ref_Rz,ref_Ux,ref_Uy,ref_Uz,var_Warping) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(self.BoundaryTypeLabelToId[str(fields.mOvalization)])
            args.append(self.BoundaryTypeLabelToId[str(fields.mPhi)])
            args.append(self.BoundaryTypeLabelToId[str(fields.mRx)])
            args.append(self.BoundaryTypeLabelToId[str(fields.mRy)])
            args.append(self.BoundaryTypeLabelToId[str(fields.mRz)])
            args.append(self.BoundaryTypeLabelToId[str(fields.mUx)])
            args.append(self.BoundaryTypeLabelToId[str(fields.mUy)])
            args.append(self.BoundaryTypeLabelToId[str(fields.mUz)])
            args.append(fields.mWarping)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMBoundary_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMBoundary object in the database.
    #
    # @param fields     FEMBoundary object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMBoundary(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('Ovalization' in fieldNames):
            cmds.append("ref_Ovalization=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mOvalization)])
        if allFields or ('Phi' in fieldNames):
            cmds.append("ref_Phi=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mPhi)])
        if allFields or ('Rx' in fieldNames):
            cmds.append("ref_Rx=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mRx)])
        if allFields or ('Ry' in fieldNames):
            cmds.append("ref_Ry=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mRy)])
        if allFields or ('Rz' in fieldNames):
            cmds.append("ref_Rz=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mRz)])
        if allFields or ('Ux' in fieldNames):
            cmds.append("ref_Ux=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mUx)])
        if allFields or ('Uy' in fieldNames):
            cmds.append("ref_Uy=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mUy)])
        if allFields or ('Uz' in fieldNames):
            cmds.append("ref_Uz=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mUz)])
        if allFields or ('Warping' in fieldNames):
            cmds.append("var_Warping=%s")
            args.append(fields.mWarping)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMBoundary SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMBoundary object IDs from the database.
    def getFEMBoundaryIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMBoundary'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMBoundary object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMBoundary object fields
    # @return \c True on success, \c False if object not found
    def getFEMBoundary(self, fields):
        selectCmd = 'SELECT '\
            'ref_Node,ref_Ovalization,ref_Phi,ref_Rx,ref_Ry,ref_Rz,ref_Ux,ref_Uy,ref_Uz,var_Warping '\
            'FROM CLASS_FEMBoundary WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mNode = zeroNone(data.pop(0))
        fields.mOvalization = self.BoundaryTypeIdToEnum[data.pop(0)]
        fields.mPhi = self.BoundaryTypeIdToEnum[data.pop(0)]
        fields.mRx = self.BoundaryTypeIdToEnum[data.pop(0)]
        fields.mRy = self.BoundaryTypeIdToEnum[data.pop(0)]
        fields.mRz = self.BoundaryTypeIdToEnum[data.pop(0)]
        fields.mUx = self.BoundaryTypeIdToEnum[data.pop(0)]
        fields.mUy = self.BoundaryTypeIdToEnum[data.pop(0)]
        fields.mUz = self.BoundaryTypeIdToEnum[data.pop(0)]
        fields.mWarping = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMBoundary objects matching the given
    # reference data.
    #
    # @param fields FEMBoundary object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMBoundary objects
    def findEqualFEMBoundary(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('Ovalization' in fieldNames):
            cmds.append("ref_Ovalization=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mOvalization)])
        if (len(fieldNames)) == 0 or ('Phi' in fieldNames):
            cmds.append("ref_Phi=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mPhi)])
        if (len(fieldNames)) == 0 or ('Rx' in fieldNames):
            cmds.append("ref_Rx=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mRx)])
        if (len(fieldNames)) == 0 or ('Ry' in fieldNames):
            cmds.append("ref_Ry=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mRy)])
        if (len(fieldNames)) == 0 or ('Rz' in fieldNames):
            cmds.append("ref_Rz=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mRz)])
        if (len(fieldNames)) == 0 or ('Ux' in fieldNames):
            cmds.append("ref_Ux=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mUx)])
        if (len(fieldNames)) == 0 or ('Uy' in fieldNames):
            cmds.append("ref_Uy=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mUy)])
        if (len(fieldNames)) == 0 or ('Uz' in fieldNames):
            cmds.append("ref_Uz=%s")
            args.append(self.BoundaryTypeLabelToId[str(fields.mUz)])
        if (len(fieldNames)) == 0 or ('Warping' in fieldNames):
            cmds.append("var_Warping=%s")
            args.append(fields.mWarping)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMBoundary WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSectionPipe object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSectionPipe object fields
    def addFEMSectionPipe(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSectionPipe '\
            '(var_Diameter,var_SC,var_SectionID,var_SSarea,var_TC,var_Thickness,var_Torfac,var_TSarea) VALUES '\
            "(%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDiameter)
        args.append(fields.mSC)
        args.append(fields.mSectionID)
        args.append(fields.mSSarea)
        args.append(fields.mTC)
        args.append(fields.mThickness)
        args.append(fields.mTorfac)
        args.append(fields.mTSarea)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSectionPipe_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSectionPipe objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSectionPipe object fields
    def addFEMSectionPipeList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSectionPipe '\
            '(var_Diameter,var_SC,var_SectionID,var_SSarea,var_TC,var_Thickness,var_Torfac,var_TSarea) VALUES '\
            "(%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDiameter)
            args.append(fields.mSC)
            args.append(fields.mSectionID)
            args.append(fields.mSSarea)
            args.append(fields.mTC)
            args.append(fields.mThickness)
            args.append(fields.mTorfac)
            args.append(fields.mTSarea)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSectionPipe_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSectionPipe object in the database.
    #
    # @param fields     FEMSectionPipe object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSectionPipe(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Diameter' in fieldNames):
            cmds.append("var_Diameter=%0.16g")
            args.append(fields.mDiameter)
        if allFields or ('SC' in fieldNames):
            cmds.append("var_SC=%0.16g")
            args.append(fields.mSC)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('SSarea' in fieldNames):
            cmds.append("var_SSarea=%0.16g")
            args.append(fields.mSSarea)
        if allFields or ('TC' in fieldNames):
            cmds.append("var_TC=%0.16g")
            args.append(fields.mTC)
        if allFields or ('Thickness' in fieldNames):
            cmds.append("var_Thickness=%0.16g")
            args.append(fields.mThickness)
        if allFields or ('Torfac' in fieldNames):
            cmds.append("var_Torfac=%0.16g")
            args.append(fields.mTorfac)
        if allFields or ('TSarea' in fieldNames):
            cmds.append("var_TSarea=%0.16g")
            args.append(fields.mTSarea)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSectionPipe SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSectionPipe object IDs from the database.
    def getFEMSectionPipeIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionPipe'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSectionPipe object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSectionPipe object fields
    # @return \c True on success, \c False if object not found
    def getFEMSectionPipe(self, fields):
        selectCmd = 'SELECT '\
            'var_Diameter,var_SC,var_SectionID,var_SSarea,var_TC,var_Thickness,var_Torfac,var_TSarea '\
            'FROM CLASS_FEMSectionPipe WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDiameter = data.pop(0)
        fields.mSC = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mSSarea = data.pop(0)
        fields.mTC = data.pop(0)
        fields.mThickness = data.pop(0)
        fields.mTorfac = data.pop(0)
        fields.mTSarea = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSectionPipe objects matching the given
    # reference data.
    #
    # @param fields FEMSectionPipe object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSectionPipe objects
    def findEqualFEMSectionPipe(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Diameter' in fieldNames):
            cmds.append("var_Diameter=%0.16g")
            args.append(fields.mDiameter)
        if (len(fieldNames)) == 0 or ('SC' in fieldNames):
            cmds.append("var_SC=%0.16g")
            args.append(fields.mSC)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('SSarea' in fieldNames):
            cmds.append("var_SSarea=%0.16g")
            args.append(fields.mSSarea)
        if (len(fieldNames)) == 0 or ('TC' in fieldNames):
            cmds.append("var_TC=%0.16g")
            args.append(fields.mTC)
        if (len(fieldNames)) == 0 or ('Thickness' in fieldNames):
            cmds.append("var_Thickness=%0.16g")
            args.append(fields.mThickness)
        if (len(fieldNames)) == 0 or ('Torfac' in fieldNames):
            cmds.append("var_Torfac=%0.16g")
            args.append(fields.mTorfac)
        if (len(fieldNames)) == 0 or ('TSarea' in fieldNames):
            cmds.append("var_TSarea=%0.16g")
            args.append(fields.mTSarea)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionPipe WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMCoordSystem object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMCoordSystem object fields
    def addFEMCoordSystem(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMCoordSystem '\
            '(var_AX,var_AY,var_AZ,var_BBY,var_BX,var_BZ,var_Description,var_Mode,var_P1,var_P2,var_P3,var_Type,var_Xorigin,var_Yorigin,var_Zorigin,var_LocalID) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g,%s)"
        args = []
        args.append(fields.mAX)
        args.append(fields.mAY)
        args.append(fields.mAZ)
        args.append(fields.mBBY)
        args.append(fields.mBX)
        args.append(fields.mBZ)
        args.append(fields.mDescription)
        args.append(fields.mMode)
        args.append(fields.mP1)
        args.append(fields.mP2)
        args.append(fields.mP3)
        args.append(fields.mType)
        args.append(fields.mXorigin)
        args.append(fields.mYorigin)
        args.append(fields.mZorigin)
        args.append(fields.mLocalID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMCoordSystem_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMCoordSystem objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMCoordSystem object fields
    def addFEMCoordSystemList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMCoordSystem '\
            '(var_AX,var_AY,var_AZ,var_BBY,var_BX,var_BZ,var_Description,var_Mode,var_P1,var_P2,var_P3,var_Type,var_Xorigin,var_Yorigin,var_Zorigin,var_LocalID) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAX)
            args.append(fields.mAY)
            args.append(fields.mAZ)
            args.append(fields.mBBY)
            args.append(fields.mBX)
            args.append(fields.mBZ)
            args.append(fields.mDescription)
            args.append(fields.mMode)
            args.append(fields.mP1)
            args.append(fields.mP2)
            args.append(fields.mP3)
            args.append(fields.mType)
            args.append(fields.mXorigin)
            args.append(fields.mYorigin)
            args.append(fields.mZorigin)
            args.append(fields.mLocalID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMCoordSystem_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMCoordSystem object in the database.
    #
    # @param fields     FEMCoordSystem object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMCoordSystem(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AX' in fieldNames):
            cmds.append("var_AX=%0.16g")
            args.append(fields.mAX)
        if allFields or ('AY' in fieldNames):
            cmds.append("var_AY=%0.16g")
            args.append(fields.mAY)
        if allFields or ('AZ' in fieldNames):
            cmds.append("var_AZ=%0.16g")
            args.append(fields.mAZ)
        if allFields or ('BBY' in fieldNames):
            cmds.append("var_BBY=%0.16g")
            args.append(fields.mBBY)
        if allFields or ('BX' in fieldNames):
            cmds.append("var_BX=%0.16g")
            args.append(fields.mBX)
        if allFields or ('BZ' in fieldNames):
            cmds.append("var_BZ=%0.16g")
            args.append(fields.mBZ)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Mode' in fieldNames):
            cmds.append("var_Mode=%s")
            args.append(fields.mMode)
        if allFields or ('P1' in fieldNames):
            cmds.append("var_P1=%s")
            args.append(fields.mP1)
        if allFields or ('P2' in fieldNames):
            cmds.append("var_P2=%s")
            args.append(fields.mP2)
        if allFields or ('P3' in fieldNames):
            cmds.append("var_P3=%s")
            args.append(fields.mP3)
        if allFields or ('Type' in fieldNames):
            cmds.append("var_Type=%s")
            args.append(fields.mType)
        if allFields or ('Xorigin' in fieldNames):
            cmds.append("var_Xorigin=%0.16g")
            args.append(fields.mXorigin)
        if allFields or ('Yorigin' in fieldNames):
            cmds.append("var_Yorigin=%0.16g")
            args.append(fields.mYorigin)
        if allFields or ('Zorigin' in fieldNames):
            cmds.append("var_Zorigin=%0.16g")
            args.append(fields.mZorigin)
        if allFields or ('LocalID' in fieldNames):
            cmds.append("var_LocalID=%s")
            args.append(fields.mLocalID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMCoordSystem SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMCoordSystem object IDs from the database.
    def getFEMCoordSystemIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMCoordSystem'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMCoordSystem object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMCoordSystem object fields
    # @return \c True on success, \c False if object not found
    def getFEMCoordSystem(self, fields):
        selectCmd = 'SELECT '\
            'var_AX,var_AY,var_AZ,var_BBY,var_BX,var_BZ,var_Description,var_Mode,var_P1,var_P2,var_P3,var_Type,var_Xorigin,var_Yorigin,var_Zorigin,var_LocalID '\
            'FROM CLASS_FEMCoordSystem WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAX = data.pop(0)
        fields.mAY = data.pop(0)
        fields.mAZ = data.pop(0)
        fields.mBBY = data.pop(0)
        fields.mBX = data.pop(0)
        fields.mBZ = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mMode = data.pop(0)
        fields.mP1 = data.pop(0)
        fields.mP2 = data.pop(0)
        fields.mP3 = data.pop(0)
        fields.mType = data.pop(0)
        fields.mXorigin = data.pop(0)
        fields.mYorigin = data.pop(0)
        fields.mZorigin = data.pop(0)
        fields.mLocalID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMCoordSystem objects matching the given
    # reference data.
    #
    # @param fields FEMCoordSystem object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMCoordSystem objects
    def findEqualFEMCoordSystem(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AX' in fieldNames):
            cmds.append("var_AX=%0.16g")
            args.append(fields.mAX)
        if (len(fieldNames)) == 0 or ('AY' in fieldNames):
            cmds.append("var_AY=%0.16g")
            args.append(fields.mAY)
        if (len(fieldNames)) == 0 or ('AZ' in fieldNames):
            cmds.append("var_AZ=%0.16g")
            args.append(fields.mAZ)
        if (len(fieldNames)) == 0 or ('BBY' in fieldNames):
            cmds.append("var_BBY=%0.16g")
            args.append(fields.mBBY)
        if (len(fieldNames)) == 0 or ('BX' in fieldNames):
            cmds.append("var_BX=%0.16g")
            args.append(fields.mBX)
        if (len(fieldNames)) == 0 or ('BZ' in fieldNames):
            cmds.append("var_BZ=%0.16g")
            args.append(fields.mBZ)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Mode' in fieldNames):
            cmds.append("var_Mode=%s")
            args.append(fields.mMode)
        if (len(fieldNames)) == 0 or ('P1' in fieldNames):
            cmds.append("var_P1=%s")
            args.append(fields.mP1)
        if (len(fieldNames)) == 0 or ('P2' in fieldNames):
            cmds.append("var_P2=%s")
            args.append(fields.mP2)
        if (len(fieldNames)) == 0 or ('P3' in fieldNames):
            cmds.append("var_P3=%s")
            args.append(fields.mP3)
        if (len(fieldNames)) == 0 or ('Type' in fieldNames):
            cmds.append("var_Type=%s")
            args.append(fields.mType)
        if (len(fieldNames)) == 0 or ('Xorigin' in fieldNames):
            cmds.append("var_Xorigin=%0.16g")
            args.append(fields.mXorigin)
        if (len(fieldNames)) == 0 or ('Yorigin' in fieldNames):
            cmds.append("var_Yorigin=%0.16g")
            args.append(fields.mYorigin)
        if (len(fieldNames)) == 0 or ('Zorigin' in fieldNames):
            cmds.append("var_Zorigin=%0.16g")
            args.append(fields.mZorigin)
        if (len(fieldNames)) == 0 or ('LocalID' in fieldNames):
            cmds.append("var_LocalID=%s")
            args.append(fields.mLocalID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMCoordSystem WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMNode object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMNode object fields
    def addFEMNode(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMNode '\
            '(var_Description,ref_System,var_X,var_Y,var_Z,var_LocalID) VALUES '\
            "(%s,%s,%0.16g,%0.16g,%0.16g,%s)"
        args = []
        args.append(fields.mDescription)
        if fields.mSystem == 0:
            args.append(None)
        else:
            args.append(fields.mSystem)
        args.append(fields.mX)
        args.append(fields.mY)
        args.append(fields.mZ)
        args.append(fields.mLocalID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMNode_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMNode objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMNode object fields
    def addFEMNodeList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMNode '\
            '(var_Description,ref_System,var_X,var_Y,var_Z,var_LocalID) VALUES '\
            "(%s,%s,%0.16g,%0.16g,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            if fields.mSystem == 0:
                args.append(None)
            else:
                args.append(fields.mSystem)
            args.append(fields.mX)
            args.append(fields.mY)
            args.append(fields.mZ)
            args.append(fields.mLocalID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMNode_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMNode object in the database.
    #
    # @param fields     FEMNode object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMNode(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('System' in fieldNames):
            cmds.append("ref_System=%s")
            args.append(fields.mSystem)
        if allFields or ('X' in fieldNames):
            cmds.append("var_X=%0.16g")
            args.append(fields.mX)
        if allFields or ('Y' in fieldNames):
            cmds.append("var_Y=%0.16g")
            args.append(fields.mY)
        if allFields or ('Z' in fieldNames):
            cmds.append("var_Z=%0.16g")
            args.append(fields.mZ)
        if allFields or ('LocalID' in fieldNames):
            cmds.append("var_LocalID=%s")
            args.append(fields.mLocalID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMNode SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMNode object IDs from the database.
    def getFEMNodeIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNode'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMNode object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMNode object fields
    # @return \c True on success, \c False if object not found
    def getFEMNode(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,ref_System,var_X,var_Y,var_Z,var_LocalID '\
            'FROM CLASS_FEMNode WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mSystem = zeroNone(data.pop(0))
        fields.mX = data.pop(0)
        fields.mY = data.pop(0)
        fields.mZ = data.pop(0)
        fields.mLocalID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMNode objects matching the given
    # reference data.
    #
    # @param fields FEMNode object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMNode objects
    def findEqualFEMNode(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('System' in fieldNames):
            cmds.append("ref_System=%s")
            args.append(fields.mSystem)
        if (len(fieldNames)) == 0 or ('X' in fieldNames):
            cmds.append("var_X=%0.16g")
            args.append(fields.mX)
        if (len(fieldNames)) == 0 or ('Y' in fieldNames):
            cmds.append("var_Y=%0.16g")
            args.append(fields.mY)
        if (len(fieldNames)) == 0 or ('Z' in fieldNames):
            cmds.append("var_Z=%0.16g")
            args.append(fields.mZ)
        if (len(fieldNames)) == 0 or ('LocalID' in fieldNames):
            cmds.append("var_LocalID=%s")
            args.append(fields.mLocalID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNode WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMTruss object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMTruss object fields
    def addFEMTruss(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMTruss '\
            '(var_Description,var_ElementID,var_Epsin,var_Gapwidth,ref_Group,ref_Material,ref_N1,ref_N2,var_RecordNmb,var_Save,var_SectionArea,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        args.append(fields.mEpsin)
        args.append(fields.mGapwidth)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        if fields.mMaterial == 0:
            args.append(None)
        else:
            args.append(fields.mMaterial)
        if fields.mN1 == 0:
            args.append(None)
        else:
            args.append(fields.mN1)
        if fields.mN2 == 0:
            args.append(None)
        else:
            args.append(fields.mN2)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        args.append(fields.mSectionArea)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMTruss_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMTruss objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMTruss object fields
    def addFEMTrussList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMTruss '\
            '(var_Description,var_ElementID,var_Epsin,var_Gapwidth,ref_Group,ref_Material,ref_N1,ref_N2,var_RecordNmb,var_Save,var_SectionArea,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            args.append(fields.mEpsin)
            args.append(fields.mGapwidth)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            if fields.mMaterial == 0:
                args.append(None)
            else:
                args.append(fields.mMaterial)
            if fields.mN1 == 0:
                args.append(None)
            else:
                args.append(fields.mN1)
            if fields.mN2 == 0:
                args.append(None)
            else:
                args.append(fields.mN2)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            args.append(fields.mSectionArea)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMTruss_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMTruss object in the database.
    #
    # @param fields     FEMTruss object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMTruss(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Epsin' in fieldNames):
            cmds.append("var_Epsin=%0.16g")
            args.append(fields.mEpsin)
        if allFields or ('Gapwidth' in fieldNames):
            cmds.append("var_Gapwidth=%0.16g")
            args.append(fields.mGapwidth)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if allFields or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if allFields or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if allFields or ('SectionArea' in fieldNames):
            cmds.append("var_SectionArea=%0.16g")
            args.append(fields.mSectionArea)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMTruss SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMTruss object IDs from the database.
    def getFEMTrussIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTruss'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMTruss object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMTruss object fields
    # @return \c True on success, \c False if object not found
    def getFEMTruss(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_ElementID,var_Epsin,var_Gapwidth,ref_Group,ref_Material,ref_N1,ref_N2,var_RecordNmb,var_Save,var_SectionArea,var_TBirth,var_TDeath '\
            'FROM CLASS_FEMTruss WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mEpsin = data.pop(0)
        fields.mGapwidth = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterial = zeroNone(data.pop(0))
        fields.mN1 = zeroNone(data.pop(0))
        fields.mN2 = zeroNone(data.pop(0))
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        fields.mSectionArea = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMTruss objects matching the given
    # reference data.
    #
    # @param fields FEMTruss object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMTruss objects
    def findEqualFEMTruss(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Epsin' in fieldNames):
            cmds.append("var_Epsin=%0.16g")
            args.append(fields.mEpsin)
        if (len(fieldNames)) == 0 or ('Gapwidth' in fieldNames):
            cmds.append("var_Gapwidth=%0.16g")
            args.append(fields.mGapwidth)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if (len(fieldNames)) == 0 or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if (len(fieldNames)) == 0 or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if (len(fieldNames)) == 0 or ('SectionArea' in fieldNames):
            cmds.append("var_SectionArea=%0.16g")
            args.append(fields.mSectionArea)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTruss WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMTimeFunctionData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMTimeFunctionData object fields
    def addFEMTimeFunctionData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMTimeFunctionData '\
            '(var_DataTime,var_GMRecordID,var_RecordNmb,var_TimeFunctionID,var_TimeValue) VALUES '\
            "(%0.16g,%s,%s,%s,%0.16g)"
        args = []
        args.append(fields.mDataTime)
        args.append(fields.mGMRecordID)
        args.append(fields.mRecordNmb)
        args.append(fields.mTimeFunctionID)
        args.append(fields.mTimeValue)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMTimeFunctionData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMTimeFunctionData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMTimeFunctionData object fields
    def addFEMTimeFunctionDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMTimeFunctionData '\
            '(var_DataTime,var_GMRecordID,var_RecordNmb,var_TimeFunctionID,var_TimeValue) VALUES '\
            "(%0.16g,%s,%s,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDataTime)
            args.append(fields.mGMRecordID)
            args.append(fields.mRecordNmb)
            args.append(fields.mTimeFunctionID)
            args.append(fields.mTimeValue)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMTimeFunctionData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMTimeFunctionData object in the database.
    #
    # @param fields     FEMTimeFunctionData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMTimeFunctionData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('DataTime' in fieldNames):
            cmds.append("var_DataTime=%0.16g")
            args.append(fields.mDataTime)
        if allFields or ('GMRecordID' in fieldNames):
            cmds.append("var_GMRecordID=%s")
            args.append(fields.mGMRecordID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if allFields or ('TimeValue' in fieldNames):
            cmds.append("var_TimeValue=%0.16g")
            args.append(fields.mTimeValue)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMTimeFunctionData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMTimeFunctionData object IDs from the database.
    def getFEMTimeFunctionDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTimeFunctionData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMTimeFunctionData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMTimeFunctionData object fields
    # @return \c True on success, \c False if object not found
    def getFEMTimeFunctionData(self, fields):
        selectCmd = 'SELECT '\
            'var_DataTime,var_GMRecordID,var_RecordNmb,var_TimeFunctionID,var_TimeValue '\
            'FROM CLASS_FEMTimeFunctionData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDataTime = data.pop(0)
        fields.mGMRecordID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTimeFunctionID = data.pop(0)
        fields.mTimeValue = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMTimeFunctionData objects matching the given
    # reference data.
    #
    # @param fields FEMTimeFunctionData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMTimeFunctionData objects
    def findEqualFEMTimeFunctionData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('DataTime' in fieldNames):
            cmds.append("var_DataTime=%0.16g")
            args.append(fields.mDataTime)
        if (len(fieldNames)) == 0 or ('GMRecordID' in fieldNames):
            cmds.append("var_GMRecordID=%s")
            args.append(fields.mGMRecordID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if (len(fieldNames)) == 0 or ('TimeValue' in fieldNames):
            cmds.append("var_TimeValue=%0.16g")
            args.append(fields.mTimeValue)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTimeFunctionData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMPlasticMlMaterials object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMPlasticMlMaterials object fields
    def addFEMPlasticMlMaterials(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMPlasticMlMaterials '\
            '(var_ALPHA,var_Density,var_E,var_Hardening,var_MaterialID,var_NU,var_TREF) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%s,%0.16g,%0.16g)"
        args = []
        args.append(fields.mALPHA)
        args.append(fields.mDensity)
        args.append(fields.mE)
        args.append(fields.mHardening)
        args.append(fields.mMaterialID)
        args.append(fields.mNU)
        args.append(fields.mTREF)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMPlasticMlMaterials_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMPlasticMlMaterials objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMPlasticMlMaterials object fields
    def addFEMPlasticMlMaterialsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMPlasticMlMaterials '\
            '(var_ALPHA,var_Density,var_E,var_Hardening,var_MaterialID,var_NU,var_TREF) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mALPHA)
            args.append(fields.mDensity)
            args.append(fields.mE)
            args.append(fields.mHardening)
            args.append(fields.mMaterialID)
            args.append(fields.mNU)
            args.append(fields.mTREF)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMPlasticMlMaterials_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMPlasticMlMaterials object in the database.
    #
    # @param fields     FEMPlasticMlMaterials object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMPlasticMlMaterials(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ALPHA' in fieldNames):
            cmds.append("var_ALPHA=%0.16g")
            args.append(fields.mALPHA)
        if allFields or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if allFields or ('E' in fieldNames):
            cmds.append("var_E=%0.16g")
            args.append(fields.mE)
        if allFields or ('Hardening' in fieldNames):
            cmds.append("var_Hardening=%s")
            args.append(fields.mHardening)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('NU' in fieldNames):
            cmds.append("var_NU=%0.16g")
            args.append(fields.mNU)
        if allFields or ('TREF' in fieldNames):
            cmds.append("var_TREF=%0.16g")
            args.append(fields.mTREF)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMPlasticMlMaterials SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMPlasticMlMaterials object IDs from the database.
    def getFEMPlasticMlMaterialsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlasticMlMaterials'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMPlasticMlMaterials object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMPlasticMlMaterials object fields
    # @return \c True on success, \c False if object not found
    def getFEMPlasticMlMaterials(self, fields):
        selectCmd = 'SELECT '\
            'var_ALPHA,var_Density,var_E,var_Hardening,var_MaterialID,var_NU,var_TREF '\
            'FROM CLASS_FEMPlasticMlMaterials WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mALPHA = data.pop(0)
        fields.mDensity = data.pop(0)
        fields.mE = data.pop(0)
        fields.mHardening = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mNU = data.pop(0)
        fields.mTREF = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMPlasticMlMaterials objects matching the given
    # reference data.
    #
    # @param fields FEMPlasticMlMaterials object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMPlasticMlMaterials objects
    def findEqualFEMPlasticMlMaterials(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ALPHA' in fieldNames):
            cmds.append("var_ALPHA=%0.16g")
            args.append(fields.mALPHA)
        if (len(fieldNames)) == 0 or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if (len(fieldNames)) == 0 or ('E' in fieldNames):
            cmds.append("var_E=%0.16g")
            args.append(fields.mE)
        if (len(fieldNames)) == 0 or ('Hardening' in fieldNames):
            cmds.append("var_Hardening=%s")
            args.append(fields.mHardening)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('NU' in fieldNames):
            cmds.append("var_NU=%0.16g")
            args.append(fields.mNU)
        if (len(fieldNames)) == 0 or ('TREF' in fieldNames):
            cmds.append("var_TREF=%0.16g")
            args.append(fields.mTREF)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlasticMlMaterials WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMPlateGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMPlateGroup object fields
    def addFEMPlateGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMPlateGroup '\
            '(var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_Result) VALUES '\
            "(%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDisplacement)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mIniStrain)
        args.append(fields.mMaterialID)
        args.append(fields.mResult)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMPlateGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMPlateGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMPlateGroup object fields
    def addFEMPlateGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMPlateGroup '\
            '(var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_Result) VALUES '\
            "(%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDisplacement)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mIniStrain)
            args.append(fields.mMaterialID)
            args.append(fields.mResult)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMPlateGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMPlateGroup object in the database.
    #
    # @param fields     FEMPlateGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMPlateGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('IniStrain' in fieldNames):
            cmds.append("var_IniStrain=%s")
            args.append(fields.mIniStrain)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMPlateGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMPlateGroup object IDs from the database.
    def getFEMPlateGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlateGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMPlateGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMPlateGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMPlateGroup(self, fields):
        selectCmd = 'SELECT '\
            'var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_Result '\
            'FROM CLASS_FEMPlateGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDisplacement = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mIniStrain = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mResult = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMPlateGroup objects matching the given
    # reference data.
    #
    # @param fields FEMPlateGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMPlateGroup objects
    def findEqualFEMPlateGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('IniStrain' in fieldNames):
            cmds.append("var_IniStrain=%s")
            args.append(fields.mIniStrain)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlateGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMBeam object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMBeam object fields
    def addFEMBeam(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMBeam '\
            '(ref_AuxNode,var_Description,var_ElementID,var_EndRelease,var_Epsin,ref_Group,var_IRigidEnd,var_JRigidEnd,var_MaterialID,ref_Node1,ref_Node2,var_RecordNmb,var_Save,var_SectionID,var_Subdivision,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%s,%0.16g,%s,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g)"
        args = []
        if fields.mAuxNode == 0:
            args.append(None)
        else:
            args.append(fields.mAuxNode)
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        args.append(fields.mEndRelease)
        args.append(fields.mEpsin)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mIRigidEnd)
        args.append(fields.mJRigidEnd)
        args.append(fields.mMaterialID)
        if fields.mNode1 == 0:
            args.append(None)
        else:
            args.append(fields.mNode1)
        if fields.mNode2 == 0:
            args.append(None)
        else:
            args.append(fields.mNode2)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        args.append(fields.mSectionID)
        args.append(fields.mSubdivision)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMBeam_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMBeam objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMBeam object fields
    def addFEMBeamList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMBeam '\
            '(ref_AuxNode,var_Description,var_ElementID,var_EndRelease,var_Epsin,ref_Group,var_IRigidEnd,var_JRigidEnd,var_MaterialID,ref_Node1,ref_Node2,var_RecordNmb,var_Save,var_SectionID,var_Subdivision,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%s,%0.16g,%s,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mAuxNode == 0:
                args.append(None)
            else:
                args.append(fields.mAuxNode)
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            args.append(fields.mEndRelease)
            args.append(fields.mEpsin)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mIRigidEnd)
            args.append(fields.mJRigidEnd)
            args.append(fields.mMaterialID)
            if fields.mNode1 == 0:
                args.append(None)
            else:
                args.append(fields.mNode1)
            if fields.mNode2 == 0:
                args.append(None)
            else:
                args.append(fields.mNode2)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            args.append(fields.mSectionID)
            args.append(fields.mSubdivision)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMBeam_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMBeam object in the database.
    #
    # @param fields     FEMBeam object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMBeam(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AuxNode' in fieldNames):
            cmds.append("ref_AuxNode=%s")
            args.append(fields.mAuxNode)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('EndRelease' in fieldNames):
            cmds.append("var_EndRelease=%s")
            args.append(fields.mEndRelease)
        if allFields or ('Epsin' in fieldNames):
            cmds.append("var_Epsin=%0.16g")
            args.append(fields.mEpsin)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('IRigidEnd' in fieldNames):
            cmds.append("var_IRigidEnd=%0.16g")
            args.append(fields.mIRigidEnd)
        if allFields or ('JRigidEnd' in fieldNames):
            cmds.append("var_JRigidEnd=%0.16g")
            args.append(fields.mJRigidEnd)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('Node1' in fieldNames):
            cmds.append("ref_Node1=%s")
            args.append(fields.mNode1)
        if allFields or ('Node2' in fieldNames):
            cmds.append("ref_Node2=%s")
            args.append(fields.mNode2)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('Subdivision' in fieldNames):
            cmds.append("var_Subdivision=%s")
            args.append(fields.mSubdivision)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMBeam SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMBeam object IDs from the database.
    def getFEMBeamIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMBeam'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMBeam object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMBeam object fields
    # @return \c True on success, \c False if object not found
    def getFEMBeam(self, fields):
        selectCmd = 'SELECT '\
            'ref_AuxNode,var_Description,var_ElementID,var_EndRelease,var_Epsin,ref_Group,var_IRigidEnd,var_JRigidEnd,var_MaterialID,ref_Node1,ref_Node2,var_RecordNmb,var_Save,var_SectionID,var_Subdivision,var_TBirth,var_TDeath '\
            'FROM CLASS_FEMBeam WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAuxNode = zeroNone(data.pop(0))
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mEndRelease = data.pop(0)
        fields.mEpsin = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mIRigidEnd = data.pop(0)
        fields.mJRigidEnd = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mNode1 = zeroNone(data.pop(0))
        fields.mNode2 = zeroNone(data.pop(0))
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mSubdivision = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMBeam objects matching the given
    # reference data.
    #
    # @param fields FEMBeam object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMBeam objects
    def findEqualFEMBeam(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AuxNode' in fieldNames):
            cmds.append("ref_AuxNode=%s")
            args.append(fields.mAuxNode)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('EndRelease' in fieldNames):
            cmds.append("var_EndRelease=%s")
            args.append(fields.mEndRelease)
        if (len(fieldNames)) == 0 or ('Epsin' in fieldNames):
            cmds.append("var_Epsin=%0.16g")
            args.append(fields.mEpsin)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('IRigidEnd' in fieldNames):
            cmds.append("var_IRigidEnd=%0.16g")
            args.append(fields.mIRigidEnd)
        if (len(fieldNames)) == 0 or ('JRigidEnd' in fieldNames):
            cmds.append("var_JRigidEnd=%0.16g")
            args.append(fields.mJRigidEnd)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('Node1' in fieldNames):
            cmds.append("ref_Node1=%s")
            args.append(fields.mNode1)
        if (len(fieldNames)) == 0 or ('Node2' in fieldNames):
            cmds.append("ref_Node2=%s")
            args.append(fields.mNode2)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('Subdivision' in fieldNames):
            cmds.append("var_Subdivision=%s")
            args.append(fields.mSubdivision)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMBeam WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMCurvMomentData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMCurvMomentData object fields
    def addFEMCurvMomentData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMCurvMomentData '\
            '(var_Curvature,var_CurvMomentID,var_Moment,var_RecordNmb) VALUES '\
            "(%0.16g,%s,%0.16g,%s)"
        args = []
        args.append(fields.mCurvature)
        args.append(fields.mCurvMomentID)
        args.append(fields.mMoment)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMCurvMomentData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMCurvMomentData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMCurvMomentData object fields
    def addFEMCurvMomentDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMCurvMomentData '\
            '(var_Curvature,var_CurvMomentID,var_Moment,var_RecordNmb) VALUES '\
            "(%0.16g,%s,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mCurvature)
            args.append(fields.mCurvMomentID)
            args.append(fields.mMoment)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMCurvMomentData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMCurvMomentData object in the database.
    #
    # @param fields     FEMCurvMomentData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMCurvMomentData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Curvature' in fieldNames):
            cmds.append("var_Curvature=%0.16g")
            args.append(fields.mCurvature)
        if allFields or ('CurvMomentID' in fieldNames):
            cmds.append("var_CurvMomentID=%s")
            args.append(fields.mCurvMomentID)
        if allFields or ('Moment' in fieldNames):
            cmds.append("var_Moment=%0.16g")
            args.append(fields.mMoment)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMCurvMomentData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMCurvMomentData object IDs from the database.
    def getFEMCurvMomentDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMCurvMomentData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMCurvMomentData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMCurvMomentData object fields
    # @return \c True on success, \c False if object not found
    def getFEMCurvMomentData(self, fields):
        selectCmd = 'SELECT '\
            'var_Curvature,var_CurvMomentID,var_Moment,var_RecordNmb '\
            'FROM CLASS_FEMCurvMomentData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mCurvature = data.pop(0)
        fields.mCurvMomentID = data.pop(0)
        fields.mMoment = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMCurvMomentData objects matching the given
    # reference data.
    #
    # @param fields FEMCurvMomentData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMCurvMomentData objects
    def findEqualFEMCurvMomentData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Curvature' in fieldNames):
            cmds.append("var_Curvature=%0.16g")
            args.append(fields.mCurvature)
        if (len(fieldNames)) == 0 or ('CurvMomentID' in fieldNames):
            cmds.append("var_CurvMomentID=%s")
            args.append(fields.mCurvMomentID)
        if (len(fieldNames)) == 0 or ('Moment' in fieldNames):
            cmds.append("var_Moment=%0.16g")
            args.append(fields.mMoment)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMCurvMomentData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMPropertysets object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMPropertysets object fields
    def addFEMPropertysets(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMPropertysets '\
            '(var_C,var_Description,var_K,var_M,var_NC,var_NK,var_NM,var_Nonlinear,var_PropertysetID,var_S) VALUES '\
            "(%0.16g,%s,%0.16g,%0.16g,%s,%s,%s,%s,%s,%0.16g)"
        args = []
        args.append(fields.mC)
        args.append(fields.mDescription)
        args.append(fields.mK)
        args.append(fields.mM)
        args.append(fields.mNC)
        args.append(fields.mNK)
        args.append(fields.mNM)
        args.append(fields.mNonlinear)
        args.append(fields.mPropertysetID)
        args.append(fields.mS)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMPropertysets_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMPropertysets objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMPropertysets object fields
    def addFEMPropertysetsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMPropertysets '\
            '(var_C,var_Description,var_K,var_M,var_NC,var_NK,var_NM,var_Nonlinear,var_PropertysetID,var_S) VALUES '\
            "(%0.16g,%s,%0.16g,%0.16g,%s,%s,%s,%s,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mC)
            args.append(fields.mDescription)
            args.append(fields.mK)
            args.append(fields.mM)
            args.append(fields.mNC)
            args.append(fields.mNK)
            args.append(fields.mNM)
            args.append(fields.mNonlinear)
            args.append(fields.mPropertysetID)
            args.append(fields.mS)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMPropertysets_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMPropertysets object in the database.
    #
    # @param fields     FEMPropertysets object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMPropertysets(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('C' in fieldNames):
            cmds.append("var_C=%0.16g")
            args.append(fields.mC)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('K' in fieldNames):
            cmds.append("var_K=%0.16g")
            args.append(fields.mK)
        if allFields or ('M' in fieldNames):
            cmds.append("var_M=%0.16g")
            args.append(fields.mM)
        if allFields or ('NC' in fieldNames):
            cmds.append("var_NC=%s")
            args.append(fields.mNC)
        if allFields or ('NK' in fieldNames):
            cmds.append("var_NK=%s")
            args.append(fields.mNK)
        if allFields or ('NM' in fieldNames):
            cmds.append("var_NM=%s")
            args.append(fields.mNM)
        if allFields or ('Nonlinear' in fieldNames):
            cmds.append("var_Nonlinear=%s")
            args.append(fields.mNonlinear)
        if allFields or ('PropertysetID' in fieldNames):
            cmds.append("var_PropertysetID=%s")
            args.append(fields.mPropertysetID)
        if allFields or ('S' in fieldNames):
            cmds.append("var_S=%0.16g")
            args.append(fields.mS)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMPropertysets SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMPropertysets object IDs from the database.
    def getFEMPropertysetsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPropertysets'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMPropertysets object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMPropertysets object fields
    # @return \c True on success, \c False if object not found
    def getFEMPropertysets(self, fields):
        selectCmd = 'SELECT '\
            'var_C,var_Description,var_K,var_M,var_NC,var_NK,var_NM,var_Nonlinear,var_PropertysetID,var_S '\
            'FROM CLASS_FEMPropertysets WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mC = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mK = data.pop(0)
        fields.mM = data.pop(0)
        fields.mNC = data.pop(0)
        fields.mNK = data.pop(0)
        fields.mNM = data.pop(0)
        fields.mNonlinear = data.pop(0)
        fields.mPropertysetID = data.pop(0)
        fields.mS = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMPropertysets objects matching the given
    # reference data.
    #
    # @param fields FEMPropertysets object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMPropertysets objects
    def findEqualFEMPropertysets(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('C' in fieldNames):
            cmds.append("var_C=%0.16g")
            args.append(fields.mC)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('K' in fieldNames):
            cmds.append("var_K=%0.16g")
            args.append(fields.mK)
        if (len(fieldNames)) == 0 or ('M' in fieldNames):
            cmds.append("var_M=%0.16g")
            args.append(fields.mM)
        if (len(fieldNames)) == 0 or ('NC' in fieldNames):
            cmds.append("var_NC=%s")
            args.append(fields.mNC)
        if (len(fieldNames)) == 0 or ('NK' in fieldNames):
            cmds.append("var_NK=%s")
            args.append(fields.mNK)
        if (len(fieldNames)) == 0 or ('NM' in fieldNames):
            cmds.append("var_NM=%s")
            args.append(fields.mNM)
        if (len(fieldNames)) == 0 or ('Nonlinear' in fieldNames):
            cmds.append("var_Nonlinear=%s")
            args.append(fields.mNonlinear)
        if (len(fieldNames)) == 0 or ('PropertysetID' in fieldNames):
            cmds.append("var_PropertysetID=%s")
            args.append(fields.mPropertysetID)
        if (len(fieldNames)) == 0 or ('S' in fieldNames):
            cmds.append("var_S=%0.16g")
            args.append(fields.mS)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPropertysets WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMOrthotropicMaterial object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMOrthotropicMaterial object fields
    def addFEMOrthotropicMaterial(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMOrthotropicMaterial '\
            '(var_Density,var_EA,var_EB,var_EC,var_GAB,var_GAC,var_GBC,ref_Material,var_NUAB,var_NUAC,var_NUBC) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDensity)
        args.append(fields.mEA)
        args.append(fields.mEB)
        args.append(fields.mEC)
        args.append(fields.mGAB)
        args.append(fields.mGAC)
        args.append(fields.mGBC)
        if fields.mMaterial == 0:
            args.append(None)
        else:
            args.append(fields.mMaterial)
        args.append(fields.mNUAB)
        args.append(fields.mNUAC)
        args.append(fields.mNUBC)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMOrthotropicMaterial_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMOrthotropicMaterial objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMOrthotropicMaterial object fields
    def addFEMOrthotropicMaterialList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMOrthotropicMaterial '\
            '(var_Density,var_EA,var_EB,var_EC,var_GAB,var_GAC,var_GBC,ref_Material,var_NUAB,var_NUAC,var_NUBC) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDensity)
            args.append(fields.mEA)
            args.append(fields.mEB)
            args.append(fields.mEC)
            args.append(fields.mGAB)
            args.append(fields.mGAC)
            args.append(fields.mGBC)
            if fields.mMaterial == 0:
                args.append(None)
            else:
                args.append(fields.mMaterial)
            args.append(fields.mNUAB)
            args.append(fields.mNUAC)
            args.append(fields.mNUBC)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMOrthotropicMaterial_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMOrthotropicMaterial object in the database.
    #
    # @param fields     FEMOrthotropicMaterial object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMOrthotropicMaterial(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if allFields or ('EA' in fieldNames):
            cmds.append("var_EA=%0.16g")
            args.append(fields.mEA)
        if allFields or ('EB' in fieldNames):
            cmds.append("var_EB=%0.16g")
            args.append(fields.mEB)
        if allFields or ('EC' in fieldNames):
            cmds.append("var_EC=%0.16g")
            args.append(fields.mEC)
        if allFields or ('GAB' in fieldNames):
            cmds.append("var_GAB=%0.16g")
            args.append(fields.mGAB)
        if allFields or ('GAC' in fieldNames):
            cmds.append("var_GAC=%0.16g")
            args.append(fields.mGAC)
        if allFields or ('GBC' in fieldNames):
            cmds.append("var_GBC=%0.16g")
            args.append(fields.mGBC)
        if allFields or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if allFields or ('NUAB' in fieldNames):
            cmds.append("var_NUAB=%0.16g")
            args.append(fields.mNUAB)
        if allFields or ('NUAC' in fieldNames):
            cmds.append("var_NUAC=%0.16g")
            args.append(fields.mNUAC)
        if allFields or ('NUBC' in fieldNames):
            cmds.append("var_NUBC=%0.16g")
            args.append(fields.mNUBC)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMOrthotropicMaterial SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMOrthotropicMaterial object IDs from the database.
    def getFEMOrthotropicMaterialIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMOrthotropicMaterial'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMOrthotropicMaterial object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMOrthotropicMaterial object fields
    # @return \c True on success, \c False if object not found
    def getFEMOrthotropicMaterial(self, fields):
        selectCmd = 'SELECT '\
            'var_Density,var_EA,var_EB,var_EC,var_GAB,var_GAC,var_GBC,ref_Material,var_NUAB,var_NUAC,var_NUBC '\
            'FROM CLASS_FEMOrthotropicMaterial WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDensity = data.pop(0)
        fields.mEA = data.pop(0)
        fields.mEB = data.pop(0)
        fields.mEC = data.pop(0)
        fields.mGAB = data.pop(0)
        fields.mGAC = data.pop(0)
        fields.mGBC = data.pop(0)
        fields.mMaterial = zeroNone(data.pop(0))
        fields.mNUAB = data.pop(0)
        fields.mNUAC = data.pop(0)
        fields.mNUBC = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMOrthotropicMaterial objects matching the given
    # reference data.
    #
    # @param fields FEMOrthotropicMaterial object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMOrthotropicMaterial objects
    def findEqualFEMOrthotropicMaterial(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if (len(fieldNames)) == 0 or ('EA' in fieldNames):
            cmds.append("var_EA=%0.16g")
            args.append(fields.mEA)
        if (len(fieldNames)) == 0 or ('EB' in fieldNames):
            cmds.append("var_EB=%0.16g")
            args.append(fields.mEB)
        if (len(fieldNames)) == 0 or ('EC' in fieldNames):
            cmds.append("var_EC=%0.16g")
            args.append(fields.mEC)
        if (len(fieldNames)) == 0 or ('GAB' in fieldNames):
            cmds.append("var_GAB=%0.16g")
            args.append(fields.mGAB)
        if (len(fieldNames)) == 0 or ('GAC' in fieldNames):
            cmds.append("var_GAC=%0.16g")
            args.append(fields.mGAC)
        if (len(fieldNames)) == 0 or ('GBC' in fieldNames):
            cmds.append("var_GBC=%0.16g")
            args.append(fields.mGBC)
        if (len(fieldNames)) == 0 or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if (len(fieldNames)) == 0 or ('NUAB' in fieldNames):
            cmds.append("var_NUAB=%0.16g")
            args.append(fields.mNUAB)
        if (len(fieldNames)) == 0 or ('NUAC' in fieldNames):
            cmds.append("var_NUAC=%0.16g")
            args.append(fields.mNUAC)
        if (len(fieldNames)) == 0 or ('NUBC' in fieldNames):
            cmds.append("var_NUBC=%0.16g")
            args.append(fields.mNUBC)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMOrthotropicMaterial WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMAppliedLoads object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMAppliedLoads object fields
    def addFEMAppliedLoads(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMAppliedLoads '\
            '(var_AppliedLoadNmb,var_ArrivalTime,var_LoadID,var_LoadType,var_SiteType,var_TimeFunctionID) VALUES '\
            "(%s,%0.16g,%s,%s,%s,%s)"
        args = []
        args.append(fields.mAppliedLoadNmb)
        args.append(fields.mArrivalTime)
        args.append(fields.mLoadID)
        args.append(fields.mLoadType)
        args.append(fields.mSiteType)
        args.append(fields.mTimeFunctionID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMAppliedLoads_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMAppliedLoads objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMAppliedLoads object fields
    def addFEMAppliedLoadsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMAppliedLoads '\
            '(var_AppliedLoadNmb,var_ArrivalTime,var_LoadID,var_LoadType,var_SiteType,var_TimeFunctionID) VALUES '\
            "(%s,%0.16g,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAppliedLoadNmb)
            args.append(fields.mArrivalTime)
            args.append(fields.mLoadID)
            args.append(fields.mLoadType)
            args.append(fields.mSiteType)
            args.append(fields.mTimeFunctionID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMAppliedLoads_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMAppliedLoads object in the database.
    #
    # @param fields     FEMAppliedLoads object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMAppliedLoads(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AppliedLoadNmb' in fieldNames):
            cmds.append("var_AppliedLoadNmb=%s")
            args.append(fields.mAppliedLoadNmb)
        if allFields or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if allFields or ('LoadID' in fieldNames):
            cmds.append("var_LoadID=%s")
            args.append(fields.mLoadID)
        if allFields or ('LoadType' in fieldNames):
            cmds.append("var_LoadType=%s")
            args.append(fields.mLoadType)
        if allFields or ('SiteType' in fieldNames):
            cmds.append("var_SiteType=%s")
            args.append(fields.mSiteType)
        if allFields or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMAppliedLoads SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMAppliedLoads object IDs from the database.
    def getFEMAppliedLoadsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAppliedLoads'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMAppliedLoads object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMAppliedLoads object fields
    # @return \c True on success, \c False if object not found
    def getFEMAppliedLoads(self, fields):
        selectCmd = 'SELECT '\
            'var_AppliedLoadNmb,var_ArrivalTime,var_LoadID,var_LoadType,var_SiteType,var_TimeFunctionID '\
            'FROM CLASS_FEMAppliedLoads WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAppliedLoadNmb = data.pop(0)
        fields.mArrivalTime = data.pop(0)
        fields.mLoadID = data.pop(0)
        fields.mLoadType = data.pop(0)
        fields.mSiteType = data.pop(0)
        fields.mTimeFunctionID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMAppliedLoads objects matching the given
    # reference data.
    #
    # @param fields FEMAppliedLoads object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMAppliedLoads objects
    def findEqualFEMAppliedLoads(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AppliedLoadNmb' in fieldNames):
            cmds.append("var_AppliedLoadNmb=%s")
            args.append(fields.mAppliedLoadNmb)
        if (len(fieldNames)) == 0 or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if (len(fieldNames)) == 0 or ('LoadID' in fieldNames):
            cmds.append("var_LoadID=%s")
            args.append(fields.mLoadID)
        if (len(fieldNames)) == 0 or ('LoadType' in fieldNames):
            cmds.append("var_LoadType=%s")
            args.append(fields.mLoadType)
        if (len(fieldNames)) == 0 or ('SiteType' in fieldNames):
            cmds.append("var_SiteType=%s")
            args.append(fields.mSiteType)
        if (len(fieldNames)) == 0 or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAppliedLoads WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMThermoOrthData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMThermoOrthData object fields
    def addFEMThermoOrthData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMThermoOrthData '\
            '(var_ALPHAA,var_ALPHAB,var_ALPHAC,var_EA,var_EB,var_EC,var_GAB,var_GAC,var_GBC,var_MaterialID,var_NUAB,var_NUAC,var_NUBC,var_RecordNmb,var_Theta) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g,%s,%0.16g)"
        args = []
        args.append(fields.mALPHAA)
        args.append(fields.mALPHAB)
        args.append(fields.mALPHAC)
        args.append(fields.mEA)
        args.append(fields.mEB)
        args.append(fields.mEC)
        args.append(fields.mGAB)
        args.append(fields.mGAC)
        args.append(fields.mGBC)
        args.append(fields.mMaterialID)
        args.append(fields.mNUAB)
        args.append(fields.mNUAC)
        args.append(fields.mNUBC)
        args.append(fields.mRecordNmb)
        args.append(fields.mTheta)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMThermoOrthData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMThermoOrthData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMThermoOrthData object fields
    def addFEMThermoOrthDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMThermoOrthData '\
            '(var_ALPHAA,var_ALPHAB,var_ALPHAC,var_EA,var_EB,var_EC,var_GAB,var_GAC,var_GBC,var_MaterialID,var_NUAB,var_NUAC,var_NUBC,var_RecordNmb,var_Theta) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mALPHAA)
            args.append(fields.mALPHAB)
            args.append(fields.mALPHAC)
            args.append(fields.mEA)
            args.append(fields.mEB)
            args.append(fields.mEC)
            args.append(fields.mGAB)
            args.append(fields.mGAC)
            args.append(fields.mGBC)
            args.append(fields.mMaterialID)
            args.append(fields.mNUAB)
            args.append(fields.mNUAC)
            args.append(fields.mNUBC)
            args.append(fields.mRecordNmb)
            args.append(fields.mTheta)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMThermoOrthData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMThermoOrthData object in the database.
    #
    # @param fields     FEMThermoOrthData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMThermoOrthData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ALPHAA' in fieldNames):
            cmds.append("var_ALPHAA=%0.16g")
            args.append(fields.mALPHAA)
        if allFields or ('ALPHAB' in fieldNames):
            cmds.append("var_ALPHAB=%0.16g")
            args.append(fields.mALPHAB)
        if allFields or ('ALPHAC' in fieldNames):
            cmds.append("var_ALPHAC=%0.16g")
            args.append(fields.mALPHAC)
        if allFields or ('EA' in fieldNames):
            cmds.append("var_EA=%0.16g")
            args.append(fields.mEA)
        if allFields or ('EB' in fieldNames):
            cmds.append("var_EB=%0.16g")
            args.append(fields.mEB)
        if allFields or ('EC' in fieldNames):
            cmds.append("var_EC=%0.16g")
            args.append(fields.mEC)
        if allFields or ('GAB' in fieldNames):
            cmds.append("var_GAB=%0.16g")
            args.append(fields.mGAB)
        if allFields or ('GAC' in fieldNames):
            cmds.append("var_GAC=%0.16g")
            args.append(fields.mGAC)
        if allFields or ('GBC' in fieldNames):
            cmds.append("var_GBC=%0.16g")
            args.append(fields.mGBC)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('NUAB' in fieldNames):
            cmds.append("var_NUAB=%0.16g")
            args.append(fields.mNUAB)
        if allFields or ('NUAC' in fieldNames):
            cmds.append("var_NUAC=%0.16g")
            args.append(fields.mNUAC)
        if allFields or ('NUBC' in fieldNames):
            cmds.append("var_NUBC=%0.16g")
            args.append(fields.mNUBC)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Theta' in fieldNames):
            cmds.append("var_Theta=%0.16g")
            args.append(fields.mTheta)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMThermoOrthData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMThermoOrthData object IDs from the database.
    def getFEMThermoOrthDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThermoOrthData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMThermoOrthData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMThermoOrthData object fields
    # @return \c True on success, \c False if object not found
    def getFEMThermoOrthData(self, fields):
        selectCmd = 'SELECT '\
            'var_ALPHAA,var_ALPHAB,var_ALPHAC,var_EA,var_EB,var_EC,var_GAB,var_GAC,var_GBC,var_MaterialID,var_NUAB,var_NUAC,var_NUBC,var_RecordNmb,var_Theta '\
            'FROM CLASS_FEMThermoOrthData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mALPHAA = data.pop(0)
        fields.mALPHAB = data.pop(0)
        fields.mALPHAC = data.pop(0)
        fields.mEA = data.pop(0)
        fields.mEB = data.pop(0)
        fields.mEC = data.pop(0)
        fields.mGAB = data.pop(0)
        fields.mGAC = data.pop(0)
        fields.mGBC = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mNUAB = data.pop(0)
        fields.mNUAC = data.pop(0)
        fields.mNUBC = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTheta = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMThermoOrthData objects matching the given
    # reference data.
    #
    # @param fields FEMThermoOrthData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMThermoOrthData objects
    def findEqualFEMThermoOrthData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ALPHAA' in fieldNames):
            cmds.append("var_ALPHAA=%0.16g")
            args.append(fields.mALPHAA)
        if (len(fieldNames)) == 0 or ('ALPHAB' in fieldNames):
            cmds.append("var_ALPHAB=%0.16g")
            args.append(fields.mALPHAB)
        if (len(fieldNames)) == 0 or ('ALPHAC' in fieldNames):
            cmds.append("var_ALPHAC=%0.16g")
            args.append(fields.mALPHAC)
        if (len(fieldNames)) == 0 or ('EA' in fieldNames):
            cmds.append("var_EA=%0.16g")
            args.append(fields.mEA)
        if (len(fieldNames)) == 0 or ('EB' in fieldNames):
            cmds.append("var_EB=%0.16g")
            args.append(fields.mEB)
        if (len(fieldNames)) == 0 or ('EC' in fieldNames):
            cmds.append("var_EC=%0.16g")
            args.append(fields.mEC)
        if (len(fieldNames)) == 0 or ('GAB' in fieldNames):
            cmds.append("var_GAB=%0.16g")
            args.append(fields.mGAB)
        if (len(fieldNames)) == 0 or ('GAC' in fieldNames):
            cmds.append("var_GAC=%0.16g")
            args.append(fields.mGAC)
        if (len(fieldNames)) == 0 or ('GBC' in fieldNames):
            cmds.append("var_GBC=%0.16g")
            args.append(fields.mGBC)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('NUAB' in fieldNames):
            cmds.append("var_NUAB=%0.16g")
            args.append(fields.mNUAB)
        if (len(fieldNames)) == 0 or ('NUAC' in fieldNames):
            cmds.append("var_NUAC=%0.16g")
            args.append(fields.mNUAC)
        if (len(fieldNames)) == 0 or ('NUBC' in fieldNames):
            cmds.append("var_NUBC=%0.16g")
            args.append(fields.mNUBC)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Theta' in fieldNames):
            cmds.append("var_Theta=%0.16g")
            args.append(fields.mTheta)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThermoOrthData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMContactPairs object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMContactPairs object fields
    def addFEMContactPairs(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMContactPairs '\
            '(var_ContactorSurf,var_ContGroupID,var_ContPair,var_FContactor,var_Friction,var_FTarget,var_HeatTransf,var_RecordNmb,var_TargetSurf,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s,%0.16g,%0.16g)"
        args = []
        args.append(fields.mContactorSurf)
        args.append(fields.mContGroupID)
        args.append(fields.mContPair)
        args.append(fields.mFContactor)
        args.append(fields.mFriction)
        args.append(fields.mFTarget)
        args.append(fields.mHeatTransf)
        args.append(fields.mRecordNmb)
        args.append(fields.mTargetSurf)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMContactPairs_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMContactPairs objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMContactPairs object fields
    def addFEMContactPairsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMContactPairs '\
            '(var_ContactorSurf,var_ContGroupID,var_ContPair,var_FContactor,var_Friction,var_FTarget,var_HeatTransf,var_RecordNmb,var_TargetSurf,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mContactorSurf)
            args.append(fields.mContGroupID)
            args.append(fields.mContPair)
            args.append(fields.mFContactor)
            args.append(fields.mFriction)
            args.append(fields.mFTarget)
            args.append(fields.mHeatTransf)
            args.append(fields.mRecordNmb)
            args.append(fields.mTargetSurf)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMContactPairs_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMContactPairs object in the database.
    #
    # @param fields     FEMContactPairs object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMContactPairs(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ContactorSurf' in fieldNames):
            cmds.append("var_ContactorSurf=%s")
            args.append(fields.mContactorSurf)
        if allFields or ('ContGroupID' in fieldNames):
            cmds.append("var_ContGroupID=%s")
            args.append(fields.mContGroupID)
        if allFields or ('ContPair' in fieldNames):
            cmds.append("var_ContPair=%s")
            args.append(fields.mContPair)
        if allFields or ('FContactor' in fieldNames):
            cmds.append("var_FContactor=%0.16g")
            args.append(fields.mFContactor)
        if allFields or ('Friction' in fieldNames):
            cmds.append("var_Friction=%0.16g")
            args.append(fields.mFriction)
        if allFields or ('FTarget' in fieldNames):
            cmds.append("var_FTarget=%0.16g")
            args.append(fields.mFTarget)
        if allFields or ('HeatTransf' in fieldNames):
            cmds.append("var_HeatTransf=%0.16g")
            args.append(fields.mHeatTransf)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('TargetSurf' in fieldNames):
            cmds.append("var_TargetSurf=%s")
            args.append(fields.mTargetSurf)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMContactPairs SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMContactPairs object IDs from the database.
    def getFEMContactPairsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMContactPairs'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMContactPairs object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMContactPairs object fields
    # @return \c True on success, \c False if object not found
    def getFEMContactPairs(self, fields):
        selectCmd = 'SELECT '\
            'var_ContactorSurf,var_ContGroupID,var_ContPair,var_FContactor,var_Friction,var_FTarget,var_HeatTransf,var_RecordNmb,var_TargetSurf,var_TBirth,var_TDeath '\
            'FROM CLASS_FEMContactPairs WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mContactorSurf = data.pop(0)
        fields.mContGroupID = data.pop(0)
        fields.mContPair = data.pop(0)
        fields.mFContactor = data.pop(0)
        fields.mFriction = data.pop(0)
        fields.mFTarget = data.pop(0)
        fields.mHeatTransf = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTargetSurf = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMContactPairs objects matching the given
    # reference data.
    #
    # @param fields FEMContactPairs object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMContactPairs objects
    def findEqualFEMContactPairs(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ContactorSurf' in fieldNames):
            cmds.append("var_ContactorSurf=%s")
            args.append(fields.mContactorSurf)
        if (len(fieldNames)) == 0 or ('ContGroupID' in fieldNames):
            cmds.append("var_ContGroupID=%s")
            args.append(fields.mContGroupID)
        if (len(fieldNames)) == 0 or ('ContPair' in fieldNames):
            cmds.append("var_ContPair=%s")
            args.append(fields.mContPair)
        if (len(fieldNames)) == 0 or ('FContactor' in fieldNames):
            cmds.append("var_FContactor=%0.16g")
            args.append(fields.mFContactor)
        if (len(fieldNames)) == 0 or ('Friction' in fieldNames):
            cmds.append("var_Friction=%0.16g")
            args.append(fields.mFriction)
        if (len(fieldNames)) == 0 or ('FTarget' in fieldNames):
            cmds.append("var_FTarget=%0.16g")
            args.append(fields.mFTarget)
        if (len(fieldNames)) == 0 or ('HeatTransf' in fieldNames):
            cmds.append("var_HeatTransf=%0.16g")
            args.append(fields.mHeatTransf)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('TargetSurf' in fieldNames):
            cmds.append("var_TargetSurf=%s")
            args.append(fields.mTargetSurf)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMContactPairs WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMGeneral object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMGeneral object fields
    def addFEMGeneral(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMGeneral '\
            '(var_Description,var_ElementID,ref_Group,var_MatrixSet,var_NodeAmount,var_RecordNmb,var_Save) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mMatrixSet)
        args.append(fields.mNodeAmount)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMGeneral_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMGeneral objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMGeneral object fields
    def addFEMGeneralList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMGeneral '\
            '(var_Description,var_ElementID,ref_Group,var_MatrixSet,var_NodeAmount,var_RecordNmb,var_Save) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mMatrixSet)
            args.append(fields.mNodeAmount)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMGeneral_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMGeneral object in the database.
    #
    # @param fields     FEMGeneral object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMGeneral(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('MatrixSet' in fieldNames):
            cmds.append("var_MatrixSet=%s")
            args.append(fields.mMatrixSet)
        if allFields or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMGeneral SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMGeneral object IDs from the database.
    def getFEMGeneralIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGeneral'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMGeneral object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMGeneral object fields
    # @return \c True on success, \c False if object not found
    def getFEMGeneral(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_ElementID,ref_Group,var_MatrixSet,var_NodeAmount,var_RecordNmb,var_Save '\
            'FROM CLASS_FEMGeneral WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMatrixSet = data.pop(0)
        fields.mNodeAmount = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMGeneral objects matching the given
    # reference data.
    #
    # @param fields FEMGeneral object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMGeneral objects
    def findEqualFEMGeneral(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('MatrixSet' in fieldNames):
            cmds.append("var_MatrixSet=%s")
            args.append(fields.mMatrixSet)
        if (len(fieldNames)) == 0 or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGeneral WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMBeamNode object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMBeamNode object fields
    def addFEMBeamNode(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMBeamNode '\
            '(var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_MC,var_MCrigidity,var_REmultiplyer,var_Result,var_REtype,var_RINT,var_SectionID,var_SINT,var_TINT) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%0.16g,%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDisplacement)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mIniStrain)
        args.append(fields.mMaterialID)
        args.append(fields.mMC)
        args.append(fields.mMCrigidity)
        args.append(fields.mREmultiplyer)
        args.append(fields.mResult)
        args.append(fields.mREtype)
        args.append(fields.mRINT)
        args.append(fields.mSectionID)
        args.append(fields.mSINT)
        args.append(fields.mTINT)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMBeamNode_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMBeamNode objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMBeamNode object fields
    def addFEMBeamNodeList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMBeamNode '\
            '(var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_MC,var_MCrigidity,var_REmultiplyer,var_Result,var_REtype,var_RINT,var_SectionID,var_SINT,var_TINT) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%0.16g,%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDisplacement)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mIniStrain)
            args.append(fields.mMaterialID)
            args.append(fields.mMC)
            args.append(fields.mMCrigidity)
            args.append(fields.mREmultiplyer)
            args.append(fields.mResult)
            args.append(fields.mREtype)
            args.append(fields.mRINT)
            args.append(fields.mSectionID)
            args.append(fields.mSINT)
            args.append(fields.mTINT)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMBeamNode_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMBeamNode object in the database.
    #
    # @param fields     FEMBeamNode object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMBeamNode(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('IniStrain' in fieldNames):
            cmds.append("var_IniStrain=%s")
            args.append(fields.mIniStrain)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('MC' in fieldNames):
            cmds.append("var_MC=%s")
            args.append(fields.mMC)
        if allFields or ('MCrigidity' in fieldNames):
            cmds.append("var_MCrigidity=%s")
            args.append(fields.mMCrigidity)
        if allFields or ('REmultiplyer' in fieldNames):
            cmds.append("var_REmultiplyer=%0.16g")
            args.append(fields.mREmultiplyer)
        if allFields or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if allFields or ('REtype' in fieldNames):
            cmds.append("var_REtype=%s")
            args.append(fields.mREtype)
        if allFields or ('RINT' in fieldNames):
            cmds.append("var_RINT=%s")
            args.append(fields.mRINT)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('SINT' in fieldNames):
            cmds.append("var_SINT=%s")
            args.append(fields.mSINT)
        if allFields or ('TINT' in fieldNames):
            cmds.append("var_TINT=%s")
            args.append(fields.mTINT)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMBeamNode SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMBeamNode object IDs from the database.
    def getFEMBeamNodeIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMBeamNode'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMBeamNode object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMBeamNode object fields
    # @return \c True on success, \c False if object not found
    def getFEMBeamNode(self, fields):
        selectCmd = 'SELECT '\
            'var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_MC,var_MCrigidity,var_REmultiplyer,var_Result,var_REtype,var_RINT,var_SectionID,var_SINT,var_TINT '\
            'FROM CLASS_FEMBeamNode WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDisplacement = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mIniStrain = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mMC = data.pop(0)
        fields.mMCrigidity = data.pop(0)
        fields.mREmultiplyer = data.pop(0)
        fields.mResult = data.pop(0)
        fields.mREtype = data.pop(0)
        fields.mRINT = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mSINT = data.pop(0)
        fields.mTINT = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMBeamNode objects matching the given
    # reference data.
    #
    # @param fields FEMBeamNode object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMBeamNode objects
    def findEqualFEMBeamNode(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('IniStrain' in fieldNames):
            cmds.append("var_IniStrain=%s")
            args.append(fields.mIniStrain)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('MC' in fieldNames):
            cmds.append("var_MC=%s")
            args.append(fields.mMC)
        if (len(fieldNames)) == 0 or ('MCrigidity' in fieldNames):
            cmds.append("var_MCrigidity=%s")
            args.append(fields.mMCrigidity)
        if (len(fieldNames)) == 0 or ('REmultiplyer' in fieldNames):
            cmds.append("var_REmultiplyer=%0.16g")
            args.append(fields.mREmultiplyer)
        if (len(fieldNames)) == 0 or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if (len(fieldNames)) == 0 or ('REtype' in fieldNames):
            cmds.append("var_REtype=%s")
            args.append(fields.mREtype)
        if (len(fieldNames)) == 0 or ('RINT' in fieldNames):
            cmds.append("var_RINT=%s")
            args.append(fields.mRINT)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('SINT' in fieldNames):
            cmds.append("var_SINT=%s")
            args.append(fields.mSINT)
        if (len(fieldNames)) == 0 or ('TINT' in fieldNames):
            cmds.append("var_TINT=%s")
            args.append(fields.mTINT)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMBeamNode WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSectionRect object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSectionRect object fields
    def addFEMSectionRect(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSectionRect '\
            '(var_Height,var_IShear,var_SC,var_SectionID,var_SSarea,var_TC,var_Torfac,var_TSarea,var_Width) VALUES '\
            "(%0.16g,%s,%0.16g,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mHeight)
        args.append(fields.mIShear)
        args.append(fields.mSC)
        args.append(fields.mSectionID)
        args.append(fields.mSSarea)
        args.append(fields.mTC)
        args.append(fields.mTorfac)
        args.append(fields.mTSarea)
        args.append(fields.mWidth)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSectionRect_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSectionRect objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSectionRect object fields
    def addFEMSectionRectList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSectionRect '\
            '(var_Height,var_IShear,var_SC,var_SectionID,var_SSarea,var_TC,var_Torfac,var_TSarea,var_Width) VALUES '\
            "(%0.16g,%s,%0.16g,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mHeight)
            args.append(fields.mIShear)
            args.append(fields.mSC)
            args.append(fields.mSectionID)
            args.append(fields.mSSarea)
            args.append(fields.mTC)
            args.append(fields.mTorfac)
            args.append(fields.mTSarea)
            args.append(fields.mWidth)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSectionRect_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSectionRect object in the database.
    #
    # @param fields     FEMSectionRect object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSectionRect(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Height' in fieldNames):
            cmds.append("var_Height=%0.16g")
            args.append(fields.mHeight)
        if allFields or ('IShear' in fieldNames):
            cmds.append("var_IShear=%s")
            args.append(fields.mIShear)
        if allFields or ('SC' in fieldNames):
            cmds.append("var_SC=%0.16g")
            args.append(fields.mSC)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('SSarea' in fieldNames):
            cmds.append("var_SSarea=%0.16g")
            args.append(fields.mSSarea)
        if allFields or ('TC' in fieldNames):
            cmds.append("var_TC=%0.16g")
            args.append(fields.mTC)
        if allFields or ('Torfac' in fieldNames):
            cmds.append("var_Torfac=%0.16g")
            args.append(fields.mTorfac)
        if allFields or ('TSarea' in fieldNames):
            cmds.append("var_TSarea=%0.16g")
            args.append(fields.mTSarea)
        if allFields or ('Width' in fieldNames):
            cmds.append("var_Width=%0.16g")
            args.append(fields.mWidth)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSectionRect SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSectionRect object IDs from the database.
    def getFEMSectionRectIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionRect'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSectionRect object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSectionRect object fields
    # @return \c True on success, \c False if object not found
    def getFEMSectionRect(self, fields):
        selectCmd = 'SELECT '\
            'var_Height,var_IShear,var_SC,var_SectionID,var_SSarea,var_TC,var_Torfac,var_TSarea,var_Width '\
            'FROM CLASS_FEMSectionRect WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mHeight = data.pop(0)
        fields.mIShear = data.pop(0)
        fields.mSC = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mSSarea = data.pop(0)
        fields.mTC = data.pop(0)
        fields.mTorfac = data.pop(0)
        fields.mTSarea = data.pop(0)
        fields.mWidth = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSectionRect objects matching the given
    # reference data.
    #
    # @param fields FEMSectionRect object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSectionRect objects
    def findEqualFEMSectionRect(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Height' in fieldNames):
            cmds.append("var_Height=%0.16g")
            args.append(fields.mHeight)
        if (len(fieldNames)) == 0 or ('IShear' in fieldNames):
            cmds.append("var_IShear=%s")
            args.append(fields.mIShear)
        if (len(fieldNames)) == 0 or ('SC' in fieldNames):
            cmds.append("var_SC=%0.16g")
            args.append(fields.mSC)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('SSarea' in fieldNames):
            cmds.append("var_SSarea=%0.16g")
            args.append(fields.mSSarea)
        if (len(fieldNames)) == 0 or ('TC' in fieldNames):
            cmds.append("var_TC=%0.16g")
            args.append(fields.mTC)
        if (len(fieldNames)) == 0 or ('Torfac' in fieldNames):
            cmds.append("var_Torfac=%0.16g")
            args.append(fields.mTorfac)
        if (len(fieldNames)) == 0 or ('TSarea' in fieldNames):
            cmds.append("var_TSarea=%0.16g")
            args.append(fields.mTSarea)
        if (len(fieldNames)) == 0 or ('Width' in fieldNames):
            cmds.append("var_Width=%0.16g")
            args.append(fields.mWidth)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionRect WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMBeamLoad object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMBeamLoad object fields
    def addFEMBeamLoad(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMBeamLoad '\
            '(var_ArrivalTime,var_DeformDepend,var_DirectFilter,var_ElementID,var_Face,ref_Group,var_P1,var_P2,var_RecordNmb,var_TimeFunctionID) VALUES '\
            "(%0.16g,%s,%s,%s,%s,%s,%0.16g,%0.16g,%s,%s)"
        args = []
        args.append(fields.mArrivalTime)
        args.append(fields.mDeformDepend)
        args.append(fields.mDirectFilter)
        args.append(fields.mElementID)
        args.append(fields.mFace)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mP1)
        args.append(fields.mP2)
        args.append(fields.mRecordNmb)
        args.append(fields.mTimeFunctionID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMBeamLoad_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMBeamLoad objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMBeamLoad object fields
    def addFEMBeamLoadList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMBeamLoad '\
            '(var_ArrivalTime,var_DeformDepend,var_DirectFilter,var_ElementID,var_Face,ref_Group,var_P1,var_P2,var_RecordNmb,var_TimeFunctionID) VALUES '\
            "(%0.16g,%s,%s,%s,%s,%s,%0.16g,%0.16g,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mArrivalTime)
            args.append(fields.mDeformDepend)
            args.append(fields.mDirectFilter)
            args.append(fields.mElementID)
            args.append(fields.mFace)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mP1)
            args.append(fields.mP2)
            args.append(fields.mRecordNmb)
            args.append(fields.mTimeFunctionID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMBeamLoad_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMBeamLoad object in the database.
    #
    # @param fields     FEMBeamLoad object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMBeamLoad(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if allFields or ('DeformDepend' in fieldNames):
            cmds.append("var_DeformDepend=%s")
            args.append(fields.mDeformDepend)
        if allFields or ('DirectFilter' in fieldNames):
            cmds.append("var_DirectFilter=%s")
            args.append(fields.mDirectFilter)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Face' in fieldNames):
            cmds.append("var_Face=%s")
            args.append(fields.mFace)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('P1' in fieldNames):
            cmds.append("var_P1=%0.16g")
            args.append(fields.mP1)
        if allFields or ('P2' in fieldNames):
            cmds.append("var_P2=%0.16g")
            args.append(fields.mP2)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMBeamLoad SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMBeamLoad object IDs from the database.
    def getFEMBeamLoadIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMBeamLoad'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMBeamLoad object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMBeamLoad object fields
    # @return \c True on success, \c False if object not found
    def getFEMBeamLoad(self, fields):
        selectCmd = 'SELECT '\
            'var_ArrivalTime,var_DeformDepend,var_DirectFilter,var_ElementID,var_Face,ref_Group,var_P1,var_P2,var_RecordNmb,var_TimeFunctionID '\
            'FROM CLASS_FEMBeamLoad WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mArrivalTime = data.pop(0)
        fields.mDeformDepend = data.pop(0)
        fields.mDirectFilter = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mFace = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mP1 = data.pop(0)
        fields.mP2 = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTimeFunctionID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMBeamLoad objects matching the given
    # reference data.
    #
    # @param fields FEMBeamLoad object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMBeamLoad objects
    def findEqualFEMBeamLoad(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if (len(fieldNames)) == 0 or ('DeformDepend' in fieldNames):
            cmds.append("var_DeformDepend=%s")
            args.append(fields.mDeformDepend)
        if (len(fieldNames)) == 0 or ('DirectFilter' in fieldNames):
            cmds.append("var_DirectFilter=%s")
            args.append(fields.mDirectFilter)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Face' in fieldNames):
            cmds.append("var_Face=%s")
            args.append(fields.mFace)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('P1' in fieldNames):
            cmds.append("var_P1=%0.16g")
            args.append(fields.mP1)
        if (len(fieldNames)) == 0 or ('P2' in fieldNames):
            cmds.append("var_P2=%0.16g")
            args.append(fields.mP2)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMBeamLoad WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMLoadMassProportional object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMLoadMassProportional object fields
    def addFEMLoadMassProportional(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMLoadMassProportional '\
            '(var_AX,var_AY,var_AZ,var_LoadID,var_Magnitude,ref_Structure) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%0.16g,%s)"
        args = []
        args.append(fields.mAX)
        args.append(fields.mAY)
        args.append(fields.mAZ)
        args.append(fields.mLoadID)
        args.append(fields.mMagnitude)
        if fields.mStructure == 0:
            args.append(None)
        else:
            args.append(fields.mStructure)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMLoadMassProportional_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMLoadMassProportional objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMLoadMassProportional object fields
    def addFEMLoadMassProportionalList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMLoadMassProportional '\
            '(var_AX,var_AY,var_AZ,var_LoadID,var_Magnitude,ref_Structure) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAX)
            args.append(fields.mAY)
            args.append(fields.mAZ)
            args.append(fields.mLoadID)
            args.append(fields.mMagnitude)
            if fields.mStructure == 0:
                args.append(None)
            else:
                args.append(fields.mStructure)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMLoadMassProportional_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMLoadMassProportional object in the database.
    #
    # @param fields     FEMLoadMassProportional object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMLoadMassProportional(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AX' in fieldNames):
            cmds.append("var_AX=%0.16g")
            args.append(fields.mAX)
        if allFields or ('AY' in fieldNames):
            cmds.append("var_AY=%0.16g")
            args.append(fields.mAY)
        if allFields or ('AZ' in fieldNames):
            cmds.append("var_AZ=%0.16g")
            args.append(fields.mAZ)
        if allFields or ('LoadID' in fieldNames):
            cmds.append("var_LoadID=%s")
            args.append(fields.mLoadID)
        if allFields or ('Magnitude' in fieldNames):
            cmds.append("var_Magnitude=%0.16g")
            args.append(fields.mMagnitude)
        if allFields or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMLoadMassProportional SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMLoadMassProportional object IDs from the database.
    def getFEMLoadMassProportionalIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMLoadMassProportional'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMLoadMassProportional object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMLoadMassProportional object fields
    # @return \c True on success, \c False if object not found
    def getFEMLoadMassProportional(self, fields):
        selectCmd = 'SELECT '\
            'var_AX,var_AY,var_AZ,var_LoadID,var_Magnitude,ref_Structure '\
            'FROM CLASS_FEMLoadMassProportional WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAX = data.pop(0)
        fields.mAY = data.pop(0)
        fields.mAZ = data.pop(0)
        fields.mLoadID = data.pop(0)
        fields.mMagnitude = data.pop(0)
        fields.mStructure = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all FEMLoadMassProportional objects matching the given
    # reference data.
    #
    # @param fields FEMLoadMassProportional object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMLoadMassProportional objects
    def findEqualFEMLoadMassProportional(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AX' in fieldNames):
            cmds.append("var_AX=%0.16g")
            args.append(fields.mAX)
        if (len(fieldNames)) == 0 or ('AY' in fieldNames):
            cmds.append("var_AY=%0.16g")
            args.append(fields.mAY)
        if (len(fieldNames)) == 0 or ('AZ' in fieldNames):
            cmds.append("var_AZ=%0.16g")
            args.append(fields.mAZ)
        if (len(fieldNames)) == 0 or ('LoadID' in fieldNames):
            cmds.append("var_LoadID=%s")
            args.append(fields.mLoadID)
        if (len(fieldNames)) == 0 or ('Magnitude' in fieldNames):
            cmds.append("var_Magnitude=%0.16g")
            args.append(fields.mMagnitude)
        if (len(fieldNames)) == 0 or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMLoadMassProportional WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMLink object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMLink object fields
    def addFEMLink(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMLink '\
            '(var_Description,var_Displacement,ref_MasterNode,var_RLID,ref_SlaveNode) VALUES '\
            "(%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mDisplacement)
        if fields.mMasterNode == 0:
            args.append(None)
        else:
            args.append(fields.mMasterNode)
        args.append(fields.mRLID)
        if fields.mSlaveNode == 0:
            args.append(None)
        else:
            args.append(fields.mSlaveNode)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMLink_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMLink objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMLink object fields
    def addFEMLinkList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMLink '\
            '(var_Description,var_Displacement,ref_MasterNode,var_RLID,ref_SlaveNode) VALUES '\
            "(%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mDisplacement)
            if fields.mMasterNode == 0:
                args.append(None)
            else:
                args.append(fields.mMasterNode)
            args.append(fields.mRLID)
            if fields.mSlaveNode == 0:
                args.append(None)
            else:
                args.append(fields.mSlaveNode)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMLink_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMLink object in the database.
    #
    # @param fields     FEMLink object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMLink(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if allFields or ('MasterNode' in fieldNames):
            cmds.append("ref_MasterNode=%s")
            args.append(fields.mMasterNode)
        if allFields or ('RLID' in fieldNames):
            cmds.append("var_RLID=%s")
            args.append(fields.mRLID)
        if allFields or ('SlaveNode' in fieldNames):
            cmds.append("ref_SlaveNode=%s")
            args.append(fields.mSlaveNode)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMLink SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMLink object IDs from the database.
    def getFEMLinkIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMLink'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMLink object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMLink object fields
    # @return \c True on success, \c False if object not found
    def getFEMLink(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_Displacement,ref_MasterNode,var_RLID,ref_SlaveNode '\
            'FROM CLASS_FEMLink WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mDisplacement = data.pop(0)
        fields.mMasterNode = zeroNone(data.pop(0))
        fields.mRLID = data.pop(0)
        fields.mSlaveNode = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all FEMLink objects matching the given
    # reference data.
    #
    # @param fields FEMLink object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMLink objects
    def findEqualFEMLink(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('MasterNode' in fieldNames):
            cmds.append("ref_MasterNode=%s")
            args.append(fields.mMasterNode)
        if (len(fieldNames)) == 0 or ('RLID' in fieldNames):
            cmds.append("var_RLID=%s")
            args.append(fields.mRLID)
        if (len(fieldNames)) == 0 or ('SlaveNode' in fieldNames):
            cmds.append("ref_SlaveNode=%s")
            args.append(fields.mSlaveNode)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMLink WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMAxesNode object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMAxesNode object fields
    def addFEMAxesNode(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMAxesNode '\
            '(var_AxNodeID,ref_Group,ref_Node1,ref_Node2,ref_Node3,var_RecordNmb) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mAxNodeID)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        if fields.mNode1 == 0:
            args.append(None)
        else:
            args.append(fields.mNode1)
        if fields.mNode2 == 0:
            args.append(None)
        else:
            args.append(fields.mNode2)
        if fields.mNode3 == 0:
            args.append(None)
        else:
            args.append(fields.mNode3)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMAxesNode_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMAxesNode objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMAxesNode object fields
    def addFEMAxesNodeList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMAxesNode '\
            '(var_AxNodeID,ref_Group,ref_Node1,ref_Node2,ref_Node3,var_RecordNmb) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAxNodeID)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            if fields.mNode1 == 0:
                args.append(None)
            else:
                args.append(fields.mNode1)
            if fields.mNode2 == 0:
                args.append(None)
            else:
                args.append(fields.mNode2)
            if fields.mNode3 == 0:
                args.append(None)
            else:
                args.append(fields.mNode3)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMAxesNode_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMAxesNode object in the database.
    #
    # @param fields     FEMAxesNode object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMAxesNode(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AxNodeID' in fieldNames):
            cmds.append("var_AxNodeID=%s")
            args.append(fields.mAxNodeID)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('Node1' in fieldNames):
            cmds.append("ref_Node1=%s")
            args.append(fields.mNode1)
        if allFields or ('Node2' in fieldNames):
            cmds.append("ref_Node2=%s")
            args.append(fields.mNode2)
        if allFields or ('Node3' in fieldNames):
            cmds.append("ref_Node3=%s")
            args.append(fields.mNode3)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMAxesNode SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMAxesNode object IDs from the database.
    def getFEMAxesNodeIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAxesNode'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMAxesNode object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMAxesNode object fields
    # @return \c True on success, \c False if object not found
    def getFEMAxesNode(self, fields):
        selectCmd = 'SELECT '\
            'var_AxNodeID,ref_Group,ref_Node1,ref_Node2,ref_Node3,var_RecordNmb '\
            'FROM CLASS_FEMAxesNode WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAxNodeID = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mNode1 = zeroNone(data.pop(0))
        fields.mNode2 = zeroNone(data.pop(0))
        fields.mNode3 = zeroNone(data.pop(0))
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMAxesNode objects matching the given
    # reference data.
    #
    # @param fields FEMAxesNode object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMAxesNode objects
    def findEqualFEMAxesNode(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AxNodeID' in fieldNames):
            cmds.append("var_AxNodeID=%s")
            args.append(fields.mAxNodeID)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('Node1' in fieldNames):
            cmds.append("ref_Node1=%s")
            args.append(fields.mNode1)
        if (len(fieldNames)) == 0 or ('Node2' in fieldNames):
            cmds.append("ref_Node2=%s")
            args.append(fields.mNode2)
        if (len(fieldNames)) == 0 or ('Node3' in fieldNames):
            cmds.append("ref_Node3=%s")
            args.append(fields.mNode3)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAxesNode WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMNMTimeMass object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMNMTimeMass object fields
    def addFEMNMTimeMass(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMNMTimeMass '\
            '(var_Mass,var_PropertyID,var_RecordNmb,var_TimeValue) VALUES '\
            "(%0.16g,%s,%s,%0.16g)"
        args = []
        args.append(fields.mMass)
        args.append(fields.mPropertyID)
        args.append(fields.mRecordNmb)
        args.append(fields.mTimeValue)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMNMTimeMass_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMNMTimeMass objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMNMTimeMass object fields
    def addFEMNMTimeMassList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMNMTimeMass '\
            '(var_Mass,var_PropertyID,var_RecordNmb,var_TimeValue) VALUES '\
            "(%0.16g,%s,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mMass)
            args.append(fields.mPropertyID)
            args.append(fields.mRecordNmb)
            args.append(fields.mTimeValue)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMNMTimeMass_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMNMTimeMass object in the database.
    #
    # @param fields     FEMNMTimeMass object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMNMTimeMass(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Mass' in fieldNames):
            cmds.append("var_Mass=%0.16g")
            args.append(fields.mMass)
        if allFields or ('PropertyID' in fieldNames):
            cmds.append("var_PropertyID=%s")
            args.append(fields.mPropertyID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('TimeValue' in fieldNames):
            cmds.append("var_TimeValue=%0.16g")
            args.append(fields.mTimeValue)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMNMTimeMass SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMNMTimeMass object IDs from the database.
    def getFEMNMTimeMassIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNMTimeMass'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMNMTimeMass object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMNMTimeMass object fields
    # @return \c True on success, \c False if object not found
    def getFEMNMTimeMass(self, fields):
        selectCmd = 'SELECT '\
            'var_Mass,var_PropertyID,var_RecordNmb,var_TimeValue '\
            'FROM CLASS_FEMNMTimeMass WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mMass = data.pop(0)
        fields.mPropertyID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTimeValue = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMNMTimeMass objects matching the given
    # reference data.
    #
    # @param fields FEMNMTimeMass object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMNMTimeMass objects
    def findEqualFEMNMTimeMass(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Mass' in fieldNames):
            cmds.append("var_Mass=%0.16g")
            args.append(fields.mMass)
        if (len(fieldNames)) == 0 or ('PropertyID' in fieldNames):
            cmds.append("var_PropertyID=%s")
            args.append(fields.mPropertyID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('TimeValue' in fieldNames):
            cmds.append("var_TimeValue=%0.16g")
            args.append(fields.mTimeValue)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNMTimeMass WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMAppliedDisplacement object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMAppliedDisplacement object fields
    def addFEMAppliedDisplacement(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMAppliedDisplacement '\
            '(var_ArrivalTime,var_Description,var_Direction,var_Factor,ref_Node,var_RecordNmb,var_TimeFunctionID) VALUES '\
            "(%0.16g,%s,%s,%0.16g,%s,%s,%s)"
        args = []
        args.append(fields.mArrivalTime)
        args.append(fields.mDescription)
        args.append(fields.mDirection)
        args.append(fields.mFactor)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(fields.mRecordNmb)
        args.append(fields.mTimeFunctionID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMAppliedDisplacement_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMAppliedDisplacement objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMAppliedDisplacement object fields
    def addFEMAppliedDisplacementList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMAppliedDisplacement '\
            '(var_ArrivalTime,var_Description,var_Direction,var_Factor,ref_Node,var_RecordNmb,var_TimeFunctionID) VALUES '\
            "(%0.16g,%s,%s,%0.16g,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mArrivalTime)
            args.append(fields.mDescription)
            args.append(fields.mDirection)
            args.append(fields.mFactor)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(fields.mRecordNmb)
            args.append(fields.mTimeFunctionID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMAppliedDisplacement_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMAppliedDisplacement object in the database.
    #
    # @param fields     FEMAppliedDisplacement object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMAppliedDisplacement(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Direction' in fieldNames):
            cmds.append("var_Direction=%s")
            args.append(fields.mDirection)
        if allFields or ('Factor' in fieldNames):
            cmds.append("var_Factor=%0.16g")
            args.append(fields.mFactor)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMAppliedDisplacement SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMAppliedDisplacement object IDs from the database.
    def getFEMAppliedDisplacementIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAppliedDisplacement'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMAppliedDisplacement object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMAppliedDisplacement object fields
    # @return \c True on success, \c False if object not found
    def getFEMAppliedDisplacement(self, fields):
        selectCmd = 'SELECT '\
            'var_ArrivalTime,var_Description,var_Direction,var_Factor,ref_Node,var_RecordNmb,var_TimeFunctionID '\
            'FROM CLASS_FEMAppliedDisplacement WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mArrivalTime = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mDirection = data.pop(0)
        fields.mFactor = data.pop(0)
        fields.mNode = zeroNone(data.pop(0))
        fields.mRecordNmb = data.pop(0)
        fields.mTimeFunctionID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMAppliedDisplacement objects matching the given
    # reference data.
    #
    # @param fields FEMAppliedDisplacement object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMAppliedDisplacement objects
    def findEqualFEMAppliedDisplacement(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Direction' in fieldNames):
            cmds.append("var_Direction=%s")
            args.append(fields.mDirection)
        if (len(fieldNames)) == 0 or ('Factor' in fieldNames):
            cmds.append("var_Factor=%0.16g")
            args.append(fields.mFactor)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAppliedDisplacement WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMTimeFunctions object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMTimeFunctions object fields
    def addFEMTimeFunctions(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMTimeFunctions '\
            '(var_Description,var_TimeFunctionID) VALUES '\
            "(%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mTimeFunctionID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMTimeFunctions_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMTimeFunctions objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMTimeFunctions object fields
    def addFEMTimeFunctionsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMTimeFunctions '\
            '(var_Description,var_TimeFunctionID) VALUES '\
            "(%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mTimeFunctionID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMTimeFunctions_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMTimeFunctions object in the database.
    #
    # @param fields     FEMTimeFunctions object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMTimeFunctions(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMTimeFunctions SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMTimeFunctions object IDs from the database.
    def getFEMTimeFunctionsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTimeFunctions'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMTimeFunctions object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMTimeFunctions object fields
    # @return \c True on success, \c False if object not found
    def getFEMTimeFunctions(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_TimeFunctionID '\
            'FROM CLASS_FEMTimeFunctions WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mTimeFunctionID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMTimeFunctions objects matching the given
    # reference data.
    #
    # @param fields FEMTimeFunctions object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMTimeFunctions objects
    def findEqualFEMTimeFunctions(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTimeFunctions WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMForceStrainData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMForceStrainData object fields
    def addFEMForceStrainData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMForceStrainData '\
            '(var_Force,var_ForceAxID,var_RecordNmb,var_Strain) VALUES '\
            "(%0.16g,%s,%s,%0.16g)"
        args = []
        args.append(fields.mForce)
        args.append(fields.mForceAxID)
        args.append(fields.mRecordNmb)
        args.append(fields.mStrain)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMForceStrainData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMForceStrainData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMForceStrainData object fields
    def addFEMForceStrainDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMForceStrainData '\
            '(var_Force,var_ForceAxID,var_RecordNmb,var_Strain) VALUES '\
            "(%0.16g,%s,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mForce)
            args.append(fields.mForceAxID)
            args.append(fields.mRecordNmb)
            args.append(fields.mStrain)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMForceStrainData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMForceStrainData object in the database.
    #
    # @param fields     FEMForceStrainData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMForceStrainData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Force' in fieldNames):
            cmds.append("var_Force=%0.16g")
            args.append(fields.mForce)
        if allFields or ('ForceAxID' in fieldNames):
            cmds.append("var_ForceAxID=%s")
            args.append(fields.mForceAxID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Strain' in fieldNames):
            cmds.append("var_Strain=%0.16g")
            args.append(fields.mStrain)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMForceStrainData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMForceStrainData object IDs from the database.
    def getFEMForceStrainDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMForceStrainData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMForceStrainData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMForceStrainData object fields
    # @return \c True on success, \c False if object not found
    def getFEMForceStrainData(self, fields):
        selectCmd = 'SELECT '\
            'var_Force,var_ForceAxID,var_RecordNmb,var_Strain '\
            'FROM CLASS_FEMForceStrainData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mForce = data.pop(0)
        fields.mForceAxID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mStrain = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMForceStrainData objects matching the given
    # reference data.
    #
    # @param fields FEMForceStrainData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMForceStrainData objects
    def findEqualFEMForceStrainData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Force' in fieldNames):
            cmds.append("var_Force=%0.16g")
            args.append(fields.mForce)
        if (len(fieldNames)) == 0 or ('ForceAxID' in fieldNames):
            cmds.append("var_ForceAxID=%s")
            args.append(fields.mForceAxID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Strain' in fieldNames):
            cmds.append("var_Strain=%0.16g")
            args.append(fields.mStrain)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMForceStrainData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSkewDOF object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSkewDOF object fields
    def addFEMSkewDOF(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSkewDOF '\
            '(var_Description,ref_Node,var_SkewSystemID) VALUES '\
            "(%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(fields.mSkewSystemID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSkewDOF_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSkewDOF objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSkewDOF object fields
    def addFEMSkewDOFList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSkewDOF '\
            '(var_Description,ref_Node,var_SkewSystemID) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(fields.mSkewSystemID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSkewDOF_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSkewDOF object in the database.
    #
    # @param fields     FEMSkewDOF object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSkewDOF(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('SkewSystemID' in fieldNames):
            cmds.append("var_SkewSystemID=%s")
            args.append(fields.mSkewSystemID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSkewDOF SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSkewDOF object IDs from the database.
    def getFEMSkewDOFIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSkewDOF'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSkewDOF object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSkewDOF object fields
    # @return \c True on success, \c False if object not found
    def getFEMSkewDOF(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,ref_Node,var_SkewSystemID '\
            'FROM CLASS_FEMSkewDOF WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mNode = zeroNone(data.pop(0))
        fields.mSkewSystemID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSkewDOF objects matching the given
    # reference data.
    #
    # @param fields FEMSkewDOF object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSkewDOF objects
    def findEqualFEMSkewDOF(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('SkewSystemID' in fieldNames):
            cmds.append("var_SkewSystemID=%s")
            args.append(fields.mSkewSystemID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSkewDOF WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSectionI object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSectionI object fields
    def addFEMSectionI(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSectionI '\
            '(var_Height,var_SC,var_SectionID,var_SSarea,var_TC,var_Thick1,var_Thick2,var_Thick3,var_Torfac,var_TSarea,var_Width1,var_Width2) VALUES '\
            "(%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mHeight)
        args.append(fields.mSC)
        args.append(fields.mSectionID)
        args.append(fields.mSSarea)
        args.append(fields.mTC)
        args.append(fields.mThick1)
        args.append(fields.mThick2)
        args.append(fields.mThick3)
        args.append(fields.mTorfac)
        args.append(fields.mTSarea)
        args.append(fields.mWidth1)
        args.append(fields.mWidth2)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSectionI_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSectionI objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSectionI object fields
    def addFEMSectionIList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSectionI '\
            '(var_Height,var_SC,var_SectionID,var_SSarea,var_TC,var_Thick1,var_Thick2,var_Thick3,var_Torfac,var_TSarea,var_Width1,var_Width2) VALUES '\
            "(%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mHeight)
            args.append(fields.mSC)
            args.append(fields.mSectionID)
            args.append(fields.mSSarea)
            args.append(fields.mTC)
            args.append(fields.mThick1)
            args.append(fields.mThick2)
            args.append(fields.mThick3)
            args.append(fields.mTorfac)
            args.append(fields.mTSarea)
            args.append(fields.mWidth1)
            args.append(fields.mWidth2)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSectionI_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSectionI object in the database.
    #
    # @param fields     FEMSectionI object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSectionI(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Height' in fieldNames):
            cmds.append("var_Height=%0.16g")
            args.append(fields.mHeight)
        if allFields or ('SC' in fieldNames):
            cmds.append("var_SC=%0.16g")
            args.append(fields.mSC)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('SSarea' in fieldNames):
            cmds.append("var_SSarea=%0.16g")
            args.append(fields.mSSarea)
        if allFields or ('TC' in fieldNames):
            cmds.append("var_TC=%0.16g")
            args.append(fields.mTC)
        if allFields or ('Thick1' in fieldNames):
            cmds.append("var_Thick1=%0.16g")
            args.append(fields.mThick1)
        if allFields or ('Thick2' in fieldNames):
            cmds.append("var_Thick2=%0.16g")
            args.append(fields.mThick2)
        if allFields or ('Thick3' in fieldNames):
            cmds.append("var_Thick3=%0.16g")
            args.append(fields.mThick3)
        if allFields or ('Torfac' in fieldNames):
            cmds.append("var_Torfac=%0.16g")
            args.append(fields.mTorfac)
        if allFields or ('TSarea' in fieldNames):
            cmds.append("var_TSarea=%0.16g")
            args.append(fields.mTSarea)
        if allFields or ('Width1' in fieldNames):
            cmds.append("var_Width1=%0.16g")
            args.append(fields.mWidth1)
        if allFields or ('Width2' in fieldNames):
            cmds.append("var_Width2=%0.16g")
            args.append(fields.mWidth2)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSectionI SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSectionI object IDs from the database.
    def getFEMSectionIIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionI'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSectionI object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSectionI object fields
    # @return \c True on success, \c False if object not found
    def getFEMSectionI(self, fields):
        selectCmd = 'SELECT '\
            'var_Height,var_SC,var_SectionID,var_SSarea,var_TC,var_Thick1,var_Thick2,var_Thick3,var_Torfac,var_TSarea,var_Width1,var_Width2 '\
            'FROM CLASS_FEMSectionI WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mHeight = data.pop(0)
        fields.mSC = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mSSarea = data.pop(0)
        fields.mTC = data.pop(0)
        fields.mThick1 = data.pop(0)
        fields.mThick2 = data.pop(0)
        fields.mThick3 = data.pop(0)
        fields.mTorfac = data.pop(0)
        fields.mTSarea = data.pop(0)
        fields.mWidth1 = data.pop(0)
        fields.mWidth2 = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSectionI objects matching the given
    # reference data.
    #
    # @param fields FEMSectionI object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSectionI objects
    def findEqualFEMSectionI(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Height' in fieldNames):
            cmds.append("var_Height=%0.16g")
            args.append(fields.mHeight)
        if (len(fieldNames)) == 0 or ('SC' in fieldNames):
            cmds.append("var_SC=%0.16g")
            args.append(fields.mSC)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('SSarea' in fieldNames):
            cmds.append("var_SSarea=%0.16g")
            args.append(fields.mSSarea)
        if (len(fieldNames)) == 0 or ('TC' in fieldNames):
            cmds.append("var_TC=%0.16g")
            args.append(fields.mTC)
        if (len(fieldNames)) == 0 or ('Thick1' in fieldNames):
            cmds.append("var_Thick1=%0.16g")
            args.append(fields.mThick1)
        if (len(fieldNames)) == 0 or ('Thick2' in fieldNames):
            cmds.append("var_Thick2=%0.16g")
            args.append(fields.mThick2)
        if (len(fieldNames)) == 0 or ('Thick3' in fieldNames):
            cmds.append("var_Thick3=%0.16g")
            args.append(fields.mThick3)
        if (len(fieldNames)) == 0 or ('Torfac' in fieldNames):
            cmds.append("var_Torfac=%0.16g")
            args.append(fields.mTorfac)
        if (len(fieldNames)) == 0 or ('TSarea' in fieldNames):
            cmds.append("var_TSarea=%0.16g")
            args.append(fields.mTSarea)
        if (len(fieldNames)) == 0 or ('Width1' in fieldNames):
            cmds.append("var_Width1=%0.16g")
            args.append(fields.mWidth1)
        if (len(fieldNames)) == 0 or ('Width2' in fieldNames):
            cmds.append("var_Width2=%0.16g")
            args.append(fields.mWidth2)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionI WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMPlasticBilinearMaterial object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMPlasticBilinearMaterial object fields
    def addFEMPlasticBilinearMaterial(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMPlasticBilinearMaterial '\
            '(var_Alpha,var_Density,var_E,var_EPA,var_ET,var_Hardening,ref_Material,var_NU,var_TRef,var_Yield) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mAlpha)
        args.append(fields.mDensity)
        args.append(fields.mE)
        args.append(fields.mEPA)
        args.append(fields.mET)
        args.append(fields.mHardening)
        if fields.mMaterial == 0:
            args.append(None)
        else:
            args.append(fields.mMaterial)
        args.append(fields.mNU)
        args.append(fields.mTRef)
        args.append(fields.mYield)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMPlasticBilinearMaterial_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMPlasticBilinearMaterial objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMPlasticBilinearMaterial object fields
    def addFEMPlasticBilinearMaterialList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMPlasticBilinearMaterial '\
            '(var_Alpha,var_Density,var_E,var_EPA,var_ET,var_Hardening,ref_Material,var_NU,var_TRef,var_Yield) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAlpha)
            args.append(fields.mDensity)
            args.append(fields.mE)
            args.append(fields.mEPA)
            args.append(fields.mET)
            args.append(fields.mHardening)
            if fields.mMaterial == 0:
                args.append(None)
            else:
                args.append(fields.mMaterial)
            args.append(fields.mNU)
            args.append(fields.mTRef)
            args.append(fields.mYield)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMPlasticBilinearMaterial_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMPlasticBilinearMaterial object in the database.
    #
    # @param fields     FEMPlasticBilinearMaterial object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMPlasticBilinearMaterial(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Alpha' in fieldNames):
            cmds.append("var_Alpha=%0.16g")
            args.append(fields.mAlpha)
        if allFields or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if allFields or ('E' in fieldNames):
            cmds.append("var_E=%0.16g")
            args.append(fields.mE)
        if allFields or ('EPA' in fieldNames):
            cmds.append("var_EPA=%0.16g")
            args.append(fields.mEPA)
        if allFields or ('ET' in fieldNames):
            cmds.append("var_ET=%0.16g")
            args.append(fields.mET)
        if allFields or ('Hardening' in fieldNames):
            cmds.append("var_Hardening=%s")
            args.append(fields.mHardening)
        if allFields or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if allFields or ('NU' in fieldNames):
            cmds.append("var_NU=%0.16g")
            args.append(fields.mNU)
        if allFields or ('TRef' in fieldNames):
            cmds.append("var_TRef=%0.16g")
            args.append(fields.mTRef)
        if allFields or ('Yield' in fieldNames):
            cmds.append("var_Yield=%0.16g")
            args.append(fields.mYield)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMPlasticBilinearMaterial SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMPlasticBilinearMaterial object IDs from the database.
    def getFEMPlasticBilinearMaterialIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlasticBilinearMaterial'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMPlasticBilinearMaterial object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMPlasticBilinearMaterial object fields
    # @return \c True on success, \c False if object not found
    def getFEMPlasticBilinearMaterial(self, fields):
        selectCmd = 'SELECT '\
            'var_Alpha,var_Density,var_E,var_EPA,var_ET,var_Hardening,ref_Material,var_NU,var_TRef,var_Yield '\
            'FROM CLASS_FEMPlasticBilinearMaterial WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAlpha = data.pop(0)
        fields.mDensity = data.pop(0)
        fields.mE = data.pop(0)
        fields.mEPA = data.pop(0)
        fields.mET = data.pop(0)
        fields.mHardening = data.pop(0)
        fields.mMaterial = zeroNone(data.pop(0))
        fields.mNU = data.pop(0)
        fields.mTRef = data.pop(0)
        fields.mYield = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMPlasticBilinearMaterial objects matching the given
    # reference data.
    #
    # @param fields FEMPlasticBilinearMaterial object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMPlasticBilinearMaterial objects
    def findEqualFEMPlasticBilinearMaterial(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Alpha' in fieldNames):
            cmds.append("var_Alpha=%0.16g")
            args.append(fields.mAlpha)
        if (len(fieldNames)) == 0 or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if (len(fieldNames)) == 0 or ('E' in fieldNames):
            cmds.append("var_E=%0.16g")
            args.append(fields.mE)
        if (len(fieldNames)) == 0 or ('EPA' in fieldNames):
            cmds.append("var_EPA=%0.16g")
            args.append(fields.mEPA)
        if (len(fieldNames)) == 0 or ('ET' in fieldNames):
            cmds.append("var_ET=%0.16g")
            args.append(fields.mET)
        if (len(fieldNames)) == 0 or ('Hardening' in fieldNames):
            cmds.append("var_Hardening=%s")
            args.append(fields.mHardening)
        if (len(fieldNames)) == 0 or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if (len(fieldNames)) == 0 or ('NU' in fieldNames):
            cmds.append("var_NU=%0.16g")
            args.append(fields.mNU)
        if (len(fieldNames)) == 0 or ('TRef' in fieldNames):
            cmds.append("var_TRef=%0.16g")
            args.append(fields.mTRef)
        if (len(fieldNames)) == 0 or ('Yield' in fieldNames):
            cmds.append("var_Yield=%0.16g")
            args.append(fields.mYield)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlasticBilinearMaterial WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMMTForceData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMMTForceData object fields
    def addFEMMTForceData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMMTForceData '\
            '(var_Force,var_MomentRID,var_RecordNmb,var_TwistMomentID) VALUES '\
            "(%0.16g,%s,%s,%s)"
        args = []
        args.append(fields.mForce)
        args.append(fields.mMomentRID)
        args.append(fields.mRecordNmb)
        args.append(fields.mTwistMomentID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMMTForceData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMMTForceData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMMTForceData object fields
    def addFEMMTForceDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMMTForceData '\
            '(var_Force,var_MomentRID,var_RecordNmb,var_TwistMomentID) VALUES '\
            "(%0.16g,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mForce)
            args.append(fields.mMomentRID)
            args.append(fields.mRecordNmb)
            args.append(fields.mTwistMomentID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMMTForceData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMMTForceData object in the database.
    #
    # @param fields     FEMMTForceData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMMTForceData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Force' in fieldNames):
            cmds.append("var_Force=%0.16g")
            args.append(fields.mForce)
        if allFields or ('MomentRID' in fieldNames):
            cmds.append("var_MomentRID=%s")
            args.append(fields.mMomentRID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('TwistMomentID' in fieldNames):
            cmds.append("var_TwistMomentID=%s")
            args.append(fields.mTwistMomentID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMMTForceData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMMTForceData object IDs from the database.
    def getFEMMTForceDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMTForceData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMMTForceData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMMTForceData object fields
    # @return \c True on success, \c False if object not found
    def getFEMMTForceData(self, fields):
        selectCmd = 'SELECT '\
            'var_Force,var_MomentRID,var_RecordNmb,var_TwistMomentID '\
            'FROM CLASS_FEMMTForceData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mForce = data.pop(0)
        fields.mMomentRID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTwistMomentID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMMTForceData objects matching the given
    # reference data.
    #
    # @param fields FEMMTForceData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMMTForceData objects
    def findEqualFEMMTForceData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Force' in fieldNames):
            cmds.append("var_Force=%0.16g")
            args.append(fields.mForce)
        if (len(fieldNames)) == 0 or ('MomentRID' in fieldNames):
            cmds.append("var_MomentRID=%s")
            args.append(fields.mMomentRID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('TwistMomentID' in fieldNames):
            cmds.append("var_TwistMomentID=%s")
            args.append(fields.mTwistMomentID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMTForceData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMShellPressure object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMShellPressure object fields
    def addFEMShellPressure(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMShellPressure '\
            '(var_ArrivalTime,var_DeformDepend,var_Description,var_DirectFilter,var_ElementID,var_Face,ref_Group,var_Nodaux,var_P1,var_P2,var_P3,var_P4,var_RecordNmb,var_TimeFunctionID) VALUES '\
            "(%0.16g,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s)"
        args = []
        args.append(fields.mArrivalTime)
        args.append(fields.mDeformDepend)
        args.append(fields.mDescription)
        args.append(fields.mDirectFilter)
        args.append(fields.mElementID)
        args.append(fields.mFace)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mNodaux)
        args.append(fields.mP1)
        args.append(fields.mP2)
        args.append(fields.mP3)
        args.append(fields.mP4)
        args.append(fields.mRecordNmb)
        args.append(fields.mTimeFunctionID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMShellPressure_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMShellPressure objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMShellPressure object fields
    def addFEMShellPressureList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMShellPressure '\
            '(var_ArrivalTime,var_DeformDepend,var_Description,var_DirectFilter,var_ElementID,var_Face,ref_Group,var_Nodaux,var_P1,var_P2,var_P3,var_P4,var_RecordNmb,var_TimeFunctionID) VALUES '\
            "(%0.16g,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mArrivalTime)
            args.append(fields.mDeformDepend)
            args.append(fields.mDescription)
            args.append(fields.mDirectFilter)
            args.append(fields.mElementID)
            args.append(fields.mFace)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mNodaux)
            args.append(fields.mP1)
            args.append(fields.mP2)
            args.append(fields.mP3)
            args.append(fields.mP4)
            args.append(fields.mRecordNmb)
            args.append(fields.mTimeFunctionID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMShellPressure_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMShellPressure object in the database.
    #
    # @param fields     FEMShellPressure object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMShellPressure(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if allFields or ('DeformDepend' in fieldNames):
            cmds.append("var_DeformDepend=%s")
            args.append(fields.mDeformDepend)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('DirectFilter' in fieldNames):
            cmds.append("var_DirectFilter=%s")
            args.append(fields.mDirectFilter)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Face' in fieldNames):
            cmds.append("var_Face=%s")
            args.append(fields.mFace)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('Nodaux' in fieldNames):
            cmds.append("var_Nodaux=%s")
            args.append(fields.mNodaux)
        if allFields or ('P1' in fieldNames):
            cmds.append("var_P1=%0.16g")
            args.append(fields.mP1)
        if allFields or ('P2' in fieldNames):
            cmds.append("var_P2=%0.16g")
            args.append(fields.mP2)
        if allFields or ('P3' in fieldNames):
            cmds.append("var_P3=%0.16g")
            args.append(fields.mP3)
        if allFields or ('P4' in fieldNames):
            cmds.append("var_P4=%0.16g")
            args.append(fields.mP4)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMShellPressure SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMShellPressure object IDs from the database.
    def getFEMShellPressureIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellPressure'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMShellPressure object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMShellPressure object fields
    # @return \c True on success, \c False if object not found
    def getFEMShellPressure(self, fields):
        selectCmd = 'SELECT '\
            'var_ArrivalTime,var_DeformDepend,var_Description,var_DirectFilter,var_ElementID,var_Face,ref_Group,var_Nodaux,var_P1,var_P2,var_P3,var_P4,var_RecordNmb,var_TimeFunctionID '\
            'FROM CLASS_FEMShellPressure WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mArrivalTime = data.pop(0)
        fields.mDeformDepend = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mDirectFilter = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mFace = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mNodaux = data.pop(0)
        fields.mP1 = data.pop(0)
        fields.mP2 = data.pop(0)
        fields.mP3 = data.pop(0)
        fields.mP4 = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTimeFunctionID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMShellPressure objects matching the given
    # reference data.
    #
    # @param fields FEMShellPressure object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMShellPressure objects
    def findEqualFEMShellPressure(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if (len(fieldNames)) == 0 or ('DeformDepend' in fieldNames):
            cmds.append("var_DeformDepend=%s")
            args.append(fields.mDeformDepend)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('DirectFilter' in fieldNames):
            cmds.append("var_DirectFilter=%s")
            args.append(fields.mDirectFilter)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Face' in fieldNames):
            cmds.append("var_Face=%s")
            args.append(fields.mFace)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('Nodaux' in fieldNames):
            cmds.append("var_Nodaux=%s")
            args.append(fields.mNodaux)
        if (len(fieldNames)) == 0 or ('P1' in fieldNames):
            cmds.append("var_P1=%0.16g")
            args.append(fields.mP1)
        if (len(fieldNames)) == 0 or ('P2' in fieldNames):
            cmds.append("var_P2=%0.16g")
            args.append(fields.mP2)
        if (len(fieldNames)) == 0 or ('P3' in fieldNames):
            cmds.append("var_P3=%0.16g")
            args.append(fields.mP3)
        if (len(fieldNames)) == 0 or ('P4' in fieldNames):
            cmds.append("var_P4=%0.16g")
            args.append(fields.mP4)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellPressure WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMMatrices object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMMatrices object fields
    def addFEMMatrices(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMMatrices '\
            '(var_Description,var_Lump,var_MatrixID,var_MatrixType,var_ND,var_NS) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mLump)
        args.append(fields.mMatrixID)
        args.append(fields.mMatrixType)
        args.append(fields.mND)
        args.append(fields.mNS)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMMatrices_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMMatrices objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMMatrices object fields
    def addFEMMatricesList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMMatrices '\
            '(var_Description,var_Lump,var_MatrixID,var_MatrixType,var_ND,var_NS) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mLump)
            args.append(fields.mMatrixID)
            args.append(fields.mMatrixType)
            args.append(fields.mND)
            args.append(fields.mNS)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMMatrices_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMMatrices object in the database.
    #
    # @param fields     FEMMatrices object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMMatrices(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Lump' in fieldNames):
            cmds.append("var_Lump=%s")
            args.append(fields.mLump)
        if allFields or ('MatrixID' in fieldNames):
            cmds.append("var_MatrixID=%s")
            args.append(fields.mMatrixID)
        if allFields or ('MatrixType' in fieldNames):
            cmds.append("var_MatrixType=%s")
            args.append(fields.mMatrixType)
        if allFields or ('ND' in fieldNames):
            cmds.append("var_ND=%s")
            args.append(fields.mND)
        if allFields or ('NS' in fieldNames):
            cmds.append("var_NS=%s")
            args.append(fields.mNS)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMMatrices SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMMatrices object IDs from the database.
    def getFEMMatricesIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMatrices'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMMatrices object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMMatrices object fields
    # @return \c True on success, \c False if object not found
    def getFEMMatrices(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_Lump,var_MatrixID,var_MatrixType,var_ND,var_NS '\
            'FROM CLASS_FEMMatrices WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mLump = data.pop(0)
        fields.mMatrixID = data.pop(0)
        fields.mMatrixType = data.pop(0)
        fields.mND = data.pop(0)
        fields.mNS = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMMatrices objects matching the given
    # reference data.
    #
    # @param fields FEMMatrices object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMMatrices objects
    def findEqualFEMMatrices(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Lump' in fieldNames):
            cmds.append("var_Lump=%s")
            args.append(fields.mLump)
        if (len(fieldNames)) == 0 or ('MatrixID' in fieldNames):
            cmds.append("var_MatrixID=%s")
            args.append(fields.mMatrixID)
        if (len(fieldNames)) == 0 or ('MatrixType' in fieldNames):
            cmds.append("var_MatrixType=%s")
            args.append(fields.mMatrixType)
        if (len(fieldNames)) == 0 or ('ND' in fieldNames):
            cmds.append("var_ND=%s")
            args.append(fields.mND)
        if (len(fieldNames)) == 0 or ('NS' in fieldNames):
            cmds.append("var_NS=%s")
            args.append(fields.mNS)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMatrices WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMDamping object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMDamping object fields
    def addFEMDamping(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMDamping '\
            '(var_ALPHA,var_BETA,ref_Group) VALUES '\
            "(%0.16g,%0.16g,%s)"
        args = []
        args.append(fields.mALPHA)
        args.append(fields.mBETA)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMDamping_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMDamping objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMDamping object fields
    def addFEMDampingList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMDamping '\
            '(var_ALPHA,var_BETA,ref_Group) VALUES '\
            "(%0.16g,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mALPHA)
            args.append(fields.mBETA)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMDamping_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMDamping object in the database.
    #
    # @param fields     FEMDamping object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMDamping(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ALPHA' in fieldNames):
            cmds.append("var_ALPHA=%0.16g")
            args.append(fields.mALPHA)
        if allFields or ('BETA' in fieldNames):
            cmds.append("var_BETA=%0.16g")
            args.append(fields.mBETA)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMDamping SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMDamping object IDs from the database.
    def getFEMDampingIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMDamping'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMDamping object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMDamping object fields
    # @return \c True on success, \c False if object not found
    def getFEMDamping(self, fields):
        selectCmd = 'SELECT '\
            'var_ALPHA,var_BETA,ref_Group '\
            'FROM CLASS_FEMDamping WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mALPHA = data.pop(0)
        fields.mBETA = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all FEMDamping objects matching the given
    # reference data.
    #
    # @param fields FEMDamping object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMDamping objects
    def findEqualFEMDamping(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ALPHA' in fieldNames):
            cmds.append("var_ALPHA=%0.16g")
            args.append(fields.mALPHA)
        if (len(fieldNames)) == 0 or ('BETA' in fieldNames):
            cmds.append("var_BETA=%0.16g")
            args.append(fields.mBETA)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMDamping WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMMaterial object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMMaterial object fields
    def addFEMMaterial(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMMaterial '\
            '(var_Description,ref_MaterialType,var_LocalID,ref_Structure) VALUES '\
            "(%s,%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(self.MaterialTypeLabelToId[str(fields.mMaterialType)])
        args.append(fields.mLocalID)
        if fields.mStructure == 0:
            args.append(None)
        else:
            args.append(fields.mStructure)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMMaterial_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMMaterial objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMMaterial object fields
    def addFEMMaterialList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMMaterial '\
            '(var_Description,ref_MaterialType,var_LocalID,ref_Structure) VALUES '\
            "(%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(self.MaterialTypeLabelToId[str(fields.mMaterialType)])
            args.append(fields.mLocalID)
            if fields.mStructure == 0:
                args.append(None)
            else:
                args.append(fields.mStructure)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMMaterial_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMMaterial object in the database.
    #
    # @param fields     FEMMaterial object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMMaterial(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('MaterialType' in fieldNames):
            cmds.append("ref_MaterialType=%s")
            args.append(self.MaterialTypeLabelToId[str(fields.mMaterialType)])
        if allFields or ('LocalID' in fieldNames):
            cmds.append("var_LocalID=%s")
            args.append(fields.mLocalID)
        if allFields or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMMaterial SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMMaterial object IDs from the database.
    def getFEMMaterialIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMaterial'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMMaterial object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMMaterial object fields
    # @return \c True on success, \c False if object not found
    def getFEMMaterial(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,ref_MaterialType,var_LocalID,ref_Structure '\
            'FROM CLASS_FEMMaterial WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mMaterialType = self.MaterialTypeIdToEnum[data.pop(0)]
        fields.mLocalID = data.pop(0)
        fields.mStructure = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all FEMMaterial objects matching the given
    # reference data.
    #
    # @param fields FEMMaterial object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMMaterial objects
    def findEqualFEMMaterial(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('MaterialType' in fieldNames):
            cmds.append("ref_MaterialType=%s")
            args.append(self.MaterialTypeLabelToId[str(fields.mMaterialType)])
        if (len(fieldNames)) == 0 or ('LocalID' in fieldNames):
            cmds.append("var_LocalID=%s")
            args.append(fields.mLocalID)
        if (len(fieldNames)) == 0 or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMaterial WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMMatrixData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMMatrixData object fields
    def addFEMMatrixData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMMatrixData '\
            '(var_Coeff,var_ColumnIndex,var_MatrixID,var_RecordNmb,var_RowIndex) VALUES '\
            "(%0.16g,%s,%s,%s,%s)"
        args = []
        args.append(fields.mCoeff)
        args.append(fields.mColumnIndex)
        args.append(fields.mMatrixID)
        args.append(fields.mRecordNmb)
        args.append(fields.mRowIndex)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMMatrixData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMMatrixData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMMatrixData object fields
    def addFEMMatrixDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMMatrixData '\
            '(var_Coeff,var_ColumnIndex,var_MatrixID,var_RecordNmb,var_RowIndex) VALUES '\
            "(%0.16g,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mCoeff)
            args.append(fields.mColumnIndex)
            args.append(fields.mMatrixID)
            args.append(fields.mRecordNmb)
            args.append(fields.mRowIndex)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMMatrixData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMMatrixData object in the database.
    #
    # @param fields     FEMMatrixData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMMatrixData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Coeff' in fieldNames):
            cmds.append("var_Coeff=%0.16g")
            args.append(fields.mCoeff)
        if allFields or ('ColumnIndex' in fieldNames):
            cmds.append("var_ColumnIndex=%s")
            args.append(fields.mColumnIndex)
        if allFields or ('MatrixID' in fieldNames):
            cmds.append("var_MatrixID=%s")
            args.append(fields.mMatrixID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('RowIndex' in fieldNames):
            cmds.append("var_RowIndex=%s")
            args.append(fields.mRowIndex)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMMatrixData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMMatrixData object IDs from the database.
    def getFEMMatrixDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMatrixData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMMatrixData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMMatrixData object fields
    # @return \c True on success, \c False if object not found
    def getFEMMatrixData(self, fields):
        selectCmd = 'SELECT '\
            'var_Coeff,var_ColumnIndex,var_MatrixID,var_RecordNmb,var_RowIndex '\
            'FROM CLASS_FEMMatrixData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mCoeff = data.pop(0)
        fields.mColumnIndex = data.pop(0)
        fields.mMatrixID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mRowIndex = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMMatrixData objects matching the given
    # reference data.
    #
    # @param fields FEMMatrixData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMMatrixData objects
    def findEqualFEMMatrixData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Coeff' in fieldNames):
            cmds.append("var_Coeff=%0.16g")
            args.append(fields.mCoeff)
        if (len(fieldNames)) == 0 or ('ColumnIndex' in fieldNames):
            cmds.append("var_ColumnIndex=%s")
            args.append(fields.mColumnIndex)
        if (len(fieldNames)) == 0 or ('MatrixID' in fieldNames):
            cmds.append("var_MatrixID=%s")
            args.append(fields.mMatrixID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('RowIndex' in fieldNames):
            cmds.append("var_RowIndex=%s")
            args.append(fields.mRowIndex)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMatrixData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMShellAxesOrtho object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMShellAxesOrtho object fields
    def addFEMShellAxesOrtho(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMShellAxesOrtho '\
            '(var_ALFA,var_AxOrthoID,ref_Group,var_LineID,var_RecordNmb) VALUES '\
            "(%0.16g,%s,%s,%s,%s)"
        args = []
        args.append(fields.mALFA)
        args.append(fields.mAxOrthoID)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mLineID)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMShellAxesOrtho_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMShellAxesOrtho objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMShellAxesOrtho object fields
    def addFEMShellAxesOrthoList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMShellAxesOrtho '\
            '(var_ALFA,var_AxOrthoID,ref_Group,var_LineID,var_RecordNmb) VALUES '\
            "(%0.16g,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mALFA)
            args.append(fields.mAxOrthoID)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mLineID)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMShellAxesOrtho_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMShellAxesOrtho object in the database.
    #
    # @param fields     FEMShellAxesOrtho object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMShellAxesOrtho(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ALFA' in fieldNames):
            cmds.append("var_ALFA=%0.16g")
            args.append(fields.mALFA)
        if allFields or ('AxOrthoID' in fieldNames):
            cmds.append("var_AxOrthoID=%s")
            args.append(fields.mAxOrthoID)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('LineID' in fieldNames):
            cmds.append("var_LineID=%s")
            args.append(fields.mLineID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMShellAxesOrtho SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMShellAxesOrtho object IDs from the database.
    def getFEMShellAxesOrthoIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellAxesOrtho'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMShellAxesOrtho object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMShellAxesOrtho object fields
    # @return \c True on success, \c False if object not found
    def getFEMShellAxesOrtho(self, fields):
        selectCmd = 'SELECT '\
            'var_ALFA,var_AxOrthoID,ref_Group,var_LineID,var_RecordNmb '\
            'FROM CLASS_FEMShellAxesOrtho WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mALFA = data.pop(0)
        fields.mAxOrthoID = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mLineID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMShellAxesOrtho objects matching the given
    # reference data.
    #
    # @param fields FEMShellAxesOrtho object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMShellAxesOrtho objects
    def findEqualFEMShellAxesOrtho(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ALFA' in fieldNames):
            cmds.append("var_ALFA=%0.16g")
            args.append(fields.mALFA)
        if (len(fieldNames)) == 0 or ('AxOrthoID' in fieldNames):
            cmds.append("var_AxOrthoID=%s")
            args.append(fields.mAxOrthoID)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('LineID' in fieldNames):
            cmds.append("var_LineID=%s")
            args.append(fields.mLineID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellAxesOrtho WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMEndreleases object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMEndreleases object fields
    def addFEMEndreleases(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMEndreleases '\
            '(var_Description,var_EndRelease,var_Moment1,var_Moment2,var_Moment3,var_Moment4,var_Moment5,var_Moment6) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mEndRelease)
        args.append(fields.mMoment1)
        args.append(fields.mMoment2)
        args.append(fields.mMoment3)
        args.append(fields.mMoment4)
        args.append(fields.mMoment5)
        args.append(fields.mMoment6)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMEndreleases_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMEndreleases objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMEndreleases object fields
    def addFEMEndreleasesList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMEndreleases '\
            '(var_Description,var_EndRelease,var_Moment1,var_Moment2,var_Moment3,var_Moment4,var_Moment5,var_Moment6) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mEndRelease)
            args.append(fields.mMoment1)
            args.append(fields.mMoment2)
            args.append(fields.mMoment3)
            args.append(fields.mMoment4)
            args.append(fields.mMoment5)
            args.append(fields.mMoment6)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMEndreleases_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMEndreleases object in the database.
    #
    # @param fields     FEMEndreleases object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMEndreleases(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('EndRelease' in fieldNames):
            cmds.append("var_EndRelease=%s")
            args.append(fields.mEndRelease)
        if allFields or ('Moment1' in fieldNames):
            cmds.append("var_Moment1=%s")
            args.append(fields.mMoment1)
        if allFields or ('Moment2' in fieldNames):
            cmds.append("var_Moment2=%s")
            args.append(fields.mMoment2)
        if allFields or ('Moment3' in fieldNames):
            cmds.append("var_Moment3=%s")
            args.append(fields.mMoment3)
        if allFields or ('Moment4' in fieldNames):
            cmds.append("var_Moment4=%s")
            args.append(fields.mMoment4)
        if allFields or ('Moment5' in fieldNames):
            cmds.append("var_Moment5=%s")
            args.append(fields.mMoment5)
        if allFields or ('Moment6' in fieldNames):
            cmds.append("var_Moment6=%s")
            args.append(fields.mMoment6)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMEndreleases SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMEndreleases object IDs from the database.
    def getFEMEndreleasesIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMEndreleases'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMEndreleases object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMEndreleases object fields
    # @return \c True on success, \c False if object not found
    def getFEMEndreleases(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_EndRelease,var_Moment1,var_Moment2,var_Moment3,var_Moment4,var_Moment5,var_Moment6 '\
            'FROM CLASS_FEMEndreleases WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mEndRelease = data.pop(0)
        fields.mMoment1 = data.pop(0)
        fields.mMoment2 = data.pop(0)
        fields.mMoment3 = data.pop(0)
        fields.mMoment4 = data.pop(0)
        fields.mMoment5 = data.pop(0)
        fields.mMoment6 = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMEndreleases objects matching the given
    # reference data.
    #
    # @param fields FEMEndreleases object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMEndreleases objects
    def findEqualFEMEndreleases(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('EndRelease' in fieldNames):
            cmds.append("var_EndRelease=%s")
            args.append(fields.mEndRelease)
        if (len(fieldNames)) == 0 or ('Moment1' in fieldNames):
            cmds.append("var_Moment1=%s")
            args.append(fields.mMoment1)
        if (len(fieldNames)) == 0 or ('Moment2' in fieldNames):
            cmds.append("var_Moment2=%s")
            args.append(fields.mMoment2)
        if (len(fieldNames)) == 0 or ('Moment3' in fieldNames):
            cmds.append("var_Moment3=%s")
            args.append(fields.mMoment3)
        if (len(fieldNames)) == 0 or ('Moment4' in fieldNames):
            cmds.append("var_Moment4=%s")
            args.append(fields.mMoment4)
        if (len(fieldNames)) == 0 or ('Moment5' in fieldNames):
            cmds.append("var_Moment5=%s")
            args.append(fields.mMoment5)
        if (len(fieldNames)) == 0 or ('Moment6' in fieldNames):
            cmds.append("var_Moment6=%s")
            args.append(fields.mMoment6)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMEndreleases WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMTrussGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMTrussGroup object fields
    def addFEMTrussGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMTrussGroup '\
            '(var_Displacement,var_GAPS,ref_Group,var_IniStrain,ref_Material,var_SectionArea) VALUES '\
            "(%s,%s,%s,%s,%s,%0.16g)"
        args = []
        args.append(fields.mDisplacement)
        args.append(fields.mGAPS)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mIniStrain)
        if fields.mMaterial == 0:
            args.append(None)
        else:
            args.append(fields.mMaterial)
        args.append(fields.mSectionArea)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMTrussGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMTrussGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMTrussGroup object fields
    def addFEMTrussGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMTrussGroup '\
            '(var_Displacement,var_GAPS,ref_Group,var_IniStrain,ref_Material,var_SectionArea) VALUES '\
            "(%s,%s,%s,%s,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDisplacement)
            args.append(fields.mGAPS)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mIniStrain)
            if fields.mMaterial == 0:
                args.append(None)
            else:
                args.append(fields.mMaterial)
            args.append(fields.mSectionArea)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMTrussGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMTrussGroup object in the database.
    #
    # @param fields     FEMTrussGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMTrussGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if allFields or ('GAPS' in fieldNames):
            cmds.append("var_GAPS=%s")
            args.append(fields.mGAPS)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('IniStrain' in fieldNames):
            cmds.append("var_IniStrain=%s")
            args.append(fields.mIniStrain)
        if allFields or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if allFields or ('SectionArea' in fieldNames):
            cmds.append("var_SectionArea=%0.16g")
            args.append(fields.mSectionArea)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMTrussGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMTrussGroup object IDs from the database.
    def getFEMTrussGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTrussGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMTrussGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMTrussGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMTrussGroup(self, fields):
        selectCmd = 'SELECT '\
            'var_Displacement,var_GAPS,ref_Group,var_IniStrain,ref_Material,var_SectionArea '\
            'FROM CLASS_FEMTrussGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDisplacement = data.pop(0)
        fields.mGAPS = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mIniStrain = data.pop(0)
        fields.mMaterial = zeroNone(data.pop(0))
        fields.mSectionArea = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMTrussGroup objects matching the given
    # reference data.
    #
    # @param fields FEMTrussGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMTrussGroup objects
    def findEqualFEMTrussGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('GAPS' in fieldNames):
            cmds.append("var_GAPS=%s")
            args.append(fields.mGAPS)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('IniStrain' in fieldNames):
            cmds.append("var_IniStrain=%s")
            args.append(fields.mIniStrain)
        if (len(fieldNames)) == 0 or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if (len(fieldNames)) == 0 or ('SectionArea' in fieldNames):
            cmds.append("var_SectionArea=%0.16g")
            args.append(fields.mSectionArea)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTrussGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMInitialTemperature object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMInitialTemperature object fields
    def addFEMInitialTemperature(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMInitialTemperature '\
            '(ref_Node,var_Temperature) VALUES '\
            "(%s,%0.16g)"
        args = []
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(fields.mTemperature)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMInitialTemperature_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMInitialTemperature objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMInitialTemperature object fields
    def addFEMInitialTemperatureList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMInitialTemperature '\
            '(ref_Node,var_Temperature) VALUES '\
            "(%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(fields.mTemperature)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMInitialTemperature_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMInitialTemperature object in the database.
    #
    # @param fields     FEMInitialTemperature object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMInitialTemperature(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('Temperature' in fieldNames):
            cmds.append("var_Temperature=%0.16g")
            args.append(fields.mTemperature)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMInitialTemperature SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMInitialTemperature object IDs from the database.
    def getFEMInitialTemperatureIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMInitialTemperature'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMInitialTemperature object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMInitialTemperature object fields
    # @return \c True on success, \c False if object not found
    def getFEMInitialTemperature(self, fields):
        selectCmd = 'SELECT '\
            'ref_Node,var_Temperature '\
            'FROM CLASS_FEMInitialTemperature WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mNode = zeroNone(data.pop(0))
        fields.mTemperature = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMInitialTemperature objects matching the given
    # reference data.
    #
    # @param fields FEMInitialTemperature object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMInitialTemperature objects
    def findEqualFEMInitialTemperature(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('Temperature' in fieldNames):
            cmds.append("var_Temperature=%0.16g")
            args.append(fields.mTemperature)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMInitialTemperature WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMThermoIsoMaterials object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMThermoIsoMaterials object fields
    def addFEMThermoIsoMaterials(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMThermoIsoMaterials '\
            '(var_Density,var_MaterialID,var_TREF) VALUES '\
            "(%0.16g,%s,%0.16g)"
        args = []
        args.append(fields.mDensity)
        args.append(fields.mMaterialID)
        args.append(fields.mTREF)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMThermoIsoMaterials_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMThermoIsoMaterials objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMThermoIsoMaterials object fields
    def addFEMThermoIsoMaterialsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMThermoIsoMaterials '\
            '(var_Density,var_MaterialID,var_TREF) VALUES '\
            "(%0.16g,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDensity)
            args.append(fields.mMaterialID)
            args.append(fields.mTREF)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMThermoIsoMaterials_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMThermoIsoMaterials object in the database.
    #
    # @param fields     FEMThermoIsoMaterials object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMThermoIsoMaterials(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('TREF' in fieldNames):
            cmds.append("var_TREF=%0.16g")
            args.append(fields.mTREF)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMThermoIsoMaterials SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMThermoIsoMaterials object IDs from the database.
    def getFEMThermoIsoMaterialsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThermoIsoMaterials'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMThermoIsoMaterials object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMThermoIsoMaterials object fields
    # @return \c True on success, \c False if object not found
    def getFEMThermoIsoMaterials(self, fields):
        selectCmd = 'SELECT '\
            'var_Density,var_MaterialID,var_TREF '\
            'FROM CLASS_FEMThermoIsoMaterials WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDensity = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mTREF = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMThermoIsoMaterials objects matching the given
    # reference data.
    #
    # @param fields FEMThermoIsoMaterials object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMThermoIsoMaterials objects
    def findEqualFEMThermoIsoMaterials(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('TREF' in fieldNames):
            cmds.append("var_TREF=%0.16g")
            args.append(fields.mTREF)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThermoIsoMaterials WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMThermoIsoData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMThermoIsoData object fields
    def addFEMThermoIsoData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMThermoIsoData '\
            '(var_ALPHA,var_E,var_MaterialID,var_NU,var_RecordNmb,var_Theta) VALUES '\
            "(%0.16g,%0.16g,%s,%0.16g,%s,%0.16g)"
        args = []
        args.append(fields.mALPHA)
        args.append(fields.mE)
        args.append(fields.mMaterialID)
        args.append(fields.mNU)
        args.append(fields.mRecordNmb)
        args.append(fields.mTheta)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMThermoIsoData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMThermoIsoData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMThermoIsoData object fields
    def addFEMThermoIsoDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMThermoIsoData '\
            '(var_ALPHA,var_E,var_MaterialID,var_NU,var_RecordNmb,var_Theta) VALUES '\
            "(%0.16g,%0.16g,%s,%0.16g,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mALPHA)
            args.append(fields.mE)
            args.append(fields.mMaterialID)
            args.append(fields.mNU)
            args.append(fields.mRecordNmb)
            args.append(fields.mTheta)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMThermoIsoData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMThermoIsoData object in the database.
    #
    # @param fields     FEMThermoIsoData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMThermoIsoData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ALPHA' in fieldNames):
            cmds.append("var_ALPHA=%0.16g")
            args.append(fields.mALPHA)
        if allFields or ('E' in fieldNames):
            cmds.append("var_E=%0.16g")
            args.append(fields.mE)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('NU' in fieldNames):
            cmds.append("var_NU=%0.16g")
            args.append(fields.mNU)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Theta' in fieldNames):
            cmds.append("var_Theta=%0.16g")
            args.append(fields.mTheta)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMThermoIsoData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMThermoIsoData object IDs from the database.
    def getFEMThermoIsoDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThermoIsoData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMThermoIsoData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMThermoIsoData object fields
    # @return \c True on success, \c False if object not found
    def getFEMThermoIsoData(self, fields):
        selectCmd = 'SELECT '\
            'var_ALPHA,var_E,var_MaterialID,var_NU,var_RecordNmb,var_Theta '\
            'FROM CLASS_FEMThermoIsoData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mALPHA = data.pop(0)
        fields.mE = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mNU = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTheta = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMThermoIsoData objects matching the given
    # reference data.
    #
    # @param fields FEMThermoIsoData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMThermoIsoData objects
    def findEqualFEMThermoIsoData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ALPHA' in fieldNames):
            cmds.append("var_ALPHA=%0.16g")
            args.append(fields.mALPHA)
        if (len(fieldNames)) == 0 or ('E' in fieldNames):
            cmds.append("var_E=%0.16g")
            args.append(fields.mE)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('NU' in fieldNames):
            cmds.append("var_NU=%0.16g")
            args.append(fields.mNU)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Theta' in fieldNames):
            cmds.append("var_Theta=%0.16g")
            args.append(fields.mTheta)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThermoIsoData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMContactGroup3 object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMContactGroup3 object fields
    def addFEMContactGroup3(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMContactGroup3 '\
            '(var_ContGroupID,var_Depth,var_Description,var_Forces,var_Friction,var_IniPenetration,var_NodeToNode,var_Offset,var_PenetrAlgorithm,var_TBirth,var_TDeath,var_Tied,var_TiedOffset,var_Tolerance,var_Tractions) VALUES '\
            "(%s,%0.16g,%s,%s,%0.16g,%s,%s,%0.16g,%s,%0.16g,%0.16g,%s,%0.16g,%0.16g,%s)"
        args = []
        args.append(fields.mContGroupID)
        args.append(fields.mDepth)
        args.append(fields.mDescription)
        args.append(fields.mForces)
        args.append(fields.mFriction)
        args.append(fields.mIniPenetration)
        args.append(fields.mNodeToNode)
        args.append(fields.mOffset)
        args.append(fields.mPenetrAlgorithm)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        args.append(fields.mTied)
        args.append(fields.mTiedOffset)
        args.append(fields.mTolerance)
        args.append(fields.mTractions)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMContactGroup3_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMContactGroup3 objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMContactGroup3 object fields
    def addFEMContactGroup3List(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMContactGroup3 '\
            '(var_ContGroupID,var_Depth,var_Description,var_Forces,var_Friction,var_IniPenetration,var_NodeToNode,var_Offset,var_PenetrAlgorithm,var_TBirth,var_TDeath,var_Tied,var_TiedOffset,var_Tolerance,var_Tractions) VALUES '\
            "(%s,%0.16g,%s,%s,%0.16g,%s,%s,%0.16g,%s,%0.16g,%0.16g,%s,%0.16g,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mContGroupID)
            args.append(fields.mDepth)
            args.append(fields.mDescription)
            args.append(fields.mForces)
            args.append(fields.mFriction)
            args.append(fields.mIniPenetration)
            args.append(fields.mNodeToNode)
            args.append(fields.mOffset)
            args.append(fields.mPenetrAlgorithm)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            args.append(fields.mTied)
            args.append(fields.mTiedOffset)
            args.append(fields.mTolerance)
            args.append(fields.mTractions)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMContactGroup3_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMContactGroup3 object in the database.
    #
    # @param fields     FEMContactGroup3 object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMContactGroup3(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ContGroupID' in fieldNames):
            cmds.append("var_ContGroupID=%s")
            args.append(fields.mContGroupID)
        if allFields or ('Depth' in fieldNames):
            cmds.append("var_Depth=%0.16g")
            args.append(fields.mDepth)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Forces' in fieldNames):
            cmds.append("var_Forces=%s")
            args.append(fields.mForces)
        if allFields or ('Friction' in fieldNames):
            cmds.append("var_Friction=%0.16g")
            args.append(fields.mFriction)
        if allFields or ('IniPenetration' in fieldNames):
            cmds.append("var_IniPenetration=%s")
            args.append(fields.mIniPenetration)
        if allFields or ('NodeToNode' in fieldNames):
            cmds.append("var_NodeToNode=%s")
            args.append(fields.mNodeToNode)
        if allFields or ('Offset' in fieldNames):
            cmds.append("var_Offset=%0.16g")
            args.append(fields.mOffset)
        if allFields or ('PenetrAlgorithm' in fieldNames):
            cmds.append("var_PenetrAlgorithm=%s")
            args.append(fields.mPenetrAlgorithm)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if allFields or ('Tied' in fieldNames):
            cmds.append("var_Tied=%s")
            args.append(fields.mTied)
        if allFields or ('TiedOffset' in fieldNames):
            cmds.append("var_TiedOffset=%0.16g")
            args.append(fields.mTiedOffset)
        if allFields or ('Tolerance' in fieldNames):
            cmds.append("var_Tolerance=%0.16g")
            args.append(fields.mTolerance)
        if allFields or ('Tractions' in fieldNames):
            cmds.append("var_Tractions=%s")
            args.append(fields.mTractions)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMContactGroup3 SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMContactGroup3 object IDs from the database.
    def getFEMContactGroup3Ids(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMContactGroup3'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMContactGroup3 object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMContactGroup3 object fields
    # @return \c True on success, \c False if object not found
    def getFEMContactGroup3(self, fields):
        selectCmd = 'SELECT '\
            'var_ContGroupID,var_Depth,var_Description,var_Forces,var_Friction,var_IniPenetration,var_NodeToNode,var_Offset,var_PenetrAlgorithm,var_TBirth,var_TDeath,var_Tied,var_TiedOffset,var_Tolerance,var_Tractions '\
            'FROM CLASS_FEMContactGroup3 WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mContGroupID = data.pop(0)
        fields.mDepth = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mForces = data.pop(0)
        fields.mFriction = data.pop(0)
        fields.mIniPenetration = data.pop(0)
        fields.mNodeToNode = data.pop(0)
        fields.mOffset = data.pop(0)
        fields.mPenetrAlgorithm = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        fields.mTied = data.pop(0)
        fields.mTiedOffset = data.pop(0)
        fields.mTolerance = data.pop(0)
        fields.mTractions = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMContactGroup3 objects matching the given
    # reference data.
    #
    # @param fields FEMContactGroup3 object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMContactGroup3 objects
    def findEqualFEMContactGroup3(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ContGroupID' in fieldNames):
            cmds.append("var_ContGroupID=%s")
            args.append(fields.mContGroupID)
        if (len(fieldNames)) == 0 or ('Depth' in fieldNames):
            cmds.append("var_Depth=%0.16g")
            args.append(fields.mDepth)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Forces' in fieldNames):
            cmds.append("var_Forces=%s")
            args.append(fields.mForces)
        if (len(fieldNames)) == 0 or ('Friction' in fieldNames):
            cmds.append("var_Friction=%0.16g")
            args.append(fields.mFriction)
        if (len(fieldNames)) == 0 or ('IniPenetration' in fieldNames):
            cmds.append("var_IniPenetration=%s")
            args.append(fields.mIniPenetration)
        if (len(fieldNames)) == 0 or ('NodeToNode' in fieldNames):
            cmds.append("var_NodeToNode=%s")
            args.append(fields.mNodeToNode)
        if (len(fieldNames)) == 0 or ('Offset' in fieldNames):
            cmds.append("var_Offset=%0.16g")
            args.append(fields.mOffset)
        if (len(fieldNames)) == 0 or ('PenetrAlgorithm' in fieldNames):
            cmds.append("var_PenetrAlgorithm=%s")
            args.append(fields.mPenetrAlgorithm)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if (len(fieldNames)) == 0 or ('Tied' in fieldNames):
            cmds.append("var_Tied=%s")
            args.append(fields.mTied)
        if (len(fieldNames)) == 0 or ('TiedOffset' in fieldNames):
            cmds.append("var_TiedOffset=%0.16g")
            args.append(fields.mTiedOffset)
        if (len(fieldNames)) == 0 or ('Tolerance' in fieldNames):
            cmds.append("var_Tolerance=%0.16g")
            args.append(fields.mTolerance)
        if (len(fieldNames)) == 0 or ('Tractions' in fieldNames):
            cmds.append("var_Tractions=%s")
            args.append(fields.mTractions)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMContactGroup3 WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMNLElasticMaterials object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMNLElasticMaterials object fields
    def addFEMNLElasticMaterials(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMNLElasticMaterials '\
            '(var_Dcurve,var_Density,var_MaterialID) VALUES '\
            "(%s,%0.16g,%s)"
        args = []
        args.append(fields.mDcurve)
        args.append(fields.mDensity)
        args.append(fields.mMaterialID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMNLElasticMaterials_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMNLElasticMaterials objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMNLElasticMaterials object fields
    def addFEMNLElasticMaterialsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMNLElasticMaterials '\
            '(var_Dcurve,var_Density,var_MaterialID) VALUES '\
            "(%s,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDcurve)
            args.append(fields.mDensity)
            args.append(fields.mMaterialID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMNLElasticMaterials_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMNLElasticMaterials object in the database.
    #
    # @param fields     FEMNLElasticMaterials object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMNLElasticMaterials(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Dcurve' in fieldNames):
            cmds.append("var_Dcurve=%s")
            args.append(fields.mDcurve)
        if allFields or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMNLElasticMaterials SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMNLElasticMaterials object IDs from the database.
    def getFEMNLElasticMaterialsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNLElasticMaterials'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMNLElasticMaterials object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMNLElasticMaterials object fields
    # @return \c True on success, \c False if object not found
    def getFEMNLElasticMaterials(self, fields):
        selectCmd = 'SELECT '\
            'var_Dcurve,var_Density,var_MaterialID '\
            'FROM CLASS_FEMNLElasticMaterials WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDcurve = data.pop(0)
        fields.mDensity = data.pop(0)
        fields.mMaterialID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMNLElasticMaterials objects matching the given
    # reference data.
    #
    # @param fields FEMNLElasticMaterials object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMNLElasticMaterials objects
    def findEqualFEMNLElasticMaterials(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Dcurve' in fieldNames):
            cmds.append("var_Dcurve=%s")
            args.append(fields.mDcurve)
        if (len(fieldNames)) == 0 or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNLElasticMaterials WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMPlate object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMPlate object fields
    def addFEMPlate(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMPlate '\
            '(var_Description,var_ElementID,var_Flex11,var_Flex12,var_Flex22,ref_Group,var_MaterialID,var_Meps11,var_Meps12,var_Meps22,ref_N1,ref_N2,ref_N3,var_RecordNmb,var_Save,var_TBirth,var_TDeath,var_Thick) VALUES '\
            "(%s,%s,%0.16g,%0.16g,%0.16g,%s,%s,%0.16g,%0.16g,%0.16g,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        args.append(fields.mFlex11)
        args.append(fields.mFlex12)
        args.append(fields.mFlex22)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mMaterialID)
        args.append(fields.mMeps11)
        args.append(fields.mMeps12)
        args.append(fields.mMeps22)
        if fields.mN1 == 0:
            args.append(None)
        else:
            args.append(fields.mN1)
        if fields.mN2 == 0:
            args.append(None)
        else:
            args.append(fields.mN2)
        if fields.mN3 == 0:
            args.append(None)
        else:
            args.append(fields.mN3)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        args.append(fields.mThick)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMPlate_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMPlate objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMPlate object fields
    def addFEMPlateList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMPlate '\
            '(var_Description,var_ElementID,var_Flex11,var_Flex12,var_Flex22,ref_Group,var_MaterialID,var_Meps11,var_Meps12,var_Meps22,ref_N1,ref_N2,ref_N3,var_RecordNmb,var_Save,var_TBirth,var_TDeath,var_Thick) VALUES '\
            "(%s,%s,%0.16g,%0.16g,%0.16g,%s,%s,%0.16g,%0.16g,%0.16g,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            args.append(fields.mFlex11)
            args.append(fields.mFlex12)
            args.append(fields.mFlex22)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mMaterialID)
            args.append(fields.mMeps11)
            args.append(fields.mMeps12)
            args.append(fields.mMeps22)
            if fields.mN1 == 0:
                args.append(None)
            else:
                args.append(fields.mN1)
            if fields.mN2 == 0:
                args.append(None)
            else:
                args.append(fields.mN2)
            if fields.mN3 == 0:
                args.append(None)
            else:
                args.append(fields.mN3)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            args.append(fields.mThick)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMPlate_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMPlate object in the database.
    #
    # @param fields     FEMPlate object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMPlate(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Flex11' in fieldNames):
            cmds.append("var_Flex11=%0.16g")
            args.append(fields.mFlex11)
        if allFields or ('Flex12' in fieldNames):
            cmds.append("var_Flex12=%0.16g")
            args.append(fields.mFlex12)
        if allFields or ('Flex22' in fieldNames):
            cmds.append("var_Flex22=%0.16g")
            args.append(fields.mFlex22)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('Meps11' in fieldNames):
            cmds.append("var_Meps11=%0.16g")
            args.append(fields.mMeps11)
        if allFields or ('Meps12' in fieldNames):
            cmds.append("var_Meps12=%0.16g")
            args.append(fields.mMeps12)
        if allFields or ('Meps22' in fieldNames):
            cmds.append("var_Meps22=%0.16g")
            args.append(fields.mMeps22)
        if allFields or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if allFields or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if allFields or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if allFields or ('Thick' in fieldNames):
            cmds.append("var_Thick=%0.16g")
            args.append(fields.mThick)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMPlate SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMPlate object IDs from the database.
    def getFEMPlateIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlate'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMPlate object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMPlate object fields
    # @return \c True on success, \c False if object not found
    def getFEMPlate(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_ElementID,var_Flex11,var_Flex12,var_Flex22,ref_Group,var_MaterialID,var_Meps11,var_Meps12,var_Meps22,ref_N1,ref_N2,ref_N3,var_RecordNmb,var_Save,var_TBirth,var_TDeath,var_Thick '\
            'FROM CLASS_FEMPlate WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mFlex11 = data.pop(0)
        fields.mFlex12 = data.pop(0)
        fields.mFlex22 = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterialID = data.pop(0)
        fields.mMeps11 = data.pop(0)
        fields.mMeps12 = data.pop(0)
        fields.mMeps22 = data.pop(0)
        fields.mN1 = zeroNone(data.pop(0))
        fields.mN2 = zeroNone(data.pop(0))
        fields.mN3 = zeroNone(data.pop(0))
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        fields.mThick = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMPlate objects matching the given
    # reference data.
    #
    # @param fields FEMPlate object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMPlate objects
    def findEqualFEMPlate(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Flex11' in fieldNames):
            cmds.append("var_Flex11=%0.16g")
            args.append(fields.mFlex11)
        if (len(fieldNames)) == 0 or ('Flex12' in fieldNames):
            cmds.append("var_Flex12=%0.16g")
            args.append(fields.mFlex12)
        if (len(fieldNames)) == 0 or ('Flex22' in fieldNames):
            cmds.append("var_Flex22=%0.16g")
            args.append(fields.mFlex22)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('Meps11' in fieldNames):
            cmds.append("var_Meps11=%0.16g")
            args.append(fields.mMeps11)
        if (len(fieldNames)) == 0 or ('Meps12' in fieldNames):
            cmds.append("var_Meps12=%0.16g")
            args.append(fields.mMeps12)
        if (len(fieldNames)) == 0 or ('Meps22' in fieldNames):
            cmds.append("var_Meps22=%0.16g")
            args.append(fields.mMeps22)
        if (len(fieldNames)) == 0 or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if (len(fieldNames)) == 0 or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if (len(fieldNames)) == 0 or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if (len(fieldNames)) == 0 or ('Thick' in fieldNames):
            cmds.append("var_Thick=%0.16g")
            args.append(fields.mThick)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlate WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMIsoBeam object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMIsoBeam object fields
    def addFEMIsoBeam(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMIsoBeam '\
            '(ref_AUX,var_Description,var_ElementID,var_Epaxl,var_Ephoop,ref_Group,var_MaterialID,ref_N1,ref_N2,ref_N3,ref_N4,var_NodeAmount,var_RecordNmb,var_Save,var_SectionID,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g)"
        args = []
        if fields.mAUX == 0:
            args.append(None)
        else:
            args.append(fields.mAUX)
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        args.append(fields.mEpaxl)
        args.append(fields.mEphoop)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mMaterialID)
        if fields.mN1 == 0:
            args.append(None)
        else:
            args.append(fields.mN1)
        if fields.mN2 == 0:
            args.append(None)
        else:
            args.append(fields.mN2)
        if fields.mN3 == 0:
            args.append(None)
        else:
            args.append(fields.mN3)
        if fields.mN4 == 0:
            args.append(None)
        else:
            args.append(fields.mN4)
        args.append(fields.mNodeAmount)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        args.append(fields.mSectionID)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMIsoBeam_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMIsoBeam objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMIsoBeam object fields
    def addFEMIsoBeamList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMIsoBeam '\
            '(ref_AUX,var_Description,var_ElementID,var_Epaxl,var_Ephoop,ref_Group,var_MaterialID,ref_N1,ref_N2,ref_N3,ref_N4,var_NodeAmount,var_RecordNmb,var_Save,var_SectionID,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mAUX == 0:
                args.append(None)
            else:
                args.append(fields.mAUX)
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            args.append(fields.mEpaxl)
            args.append(fields.mEphoop)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mMaterialID)
            if fields.mN1 == 0:
                args.append(None)
            else:
                args.append(fields.mN1)
            if fields.mN2 == 0:
                args.append(None)
            else:
                args.append(fields.mN2)
            if fields.mN3 == 0:
                args.append(None)
            else:
                args.append(fields.mN3)
            if fields.mN4 == 0:
                args.append(None)
            else:
                args.append(fields.mN4)
            args.append(fields.mNodeAmount)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            args.append(fields.mSectionID)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMIsoBeam_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMIsoBeam object in the database.
    #
    # @param fields     FEMIsoBeam object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMIsoBeam(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AUX' in fieldNames):
            cmds.append("ref_AUX=%s")
            args.append(fields.mAUX)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Epaxl' in fieldNames):
            cmds.append("var_Epaxl=%0.16g")
            args.append(fields.mEpaxl)
        if allFields or ('Ephoop' in fieldNames):
            cmds.append("var_Ephoop=%0.16g")
            args.append(fields.mEphoop)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if allFields or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if allFields or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if allFields or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if allFields or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMIsoBeam SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMIsoBeam object IDs from the database.
    def getFEMIsoBeamIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMIsoBeam'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMIsoBeam object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMIsoBeam object fields
    # @return \c True on success, \c False if object not found
    def getFEMIsoBeam(self, fields):
        selectCmd = 'SELECT '\
            'ref_AUX,var_Description,var_ElementID,var_Epaxl,var_Ephoop,ref_Group,var_MaterialID,ref_N1,ref_N2,ref_N3,ref_N4,var_NodeAmount,var_RecordNmb,var_Save,var_SectionID,var_TBirth,var_TDeath '\
            'FROM CLASS_FEMIsoBeam WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAUX = zeroNone(data.pop(0))
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mEpaxl = data.pop(0)
        fields.mEphoop = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterialID = data.pop(0)
        fields.mN1 = zeroNone(data.pop(0))
        fields.mN2 = zeroNone(data.pop(0))
        fields.mN3 = zeroNone(data.pop(0))
        fields.mN4 = zeroNone(data.pop(0))
        fields.mNodeAmount = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMIsoBeam objects matching the given
    # reference data.
    #
    # @param fields FEMIsoBeam object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMIsoBeam objects
    def findEqualFEMIsoBeam(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AUX' in fieldNames):
            cmds.append("ref_AUX=%s")
            args.append(fields.mAUX)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Epaxl' in fieldNames):
            cmds.append("var_Epaxl=%0.16g")
            args.append(fields.mEpaxl)
        if (len(fieldNames)) == 0 or ('Ephoop' in fieldNames):
            cmds.append("var_Ephoop=%0.16g")
            args.append(fields.mEphoop)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if (len(fieldNames)) == 0 or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if (len(fieldNames)) == 0 or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if (len(fieldNames)) == 0 or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if (len(fieldNames)) == 0 or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMIsoBeam WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMAppliedConcentratedLoad object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMAppliedConcentratedLoad object fields
    def addFEMAppliedConcentratedLoad(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMAppliedConcentratedLoad '\
            '(var_ArrivalTime,var_Description,var_Direction,var_Factor,ref_NodeAux,ref_NodeID,var_RecordNmb,var_TimeFunctionID) VALUES '\
            "(%0.16g,%s,%s,%0.16g,%s,%s,%s,%s)"
        args = []
        args.append(fields.mArrivalTime)
        args.append(fields.mDescription)
        args.append(fields.mDirection)
        args.append(fields.mFactor)
        if fields.mNodeAux == 0:
            args.append(None)
        else:
            args.append(fields.mNodeAux)
        if fields.mNodeID == 0:
            args.append(None)
        else:
            args.append(fields.mNodeID)
        args.append(fields.mRecordNmb)
        args.append(fields.mTimeFunctionID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMAppliedConcentratedLoad_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMAppliedConcentratedLoad objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMAppliedConcentratedLoad object fields
    def addFEMAppliedConcentratedLoadList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMAppliedConcentratedLoad '\
            '(var_ArrivalTime,var_Description,var_Direction,var_Factor,ref_NodeAux,ref_NodeID,var_RecordNmb,var_TimeFunctionID) VALUES '\
            "(%0.16g,%s,%s,%0.16g,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mArrivalTime)
            args.append(fields.mDescription)
            args.append(fields.mDirection)
            args.append(fields.mFactor)
            if fields.mNodeAux == 0:
                args.append(None)
            else:
                args.append(fields.mNodeAux)
            if fields.mNodeID == 0:
                args.append(None)
            else:
                args.append(fields.mNodeID)
            args.append(fields.mRecordNmb)
            args.append(fields.mTimeFunctionID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMAppliedConcentratedLoad_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMAppliedConcentratedLoad object in the database.
    #
    # @param fields     FEMAppliedConcentratedLoad object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMAppliedConcentratedLoad(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Direction' in fieldNames):
            cmds.append("var_Direction=%s")
            args.append(fields.mDirection)
        if allFields or ('Factor' in fieldNames):
            cmds.append("var_Factor=%0.16g")
            args.append(fields.mFactor)
        if allFields or ('NodeAux' in fieldNames):
            cmds.append("ref_NodeAux=%s")
            args.append(fields.mNodeAux)
        if allFields or ('NodeID' in fieldNames):
            cmds.append("ref_NodeID=%s")
            args.append(fields.mNodeID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMAppliedConcentratedLoad SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMAppliedConcentratedLoad object IDs from the database.
    def getFEMAppliedConcentratedLoadIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAppliedConcentratedLoad'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMAppliedConcentratedLoad object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMAppliedConcentratedLoad object fields
    # @return \c True on success, \c False if object not found
    def getFEMAppliedConcentratedLoad(self, fields):
        selectCmd = 'SELECT '\
            'var_ArrivalTime,var_Description,var_Direction,var_Factor,ref_NodeAux,ref_NodeID,var_RecordNmb,var_TimeFunctionID '\
            'FROM CLASS_FEMAppliedConcentratedLoad WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mArrivalTime = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mDirection = data.pop(0)
        fields.mFactor = data.pop(0)
        fields.mNodeAux = zeroNone(data.pop(0))
        fields.mNodeID = zeroNone(data.pop(0))
        fields.mRecordNmb = data.pop(0)
        fields.mTimeFunctionID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMAppliedConcentratedLoad objects matching the given
    # reference data.
    #
    # @param fields FEMAppliedConcentratedLoad object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMAppliedConcentratedLoad objects
    def findEqualFEMAppliedConcentratedLoad(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Direction' in fieldNames):
            cmds.append("var_Direction=%s")
            args.append(fields.mDirection)
        if (len(fieldNames)) == 0 or ('Factor' in fieldNames):
            cmds.append("var_Factor=%0.16g")
            args.append(fields.mFactor)
        if (len(fieldNames)) == 0 or ('NodeAux' in fieldNames):
            cmds.append("ref_NodeAux=%s")
            args.append(fields.mNodeAux)
        if (len(fieldNames)) == 0 or ('NodeID' in fieldNames):
            cmds.append("ref_NodeID=%s")
            args.append(fields.mNodeID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAppliedConcentratedLoad WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMTwoDSolidGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMTwoDSolidGroup object fields
    def addFEMTwoDSolidGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMTwoDSolidGroup '\
            '(ref_AuxNode,var_Displacement,ref_Group,var_MaterialID,var_Result,var_Subtype) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        args = []
        if fields.mAuxNode == 0:
            args.append(None)
        else:
            args.append(fields.mAuxNode)
        args.append(fields.mDisplacement)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mMaterialID)
        args.append(fields.mResult)
        args.append(fields.mSubtype)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMTwoDSolidGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMTwoDSolidGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMTwoDSolidGroup object fields
    def addFEMTwoDSolidGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMTwoDSolidGroup '\
            '(ref_AuxNode,var_Displacement,ref_Group,var_MaterialID,var_Result,var_Subtype) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mAuxNode == 0:
                args.append(None)
            else:
                args.append(fields.mAuxNode)
            args.append(fields.mDisplacement)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mMaterialID)
            args.append(fields.mResult)
            args.append(fields.mSubtype)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMTwoDSolidGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMTwoDSolidGroup object in the database.
    #
    # @param fields     FEMTwoDSolidGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMTwoDSolidGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AuxNode' in fieldNames):
            cmds.append("ref_AuxNode=%s")
            args.append(fields.mAuxNode)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if allFields or ('Subtype' in fieldNames):
            cmds.append("var_Subtype=%s")
            args.append(fields.mSubtype)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMTwoDSolidGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMTwoDSolidGroup object IDs from the database.
    def getFEMTwoDSolidGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTwoDSolidGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMTwoDSolidGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMTwoDSolidGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMTwoDSolidGroup(self, fields):
        selectCmd = 'SELECT '\
            'ref_AuxNode,var_Displacement,ref_Group,var_MaterialID,var_Result,var_Subtype '\
            'FROM CLASS_FEMTwoDSolidGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAuxNode = zeroNone(data.pop(0))
        fields.mDisplacement = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterialID = data.pop(0)
        fields.mResult = data.pop(0)
        fields.mSubtype = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMTwoDSolidGroup objects matching the given
    # reference data.
    #
    # @param fields FEMTwoDSolidGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMTwoDSolidGroup objects
    def findEqualFEMTwoDSolidGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AuxNode' in fieldNames):
            cmds.append("ref_AuxNode=%s")
            args.append(fields.mAuxNode)
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if (len(fieldNames)) == 0 or ('Subtype' in fieldNames):
            cmds.append("var_Subtype=%s")
            args.append(fields.mSubtype)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTwoDSolidGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMGroup object fields
    def addFEMGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMGroup '\
            '(var_Description,ref_GroupType,var_LocalID,ref_Structure) VALUES '\
            "(%s,%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(self.GroupTypeLabelToId[str(fields.mGroupType)])
        args.append(fields.mLocalID)
        if fields.mStructure == 0:
            args.append(None)
        else:
            args.append(fields.mStructure)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMGroup object fields
    def addFEMGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMGroup '\
            '(var_Description,ref_GroupType,var_LocalID,ref_Structure) VALUES '\
            "(%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(self.GroupTypeLabelToId[str(fields.mGroupType)])
            args.append(fields.mLocalID)
            if fields.mStructure == 0:
                args.append(None)
            else:
                args.append(fields.mStructure)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMGroup object in the database.
    #
    # @param fields     FEMGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('GroupType' in fieldNames):
            cmds.append("ref_GroupType=%s")
            args.append(self.GroupTypeLabelToId[str(fields.mGroupType)])
        if allFields or ('LocalID' in fieldNames):
            cmds.append("var_LocalID=%s")
            args.append(fields.mLocalID)
        if allFields or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMGroup object IDs from the database.
    def getFEMGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMGroup(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,ref_GroupType,var_LocalID,ref_Structure '\
            'FROM CLASS_FEMGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mGroupType = self.GroupTypeIdToEnum[data.pop(0)]
        fields.mLocalID = data.pop(0)
        fields.mStructure = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all FEMGroup objects matching the given
    # reference data.
    #
    # @param fields FEMGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMGroup objects
    def findEqualFEMGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('GroupType' in fieldNames):
            cmds.append("ref_GroupType=%s")
            args.append(self.GroupTypeLabelToId[str(fields.mGroupType)])
        if (len(fieldNames)) == 0 or ('LocalID' in fieldNames):
            cmds.append("var_LocalID=%s")
            args.append(fields.mLocalID)
        if (len(fieldNames)) == 0 or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMProperties object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMProperties object fields
    def addFEMProperties(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMProperties '\
            '(var_Description,var_PropertyID,var_PropertyType,var_Rupture,var_XC,var_XN) VALUES '\
            "(%s,%s,%s,%s,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mPropertyID)
        args.append(fields.mPropertyType)
        args.append(fields.mRupture)
        args.append(fields.mXC)
        args.append(fields.mXN)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMProperties_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMProperties objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMProperties object fields
    def addFEMPropertiesList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMProperties '\
            '(var_Description,var_PropertyID,var_PropertyType,var_Rupture,var_XC,var_XN) VALUES '\
            "(%s,%s,%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mPropertyID)
            args.append(fields.mPropertyType)
            args.append(fields.mRupture)
            args.append(fields.mXC)
            args.append(fields.mXN)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMProperties_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMProperties object in the database.
    #
    # @param fields     FEMProperties object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMProperties(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('PropertyID' in fieldNames):
            cmds.append("var_PropertyID=%s")
            args.append(fields.mPropertyID)
        if allFields or ('PropertyType' in fieldNames):
            cmds.append("var_PropertyType=%s")
            args.append(fields.mPropertyType)
        if allFields or ('Rupture' in fieldNames):
            cmds.append("var_Rupture=%s")
            args.append(fields.mRupture)
        if allFields or ('XC' in fieldNames):
            cmds.append("var_XC=%0.16g")
            args.append(fields.mXC)
        if allFields or ('XN' in fieldNames):
            cmds.append("var_XN=%0.16g")
            args.append(fields.mXN)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMProperties SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMProperties object IDs from the database.
    def getFEMPropertiesIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMProperties'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMProperties object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMProperties object fields
    # @return \c True on success, \c False if object not found
    def getFEMProperties(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_PropertyID,var_PropertyType,var_Rupture,var_XC,var_XN '\
            'FROM CLASS_FEMProperties WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mPropertyID = data.pop(0)
        fields.mPropertyType = data.pop(0)
        fields.mRupture = data.pop(0)
        fields.mXC = data.pop(0)
        fields.mXN = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMProperties objects matching the given
    # reference data.
    #
    # @param fields FEMProperties object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMProperties objects
    def findEqualFEMProperties(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('PropertyID' in fieldNames):
            cmds.append("var_PropertyID=%s")
            args.append(fields.mPropertyID)
        if (len(fieldNames)) == 0 or ('PropertyType' in fieldNames):
            cmds.append("var_PropertyType=%s")
            args.append(fields.mPropertyType)
        if (len(fieldNames)) == 0 or ('Rupture' in fieldNames):
            cmds.append("var_Rupture=%s")
            args.append(fields.mRupture)
        if (len(fieldNames)) == 0 or ('XC' in fieldNames):
            cmds.append("var_XC=%0.16g")
            args.append(fields.mXC)
        if (len(fieldNames)) == 0 or ('XN' in fieldNames):
            cmds.append("var_XN=%0.16g")
            args.append(fields.mXN)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMProperties WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMThreeDSolidGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMThreeDSolidGroup object fields
    def addFEMThreeDSolidGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMThreeDSolidGroup '\
            '(var_Displacement,ref_Group,var_MaterialID,var_Result) VALUES '\
            "(%s,%s,%s,%s)"
        args = []
        args.append(fields.mDisplacement)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mMaterialID)
        args.append(fields.mResult)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMThreeDSolidGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMThreeDSolidGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMThreeDSolidGroup object fields
    def addFEMThreeDSolidGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMThreeDSolidGroup '\
            '(var_Displacement,ref_Group,var_MaterialID,var_Result) VALUES '\
            "(%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDisplacement)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mMaterialID)
            args.append(fields.mResult)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMThreeDSolidGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMThreeDSolidGroup object in the database.
    #
    # @param fields     FEMThreeDSolidGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMThreeDSolidGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMThreeDSolidGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMThreeDSolidGroup object IDs from the database.
    def getFEMThreeDSolidGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThreeDSolidGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMThreeDSolidGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMThreeDSolidGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMThreeDSolidGroup(self, fields):
        selectCmd = 'SELECT '\
            'var_Displacement,ref_Group,var_MaterialID,var_Result '\
            'FROM CLASS_FEMThreeDSolidGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDisplacement = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterialID = data.pop(0)
        fields.mResult = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMThreeDSolidGroup objects matching the given
    # reference data.
    #
    # @param fields FEMThreeDSolidGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMThreeDSolidGroup objects
    def findEqualFEMThreeDSolidGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThreeDSolidGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMThreeDSolid object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMThreeDSolid object fields
    def addFEMThreeDSolid(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMThreeDSolid '\
            '(var_Description,var_ElementID,ref_Group,var_MaterialID,var_Maxes,ref_N1,ref_N10,ref_N11,ref_N12,ref_N13,ref_N14,ref_N15,ref_N16,ref_N17,ref_N18,ref_N19,ref_N2,ref_N20,ref_N21,ref_N22,ref_N23,ref_N24,ref_N25,ref_N26,ref_N27,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mMaterialID)
        args.append(fields.mMaxes)
        if fields.mN1 == 0:
            args.append(None)
        else:
            args.append(fields.mN1)
        if fields.mN10 == 0:
            args.append(None)
        else:
            args.append(fields.mN10)
        if fields.mN11 == 0:
            args.append(None)
        else:
            args.append(fields.mN11)
        if fields.mN12 == 0:
            args.append(None)
        else:
            args.append(fields.mN12)
        if fields.mN13 == 0:
            args.append(None)
        else:
            args.append(fields.mN13)
        if fields.mN14 == 0:
            args.append(None)
        else:
            args.append(fields.mN14)
        if fields.mN15 == 0:
            args.append(None)
        else:
            args.append(fields.mN15)
        if fields.mN16 == 0:
            args.append(None)
        else:
            args.append(fields.mN16)
        if fields.mN17 == 0:
            args.append(None)
        else:
            args.append(fields.mN17)
        if fields.mN18 == 0:
            args.append(None)
        else:
            args.append(fields.mN18)
        if fields.mN19 == 0:
            args.append(None)
        else:
            args.append(fields.mN19)
        if fields.mN2 == 0:
            args.append(None)
        else:
            args.append(fields.mN2)
        if fields.mN20 == 0:
            args.append(None)
        else:
            args.append(fields.mN20)
        if fields.mN21 == 0:
            args.append(None)
        else:
            args.append(fields.mN21)
        if fields.mN22 == 0:
            args.append(None)
        else:
            args.append(fields.mN22)
        if fields.mN23 == 0:
            args.append(None)
        else:
            args.append(fields.mN23)
        if fields.mN24 == 0:
            args.append(None)
        else:
            args.append(fields.mN24)
        if fields.mN25 == 0:
            args.append(None)
        else:
            args.append(fields.mN25)
        if fields.mN26 == 0:
            args.append(None)
        else:
            args.append(fields.mN26)
        if fields.mN27 == 0:
            args.append(None)
        else:
            args.append(fields.mN27)
        if fields.mN3 == 0:
            args.append(None)
        else:
            args.append(fields.mN3)
        if fields.mN4 == 0:
            args.append(None)
        else:
            args.append(fields.mN4)
        if fields.mN5 == 0:
            args.append(None)
        else:
            args.append(fields.mN5)
        if fields.mN6 == 0:
            args.append(None)
        else:
            args.append(fields.mN6)
        if fields.mN7 == 0:
            args.append(None)
        else:
            args.append(fields.mN7)
        if fields.mN8 == 0:
            args.append(None)
        else:
            args.append(fields.mN8)
        if fields.mN9 == 0:
            args.append(None)
        else:
            args.append(fields.mN9)
        args.append(fields.mNodeAmount)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMThreeDSolid_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMThreeDSolid objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMThreeDSolid object fields
    def addFEMThreeDSolidList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMThreeDSolid '\
            '(var_Description,var_ElementID,ref_Group,var_MaterialID,var_Maxes,ref_N1,ref_N10,ref_N11,ref_N12,ref_N13,ref_N14,ref_N15,ref_N16,ref_N17,ref_N18,ref_N19,ref_N2,ref_N20,ref_N21,ref_N22,ref_N23,ref_N24,ref_N25,ref_N26,ref_N27,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mMaterialID)
            args.append(fields.mMaxes)
            if fields.mN1 == 0:
                args.append(None)
            else:
                args.append(fields.mN1)
            if fields.mN10 == 0:
                args.append(None)
            else:
                args.append(fields.mN10)
            if fields.mN11 == 0:
                args.append(None)
            else:
                args.append(fields.mN11)
            if fields.mN12 == 0:
                args.append(None)
            else:
                args.append(fields.mN12)
            if fields.mN13 == 0:
                args.append(None)
            else:
                args.append(fields.mN13)
            if fields.mN14 == 0:
                args.append(None)
            else:
                args.append(fields.mN14)
            if fields.mN15 == 0:
                args.append(None)
            else:
                args.append(fields.mN15)
            if fields.mN16 == 0:
                args.append(None)
            else:
                args.append(fields.mN16)
            if fields.mN17 == 0:
                args.append(None)
            else:
                args.append(fields.mN17)
            if fields.mN18 == 0:
                args.append(None)
            else:
                args.append(fields.mN18)
            if fields.mN19 == 0:
                args.append(None)
            else:
                args.append(fields.mN19)
            if fields.mN2 == 0:
                args.append(None)
            else:
                args.append(fields.mN2)
            if fields.mN20 == 0:
                args.append(None)
            else:
                args.append(fields.mN20)
            if fields.mN21 == 0:
                args.append(None)
            else:
                args.append(fields.mN21)
            if fields.mN22 == 0:
                args.append(None)
            else:
                args.append(fields.mN22)
            if fields.mN23 == 0:
                args.append(None)
            else:
                args.append(fields.mN23)
            if fields.mN24 == 0:
                args.append(None)
            else:
                args.append(fields.mN24)
            if fields.mN25 == 0:
                args.append(None)
            else:
                args.append(fields.mN25)
            if fields.mN26 == 0:
                args.append(None)
            else:
                args.append(fields.mN26)
            if fields.mN27 == 0:
                args.append(None)
            else:
                args.append(fields.mN27)
            if fields.mN3 == 0:
                args.append(None)
            else:
                args.append(fields.mN3)
            if fields.mN4 == 0:
                args.append(None)
            else:
                args.append(fields.mN4)
            if fields.mN5 == 0:
                args.append(None)
            else:
                args.append(fields.mN5)
            if fields.mN6 == 0:
                args.append(None)
            else:
                args.append(fields.mN6)
            if fields.mN7 == 0:
                args.append(None)
            else:
                args.append(fields.mN7)
            if fields.mN8 == 0:
                args.append(None)
            else:
                args.append(fields.mN8)
            if fields.mN9 == 0:
                args.append(None)
            else:
                args.append(fields.mN9)
            args.append(fields.mNodeAmount)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMThreeDSolid_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMThreeDSolid object in the database.
    #
    # @param fields     FEMThreeDSolid object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMThreeDSolid(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('Maxes' in fieldNames):
            cmds.append("var_Maxes=%s")
            args.append(fields.mMaxes)
        if allFields or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if allFields or ('N10' in fieldNames):
            cmds.append("ref_N10=%s")
            args.append(fields.mN10)
        if allFields or ('N11' in fieldNames):
            cmds.append("ref_N11=%s")
            args.append(fields.mN11)
        if allFields or ('N12' in fieldNames):
            cmds.append("ref_N12=%s")
            args.append(fields.mN12)
        if allFields or ('N13' in fieldNames):
            cmds.append("ref_N13=%s")
            args.append(fields.mN13)
        if allFields or ('N14' in fieldNames):
            cmds.append("ref_N14=%s")
            args.append(fields.mN14)
        if allFields or ('N15' in fieldNames):
            cmds.append("ref_N15=%s")
            args.append(fields.mN15)
        if allFields or ('N16' in fieldNames):
            cmds.append("ref_N16=%s")
            args.append(fields.mN16)
        if allFields or ('N17' in fieldNames):
            cmds.append("ref_N17=%s")
            args.append(fields.mN17)
        if allFields or ('N18' in fieldNames):
            cmds.append("ref_N18=%s")
            args.append(fields.mN18)
        if allFields or ('N19' in fieldNames):
            cmds.append("ref_N19=%s")
            args.append(fields.mN19)
        if allFields or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if allFields or ('N20' in fieldNames):
            cmds.append("ref_N20=%s")
            args.append(fields.mN20)
        if allFields or ('N21' in fieldNames):
            cmds.append("ref_N21=%s")
            args.append(fields.mN21)
        if allFields or ('N22' in fieldNames):
            cmds.append("ref_N22=%s")
            args.append(fields.mN22)
        if allFields or ('N23' in fieldNames):
            cmds.append("ref_N23=%s")
            args.append(fields.mN23)
        if allFields or ('N24' in fieldNames):
            cmds.append("ref_N24=%s")
            args.append(fields.mN24)
        if allFields or ('N25' in fieldNames):
            cmds.append("ref_N25=%s")
            args.append(fields.mN25)
        if allFields or ('N26' in fieldNames):
            cmds.append("ref_N26=%s")
            args.append(fields.mN26)
        if allFields or ('N27' in fieldNames):
            cmds.append("ref_N27=%s")
            args.append(fields.mN27)
        if allFields or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if allFields or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if allFields or ('N5' in fieldNames):
            cmds.append("ref_N5=%s")
            args.append(fields.mN5)
        if allFields or ('N6' in fieldNames):
            cmds.append("ref_N6=%s")
            args.append(fields.mN6)
        if allFields or ('N7' in fieldNames):
            cmds.append("ref_N7=%s")
            args.append(fields.mN7)
        if allFields or ('N8' in fieldNames):
            cmds.append("ref_N8=%s")
            args.append(fields.mN8)
        if allFields or ('N9' in fieldNames):
            cmds.append("ref_N9=%s")
            args.append(fields.mN9)
        if allFields or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMThreeDSolid SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMThreeDSolid object IDs from the database.
    def getFEMThreeDSolidIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThreeDSolid'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMThreeDSolid object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMThreeDSolid object fields
    # @return \c True on success, \c False if object not found
    def getFEMThreeDSolid(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_ElementID,ref_Group,var_MaterialID,var_Maxes,ref_N1,ref_N10,ref_N11,ref_N12,ref_N13,ref_N14,ref_N15,ref_N16,ref_N17,ref_N18,ref_N19,ref_N2,ref_N20,ref_N21,ref_N22,ref_N23,ref_N24,ref_N25,ref_N26,ref_N27,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_TBirth,var_TDeath '\
            'FROM CLASS_FEMThreeDSolid WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterialID = data.pop(0)
        fields.mMaxes = data.pop(0)
        fields.mN1 = zeroNone(data.pop(0))
        fields.mN10 = zeroNone(data.pop(0))
        fields.mN11 = zeroNone(data.pop(0))
        fields.mN12 = zeroNone(data.pop(0))
        fields.mN13 = zeroNone(data.pop(0))
        fields.mN14 = zeroNone(data.pop(0))
        fields.mN15 = zeroNone(data.pop(0))
        fields.mN16 = zeroNone(data.pop(0))
        fields.mN17 = zeroNone(data.pop(0))
        fields.mN18 = zeroNone(data.pop(0))
        fields.mN19 = zeroNone(data.pop(0))
        fields.mN2 = zeroNone(data.pop(0))
        fields.mN20 = zeroNone(data.pop(0))
        fields.mN21 = zeroNone(data.pop(0))
        fields.mN22 = zeroNone(data.pop(0))
        fields.mN23 = zeroNone(data.pop(0))
        fields.mN24 = zeroNone(data.pop(0))
        fields.mN25 = zeroNone(data.pop(0))
        fields.mN26 = zeroNone(data.pop(0))
        fields.mN27 = zeroNone(data.pop(0))
        fields.mN3 = zeroNone(data.pop(0))
        fields.mN4 = zeroNone(data.pop(0))
        fields.mN5 = zeroNone(data.pop(0))
        fields.mN6 = zeroNone(data.pop(0))
        fields.mN7 = zeroNone(data.pop(0))
        fields.mN8 = zeroNone(data.pop(0))
        fields.mN9 = zeroNone(data.pop(0))
        fields.mNodeAmount = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMThreeDSolid objects matching the given
    # reference data.
    #
    # @param fields FEMThreeDSolid object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMThreeDSolid objects
    def findEqualFEMThreeDSolid(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('Maxes' in fieldNames):
            cmds.append("var_Maxes=%s")
            args.append(fields.mMaxes)
        if (len(fieldNames)) == 0 or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if (len(fieldNames)) == 0 or ('N10' in fieldNames):
            cmds.append("ref_N10=%s")
            args.append(fields.mN10)
        if (len(fieldNames)) == 0 or ('N11' in fieldNames):
            cmds.append("ref_N11=%s")
            args.append(fields.mN11)
        if (len(fieldNames)) == 0 or ('N12' in fieldNames):
            cmds.append("ref_N12=%s")
            args.append(fields.mN12)
        if (len(fieldNames)) == 0 or ('N13' in fieldNames):
            cmds.append("ref_N13=%s")
            args.append(fields.mN13)
        if (len(fieldNames)) == 0 or ('N14' in fieldNames):
            cmds.append("ref_N14=%s")
            args.append(fields.mN14)
        if (len(fieldNames)) == 0 or ('N15' in fieldNames):
            cmds.append("ref_N15=%s")
            args.append(fields.mN15)
        if (len(fieldNames)) == 0 or ('N16' in fieldNames):
            cmds.append("ref_N16=%s")
            args.append(fields.mN16)
        if (len(fieldNames)) == 0 or ('N17' in fieldNames):
            cmds.append("ref_N17=%s")
            args.append(fields.mN17)
        if (len(fieldNames)) == 0 or ('N18' in fieldNames):
            cmds.append("ref_N18=%s")
            args.append(fields.mN18)
        if (len(fieldNames)) == 0 or ('N19' in fieldNames):
            cmds.append("ref_N19=%s")
            args.append(fields.mN19)
        if (len(fieldNames)) == 0 or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if (len(fieldNames)) == 0 or ('N20' in fieldNames):
            cmds.append("ref_N20=%s")
            args.append(fields.mN20)
        if (len(fieldNames)) == 0 or ('N21' in fieldNames):
            cmds.append("ref_N21=%s")
            args.append(fields.mN21)
        if (len(fieldNames)) == 0 or ('N22' in fieldNames):
            cmds.append("ref_N22=%s")
            args.append(fields.mN22)
        if (len(fieldNames)) == 0 or ('N23' in fieldNames):
            cmds.append("ref_N23=%s")
            args.append(fields.mN23)
        if (len(fieldNames)) == 0 or ('N24' in fieldNames):
            cmds.append("ref_N24=%s")
            args.append(fields.mN24)
        if (len(fieldNames)) == 0 or ('N25' in fieldNames):
            cmds.append("ref_N25=%s")
            args.append(fields.mN25)
        if (len(fieldNames)) == 0 or ('N26' in fieldNames):
            cmds.append("ref_N26=%s")
            args.append(fields.mN26)
        if (len(fieldNames)) == 0 or ('N27' in fieldNames):
            cmds.append("ref_N27=%s")
            args.append(fields.mN27)
        if (len(fieldNames)) == 0 or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if (len(fieldNames)) == 0 or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if (len(fieldNames)) == 0 or ('N5' in fieldNames):
            cmds.append("ref_N5=%s")
            args.append(fields.mN5)
        if (len(fieldNames)) == 0 or ('N6' in fieldNames):
            cmds.append("ref_N6=%s")
            args.append(fields.mN6)
        if (len(fieldNames)) == 0 or ('N7' in fieldNames):
            cmds.append("ref_N7=%s")
            args.append(fields.mN7)
        if (len(fieldNames)) == 0 or ('N8' in fieldNames):
            cmds.append("ref_N8=%s")
            args.append(fields.mN8)
        if (len(fieldNames)) == 0 or ('N9' in fieldNames):
            cmds.append("ref_N9=%s")
            args.append(fields.mN9)
        if (len(fieldNames)) == 0 or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThreeDSolid WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSectionProp object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSectionProp object fields
    def addFEMSectionProp(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSectionProp '\
            '(var_Area,var_Rinertia,var_Sarea,var_SectionID,var_Sinertia,var_Tarea,var_Tinertia) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mArea)
        args.append(fields.mRinertia)
        args.append(fields.mSarea)
        args.append(fields.mSectionID)
        args.append(fields.mSinertia)
        args.append(fields.mTarea)
        args.append(fields.mTinertia)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSectionProp_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSectionProp objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSectionProp object fields
    def addFEMSectionPropList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSectionProp '\
            '(var_Area,var_Rinertia,var_Sarea,var_SectionID,var_Sinertia,var_Tarea,var_Tinertia) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mArea)
            args.append(fields.mRinertia)
            args.append(fields.mSarea)
            args.append(fields.mSectionID)
            args.append(fields.mSinertia)
            args.append(fields.mTarea)
            args.append(fields.mTinertia)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSectionProp_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSectionProp object in the database.
    #
    # @param fields     FEMSectionProp object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSectionProp(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Area' in fieldNames):
            cmds.append("var_Area=%0.16g")
            args.append(fields.mArea)
        if allFields or ('Rinertia' in fieldNames):
            cmds.append("var_Rinertia=%0.16g")
            args.append(fields.mRinertia)
        if allFields or ('Sarea' in fieldNames):
            cmds.append("var_Sarea=%0.16g")
            args.append(fields.mSarea)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('Sinertia' in fieldNames):
            cmds.append("var_Sinertia=%0.16g")
            args.append(fields.mSinertia)
        if allFields or ('Tarea' in fieldNames):
            cmds.append("var_Tarea=%0.16g")
            args.append(fields.mTarea)
        if allFields or ('Tinertia' in fieldNames):
            cmds.append("var_Tinertia=%0.16g")
            args.append(fields.mTinertia)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSectionProp SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSectionProp object IDs from the database.
    def getFEMSectionPropIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionProp'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSectionProp object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSectionProp object fields
    # @return \c True on success, \c False if object not found
    def getFEMSectionProp(self, fields):
        selectCmd = 'SELECT '\
            'var_Area,var_Rinertia,var_Sarea,var_SectionID,var_Sinertia,var_Tarea,var_Tinertia '\
            'FROM CLASS_FEMSectionProp WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mArea = data.pop(0)
        fields.mRinertia = data.pop(0)
        fields.mSarea = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mSinertia = data.pop(0)
        fields.mTarea = data.pop(0)
        fields.mTinertia = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSectionProp objects matching the given
    # reference data.
    #
    # @param fields FEMSectionProp object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSectionProp objects
    def findEqualFEMSectionProp(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Area' in fieldNames):
            cmds.append("var_Area=%0.16g")
            args.append(fields.mArea)
        if (len(fieldNames)) == 0 or ('Rinertia' in fieldNames):
            cmds.append("var_Rinertia=%0.16g")
            args.append(fields.mRinertia)
        if (len(fieldNames)) == 0 or ('Sarea' in fieldNames):
            cmds.append("var_Sarea=%0.16g")
            args.append(fields.mSarea)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('Sinertia' in fieldNames):
            cmds.append("var_Sinertia=%0.16g")
            args.append(fields.mSinertia)
        if (len(fieldNames)) == 0 or ('Tarea' in fieldNames):
            cmds.append("var_Tarea=%0.16g")
            args.append(fields.mTarea)
        if (len(fieldNames)) == 0 or ('Tinertia' in fieldNames):
            cmds.append("var_Tinertia=%0.16g")
            args.append(fields.mTinertia)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionProp WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMElasticMaterial object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMElasticMaterial object fields
    def addFEMElasticMaterial(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMElasticMaterial '\
            '(var_Alpha,var_Density,var_E,ref_Material,var_NU) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%0.16g)"
        args = []
        args.append(fields.mAlpha)
        args.append(fields.mDensity)
        args.append(fields.mE)
        if fields.mMaterial == 0:
            args.append(None)
        else:
            args.append(fields.mMaterial)
        args.append(fields.mNU)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMElasticMaterial_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMElasticMaterial objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMElasticMaterial object fields
    def addFEMElasticMaterialList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMElasticMaterial '\
            '(var_Alpha,var_Density,var_E,ref_Material,var_NU) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAlpha)
            args.append(fields.mDensity)
            args.append(fields.mE)
            if fields.mMaterial == 0:
                args.append(None)
            else:
                args.append(fields.mMaterial)
            args.append(fields.mNU)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMElasticMaterial_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMElasticMaterial object in the database.
    #
    # @param fields     FEMElasticMaterial object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMElasticMaterial(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Alpha' in fieldNames):
            cmds.append("var_Alpha=%0.16g")
            args.append(fields.mAlpha)
        if allFields or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if allFields or ('E' in fieldNames):
            cmds.append("var_E=%0.16g")
            args.append(fields.mE)
        if allFields or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if allFields or ('NU' in fieldNames):
            cmds.append("var_NU=%0.16g")
            args.append(fields.mNU)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMElasticMaterial SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMElasticMaterial object IDs from the database.
    def getFEMElasticMaterialIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMElasticMaterial'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMElasticMaterial object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMElasticMaterial object fields
    # @return \c True on success, \c False if object not found
    def getFEMElasticMaterial(self, fields):
        selectCmd = 'SELECT '\
            'var_Alpha,var_Density,var_E,ref_Material,var_NU '\
            'FROM CLASS_FEMElasticMaterial WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAlpha = data.pop(0)
        fields.mDensity = data.pop(0)
        fields.mE = data.pop(0)
        fields.mMaterial = zeroNone(data.pop(0))
        fields.mNU = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMElasticMaterial objects matching the given
    # reference data.
    #
    # @param fields FEMElasticMaterial object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMElasticMaterial objects
    def findEqualFEMElasticMaterial(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Alpha' in fieldNames):
            cmds.append("var_Alpha=%0.16g")
            args.append(fields.mAlpha)
        if (len(fieldNames)) == 0 or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if (len(fieldNames)) == 0 or ('E' in fieldNames):
            cmds.append("var_E=%0.16g")
            args.append(fields.mE)
        if (len(fieldNames)) == 0 or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if (len(fieldNames)) == 0 or ('NU' in fieldNames):
            cmds.append("var_NU=%0.16g")
            args.append(fields.mNU)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMElasticMaterial WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMPoints object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMPoints object fields
    def addFEMPoints(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMPoints '\
            '(var_Description,var_PointID,var_SystemID,var_X,var_Y,var_Z) VALUES '\
            "(%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mPointID)
        args.append(fields.mSystemID)
        args.append(fields.mX)
        args.append(fields.mY)
        args.append(fields.mZ)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMPoints_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMPoints objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMPoints object fields
    def addFEMPointsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMPoints '\
            '(var_Description,var_PointID,var_SystemID,var_X,var_Y,var_Z) VALUES '\
            "(%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mPointID)
            args.append(fields.mSystemID)
            args.append(fields.mX)
            args.append(fields.mY)
            args.append(fields.mZ)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMPoints_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMPoints object in the database.
    #
    # @param fields     FEMPoints object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMPoints(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('PointID' in fieldNames):
            cmds.append("var_PointID=%s")
            args.append(fields.mPointID)
        if allFields or ('SystemID' in fieldNames):
            cmds.append("var_SystemID=%s")
            args.append(fields.mSystemID)
        if allFields or ('X' in fieldNames):
            cmds.append("var_X=%0.16g")
            args.append(fields.mX)
        if allFields or ('Y' in fieldNames):
            cmds.append("var_Y=%0.16g")
            args.append(fields.mY)
        if allFields or ('Z' in fieldNames):
            cmds.append("var_Z=%0.16g")
            args.append(fields.mZ)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMPoints SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMPoints object IDs from the database.
    def getFEMPointsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPoints'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMPoints object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMPoints object fields
    # @return \c True on success, \c False if object not found
    def getFEMPoints(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_PointID,var_SystemID,var_X,var_Y,var_Z '\
            'FROM CLASS_FEMPoints WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mPointID = data.pop(0)
        fields.mSystemID = data.pop(0)
        fields.mX = data.pop(0)
        fields.mY = data.pop(0)
        fields.mZ = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMPoints objects matching the given
    # reference data.
    #
    # @param fields FEMPoints object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMPoints objects
    def findEqualFEMPoints(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('PointID' in fieldNames):
            cmds.append("var_PointID=%s")
            args.append(fields.mPointID)
        if (len(fieldNames)) == 0 or ('SystemID' in fieldNames):
            cmds.append("var_SystemID=%s")
            args.append(fields.mSystemID)
        if (len(fieldNames)) == 0 or ('X' in fieldNames):
            cmds.append("var_X=%0.16g")
            args.append(fields.mX)
        if (len(fieldNames)) == 0 or ('Y' in fieldNames):
            cmds.append("var_Y=%0.16g")
            args.append(fields.mY)
        if (len(fieldNames)) == 0 or ('Z' in fieldNames):
            cmds.append("var_Z=%0.16g")
            args.append(fields.mZ)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPoints WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMThermoOrthMaterials object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMThermoOrthMaterials object fields
    def addFEMThermoOrthMaterials(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMThermoOrthMaterials '\
            '(var_Density,var_MaterialID,var_TREF) VALUES '\
            "(%0.16g,%s,%0.16g)"
        args = []
        args.append(fields.mDensity)
        args.append(fields.mMaterialID)
        args.append(fields.mTREF)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMThermoOrthMaterials_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMThermoOrthMaterials objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMThermoOrthMaterials object fields
    def addFEMThermoOrthMaterialsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMThermoOrthMaterials '\
            '(var_Density,var_MaterialID,var_TREF) VALUES '\
            "(%0.16g,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDensity)
            args.append(fields.mMaterialID)
            args.append(fields.mTREF)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMThermoOrthMaterials_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMThermoOrthMaterials object in the database.
    #
    # @param fields     FEMThermoOrthMaterials object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMThermoOrthMaterials(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('TREF' in fieldNames):
            cmds.append("var_TREF=%0.16g")
            args.append(fields.mTREF)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMThermoOrthMaterials SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMThermoOrthMaterials object IDs from the database.
    def getFEMThermoOrthMaterialsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThermoOrthMaterials'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMThermoOrthMaterials object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMThermoOrthMaterials object fields
    # @return \c True on success, \c False if object not found
    def getFEMThermoOrthMaterials(self, fields):
        selectCmd = 'SELECT '\
            'var_Density,var_MaterialID,var_TREF '\
            'FROM CLASS_FEMThermoOrthMaterials WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDensity = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mTREF = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMThermoOrthMaterials objects matching the given
    # reference data.
    #
    # @param fields FEMThermoOrthMaterials object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMThermoOrthMaterials objects
    def findEqualFEMThermoOrthMaterials(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('TREF' in fieldNames):
            cmds.append("var_TREF=%0.16g")
            args.append(fields.mTREF)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMThermoOrthMaterials WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMConstraints object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMConstraints object fields
    def addFEMConstraints(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMConstraints '\
            '(var_ConstraintID,var_Description,var_SlaveDOF,var_SlaveNode) VALUES '\
            "(%s,%s,%s,%s)"
        args = []
        args.append(fields.mConstraintID)
        args.append(fields.mDescription)
        args.append(fields.mSlaveDOF)
        args.append(fields.mSlaveNode)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMConstraints_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMConstraints objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMConstraints object fields
    def addFEMConstraintsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMConstraints '\
            '(var_ConstraintID,var_Description,var_SlaveDOF,var_SlaveNode) VALUES '\
            "(%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mConstraintID)
            args.append(fields.mDescription)
            args.append(fields.mSlaveDOF)
            args.append(fields.mSlaveNode)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMConstraints_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMConstraints object in the database.
    #
    # @param fields     FEMConstraints object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMConstraints(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ConstraintID' in fieldNames):
            cmds.append("var_ConstraintID=%s")
            args.append(fields.mConstraintID)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('SlaveDOF' in fieldNames):
            cmds.append("var_SlaveDOF=%s")
            args.append(fields.mSlaveDOF)
        if allFields or ('SlaveNode' in fieldNames):
            cmds.append("var_SlaveNode=%s")
            args.append(fields.mSlaveNode)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMConstraints SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMConstraints object IDs from the database.
    def getFEMConstraintsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMConstraints'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMConstraints object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMConstraints object fields
    # @return \c True on success, \c False if object not found
    def getFEMConstraints(self, fields):
        selectCmd = 'SELECT '\
            'var_ConstraintID,var_Description,var_SlaveDOF,var_SlaveNode '\
            'FROM CLASS_FEMConstraints WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mConstraintID = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mSlaveDOF = data.pop(0)
        fields.mSlaveNode = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMConstraints objects matching the given
    # reference data.
    #
    # @param fields FEMConstraints object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMConstraints objects
    def findEqualFEMConstraints(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ConstraintID' in fieldNames):
            cmds.append("var_ConstraintID=%s")
            args.append(fields.mConstraintID)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('SlaveDOF' in fieldNames):
            cmds.append("var_SlaveDOF=%s")
            args.append(fields.mSlaveDOF)
        if (len(fieldNames)) == 0 or ('SlaveNode' in fieldNames):
            cmds.append("var_SlaveNode=%s")
            args.append(fields.mSlaveNode)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMConstraints WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMMCrigidities object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMMCrigidities object fields
    def addFEMMCrigidities(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMMCrigidities '\
            '(var_AcurveType,var_Alpha,var_AxialCF,var_BcurveType,var_BendingCF,var_Beta,var_Density,var_ForceAxID,var_Hardening,var_MassArea,var_MassR,var_MassS,var_MassT,var_MomentRID,var_MomentSID,var_MomentTID,var_RigidityID,var_TcurveType,var_TorsionCF) VALUES '\
            "(%s,%s,%0.16g,%s,%0.16g,%s,%0.16g,%s,%s,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s,%s,%s,%s,%0.16g)"
        args = []
        args.append(fields.mAcurveType)
        args.append(fields.mAlpha)
        args.append(fields.mAxialCF)
        args.append(fields.mBcurveType)
        args.append(fields.mBendingCF)
        args.append(fields.mBeta)
        args.append(fields.mDensity)
        args.append(fields.mForceAxID)
        args.append(fields.mHardening)
        args.append(fields.mMassArea)
        args.append(fields.mMassR)
        args.append(fields.mMassS)
        args.append(fields.mMassT)
        args.append(fields.mMomentRID)
        args.append(fields.mMomentSID)
        args.append(fields.mMomentTID)
        args.append(fields.mRigidityID)
        args.append(fields.mTcurveType)
        args.append(fields.mTorsionCF)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMMCrigidities_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMMCrigidities objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMMCrigidities object fields
    def addFEMMCrigiditiesList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMMCrigidities '\
            '(var_AcurveType,var_Alpha,var_AxialCF,var_BcurveType,var_BendingCF,var_Beta,var_Density,var_ForceAxID,var_Hardening,var_MassArea,var_MassR,var_MassS,var_MassT,var_MomentRID,var_MomentSID,var_MomentTID,var_RigidityID,var_TcurveType,var_TorsionCF) VALUES '\
            "(%s,%s,%0.16g,%s,%0.16g,%s,%0.16g,%s,%s,%0.16g,%0.16g,%0.16g,%0.16g,%s,%s,%s,%s,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAcurveType)
            args.append(fields.mAlpha)
            args.append(fields.mAxialCF)
            args.append(fields.mBcurveType)
            args.append(fields.mBendingCF)
            args.append(fields.mBeta)
            args.append(fields.mDensity)
            args.append(fields.mForceAxID)
            args.append(fields.mHardening)
            args.append(fields.mMassArea)
            args.append(fields.mMassR)
            args.append(fields.mMassS)
            args.append(fields.mMassT)
            args.append(fields.mMomentRID)
            args.append(fields.mMomentSID)
            args.append(fields.mMomentTID)
            args.append(fields.mRigidityID)
            args.append(fields.mTcurveType)
            args.append(fields.mTorsionCF)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMMCrigidities_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMMCrigidities object in the database.
    #
    # @param fields     FEMMCrigidities object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMMCrigidities(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AcurveType' in fieldNames):
            cmds.append("var_AcurveType=%s")
            args.append(fields.mAcurveType)
        if allFields or ('Alpha' in fieldNames):
            cmds.append("var_Alpha=%s")
            args.append(fields.mAlpha)
        if allFields or ('AxialCF' in fieldNames):
            cmds.append("var_AxialCF=%0.16g")
            args.append(fields.mAxialCF)
        if allFields or ('BcurveType' in fieldNames):
            cmds.append("var_BcurveType=%s")
            args.append(fields.mBcurveType)
        if allFields or ('BendingCF' in fieldNames):
            cmds.append("var_BendingCF=%0.16g")
            args.append(fields.mBendingCF)
        if allFields or ('Beta' in fieldNames):
            cmds.append("var_Beta=%s")
            args.append(fields.mBeta)
        if allFields or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if allFields or ('ForceAxID' in fieldNames):
            cmds.append("var_ForceAxID=%s")
            args.append(fields.mForceAxID)
        if allFields or ('Hardening' in fieldNames):
            cmds.append("var_Hardening=%s")
            args.append(fields.mHardening)
        if allFields or ('MassArea' in fieldNames):
            cmds.append("var_MassArea=%0.16g")
            args.append(fields.mMassArea)
        if allFields or ('MassR' in fieldNames):
            cmds.append("var_MassR=%0.16g")
            args.append(fields.mMassR)
        if allFields or ('MassS' in fieldNames):
            cmds.append("var_MassS=%0.16g")
            args.append(fields.mMassS)
        if allFields or ('MassT' in fieldNames):
            cmds.append("var_MassT=%0.16g")
            args.append(fields.mMassT)
        if allFields or ('MomentRID' in fieldNames):
            cmds.append("var_MomentRID=%s")
            args.append(fields.mMomentRID)
        if allFields or ('MomentSID' in fieldNames):
            cmds.append("var_MomentSID=%s")
            args.append(fields.mMomentSID)
        if allFields or ('MomentTID' in fieldNames):
            cmds.append("var_MomentTID=%s")
            args.append(fields.mMomentTID)
        if allFields or ('RigidityID' in fieldNames):
            cmds.append("var_RigidityID=%s")
            args.append(fields.mRigidityID)
        if allFields or ('TcurveType' in fieldNames):
            cmds.append("var_TcurveType=%s")
            args.append(fields.mTcurveType)
        if allFields or ('TorsionCF' in fieldNames):
            cmds.append("var_TorsionCF=%0.16g")
            args.append(fields.mTorsionCF)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMMCrigidities SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMMCrigidities object IDs from the database.
    def getFEMMCrigiditiesIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMCrigidities'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMMCrigidities object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMMCrigidities object fields
    # @return \c True on success, \c False if object not found
    def getFEMMCrigidities(self, fields):
        selectCmd = 'SELECT '\
            'var_AcurveType,var_Alpha,var_AxialCF,var_BcurveType,var_BendingCF,var_Beta,var_Density,var_ForceAxID,var_Hardening,var_MassArea,var_MassR,var_MassS,var_MassT,var_MomentRID,var_MomentSID,var_MomentTID,var_RigidityID,var_TcurveType,var_TorsionCF '\
            'FROM CLASS_FEMMCrigidities WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAcurveType = data.pop(0)
        fields.mAlpha = data.pop(0)
        fields.mAxialCF = data.pop(0)
        fields.mBcurveType = data.pop(0)
        fields.mBendingCF = data.pop(0)
        fields.mBeta = data.pop(0)
        fields.mDensity = data.pop(0)
        fields.mForceAxID = data.pop(0)
        fields.mHardening = data.pop(0)
        fields.mMassArea = data.pop(0)
        fields.mMassR = data.pop(0)
        fields.mMassS = data.pop(0)
        fields.mMassT = data.pop(0)
        fields.mMomentRID = data.pop(0)
        fields.mMomentSID = data.pop(0)
        fields.mMomentTID = data.pop(0)
        fields.mRigidityID = data.pop(0)
        fields.mTcurveType = data.pop(0)
        fields.mTorsionCF = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMMCrigidities objects matching the given
    # reference data.
    #
    # @param fields FEMMCrigidities object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMMCrigidities objects
    def findEqualFEMMCrigidities(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AcurveType' in fieldNames):
            cmds.append("var_AcurveType=%s")
            args.append(fields.mAcurveType)
        if (len(fieldNames)) == 0 or ('Alpha' in fieldNames):
            cmds.append("var_Alpha=%s")
            args.append(fields.mAlpha)
        if (len(fieldNames)) == 0 or ('AxialCF' in fieldNames):
            cmds.append("var_AxialCF=%0.16g")
            args.append(fields.mAxialCF)
        if (len(fieldNames)) == 0 or ('BcurveType' in fieldNames):
            cmds.append("var_BcurveType=%s")
            args.append(fields.mBcurveType)
        if (len(fieldNames)) == 0 or ('BendingCF' in fieldNames):
            cmds.append("var_BendingCF=%0.16g")
            args.append(fields.mBendingCF)
        if (len(fieldNames)) == 0 or ('Beta' in fieldNames):
            cmds.append("var_Beta=%s")
            args.append(fields.mBeta)
        if (len(fieldNames)) == 0 or ('Density' in fieldNames):
            cmds.append("var_Density=%0.16g")
            args.append(fields.mDensity)
        if (len(fieldNames)) == 0 or ('ForceAxID' in fieldNames):
            cmds.append("var_ForceAxID=%s")
            args.append(fields.mForceAxID)
        if (len(fieldNames)) == 0 or ('Hardening' in fieldNames):
            cmds.append("var_Hardening=%s")
            args.append(fields.mHardening)
        if (len(fieldNames)) == 0 or ('MassArea' in fieldNames):
            cmds.append("var_MassArea=%0.16g")
            args.append(fields.mMassArea)
        if (len(fieldNames)) == 0 or ('MassR' in fieldNames):
            cmds.append("var_MassR=%0.16g")
            args.append(fields.mMassR)
        if (len(fieldNames)) == 0 or ('MassS' in fieldNames):
            cmds.append("var_MassS=%0.16g")
            args.append(fields.mMassS)
        if (len(fieldNames)) == 0 or ('MassT' in fieldNames):
            cmds.append("var_MassT=%0.16g")
            args.append(fields.mMassT)
        if (len(fieldNames)) == 0 or ('MomentRID' in fieldNames):
            cmds.append("var_MomentRID=%s")
            args.append(fields.mMomentRID)
        if (len(fieldNames)) == 0 or ('MomentSID' in fieldNames):
            cmds.append("var_MomentSID=%s")
            args.append(fields.mMomentSID)
        if (len(fieldNames)) == 0 or ('MomentTID' in fieldNames):
            cmds.append("var_MomentTID=%s")
            args.append(fields.mMomentTID)
        if (len(fieldNames)) == 0 or ('RigidityID' in fieldNames):
            cmds.append("var_RigidityID=%s")
            args.append(fields.mRigidityID)
        if (len(fieldNames)) == 0 or ('TcurveType' in fieldNames):
            cmds.append("var_TcurveType=%s")
            args.append(fields.mTcurveType)
        if (len(fieldNames)) == 0 or ('TorsionCF' in fieldNames):
            cmds.append("var_TorsionCF=%0.16g")
            args.append(fields.mTorsionCF)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMCrigidities WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSkeySysNode object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSkeySysNode object fields
    def addFEMSkeySysNode(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSkeySysNode '\
            '(var_Description,ref_Node1,ref_Node2,ref_Node3,var_SkewSystemID) VALUES '\
            "(%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        if fields.mNode1 == 0:
            args.append(None)
        else:
            args.append(fields.mNode1)
        if fields.mNode2 == 0:
            args.append(None)
        else:
            args.append(fields.mNode2)
        if fields.mNode3 == 0:
            args.append(None)
        else:
            args.append(fields.mNode3)
        args.append(fields.mSkewSystemID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSkeySysNode_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSkeySysNode objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSkeySysNode object fields
    def addFEMSkeySysNodeList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSkeySysNode '\
            '(var_Description,ref_Node1,ref_Node2,ref_Node3,var_SkewSystemID) VALUES '\
            "(%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            if fields.mNode1 == 0:
                args.append(None)
            else:
                args.append(fields.mNode1)
            if fields.mNode2 == 0:
                args.append(None)
            else:
                args.append(fields.mNode2)
            if fields.mNode3 == 0:
                args.append(None)
            else:
                args.append(fields.mNode3)
            args.append(fields.mSkewSystemID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSkeySysNode_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSkeySysNode object in the database.
    #
    # @param fields     FEMSkeySysNode object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSkeySysNode(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Node1' in fieldNames):
            cmds.append("ref_Node1=%s")
            args.append(fields.mNode1)
        if allFields or ('Node2' in fieldNames):
            cmds.append("ref_Node2=%s")
            args.append(fields.mNode2)
        if allFields or ('Node3' in fieldNames):
            cmds.append("ref_Node3=%s")
            args.append(fields.mNode3)
        if allFields or ('SkewSystemID' in fieldNames):
            cmds.append("var_SkewSystemID=%s")
            args.append(fields.mSkewSystemID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSkeySysNode SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSkeySysNode object IDs from the database.
    def getFEMSkeySysNodeIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSkeySysNode'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSkeySysNode object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSkeySysNode object fields
    # @return \c True on success, \c False if object not found
    def getFEMSkeySysNode(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,ref_Node1,ref_Node2,ref_Node3,var_SkewSystemID '\
            'FROM CLASS_FEMSkeySysNode WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mNode1 = zeroNone(data.pop(0))
        fields.mNode2 = zeroNone(data.pop(0))
        fields.mNode3 = zeroNone(data.pop(0))
        fields.mSkewSystemID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSkeySysNode objects matching the given
    # reference data.
    #
    # @param fields FEMSkeySysNode object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSkeySysNode objects
    def findEqualFEMSkeySysNode(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Node1' in fieldNames):
            cmds.append("ref_Node1=%s")
            args.append(fields.mNode1)
        if (len(fieldNames)) == 0 or ('Node2' in fieldNames):
            cmds.append("ref_Node2=%s")
            args.append(fields.mNode2)
        if (len(fieldNames)) == 0 or ('Node3' in fieldNames):
            cmds.append("ref_Node3=%s")
            args.append(fields.mNode3)
        if (len(fieldNames)) == 0 or ('SkewSystemID' in fieldNames):
            cmds.append("var_SkewSystemID=%s")
            args.append(fields.mSkewSystemID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSkeySysNode WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMIsoBeamGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMIsoBeamGroup object fields
    def addFEMIsoBeamGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMIsoBeamGroup '\
            '(var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_Result,var_SectionID) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDisplacement)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mIniStrain)
        args.append(fields.mMaterialID)
        args.append(fields.mResult)
        args.append(fields.mSectionID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMIsoBeamGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMIsoBeamGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMIsoBeamGroup object fields
    def addFEMIsoBeamGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMIsoBeamGroup '\
            '(var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_Result,var_SectionID) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDisplacement)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mIniStrain)
            args.append(fields.mMaterialID)
            args.append(fields.mResult)
            args.append(fields.mSectionID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMIsoBeamGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMIsoBeamGroup object in the database.
    #
    # @param fields     FEMIsoBeamGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMIsoBeamGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('IniStrain' in fieldNames):
            cmds.append("var_IniStrain=%s")
            args.append(fields.mIniStrain)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMIsoBeamGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMIsoBeamGroup object IDs from the database.
    def getFEMIsoBeamGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMIsoBeamGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMIsoBeamGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMIsoBeamGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMIsoBeamGroup(self, fields):
        selectCmd = 'SELECT '\
            'var_Displacement,ref_Group,var_IniStrain,var_MaterialID,var_Result,var_SectionID '\
            'FROM CLASS_FEMIsoBeamGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDisplacement = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mIniStrain = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mResult = data.pop(0)
        fields.mSectionID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMIsoBeamGroup objects matching the given
    # reference data.
    #
    # @param fields FEMIsoBeamGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMIsoBeamGroup objects
    def findEqualFEMIsoBeamGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('IniStrain' in fieldNames):
            cmds.append("var_IniStrain=%s")
            args.append(fields.mIniStrain)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMIsoBeamGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMShellDOF object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMShellDOF object fields
    def addFEMShellDOF(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMShellDOF '\
            '(var_DOFnumber,ref_Node,var_VectorID) VALUES '\
            "(%s,%s,%s)"
        args = []
        args.append(fields.mDOFnumber)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(fields.mVectorID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMShellDOF_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMShellDOF objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMShellDOF object fields
    def addFEMShellDOFList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMShellDOF '\
            '(var_DOFnumber,ref_Node,var_VectorID) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDOFnumber)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(fields.mVectorID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMShellDOF_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMShellDOF object in the database.
    #
    # @param fields     FEMShellDOF object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMShellDOF(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('DOFnumber' in fieldNames):
            cmds.append("var_DOFnumber=%s")
            args.append(fields.mDOFnumber)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('VectorID' in fieldNames):
            cmds.append("var_VectorID=%s")
            args.append(fields.mVectorID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMShellDOF SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMShellDOF object IDs from the database.
    def getFEMShellDOFIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellDOF'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMShellDOF object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMShellDOF object fields
    # @return \c True on success, \c False if object not found
    def getFEMShellDOF(self, fields):
        selectCmd = 'SELECT '\
            'var_DOFnumber,ref_Node,var_VectorID '\
            'FROM CLASS_FEMShellDOF WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDOFnumber = data.pop(0)
        fields.mNode = zeroNone(data.pop(0))
        fields.mVectorID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMShellDOF objects matching the given
    # reference data.
    #
    # @param fields FEMShellDOF object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMShellDOF objects
    def findEqualFEMShellDOF(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('DOFnumber' in fieldNames):
            cmds.append("var_DOFnumber=%s")
            args.append(fields.mDOFnumber)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('VectorID' in fieldNames):
            cmds.append("var_VectorID=%s")
            args.append(fields.mVectorID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellDOF WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMCrossSections object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMCrossSections object fields
    def addFEMCrossSections(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMCrossSections '\
            '(var_Description,var_SectionID,var_SectionType) VALUES '\
            "(%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mSectionID)
        args.append(fields.mSectionType)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMCrossSections_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMCrossSections objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMCrossSections object fields
    def addFEMCrossSectionsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMCrossSections '\
            '(var_Description,var_SectionID,var_SectionType) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mSectionID)
            args.append(fields.mSectionType)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMCrossSections_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMCrossSections object in the database.
    #
    # @param fields     FEMCrossSections object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMCrossSections(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('SectionType' in fieldNames):
            cmds.append("var_SectionType=%s")
            args.append(fields.mSectionType)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMCrossSections SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMCrossSections object IDs from the database.
    def getFEMCrossSectionsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMCrossSections'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMCrossSections object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMCrossSections object fields
    # @return \c True on success, \c False if object not found
    def getFEMCrossSections(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_SectionID,var_SectionType '\
            'FROM CLASS_FEMCrossSections WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mSectionType = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMCrossSections objects matching the given
    # reference data.
    #
    # @param fields FEMCrossSections object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMCrossSections objects
    def findEqualFEMCrossSections(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('SectionType' in fieldNames):
            cmds.append("var_SectionType=%s")
            args.append(fields.mSectionType)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMCrossSections WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMTwistMomentData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMTwistMomentData object fields
    def addFEMTwistMomentData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMTwistMomentData '\
            '(var_Moment,var_RecordNmb,var_Twist,var_TwistMomentID) VALUES '\
            "(%0.16g,%s,%0.16g,%s)"
        args = []
        args.append(fields.mMoment)
        args.append(fields.mRecordNmb)
        args.append(fields.mTwist)
        args.append(fields.mTwistMomentID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMTwistMomentData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMTwistMomentData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMTwistMomentData object fields
    def addFEMTwistMomentDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMTwistMomentData '\
            '(var_Moment,var_RecordNmb,var_Twist,var_TwistMomentID) VALUES '\
            "(%0.16g,%s,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mMoment)
            args.append(fields.mRecordNmb)
            args.append(fields.mTwist)
            args.append(fields.mTwistMomentID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMTwistMomentData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMTwistMomentData object in the database.
    #
    # @param fields     FEMTwistMomentData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMTwistMomentData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Moment' in fieldNames):
            cmds.append("var_Moment=%0.16g")
            args.append(fields.mMoment)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Twist' in fieldNames):
            cmds.append("var_Twist=%0.16g")
            args.append(fields.mTwist)
        if allFields or ('TwistMomentID' in fieldNames):
            cmds.append("var_TwistMomentID=%s")
            args.append(fields.mTwistMomentID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMTwistMomentData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMTwistMomentData object IDs from the database.
    def getFEMTwistMomentDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTwistMomentData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMTwistMomentData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMTwistMomentData object fields
    # @return \c True on success, \c False if object not found
    def getFEMTwistMomentData(self, fields):
        selectCmd = 'SELECT '\
            'var_Moment,var_RecordNmb,var_Twist,var_TwistMomentID '\
            'FROM CLASS_FEMTwistMomentData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mMoment = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mTwist = data.pop(0)
        fields.mTwistMomentID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMTwistMomentData objects matching the given
    # reference data.
    #
    # @param fields FEMTwistMomentData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMTwistMomentData objects
    def findEqualFEMTwistMomentData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Moment' in fieldNames):
            cmds.append("var_Moment=%0.16g")
            args.append(fields.mMoment)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Twist' in fieldNames):
            cmds.append("var_Twist=%0.16g")
            args.append(fields.mTwist)
        if (len(fieldNames)) == 0 or ('TwistMomentID' in fieldNames):
            cmds.append("var_TwistMomentID=%s")
            args.append(fields.mTwistMomentID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTwistMomentData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMShell object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMShell object fields
    def addFEMShell(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMShell '\
            '(var_Description,var_ElementID,ref_Group,ref_Material,ref_N1,ref_N2,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_T1,var_T2,var_T3,var_T4,var_T5,var_T6,var_T7,var_T8,var_T9,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        if fields.mMaterial == 0:
            args.append(None)
        else:
            args.append(fields.mMaterial)
        if fields.mN1 == 0:
            args.append(None)
        else:
            args.append(fields.mN1)
        if fields.mN2 == 0:
            args.append(None)
        else:
            args.append(fields.mN2)
        if fields.mN3 == 0:
            args.append(None)
        else:
            args.append(fields.mN3)
        if fields.mN4 == 0:
            args.append(None)
        else:
            args.append(fields.mN4)
        if fields.mN5 == 0:
            args.append(None)
        else:
            args.append(fields.mN5)
        if fields.mN6 == 0:
            args.append(None)
        else:
            args.append(fields.mN6)
        if fields.mN7 == 0:
            args.append(None)
        else:
            args.append(fields.mN7)
        if fields.mN8 == 0:
            args.append(None)
        else:
            args.append(fields.mN8)
        if fields.mN9 == 0:
            args.append(None)
        else:
            args.append(fields.mN9)
        args.append(fields.mNodeAmount)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        args.append(fields.mT1)
        args.append(fields.mT2)
        args.append(fields.mT3)
        args.append(fields.mT4)
        args.append(fields.mT5)
        args.append(fields.mT6)
        args.append(fields.mT7)
        args.append(fields.mT8)
        args.append(fields.mT9)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMShell_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMShell objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMShell object fields
    def addFEMShellList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMShell '\
            '(var_Description,var_ElementID,ref_Group,ref_Material,ref_N1,ref_N2,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_T1,var_T2,var_T3,var_T4,var_T5,var_T6,var_T7,var_T8,var_T9,var_TBirth,var_TDeath) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            if fields.mMaterial == 0:
                args.append(None)
            else:
                args.append(fields.mMaterial)
            if fields.mN1 == 0:
                args.append(None)
            else:
                args.append(fields.mN1)
            if fields.mN2 == 0:
                args.append(None)
            else:
                args.append(fields.mN2)
            if fields.mN3 == 0:
                args.append(None)
            else:
                args.append(fields.mN3)
            if fields.mN4 == 0:
                args.append(None)
            else:
                args.append(fields.mN4)
            if fields.mN5 == 0:
                args.append(None)
            else:
                args.append(fields.mN5)
            if fields.mN6 == 0:
                args.append(None)
            else:
                args.append(fields.mN6)
            if fields.mN7 == 0:
                args.append(None)
            else:
                args.append(fields.mN7)
            if fields.mN8 == 0:
                args.append(None)
            else:
                args.append(fields.mN8)
            if fields.mN9 == 0:
                args.append(None)
            else:
                args.append(fields.mN9)
            args.append(fields.mNodeAmount)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            args.append(fields.mT1)
            args.append(fields.mT2)
            args.append(fields.mT3)
            args.append(fields.mT4)
            args.append(fields.mT5)
            args.append(fields.mT6)
            args.append(fields.mT7)
            args.append(fields.mT8)
            args.append(fields.mT9)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMShell_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMShell object in the database.
    #
    # @param fields     FEMShell object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMShell(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if allFields or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if allFields or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if allFields or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if allFields or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if allFields or ('N5' in fieldNames):
            cmds.append("ref_N5=%s")
            args.append(fields.mN5)
        if allFields or ('N6' in fieldNames):
            cmds.append("ref_N6=%s")
            args.append(fields.mN6)
        if allFields or ('N7' in fieldNames):
            cmds.append("ref_N7=%s")
            args.append(fields.mN7)
        if allFields or ('N8' in fieldNames):
            cmds.append("ref_N8=%s")
            args.append(fields.mN8)
        if allFields or ('N9' in fieldNames):
            cmds.append("ref_N9=%s")
            args.append(fields.mN9)
        if allFields or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if allFields or ('T1' in fieldNames):
            cmds.append("var_T1=%0.16g")
            args.append(fields.mT1)
        if allFields or ('T2' in fieldNames):
            cmds.append("var_T2=%0.16g")
            args.append(fields.mT2)
        if allFields or ('T3' in fieldNames):
            cmds.append("var_T3=%0.16g")
            args.append(fields.mT3)
        if allFields or ('T4' in fieldNames):
            cmds.append("var_T4=%0.16g")
            args.append(fields.mT4)
        if allFields or ('T5' in fieldNames):
            cmds.append("var_T5=%0.16g")
            args.append(fields.mT5)
        if allFields or ('T6' in fieldNames):
            cmds.append("var_T6=%0.16g")
            args.append(fields.mT6)
        if allFields or ('T7' in fieldNames):
            cmds.append("var_T7=%0.16g")
            args.append(fields.mT7)
        if allFields or ('T8' in fieldNames):
            cmds.append("var_T8=%0.16g")
            args.append(fields.mT8)
        if allFields or ('T9' in fieldNames):
            cmds.append("var_T9=%0.16g")
            args.append(fields.mT9)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMShell SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMShell object IDs from the database.
    def getFEMShellIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShell'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMShell object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMShell object fields
    # @return \c True on success, \c False if object not found
    def getFEMShell(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_ElementID,ref_Group,ref_Material,ref_N1,ref_N2,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_T1,var_T2,var_T3,var_T4,var_T5,var_T6,var_T7,var_T8,var_T9,var_TBirth,var_TDeath '\
            'FROM CLASS_FEMShell WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterial = zeroNone(data.pop(0))
        fields.mN1 = zeroNone(data.pop(0))
        fields.mN2 = zeroNone(data.pop(0))
        fields.mN3 = zeroNone(data.pop(0))
        fields.mN4 = zeroNone(data.pop(0))
        fields.mN5 = zeroNone(data.pop(0))
        fields.mN6 = zeroNone(data.pop(0))
        fields.mN7 = zeroNone(data.pop(0))
        fields.mN8 = zeroNone(data.pop(0))
        fields.mN9 = zeroNone(data.pop(0))
        fields.mNodeAmount = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        fields.mT1 = data.pop(0)
        fields.mT2 = data.pop(0)
        fields.mT3 = data.pop(0)
        fields.mT4 = data.pop(0)
        fields.mT5 = data.pop(0)
        fields.mT6 = data.pop(0)
        fields.mT7 = data.pop(0)
        fields.mT8 = data.pop(0)
        fields.mT9 = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMShell objects matching the given
    # reference data.
    #
    # @param fields FEMShell object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMShell objects
    def findEqualFEMShell(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if (len(fieldNames)) == 0 or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if (len(fieldNames)) == 0 or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if (len(fieldNames)) == 0 or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if (len(fieldNames)) == 0 or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if (len(fieldNames)) == 0 or ('N5' in fieldNames):
            cmds.append("ref_N5=%s")
            args.append(fields.mN5)
        if (len(fieldNames)) == 0 or ('N6' in fieldNames):
            cmds.append("ref_N6=%s")
            args.append(fields.mN6)
        if (len(fieldNames)) == 0 or ('N7' in fieldNames):
            cmds.append("ref_N7=%s")
            args.append(fields.mN7)
        if (len(fieldNames)) == 0 or ('N8' in fieldNames):
            cmds.append("ref_N8=%s")
            args.append(fields.mN8)
        if (len(fieldNames)) == 0 or ('N9' in fieldNames):
            cmds.append("ref_N9=%s")
            args.append(fields.mN9)
        if (len(fieldNames)) == 0 or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if (len(fieldNames)) == 0 or ('T1' in fieldNames):
            cmds.append("var_T1=%0.16g")
            args.append(fields.mT1)
        if (len(fieldNames)) == 0 or ('T2' in fieldNames):
            cmds.append("var_T2=%0.16g")
            args.append(fields.mT2)
        if (len(fieldNames)) == 0 or ('T3' in fieldNames):
            cmds.append("var_T3=%0.16g")
            args.append(fields.mT3)
        if (len(fieldNames)) == 0 or ('T4' in fieldNames):
            cmds.append("var_T4=%0.16g")
            args.append(fields.mT4)
        if (len(fieldNames)) == 0 or ('T5' in fieldNames):
            cmds.append("var_T5=%0.16g")
            args.append(fields.mT5)
        if (len(fieldNames)) == 0 or ('T6' in fieldNames):
            cmds.append("var_T6=%0.16g")
            args.append(fields.mT6)
        if (len(fieldNames)) == 0 or ('T7' in fieldNames):
            cmds.append("var_T7=%0.16g")
            args.append(fields.mT7)
        if (len(fieldNames)) == 0 or ('T8' in fieldNames):
            cmds.append("var_T8=%0.16g")
            args.append(fields.mT8)
        if (len(fieldNames)) == 0 or ('T9' in fieldNames):
            cmds.append("var_T9=%0.16g")
            args.append(fields.mT9)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShell WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMNTNContact object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMNTNContact object fields
    def addFEMNTNContact(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMNTNContact '\
            '(var_ContactorNode,var_ContGroupID,var_ContPair,var_PrintRes,var_RecordNmb,var_SaveRes,var_TargetNode,var_TargetNx,var_TargetNy,var_TargetNz) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mContactorNode)
        args.append(fields.mContGroupID)
        args.append(fields.mContPair)
        args.append(fields.mPrintRes)
        args.append(fields.mRecordNmb)
        args.append(fields.mSaveRes)
        args.append(fields.mTargetNode)
        args.append(fields.mTargetNx)
        args.append(fields.mTargetNy)
        args.append(fields.mTargetNz)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMNTNContact_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMNTNContact objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMNTNContact object fields
    def addFEMNTNContactList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMNTNContact '\
            '(var_ContactorNode,var_ContGroupID,var_ContPair,var_PrintRes,var_RecordNmb,var_SaveRes,var_TargetNode,var_TargetNx,var_TargetNy,var_TargetNz) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mContactorNode)
            args.append(fields.mContGroupID)
            args.append(fields.mContPair)
            args.append(fields.mPrintRes)
            args.append(fields.mRecordNmb)
            args.append(fields.mSaveRes)
            args.append(fields.mTargetNode)
            args.append(fields.mTargetNx)
            args.append(fields.mTargetNy)
            args.append(fields.mTargetNz)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMNTNContact_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMNTNContact object in the database.
    #
    # @param fields     FEMNTNContact object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMNTNContact(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ContactorNode' in fieldNames):
            cmds.append("var_ContactorNode=%s")
            args.append(fields.mContactorNode)
        if allFields or ('ContGroupID' in fieldNames):
            cmds.append("var_ContGroupID=%s")
            args.append(fields.mContGroupID)
        if allFields or ('ContPair' in fieldNames):
            cmds.append("var_ContPair=%s")
            args.append(fields.mContPair)
        if allFields or ('PrintRes' in fieldNames):
            cmds.append("var_PrintRes=%s")
            args.append(fields.mPrintRes)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('SaveRes' in fieldNames):
            cmds.append("var_SaveRes=%s")
            args.append(fields.mSaveRes)
        if allFields or ('TargetNode' in fieldNames):
            cmds.append("var_TargetNode=%s")
            args.append(fields.mTargetNode)
        if allFields or ('TargetNx' in fieldNames):
            cmds.append("var_TargetNx=%0.16g")
            args.append(fields.mTargetNx)
        if allFields or ('TargetNy' in fieldNames):
            cmds.append("var_TargetNy=%0.16g")
            args.append(fields.mTargetNy)
        if allFields or ('TargetNz' in fieldNames):
            cmds.append("var_TargetNz=%0.16g")
            args.append(fields.mTargetNz)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMNTNContact SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMNTNContact object IDs from the database.
    def getFEMNTNContactIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNTNContact'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMNTNContact object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMNTNContact object fields
    # @return \c True on success, \c False if object not found
    def getFEMNTNContact(self, fields):
        selectCmd = 'SELECT '\
            'var_ContactorNode,var_ContGroupID,var_ContPair,var_PrintRes,var_RecordNmb,var_SaveRes,var_TargetNode,var_TargetNx,var_TargetNy,var_TargetNz '\
            'FROM CLASS_FEMNTNContact WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mContactorNode = data.pop(0)
        fields.mContGroupID = data.pop(0)
        fields.mContPair = data.pop(0)
        fields.mPrintRes = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mSaveRes = data.pop(0)
        fields.mTargetNode = data.pop(0)
        fields.mTargetNx = data.pop(0)
        fields.mTargetNy = data.pop(0)
        fields.mTargetNz = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMNTNContact objects matching the given
    # reference data.
    #
    # @param fields FEMNTNContact object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMNTNContact objects
    def findEqualFEMNTNContact(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ContactorNode' in fieldNames):
            cmds.append("var_ContactorNode=%s")
            args.append(fields.mContactorNode)
        if (len(fieldNames)) == 0 or ('ContGroupID' in fieldNames):
            cmds.append("var_ContGroupID=%s")
            args.append(fields.mContGroupID)
        if (len(fieldNames)) == 0 or ('ContPair' in fieldNames):
            cmds.append("var_ContPair=%s")
            args.append(fields.mContPair)
        if (len(fieldNames)) == 0 or ('PrintRes' in fieldNames):
            cmds.append("var_PrintRes=%s")
            args.append(fields.mPrintRes)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('SaveRes' in fieldNames):
            cmds.append("var_SaveRes=%s")
            args.append(fields.mSaveRes)
        if (len(fieldNames)) == 0 or ('TargetNode' in fieldNames):
            cmds.append("var_TargetNode=%s")
            args.append(fields.mTargetNode)
        if (len(fieldNames)) == 0 or ('TargetNx' in fieldNames):
            cmds.append("var_TargetNx=%0.16g")
            args.append(fields.mTargetNx)
        if (len(fieldNames)) == 0 or ('TargetNy' in fieldNames):
            cmds.append("var_TargetNy=%0.16g")
            args.append(fields.mTargetNy)
        if (len(fieldNames)) == 0 or ('TargetNz' in fieldNames):
            cmds.append("var_TargetNz=%0.16g")
            args.append(fields.mTargetNz)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNTNContact WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMShellLayer object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMShellLayer object fields
    def addFEMShellLayer(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMShellLayer '\
            '(ref_Group,var_LayerNumber,var_MaterialID,var_PThick,var_RecordNmb) VALUES '\
            "(%s,%s,%s,%0.16g,%s)"
        args = []
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mLayerNumber)
        args.append(fields.mMaterialID)
        args.append(fields.mPThick)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMShellLayer_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMShellLayer objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMShellLayer object fields
    def addFEMShellLayerList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMShellLayer '\
            '(ref_Group,var_LayerNumber,var_MaterialID,var_PThick,var_RecordNmb) VALUES '\
            "(%s,%s,%s,%0.16g,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mLayerNumber)
            args.append(fields.mMaterialID)
            args.append(fields.mPThick)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMShellLayer_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMShellLayer object in the database.
    #
    # @param fields     FEMShellLayer object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMShellLayer(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('LayerNumber' in fieldNames):
            cmds.append("var_LayerNumber=%s")
            args.append(fields.mLayerNumber)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('PThick' in fieldNames):
            cmds.append("var_PThick=%0.16g")
            args.append(fields.mPThick)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMShellLayer SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMShellLayer object IDs from the database.
    def getFEMShellLayerIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellLayer'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMShellLayer object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMShellLayer object fields
    # @return \c True on success, \c False if object not found
    def getFEMShellLayer(self, fields):
        selectCmd = 'SELECT '\
            'ref_Group,var_LayerNumber,var_MaterialID,var_PThick,var_RecordNmb '\
            'FROM CLASS_FEMShellLayer WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mGroup = zeroNone(data.pop(0))
        fields.mLayerNumber = data.pop(0)
        fields.mMaterialID = data.pop(0)
        fields.mPThick = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMShellLayer objects matching the given
    # reference data.
    #
    # @param fields FEMShellLayer object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMShellLayer objects
    def findEqualFEMShellLayer(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('LayerNumber' in fieldNames):
            cmds.append("var_LayerNumber=%s")
            args.append(fields.mLayerNumber)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('PThick' in fieldNames):
            cmds.append("var_PThick=%0.16g")
            args.append(fields.mPThick)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellLayer WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSkewSysAngles object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSkewSysAngles object fields
    def addFEMSkewSysAngles(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSkewSysAngles '\
            '(var_Description,var_PHI,var_SkewSystemID,var_THETA,var_XSI) VALUES '\
            "(%s,%0.16g,%s,%0.16g,%0.16g)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mPHI)
        args.append(fields.mSkewSystemID)
        args.append(fields.mTHETA)
        args.append(fields.mXSI)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSkewSysAngles_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSkewSysAngles objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSkewSysAngles object fields
    def addFEMSkewSysAnglesList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSkewSysAngles '\
            '(var_Description,var_PHI,var_SkewSystemID,var_THETA,var_XSI) VALUES '\
            "(%s,%0.16g,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mPHI)
            args.append(fields.mSkewSystemID)
            args.append(fields.mTHETA)
            args.append(fields.mXSI)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSkewSysAngles_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSkewSysAngles object in the database.
    #
    # @param fields     FEMSkewSysAngles object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSkewSysAngles(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('PHI' in fieldNames):
            cmds.append("var_PHI=%0.16g")
            args.append(fields.mPHI)
        if allFields or ('SkewSystemID' in fieldNames):
            cmds.append("var_SkewSystemID=%s")
            args.append(fields.mSkewSystemID)
        if allFields or ('THETA' in fieldNames):
            cmds.append("var_THETA=%0.16g")
            args.append(fields.mTHETA)
        if allFields or ('XSI' in fieldNames):
            cmds.append("var_XSI=%0.16g")
            args.append(fields.mXSI)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSkewSysAngles SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSkewSysAngles object IDs from the database.
    def getFEMSkewSysAnglesIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSkewSysAngles'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSkewSysAngles object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSkewSysAngles object fields
    # @return \c True on success, \c False if object not found
    def getFEMSkewSysAngles(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_PHI,var_SkewSystemID,var_THETA,var_XSI '\
            'FROM CLASS_FEMSkewSysAngles WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mPHI = data.pop(0)
        fields.mSkewSystemID = data.pop(0)
        fields.mTHETA = data.pop(0)
        fields.mXSI = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSkewSysAngles objects matching the given
    # reference data.
    #
    # @param fields FEMSkewSysAngles object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSkewSysAngles objects
    def findEqualFEMSkewSysAngles(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('PHI' in fieldNames):
            cmds.append("var_PHI=%0.16g")
            args.append(fields.mPHI)
        if (len(fieldNames)) == 0 or ('SkewSystemID' in fieldNames):
            cmds.append("var_SkewSystemID=%s")
            args.append(fields.mSkewSystemID)
        if (len(fieldNames)) == 0 or ('THETA' in fieldNames):
            cmds.append("var_THETA=%0.16g")
            args.append(fields.mTHETA)
        if (len(fieldNames)) == 0 or ('XSI' in fieldNames):
            cmds.append("var_XSI=%0.16g")
            args.append(fields.mXSI)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSkewSysAngles WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMGroundMotionRecord object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMGroundMotionRecord object fields
    def addFEMGroundMotionRecord(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMGroundMotionRecord '\
            '(var_Description,var_GMRecordID,var_GMRecordName) VALUES '\
            "(%s,%s,%s)"
        args = []
        args.append(fields.mDescription)
        args.append(fields.mGMRecordID)
        args.append(fields.mGMRecordName)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMGroundMotionRecord_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMGroundMotionRecord objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMGroundMotionRecord object fields
    def addFEMGroundMotionRecordList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMGroundMotionRecord '\
            '(var_Description,var_GMRecordID,var_GMRecordName) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDescription)
            args.append(fields.mGMRecordID)
            args.append(fields.mGMRecordName)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMGroundMotionRecord_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMGroundMotionRecord object in the database.
    #
    # @param fields     FEMGroundMotionRecord object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMGroundMotionRecord(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('GMRecordID' in fieldNames):
            cmds.append("var_GMRecordID=%s")
            args.append(fields.mGMRecordID)
        if allFields or ('GMRecordName' in fieldNames):
            cmds.append("var_GMRecordName=%s")
            args.append(fields.mGMRecordName)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMGroundMotionRecord SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMGroundMotionRecord object IDs from the database.
    def getFEMGroundMotionRecordIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGroundMotionRecord'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMGroundMotionRecord object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMGroundMotionRecord object fields
    # @return \c True on success, \c False if object not found
    def getFEMGroundMotionRecord(self, fields):
        selectCmd = 'SELECT '\
            'var_Description,var_GMRecordID,var_GMRecordName '\
            'FROM CLASS_FEMGroundMotionRecord WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDescription = data.pop(0)
        fields.mGMRecordID = data.pop(0)
        fields.mGMRecordName = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMGroundMotionRecord objects matching the given
    # reference data.
    #
    # @param fields FEMGroundMotionRecord object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMGroundMotionRecord objects
    def findEqualFEMGroundMotionRecord(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('GMRecordID' in fieldNames):
            cmds.append("var_GMRecordID=%s")
            args.append(fields.mGMRecordID)
        if (len(fieldNames)) == 0 or ('GMRecordName' in fieldNames):
            cmds.append("var_GMRecordName=%s")
            args.append(fields.mGMRecordName)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGroundMotionRecord WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMGeneralGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMGeneralGroup object fields
    def addFEMGeneralGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMGeneralGroup '\
            '(ref_Group,var_MatrixSet,var_Result,var_SkewSystem) VALUES '\
            "(%s,%s,%s,%s)"
        args = []
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mMatrixSet)
        args.append(fields.mResult)
        args.append(fields.mSkewSystem)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMGeneralGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMGeneralGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMGeneralGroup object fields
    def addFEMGeneralGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMGeneralGroup '\
            '(ref_Group,var_MatrixSet,var_Result,var_SkewSystem) VALUES '\
            "(%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mMatrixSet)
            args.append(fields.mResult)
            args.append(fields.mSkewSystem)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMGeneralGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMGeneralGroup object in the database.
    #
    # @param fields     FEMGeneralGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMGeneralGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('MatrixSet' in fieldNames):
            cmds.append("var_MatrixSet=%s")
            args.append(fields.mMatrixSet)
        if allFields or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if allFields or ('SkewSystem' in fieldNames):
            cmds.append("var_SkewSystem=%s")
            args.append(fields.mSkewSystem)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMGeneralGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMGeneralGroup object IDs from the database.
    def getFEMGeneralGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGeneralGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMGeneralGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMGeneralGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMGeneralGroup(self, fields):
        selectCmd = 'SELECT '\
            'ref_Group,var_MatrixSet,var_Result,var_SkewSystem '\
            'FROM CLASS_FEMGeneralGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMatrixSet = data.pop(0)
        fields.mResult = data.pop(0)
        fields.mSkewSystem = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMGeneralGroup objects matching the given
    # reference data.
    #
    # @param fields FEMGeneralGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMGeneralGroup objects
    def findEqualFEMGeneralGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('MatrixSet' in fieldNames):
            cmds.append("var_MatrixSet=%s")
            args.append(fields.mMatrixSet)
        if (len(fieldNames)) == 0 or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if (len(fieldNames)) == 0 or ('SkewSystem' in fieldNames):
            cmds.append("var_SkewSystem=%s")
            args.append(fields.mSkewSystem)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGeneralGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMTwoDSolid object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMTwoDSolid object fields
    def addFEMTwoDSolid(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMTwoDSolid '\
            '(var_Bet,var_Description,var_ElementID,ref_Group,var_MaterialID,ref_N1,ref_N2,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_TBirth,var_TDeath,var_Thick) VALUES '\
            "(%0.16g,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mBet)
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mMaterialID)
        if fields.mN1 == 0:
            args.append(None)
        else:
            args.append(fields.mN1)
        if fields.mN2 == 0:
            args.append(None)
        else:
            args.append(fields.mN2)
        if fields.mN3 == 0:
            args.append(None)
        else:
            args.append(fields.mN3)
        if fields.mN4 == 0:
            args.append(None)
        else:
            args.append(fields.mN4)
        if fields.mN5 == 0:
            args.append(None)
        else:
            args.append(fields.mN5)
        if fields.mN6 == 0:
            args.append(None)
        else:
            args.append(fields.mN6)
        if fields.mN7 == 0:
            args.append(None)
        else:
            args.append(fields.mN7)
        if fields.mN8 == 0:
            args.append(None)
        else:
            args.append(fields.mN8)
        if fields.mN9 == 0:
            args.append(None)
        else:
            args.append(fields.mN9)
        args.append(fields.mNodeAmount)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        args.append(fields.mThick)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMTwoDSolid_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMTwoDSolid objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMTwoDSolid object fields
    def addFEMTwoDSolidList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMTwoDSolid '\
            '(var_Bet,var_Description,var_ElementID,ref_Group,var_MaterialID,ref_N1,ref_N2,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_TBirth,var_TDeath,var_Thick) VALUES '\
            "(%0.16g,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mBet)
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mMaterialID)
            if fields.mN1 == 0:
                args.append(None)
            else:
                args.append(fields.mN1)
            if fields.mN2 == 0:
                args.append(None)
            else:
                args.append(fields.mN2)
            if fields.mN3 == 0:
                args.append(None)
            else:
                args.append(fields.mN3)
            if fields.mN4 == 0:
                args.append(None)
            else:
                args.append(fields.mN4)
            if fields.mN5 == 0:
                args.append(None)
            else:
                args.append(fields.mN5)
            if fields.mN6 == 0:
                args.append(None)
            else:
                args.append(fields.mN6)
            if fields.mN7 == 0:
                args.append(None)
            else:
                args.append(fields.mN7)
            if fields.mN8 == 0:
                args.append(None)
            else:
                args.append(fields.mN8)
            if fields.mN9 == 0:
                args.append(None)
            else:
                args.append(fields.mN9)
            args.append(fields.mNodeAmount)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            args.append(fields.mThick)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMTwoDSolid_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMTwoDSolid object in the database.
    #
    # @param fields     FEMTwoDSolid object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMTwoDSolid(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Bet' in fieldNames):
            cmds.append("var_Bet=%0.16g")
            args.append(fields.mBet)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if allFields or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if allFields or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if allFields or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if allFields or ('N5' in fieldNames):
            cmds.append("ref_N5=%s")
            args.append(fields.mN5)
        if allFields or ('N6' in fieldNames):
            cmds.append("ref_N6=%s")
            args.append(fields.mN6)
        if allFields or ('N7' in fieldNames):
            cmds.append("ref_N7=%s")
            args.append(fields.mN7)
        if allFields or ('N8' in fieldNames):
            cmds.append("ref_N8=%s")
            args.append(fields.mN8)
        if allFields or ('N9' in fieldNames):
            cmds.append("ref_N9=%s")
            args.append(fields.mN9)
        if allFields or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if allFields or ('Thick' in fieldNames):
            cmds.append("var_Thick=%0.16g")
            args.append(fields.mThick)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMTwoDSolid SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMTwoDSolid object IDs from the database.
    def getFEMTwoDSolidIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTwoDSolid'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMTwoDSolid object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMTwoDSolid object fields
    # @return \c True on success, \c False if object not found
    def getFEMTwoDSolid(self, fields):
        selectCmd = 'SELECT '\
            'var_Bet,var_Description,var_ElementID,ref_Group,var_MaterialID,ref_N1,ref_N2,ref_N3,ref_N4,ref_N5,ref_N6,ref_N7,ref_N8,ref_N9,var_NodeAmount,var_RecordNmb,var_Save,var_TBirth,var_TDeath,var_Thick '\
            'FROM CLASS_FEMTwoDSolid WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mBet = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterialID = data.pop(0)
        fields.mN1 = zeroNone(data.pop(0))
        fields.mN2 = zeroNone(data.pop(0))
        fields.mN3 = zeroNone(data.pop(0))
        fields.mN4 = zeroNone(data.pop(0))
        fields.mN5 = zeroNone(data.pop(0))
        fields.mN6 = zeroNone(data.pop(0))
        fields.mN7 = zeroNone(data.pop(0))
        fields.mN8 = zeroNone(data.pop(0))
        fields.mN9 = zeroNone(data.pop(0))
        fields.mNodeAmount = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        fields.mThick = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMTwoDSolid objects matching the given
    # reference data.
    #
    # @param fields FEMTwoDSolid object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMTwoDSolid objects
    def findEqualFEMTwoDSolid(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Bet' in fieldNames):
            cmds.append("var_Bet=%0.16g")
            args.append(fields.mBet)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if (len(fieldNames)) == 0 or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if (len(fieldNames)) == 0 or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if (len(fieldNames)) == 0 or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if (len(fieldNames)) == 0 or ('N5' in fieldNames):
            cmds.append("ref_N5=%s")
            args.append(fields.mN5)
        if (len(fieldNames)) == 0 or ('N6' in fieldNames):
            cmds.append("ref_N6=%s")
            args.append(fields.mN6)
        if (len(fieldNames)) == 0 or ('N7' in fieldNames):
            cmds.append("ref_N7=%s")
            args.append(fields.mN7)
        if (len(fieldNames)) == 0 or ('N8' in fieldNames):
            cmds.append("ref_N8=%s")
            args.append(fields.mN8)
        if (len(fieldNames)) == 0 or ('N9' in fieldNames):
            cmds.append("ref_N9=%s")
            args.append(fields.mN9)
        if (len(fieldNames)) == 0 or ('NodeAmount' in fieldNames):
            cmds.append("var_NodeAmount=%s")
            args.append(fields.mNodeAmount)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if (len(fieldNames)) == 0 or ('Thick' in fieldNames):
            cmds.append("var_Thick=%0.16g")
            args.append(fields.mThick)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMTwoDSolid WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMAppliedTemperature object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMAppliedTemperature object fields
    def addFEMAppliedTemperature(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMAppliedTemperature '\
            '(var_ArrivalTime,var_Factor,ref_Node,var_RecordNmbr,var_TimeFunctionID) VALUES '\
            "(%0.16g,%0.16g,%s,%s,%s)"
        args = []
        args.append(fields.mArrivalTime)
        args.append(fields.mFactor)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(fields.mRecordNmbr)
        args.append(fields.mTimeFunctionID)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMAppliedTemperature_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMAppliedTemperature objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMAppliedTemperature object fields
    def addFEMAppliedTemperatureList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMAppliedTemperature '\
            '(var_ArrivalTime,var_Factor,ref_Node,var_RecordNmbr,var_TimeFunctionID) VALUES '\
            "(%0.16g,%0.16g,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mArrivalTime)
            args.append(fields.mFactor)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(fields.mRecordNmbr)
            args.append(fields.mTimeFunctionID)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMAppliedTemperature_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMAppliedTemperature object in the database.
    #
    # @param fields     FEMAppliedTemperature object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMAppliedTemperature(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if allFields or ('Factor' in fieldNames):
            cmds.append("var_Factor=%0.16g")
            args.append(fields.mFactor)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('RecordNmbr' in fieldNames):
            cmds.append("var_RecordNmbr=%s")
            args.append(fields.mRecordNmbr)
        if allFields or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMAppliedTemperature SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMAppliedTemperature object IDs from the database.
    def getFEMAppliedTemperatureIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAppliedTemperature'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMAppliedTemperature object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMAppliedTemperature object fields
    # @return \c True on success, \c False if object not found
    def getFEMAppliedTemperature(self, fields):
        selectCmd = 'SELECT '\
            'var_ArrivalTime,var_Factor,ref_Node,var_RecordNmbr,var_TimeFunctionID '\
            'FROM CLASS_FEMAppliedTemperature WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mArrivalTime = data.pop(0)
        fields.mFactor = data.pop(0)
        fields.mNode = zeroNone(data.pop(0))
        fields.mRecordNmbr = data.pop(0)
        fields.mTimeFunctionID = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMAppliedTemperature objects matching the given
    # reference data.
    #
    # @param fields FEMAppliedTemperature object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMAppliedTemperature objects
    def findEqualFEMAppliedTemperature(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ArrivalTime' in fieldNames):
            cmds.append("var_ArrivalTime=%0.16g")
            args.append(fields.mArrivalTime)
        if (len(fieldNames)) == 0 or ('Factor' in fieldNames):
            cmds.append("var_Factor=%0.16g")
            args.append(fields.mFactor)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('RecordNmbr' in fieldNames):
            cmds.append("var_RecordNmbr=%s")
            args.append(fields.mRecordNmbr)
        if (len(fieldNames)) == 0 or ('TimeFunctionID' in fieldNames):
            cmds.append("var_TimeFunctionID=%s")
            args.append(fields.mTimeFunctionID)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMAppliedTemperature WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMMatrixSets object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMMatrixSets object fields
    def addFEMMatrixSets(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMMatrixSets '\
            '(var_Damping,var_Description,var_Mass,var_MatrixSetID,var_Stiffness,var_Stress) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mDamping)
        args.append(fields.mDescription)
        args.append(fields.mMass)
        args.append(fields.mMatrixSetID)
        args.append(fields.mStiffness)
        args.append(fields.mStress)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMMatrixSets_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMMatrixSets objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMMatrixSets object fields
    def addFEMMatrixSetsList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMMatrixSets '\
            '(var_Damping,var_Description,var_Mass,var_MatrixSetID,var_Stiffness,var_Stress) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDamping)
            args.append(fields.mDescription)
            args.append(fields.mMass)
            args.append(fields.mMatrixSetID)
            args.append(fields.mStiffness)
            args.append(fields.mStress)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMMatrixSets_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMMatrixSets object in the database.
    #
    # @param fields     FEMMatrixSets object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMMatrixSets(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Damping' in fieldNames):
            cmds.append("var_Damping=%s")
            args.append(fields.mDamping)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('Mass' in fieldNames):
            cmds.append("var_Mass=%s")
            args.append(fields.mMass)
        if allFields or ('MatrixSetID' in fieldNames):
            cmds.append("var_MatrixSetID=%s")
            args.append(fields.mMatrixSetID)
        if allFields or ('Stiffness' in fieldNames):
            cmds.append("var_Stiffness=%s")
            args.append(fields.mStiffness)
        if allFields or ('Stress' in fieldNames):
            cmds.append("var_Stress=%s")
            args.append(fields.mStress)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMMatrixSets SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMMatrixSets object IDs from the database.
    def getFEMMatrixSetsIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMatrixSets'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMMatrixSets object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMMatrixSets object fields
    # @return \c True on success, \c False if object not found
    def getFEMMatrixSets(self, fields):
        selectCmd = 'SELECT '\
            'var_Damping,var_Description,var_Mass,var_MatrixSetID,var_Stiffness,var_Stress '\
            'FROM CLASS_FEMMatrixSets WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDamping = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mMass = data.pop(0)
        fields.mMatrixSetID = data.pop(0)
        fields.mStiffness = data.pop(0)
        fields.mStress = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMMatrixSets objects matching the given
    # reference data.
    #
    # @param fields FEMMatrixSets object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMMatrixSets objects
    def findEqualFEMMatrixSets(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Damping' in fieldNames):
            cmds.append("var_Damping=%s")
            args.append(fields.mDamping)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('Mass' in fieldNames):
            cmds.append("var_Mass=%s")
            args.append(fields.mMass)
        if (len(fieldNames)) == 0 or ('MatrixSetID' in fieldNames):
            cmds.append("var_MatrixSetID=%s")
            args.append(fields.mMatrixSetID)
        if (len(fieldNames)) == 0 or ('Stiffness' in fieldNames):
            cmds.append("var_Stiffness=%s")
            args.append(fields.mStiffness)
        if (len(fieldNames)) == 0 or ('Stress' in fieldNames):
            cmds.append("var_Stress=%s")
            args.append(fields.mStress)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMatrixSets WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMConstraintCoef object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMConstraintCoef object fields
    def addFEMConstraintCoef(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMConstraintCoef '\
            '(var_Coefficient,var_ConstraintID,var_Description,var_MasterDOF,var_MasterNode,var_RecordNmb) VALUES '\
            "(%0.16g,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mCoefficient)
        args.append(fields.mConstraintID)
        args.append(fields.mDescription)
        args.append(fields.mMasterDOF)
        args.append(fields.mMasterNode)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMConstraintCoef_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMConstraintCoef objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMConstraintCoef object fields
    def addFEMConstraintCoefList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMConstraintCoef '\
            '(var_Coefficient,var_ConstraintID,var_Description,var_MasterDOF,var_MasterNode,var_RecordNmb) VALUES '\
            "(%0.16g,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mCoefficient)
            args.append(fields.mConstraintID)
            args.append(fields.mDescription)
            args.append(fields.mMasterDOF)
            args.append(fields.mMasterNode)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMConstraintCoef_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMConstraintCoef object in the database.
    #
    # @param fields     FEMConstraintCoef object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMConstraintCoef(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Coefficient' in fieldNames):
            cmds.append("var_Coefficient=%0.16g")
            args.append(fields.mCoefficient)
        if allFields or ('ConstraintID' in fieldNames):
            cmds.append("var_ConstraintID=%s")
            args.append(fields.mConstraintID)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('MasterDOF' in fieldNames):
            cmds.append("var_MasterDOF=%s")
            args.append(fields.mMasterDOF)
        if allFields or ('MasterNode' in fieldNames):
            cmds.append("var_MasterNode=%s")
            args.append(fields.mMasterNode)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMConstraintCoef SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMConstraintCoef object IDs from the database.
    def getFEMConstraintCoefIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMConstraintCoef'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMConstraintCoef object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMConstraintCoef object fields
    # @return \c True on success, \c False if object not found
    def getFEMConstraintCoef(self, fields):
        selectCmd = 'SELECT '\
            'var_Coefficient,var_ConstraintID,var_Description,var_MasterDOF,var_MasterNode,var_RecordNmb '\
            'FROM CLASS_FEMConstraintCoef WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mCoefficient = data.pop(0)
        fields.mConstraintID = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mMasterDOF = data.pop(0)
        fields.mMasterNode = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMConstraintCoef objects matching the given
    # reference data.
    #
    # @param fields FEMConstraintCoef object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMConstraintCoef objects
    def findEqualFEMConstraintCoef(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Coefficient' in fieldNames):
            cmds.append("var_Coefficient=%0.16g")
            args.append(fields.mCoefficient)
        if (len(fieldNames)) == 0 or ('ConstraintID' in fieldNames):
            cmds.append("var_ConstraintID=%s")
            args.append(fields.mConstraintID)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('MasterDOF' in fieldNames):
            cmds.append("var_MasterDOF=%s")
            args.append(fields.mMasterDOF)
        if (len(fieldNames)) == 0 or ('MasterNode' in fieldNames):
            cmds.append("var_MasterNode=%s")
            args.append(fields.mMasterNode)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMConstraintCoef WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSectionBox object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSectionBox object fields
    def addFEMSectionBox(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSectionBox '\
            '(var_Height,var_SC,var_SectionID,var_SSarea,var_TC,var_Thick1,var_Thick2,var_Torfac,var_TSarea,var_Width) VALUES '\
            "(%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        args = []
        args.append(fields.mHeight)
        args.append(fields.mSC)
        args.append(fields.mSectionID)
        args.append(fields.mSSarea)
        args.append(fields.mTC)
        args.append(fields.mThick1)
        args.append(fields.mThick2)
        args.append(fields.mTorfac)
        args.append(fields.mTSarea)
        args.append(fields.mWidth)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSectionBox_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSectionBox objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSectionBox object fields
    def addFEMSectionBoxList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSectionBox '\
            '(var_Height,var_SC,var_SectionID,var_SSarea,var_TC,var_Thick1,var_Thick2,var_Torfac,var_TSarea,var_Width) VALUES '\
            "(%0.16g,%0.16g,%s,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mHeight)
            args.append(fields.mSC)
            args.append(fields.mSectionID)
            args.append(fields.mSSarea)
            args.append(fields.mTC)
            args.append(fields.mThick1)
            args.append(fields.mThick2)
            args.append(fields.mTorfac)
            args.append(fields.mTSarea)
            args.append(fields.mWidth)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSectionBox_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSectionBox object in the database.
    #
    # @param fields     FEMSectionBox object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSectionBox(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Height' in fieldNames):
            cmds.append("var_Height=%0.16g")
            args.append(fields.mHeight)
        if allFields or ('SC' in fieldNames):
            cmds.append("var_SC=%0.16g")
            args.append(fields.mSC)
        if allFields or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if allFields or ('SSarea' in fieldNames):
            cmds.append("var_SSarea=%0.16g")
            args.append(fields.mSSarea)
        if allFields or ('TC' in fieldNames):
            cmds.append("var_TC=%0.16g")
            args.append(fields.mTC)
        if allFields or ('Thick1' in fieldNames):
            cmds.append("var_Thick1=%0.16g")
            args.append(fields.mThick1)
        if allFields or ('Thick2' in fieldNames):
            cmds.append("var_Thick2=%0.16g")
            args.append(fields.mThick2)
        if allFields or ('Torfac' in fieldNames):
            cmds.append("var_Torfac=%0.16g")
            args.append(fields.mTorfac)
        if allFields or ('TSarea' in fieldNames):
            cmds.append("var_TSarea=%0.16g")
            args.append(fields.mTSarea)
        if allFields or ('Width' in fieldNames):
            cmds.append("var_Width=%0.16g")
            args.append(fields.mWidth)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSectionBox SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSectionBox object IDs from the database.
    def getFEMSectionBoxIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionBox'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSectionBox object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSectionBox object fields
    # @return \c True on success, \c False if object not found
    def getFEMSectionBox(self, fields):
        selectCmd = 'SELECT '\
            'var_Height,var_SC,var_SectionID,var_SSarea,var_TC,var_Thick1,var_Thick2,var_Torfac,var_TSarea,var_Width '\
            'FROM CLASS_FEMSectionBox WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mHeight = data.pop(0)
        fields.mSC = data.pop(0)
        fields.mSectionID = data.pop(0)
        fields.mSSarea = data.pop(0)
        fields.mTC = data.pop(0)
        fields.mThick1 = data.pop(0)
        fields.mThick2 = data.pop(0)
        fields.mTorfac = data.pop(0)
        fields.mTSarea = data.pop(0)
        fields.mWidth = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSectionBox objects matching the given
    # reference data.
    #
    # @param fields FEMSectionBox object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSectionBox objects
    def findEqualFEMSectionBox(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Height' in fieldNames):
            cmds.append("var_Height=%0.16g")
            args.append(fields.mHeight)
        if (len(fieldNames)) == 0 or ('SC' in fieldNames):
            cmds.append("var_SC=%0.16g")
            args.append(fields.mSC)
        if (len(fieldNames)) == 0 or ('SectionID' in fieldNames):
            cmds.append("var_SectionID=%s")
            args.append(fields.mSectionID)
        if (len(fieldNames)) == 0 or ('SSarea' in fieldNames):
            cmds.append("var_SSarea=%0.16g")
            args.append(fields.mSSarea)
        if (len(fieldNames)) == 0 or ('TC' in fieldNames):
            cmds.append("var_TC=%0.16g")
            args.append(fields.mTC)
        if (len(fieldNames)) == 0 or ('Thick1' in fieldNames):
            cmds.append("var_Thick1=%0.16g")
            args.append(fields.mThick1)
        if (len(fieldNames)) == 0 or ('Thick2' in fieldNames):
            cmds.append("var_Thick2=%0.16g")
            args.append(fields.mThick2)
        if (len(fieldNames)) == 0 or ('Torfac' in fieldNames):
            cmds.append("var_Torfac=%0.16g")
            args.append(fields.mTorfac)
        if (len(fieldNames)) == 0 or ('TSarea' in fieldNames):
            cmds.append("var_TSarea=%0.16g")
            args.append(fields.mTSarea)
        if (len(fieldNames)) == 0 or ('Width' in fieldNames):
            cmds.append("var_Width=%0.16g")
            args.append(fields.mWidth)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSectionBox WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMNKDisplForce object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMNKDisplForce object fields
    def addFEMNKDisplForce(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMNKDisplForce '\
            '(var_Displacement,var_Force,var_PropertyID,var_RecordNmb) VALUES '\
            "(%0.16g,%0.16g,%s,%s)"
        args = []
        args.append(fields.mDisplacement)
        args.append(fields.mForce)
        args.append(fields.mPropertyID)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMNKDisplForce_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMNKDisplForce objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMNKDisplForce object fields
    def addFEMNKDisplForceList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMNKDisplForce '\
            '(var_Displacement,var_Force,var_PropertyID,var_RecordNmb) VALUES '\
            "(%0.16g,%0.16g,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDisplacement)
            args.append(fields.mForce)
            args.append(fields.mPropertyID)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMNKDisplForce_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMNKDisplForce object in the database.
    #
    # @param fields     FEMNKDisplForce object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMNKDisplForce(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%0.16g")
            args.append(fields.mDisplacement)
        if allFields or ('Force' in fieldNames):
            cmds.append("var_Force=%0.16g")
            args.append(fields.mForce)
        if allFields or ('PropertyID' in fieldNames):
            cmds.append("var_PropertyID=%s")
            args.append(fields.mPropertyID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMNKDisplForce SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMNKDisplForce object IDs from the database.
    def getFEMNKDisplForceIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNKDisplForce'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMNKDisplForce object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMNKDisplForce object fields
    # @return \c True on success, \c False if object not found
    def getFEMNKDisplForce(self, fields):
        selectCmd = 'SELECT '\
            'var_Displacement,var_Force,var_PropertyID,var_RecordNmb '\
            'FROM CLASS_FEMNKDisplForce WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDisplacement = data.pop(0)
        fields.mForce = data.pop(0)
        fields.mPropertyID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMNKDisplForce objects matching the given
    # reference data.
    #
    # @param fields FEMNKDisplForce object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMNKDisplForce objects
    def findEqualFEMNKDisplForce(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%0.16g")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('Force' in fieldNames):
            cmds.append("var_Force=%0.16g")
            args.append(fields.mForce)
        if (len(fieldNames)) == 0 or ('PropertyID' in fieldNames):
            cmds.append("var_PropertyID=%s")
            args.append(fields.mPropertyID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMNKDisplForce WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMPlasticStrainStress object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMPlasticStrainStress object fields
    def addFEMPlasticStrainStress(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMPlasticStrainStress '\
            '(var_MaterialID,var_RecordNumber,var_Strain,var_Stress) VALUES '\
            "(%s,%s,%0.16g,%0.16g)"
        args = []
        args.append(fields.mMaterialID)
        args.append(fields.mRecordNumber)
        args.append(fields.mStrain)
        args.append(fields.mStress)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMPlasticStrainStress_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMPlasticStrainStress objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMPlasticStrainStress object fields
    def addFEMPlasticStrainStressList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMPlasticStrainStress '\
            '(var_MaterialID,var_RecordNumber,var_Strain,var_Stress) VALUES '\
            "(%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mMaterialID)
            args.append(fields.mRecordNumber)
            args.append(fields.mStrain)
            args.append(fields.mStress)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMPlasticStrainStress_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMPlasticStrainStress object in the database.
    #
    # @param fields     FEMPlasticStrainStress object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMPlasticStrainStress(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if allFields or ('RecordNumber' in fieldNames):
            cmds.append("var_RecordNumber=%s")
            args.append(fields.mRecordNumber)
        if allFields or ('Strain' in fieldNames):
            cmds.append("var_Strain=%0.16g")
            args.append(fields.mStrain)
        if allFields or ('Stress' in fieldNames):
            cmds.append("var_Stress=%0.16g")
            args.append(fields.mStress)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMPlasticStrainStress SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMPlasticStrainStress object IDs from the database.
    def getFEMPlasticStrainStressIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlasticStrainStress'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMPlasticStrainStress object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMPlasticStrainStress object fields
    # @return \c True on success, \c False if object not found
    def getFEMPlasticStrainStress(self, fields):
        selectCmd = 'SELECT '\
            'var_MaterialID,var_RecordNumber,var_Strain,var_Stress '\
            'FROM CLASS_FEMPlasticStrainStress WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mMaterialID = data.pop(0)
        fields.mRecordNumber = data.pop(0)
        fields.mStrain = data.pop(0)
        fields.mStress = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMPlasticStrainStress objects matching the given
    # reference data.
    #
    # @param fields FEMPlasticStrainStress object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMPlasticStrainStress objects
    def findEqualFEMPlasticStrainStress(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('MaterialID' in fieldNames):
            cmds.append("var_MaterialID=%s")
            args.append(fields.mMaterialID)
        if (len(fieldNames)) == 0 or ('RecordNumber' in fieldNames):
            cmds.append("var_RecordNumber=%s")
            args.append(fields.mRecordNumber)
        if (len(fieldNames)) == 0 or ('Strain' in fieldNames):
            cmds.append("var_Strain=%0.16g")
            args.append(fields.mStrain)
        if (len(fieldNames)) == 0 or ('Stress' in fieldNames):
            cmds.append("var_Stress=%0.16g")
            args.append(fields.mStress)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMPlasticStrainStress WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMShellAxesOrthoData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMShellAxesOrthoData object fields
    def addFEMShellAxesOrthoData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMShellAxesOrthoData '\
            '(var_AxOrthoID,var_ElementID,var_RecordNmb) VALUES '\
            "(%s,%s,%s)"
        args = []
        args.append(fields.mAxOrthoID)
        args.append(fields.mElementID)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMShellAxesOrthoData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMShellAxesOrthoData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMShellAxesOrthoData object fields
    def addFEMShellAxesOrthoDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMShellAxesOrthoData '\
            '(var_AxOrthoID,var_ElementID,var_RecordNmb) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAxOrthoID)
            args.append(fields.mElementID)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMShellAxesOrthoData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMShellAxesOrthoData object in the database.
    #
    # @param fields     FEMShellAxesOrthoData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMShellAxesOrthoData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AxOrthoID' in fieldNames):
            cmds.append("var_AxOrthoID=%s")
            args.append(fields.mAxOrthoID)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMShellAxesOrthoData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMShellAxesOrthoData object IDs from the database.
    def getFEMShellAxesOrthoDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellAxesOrthoData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMShellAxesOrthoData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMShellAxesOrthoData object fields
    # @return \c True on success, \c False if object not found
    def getFEMShellAxesOrthoData(self, fields):
        selectCmd = 'SELECT '\
            'var_AxOrthoID,var_ElementID,var_RecordNmb '\
            'FROM CLASS_FEMShellAxesOrthoData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAxOrthoID = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMShellAxesOrthoData objects matching the given
    # reference data.
    #
    # @param fields FEMShellAxesOrthoData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMShellAxesOrthoData objects
    def findEqualFEMShellAxesOrthoData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AxOrthoID' in fieldNames):
            cmds.append("var_AxOrthoID=%s")
            args.append(fields.mAxOrthoID)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellAxesOrthoData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMGeneralNode object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMGeneralNode object fields
    def addFEMGeneralNode(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMGeneralNode '\
            '(var_ElementID,ref_Group,var_LocalNmb,ref_Node,var_RecordNmb) VALUES '\
            "(%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mElementID)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mLocalNmb)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMGeneralNode_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMGeneralNode objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMGeneralNode object fields
    def addFEMGeneralNodeList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMGeneralNode '\
            '(var_ElementID,ref_Group,var_LocalNmb,ref_Node,var_RecordNmb) VALUES '\
            "(%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mElementID)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mLocalNmb)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMGeneralNode_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMGeneralNode object in the database.
    #
    # @param fields     FEMGeneralNode object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMGeneralNode(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('LocalNmb' in fieldNames):
            cmds.append("var_LocalNmb=%s")
            args.append(fields.mLocalNmb)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMGeneralNode SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMGeneralNode object IDs from the database.
    def getFEMGeneralNodeIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGeneralNode'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMGeneralNode object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMGeneralNode object fields
    # @return \c True on success, \c False if object not found
    def getFEMGeneralNode(self, fields):
        selectCmd = 'SELECT '\
            'var_ElementID,ref_Group,var_LocalNmb,ref_Node,var_RecordNmb '\
            'FROM CLASS_FEMGeneralNode WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mElementID = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mLocalNmb = data.pop(0)
        fields.mNode = zeroNone(data.pop(0))
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMGeneralNode objects matching the given
    # reference data.
    #
    # @param fields FEMGeneralNode object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMGeneralNode objects
    def findEqualFEMGeneralNode(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('LocalNmb' in fieldNames):
            cmds.append("var_LocalNmb=%s")
            args.append(fields.mLocalNmb)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMGeneralNode WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMStrLines object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMStrLines object fields
    def addFEMStrLines(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMStrLines '\
            '(var_LineID,var_P1,var_P2) VALUES '\
            "(%s,%s,%s)"
        args = []
        args.append(fields.mLineID)
        args.append(fields.mP1)
        args.append(fields.mP2)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMStrLines_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMStrLines objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMStrLines object fields
    def addFEMStrLinesList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMStrLines '\
            '(var_LineID,var_P1,var_P2) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mLineID)
            args.append(fields.mP1)
            args.append(fields.mP2)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMStrLines_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMStrLines object in the database.
    #
    # @param fields     FEMStrLines object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMStrLines(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('LineID' in fieldNames):
            cmds.append("var_LineID=%s")
            args.append(fields.mLineID)
        if allFields or ('P1' in fieldNames):
            cmds.append("var_P1=%s")
            args.append(fields.mP1)
        if allFields or ('P2' in fieldNames):
            cmds.append("var_P2=%s")
            args.append(fields.mP2)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMStrLines SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMStrLines object IDs from the database.
    def getFEMStrLinesIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMStrLines'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMStrLines object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMStrLines object fields
    # @return \c True on success, \c False if object not found
    def getFEMStrLines(self, fields):
        selectCmd = 'SELECT '\
            'var_LineID,var_P1,var_P2 '\
            'FROM CLASS_FEMStrLines WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mLineID = data.pop(0)
        fields.mP1 = data.pop(0)
        fields.mP2 = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMStrLines objects matching the given
    # reference data.
    #
    # @param fields FEMStrLines object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMStrLines objects
    def findEqualFEMStrLines(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('LineID' in fieldNames):
            cmds.append("var_LineID=%s")
            args.append(fields.mLineID)
        if (len(fieldNames)) == 0 or ('P1' in fieldNames):
            cmds.append("var_P1=%s")
            args.append(fields.mP1)
        if (len(fieldNames)) == 0 or ('P2' in fieldNames):
            cmds.append("var_P2=%s")
            args.append(fields.mP2)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMStrLines WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMContactSurface object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMContactSurface object fields
    def addFEMContactSurface(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMContactSurface '\
            '(var_ContGroupID,var_ContSegment,var_ContSurface,ref_N1,ref_N2,ref_N3,ref_N4,var_RecordNmb) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mContGroupID)
        args.append(fields.mContSegment)
        args.append(fields.mContSurface)
        if fields.mN1 == 0:
            args.append(None)
        else:
            args.append(fields.mN1)
        if fields.mN2 == 0:
            args.append(None)
        else:
            args.append(fields.mN2)
        if fields.mN3 == 0:
            args.append(None)
        else:
            args.append(fields.mN3)
        if fields.mN4 == 0:
            args.append(None)
        else:
            args.append(fields.mN4)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMContactSurface_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMContactSurface objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMContactSurface object fields
    def addFEMContactSurfaceList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMContactSurface '\
            '(var_ContGroupID,var_ContSegment,var_ContSurface,ref_N1,ref_N2,ref_N3,ref_N4,var_RecordNmb) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mContGroupID)
            args.append(fields.mContSegment)
            args.append(fields.mContSurface)
            if fields.mN1 == 0:
                args.append(None)
            else:
                args.append(fields.mN1)
            if fields.mN2 == 0:
                args.append(None)
            else:
                args.append(fields.mN2)
            if fields.mN3 == 0:
                args.append(None)
            else:
                args.append(fields.mN3)
            if fields.mN4 == 0:
                args.append(None)
            else:
                args.append(fields.mN4)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMContactSurface_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMContactSurface object in the database.
    #
    # @param fields     FEMContactSurface object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMContactSurface(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('ContGroupID' in fieldNames):
            cmds.append("var_ContGroupID=%s")
            args.append(fields.mContGroupID)
        if allFields or ('ContSegment' in fieldNames):
            cmds.append("var_ContSegment=%s")
            args.append(fields.mContSegment)
        if allFields or ('ContSurface' in fieldNames):
            cmds.append("var_ContSurface=%s")
            args.append(fields.mContSurface)
        if allFields or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if allFields or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if allFields or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if allFields or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMContactSurface SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMContactSurface object IDs from the database.
    def getFEMContactSurfaceIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMContactSurface'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMContactSurface object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMContactSurface object fields
    # @return \c True on success, \c False if object not found
    def getFEMContactSurface(self, fields):
        selectCmd = 'SELECT '\
            'var_ContGroupID,var_ContSegment,var_ContSurface,ref_N1,ref_N2,ref_N3,ref_N4,var_RecordNmb '\
            'FROM CLASS_FEMContactSurface WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mContGroupID = data.pop(0)
        fields.mContSegment = data.pop(0)
        fields.mContSurface = data.pop(0)
        fields.mN1 = zeroNone(data.pop(0))
        fields.mN2 = zeroNone(data.pop(0))
        fields.mN3 = zeroNone(data.pop(0))
        fields.mN4 = zeroNone(data.pop(0))
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMContactSurface objects matching the given
    # reference data.
    #
    # @param fields FEMContactSurface object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMContactSurface objects
    def findEqualFEMContactSurface(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('ContGroupID' in fieldNames):
            cmds.append("var_ContGroupID=%s")
            args.append(fields.mContGroupID)
        if (len(fieldNames)) == 0 or ('ContSegment' in fieldNames):
            cmds.append("var_ContSegment=%s")
            args.append(fields.mContSegment)
        if (len(fieldNames)) == 0 or ('ContSurface' in fieldNames):
            cmds.append("var_ContSurface=%s")
            args.append(fields.mContSurface)
        if (len(fieldNames)) == 0 or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if (len(fieldNames)) == 0 or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if (len(fieldNames)) == 0 or ('N3' in fieldNames):
            cmds.append("ref_N3=%s")
            args.append(fields.mN3)
        if (len(fieldNames)) == 0 or ('N4' in fieldNames):
            cmds.append("ref_N4=%s")
            args.append(fields.mN4)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMContactSurface WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMMCForceData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMMCForceData object fields
    def addFEMMCForceData(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMMCForceData '\
            '(var_CurvMomentID,var_Force,var_MomentSTID,var_RecordNmb) VALUES '\
            "(%s,%0.16g,%s,%s)"
        args = []
        args.append(fields.mCurvMomentID)
        args.append(fields.mForce)
        args.append(fields.mMomentSTID)
        args.append(fields.mRecordNmb)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMMCForceData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMMCForceData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMMCForceData object fields
    def addFEMMCForceDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMMCForceData '\
            '(var_CurvMomentID,var_Force,var_MomentSTID,var_RecordNmb) VALUES '\
            "(%s,%0.16g,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mCurvMomentID)
            args.append(fields.mForce)
            args.append(fields.mMomentSTID)
            args.append(fields.mRecordNmb)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMMCForceData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMMCForceData object in the database.
    #
    # @param fields     FEMMCForceData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMMCForceData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('CurvMomentID' in fieldNames):
            cmds.append("var_CurvMomentID=%s")
            args.append(fields.mCurvMomentID)
        if allFields or ('Force' in fieldNames):
            cmds.append("var_Force=%0.16g")
            args.append(fields.mForce)
        if allFields or ('MomentSTID' in fieldNames):
            cmds.append("var_MomentSTID=%s")
            args.append(fields.mMomentSTID)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMMCForceData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMMCForceData object IDs from the database.
    def getFEMMCForceDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMCForceData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMMCForceData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMMCForceData object fields
    # @return \c True on success, \c False if object not found
    def getFEMMCForceData(self, fields):
        selectCmd = 'SELECT '\
            'var_CurvMomentID,var_Force,var_MomentSTID,var_RecordNmb '\
            'FROM CLASS_FEMMCForceData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mCurvMomentID = data.pop(0)
        fields.mForce = data.pop(0)
        fields.mMomentSTID = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMMCForceData objects matching the given
    # reference data.
    #
    # @param fields FEMMCForceData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMMCForceData objects
    def findEqualFEMMCForceData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('CurvMomentID' in fieldNames):
            cmds.append("var_CurvMomentID=%s")
            args.append(fields.mCurvMomentID)
        if (len(fieldNames)) == 0 or ('Force' in fieldNames):
            cmds.append("var_Force=%0.16g")
            args.append(fields.mForce)
        if (len(fieldNames)) == 0 or ('MomentSTID' in fieldNames):
            cmds.append("var_MomentSTID=%s")
            args.append(fields.mMomentSTID)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMMCForceData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSpring object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSpring object fields
    def addFEMSpring(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSpring '\
            '(var_AX,var_AY,var_AZ,var_Description,var_ElementID,ref_Group,var_ID1,var_ID2,ref_N1,ref_N2,var_PropertySet,var_RecordNmb,var_Save,var_TBirth,var_TDeath) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g)"
        args = []
        args.append(fields.mAX)
        args.append(fields.mAY)
        args.append(fields.mAZ)
        args.append(fields.mDescription)
        args.append(fields.mElementID)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mID1)
        args.append(fields.mID2)
        if fields.mN1 == 0:
            args.append(None)
        else:
            args.append(fields.mN1)
        if fields.mN2 == 0:
            args.append(None)
        else:
            args.append(fields.mN2)
        args.append(fields.mPropertySet)
        args.append(fields.mRecordNmb)
        args.append(fields.mSave)
        args.append(fields.mTBirth)
        args.append(fields.mTDeath)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSpring_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSpring objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSpring object fields
    def addFEMSpringList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSpring '\
            '(var_AX,var_AY,var_AZ,var_Description,var_ElementID,ref_Group,var_ID1,var_ID2,ref_N1,ref_N2,var_PropertySet,var_RecordNmb,var_Save,var_TBirth,var_TDeath) VALUES '\
            "(%0.16g,%0.16g,%0.16g,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%0.16g,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mAX)
            args.append(fields.mAY)
            args.append(fields.mAZ)
            args.append(fields.mDescription)
            args.append(fields.mElementID)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mID1)
            args.append(fields.mID2)
            if fields.mN1 == 0:
                args.append(None)
            else:
                args.append(fields.mN1)
            if fields.mN2 == 0:
                args.append(None)
            else:
                args.append(fields.mN2)
            args.append(fields.mPropertySet)
            args.append(fields.mRecordNmb)
            args.append(fields.mSave)
            args.append(fields.mTBirth)
            args.append(fields.mTDeath)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSpring_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSpring object in the database.
    #
    # @param fields     FEMSpring object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSpring(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('AX' in fieldNames):
            cmds.append("var_AX=%0.16g")
            args.append(fields.mAX)
        if allFields or ('AY' in fieldNames):
            cmds.append("var_AY=%0.16g")
            args.append(fields.mAY)
        if allFields or ('AZ' in fieldNames):
            cmds.append("var_AZ=%0.16g")
            args.append(fields.mAZ)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('ID1' in fieldNames):
            cmds.append("var_ID1=%s")
            args.append(fields.mID1)
        if allFields or ('ID2' in fieldNames):
            cmds.append("var_ID2=%s")
            args.append(fields.mID2)
        if allFields or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if allFields or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if allFields or ('PropertySet' in fieldNames):
            cmds.append("var_PropertySet=%s")
            args.append(fields.mPropertySet)
        if allFields or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if allFields or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if allFields or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if allFields or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSpring SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSpring object IDs from the database.
    def getFEMSpringIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSpring'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSpring object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSpring object fields
    # @return \c True on success, \c False if object not found
    def getFEMSpring(self, fields):
        selectCmd = 'SELECT '\
            'var_AX,var_AY,var_AZ,var_Description,var_ElementID,ref_Group,var_ID1,var_ID2,ref_N1,ref_N2,var_PropertySet,var_RecordNmb,var_Save,var_TBirth,var_TDeath '\
            'FROM CLASS_FEMSpring WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mAX = data.pop(0)
        fields.mAY = data.pop(0)
        fields.mAZ = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mElementID = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mID1 = data.pop(0)
        fields.mID2 = data.pop(0)
        fields.mN1 = zeroNone(data.pop(0))
        fields.mN2 = zeroNone(data.pop(0))
        fields.mPropertySet = data.pop(0)
        fields.mRecordNmb = data.pop(0)
        fields.mSave = data.pop(0)
        fields.mTBirth = data.pop(0)
        fields.mTDeath = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSpring objects matching the given
    # reference data.
    #
    # @param fields FEMSpring object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSpring objects
    def findEqualFEMSpring(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('AX' in fieldNames):
            cmds.append("var_AX=%0.16g")
            args.append(fields.mAX)
        if (len(fieldNames)) == 0 or ('AY' in fieldNames):
            cmds.append("var_AY=%0.16g")
            args.append(fields.mAY)
        if (len(fieldNames)) == 0 or ('AZ' in fieldNames):
            cmds.append("var_AZ=%0.16g")
            args.append(fields.mAZ)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('ElementID' in fieldNames):
            cmds.append("var_ElementID=%s")
            args.append(fields.mElementID)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('ID1' in fieldNames):
            cmds.append("var_ID1=%s")
            args.append(fields.mID1)
        if (len(fieldNames)) == 0 or ('ID2' in fieldNames):
            cmds.append("var_ID2=%s")
            args.append(fields.mID2)
        if (len(fieldNames)) == 0 or ('N1' in fieldNames):
            cmds.append("ref_N1=%s")
            args.append(fields.mN1)
        if (len(fieldNames)) == 0 or ('N2' in fieldNames):
            cmds.append("ref_N2=%s")
            args.append(fields.mN2)
        if (len(fieldNames)) == 0 or ('PropertySet' in fieldNames):
            cmds.append("var_PropertySet=%s")
            args.append(fields.mPropertySet)
        if (len(fieldNames)) == 0 or ('RecordNmb' in fieldNames):
            cmds.append("var_RecordNmb=%s")
            args.append(fields.mRecordNmb)
        if (len(fieldNames)) == 0 or ('Save' in fieldNames):
            cmds.append("var_Save=%s")
            args.append(fields.mSave)
        if (len(fieldNames)) == 0 or ('TBirth' in fieldNames):
            cmds.append("var_TBirth=%0.16g")
            args.append(fields.mTBirth)
        if (len(fieldNames)) == 0 or ('TDeath' in fieldNames):
            cmds.append("var_TDeath=%0.16g")
            args.append(fields.mTDeath)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSpring WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMSpringGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMSpringGroup object fields
    def addFEMSpringGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMSpringGroup '\
            '(var_Bolt,ref_Group,var_Nonlinear,var_PropertySet,var_Result,var_SkewSystem) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        args = []
        args.append(fields.mBolt)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        args.append(fields.mNonlinear)
        args.append(fields.mPropertySet)
        args.append(fields.mResult)
        args.append(fields.mSkewSystem)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMSpringGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMSpringGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMSpringGroup object fields
    def addFEMSpringGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMSpringGroup '\
            '(var_Bolt,ref_Group,var_Nonlinear,var_PropertySet,var_Result,var_SkewSystem) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mBolt)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            args.append(fields.mNonlinear)
            args.append(fields.mPropertySet)
            args.append(fields.mResult)
            args.append(fields.mSkewSystem)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMSpringGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMSpringGroup object in the database.
    #
    # @param fields     FEMSpringGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMSpringGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Bolt' in fieldNames):
            cmds.append("var_Bolt=%s")
            args.append(fields.mBolt)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('Nonlinear' in fieldNames):
            cmds.append("var_Nonlinear=%s")
            args.append(fields.mNonlinear)
        if allFields or ('PropertySet' in fieldNames):
            cmds.append("var_PropertySet=%s")
            args.append(fields.mPropertySet)
        if allFields or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if allFields or ('SkewSystem' in fieldNames):
            cmds.append("var_SkewSystem=%s")
            args.append(fields.mSkewSystem)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMSpringGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMSpringGroup object IDs from the database.
    def getFEMSpringGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSpringGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMSpringGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMSpringGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMSpringGroup(self, fields):
        selectCmd = 'SELECT '\
            'var_Bolt,ref_Group,var_Nonlinear,var_PropertySet,var_Result,var_SkewSystem '\
            'FROM CLASS_FEMSpringGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mBolt = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mNonlinear = data.pop(0)
        fields.mPropertySet = data.pop(0)
        fields.mResult = data.pop(0)
        fields.mSkewSystem = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMSpringGroup objects matching the given
    # reference data.
    #
    # @param fields FEMSpringGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMSpringGroup objects
    def findEqualFEMSpringGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Bolt' in fieldNames):
            cmds.append("var_Bolt=%s")
            args.append(fields.mBolt)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('Nonlinear' in fieldNames):
            cmds.append("var_Nonlinear=%s")
            args.append(fields.mNonlinear)
        if (len(fieldNames)) == 0 or ('PropertySet' in fieldNames):
            cmds.append("var_PropertySet=%s")
            args.append(fields.mPropertySet)
        if (len(fieldNames)) == 0 or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if (len(fieldNames)) == 0 or ('SkewSystem' in fieldNames):
            cmds.append("var_SkewSystem=%s")
            args.append(fields.mSkewSystem)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMSpringGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a FEMShellGroup object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  FEMShellGroup object fields
    def addFEMShellGroup(self, fields):
        insertCmd = 'INSERT INTO CLASS_FEMShellGroup '\
            '(var_Displacement,ref_Group,ref_Material,var_NLayers,var_Result,var_SectionResult,var_StressReference,var_Thickness) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%0.16g)"
        args = []
        args.append(fields.mDisplacement)
        if fields.mGroup == 0:
            args.append(None)
        else:
            args.append(fields.mGroup)
        if fields.mMaterial == 0:
            args.append(None)
        else:
            args.append(fields.mMaterial)
        args.append(fields.mNLayers)
        args.append(fields.mResult)
        args.append(fields.mSectionResult)
        args.append(fields.mStressReference)
        args.append(fields.mThickness)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_FEMShellGroup_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of FEMShellGroup objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of FEMShellGroup object fields
    def addFEMShellGroupList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_FEMShellGroup '\
            '(var_Displacement,ref_Group,ref_Material,var_NLayers,var_Result,var_SectionResult,var_StressReference,var_Thickness) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%0.16g)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mDisplacement)
            if fields.mGroup == 0:
                args.append(None)
            else:
                args.append(fields.mGroup)
            if fields.mMaterial == 0:
                args.append(None)
            else:
                args.append(fields.mMaterial)
            args.append(fields.mNLayers)
            args.append(fields.mResult)
            args.append(fields.mSectionResult)
            args.append(fields.mStressReference)
            args.append(fields.mThickness)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_FEMShellGroup_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a FEMShellGroup object in the database.
    #
    # @param fields     FEMShellGroup object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setFEMShellGroup(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if allFields or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if allFields or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if allFields or ('NLayers' in fieldNames):
            cmds.append("var_NLayers=%s")
            args.append(fields.mNLayers)
        if allFields or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if allFields or ('SectionResult' in fieldNames):
            cmds.append("var_SectionResult=%s")
            args.append(fields.mSectionResult)
        if allFields or ('StressReference' in fieldNames):
            cmds.append("var_StressReference=%s")
            args.append(fields.mStressReference)
        if allFields or ('Thickness' in fieldNames):
            cmds.append("var_Thickness=%0.16g")
            args.append(fields.mThickness)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_FEMShellGroup SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the FEMShellGroup object IDs from the database.
    def getFEMShellGroupIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellGroup'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the FEMShellGroup object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  FEMShellGroup object fields
    # @return \c True on success, \c False if object not found
    def getFEMShellGroup(self, fields):
        selectCmd = 'SELECT '\
            'var_Displacement,ref_Group,ref_Material,var_NLayers,var_Result,var_SectionResult,var_StressReference,var_Thickness '\
            'FROM CLASS_FEMShellGroup WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mDisplacement = data.pop(0)
        fields.mGroup = zeroNone(data.pop(0))
        fields.mMaterial = zeroNone(data.pop(0))
        fields.mNLayers = data.pop(0)
        fields.mResult = data.pop(0)
        fields.mSectionResult = data.pop(0)
        fields.mStressReference = data.pop(0)
        fields.mThickness = data.pop(0)
        return True

    ## Gets a list of IDs of all FEMShellGroup objects matching the given
    # reference data.
    #
    # @param fields FEMShellGroup object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching FEMShellGroup objects
    def findEqualFEMShellGroup(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Displacement' in fieldNames):
            cmds.append("var_Displacement=%s")
            args.append(fields.mDisplacement)
        if (len(fieldNames)) == 0 or ('Group' in fieldNames):
            cmds.append("ref_Group=%s")
            args.append(fields.mGroup)
        if (len(fieldNames)) == 0 or ('Material' in fieldNames):
            cmds.append("ref_Material=%s")
            args.append(fields.mMaterial)
        if (len(fieldNames)) == 0 or ('NLayers' in fieldNames):
            cmds.append("var_NLayers=%s")
            args.append(fields.mNLayers)
        if (len(fieldNames)) == 0 or ('Result' in fieldNames):
            cmds.append("var_Result=%s")
            args.append(fields.mResult)
        if (len(fieldNames)) == 0 or ('SectionResult' in fieldNames):
            cmds.append("var_SectionResult=%s")
            args.append(fields.mSectionResult)
        if (len(fieldNames)) == 0 or ('StressReference' in fieldNames):
            cmds.append("var_StressReference=%s")
            args.append(fields.mStressReference)
        if (len(fieldNames)) == 0 or ('Thickness' in fieldNames):
            cmds.append("var_Thickness=%0.16g")
            args.append(fields.mThickness)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_FEMShellGroup WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a DaqUnit object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  DaqUnit object fields
    def addDaqUnit(self, fields):
        insertCmd = 'INSERT INTO CLASS_DaqUnit '\
            '(var_Model,var_Identifier) VALUES '\
            "(%s,%s)"
        args = []
        args.append(fields.mModel)
        args.append(fields.mIdentifier)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_DaqUnit_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of DaqUnit objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of DaqUnit object fields
    def addDaqUnitList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_DaqUnit '\
            '(var_Model,var_Identifier) VALUES '\
            "(%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mModel)
            args.append(fields.mIdentifier)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_DaqUnit_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a DaqUnit object in the database.
    #
    # @param fields     DaqUnit object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setDaqUnit(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Model' in fieldNames):
            cmds.append("var_Model=%s")
            args.append(fields.mModel)
        if allFields or ('Identifier' in fieldNames):
            cmds.append("var_Identifier=%s")
            args.append(fields.mIdentifier)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_DaqUnit SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the DaqUnit object IDs from the database.
    def getDaqUnitIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_DaqUnit'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the DaqUnit object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  DaqUnit object fields
    # @return \c True on success, \c False if object not found
    def getDaqUnit(self, fields):
        selectCmd = 'SELECT '\
            'var_Model,var_Identifier '\
            'FROM CLASS_DaqUnit WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mModel = data.pop(0)
        fields.mIdentifier = data.pop(0)
        return True

    ## Gets a list of IDs of all DaqUnit objects matching the given
    # reference data.
    #
    # @param fields DaqUnit object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching DaqUnit objects
    def findEqualDaqUnit(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Model' in fieldNames):
            cmds.append("var_Model=%s")
            args.append(fields.mModel)
        if (len(fieldNames)) == 0 or ('Identifier' in fieldNames):
            cmds.append("var_Identifier=%s")
            args.append(fields.mIdentifier)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_DaqUnit WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a DaqUnitChannel object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  DaqUnitChannel object fields
    def addDaqUnitChannel(self, fields):
        insertCmd = 'INSERT INTO CLASS_DaqUnitChannel '\
            '(ref_Unit,var_Number) VALUES '\
            "(%s,%s)"
        args = []
        if fields.mUnit == 0:
            args.append(None)
        else:
            args.append(fields.mUnit)
        args.append(fields.mNumber)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_DaqUnitChannel_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of DaqUnitChannel objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of DaqUnitChannel object fields
    def addDaqUnitChannelList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_DaqUnitChannel '\
            '(ref_Unit,var_Number) VALUES '\
            "(%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mUnit == 0:
                args.append(None)
            else:
                args.append(fields.mUnit)
            args.append(fields.mNumber)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_DaqUnitChannel_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a DaqUnitChannel object in the database.
    #
    # @param fields     DaqUnitChannel object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setDaqUnitChannel(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Unit' in fieldNames):
            cmds.append("ref_Unit=%s")
            args.append(fields.mUnit)
        if allFields or ('Number' in fieldNames):
            cmds.append("var_Number=%s")
            args.append(fields.mNumber)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_DaqUnitChannel SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the DaqUnitChannel object IDs from the database.
    def getDaqUnitChannelIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_DaqUnitChannel'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the DaqUnitChannel object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  DaqUnitChannel object fields
    # @return \c True on success, \c False if object not found
    def getDaqUnitChannel(self, fields):
        selectCmd = 'SELECT '\
            'ref_Unit,var_Number '\
            'FROM CLASS_DaqUnitChannel WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mUnit = zeroNone(data.pop(0))
        fields.mNumber = data.pop(0)
        return True

    ## Gets a list of IDs of all DaqUnitChannel objects matching the given
    # reference data.
    #
    # @param fields DaqUnitChannel object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching DaqUnitChannel objects
    def findEqualDaqUnitChannel(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Unit' in fieldNames):
            cmds.append("ref_Unit=%s")
            args.append(fields.mUnit)
        if (len(fieldNames)) == 0 or ('Number' in fieldNames):
            cmds.append("var_Number=%s")
            args.append(fields.mNumber)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_DaqUnitChannel WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a Sensor object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  Sensor object fields
    def addSensor(self, fields):
        insertCmd = 'INSERT INTO CLASS_Sensor '\
            '(ref_Type,var_Model,var_Identifier) VALUES '\
            "(%s,%s,%s)"
        args = []
        args.append(self.SensorTypeLabelToId[str(fields.mType)])
        args.append(fields.mModel)
        args.append(fields.mIdentifier)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_Sensor_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of Sensor objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of Sensor object fields
    def addSensorList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_Sensor '\
            '(ref_Type,var_Model,var_Identifier) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(self.SensorTypeLabelToId[str(fields.mType)])
            args.append(fields.mModel)
            args.append(fields.mIdentifier)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_Sensor_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a Sensor object in the database.
    #
    # @param fields     Sensor object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setSensor(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Type' in fieldNames):
            cmds.append("ref_Type=%s")
            args.append(self.SensorTypeLabelToId[str(fields.mType)])
        if allFields or ('Model' in fieldNames):
            cmds.append("var_Model=%s")
            args.append(fields.mModel)
        if allFields or ('Identifier' in fieldNames):
            cmds.append("var_Identifier=%s")
            args.append(fields.mIdentifier)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_Sensor SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the Sensor object IDs from the database.
    def getSensorIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_Sensor'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the Sensor object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  Sensor object fields
    # @return \c True on success, \c False if object not found
    def getSensor(self, fields):
        selectCmd = 'SELECT '\
            'ref_Type,var_Model,var_Identifier '\
            'FROM CLASS_Sensor WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mType = self.SensorTypeIdToEnum[data.pop(0)]
        fields.mModel = data.pop(0)
        fields.mIdentifier = data.pop(0)
        return True

    ## Gets a list of IDs of all Sensor objects matching the given
    # reference data.
    #
    # @param fields Sensor object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching Sensor objects
    def findEqualSensor(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Type' in fieldNames):
            cmds.append("ref_Type=%s")
            args.append(self.SensorTypeLabelToId[str(fields.mType)])
        if (len(fieldNames)) == 0 or ('Model' in fieldNames):
            cmds.append("var_Model=%s")
            args.append(fields.mModel)
        if (len(fieldNames)) == 0 or ('Identifier' in fieldNames):
            cmds.append("var_Identifier=%s")
            args.append(fields.mIdentifier)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_Sensor WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a Transducer object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  Transducer object fields
    def addTransducer(self, fields):
        insertCmd = 'INSERT INTO CLASS_Transducer '\
            '(var_Model,var_Identifier) VALUES '\
            "(%s,%s)"
        args = []
        args.append(fields.mModel)
        args.append(fields.mIdentifier)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_Transducer_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of Transducer objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of Transducer object fields
    def addTransducerList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_Transducer '\
            '(var_Model,var_Identifier) VALUES '\
            "(%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mModel)
            args.append(fields.mIdentifier)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_Transducer_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a Transducer object in the database.
    #
    # @param fields     Transducer object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setTransducer(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Model' in fieldNames):
            cmds.append("var_Model=%s")
            args.append(fields.mModel)
        if allFields or ('Identifier' in fieldNames):
            cmds.append("var_Identifier=%s")
            args.append(fields.mIdentifier)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_Transducer SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the Transducer object IDs from the database.
    def getTransducerIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_Transducer'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the Transducer object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  Transducer object fields
    # @return \c True on success, \c False if object not found
    def getTransducer(self, fields):
        selectCmd = 'SELECT '\
            'var_Model,var_Identifier '\
            'FROM CLASS_Transducer WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mModel = data.pop(0)
        fields.mIdentifier = data.pop(0)
        return True

    ## Gets a list of IDs of all Transducer objects matching the given
    # reference data.
    #
    # @param fields Transducer object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching Transducer objects
    def findEqualTransducer(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Model' in fieldNames):
            cmds.append("var_Model=%s")
            args.append(fields.mModel)
        if (len(fieldNames)) == 0 or ('Identifier' in fieldNames):
            cmds.append("var_Identifier=%s")
            args.append(fields.mIdentifier)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_Transducer WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a Experiment object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  Experiment object fields
    def addExperiment(self, fields):
        insertCmd = 'INSERT INTO CLASS_Experiment '\
            '(var_Frequency,var_Duration,var_BufferData) VALUES '\
            "(%s,%s,%s)"
        args = []
        args.append(fields.mFrequency)
        args.append(fields.mDuration)
        args.append(fields.mBufferData)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_Experiment_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of Experiment objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of Experiment object fields
    def addExperimentList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_Experiment '\
            '(var_Frequency,var_Duration,var_BufferData) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mFrequency)
            args.append(fields.mDuration)
            args.append(fields.mBufferData)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_Experiment_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a Experiment object in the database.
    #
    # @param fields     Experiment object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setExperiment(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Frequency' in fieldNames):
            cmds.append("var_Frequency=%s")
            args.append(fields.mFrequency)
        if allFields or ('Duration' in fieldNames):
            cmds.append("var_Duration=%s")
            args.append(fields.mDuration)
        if allFields or ('BufferData' in fieldNames):
            cmds.append("var_BufferData=%s")
            args.append(fields.mBufferData)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_Experiment SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the Experiment object IDs from the database.
    def getExperimentIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_Experiment'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the Experiment object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  Experiment object fields
    # @return \c True on success, \c False if object not found
    def getExperiment(self, fields):
        selectCmd = 'SELECT '\
            'var_Frequency,var_Duration,var_BufferData '\
            'FROM CLASS_Experiment WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mFrequency = data.pop(0)
        fields.mDuration = data.pop(0)
        fields.mBufferData = data.pop(0)
        return True

    ## Gets a list of IDs of all Experiment objects matching the given
    # reference data.
    #
    # @param fields Experiment object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching Experiment objects
    def findEqualExperiment(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Frequency' in fieldNames):
            cmds.append("var_Frequency=%s")
            args.append(fields.mFrequency)
        if (len(fieldNames)) == 0 or ('Duration' in fieldNames):
            cmds.append("var_Duration=%s")
            args.append(fields.mDuration)
        if (len(fieldNames)) == 0 or ('BufferData' in fieldNames):
            cmds.append("var_BufferData=%s")
            args.append(fields.mBufferData)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_Experiment WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a Cycle object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  Cycle object fields
    def addCycle(self, fields):
        insertCmd = 'INSERT INTO CLASS_Cycle '\
            '(var_Time,ref_Experiment) VALUES '\
            "(%s,%s)"
        args = []
        args.append(fields.mTime)
        if fields.mExperiment == 0:
            args.append(None)
        else:
            args.append(fields.mExperiment)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_Cycle_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of Cycle objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of Cycle object fields
    def addCycleList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_Cycle '\
            '(var_Time,ref_Experiment) VALUES '\
            "(%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            args.append(fields.mTime)
            if fields.mExperiment == 0:
                args.append(None)
            else:
                args.append(fields.mExperiment)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_Cycle_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a Cycle object in the database.
    #
    # @param fields     Cycle object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setCycle(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Time' in fieldNames):
            cmds.append("var_Time=%s")
            args.append(fields.mTime)
        if allFields or ('Experiment' in fieldNames):
            cmds.append("ref_Experiment=%s")
            args.append(fields.mExperiment)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_Cycle SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the Cycle object IDs from the database.
    def getCycleIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_Cycle'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the Cycle object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  Cycle object fields
    # @return \c True on success, \c False if object not found
    def getCycle(self, fields):
        selectCmd = 'SELECT '\
            'var_Time,ref_Experiment '\
            'FROM CLASS_Cycle WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mTime = data.pop(0)
        fields.mExperiment = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all Cycle objects matching the given
    # reference data.
    #
    # @param fields Cycle object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching Cycle objects
    def findEqualCycle(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Time' in fieldNames):
            cmds.append("var_Time=%s")
            args.append(fields.mTime)
        if (len(fieldNames)) == 0 or ('Experiment' in fieldNames):
            cmds.append("ref_Experiment=%s")
            args.append(fields.mExperiment)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_Cycle WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a SensorTimeData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  SensorTimeData object fields
    def addSensorTimeData(self, fields):
        insertCmd = 'INSERT INTO CLASS_SensorTimeData '\
            '(ref_Structure,ref_Sensor,var_Channel,var_StartTime,var_Status,ref_Node) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        args = []
        if fields.mStructure == 0:
            args.append(None)
        else:
            args.append(fields.mStructure)
        if fields.mSensor == 0:
            args.append(None)
        else:
            args.append(fields.mSensor)
        args.append(fields.mChannel)
        args.append(fields.mStartTime)
        args.append(fields.mStatus)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_SensorTimeData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of SensorTimeData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of SensorTimeData object fields
    def addSensorTimeDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_SensorTimeData '\
            '(ref_Structure,ref_Sensor,var_Channel,var_StartTime,var_Status,ref_Node) VALUES '\
            "(%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mStructure == 0:
                args.append(None)
            else:
                args.append(fields.mStructure)
            if fields.mSensor == 0:
                args.append(None)
            else:
                args.append(fields.mSensor)
            args.append(fields.mChannel)
            args.append(fields.mStartTime)
            args.append(fields.mStatus)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_SensorTimeData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a SensorTimeData object in the database.
    #
    # @param fields     SensorTimeData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setSensorTimeData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if allFields or ('Sensor' in fieldNames):
            cmds.append("ref_Sensor=%s")
            args.append(fields.mSensor)
        if allFields or ('Channel' in fieldNames):
            cmds.append("var_Channel=%s")
            args.append(fields.mChannel)
        if allFields or ('StartTime' in fieldNames):
            cmds.append("var_StartTime=%s")
            args.append(fields.mStartTime)
        if allFields or ('Status' in fieldNames):
            cmds.append("var_Status=%s")
            args.append(fields.mStatus)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_SensorTimeData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the SensorTimeData object IDs from the database.
    def getSensorTimeDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_SensorTimeData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the SensorTimeData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  SensorTimeData object fields
    # @return \c True on success, \c False if object not found
    def getSensorTimeData(self, fields):
        selectCmd = 'SELECT '\
            'ref_Structure,ref_Sensor,var_Channel,var_StartTime,var_Status,ref_Node '\
            'FROM CLASS_SensorTimeData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mStructure = zeroNone(data.pop(0))
        fields.mSensor = zeroNone(data.pop(0))
        fields.mChannel = data.pop(0)
        fields.mStartTime = data.pop(0)
        fields.mStatus = data.pop(0)
        fields.mNode = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all SensorTimeData objects matching the given
    # reference data.
    #
    # @param fields SensorTimeData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching SensorTimeData objects
    def findEqualSensorTimeData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if (len(fieldNames)) == 0 or ('Sensor' in fieldNames):
            cmds.append("ref_Sensor=%s")
            args.append(fields.mSensor)
        if (len(fieldNames)) == 0 or ('Channel' in fieldNames):
            cmds.append("var_Channel=%s")
            args.append(fields.mChannel)
        if (len(fieldNames)) == 0 or ('StartTime' in fieldNames):
            cmds.append("var_StartTime=%s")
            args.append(fields.mStartTime)
        if (len(fieldNames)) == 0 or ('Status' in fieldNames):
            cmds.append("var_Status=%s")
            args.append(fields.mStatus)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_SensorTimeData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a TransducerTimeData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  TransducerTimeData object fields
    def addTransducerTimeData(self, fields):
        insertCmd = 'INSERT INTO CLASS_TransducerTimeData '\
            '(ref_Transducer,ref_Sensor,ref_Node,var_Orientation,var_Amplification,var_Channel,var_Filter,var_StartTime,var_Status) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        args = []
        if fields.mTransducer == 0:
            args.append(None)
        else:
            args.append(fields.mTransducer)
        if fields.mSensor == 0:
            args.append(None)
        else:
            args.append(fields.mSensor)
        if fields.mNode == 0:
            args.append(None)
        else:
            args.append(fields.mNode)
        args.append(fields.mOrientation)
        args.append(fields.mAmplification)
        args.append(fields.mChannel)
        args.append(fields.mFilter)
        args.append(fields.mStartTime)
        args.append(fields.mStatus)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_TransducerTimeData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of TransducerTimeData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of TransducerTimeData object fields
    def addTransducerTimeDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_TransducerTimeData '\
            '(ref_Transducer,ref_Sensor,ref_Node,var_Orientation,var_Amplification,var_Channel,var_Filter,var_StartTime,var_Status) VALUES '\
            "(%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mTransducer == 0:
                args.append(None)
            else:
                args.append(fields.mTransducer)
            if fields.mSensor == 0:
                args.append(None)
            else:
                args.append(fields.mSensor)
            if fields.mNode == 0:
                args.append(None)
            else:
                args.append(fields.mNode)
            args.append(fields.mOrientation)
            args.append(fields.mAmplification)
            args.append(fields.mChannel)
            args.append(fields.mFilter)
            args.append(fields.mStartTime)
            args.append(fields.mStatus)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_TransducerTimeData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a TransducerTimeData object in the database.
    #
    # @param fields     TransducerTimeData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setTransducerTimeData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Transducer' in fieldNames):
            cmds.append("ref_Transducer=%s")
            args.append(fields.mTransducer)
        if allFields or ('Sensor' in fieldNames):
            cmds.append("ref_Sensor=%s")
            args.append(fields.mSensor)
        if allFields or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if allFields or ('Orientation' in fieldNames):
            cmds.append("var_Orientation=%s")
            args.append(fields.mOrientation)
        if allFields or ('Amplification' in fieldNames):
            cmds.append("var_Amplification=%s")
            args.append(fields.mAmplification)
        if allFields or ('Channel' in fieldNames):
            cmds.append("var_Channel=%s")
            args.append(fields.mChannel)
        if allFields or ('Filter' in fieldNames):
            cmds.append("var_Filter=%s")
            args.append(fields.mFilter)
        if allFields or ('StartTime' in fieldNames):
            cmds.append("var_StartTime=%s")
            args.append(fields.mStartTime)
        if allFields or ('Status' in fieldNames):
            cmds.append("var_Status=%s")
            args.append(fields.mStatus)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_TransducerTimeData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the TransducerTimeData object IDs from the database.
    def getTransducerTimeDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_TransducerTimeData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the TransducerTimeData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  TransducerTimeData object fields
    # @return \c True on success, \c False if object not found
    def getTransducerTimeData(self, fields):
        selectCmd = 'SELECT '\
            'ref_Transducer,ref_Sensor,ref_Node,var_Orientation,var_Amplification,var_Channel,var_Filter,var_StartTime,var_Status '\
            'FROM CLASS_TransducerTimeData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mTransducer = zeroNone(data.pop(0))
        fields.mSensor = zeroNone(data.pop(0))
        fields.mNode = zeroNone(data.pop(0))
        fields.mOrientation = data.pop(0)
        fields.mAmplification = data.pop(0)
        fields.mChannel = data.pop(0)
        fields.mFilter = data.pop(0)
        fields.mStartTime = data.pop(0)
        fields.mStatus = data.pop(0)
        return True

    ## Gets a list of IDs of all TransducerTimeData objects matching the given
    # reference data.
    #
    # @param fields TransducerTimeData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching TransducerTimeData objects
    def findEqualTransducerTimeData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Transducer' in fieldNames):
            cmds.append("ref_Transducer=%s")
            args.append(fields.mTransducer)
        if (len(fieldNames)) == 0 or ('Sensor' in fieldNames):
            cmds.append("ref_Sensor=%s")
            args.append(fields.mSensor)
        if (len(fieldNames)) == 0 or ('Node' in fieldNames):
            cmds.append("ref_Node=%s")
            args.append(fields.mNode)
        if (len(fieldNames)) == 0 or ('Orientation' in fieldNames):
            cmds.append("var_Orientation=%s")
            args.append(fields.mOrientation)
        if (len(fieldNames)) == 0 or ('Amplification' in fieldNames):
            cmds.append("var_Amplification=%s")
            args.append(fields.mAmplification)
        if (len(fieldNames)) == 0 or ('Channel' in fieldNames):
            cmds.append("var_Channel=%s")
            args.append(fields.mChannel)
        if (len(fieldNames)) == 0 or ('Filter' in fieldNames):
            cmds.append("var_Filter=%s")
            args.append(fields.mFilter)
        if (len(fieldNames)) == 0 or ('StartTime' in fieldNames):
            cmds.append("var_StartTime=%s")
            args.append(fields.mStartTime)
        if (len(fieldNames)) == 0 or ('Status' in fieldNames):
            cmds.append("var_Status=%s")
            args.append(fields.mStatus)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_TransducerTimeData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a ExperimentStructureData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  ExperimentStructureData object fields
    def addExperimentStructureData(self, fields):
        insertCmd = 'INSERT INTO CLASS_ExperimentStructureData '\
            '(ref_Experiment,ref_Structure,var_StartTime,var_Status) VALUES '\
            "(%s,%s,%s,%s)"
        args = []
        if fields.mExperiment == 0:
            args.append(None)
        else:
            args.append(fields.mExperiment)
        if fields.mStructure == 0:
            args.append(None)
        else:
            args.append(fields.mStructure)
        args.append(fields.mStartTime)
        args.append(fields.mStatus)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_ExperimentStructureData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of ExperimentStructureData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of ExperimentStructureData object fields
    def addExperimentStructureDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_ExperimentStructureData '\
            '(ref_Experiment,ref_Structure,var_StartTime,var_Status) VALUES '\
            "(%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mExperiment == 0:
                args.append(None)
            else:
                args.append(fields.mExperiment)
            if fields.mStructure == 0:
                args.append(None)
            else:
                args.append(fields.mStructure)
            args.append(fields.mStartTime)
            args.append(fields.mStatus)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_ExperimentStructureData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a ExperimentStructureData object in the database.
    #
    # @param fields     ExperimentStructureData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setExperimentStructureData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Experiment' in fieldNames):
            cmds.append("ref_Experiment=%s")
            args.append(fields.mExperiment)
        if allFields or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if allFields or ('StartTime' in fieldNames):
            cmds.append("var_StartTime=%s")
            args.append(fields.mStartTime)
        if allFields or ('Status' in fieldNames):
            cmds.append("var_Status=%s")
            args.append(fields.mStatus)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_ExperimentStructureData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the ExperimentStructureData object IDs from the database.
    def getExperimentStructureDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_ExperimentStructureData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the ExperimentStructureData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  ExperimentStructureData object fields
    # @return \c True on success, \c False if object not found
    def getExperimentStructureData(self, fields):
        selectCmd = 'SELECT '\
            'ref_Experiment,ref_Structure,var_StartTime,var_Status '\
            'FROM CLASS_ExperimentStructureData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mExperiment = zeroNone(data.pop(0))
        fields.mStructure = zeroNone(data.pop(0))
        fields.mStartTime = data.pop(0)
        fields.mStatus = data.pop(0)
        return True

    ## Gets a list of IDs of all ExperimentStructureData objects matching the given
    # reference data.
    #
    # @param fields ExperimentStructureData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching ExperimentStructureData objects
    def findEqualExperimentStructureData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Experiment' in fieldNames):
            cmds.append("ref_Experiment=%s")
            args.append(fields.mExperiment)
        if (len(fieldNames)) == 0 or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if (len(fieldNames)) == 0 or ('StartTime' in fieldNames):
            cmds.append("var_StartTime=%s")
            args.append(fields.mStartTime)
        if (len(fieldNames)) == 0 or ('Status' in fieldNames):
            cmds.append("var_Status=%s")
            args.append(fields.mStatus)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_ExperimentStructureData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a TransducerCycleData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  TransducerCycleData object fields
    def addTransducerCycleData(self, fields):
        insertCmd = 'INSERT INTO CLASS_TransducerCycleData '\
            '(ref_Transducer,ref_Cycle,var_SoftwareScaling) VALUES '\
            "(%s,%s,%0.8g)"
        args = []
        if fields.mTransducer == 0:
            args.append(None)
        else:
            args.append(fields.mTransducer)
        if fields.mCycle == 0:
            args.append(None)
        else:
            args.append(fields.mCycle)
        args.append(fields.mSoftwareScaling)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_TransducerCycleData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of TransducerCycleData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of TransducerCycleData object fields
    def addTransducerCycleDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_TransducerCycleData '\
            '(ref_Transducer,ref_Cycle,var_SoftwareScaling) VALUES '\
            "(%s,%s,%0.8g)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mTransducer == 0:
                args.append(None)
            else:
                args.append(fields.mTransducer)
            if fields.mCycle == 0:
                args.append(None)
            else:
                args.append(fields.mCycle)
            args.append(fields.mSoftwareScaling)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_TransducerCycleData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a TransducerCycleData object in the database.
    #
    # @param fields     TransducerCycleData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setTransducerCycleData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Transducer' in fieldNames):
            cmds.append("ref_Transducer=%s")
            args.append(fields.mTransducer)
        if allFields or ('Cycle' in fieldNames):
            cmds.append("ref_Cycle=%s")
            args.append(fields.mCycle)
        if allFields or ('SoftwareScaling' in fieldNames):
            cmds.append("var_SoftwareScaling=%0.8g")
            args.append(fields.mSoftwareScaling)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_TransducerCycleData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the TransducerCycleData object IDs from the database.
    def getTransducerCycleDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_TransducerCycleData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the TransducerCycleData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  TransducerCycleData object fields
    # @return \c True on success, \c False if object not found
    def getTransducerCycleData(self, fields):
        selectCmd = 'SELECT '\
            'ref_Transducer,ref_Cycle,var_SoftwareScaling '\
            'FROM CLASS_TransducerCycleData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mTransducer = zeroNone(data.pop(0))
        fields.mCycle = zeroNone(data.pop(0))
        fields.mSoftwareScaling = data.pop(0)
        return True

    ## Gets a list of IDs of all TransducerCycleData objects matching the given
    # reference data.
    #
    # @param fields TransducerCycleData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching TransducerCycleData objects
    def findEqualTransducerCycleData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Transducer' in fieldNames):
            cmds.append("ref_Transducer=%s")
            args.append(fields.mTransducer)
        if (len(fieldNames)) == 0 or ('Cycle' in fieldNames):
            cmds.append("ref_Cycle=%s")
            args.append(fields.mCycle)
        if (len(fieldNames)) == 0 or ('SoftwareScaling' in fieldNames):
            cmds.append("var_SoftwareScaling=%0.8g")
            args.append(fields.mSoftwareScaling)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_TransducerCycleData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a MappingMatrix object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  MappingMatrix object fields
    def addMappingMatrix(self, fields):
        insertCmd = 'INSERT INTO CLASS_MappingMatrix '\
            '(ref_Structure,var_Name,var_Description,ref_OutputQuantity,ref_InputQuantity) VALUES '\
            "(%s,%s,%s,%s,%s)"
        args = []
        if fields.mStructure == 0:
            args.append(None)
        else:
            args.append(fields.mStructure)
        args.append(fields.mName)
        args.append(fields.mDescription)
        args.append(self.QuantityLabelToId[str(fields.mOutputQuantity)])
        args.append(self.QuantityLabelToId[str(fields.mInputQuantity)])
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_MappingMatrix_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of MappingMatrix objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of MappingMatrix object fields
    def addMappingMatrixList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_MappingMatrix '\
            '(ref_Structure,var_Name,var_Description,ref_OutputQuantity,ref_InputQuantity) VALUES '\
            "(%s,%s,%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mStructure == 0:
                args.append(None)
            else:
                args.append(fields.mStructure)
            args.append(fields.mName)
            args.append(fields.mDescription)
            args.append(self.QuantityLabelToId[str(fields.mOutputQuantity)])
            args.append(self.QuantityLabelToId[str(fields.mInputQuantity)])
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_MappingMatrix_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a MappingMatrix object in the database.
    #
    # @param fields     MappingMatrix object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setMappingMatrix(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if allFields or ('Name' in fieldNames):
            cmds.append("var_Name=%s")
            args.append(fields.mName)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if allFields or ('OutputQuantity' in fieldNames):
            cmds.append("ref_OutputQuantity=%s")
            args.append(self.QuantityLabelToId[str(fields.mOutputQuantity)])
        if allFields or ('InputQuantity' in fieldNames):
            cmds.append("ref_InputQuantity=%s")
            args.append(self.QuantityLabelToId[str(fields.mInputQuantity)])
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_MappingMatrix SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the MappingMatrix object IDs from the database.
    def getMappingMatrixIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_MappingMatrix'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the MappingMatrix object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  MappingMatrix object fields
    # @return \c True on success, \c False if object not found
    def getMappingMatrix(self, fields):
        selectCmd = 'SELECT '\
            'ref_Structure,var_Name,var_Description,ref_OutputQuantity,ref_InputQuantity '\
            'FROM CLASS_MappingMatrix WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mStructure = zeroNone(data.pop(0))
        fields.mName = data.pop(0)
        fields.mDescription = data.pop(0)
        fields.mOutputQuantity = self.QuantityIdToEnum[data.pop(0)]
        fields.mInputQuantity = self.QuantityIdToEnum[data.pop(0)]
        return True

    ## Gets a list of IDs of all MappingMatrix objects matching the given
    # reference data.
    #
    # @param fields MappingMatrix object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching MappingMatrix objects
    def findEqualMappingMatrix(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Structure' in fieldNames):
            cmds.append("ref_Structure=%s")
            args.append(fields.mStructure)
        if (len(fieldNames)) == 0 or ('Name' in fieldNames):
            cmds.append("var_Name=%s")
            args.append(fields.mName)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if (len(fieldNames)) == 0 or ('OutputQuantity' in fieldNames):
            cmds.append("ref_OutputQuantity=%s")
            args.append(self.QuantityLabelToId[str(fields.mOutputQuantity)])
        if (len(fieldNames)) == 0 or ('InputQuantity' in fieldNames):
            cmds.append("ref_InputQuantity=%s")
            args.append(self.QuantityLabelToId[str(fields.mInputQuantity)])
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_MappingMatrix WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a DaqUnitChannelData object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  DaqUnitChannelData object fields
    def addDaqUnitChannelData(self, fields):
        insertCmd = 'INSERT INTO CLASS_DaqUnitChannelData '\
            '(ref_Channel) VALUES '\
            "(%s)"
        args = []
        if fields.mChannel == 0:
            args.append(None)
        else:
            args.append(fields.mChannel)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_DaqUnitChannelData_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of DaqUnitChannelData objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of DaqUnitChannelData object fields
    def addDaqUnitChannelDataList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_DaqUnitChannelData '\
            '(ref_Channel) VALUES '\
            "(%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mChannel == 0:
                args.append(None)
            else:
                args.append(fields.mChannel)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_DaqUnitChannelData_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a DaqUnitChannelData object in the database.
    #
    # @param fields     DaqUnitChannelData object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setDaqUnitChannelData(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Channel' in fieldNames):
            cmds.append("ref_Channel=%s")
            args.append(fields.mChannel)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_DaqUnitChannelData SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the DaqUnitChannelData object IDs from the database.
    def getDaqUnitChannelDataIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_DaqUnitChannelData'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the DaqUnitChannelData object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  DaqUnitChannelData object fields
    # @return \c True on success, \c False if object not found
    def getDaqUnitChannelData(self, fields):
        selectCmd = 'SELECT '\
            'ref_Channel '\
            'FROM CLASS_DaqUnitChannelData WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mChannel = zeroNone(data.pop(0))
        return True

    ## Gets a list of IDs of all DaqUnitChannelData objects matching the given
    # reference data.
    #
    # @param fields DaqUnitChannelData object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching DaqUnitChannelData objects
    def findEqualDaqUnitChannelData(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Channel' in fieldNames):
            cmds.append("ref_Channel=%s")
            args.append(fields.mChannel)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_DaqUnitChannelData WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Adds a AccelMeas object to the database.
    # The id field of the object will be updated with the object ID
    # assigned by the database.
    #
    # @param  fields  AccelMeas object fields
    def addAccelMeas(self, fields):
        insertCmd = 'INSERT INTO CLASS_AccelMeas '\
            '(ref_Sensor,var_Name,var_Description) VALUES '\
            "(%s,%s,%s)"
        args = []
        if fields.mSensor == 0:
            args.append(None)
        else:
            args.append(fields.mSensor)
        args.append(fields.mName)
        args.append(fields.mDescription)
        try:
            self.cur.execute(insertCmd, args)
            self.cur.execute("SELECT currval('CLASS_AccelMeas_objectid_seq')")
            fields.id = self.cur.fetchone()[0]
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Adds a set of AccelMeas objects to the database.
    # The id field of the objects will be updated with the object ID
    # assigned by the database.
    #
    # @param  fieldsList  list of AccelMeas object fields
    def addAccelMeasList(self, fieldsList):
        insertCmd = 'INSERT INTO CLASS_AccelMeas '\
            '(ref_Sensor,var_Name,var_Description) VALUES '\
            "(%s,%s,%s)"
        argsList = []
        for fields in fieldsList:
            args = []
            if fields.mSensor == 0:
                args.append(None)
            else:
                args.append(fields.mSensor)
            args.append(fields.mName)
            args.append(fields.mDescription)
            argsList.append(args)
        try:
            self.cur.executemany(insertCmd, argsList)
            self.cur.execute("SELECT currval('CLASS_AccelMeas_objectid_seq')")
            oid = self.cur.fetchone()[0] - len(fieldsList)
            self.db.commit()
            # Update object ID fields
            for fields in fieldsList:
                oid += 1
                fields.id = oid
        except:
            self.db.rollback()
            raise

    ## Updates a AccelMeas object in the database.
    #
    # @param fields     AccelMeas object fields
    # @param fieldNames list of names of fields to update (all if empty)
    def setAccelMeas(self, fields, fieldNames):
        # Build prepared statement
        cmds = []
        args = []
        allFields = (len(fieldNames) == 0)
        if allFields or ('Sensor' in fieldNames):
            cmds.append("ref_Sensor=%s")
            args.append(fields.mSensor)
        if allFields or ('Name' in fieldNames):
            cmds.append("var_Name=%s")
            args.append(fields.mName)
        if allFields or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if len(cmds) == 0:
            # Nothing to do
            return
        updateCmd = 'UPDATE CLASS_AccelMeas SET ' + ",".join(cmds) + \
            ' WHERE ObjectID=%d'
        args.append(fields.id)
        # Execute statement
        try:
            self.cur.execute(updateCmd, args)
            self.db.commit()
        except:
            self.db.rollback()
            raise

    ## Get the AccelMeas object IDs from the database.
    def getAccelMeasIds(self):
        selectCmd = 'SELECT ObjectID FROM CLASS_AccelMeas'
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    ## Get the AccelMeas object fields from the database.
    # The fields of the object will be updated with the data retrieved
    # from the database.
    #
    # @param  fields  AccelMeas object fields
    # @return \c True on success, \c False if object not found
    def getAccelMeas(self, fields):
        selectCmd = 'SELECT '\
            'ref_Sensor,var_Name,var_Description '\
            'FROM CLASS_AccelMeas WHERE ObjectID=%d ' % fields.id
        # TODO: error checking
        self.cur.execute(selectCmd)
        data = self.cur.fetchone()
        if not data:
            return False
        fields.mSensor = zeroNone(data.pop(0))
        fields.mName = data.pop(0)
        fields.mDescription = data.pop(0)
        return True

    ## Gets a list of IDs of all AccelMeas objects matching the given
    # reference data.
    #
    # @param fields AccelMeas object fields to compare to
    # @param fieldNames list of names of fields to compare (empty is all)
    # @return list of IDs of matching AccelMeas objects
    def findEqualAccelMeas(self, fields, fieldNames):
        cmds = []
        args = []
        if (len(fieldNames)) == 0 or ('Sensor' in fieldNames):
            cmds.append("ref_Sensor=%s")
            args.append(fields.mSensor)
        if (len(fieldNames)) == 0 or ('Name' in fieldNames):
            cmds.append("var_Name=%s")
            args.append(fields.mName)
        if (len(fieldNames)) == 0 or ('Description' in fieldNames):
            cmds.append("var_Description=%s")
            args.append(fields.mDescription)
        if len(cmds) == 0:
            return []
        selectCmd = 'SELECT ObjectID FROM CLASS_AccelMeas WHERE ' + \
            ' AND '.join(cmds)
        # TODO: error checking
        self.cur.execute(selectCmd, args)
        data = self.cur.fetchall()
        return [x[0] for x in data]

    def addMultimedia(self, file_name, file_type, desc_short, desc_long):
      insertCmd = "INSERT INTO EXT_Multimedia (file_name, file_type, desc_short, desc_long) VALUES (%s, %s, %s, %s)"
      try:
        self.cur.execute(insertCmd, [file_name, file_type, desc_short, desc_long])
        self.db.commit()
      except:
        self.db.rollback()
        raise

    ## Closes the database.
    def close(self):
        self.db.close()
