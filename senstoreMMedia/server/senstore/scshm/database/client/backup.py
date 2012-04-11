# Copyright (c) 2010 SC Solutions, Inc.
# See LICENCE.TXT for licensing terms.
# $Id: backup-tpl.py,v 1.11 2010/09/03 22:29:10 glinden Exp $
import sys
import traceback
import logging
import ConfigParser
from tables import *
import numpy
from scshm.database.client.SenStoreClient import SenStoreClient, SenStore
from scshm.database import slicing

## \package scshm.database.client.backup  Database backup implementation.
#
# This package defines the PyTable table definitions needed for storing
# all the SenStore database content in a stand-alone HDF5 file.  The
# access to the HDF5 file is handled by the BackupFile class.
# The functions backup() and restore() handle all the details of backing
# up and restoring data. 

## Number of array elements in a transmitted data chunk
CHUNK_ELEM_COUNT = 50000

## PyTables column definition for enumerations.
class EnumCols(IsDescription):
    ## Enumeration label.
    label = StringCol(256)


## PyTables column definition for the Structure table.
class StructureCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## Short name. 
    Name = StringCol(256)
    ## One-line description. 
    Description = StringCol(256)
    ## Type of structure, e.g. suspension bridge. 
    Type = StringCol(256)
    ## Unit of distance 
    # (Reference to Unit enumeration).
    DistanceUnit = UInt16Col()
    ## Unit of force 
    # (Reference to Unit enumeration).
    ForceUnit = UInt16Col()
    ## Unit of weight 
    # (Reference to Unit enumeration).
    WeightUnit = UInt16Col()


## PyTables column definition for the FEMDof table.
class FEMDofCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## Structure 
    # (Reference to Structure object).
    Structure = Int64Col()
    ## Local DOF number. 
    LocalId = Int32Col()
    ## Finite element node. 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## Direction of motion. 
    # (Reference to Quantity enumeration).
    Direction = UInt16Col()


## PyTables column definition for the FEMNodalMass table.
class FEMNodalMassCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    Mass1 = Float64Col()
    ## 
    Mass2 = Float64Col()
    ## 
    Mass3 = Float64Col()
    ## 
    Mass4 = Float64Col()
    ## 
    Mass5 = Float64Col()
    ## 
    Mass6 = Float64Col()
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()


## PyTables column definition for the FEMNLElasticStrainStress table.
class FEMNLElasticStrainStressCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    RecordNumber = Int32Col()
    ## 
    Strain = Float64Col()
    ## 
    Stress = Float64Col()


## PyTables column definition for the FEMBoundary table.
class FEMBoundaryCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## 
    # (Reference to BoundaryType enumeration).
    Ovalization = UInt16Col()
    ## 
    # (Reference to BoundaryType enumeration).
    Phi = UInt16Col()
    ## 
    # (Reference to BoundaryType enumeration).
    Rx = UInt16Col()
    ## 
    # (Reference to BoundaryType enumeration).
    Ry = UInt16Col()
    ## 
    # (Reference to BoundaryType enumeration).
    Rz = UInt16Col()
    ## 
    # (Reference to BoundaryType enumeration).
    Ux = UInt16Col()
    ## 
    # (Reference to BoundaryType enumeration).
    Uy = UInt16Col()
    ## 
    # (Reference to BoundaryType enumeration).
    Uz = UInt16Col()
    ## 
    Warping = StringCol(256)


## PyTables column definition for the FEMSectionPipe table.
class FEMSectionPipeCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Diameter = Float64Col()
    ## 
    SC = Float64Col()
    ## 
    SectionID = Int32Col()
    ## 
    SSarea = Float64Col()
    ## 
    TC = Float64Col()
    ## 
    Thickness = Float64Col()
    ## 
    Torfac = Float64Col()
    ## 
    TSarea = Float64Col()


## PyTables column definition for the FEMCoordSystem table.
class FEMCoordSystemCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    AX = Float64Col()
    ## 
    AY = Float64Col()
    ## 
    AZ = Float64Col()
    ## 
    BBY = Float64Col()
    ## 
    BX = Float64Col()
    ## 
    BZ = Float64Col()
    ## 
    Description = StringCol(256)
    ## 
    Mode = Int16Col()
    ## 
    P1 = Int32Col()
    ## 
    P2 = Int32Col()
    ## 
    P3 = Int32Col()
    ## 
    Type = StringCol(256)
    ## 
    Xorigin = Float64Col()
    ## 
    Yorigin = Float64Col()
    ## 
    Zorigin = Float64Col()
    ## 
    LocalID = Int32Col()


## PyTables column definition for the FEMNode table.
class FEMNodeCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    # (Reference to FEMCoordSystem object).
    System = Int64Col()
    ## 
    X = Float64Col()
    ## 
    Y = Float64Col()
    ## 
    Z = Float64Col()
    ## 
    LocalID = Int32Col()


## PyTables column definition for the FEMTruss table.
class FEMTrussCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    Epsin = Float64Col()
    ## 
    Gapwidth = Float64Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    # (Reference to FEMMaterial object).
    Material = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N2 = Int64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)
    ## 
    SectionArea = Float64Col()
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()


## PyTables column definition for the FEMTimeFunctionData table.
class FEMTimeFunctionDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    DataTime = Float64Col()
    ## 
    GMRecordID = Int16Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    TimeFunctionID = Int32Col()
    ## 
    TimeValue = Float64Col()


## PyTables column definition for the FEMPlasticMlMaterials table.
class FEMPlasticMlMaterialsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ALPHA = Float64Col()
    ## 
    Density = Float64Col()
    ## 
    E = Float64Col()
    ## 
    Hardening = StringCol(256)
    ## 
    MaterialID = Int32Col()
    ## 
    NU = Float64Col()
    ## 
    TREF = Float64Col()


## PyTables column definition for the FEMPlateGroup table.
class FEMPlateGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Displacement = StringCol(256)
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    IniStrain = StringCol(256)
    ## 
    MaterialID = Int32Col()
    ## 
    Result = StringCol(256)


## PyTables column definition for the FEMBeam table.
class FEMBeamCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to FEMNode object).
    AuxNode = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    EndRelease = Int32Col()
    ## 
    Epsin = Float64Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    IRigidEnd = Float64Col()
    ## 
    JRigidEnd = Float64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    # (Reference to FEMNode object).
    Node1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node2 = Int64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)
    ## 
    SectionID = Int32Col()
    ## 
    Subdivision = Int32Col()
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()


## PyTables column definition for the FEMCurvMomentData table.
class FEMCurvMomentDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Curvature = Float64Col()
    ## 
    CurvMomentID = Int32Col()
    ## 
    Moment = Float64Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMPropertysets table.
class FEMPropertysetsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    C = Float64Col()
    ## 
    Description = StringCol(256)
    ## 
    K = Float64Col()
    ## 
    M = Float64Col()
    ## 
    NC = Int32Col()
    ## 
    NK = Int32Col()
    ## 
    NM = Int32Col()
    ## 
    Nonlinear = StringCol(256)
    ## 
    PropertysetID = Int32Col()
    ## 
    S = Float64Col()


## PyTables column definition for the FEMOrthotropicMaterial table.
class FEMOrthotropicMaterialCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Density = Float64Col()
    ## 
    EA = Float64Col()
    ## 
    EB = Float64Col()
    ## 
    EC = Float64Col()
    ## 
    GAB = Float64Col()
    ## 
    GAC = Float64Col()
    ## 
    GBC = Float64Col()
    ## 
    # (Reference to FEMMaterial object).
    Material = Int64Col()
    ## 
    NUAB = Float64Col()
    ## 
    NUAC = Float64Col()
    ## 
    NUBC = Float64Col()


## PyTables column definition for the FEMAppliedLoads table.
class FEMAppliedLoadsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    AppliedLoadNmb = Int32Col()
    ## 
    ArrivalTime = Float64Col()
    ## 
    LoadID = Int16Col()
    ## 
    LoadType = StringCol(256)
    ## 
    SiteType = StringCol(256)
    ## 
    TimeFunctionID = Int32Col()


## PyTables column definition for the FEMThermoOrthData table.
class FEMThermoOrthDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ALPHAA = Float64Col()
    ## 
    ALPHAB = Float64Col()
    ## 
    ALPHAC = Float64Col()
    ## 
    EA = Float64Col()
    ## 
    EB = Float64Col()
    ## 
    EC = Float64Col()
    ## 
    GAB = Float64Col()
    ## 
    GAC = Float64Col()
    ## 
    GBC = Float64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    NUAB = Float64Col()
    ## 
    NUAC = Float64Col()
    ## 
    NUBC = Float64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Theta = Float64Col()


## PyTables column definition for the FEMContactPairs table.
class FEMContactPairsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ContactorSurf = Int32Col()
    ## 
    ContGroupID = Int32Col()
    ## 
    ContPair = Int32Col()
    ## 
    FContactor = Float64Col()
    ## 
    Friction = Float64Col()
    ## 
    FTarget = Float64Col()
    ## 
    HeatTransf = Float64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    TargetSurf = Int32Col()
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()


## PyTables column definition for the FEMGeneral table.
class FEMGeneralCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    MatrixSet = Int32Col()
    ## 
    NodeAmount = Int16Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)


## PyTables column definition for the FEMBeamNode table.
class FEMBeamNodeCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Displacement = StringCol(256)
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    IniStrain = StringCol(256)
    ## 
    MaterialID = Int32Col()
    ## 
    MC = StringCol(256)
    ## 
    MCrigidity = Int32Col()
    ## 
    REmultiplyer = Float64Col()
    ## 
    Result = StringCol(256)
    ## 
    REtype = StringCol(256)
    ## 
    RINT = Int16Col()
    ## 
    SectionID = Int32Col()
    ## 
    SINT = Int16Col()
    ## 
    TINT = Int16Col()


## PyTables column definition for the FEMSectionRect table.
class FEMSectionRectCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Height = Float64Col()
    ## 
    IShear = StringCol(256)
    ## 
    SC = Float64Col()
    ## 
    SectionID = Int32Col()
    ## 
    SSarea = Float64Col()
    ## 
    TC = Float64Col()
    ## 
    Torfac = Float64Col()
    ## 
    TSarea = Float64Col()
    ## 
    Width = Float64Col()


## PyTables column definition for the FEMBeamLoad table.
class FEMBeamLoadCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ArrivalTime = Float64Col()
    ## 
    DeformDepend = Int16Col()
    ## 
    DirectFilter = Int16Col()
    ## 
    ElementID = Int32Col()
    ## 
    Face = Int16Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    P1 = Float64Col()
    ## 
    P2 = Float64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    TimeFunctionID = Int32Col()


## PyTables column definition for the FEMLoadMassProportional table.
class FEMLoadMassProportionalCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    AX = Float64Col()
    ## 
    AY = Float64Col()
    ## 
    AZ = Float64Col()
    ## 
    LoadID = Int16Col()
    ## 
    Magnitude = Float64Col()
    ## 
    # (Reference to Structure object).
    Structure = Int64Col()


## PyTables column definition for the FEMLink table.
class FEMLinkCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    Displacement = StringCol(256)
    ## 
    # (Reference to FEMNode object).
    MasterNode = Int64Col()
    ## 
    RLID = Int32Col()
    ## 
    # (Reference to FEMNode object).
    SlaveNode = Int64Col()


## PyTables column definition for the FEMAxesNode table.
class FEMAxesNodeCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    AxNodeID = Int32Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node2 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node3 = Int64Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMNMTimeMass table.
class FEMNMTimeMassCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Mass = Float64Col()
    ## 
    PropertyID = Int32Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    TimeValue = Float64Col()


## PyTables column definition for the FEMAppliedDisplacement table.
class FEMAppliedDisplacementCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ArrivalTime = Float64Col()
    ## 
    Description = StringCol(256)
    ## 
    Direction = Int16Col()
    ## 
    Factor = Float64Col()
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    TimeFunctionID = Int32Col()


## PyTables column definition for the FEMTimeFunctions table.
class FEMTimeFunctionsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    TimeFunctionID = Int32Col()


## PyTables column definition for the FEMForceStrainData table.
class FEMForceStrainDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Force = Float64Col()
    ## 
    ForceAxID = Int32Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Strain = Float64Col()


## PyTables column definition for the FEMSkewDOF table.
class FEMSkewDOFCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## 
    SkewSystemID = Int32Col()


## PyTables column definition for the FEMSectionI table.
class FEMSectionICols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Height = Float64Col()
    ## 
    SC = Float64Col()
    ## 
    SectionID = Int32Col()
    ## 
    SSarea = Float64Col()
    ## 
    TC = Float64Col()
    ## 
    Thick1 = Float64Col()
    ## 
    Thick2 = Float64Col()
    ## 
    Thick3 = Float64Col()
    ## 
    Torfac = Float64Col()
    ## 
    TSarea = Float64Col()
    ## 
    Width1 = Float64Col()
    ## 
    Width2 = Float64Col()


## PyTables column definition for the FEMPlasticBilinearMaterial table.
class FEMPlasticBilinearMaterialCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Alpha = Float64Col()
    ## 
    Density = Float64Col()
    ## 
    E = Float64Col()
    ## 
    EPA = Float64Col()
    ## 
    ET = Float64Col()
    ## 
    Hardening = StringCol(256)
    ## 
    # (Reference to FEMMaterial object).
    Material = Int64Col()
    ## 
    NU = Float64Col()
    ## 
    TRef = Float64Col()
    ## 
    Yield = Float64Col()


## PyTables column definition for the FEMMTForceData table.
class FEMMTForceDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Force = Float64Col()
    ## 
    MomentRID = Int32Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    TwistMomentID = Int32Col()


## PyTables column definition for the FEMShellPressure table.
class FEMShellPressureCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ArrivalTime = Float64Col()
    ## 
    DeformDepend = Int16Col()
    ## 
    Description = StringCol(256)
    ## 
    DirectFilter = Int16Col()
    ## 
    ElementID = Int32Col()
    ## 
    Face = Int16Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    Nodaux = Int32Col()
    ## 
    P1 = Float64Col()
    ## 
    P2 = Float64Col()
    ## 
    P3 = Float64Col()
    ## 
    P4 = Float64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    TimeFunctionID = Int32Col()


## PyTables column definition for the FEMMatrices table.
class FEMMatricesCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    Lump = Int32Col()
    ## 
    MatrixID = Int32Col()
    ## 
    MatrixType = StringCol(256)
    ## 
    ND = Int32Col()
    ## 
    NS = Int32Col()


## PyTables column definition for the FEMDamping table.
class FEMDampingCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ALPHA = Float64Col()
    ## 
    BETA = Float64Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()


## PyTables column definition for the FEMMaterial table.
class FEMMaterialCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    # (Reference to MaterialType enumeration).
    MaterialType = UInt16Col()
    ## 
    LocalID = Int32Col()
    ## 
    # (Reference to Structure object).
    Structure = Int64Col()


## PyTables column definition for the FEMMatrixData table.
class FEMMatrixDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Coeff = Float64Col()
    ## 
    ColumnIndex = Int32Col()
    ## 
    MatrixID = Int32Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    RowIndex = Int32Col()


## PyTables column definition for the FEMShellAxesOrtho table.
class FEMShellAxesOrthoCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ALFA = Float64Col()
    ## 
    AxOrthoID = Int32Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    LineID = Int32Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMEndreleases table.
class FEMEndreleasesCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    EndRelease = Int32Col()
    ## 
    Moment1 = Int16Col()
    ## 
    Moment2 = Int16Col()
    ## 
    Moment3 = Int16Col()
    ## 
    Moment4 = Int16Col()
    ## 
    Moment5 = Int16Col()
    ## 
    Moment6 = Int16Col()


## PyTables column definition for the FEMTrussGroup table.
class FEMTrussGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Displacement = StringCol(256)
    ## 
    GAPS = StringCol(256)
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    IniStrain = StringCol(256)
    ## 
    # (Reference to FEMMaterial object).
    Material = Int64Col()
    ## 
    SectionArea = Float64Col()


## PyTables column definition for the FEMInitialTemperature table.
class FEMInitialTemperatureCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## 
    Temperature = Float64Col()


## PyTables column definition for the FEMThermoIsoMaterials table.
class FEMThermoIsoMaterialsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Density = Float64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    TREF = Float64Col()


## PyTables column definition for the FEMThermoIsoData table.
class FEMThermoIsoDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ALPHA = Float64Col()
    ## 
    E = Float64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    NU = Float64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Theta = Float64Col()


## PyTables column definition for the FEMContactGroup3 table.
class FEMContactGroup3Cols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ContGroupID = Int32Col()
    ## 
    Depth = Float64Col()
    ## 
    Description = StringCol(256)
    ## 
    Forces = StringCol(256)
    ## 
    Friction = Float64Col()
    ## 
    IniPenetration = StringCol(256)
    ## 
    NodeToNode = StringCol(256)
    ## 
    Offset = Float64Col()
    ## 
    PenetrAlgorithm = StringCol(256)
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()
    ## 
    Tied = StringCol(256)
    ## 
    TiedOffset = Float64Col()
    ## 
    Tolerance = Float64Col()
    ## 
    Tractions = StringCol(256)


## PyTables column definition for the FEMNLElasticMaterials table.
class FEMNLElasticMaterialsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Dcurve = Int32Col()
    ## 
    Density = Float64Col()
    ## 
    MaterialID = Int32Col()


## PyTables column definition for the FEMPlate table.
class FEMPlateCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    Flex11 = Float64Col()
    ## 
    Flex12 = Float64Col()
    ## 
    Flex22 = Float64Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    Meps11 = Float64Col()
    ## 
    Meps12 = Float64Col()
    ## 
    Meps22 = Float64Col()
    ## 
    # (Reference to FEMNode object).
    N1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N2 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N3 = Int64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()
    ## 
    Thick = Float64Col()


## PyTables column definition for the FEMIsoBeam table.
class FEMIsoBeamCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to FEMNode object).
    AUX = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    Epaxl = Float64Col()
    ## 
    Ephoop = Float64Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    # (Reference to FEMNode object).
    N1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N2 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N3 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N4 = Int64Col()
    ## 
    NodeAmount = Int16Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)
    ## 
    SectionID = Int32Col()
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()


## PyTables column definition for the FEMAppliedConcentratedLoad table.
class FEMAppliedConcentratedLoadCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ArrivalTime = Float64Col()
    ## 
    Description = StringCol(256)
    ## 
    Direction = Int16Col()
    ## 
    Factor = Float64Col()
    ## 
    # (Reference to FEMNode object).
    NodeAux = Int64Col()
    ## 
    # (Reference to FEMNode object).
    NodeID = Int64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    TimeFunctionID = Int32Col()


## PyTables column definition for the FEMTwoDSolidGroup table.
class FEMTwoDSolidGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to FEMNode object).
    AuxNode = Int64Col()
    ## 
    Displacement = StringCol(256)
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    Result = StringCol(256)
    ## 
    Subtype = StringCol(256)


## PyTables column definition for the FEMGroup table.
class FEMGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    # (Reference to GroupType enumeration).
    GroupType = UInt16Col()
    ## 
    LocalID = Int32Col()
    ## 
    # (Reference to Structure object).
    Structure = Int64Col()


## PyTables column definition for the FEMProperties table.
class FEMPropertiesCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    PropertyID = Int32Col()
    ## 
    PropertyType = StringCol(256)
    ## 
    Rupture = StringCol(256)
    ## 
    XC = Float64Col()
    ## 
    XN = Float64Col()


## PyTables column definition for the FEMThreeDSolidGroup table.
class FEMThreeDSolidGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Displacement = StringCol(256)
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    Result = StringCol(256)


## PyTables column definition for the FEMThreeDSolid table.
class FEMThreeDSolidCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    Maxes = Int32Col()
    ## 
    # (Reference to FEMNode object).
    N1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N10 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N11 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N12 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N13 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N14 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N15 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N16 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N17 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N18 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N19 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N2 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N20 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N21 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N22 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N23 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N24 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N25 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N26 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N27 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N3 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N4 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N5 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N6 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N7 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N8 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N9 = Int64Col()
    ## 
    NodeAmount = Int16Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()


## PyTables column definition for the FEMSectionProp table.
class FEMSectionPropCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Area = Float64Col()
    ## 
    Rinertia = Float64Col()
    ## 
    Sarea = Float64Col()
    ## 
    SectionID = Int32Col()
    ## 
    Sinertia = Float64Col()
    ## 
    Tarea = Float64Col()
    ## 
    Tinertia = Float64Col()


## PyTables column definition for the FEMElasticMaterial table.
class FEMElasticMaterialCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Alpha = Float64Col()
    ## 
    Density = Float64Col()
    ## 
    E = Float64Col()
    ## 
    # (Reference to FEMMaterial object).
    Material = Int64Col()
    ## 
    NU = Float64Col()


## PyTables column definition for the FEMPoints table.
class FEMPointsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    PointID = Int32Col()
    ## 
    SystemID = Int32Col()
    ## 
    X = Float64Col()
    ## 
    Y = Float64Col()
    ## 
    Z = Float64Col()


## PyTables column definition for the FEMThermoOrthMaterials table.
class FEMThermoOrthMaterialsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Density = Float64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    TREF = Float64Col()


## PyTables column definition for the FEMConstraints table.
class FEMConstraintsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ConstraintID = Int32Col()
    ## 
    Description = StringCol(256)
    ## 
    SlaveDOF = StringCol(256)
    ## 
    SlaveNode = Int32Col()


## PyTables column definition for the FEMMCrigidities table.
class FEMMCrigiditiesCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    AcurveType = StringCol(256)
    ## 
    Alpha = Int32Col()
    ## 
    AxialCF = Float64Col()
    ## 
    BcurveType = StringCol(256)
    ## 
    BendingCF = Float64Col()
    ## 
    Beta = Int32Col()
    ## 
    Density = Float64Col()
    ## 
    ForceAxID = Int32Col()
    ## 
    Hardening = StringCol(256)
    ## 
    MassArea = Float64Col()
    ## 
    MassR = Float64Col()
    ## 
    MassS = Float64Col()
    ## 
    MassT = Float64Col()
    ## 
    MomentRID = Int32Col()
    ## 
    MomentSID = Int32Col()
    ## 
    MomentTID = Int32Col()
    ## 
    RigidityID = Int32Col()
    ## 
    TcurveType = StringCol(256)
    ## 
    TorsionCF = Float64Col()


## PyTables column definition for the FEMSkeySysNode table.
class FEMSkeySysNodeCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    # (Reference to FEMNode object).
    Node1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node2 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node3 = Int64Col()
    ## 
    SkewSystemID = Int32Col()


## PyTables column definition for the FEMIsoBeamGroup table.
class FEMIsoBeamGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Displacement = StringCol(256)
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    IniStrain = StringCol(256)
    ## 
    MaterialID = Int32Col()
    ## 
    Result = StringCol(256)
    ## 
    SectionID = Int32Col()


## PyTables column definition for the FEMShellDOF table.
class FEMShellDOFCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    DOFnumber = StringCol(256)
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## 
    VectorID = Int32Col()


## PyTables column definition for the FEMCrossSections table.
class FEMCrossSectionsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    SectionID = Int32Col()
    ## 
    SectionType = StringCol(256)


## PyTables column definition for the FEMTwistMomentData table.
class FEMTwistMomentDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Moment = Float64Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Twist = Float64Col()
    ## 
    TwistMomentID = Int32Col()


## PyTables column definition for the FEMShell table.
class FEMShellCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    # (Reference to FEMMaterial object).
    Material = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N2 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N3 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N4 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N5 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N6 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N7 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N8 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N9 = Int64Col()
    ## 
    NodeAmount = Int16Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)
    ## 
    T1 = Float64Col()
    ## 
    T2 = Float64Col()
    ## 
    T3 = Float64Col()
    ## 
    T4 = Float64Col()
    ## 
    T5 = Float64Col()
    ## 
    T6 = Float64Col()
    ## 
    T7 = Float64Col()
    ## 
    T8 = Float64Col()
    ## 
    T9 = Float64Col()
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()


## PyTables column definition for the FEMNTNContact table.
class FEMNTNContactCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ContactorNode = Int32Col()
    ## 
    ContGroupID = Int32Col()
    ## 
    ContPair = Int32Col()
    ## 
    PrintRes = StringCol(256)
    ## 
    RecordNmb = Int32Col()
    ## 
    SaveRes = StringCol(256)
    ## 
    TargetNode = Int32Col()
    ## 
    TargetNx = Float64Col()
    ## 
    TargetNy = Float64Col()
    ## 
    TargetNz = Float64Col()


## PyTables column definition for the FEMShellLayer table.
class FEMShellLayerCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    LayerNumber = Int32Col()
    ## 
    MaterialID = Int32Col()
    ## 
    PThick = Float64Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMSkewSysAngles table.
class FEMSkewSysAnglesCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    PHI = Float64Col()
    ## 
    SkewSystemID = Int32Col()
    ## 
    THETA = Float64Col()
    ## 
    XSI = Float64Col()


## PyTables column definition for the FEMGroundMotionRecord table.
class FEMGroundMotionRecordCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Description = StringCol(256)
    ## 
    GMRecordID = Int16Col()
    ## 
    GMRecordName = StringCol(256)


## PyTables column definition for the FEMGeneralGroup table.
class FEMGeneralGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    MatrixSet = Int32Col()
    ## 
    Result = StringCol(256)
    ## 
    SkewSystem = StringCol(256)


## PyTables column definition for the FEMTwoDSolid table.
class FEMTwoDSolidCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Bet = Float64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    # (Reference to FEMNode object).
    N1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N2 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N3 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N4 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N5 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N6 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N7 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N8 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N9 = Int64Col()
    ## 
    NodeAmount = Int16Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()
    ## 
    Thick = Float64Col()


## PyTables column definition for the FEMAppliedTemperature table.
class FEMAppliedTemperatureCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ArrivalTime = Float64Col()
    ## 
    Factor = Float64Col()
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## 
    RecordNmbr = Int32Col()
    ## 
    TimeFunctionID = Int32Col()


## PyTables column definition for the FEMMatrixSets table.
class FEMMatrixSetsCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Damping = Int32Col()
    ## 
    Description = StringCol(256)
    ## 
    Mass = Int32Col()
    ## 
    MatrixSetID = Int32Col()
    ## 
    Stiffness = Int32Col()
    ## 
    Stress = Int32Col()


## PyTables column definition for the FEMConstraintCoef table.
class FEMConstraintCoefCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Coefficient = Float64Col()
    ## 
    ConstraintID = Int32Col()
    ## 
    Description = StringCol(256)
    ## 
    MasterDOF = StringCol(256)
    ## 
    MasterNode = Int32Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMSectionBox table.
class FEMSectionBoxCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Height = Float64Col()
    ## 
    SC = Float64Col()
    ## 
    SectionID = Int32Col()
    ## 
    SSarea = Float64Col()
    ## 
    TC = Float64Col()
    ## 
    Thick1 = Float64Col()
    ## 
    Thick2 = Float64Col()
    ## 
    Torfac = Float64Col()
    ## 
    TSarea = Float64Col()
    ## 
    Width = Float64Col()


## PyTables column definition for the FEMNKDisplForce table.
class FEMNKDisplForceCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Displacement = Float64Col()
    ## 
    Force = Float64Col()
    ## 
    PropertyID = Int32Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMPlasticStrainStress table.
class FEMPlasticStrainStressCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    MaterialID = Int32Col()
    ## 
    RecordNumber = Int32Col()
    ## 
    Strain = Float64Col()
    ## 
    Stress = Float64Col()


## PyTables column definition for the FEMShellAxesOrthoData table.
class FEMShellAxesOrthoDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    AxOrthoID = Int32Col()
    ## 
    ElementID = Int32Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMGeneralNode table.
class FEMGeneralNodeCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ElementID = Int32Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    LocalNmb = Int16Col()
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMStrLines table.
class FEMStrLinesCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    LineID = Int32Col()
    ## 
    P1 = Int32Col()
    ## 
    P2 = Int32Col()


## PyTables column definition for the FEMContactSurface table.
class FEMContactSurfaceCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    ContGroupID = Int32Col()
    ## 
    ContSegment = Int32Col()
    ## 
    ContSurface = Int32Col()
    ## 
    # (Reference to FEMNode object).
    N1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N2 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N3 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N4 = Int64Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMMCForceData table.
class FEMMCForceDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    CurvMomentID = Int32Col()
    ## 
    Force = Float64Col()
    ## 
    MomentSTID = Int32Col()
    ## 
    RecordNmb = Int32Col()


## PyTables column definition for the FEMSpring table.
class FEMSpringCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    AX = Float64Col()
    ## 
    AY = Float64Col()
    ## 
    AZ = Float64Col()
    ## 
    Description = StringCol(256)
    ## 
    ElementID = Int32Col()
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    ID1 = Int16Col()
    ## 
    ID2 = Int16Col()
    ## 
    # (Reference to FEMNode object).
    N1 = Int64Col()
    ## 
    # (Reference to FEMNode object).
    N2 = Int64Col()
    ## 
    PropertySet = Int32Col()
    ## 
    RecordNmb = Int32Col()
    ## 
    Save = StringCol(256)
    ## 
    TBirth = Float64Col()
    ## 
    TDeath = Float64Col()


## PyTables column definition for the FEMSpringGroup table.
class FEMSpringGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Bolt = StringCol(256)
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    Nonlinear = StringCol(256)
    ## 
    PropertySet = Int32Col()
    ## 
    Result = StringCol(256)
    ## 
    SkewSystem = StringCol(256)


## PyTables column definition for the FEMShellGroup table.
class FEMShellGroupCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Displacement = StringCol(256)
    ## 
    # (Reference to FEMGroup object).
    Group = Int64Col()
    ## 
    # (Reference to FEMMaterial object).
    Material = Int64Col()
    ## 
    NLayers = Int32Col()
    ## 
    Result = StringCol(256)
    ## 
    SectionResult = Int16Col()
    ## 
    StressReference = StringCol(256)
    ## 
    Thickness = Float64Col()


## PyTables column definition for the DaqUnit table.
class DaqUnitCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Model = StringCol(256)
    ## 
    Identifier = StringCol(256)


## PyTables column definition for the DaqUnitChannel table.
class DaqUnitChannelCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## Corresponding data acquisition unit. 
    # (Reference to DaqUnit object).
    Unit = Int64Col()
    ## Local channel number. 
    Number = Int16Col()


## PyTables column definition for the Sensor table.
class SensorCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to SensorType enumeration).
    Type = UInt16Col()
    ## 
    Model = StringCol(256)
    ## 
    Identifier = StringCol(256)


## PyTables column definition for the Transducer table.
class TransducerCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Model = StringCol(256)
    ## 
    Identifier = StringCol(256)


## PyTables column definition for the Experiment table.
class ExperimentCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Frequency = Int16Col()
    ## 
    Duration = Int32Col()
    ## 
    BufferData = Int32Col()


## PyTables column definition for the Cycle table.
class CycleCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    Time = Time64Col()
    ## 
    # (Reference to Experiment object).
    Experiment = Int64Col()


## PyTables column definition for the SensorTimeData table.
class SensorTimeDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to Structure object).
    Structure = Int64Col()
    ## 
    # (Reference to Sensor object).
    Sensor = Int64Col()
    ## 
    Channel = Int16Col()
    ## 
    StartTime = Time64Col()
    ## 
    Status = StringCol(256)
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()


## PyTables column definition for the TransducerTimeData table.
class TransducerTimeDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to Transducer object).
    Transducer = Int64Col()
    ## 
    # (Reference to Sensor object).
    Sensor = Int64Col()
    ## 
    # (Reference to FEMNode object).
    Node = Int64Col()
    ## 
    Orientation = Int16Col()
    ## 
    Amplification = Int32Col()
    ## 
    Channel = Int16Col()
    ## 
    Filter = StringCol(256)
    ## 
    StartTime = Time64Col()
    ## 
    Status = StringCol(256)


## PyTables column definition for the ExperimentStructureData table.
class ExperimentStructureDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to Experiment object).
    Experiment = Int64Col()
    ## 
    # (Reference to Structure object).
    Structure = Int64Col()
    ## 
    StartTime = Time64Col()
    ## 
    Status = StringCol(256)


## PyTables column definition for the TransducerCycleData table.
class TransducerCycleDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## 
    # (Reference to Transducer object).
    Transducer = Int64Col()
    ## 
    # (Reference to Cycle object).
    Cycle = Int64Col()
    ## 
    SoftwareScaling = Float32Col()


## PyTables column definition for the MappingMatrix table.
class MappingMatrixCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## Parent structure 
    # (Reference to Structure object).
    Structure = Int64Col()
    ## Short name 
    Name = StringCol(256)
    ## Description 
    Description = StringCol(256)
    ## Output quantity 
    # (Reference to Quantity enumeration).
    OutputQuantity = UInt16Col()
    ## Input quantity 
    # (Reference to Quantity enumeration).
    InputQuantity = UInt16Col()


## PyTables column definition for the DaqUnitChannelData table.
class DaqUnitChannelDataCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## Parent sensor 
    # (Reference to DaqUnitChannel object).
    Channel = Int64Col()


## PyTables column definition for the AccelMeas table.
class AccelMeasCols(IsDescription):
    ## Object ID.
    id = Int64Col()
    ## Parent sensor 
    # (Reference to Sensor object).
    Sensor = Int64Col()
    ## Short name 
    Name = StringCol(256)
    ## Description 
    Description = StringCol(256)


## Checks if an HDF5 group has a child with the given name.
#
# @param group HDF5 group object
# @param name child name
def nodeExists(group, name):
    return group._v_children.has_key(name)


## Gets the largest ID in use at the given table
#
# @param table HDF5 table with an 'id' field
def getMaxId(table):
    maxId = 0
    for row in table.iterrows():
        if maxId < row['id']:
            maxId = row['id']
    return maxId


## HDF5 database backup file implementation
#
# Provides read/write access to an HDF5 database backup file.
#
# When creating a backup file, the version of this software must
# match the server version.  When reading a backup file, the
# versions may be different.
#
# The HDF5 file has the following layout:
# - <tt>/enums/<<i>enumname</i>>/</tt> contains enumeration definitions
# - <tt>/objects/<<i>classname</i>>/</tt> contains class objects
# - <tt>/arrays/<<i>classname</i>>/id<<i>id</i>>/data</tt> contains array data
# - <tt>/arrays/<<i>classname</i>>/id<<i>id</i>>/<<i>axis</i>>Axis</tt>
#   contains array axis definition
# - <tt>/signals/<<i>classname</i>>/id<<i>id</i>>/data</tt> contains signal data
# - <tt>/signals/<<i>classname</i>>/id<<i>id</i>>/<<i>axis</i>>Axis</tt>
#   contains signal axis definition
class BackupFile:
    ## Opens or creates a backup file.
    #
    # If the file mode is 'w', a new backup file will be created
    # and initialized.  If the mode is 'r' or 'a', the backup file
    # is assumed to exist and be initialized.  The append mode should
    # be used with caution, as it is often undesirable to change backup
    # files.
    #
    # @param filename HDF5 file name
    # @param mode     file open mode ('r' to read (default), 'w' to write, 'a' to append)
    # @param title    HDF5 file title (only used on creation)
    def __init__(self, filename, mode = 'r', title = ''):
        self.h5 = openFile(filename, mode = mode, title = title)
        #
        # Create missing tables
        #
        if mode == 'w':
            enums   = self.h5.createGroup('/', 'enums',   'Enumerations')
            objs    = self.h5.createGroup('/', 'objects', 'Objects')
            arrays  = self.h5.createGroup('/', 'arrays',  'Array data')
            signals = self.h5.createGroup('/', 'signals', 'Signal data')
        else:
            enums   = self.h5.root.enums
            objs    = self.h5.root.objects
            arrays  = self.h5.root.arrays
            signals = self.h5.root.signals
        self.MaterialTypeEnumList = []
        if mode == 'w':
            # Create MaterialType enumeration table
            self.tblMaterialType = self.h5.createTable(enums,
                'MaterialType', EnumCols,
                'Supported material types. ')
            logging.info('Creating HDF5 table for MaterialType enumerations')
            row = self.tblMaterialType.row
            row['label'] = 'ELASTIC'
            row.append()
            self.MaterialTypeEnumList.append('MaterialTypeELASTIC')
            row = self.tblMaterialType.row
            row['label'] = 'ORTHOTROPIC'
            row.append()
            self.MaterialTypeEnumList.append('MaterialTypeORTHOTROPIC')
            row = self.tblMaterialType.row
            row['label'] = 'NONLINEARELASTIC'
            row.append()
            self.MaterialTypeEnumList.append('MaterialTypeNONLINEARELASTIC')
            row = self.tblMaterialType.row
            row['label'] = 'PLASTICBILINEAR'
            row.append()
            self.MaterialTypeEnumList.append('MaterialTypePLASTICBILINEAR')
            row = self.tblMaterialType.row
            row['label'] = 'PLASTICMULTILINEAR'
            row.append()
            self.MaterialTypeEnumList.append('MaterialTypePLASTICMULTILINEAR')
            row = self.tblMaterialType.row
            row['label'] = 'THERMOISOTROPIC'
            row.append()
            self.MaterialTypeEnumList.append('MaterialTypeTHERMOISOTROPIC')
            row = self.tblMaterialType.row
            row['label'] = 'THERMOORTHOTROPIC'
            row.append()
            self.MaterialTypeEnumList.append('MaterialTypeTHERMOORTHOTROPIC')
        else:
            self.tblMaterialType = enums.MaterialType
            # Retrieve enumeration labels
            self._MaterialTypeEnumList = []
            for row in self.tblMaterialType.iterrows():
                self.MaterialTypeEnumList.append('MaterialType'+row['label'])
        self.UnitEnumList = []
        if mode == 'w':
            # Create Unit enumeration table
            self.tblUnit = self.h5.createTable(enums,
                'Unit', EnumCols,
                'Supported units. ')
            logging.info('Creating HDF5 table for Unit enumerations')
            row = self.tblUnit.row
            row['label'] = 'METER'
            row.append()
            self.UnitEnumList.append('UnitMETER')
            row = self.tblUnit.row
            row['label'] = 'KILOGRAM'
            row.append()
            self.UnitEnumList.append('UnitKILOGRAM')
            row = self.tblUnit.row
            row['label'] = 'NEWTON'
            row.append()
            self.UnitEnumList.append('UnitNEWTON')
            row = self.tblUnit.row
            row['label'] = 'FOOT'
            row.append()
            self.UnitEnumList.append('UnitFOOT')
            row = self.tblUnit.row
            row['label'] = 'POUND'
            row.append()
            self.UnitEnumList.append('UnitPOUND')
            row = self.tblUnit.row
            row['label'] = 'KILOPOUND'
            row.append()
            self.UnitEnumList.append('UnitKILOPOUND')
            row = self.tblUnit.row
            row['label'] = 'SLUG'
            row.append()
            self.UnitEnumList.append('UnitSLUG')
            row = self.tblUnit.row
            row['label'] = 'KILOSLUG'
            row.append()
            self.UnitEnumList.append('UnitKILOSLUG')
        else:
            self.tblUnit = enums.Unit
            # Retrieve enumeration labels
            self._UnitEnumList = []
            for row in self.tblUnit.iterrows():
                self.UnitEnumList.append('Unit'+row['label'])
        self.QuantityEnumList = []
        if mode == 'w':
            # Create Quantity enumeration table
            self.tblQuantity = self.h5.createTable(enums,
                'Quantity', EnumCols,
                'Available quantities (degrees of freedom). ')
            logging.info('Creating HDF5 table for Quantity enumerations')
            row = self.tblQuantity.row
            row['label'] = 'XDISPLACEMENT'
            row.append()
            self.QuantityEnumList.append('QuantityXDISPLACEMENT')
            row = self.tblQuantity.row
            row['label'] = 'YDISPLACEMENT'
            row.append()
            self.QuantityEnumList.append('QuantityYDISPLACEMENT')
            row = self.tblQuantity.row
            row['label'] = 'ZDISPLACEMENT'
            row.append()
            self.QuantityEnumList.append('QuantityZDISPLACEMENT')
            row = self.tblQuantity.row
            row['label'] = 'XROTATION'
            row.append()
            self.QuantityEnumList.append('QuantityXROTATION')
            row = self.tblQuantity.row
            row['label'] = 'YROTATION'
            row.append()
            self.QuantityEnumList.append('QuantityYROTATION')
            row = self.tblQuantity.row
            row['label'] = 'ZROTATION'
            row.append()
            self.QuantityEnumList.append('QuantityZROTATION')
            row = self.tblQuantity.row
            row['label'] = 'XFORCE'
            row.append()
            self.QuantityEnumList.append('QuantityXFORCE')
            row = self.tblQuantity.row
            row['label'] = 'YFORCE'
            row.append()
            self.QuantityEnumList.append('QuantityYFORCE')
            row = self.tblQuantity.row
            row['label'] = 'ZFORCE'
            row.append()
            self.QuantityEnumList.append('QuantityZFORCE')
            row = self.tblQuantity.row
            row['label'] = 'RAW'
            row.append()
            self.QuantityEnumList.append('QuantityRAW')
        else:
            self.tblQuantity = enums.Quantity
            # Retrieve enumeration labels
            self._QuantityEnumList = []
            for row in self.tblQuantity.iterrows():
                self.QuantityEnumList.append('Quantity'+row['label'])
        self.CoordinateSystemTypeEnumList = []
        if mode == 'w':
            # Create CoordinateSystemType enumeration table
            self.tblCoordinateSystemType = self.h5.createTable(enums,
                'CoordinateSystemType', EnumCols,
                'Supported coordinate system types. ')
            logging.info('Creating HDF5 table for CoordinateSystemType enumerations')
            row = self.tblCoordinateSystemType.row
            row['label'] = 'CARTESIAN'
            row.append()
            self.CoordinateSystemTypeEnumList.append('CoordinateSystemTypeCARTESIAN')
            row = self.tblCoordinateSystemType.row
            row['label'] = 'SPHERICAL'
            row.append()
            self.CoordinateSystemTypeEnumList.append('CoordinateSystemTypeSPHERICAL')
            row = self.tblCoordinateSystemType.row
            row['label'] = 'CYLINDRICAL'
            row.append()
            self.CoordinateSystemTypeEnumList.append('CoordinateSystemTypeCYLINDRICAL')
        else:
            self.tblCoordinateSystemType = enums.CoordinateSystemType
            # Retrieve enumeration labels
            self._CoordinateSystemTypeEnumList = []
            for row in self.tblCoordinateSystemType.iterrows():
                self.CoordinateSystemTypeEnumList.append('CoordinateSystemType'+row['label'])
        self.BoundaryTypeEnumList = []
        if mode == 'w':
            # Create BoundaryType enumeration table
            self.tblBoundaryType = self.h5.createTable(enums,
                'BoundaryType', EnumCols,
                'Supported boundary types. ')
            logging.info('Creating HDF5 table for BoundaryType enumerations')
            row = self.tblBoundaryType.row
            row['label'] = 'FREE'
            row.append()
            self.BoundaryTypeEnumList.append('BoundaryTypeFREE')
            row = self.tblBoundaryType.row
            row['label'] = 'FIXED'
            row.append()
            self.BoundaryTypeEnumList.append('BoundaryTypeFIXED')
        else:
            self.tblBoundaryType = enums.BoundaryType
            # Retrieve enumeration labels
            self._BoundaryTypeEnumList = []
            for row in self.tblBoundaryType.iterrows():
                self.BoundaryTypeEnumList.append('BoundaryType'+row['label'])
        self.SectionTypeEnumList = []
        if mode == 'w':
            # Create SectionType enumeration table
            self.tblSectionType = self.h5.createTable(enums,
                'SectionType', EnumCols,
                'Supported cross sections. ')
            logging.info('Creating HDF5 table for SectionType enumerations')
            row = self.tblSectionType.row
            row['label'] = 'BOX'
            row.append()
            self.SectionTypeEnumList.append('SectionTypeBOX')
            row = self.tblSectionType.row
            row['label'] = 'PROPERTIES'
            row.append()
            self.SectionTypeEnumList.append('SectionTypePROPERTIES')
            row = self.tblSectionType.row
            row['label'] = 'RECTANGULAR'
            row.append()
            self.SectionTypeEnumList.append('SectionTypeRECTANGULAR')
            row = self.tblSectionType.row
            row['label'] = 'PIPE'
            row.append()
            self.SectionTypeEnumList.append('SectionTypePIPE')
            row = self.tblSectionType.row
            row['label'] = 'I'
            row.append()
            self.SectionTypeEnumList.append('SectionTypeI')
            row = self.tblSectionType.row
            row['label'] = 'L'
            row.append()
            self.SectionTypeEnumList.append('SectionTypeL')
            row = self.tblSectionType.row
            row['label'] = 'U'
            row.append()
            self.SectionTypeEnumList.append('SectionTypeU')
        else:
            self.tblSectionType = enums.SectionType
            # Retrieve enumeration labels
            self._SectionTypeEnumList = []
            for row in self.tblSectionType.iterrows():
                self.SectionTypeEnumList.append('SectionType'+row['label'])
        self.GroupTypeEnumList = []
        if mode == 'w':
            # Create GroupType enumeration table
            self.tblGroupType = self.h5.createTable(enums,
                'GroupType', EnumCols,
                'Supported group types. ')
            logging.info('Creating HDF5 table for GroupType enumerations')
            row = self.tblGroupType.row
            row['label'] = 'BEAM'
            row.append()
            self.GroupTypeEnumList.append('GroupTypeBEAM')
            row = self.tblGroupType.row
            row['label'] = 'TRUSS'
            row.append()
            self.GroupTypeEnumList.append('GroupTypeTRUSS')
            row = self.tblGroupType.row
            row['label'] = 'GENERAL'
            row.append()
            self.GroupTypeEnumList.append('GroupTypeGENERAL')
            row = self.tblGroupType.row
            row['label'] = 'ISOBEAM'
            row.append()
            self.GroupTypeEnumList.append('GroupTypeISOBEAM')
            row = self.tblGroupType.row
            row['label'] = 'PLATE'
            row.append()
            self.GroupTypeEnumList.append('GroupTypePLATE')
            row = self.tblGroupType.row
            row['label'] = 'SHELL'
            row.append()
            self.GroupTypeEnumList.append('GroupTypeSHELL')
            row = self.tblGroupType.row
            row['label'] = 'SPRING'
            row.append()
            self.GroupTypeEnumList.append('GroupTypeSPRING')
            row = self.tblGroupType.row
            row['label'] = 'THREEDSOLID'
            row.append()
            self.GroupTypeEnumList.append('GroupTypeTHREEDSOLID')
            row = self.tblGroupType.row
            row['label'] = 'TWODSOLID'
            row.append()
            self.GroupTypeEnumList.append('GroupTypeTWODSOLID')
        else:
            self.tblGroupType = enums.GroupType
            # Retrieve enumeration labels
            self._GroupTypeEnumList = []
            for row in self.tblGroupType.iterrows():
                self.GroupTypeEnumList.append('GroupType'+row['label'])
        self.SensorTypeEnumList = []
        if mode == 'w':
            # Create SensorType enumeration table
            self.tblSensorType = self.h5.createTable(enums,
                'SensorType', EnumCols,
                'Supported sensor types. ')
            logging.info('Creating HDF5 table for SensorType enumerations')
            row = self.tblSensorType.row
            row['label'] = 'ACCELEROMETER'
            row.append()
            self.SensorTypeEnumList.append('SensorTypeACCELEROMETER')
            row = self.tblSensorType.row
            row['label'] = 'ANEMOMETER'
            row.append()
            self.SensorTypeEnumList.append('SensorTypeANEMOMETER')
            row = self.tblSensorType.row
            row['label'] = 'WINDVANE'
            row.append()
            self.SensorTypeEnumList.append('SensorTypeWINDVANE')
            row = self.tblSensorType.row
            row['label'] = 'STRAINGAUGE'
            row.append()
            self.SensorTypeEnumList.append('SensorTypeSTRAINGAUGE')
        else:
            self.tblSensorType = enums.SensorType
            # Retrieve enumeration labels
            self._SensorTypeEnumList = []
            for row in self.tblSensorType.iterrows():
                self.SensorTypeEnumList.append('SensorType'+row['label'])
        if mode == 'w':
            # Create Structure object table
            self.tblStructure = self.h5.createTable(objs,
                'Structure', StructureCols,
                'Structure, such as a bridge. ')
            logging.info('Creating HDF5 table for Structure objects')
        else:
            self.tblStructure = objs.Structure
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMDof object table
            self.tblFEMDof = self.h5.createTable(objs,
                'FEMDof', FEMDofCols,
                'Finite element model degrees of freedom. ')
            logging.info('Creating HDF5 table for FEMDof objects')
        else:
            self.tblFEMDof = objs.FEMDof
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMNodalMass object table
            self.tblFEMNodalMass = self.h5.createTable(objs,
                'FEMNodalMass', FEMNodalMassCols,
                '')
            logging.info('Creating HDF5 table for FEMNodalMass objects')
        else:
            self.tblFEMNodalMass = objs.FEMNodalMass
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMNLElasticStrainStress object table
            self.tblFEMNLElasticStrainStress = self.h5.createTable(objs,
                'FEMNLElasticStrainStress', FEMNLElasticStrainStressCols,
                '')
            logging.info('Creating HDF5 table for FEMNLElasticStrainStress objects')
        else:
            self.tblFEMNLElasticStrainStress = objs.FEMNLElasticStrainStress
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMBoundary object table
            self.tblFEMBoundary = self.h5.createTable(objs,
                'FEMBoundary', FEMBoundaryCols,
                '')
            logging.info('Creating HDF5 table for FEMBoundary objects')
        else:
            self.tblFEMBoundary = objs.FEMBoundary
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSectionPipe object table
            self.tblFEMSectionPipe = self.h5.createTable(objs,
                'FEMSectionPipe', FEMSectionPipeCols,
                '')
            logging.info('Creating HDF5 table for FEMSectionPipe objects')
        else:
            self.tblFEMSectionPipe = objs.FEMSectionPipe
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMCoordSystem object table
            self.tblFEMCoordSystem = self.h5.createTable(objs,
                'FEMCoordSystem', FEMCoordSystemCols,
                '')
            logging.info('Creating HDF5 table for FEMCoordSystem objects')
        else:
            self.tblFEMCoordSystem = objs.FEMCoordSystem
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMNode object table
            self.tblFEMNode = self.h5.createTable(objs,
                'FEMNode', FEMNodeCols,
                '')
            logging.info('Creating HDF5 table for FEMNode objects')
        else:
            self.tblFEMNode = objs.FEMNode
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMTruss object table
            self.tblFEMTruss = self.h5.createTable(objs,
                'FEMTruss', FEMTrussCols,
                '')
            logging.info('Creating HDF5 table for FEMTruss objects')
        else:
            self.tblFEMTruss = objs.FEMTruss
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMTimeFunctionData object table
            self.tblFEMTimeFunctionData = self.h5.createTable(objs,
                'FEMTimeFunctionData', FEMTimeFunctionDataCols,
                '')
            logging.info('Creating HDF5 table for FEMTimeFunctionData objects')
        else:
            self.tblFEMTimeFunctionData = objs.FEMTimeFunctionData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMPlasticMlMaterials object table
            self.tblFEMPlasticMlMaterials = self.h5.createTable(objs,
                'FEMPlasticMlMaterials', FEMPlasticMlMaterialsCols,
                '')
            logging.info('Creating HDF5 table for FEMPlasticMlMaterials objects')
        else:
            self.tblFEMPlasticMlMaterials = objs.FEMPlasticMlMaterials
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMPlateGroup object table
            self.tblFEMPlateGroup = self.h5.createTable(objs,
                'FEMPlateGroup', FEMPlateGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMPlateGroup objects')
        else:
            self.tblFEMPlateGroup = objs.FEMPlateGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMBeam object table
            self.tblFEMBeam = self.h5.createTable(objs,
                'FEMBeam', FEMBeamCols,
                '')
            logging.info('Creating HDF5 table for FEMBeam objects')
        else:
            self.tblFEMBeam = objs.FEMBeam
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMCurvMomentData object table
            self.tblFEMCurvMomentData = self.h5.createTable(objs,
                'FEMCurvMomentData', FEMCurvMomentDataCols,
                '')
            logging.info('Creating HDF5 table for FEMCurvMomentData objects')
        else:
            self.tblFEMCurvMomentData = objs.FEMCurvMomentData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMPropertysets object table
            self.tblFEMPropertysets = self.h5.createTable(objs,
                'FEMPropertysets', FEMPropertysetsCols,
                '')
            logging.info('Creating HDF5 table for FEMPropertysets objects')
        else:
            self.tblFEMPropertysets = objs.FEMPropertysets
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMOrthotropicMaterial object table
            self.tblFEMOrthotropicMaterial = self.h5.createTable(objs,
                'FEMOrthotropicMaterial', FEMOrthotropicMaterialCols,
                '')
            logging.info('Creating HDF5 table for FEMOrthotropicMaterial objects')
        else:
            self.tblFEMOrthotropicMaterial = objs.FEMOrthotropicMaterial
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMAppliedLoads object table
            self.tblFEMAppliedLoads = self.h5.createTable(objs,
                'FEMAppliedLoads', FEMAppliedLoadsCols,
                '')
            logging.info('Creating HDF5 table for FEMAppliedLoads objects')
        else:
            self.tblFEMAppliedLoads = objs.FEMAppliedLoads
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMThermoOrthData object table
            self.tblFEMThermoOrthData = self.h5.createTable(objs,
                'FEMThermoOrthData', FEMThermoOrthDataCols,
                '')
            logging.info('Creating HDF5 table for FEMThermoOrthData objects')
        else:
            self.tblFEMThermoOrthData = objs.FEMThermoOrthData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMContactPairs object table
            self.tblFEMContactPairs = self.h5.createTable(objs,
                'FEMContactPairs', FEMContactPairsCols,
                '')
            logging.info('Creating HDF5 table for FEMContactPairs objects')
        else:
            self.tblFEMContactPairs = objs.FEMContactPairs
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMGeneral object table
            self.tblFEMGeneral = self.h5.createTable(objs,
                'FEMGeneral', FEMGeneralCols,
                '')
            logging.info('Creating HDF5 table for FEMGeneral objects')
        else:
            self.tblFEMGeneral = objs.FEMGeneral
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMBeamNode object table
            self.tblFEMBeamNode = self.h5.createTable(objs,
                'FEMBeamNode', FEMBeamNodeCols,
                '')
            logging.info('Creating HDF5 table for FEMBeamNode objects')
        else:
            self.tblFEMBeamNode = objs.FEMBeamNode
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSectionRect object table
            self.tblFEMSectionRect = self.h5.createTable(objs,
                'FEMSectionRect', FEMSectionRectCols,
                '')
            logging.info('Creating HDF5 table for FEMSectionRect objects')
        else:
            self.tblFEMSectionRect = objs.FEMSectionRect
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMBeamLoad object table
            self.tblFEMBeamLoad = self.h5.createTable(objs,
                'FEMBeamLoad', FEMBeamLoadCols,
                '')
            logging.info('Creating HDF5 table for FEMBeamLoad objects')
        else:
            self.tblFEMBeamLoad = objs.FEMBeamLoad
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMLoadMassProportional object table
            self.tblFEMLoadMassProportional = self.h5.createTable(objs,
                'FEMLoadMassProportional', FEMLoadMassProportionalCols,
                '')
            logging.info('Creating HDF5 table for FEMLoadMassProportional objects')
        else:
            self.tblFEMLoadMassProportional = objs.FEMLoadMassProportional
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMLink object table
            self.tblFEMLink = self.h5.createTable(objs,
                'FEMLink', FEMLinkCols,
                '')
            logging.info('Creating HDF5 table for FEMLink objects')
        else:
            self.tblFEMLink = objs.FEMLink
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMAxesNode object table
            self.tblFEMAxesNode = self.h5.createTable(objs,
                'FEMAxesNode', FEMAxesNodeCols,
                '')
            logging.info('Creating HDF5 table for FEMAxesNode objects')
        else:
            self.tblFEMAxesNode = objs.FEMAxesNode
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMNMTimeMass object table
            self.tblFEMNMTimeMass = self.h5.createTable(objs,
                'FEMNMTimeMass', FEMNMTimeMassCols,
                '')
            logging.info('Creating HDF5 table for FEMNMTimeMass objects')
        else:
            self.tblFEMNMTimeMass = objs.FEMNMTimeMass
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMAppliedDisplacement object table
            self.tblFEMAppliedDisplacement = self.h5.createTable(objs,
                'FEMAppliedDisplacement', FEMAppliedDisplacementCols,
                '')
            logging.info('Creating HDF5 table for FEMAppliedDisplacement objects')
        else:
            self.tblFEMAppliedDisplacement = objs.FEMAppliedDisplacement
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMTimeFunctions object table
            self.tblFEMTimeFunctions = self.h5.createTable(objs,
                'FEMTimeFunctions', FEMTimeFunctionsCols,
                '')
            logging.info('Creating HDF5 table for FEMTimeFunctions objects')
        else:
            self.tblFEMTimeFunctions = objs.FEMTimeFunctions
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMForceStrainData object table
            self.tblFEMForceStrainData = self.h5.createTable(objs,
                'FEMForceStrainData', FEMForceStrainDataCols,
                '')
            logging.info('Creating HDF5 table for FEMForceStrainData objects')
        else:
            self.tblFEMForceStrainData = objs.FEMForceStrainData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSkewDOF object table
            self.tblFEMSkewDOF = self.h5.createTable(objs,
                'FEMSkewDOF', FEMSkewDOFCols,
                '')
            logging.info('Creating HDF5 table for FEMSkewDOF objects')
        else:
            self.tblFEMSkewDOF = objs.FEMSkewDOF
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSectionI object table
            self.tblFEMSectionI = self.h5.createTable(objs,
                'FEMSectionI', FEMSectionICols,
                '')
            logging.info('Creating HDF5 table for FEMSectionI objects')
        else:
            self.tblFEMSectionI = objs.FEMSectionI
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMPlasticBilinearMaterial object table
            self.tblFEMPlasticBilinearMaterial = self.h5.createTable(objs,
                'FEMPlasticBilinearMaterial', FEMPlasticBilinearMaterialCols,
                '')
            logging.info('Creating HDF5 table for FEMPlasticBilinearMaterial objects')
        else:
            self.tblFEMPlasticBilinearMaterial = objs.FEMPlasticBilinearMaterial
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMMTForceData object table
            self.tblFEMMTForceData = self.h5.createTable(objs,
                'FEMMTForceData', FEMMTForceDataCols,
                '')
            logging.info('Creating HDF5 table for FEMMTForceData objects')
        else:
            self.tblFEMMTForceData = objs.FEMMTForceData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMShellPressure object table
            self.tblFEMShellPressure = self.h5.createTable(objs,
                'FEMShellPressure', FEMShellPressureCols,
                '')
            logging.info('Creating HDF5 table for FEMShellPressure objects')
        else:
            self.tblFEMShellPressure = objs.FEMShellPressure
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMMatrices object table
            self.tblFEMMatrices = self.h5.createTable(objs,
                'FEMMatrices', FEMMatricesCols,
                '')
            logging.info('Creating HDF5 table for FEMMatrices objects')
        else:
            self.tblFEMMatrices = objs.FEMMatrices
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMDamping object table
            self.tblFEMDamping = self.h5.createTable(objs,
                'FEMDamping', FEMDampingCols,
                '')
            logging.info('Creating HDF5 table for FEMDamping objects')
        else:
            self.tblFEMDamping = objs.FEMDamping
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMMaterial object table
            self.tblFEMMaterial = self.h5.createTable(objs,
                'FEMMaterial', FEMMaterialCols,
                '')
            logging.info('Creating HDF5 table for FEMMaterial objects')
        else:
            self.tblFEMMaterial = objs.FEMMaterial
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMMatrixData object table
            self.tblFEMMatrixData = self.h5.createTable(objs,
                'FEMMatrixData', FEMMatrixDataCols,
                '')
            logging.info('Creating HDF5 table for FEMMatrixData objects')
        else:
            self.tblFEMMatrixData = objs.FEMMatrixData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMShellAxesOrtho object table
            self.tblFEMShellAxesOrtho = self.h5.createTable(objs,
                'FEMShellAxesOrtho', FEMShellAxesOrthoCols,
                '')
            logging.info('Creating HDF5 table for FEMShellAxesOrtho objects')
        else:
            self.tblFEMShellAxesOrtho = objs.FEMShellAxesOrtho
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMEndreleases object table
            self.tblFEMEndreleases = self.h5.createTable(objs,
                'FEMEndreleases', FEMEndreleasesCols,
                '')
            logging.info('Creating HDF5 table for FEMEndreleases objects')
        else:
            self.tblFEMEndreleases = objs.FEMEndreleases
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMTrussGroup object table
            self.tblFEMTrussGroup = self.h5.createTable(objs,
                'FEMTrussGroup', FEMTrussGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMTrussGroup objects')
        else:
            self.tblFEMTrussGroup = objs.FEMTrussGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMInitialTemperature object table
            self.tblFEMInitialTemperature = self.h5.createTable(objs,
                'FEMInitialTemperature', FEMInitialTemperatureCols,
                '')
            logging.info('Creating HDF5 table for FEMInitialTemperature objects')
        else:
            self.tblFEMInitialTemperature = objs.FEMInitialTemperature
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMThermoIsoMaterials object table
            self.tblFEMThermoIsoMaterials = self.h5.createTable(objs,
                'FEMThermoIsoMaterials', FEMThermoIsoMaterialsCols,
                '')
            logging.info('Creating HDF5 table for FEMThermoIsoMaterials objects')
        else:
            self.tblFEMThermoIsoMaterials = objs.FEMThermoIsoMaterials
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMThermoIsoData object table
            self.tblFEMThermoIsoData = self.h5.createTable(objs,
                'FEMThermoIsoData', FEMThermoIsoDataCols,
                '')
            logging.info('Creating HDF5 table for FEMThermoIsoData objects')
        else:
            self.tblFEMThermoIsoData = objs.FEMThermoIsoData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMContactGroup3 object table
            self.tblFEMContactGroup3 = self.h5.createTable(objs,
                'FEMContactGroup3', FEMContactGroup3Cols,
                '')
            logging.info('Creating HDF5 table for FEMContactGroup3 objects')
        else:
            self.tblFEMContactGroup3 = objs.FEMContactGroup3
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMNLElasticMaterials object table
            self.tblFEMNLElasticMaterials = self.h5.createTable(objs,
                'FEMNLElasticMaterials', FEMNLElasticMaterialsCols,
                '')
            logging.info('Creating HDF5 table for FEMNLElasticMaterials objects')
        else:
            self.tblFEMNLElasticMaterials = objs.FEMNLElasticMaterials
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMPlate object table
            self.tblFEMPlate = self.h5.createTable(objs,
                'FEMPlate', FEMPlateCols,
                '')
            logging.info('Creating HDF5 table for FEMPlate objects')
        else:
            self.tblFEMPlate = objs.FEMPlate
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMIsoBeam object table
            self.tblFEMIsoBeam = self.h5.createTable(objs,
                'FEMIsoBeam', FEMIsoBeamCols,
                '')
            logging.info('Creating HDF5 table for FEMIsoBeam objects')
        else:
            self.tblFEMIsoBeam = objs.FEMIsoBeam
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMAppliedConcentratedLoad object table
            self.tblFEMAppliedConcentratedLoad = self.h5.createTable(objs,
                'FEMAppliedConcentratedLoad', FEMAppliedConcentratedLoadCols,
                '')
            logging.info('Creating HDF5 table for FEMAppliedConcentratedLoad objects')
        else:
            self.tblFEMAppliedConcentratedLoad = objs.FEMAppliedConcentratedLoad
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMTwoDSolidGroup object table
            self.tblFEMTwoDSolidGroup = self.h5.createTable(objs,
                'FEMTwoDSolidGroup', FEMTwoDSolidGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMTwoDSolidGroup objects')
        else:
            self.tblFEMTwoDSolidGroup = objs.FEMTwoDSolidGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMGroup object table
            self.tblFEMGroup = self.h5.createTable(objs,
                'FEMGroup', FEMGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMGroup objects')
        else:
            self.tblFEMGroup = objs.FEMGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMProperties object table
            self.tblFEMProperties = self.h5.createTable(objs,
                'FEMProperties', FEMPropertiesCols,
                '')
            logging.info('Creating HDF5 table for FEMProperties objects')
        else:
            self.tblFEMProperties = objs.FEMProperties
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMThreeDSolidGroup object table
            self.tblFEMThreeDSolidGroup = self.h5.createTable(objs,
                'FEMThreeDSolidGroup', FEMThreeDSolidGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMThreeDSolidGroup objects')
        else:
            self.tblFEMThreeDSolidGroup = objs.FEMThreeDSolidGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMThreeDSolid object table
            self.tblFEMThreeDSolid = self.h5.createTable(objs,
                'FEMThreeDSolid', FEMThreeDSolidCols,
                '')
            logging.info('Creating HDF5 table for FEMThreeDSolid objects')
        else:
            self.tblFEMThreeDSolid = objs.FEMThreeDSolid
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSectionProp object table
            self.tblFEMSectionProp = self.h5.createTable(objs,
                'FEMSectionProp', FEMSectionPropCols,
                '')
            logging.info('Creating HDF5 table for FEMSectionProp objects')
        else:
            self.tblFEMSectionProp = objs.FEMSectionProp
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMElasticMaterial object table
            self.tblFEMElasticMaterial = self.h5.createTable(objs,
                'FEMElasticMaterial', FEMElasticMaterialCols,
                '')
            logging.info('Creating HDF5 table for FEMElasticMaterial objects')
        else:
            self.tblFEMElasticMaterial = objs.FEMElasticMaterial
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMPoints object table
            self.tblFEMPoints = self.h5.createTable(objs,
                'FEMPoints', FEMPointsCols,
                '')
            logging.info('Creating HDF5 table for FEMPoints objects')
        else:
            self.tblFEMPoints = objs.FEMPoints
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMThermoOrthMaterials object table
            self.tblFEMThermoOrthMaterials = self.h5.createTable(objs,
                'FEMThermoOrthMaterials', FEMThermoOrthMaterialsCols,
                '')
            logging.info('Creating HDF5 table for FEMThermoOrthMaterials objects')
        else:
            self.tblFEMThermoOrthMaterials = objs.FEMThermoOrthMaterials
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMConstraints object table
            self.tblFEMConstraints = self.h5.createTable(objs,
                'FEMConstraints', FEMConstraintsCols,
                '')
            logging.info('Creating HDF5 table for FEMConstraints objects')
        else:
            self.tblFEMConstraints = objs.FEMConstraints
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMMCrigidities object table
            self.tblFEMMCrigidities = self.h5.createTable(objs,
                'FEMMCrigidities', FEMMCrigiditiesCols,
                '')
            logging.info('Creating HDF5 table for FEMMCrigidities objects')
        else:
            self.tblFEMMCrigidities = objs.FEMMCrigidities
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSkeySysNode object table
            self.tblFEMSkeySysNode = self.h5.createTable(objs,
                'FEMSkeySysNode', FEMSkeySysNodeCols,
                '')
            logging.info('Creating HDF5 table for FEMSkeySysNode objects')
        else:
            self.tblFEMSkeySysNode = objs.FEMSkeySysNode
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMIsoBeamGroup object table
            self.tblFEMIsoBeamGroup = self.h5.createTable(objs,
                'FEMIsoBeamGroup', FEMIsoBeamGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMIsoBeamGroup objects')
        else:
            self.tblFEMIsoBeamGroup = objs.FEMIsoBeamGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMShellDOF object table
            self.tblFEMShellDOF = self.h5.createTable(objs,
                'FEMShellDOF', FEMShellDOFCols,
                '')
            logging.info('Creating HDF5 table for FEMShellDOF objects')
        else:
            self.tblFEMShellDOF = objs.FEMShellDOF
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMCrossSections object table
            self.tblFEMCrossSections = self.h5.createTable(objs,
                'FEMCrossSections', FEMCrossSectionsCols,
                '')
            logging.info('Creating HDF5 table for FEMCrossSections objects')
        else:
            self.tblFEMCrossSections = objs.FEMCrossSections
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMTwistMomentData object table
            self.tblFEMTwistMomentData = self.h5.createTable(objs,
                'FEMTwistMomentData', FEMTwistMomentDataCols,
                '')
            logging.info('Creating HDF5 table for FEMTwistMomentData objects')
        else:
            self.tblFEMTwistMomentData = objs.FEMTwistMomentData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMShell object table
            self.tblFEMShell = self.h5.createTable(objs,
                'FEMShell', FEMShellCols,
                '')
            logging.info('Creating HDF5 table for FEMShell objects')
        else:
            self.tblFEMShell = objs.FEMShell
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMNTNContact object table
            self.tblFEMNTNContact = self.h5.createTable(objs,
                'FEMNTNContact', FEMNTNContactCols,
                '')
            logging.info('Creating HDF5 table for FEMNTNContact objects')
        else:
            self.tblFEMNTNContact = objs.FEMNTNContact
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMShellLayer object table
            self.tblFEMShellLayer = self.h5.createTable(objs,
                'FEMShellLayer', FEMShellLayerCols,
                '')
            logging.info('Creating HDF5 table for FEMShellLayer objects')
        else:
            self.tblFEMShellLayer = objs.FEMShellLayer
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSkewSysAngles object table
            self.tblFEMSkewSysAngles = self.h5.createTable(objs,
                'FEMSkewSysAngles', FEMSkewSysAnglesCols,
                '')
            logging.info('Creating HDF5 table for FEMSkewSysAngles objects')
        else:
            self.tblFEMSkewSysAngles = objs.FEMSkewSysAngles
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMGroundMotionRecord object table
            self.tblFEMGroundMotionRecord = self.h5.createTable(objs,
                'FEMGroundMotionRecord', FEMGroundMotionRecordCols,
                '')
            logging.info('Creating HDF5 table for FEMGroundMotionRecord objects')
        else:
            self.tblFEMGroundMotionRecord = objs.FEMGroundMotionRecord
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMGeneralGroup object table
            self.tblFEMGeneralGroup = self.h5.createTable(objs,
                'FEMGeneralGroup', FEMGeneralGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMGeneralGroup objects')
        else:
            self.tblFEMGeneralGroup = objs.FEMGeneralGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMTwoDSolid object table
            self.tblFEMTwoDSolid = self.h5.createTable(objs,
                'FEMTwoDSolid', FEMTwoDSolidCols,
                '')
            logging.info('Creating HDF5 table for FEMTwoDSolid objects')
        else:
            self.tblFEMTwoDSolid = objs.FEMTwoDSolid
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMAppliedTemperature object table
            self.tblFEMAppliedTemperature = self.h5.createTable(objs,
                'FEMAppliedTemperature', FEMAppliedTemperatureCols,
                '')
            logging.info('Creating HDF5 table for FEMAppliedTemperature objects')
        else:
            self.tblFEMAppliedTemperature = objs.FEMAppliedTemperature
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMMatrixSets object table
            self.tblFEMMatrixSets = self.h5.createTable(objs,
                'FEMMatrixSets', FEMMatrixSetsCols,
                '')
            logging.info('Creating HDF5 table for FEMMatrixSets objects')
        else:
            self.tblFEMMatrixSets = objs.FEMMatrixSets
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMConstraintCoef object table
            self.tblFEMConstraintCoef = self.h5.createTable(objs,
                'FEMConstraintCoef', FEMConstraintCoefCols,
                '')
            logging.info('Creating HDF5 table for FEMConstraintCoef objects')
        else:
            self.tblFEMConstraintCoef = objs.FEMConstraintCoef
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSectionBox object table
            self.tblFEMSectionBox = self.h5.createTable(objs,
                'FEMSectionBox', FEMSectionBoxCols,
                '')
            logging.info('Creating HDF5 table for FEMSectionBox objects')
        else:
            self.tblFEMSectionBox = objs.FEMSectionBox
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMNKDisplForce object table
            self.tblFEMNKDisplForce = self.h5.createTable(objs,
                'FEMNKDisplForce', FEMNKDisplForceCols,
                '')
            logging.info('Creating HDF5 table for FEMNKDisplForce objects')
        else:
            self.tblFEMNKDisplForce = objs.FEMNKDisplForce
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMPlasticStrainStress object table
            self.tblFEMPlasticStrainStress = self.h5.createTable(objs,
                'FEMPlasticStrainStress', FEMPlasticStrainStressCols,
                '')
            logging.info('Creating HDF5 table for FEMPlasticStrainStress objects')
        else:
            self.tblFEMPlasticStrainStress = objs.FEMPlasticStrainStress
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMShellAxesOrthoData object table
            self.tblFEMShellAxesOrthoData = self.h5.createTable(objs,
                'FEMShellAxesOrthoData', FEMShellAxesOrthoDataCols,
                '')
            logging.info('Creating HDF5 table for FEMShellAxesOrthoData objects')
        else:
            self.tblFEMShellAxesOrthoData = objs.FEMShellAxesOrthoData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMGeneralNode object table
            self.tblFEMGeneralNode = self.h5.createTable(objs,
                'FEMGeneralNode', FEMGeneralNodeCols,
                '')
            logging.info('Creating HDF5 table for FEMGeneralNode objects')
        else:
            self.tblFEMGeneralNode = objs.FEMGeneralNode
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMStrLines object table
            self.tblFEMStrLines = self.h5.createTable(objs,
                'FEMStrLines', FEMStrLinesCols,
                '')
            logging.info('Creating HDF5 table for FEMStrLines objects')
        else:
            self.tblFEMStrLines = objs.FEMStrLines
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMContactSurface object table
            self.tblFEMContactSurface = self.h5.createTable(objs,
                'FEMContactSurface', FEMContactSurfaceCols,
                '')
            logging.info('Creating HDF5 table for FEMContactSurface objects')
        else:
            self.tblFEMContactSurface = objs.FEMContactSurface
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMMCForceData object table
            self.tblFEMMCForceData = self.h5.createTable(objs,
                'FEMMCForceData', FEMMCForceDataCols,
                '')
            logging.info('Creating HDF5 table for FEMMCForceData objects')
        else:
            self.tblFEMMCForceData = objs.FEMMCForceData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSpring object table
            self.tblFEMSpring = self.h5.createTable(objs,
                'FEMSpring', FEMSpringCols,
                '')
            logging.info('Creating HDF5 table for FEMSpring objects')
        else:
            self.tblFEMSpring = objs.FEMSpring
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMSpringGroup object table
            self.tblFEMSpringGroup = self.h5.createTable(objs,
                'FEMSpringGroup', FEMSpringGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMSpringGroup objects')
        else:
            self.tblFEMSpringGroup = objs.FEMSpringGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create FEMShellGroup object table
            self.tblFEMShellGroup = self.h5.createTable(objs,
                'FEMShellGroup', FEMShellGroupCols,
                '')
            logging.info('Creating HDF5 table for FEMShellGroup objects')
        else:
            self.tblFEMShellGroup = objs.FEMShellGroup
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create DaqUnit object table
            self.tblDaqUnit = self.h5.createTable(objs,
                'DaqUnit', DaqUnitCols,
                'Data acquisition unit (e.g. Narada node). ')
            logging.info('Creating HDF5 table for DaqUnit objects')
        else:
            self.tblDaqUnit = objs.DaqUnit
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create DaqUnitChannel object table
            self.tblDaqUnitChannel = self.h5.createTable(objs,
                'DaqUnitChannel', DaqUnitChannelCols,
                'Data acquisition unit channel. ')
            logging.info('Creating HDF5 table for DaqUnitChannel objects')
        else:
            self.tblDaqUnitChannel = objs.DaqUnitChannel
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create Sensor object table
            self.tblSensor = self.h5.createTable(objs,
                'Sensor', SensorCols,
                '')
            logging.info('Creating HDF5 table for Sensor objects')
        else:
            self.tblSensor = objs.Sensor
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create Transducer object table
            self.tblTransducer = self.h5.createTable(objs,
                'Transducer', TransducerCols,
                '')
            logging.info('Creating HDF5 table for Transducer objects')
        else:
            self.tblTransducer = objs.Transducer
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create Experiment object table
            self.tblExperiment = self.h5.createTable(objs,
                'Experiment', ExperimentCols,
                '')
            logging.info('Creating HDF5 table for Experiment objects')
        else:
            self.tblExperiment = objs.Experiment
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create Cycle object table
            self.tblCycle = self.h5.createTable(objs,
                'Cycle', CycleCols,
                '')
            logging.info('Creating HDF5 table for Cycle objects')
        else:
            self.tblCycle = objs.Cycle
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create SensorTimeData object table
            self.tblSensorTimeData = self.h5.createTable(objs,
                'SensorTimeData', SensorTimeDataCols,
                '')
            logging.info('Creating HDF5 table for SensorTimeData objects')
        else:
            self.tblSensorTimeData = objs.SensorTimeData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create TransducerTimeData object table
            self.tblTransducerTimeData = self.h5.createTable(objs,
                'TransducerTimeData', TransducerTimeDataCols,
                '')
            logging.info('Creating HDF5 table for TransducerTimeData objects')
        else:
            self.tblTransducerTimeData = objs.TransducerTimeData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create ExperimentStructureData object table
            self.tblExperimentStructureData = self.h5.createTable(objs,
                'ExperimentStructureData', ExperimentStructureDataCols,
                '')
            logging.info('Creating HDF5 table for ExperimentStructureData objects')
        else:
            self.tblExperimentStructureData = objs.ExperimentStructureData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create TransducerCycleData object table
            self.tblTransducerCycleData = self.h5.createTable(objs,
                'TransducerCycleData', TransducerCycleDataCols,
                '')
            logging.info('Creating HDF5 table for TransducerCycleData objects')
        else:
            self.tblTransducerCycleData = objs.TransducerCycleData
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create MappingMatrix object table
            self.tblMappingMatrix = self.h5.createTable(objs,
                'MappingMatrix', MappingMatrixCols,
                'Linear mapping between two node-related quantities. ')
            logging.info('Creating HDF5 table for MappingMatrix objects')
        else:
            self.tblMappingMatrix = objs.MappingMatrix
        if mode == 'w':
            # Create MappingMatrix array data group
            self.grpMappingMatrix = self.h5.createGroup(arrays,
                'MappingMatrix', 'Linear mapping between two node-related quantities. ')
            logging.info('Creating HDF5 group for MappingMatrix array data')
        else:
            self.grpMappingMatrix = self.h5.getNode(arrays, 'MappingMatrix')
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create DaqUnitChannelData object table
            self.tblDaqUnitChannelData = self.h5.createTable(objs,
                'DaqUnitChannelData', DaqUnitChannelDataCols,
                'Data acquisition unit raw channel data. ')
            logging.info('Creating HDF5 table for DaqUnitChannelData objects')
        else:
            self.tblDaqUnitChannelData = objs.DaqUnitChannelData
        if mode == 'w':
            # Create DaqUnitChannelData signal data group
            self.grpDaqUnitChannelData = self.h5.createGroup(signals,
                'DaqUnitChannelData', 'Data acquisition unit raw channel data. ')
            logging.info('Creating HDF5 group for DaqUnitChannelData signal data')
        else:
            self.grpDaqUnitChannelData = self.h5.getNode(signals, 'DaqUnitChannelData')
        if mode == 'w':
            self.h5.flush()
        if mode == 'w':
            # Create AccelMeas object table
            self.tblAccelMeas = self.h5.createTable(objs,
                'AccelMeas', AccelMeasCols,
                'Multi-axis accelerometer measurement data. ')
            logging.info('Creating HDF5 table for AccelMeas objects')
        else:
            self.tblAccelMeas = objs.AccelMeas
        if mode == 'w':
            # Create AccelMeas signal data group
            self.grpAccelMeas = self.h5.createGroup(signals,
                'AccelMeas', 'Multi-axis accelerometer measurement data. ')
            logging.info('Creating HDF5 group for AccelMeas signal data')
        else:
            self.grpAccelMeas = self.h5.getNode(signals, 'AccelMeas')
        if mode == 'w':
            self.h5.flush()

    ## Gets the HDF5 table row of an Structure object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see StructureProps
    def getStructure(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblStructure.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an Structure object.
    #
    # The ID field of the given Structure properties must be set to
    # the SQL object ID.
    #
    # @param fields Structure object fields
    # @see getStructure()
    def addStructure(self, fields):
        row = self.tblStructure.row
        row['id'] = fields.id
        row['Name'] = fields.mName
        row['Description'] = fields.mDescription
        row['Type'] = fields.mType
        row['DistanceUnit'] = self.UnitEnumList.index(str(fields.mDistanceUnit))
        row['ForceUnit'] = self.UnitEnumList.index(str(fields.mForceUnit))
        row['WeightUnit'] = self.UnitEnumList.index(str(fields.mWeightUnit))
        row.append()
        self.tblStructure.flush()

    ## Adds a Structure set.
    #
    # The ID field of each Structure property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of StructureFields objects
    # @see StructureProps
    # @see getStructure()
    def addStructureList(self, fieldsList):
        row = self.tblStructure.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Name'] = fields.mName
            row['Description'] = fields.mDescription
            row['Type'] = fields.mType
            row['DistanceUnit'] = fields.mDistanceUnit
            row['ForceUnit'] = fields.mForceUnit
            row['WeightUnit'] = fields.mWeightUnit
            row.append()
        self.tblStructure.flush()

    ## Gets the HDF5 table row of an FEMDof object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMDofProps
    def getFEMDof(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMDof.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMDof object.
    #
    # The ID field of the given FEMDof properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMDof object fields
    # @see getFEMDof()
    def addFEMDof(self, fields):
        row = self.tblFEMDof.row
        row['id'] = fields.id
        row['Structure'] = fields.mStructure
        row['LocalId'] = fields.mLocalId
        row['Node'] = fields.mNode
        row['Direction'] = self.QuantityEnumList.index(str(fields.mDirection))
        row.append()
        self.tblFEMDof.flush()

    ## Adds a FEMDof set.
    #
    # The ID field of each FEMDof property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMDofFields objects
    # @see FEMDofProps
    # @see getFEMDof()
    def addFEMDofList(self, fieldsList):
        row = self.tblFEMDof.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Structure'] = fields.mStructure
            row['LocalId'] = fields.mLocalId
            row['Node'] = fields.mNode
            row['Direction'] = fields.mDirection
            row.append()
        self.tblFEMDof.flush()

    ## Gets the HDF5 table row of an FEMNodalMass object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMNodalMassProps
    def getFEMNodalMass(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMNodalMass.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMNodalMass object.
    #
    # The ID field of the given FEMNodalMass properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMNodalMass object fields
    # @see getFEMNodalMass()
    def addFEMNodalMass(self, fields):
        row = self.tblFEMNodalMass.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['Mass1'] = fields.mMass1
        row['Mass2'] = fields.mMass2
        row['Mass3'] = fields.mMass3
        row['Mass4'] = fields.mMass4
        row['Mass5'] = fields.mMass5
        row['Mass6'] = fields.mMass6
        row['Node'] = fields.mNode
        row.append()
        self.tblFEMNodalMass.flush()

    ## Adds a FEMNodalMass set.
    #
    # The ID field of each FEMNodalMass property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMNodalMassFields objects
    # @see FEMNodalMassProps
    # @see getFEMNodalMass()
    def addFEMNodalMassList(self, fieldsList):
        row = self.tblFEMNodalMass.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['Mass1'] = fields.mMass1
            row['Mass2'] = fields.mMass2
            row['Mass3'] = fields.mMass3
            row['Mass4'] = fields.mMass4
            row['Mass5'] = fields.mMass5
            row['Mass6'] = fields.mMass6
            row['Node'] = fields.mNode
            row.append()
        self.tblFEMNodalMass.flush()

    ## Gets the HDF5 table row of an FEMNLElasticStrainStress object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMNLElasticStrainStressProps
    def getFEMNLElasticStrainStress(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMNLElasticStrainStress.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMNLElasticStrainStress object.
    #
    # The ID field of the given FEMNLElasticStrainStress properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMNLElasticStrainStress object fields
    # @see getFEMNLElasticStrainStress()
    def addFEMNLElasticStrainStress(self, fields):
        row = self.tblFEMNLElasticStrainStress.row
        row['id'] = fields.id
        row['MaterialID'] = fields.mMaterialID
        row['RecordNumber'] = fields.mRecordNumber
        row['Strain'] = fields.mStrain
        row['Stress'] = fields.mStress
        row.append()
        self.tblFEMNLElasticStrainStress.flush()

    ## Adds a FEMNLElasticStrainStress set.
    #
    # The ID field of each FEMNLElasticStrainStress property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMNLElasticStrainStressFields objects
    # @see FEMNLElasticStrainStressProps
    # @see getFEMNLElasticStrainStress()
    def addFEMNLElasticStrainStressList(self, fieldsList):
        row = self.tblFEMNLElasticStrainStress.row
        for fields in fieldsList:
            row[name] = fields.id
            row['MaterialID'] = fields.mMaterialID
            row['RecordNumber'] = fields.mRecordNumber
            row['Strain'] = fields.mStrain
            row['Stress'] = fields.mStress
            row.append()
        self.tblFEMNLElasticStrainStress.flush()

    ## Gets the HDF5 table row of an FEMBoundary object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMBoundaryProps
    def getFEMBoundary(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMBoundary.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMBoundary object.
    #
    # The ID field of the given FEMBoundary properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMBoundary object fields
    # @see getFEMBoundary()
    def addFEMBoundary(self, fields):
        row = self.tblFEMBoundary.row
        row['id'] = fields.id
        row['Node'] = fields.mNode
        row['Ovalization'] = self.BoundaryTypeEnumList.index(str(fields.mOvalization))
        row['Phi'] = self.BoundaryTypeEnumList.index(str(fields.mPhi))
        row['Rx'] = self.BoundaryTypeEnumList.index(str(fields.mRx))
        row['Ry'] = self.BoundaryTypeEnumList.index(str(fields.mRy))
        row['Rz'] = self.BoundaryTypeEnumList.index(str(fields.mRz))
        row['Ux'] = self.BoundaryTypeEnumList.index(str(fields.mUx))
        row['Uy'] = self.BoundaryTypeEnumList.index(str(fields.mUy))
        row['Uz'] = self.BoundaryTypeEnumList.index(str(fields.mUz))
        row['Warping'] = fields.mWarping
        row.append()
        self.tblFEMBoundary.flush()

    ## Adds a FEMBoundary set.
    #
    # The ID field of each FEMBoundary property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMBoundaryFields objects
    # @see FEMBoundaryProps
    # @see getFEMBoundary()
    def addFEMBoundaryList(self, fieldsList):
        row = self.tblFEMBoundary.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Node'] = fields.mNode
            row['Ovalization'] = fields.mOvalization
            row['Phi'] = fields.mPhi
            row['Rx'] = fields.mRx
            row['Ry'] = fields.mRy
            row['Rz'] = fields.mRz
            row['Ux'] = fields.mUx
            row['Uy'] = fields.mUy
            row['Uz'] = fields.mUz
            row['Warping'] = fields.mWarping
            row.append()
        self.tblFEMBoundary.flush()

    ## Gets the HDF5 table row of an FEMSectionPipe object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSectionPipeProps
    def getFEMSectionPipe(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSectionPipe.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSectionPipe object.
    #
    # The ID field of the given FEMSectionPipe properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSectionPipe object fields
    # @see getFEMSectionPipe()
    def addFEMSectionPipe(self, fields):
        row = self.tblFEMSectionPipe.row
        row['id'] = fields.id
        row['Diameter'] = fields.mDiameter
        row['SC'] = fields.mSC
        row['SectionID'] = fields.mSectionID
        row['SSarea'] = fields.mSSarea
        row['TC'] = fields.mTC
        row['Thickness'] = fields.mThickness
        row['Torfac'] = fields.mTorfac
        row['TSarea'] = fields.mTSarea
        row.append()
        self.tblFEMSectionPipe.flush()

    ## Adds a FEMSectionPipe set.
    #
    # The ID field of each FEMSectionPipe property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSectionPipeFields objects
    # @see FEMSectionPipeProps
    # @see getFEMSectionPipe()
    def addFEMSectionPipeList(self, fieldsList):
        row = self.tblFEMSectionPipe.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Diameter'] = fields.mDiameter
            row['SC'] = fields.mSC
            row['SectionID'] = fields.mSectionID
            row['SSarea'] = fields.mSSarea
            row['TC'] = fields.mTC
            row['Thickness'] = fields.mThickness
            row['Torfac'] = fields.mTorfac
            row['TSarea'] = fields.mTSarea
            row.append()
        self.tblFEMSectionPipe.flush()

    ## Gets the HDF5 table row of an FEMCoordSystem object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMCoordSystemProps
    def getFEMCoordSystem(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMCoordSystem.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMCoordSystem object.
    #
    # The ID field of the given FEMCoordSystem properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMCoordSystem object fields
    # @see getFEMCoordSystem()
    def addFEMCoordSystem(self, fields):
        row = self.tblFEMCoordSystem.row
        row['id'] = fields.id
        row['AX'] = fields.mAX
        row['AY'] = fields.mAY
        row['AZ'] = fields.mAZ
        row['BBY'] = fields.mBBY
        row['BX'] = fields.mBX
        row['BZ'] = fields.mBZ
        row['Description'] = fields.mDescription
        row['Mode'] = fields.mMode
        row['P1'] = fields.mP1
        row['P2'] = fields.mP2
        row['P3'] = fields.mP3
        row['Type'] = fields.mType
        row['Xorigin'] = fields.mXorigin
        row['Yorigin'] = fields.mYorigin
        row['Zorigin'] = fields.mZorigin
        row['LocalID'] = fields.mLocalID
        row.append()
        self.tblFEMCoordSystem.flush()

    ## Adds a FEMCoordSystem set.
    #
    # The ID field of each FEMCoordSystem property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMCoordSystemFields objects
    # @see FEMCoordSystemProps
    # @see getFEMCoordSystem()
    def addFEMCoordSystemList(self, fieldsList):
        row = self.tblFEMCoordSystem.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AX'] = fields.mAX
            row['AY'] = fields.mAY
            row['AZ'] = fields.mAZ
            row['BBY'] = fields.mBBY
            row['BX'] = fields.mBX
            row['BZ'] = fields.mBZ
            row['Description'] = fields.mDescription
            row['Mode'] = fields.mMode
            row['P1'] = fields.mP1
            row['P2'] = fields.mP2
            row['P3'] = fields.mP3
            row['Type'] = fields.mType
            row['Xorigin'] = fields.mXorigin
            row['Yorigin'] = fields.mYorigin
            row['Zorigin'] = fields.mZorigin
            row['LocalID'] = fields.mLocalID
            row.append()
        self.tblFEMCoordSystem.flush()

    ## Gets the HDF5 table row of an FEMNode object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMNodeProps
    def getFEMNode(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMNode.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMNode object.
    #
    # The ID field of the given FEMNode properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMNode object fields
    # @see getFEMNode()
    def addFEMNode(self, fields):
        row = self.tblFEMNode.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['System'] = fields.mSystem
        row['X'] = fields.mX
        row['Y'] = fields.mY
        row['Z'] = fields.mZ
        row['LocalID'] = fields.mLocalID
        row.append()
        self.tblFEMNode.flush()

    ## Adds a FEMNode set.
    #
    # The ID field of each FEMNode property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMNodeFields objects
    # @see FEMNodeProps
    # @see getFEMNode()
    def addFEMNodeList(self, fieldsList):
        row = self.tblFEMNode.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['System'] = fields.mSystem
            row['X'] = fields.mX
            row['Y'] = fields.mY
            row['Z'] = fields.mZ
            row['LocalID'] = fields.mLocalID
            row.append()
        self.tblFEMNode.flush()

    ## Gets the HDF5 table row of an FEMTruss object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMTrussProps
    def getFEMTruss(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMTruss.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMTruss object.
    #
    # The ID field of the given FEMTruss properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMTruss object fields
    # @see getFEMTruss()
    def addFEMTruss(self, fields):
        row = self.tblFEMTruss.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['Epsin'] = fields.mEpsin
        row['Gapwidth'] = fields.mGapwidth
        row['Group'] = fields.mGroup
        row['Material'] = fields.mMaterial
        row['N1'] = fields.mN1
        row['N2'] = fields.mN2
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row['SectionArea'] = fields.mSectionArea
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row.append()
        self.tblFEMTruss.flush()

    ## Adds a FEMTruss set.
    #
    # The ID field of each FEMTruss property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMTrussFields objects
    # @see FEMTrussProps
    # @see getFEMTruss()
    def addFEMTrussList(self, fieldsList):
        row = self.tblFEMTruss.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['Epsin'] = fields.mEpsin
            row['Gapwidth'] = fields.mGapwidth
            row['Group'] = fields.mGroup
            row['Material'] = fields.mMaterial
            row['N1'] = fields.mN1
            row['N2'] = fields.mN2
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row['SectionArea'] = fields.mSectionArea
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row.append()
        self.tblFEMTruss.flush()

    ## Gets the HDF5 table row of an FEMTimeFunctionData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMTimeFunctionDataProps
    def getFEMTimeFunctionData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMTimeFunctionData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMTimeFunctionData object.
    #
    # The ID field of the given FEMTimeFunctionData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMTimeFunctionData object fields
    # @see getFEMTimeFunctionData()
    def addFEMTimeFunctionData(self, fields):
        row = self.tblFEMTimeFunctionData.row
        row['id'] = fields.id
        row['DataTime'] = fields.mDataTime
        row['GMRecordID'] = fields.mGMRecordID
        row['RecordNmb'] = fields.mRecordNmb
        row['TimeFunctionID'] = fields.mTimeFunctionID
        row['TimeValue'] = fields.mTimeValue
        row.append()
        self.tblFEMTimeFunctionData.flush()

    ## Adds a FEMTimeFunctionData set.
    #
    # The ID field of each FEMTimeFunctionData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMTimeFunctionDataFields objects
    # @see FEMTimeFunctionDataProps
    # @see getFEMTimeFunctionData()
    def addFEMTimeFunctionDataList(self, fieldsList):
        row = self.tblFEMTimeFunctionData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['DataTime'] = fields.mDataTime
            row['GMRecordID'] = fields.mGMRecordID
            row['RecordNmb'] = fields.mRecordNmb
            row['TimeFunctionID'] = fields.mTimeFunctionID
            row['TimeValue'] = fields.mTimeValue
            row.append()
        self.tblFEMTimeFunctionData.flush()

    ## Gets the HDF5 table row of an FEMPlasticMlMaterials object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMPlasticMlMaterialsProps
    def getFEMPlasticMlMaterials(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMPlasticMlMaterials.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMPlasticMlMaterials object.
    #
    # The ID field of the given FEMPlasticMlMaterials properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMPlasticMlMaterials object fields
    # @see getFEMPlasticMlMaterials()
    def addFEMPlasticMlMaterials(self, fields):
        row = self.tblFEMPlasticMlMaterials.row
        row['id'] = fields.id
        row['ALPHA'] = fields.mALPHA
        row['Density'] = fields.mDensity
        row['E'] = fields.mE
        row['Hardening'] = fields.mHardening
        row['MaterialID'] = fields.mMaterialID
        row['NU'] = fields.mNU
        row['TREF'] = fields.mTREF
        row.append()
        self.tblFEMPlasticMlMaterials.flush()

    ## Adds a FEMPlasticMlMaterials set.
    #
    # The ID field of each FEMPlasticMlMaterials property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMPlasticMlMaterialsFields objects
    # @see FEMPlasticMlMaterialsProps
    # @see getFEMPlasticMlMaterials()
    def addFEMPlasticMlMaterialsList(self, fieldsList):
        row = self.tblFEMPlasticMlMaterials.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ALPHA'] = fields.mALPHA
            row['Density'] = fields.mDensity
            row['E'] = fields.mE
            row['Hardening'] = fields.mHardening
            row['MaterialID'] = fields.mMaterialID
            row['NU'] = fields.mNU
            row['TREF'] = fields.mTREF
            row.append()
        self.tblFEMPlasticMlMaterials.flush()

    ## Gets the HDF5 table row of an FEMPlateGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMPlateGroupProps
    def getFEMPlateGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMPlateGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMPlateGroup object.
    #
    # The ID field of the given FEMPlateGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMPlateGroup object fields
    # @see getFEMPlateGroup()
    def addFEMPlateGroup(self, fields):
        row = self.tblFEMPlateGroup.row
        row['id'] = fields.id
        row['Displacement'] = fields.mDisplacement
        row['Group'] = fields.mGroup
        row['IniStrain'] = fields.mIniStrain
        row['MaterialID'] = fields.mMaterialID
        row['Result'] = fields.mResult
        row.append()
        self.tblFEMPlateGroup.flush()

    ## Adds a FEMPlateGroup set.
    #
    # The ID field of each FEMPlateGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMPlateGroupFields objects
    # @see FEMPlateGroupProps
    # @see getFEMPlateGroup()
    def addFEMPlateGroupList(self, fieldsList):
        row = self.tblFEMPlateGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Displacement'] = fields.mDisplacement
            row['Group'] = fields.mGroup
            row['IniStrain'] = fields.mIniStrain
            row['MaterialID'] = fields.mMaterialID
            row['Result'] = fields.mResult
            row.append()
        self.tblFEMPlateGroup.flush()

    ## Gets the HDF5 table row of an FEMBeam object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMBeamProps
    def getFEMBeam(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMBeam.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMBeam object.
    #
    # The ID field of the given FEMBeam properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMBeam object fields
    # @see getFEMBeam()
    def addFEMBeam(self, fields):
        row = self.tblFEMBeam.row
        row['id'] = fields.id
        row['AuxNode'] = fields.mAuxNode
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['EndRelease'] = fields.mEndRelease
        row['Epsin'] = fields.mEpsin
        row['Group'] = fields.mGroup
        row['IRigidEnd'] = fields.mIRigidEnd
        row['JRigidEnd'] = fields.mJRigidEnd
        row['MaterialID'] = fields.mMaterialID
        row['Node1'] = fields.mNode1
        row['Node2'] = fields.mNode2
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row['SectionID'] = fields.mSectionID
        row['Subdivision'] = fields.mSubdivision
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row.append()
        self.tblFEMBeam.flush()

    ## Adds a FEMBeam set.
    #
    # The ID field of each FEMBeam property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMBeamFields objects
    # @see FEMBeamProps
    # @see getFEMBeam()
    def addFEMBeamList(self, fieldsList):
        row = self.tblFEMBeam.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AuxNode'] = fields.mAuxNode
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['EndRelease'] = fields.mEndRelease
            row['Epsin'] = fields.mEpsin
            row['Group'] = fields.mGroup
            row['IRigidEnd'] = fields.mIRigidEnd
            row['JRigidEnd'] = fields.mJRigidEnd
            row['MaterialID'] = fields.mMaterialID
            row['Node1'] = fields.mNode1
            row['Node2'] = fields.mNode2
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row['SectionID'] = fields.mSectionID
            row['Subdivision'] = fields.mSubdivision
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row.append()
        self.tblFEMBeam.flush()

    ## Gets the HDF5 table row of an FEMCurvMomentData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMCurvMomentDataProps
    def getFEMCurvMomentData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMCurvMomentData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMCurvMomentData object.
    #
    # The ID field of the given FEMCurvMomentData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMCurvMomentData object fields
    # @see getFEMCurvMomentData()
    def addFEMCurvMomentData(self, fields):
        row = self.tblFEMCurvMomentData.row
        row['id'] = fields.id
        row['Curvature'] = fields.mCurvature
        row['CurvMomentID'] = fields.mCurvMomentID
        row['Moment'] = fields.mMoment
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMCurvMomentData.flush()

    ## Adds a FEMCurvMomentData set.
    #
    # The ID field of each FEMCurvMomentData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMCurvMomentDataFields objects
    # @see FEMCurvMomentDataProps
    # @see getFEMCurvMomentData()
    def addFEMCurvMomentDataList(self, fieldsList):
        row = self.tblFEMCurvMomentData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Curvature'] = fields.mCurvature
            row['CurvMomentID'] = fields.mCurvMomentID
            row['Moment'] = fields.mMoment
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMCurvMomentData.flush()

    ## Gets the HDF5 table row of an FEMPropertysets object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMPropertysetsProps
    def getFEMPropertysets(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMPropertysets.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMPropertysets object.
    #
    # The ID field of the given FEMPropertysets properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMPropertysets object fields
    # @see getFEMPropertysets()
    def addFEMPropertysets(self, fields):
        row = self.tblFEMPropertysets.row
        row['id'] = fields.id
        row['C'] = fields.mC
        row['Description'] = fields.mDescription
        row['K'] = fields.mK
        row['M'] = fields.mM
        row['NC'] = fields.mNC
        row['NK'] = fields.mNK
        row['NM'] = fields.mNM
        row['Nonlinear'] = fields.mNonlinear
        row['PropertysetID'] = fields.mPropertysetID
        row['S'] = fields.mS
        row.append()
        self.tblFEMPropertysets.flush()

    ## Adds a FEMPropertysets set.
    #
    # The ID field of each FEMPropertysets property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMPropertysetsFields objects
    # @see FEMPropertysetsProps
    # @see getFEMPropertysets()
    def addFEMPropertysetsList(self, fieldsList):
        row = self.tblFEMPropertysets.row
        for fields in fieldsList:
            row[name] = fields.id
            row['C'] = fields.mC
            row['Description'] = fields.mDescription
            row['K'] = fields.mK
            row['M'] = fields.mM
            row['NC'] = fields.mNC
            row['NK'] = fields.mNK
            row['NM'] = fields.mNM
            row['Nonlinear'] = fields.mNonlinear
            row['PropertysetID'] = fields.mPropertysetID
            row['S'] = fields.mS
            row.append()
        self.tblFEMPropertysets.flush()

    ## Gets the HDF5 table row of an FEMOrthotropicMaterial object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMOrthotropicMaterialProps
    def getFEMOrthotropicMaterial(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMOrthotropicMaterial.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMOrthotropicMaterial object.
    #
    # The ID field of the given FEMOrthotropicMaterial properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMOrthotropicMaterial object fields
    # @see getFEMOrthotropicMaterial()
    def addFEMOrthotropicMaterial(self, fields):
        row = self.tblFEMOrthotropicMaterial.row
        row['id'] = fields.id
        row['Density'] = fields.mDensity
        row['EA'] = fields.mEA
        row['EB'] = fields.mEB
        row['EC'] = fields.mEC
        row['GAB'] = fields.mGAB
        row['GAC'] = fields.mGAC
        row['GBC'] = fields.mGBC
        row['Material'] = fields.mMaterial
        row['NUAB'] = fields.mNUAB
        row['NUAC'] = fields.mNUAC
        row['NUBC'] = fields.mNUBC
        row.append()
        self.tblFEMOrthotropicMaterial.flush()

    ## Adds a FEMOrthotropicMaterial set.
    #
    # The ID field of each FEMOrthotropicMaterial property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMOrthotropicMaterialFields objects
    # @see FEMOrthotropicMaterialProps
    # @see getFEMOrthotropicMaterial()
    def addFEMOrthotropicMaterialList(self, fieldsList):
        row = self.tblFEMOrthotropicMaterial.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Density'] = fields.mDensity
            row['EA'] = fields.mEA
            row['EB'] = fields.mEB
            row['EC'] = fields.mEC
            row['GAB'] = fields.mGAB
            row['GAC'] = fields.mGAC
            row['GBC'] = fields.mGBC
            row['Material'] = fields.mMaterial
            row['NUAB'] = fields.mNUAB
            row['NUAC'] = fields.mNUAC
            row['NUBC'] = fields.mNUBC
            row.append()
        self.tblFEMOrthotropicMaterial.flush()

    ## Gets the HDF5 table row of an FEMAppliedLoads object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMAppliedLoadsProps
    def getFEMAppliedLoads(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMAppliedLoads.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMAppliedLoads object.
    #
    # The ID field of the given FEMAppliedLoads properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMAppliedLoads object fields
    # @see getFEMAppliedLoads()
    def addFEMAppliedLoads(self, fields):
        row = self.tblFEMAppliedLoads.row
        row['id'] = fields.id
        row['AppliedLoadNmb'] = fields.mAppliedLoadNmb
        row['ArrivalTime'] = fields.mArrivalTime
        row['LoadID'] = fields.mLoadID
        row['LoadType'] = fields.mLoadType
        row['SiteType'] = fields.mSiteType
        row['TimeFunctionID'] = fields.mTimeFunctionID
        row.append()
        self.tblFEMAppliedLoads.flush()

    ## Adds a FEMAppliedLoads set.
    #
    # The ID field of each FEMAppliedLoads property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMAppliedLoadsFields objects
    # @see FEMAppliedLoadsProps
    # @see getFEMAppliedLoads()
    def addFEMAppliedLoadsList(self, fieldsList):
        row = self.tblFEMAppliedLoads.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AppliedLoadNmb'] = fields.mAppliedLoadNmb
            row['ArrivalTime'] = fields.mArrivalTime
            row['LoadID'] = fields.mLoadID
            row['LoadType'] = fields.mLoadType
            row['SiteType'] = fields.mSiteType
            row['TimeFunctionID'] = fields.mTimeFunctionID
            row.append()
        self.tblFEMAppliedLoads.flush()

    ## Gets the HDF5 table row of an FEMThermoOrthData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMThermoOrthDataProps
    def getFEMThermoOrthData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMThermoOrthData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMThermoOrthData object.
    #
    # The ID field of the given FEMThermoOrthData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMThermoOrthData object fields
    # @see getFEMThermoOrthData()
    def addFEMThermoOrthData(self, fields):
        row = self.tblFEMThermoOrthData.row
        row['id'] = fields.id
        row['ALPHAA'] = fields.mALPHAA
        row['ALPHAB'] = fields.mALPHAB
        row['ALPHAC'] = fields.mALPHAC
        row['EA'] = fields.mEA
        row['EB'] = fields.mEB
        row['EC'] = fields.mEC
        row['GAB'] = fields.mGAB
        row['GAC'] = fields.mGAC
        row['GBC'] = fields.mGBC
        row['MaterialID'] = fields.mMaterialID
        row['NUAB'] = fields.mNUAB
        row['NUAC'] = fields.mNUAC
        row['NUBC'] = fields.mNUBC
        row['RecordNmb'] = fields.mRecordNmb
        row['Theta'] = fields.mTheta
        row.append()
        self.tblFEMThermoOrthData.flush()

    ## Adds a FEMThermoOrthData set.
    #
    # The ID field of each FEMThermoOrthData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMThermoOrthDataFields objects
    # @see FEMThermoOrthDataProps
    # @see getFEMThermoOrthData()
    def addFEMThermoOrthDataList(self, fieldsList):
        row = self.tblFEMThermoOrthData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ALPHAA'] = fields.mALPHAA
            row['ALPHAB'] = fields.mALPHAB
            row['ALPHAC'] = fields.mALPHAC
            row['EA'] = fields.mEA
            row['EB'] = fields.mEB
            row['EC'] = fields.mEC
            row['GAB'] = fields.mGAB
            row['GAC'] = fields.mGAC
            row['GBC'] = fields.mGBC
            row['MaterialID'] = fields.mMaterialID
            row['NUAB'] = fields.mNUAB
            row['NUAC'] = fields.mNUAC
            row['NUBC'] = fields.mNUBC
            row['RecordNmb'] = fields.mRecordNmb
            row['Theta'] = fields.mTheta
            row.append()
        self.tblFEMThermoOrthData.flush()

    ## Gets the HDF5 table row of an FEMContactPairs object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMContactPairsProps
    def getFEMContactPairs(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMContactPairs.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMContactPairs object.
    #
    # The ID field of the given FEMContactPairs properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMContactPairs object fields
    # @see getFEMContactPairs()
    def addFEMContactPairs(self, fields):
        row = self.tblFEMContactPairs.row
        row['id'] = fields.id
        row['ContactorSurf'] = fields.mContactorSurf
        row['ContGroupID'] = fields.mContGroupID
        row['ContPair'] = fields.mContPair
        row['FContactor'] = fields.mFContactor
        row['Friction'] = fields.mFriction
        row['FTarget'] = fields.mFTarget
        row['HeatTransf'] = fields.mHeatTransf
        row['RecordNmb'] = fields.mRecordNmb
        row['TargetSurf'] = fields.mTargetSurf
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row.append()
        self.tblFEMContactPairs.flush()

    ## Adds a FEMContactPairs set.
    #
    # The ID field of each FEMContactPairs property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMContactPairsFields objects
    # @see FEMContactPairsProps
    # @see getFEMContactPairs()
    def addFEMContactPairsList(self, fieldsList):
        row = self.tblFEMContactPairs.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ContactorSurf'] = fields.mContactorSurf
            row['ContGroupID'] = fields.mContGroupID
            row['ContPair'] = fields.mContPair
            row['FContactor'] = fields.mFContactor
            row['Friction'] = fields.mFriction
            row['FTarget'] = fields.mFTarget
            row['HeatTransf'] = fields.mHeatTransf
            row['RecordNmb'] = fields.mRecordNmb
            row['TargetSurf'] = fields.mTargetSurf
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row.append()
        self.tblFEMContactPairs.flush()

    ## Gets the HDF5 table row of an FEMGeneral object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMGeneralProps
    def getFEMGeneral(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMGeneral.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMGeneral object.
    #
    # The ID field of the given FEMGeneral properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMGeneral object fields
    # @see getFEMGeneral()
    def addFEMGeneral(self, fields):
        row = self.tblFEMGeneral.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['Group'] = fields.mGroup
        row['MatrixSet'] = fields.mMatrixSet
        row['NodeAmount'] = fields.mNodeAmount
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row.append()
        self.tblFEMGeneral.flush()

    ## Adds a FEMGeneral set.
    #
    # The ID field of each FEMGeneral property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMGeneralFields objects
    # @see FEMGeneralProps
    # @see getFEMGeneral()
    def addFEMGeneralList(self, fieldsList):
        row = self.tblFEMGeneral.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['Group'] = fields.mGroup
            row['MatrixSet'] = fields.mMatrixSet
            row['NodeAmount'] = fields.mNodeAmount
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row.append()
        self.tblFEMGeneral.flush()

    ## Gets the HDF5 table row of an FEMBeamNode object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMBeamNodeProps
    def getFEMBeamNode(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMBeamNode.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMBeamNode object.
    #
    # The ID field of the given FEMBeamNode properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMBeamNode object fields
    # @see getFEMBeamNode()
    def addFEMBeamNode(self, fields):
        row = self.tblFEMBeamNode.row
        row['id'] = fields.id
        row['Displacement'] = fields.mDisplacement
        row['Group'] = fields.mGroup
        row['IniStrain'] = fields.mIniStrain
        row['MaterialID'] = fields.mMaterialID
        row['MC'] = fields.mMC
        row['MCrigidity'] = fields.mMCrigidity
        row['REmultiplyer'] = fields.mREmultiplyer
        row['Result'] = fields.mResult
        row['REtype'] = fields.mREtype
        row['RINT'] = fields.mRINT
        row['SectionID'] = fields.mSectionID
        row['SINT'] = fields.mSINT
        row['TINT'] = fields.mTINT
        row.append()
        self.tblFEMBeamNode.flush()

    ## Adds a FEMBeamNode set.
    #
    # The ID field of each FEMBeamNode property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMBeamNodeFields objects
    # @see FEMBeamNodeProps
    # @see getFEMBeamNode()
    def addFEMBeamNodeList(self, fieldsList):
        row = self.tblFEMBeamNode.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Displacement'] = fields.mDisplacement
            row['Group'] = fields.mGroup
            row['IniStrain'] = fields.mIniStrain
            row['MaterialID'] = fields.mMaterialID
            row['MC'] = fields.mMC
            row['MCrigidity'] = fields.mMCrigidity
            row['REmultiplyer'] = fields.mREmultiplyer
            row['Result'] = fields.mResult
            row['REtype'] = fields.mREtype
            row['RINT'] = fields.mRINT
            row['SectionID'] = fields.mSectionID
            row['SINT'] = fields.mSINT
            row['TINT'] = fields.mTINT
            row.append()
        self.tblFEMBeamNode.flush()

    ## Gets the HDF5 table row of an FEMSectionRect object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSectionRectProps
    def getFEMSectionRect(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSectionRect.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSectionRect object.
    #
    # The ID field of the given FEMSectionRect properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSectionRect object fields
    # @see getFEMSectionRect()
    def addFEMSectionRect(self, fields):
        row = self.tblFEMSectionRect.row
        row['id'] = fields.id
        row['Height'] = fields.mHeight
        row['IShear'] = fields.mIShear
        row['SC'] = fields.mSC
        row['SectionID'] = fields.mSectionID
        row['SSarea'] = fields.mSSarea
        row['TC'] = fields.mTC
        row['Torfac'] = fields.mTorfac
        row['TSarea'] = fields.mTSarea
        row['Width'] = fields.mWidth
        row.append()
        self.tblFEMSectionRect.flush()

    ## Adds a FEMSectionRect set.
    #
    # The ID field of each FEMSectionRect property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSectionRectFields objects
    # @see FEMSectionRectProps
    # @see getFEMSectionRect()
    def addFEMSectionRectList(self, fieldsList):
        row = self.tblFEMSectionRect.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Height'] = fields.mHeight
            row['IShear'] = fields.mIShear
            row['SC'] = fields.mSC
            row['SectionID'] = fields.mSectionID
            row['SSarea'] = fields.mSSarea
            row['TC'] = fields.mTC
            row['Torfac'] = fields.mTorfac
            row['TSarea'] = fields.mTSarea
            row['Width'] = fields.mWidth
            row.append()
        self.tblFEMSectionRect.flush()

    ## Gets the HDF5 table row of an FEMBeamLoad object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMBeamLoadProps
    def getFEMBeamLoad(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMBeamLoad.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMBeamLoad object.
    #
    # The ID field of the given FEMBeamLoad properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMBeamLoad object fields
    # @see getFEMBeamLoad()
    def addFEMBeamLoad(self, fields):
        row = self.tblFEMBeamLoad.row
        row['id'] = fields.id
        row['ArrivalTime'] = fields.mArrivalTime
        row['DeformDepend'] = fields.mDeformDepend
        row['DirectFilter'] = fields.mDirectFilter
        row['ElementID'] = fields.mElementID
        row['Face'] = fields.mFace
        row['Group'] = fields.mGroup
        row['P1'] = fields.mP1
        row['P2'] = fields.mP2
        row['RecordNmb'] = fields.mRecordNmb
        row['TimeFunctionID'] = fields.mTimeFunctionID
        row.append()
        self.tblFEMBeamLoad.flush()

    ## Adds a FEMBeamLoad set.
    #
    # The ID field of each FEMBeamLoad property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMBeamLoadFields objects
    # @see FEMBeamLoadProps
    # @see getFEMBeamLoad()
    def addFEMBeamLoadList(self, fieldsList):
        row = self.tblFEMBeamLoad.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ArrivalTime'] = fields.mArrivalTime
            row['DeformDepend'] = fields.mDeformDepend
            row['DirectFilter'] = fields.mDirectFilter
            row['ElementID'] = fields.mElementID
            row['Face'] = fields.mFace
            row['Group'] = fields.mGroup
            row['P1'] = fields.mP1
            row['P2'] = fields.mP2
            row['RecordNmb'] = fields.mRecordNmb
            row['TimeFunctionID'] = fields.mTimeFunctionID
            row.append()
        self.tblFEMBeamLoad.flush()

    ## Gets the HDF5 table row of an FEMLoadMassProportional object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMLoadMassProportionalProps
    def getFEMLoadMassProportional(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMLoadMassProportional.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMLoadMassProportional object.
    #
    # The ID field of the given FEMLoadMassProportional properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMLoadMassProportional object fields
    # @see getFEMLoadMassProportional()
    def addFEMLoadMassProportional(self, fields):
        row = self.tblFEMLoadMassProportional.row
        row['id'] = fields.id
        row['AX'] = fields.mAX
        row['AY'] = fields.mAY
        row['AZ'] = fields.mAZ
        row['LoadID'] = fields.mLoadID
        row['Magnitude'] = fields.mMagnitude
        row['Structure'] = fields.mStructure
        row.append()
        self.tblFEMLoadMassProportional.flush()

    ## Adds a FEMLoadMassProportional set.
    #
    # The ID field of each FEMLoadMassProportional property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMLoadMassProportionalFields objects
    # @see FEMLoadMassProportionalProps
    # @see getFEMLoadMassProportional()
    def addFEMLoadMassProportionalList(self, fieldsList):
        row = self.tblFEMLoadMassProportional.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AX'] = fields.mAX
            row['AY'] = fields.mAY
            row['AZ'] = fields.mAZ
            row['LoadID'] = fields.mLoadID
            row['Magnitude'] = fields.mMagnitude
            row['Structure'] = fields.mStructure
            row.append()
        self.tblFEMLoadMassProportional.flush()

    ## Gets the HDF5 table row of an FEMLink object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMLinkProps
    def getFEMLink(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMLink.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMLink object.
    #
    # The ID field of the given FEMLink properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMLink object fields
    # @see getFEMLink()
    def addFEMLink(self, fields):
        row = self.tblFEMLink.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['Displacement'] = fields.mDisplacement
        row['MasterNode'] = fields.mMasterNode
        row['RLID'] = fields.mRLID
        row['SlaveNode'] = fields.mSlaveNode
        row.append()
        self.tblFEMLink.flush()

    ## Adds a FEMLink set.
    #
    # The ID field of each FEMLink property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMLinkFields objects
    # @see FEMLinkProps
    # @see getFEMLink()
    def addFEMLinkList(self, fieldsList):
        row = self.tblFEMLink.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['Displacement'] = fields.mDisplacement
            row['MasterNode'] = fields.mMasterNode
            row['RLID'] = fields.mRLID
            row['SlaveNode'] = fields.mSlaveNode
            row.append()
        self.tblFEMLink.flush()

    ## Gets the HDF5 table row of an FEMAxesNode object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMAxesNodeProps
    def getFEMAxesNode(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMAxesNode.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMAxesNode object.
    #
    # The ID field of the given FEMAxesNode properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMAxesNode object fields
    # @see getFEMAxesNode()
    def addFEMAxesNode(self, fields):
        row = self.tblFEMAxesNode.row
        row['id'] = fields.id
        row['AxNodeID'] = fields.mAxNodeID
        row['Group'] = fields.mGroup
        row['Node1'] = fields.mNode1
        row['Node2'] = fields.mNode2
        row['Node3'] = fields.mNode3
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMAxesNode.flush()

    ## Adds a FEMAxesNode set.
    #
    # The ID field of each FEMAxesNode property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMAxesNodeFields objects
    # @see FEMAxesNodeProps
    # @see getFEMAxesNode()
    def addFEMAxesNodeList(self, fieldsList):
        row = self.tblFEMAxesNode.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AxNodeID'] = fields.mAxNodeID
            row['Group'] = fields.mGroup
            row['Node1'] = fields.mNode1
            row['Node2'] = fields.mNode2
            row['Node3'] = fields.mNode3
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMAxesNode.flush()

    ## Gets the HDF5 table row of an FEMNMTimeMass object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMNMTimeMassProps
    def getFEMNMTimeMass(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMNMTimeMass.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMNMTimeMass object.
    #
    # The ID field of the given FEMNMTimeMass properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMNMTimeMass object fields
    # @see getFEMNMTimeMass()
    def addFEMNMTimeMass(self, fields):
        row = self.tblFEMNMTimeMass.row
        row['id'] = fields.id
        row['Mass'] = fields.mMass
        row['PropertyID'] = fields.mPropertyID
        row['RecordNmb'] = fields.mRecordNmb
        row['TimeValue'] = fields.mTimeValue
        row.append()
        self.tblFEMNMTimeMass.flush()

    ## Adds a FEMNMTimeMass set.
    #
    # The ID field of each FEMNMTimeMass property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMNMTimeMassFields objects
    # @see FEMNMTimeMassProps
    # @see getFEMNMTimeMass()
    def addFEMNMTimeMassList(self, fieldsList):
        row = self.tblFEMNMTimeMass.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Mass'] = fields.mMass
            row['PropertyID'] = fields.mPropertyID
            row['RecordNmb'] = fields.mRecordNmb
            row['TimeValue'] = fields.mTimeValue
            row.append()
        self.tblFEMNMTimeMass.flush()

    ## Gets the HDF5 table row of an FEMAppliedDisplacement object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMAppliedDisplacementProps
    def getFEMAppliedDisplacement(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMAppliedDisplacement.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMAppliedDisplacement object.
    #
    # The ID field of the given FEMAppliedDisplacement properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMAppliedDisplacement object fields
    # @see getFEMAppliedDisplacement()
    def addFEMAppliedDisplacement(self, fields):
        row = self.tblFEMAppliedDisplacement.row
        row['id'] = fields.id
        row['ArrivalTime'] = fields.mArrivalTime
        row['Description'] = fields.mDescription
        row['Direction'] = fields.mDirection
        row['Factor'] = fields.mFactor
        row['Node'] = fields.mNode
        row['RecordNmb'] = fields.mRecordNmb
        row['TimeFunctionID'] = fields.mTimeFunctionID
        row.append()
        self.tblFEMAppliedDisplacement.flush()

    ## Adds a FEMAppliedDisplacement set.
    #
    # The ID field of each FEMAppliedDisplacement property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMAppliedDisplacementFields objects
    # @see FEMAppliedDisplacementProps
    # @see getFEMAppliedDisplacement()
    def addFEMAppliedDisplacementList(self, fieldsList):
        row = self.tblFEMAppliedDisplacement.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ArrivalTime'] = fields.mArrivalTime
            row['Description'] = fields.mDescription
            row['Direction'] = fields.mDirection
            row['Factor'] = fields.mFactor
            row['Node'] = fields.mNode
            row['RecordNmb'] = fields.mRecordNmb
            row['TimeFunctionID'] = fields.mTimeFunctionID
            row.append()
        self.tblFEMAppliedDisplacement.flush()

    ## Gets the HDF5 table row of an FEMTimeFunctions object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMTimeFunctionsProps
    def getFEMTimeFunctions(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMTimeFunctions.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMTimeFunctions object.
    #
    # The ID field of the given FEMTimeFunctions properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMTimeFunctions object fields
    # @see getFEMTimeFunctions()
    def addFEMTimeFunctions(self, fields):
        row = self.tblFEMTimeFunctions.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['TimeFunctionID'] = fields.mTimeFunctionID
        row.append()
        self.tblFEMTimeFunctions.flush()

    ## Adds a FEMTimeFunctions set.
    #
    # The ID field of each FEMTimeFunctions property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMTimeFunctionsFields objects
    # @see FEMTimeFunctionsProps
    # @see getFEMTimeFunctions()
    def addFEMTimeFunctionsList(self, fieldsList):
        row = self.tblFEMTimeFunctions.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['TimeFunctionID'] = fields.mTimeFunctionID
            row.append()
        self.tblFEMTimeFunctions.flush()

    ## Gets the HDF5 table row of an FEMForceStrainData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMForceStrainDataProps
    def getFEMForceStrainData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMForceStrainData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMForceStrainData object.
    #
    # The ID field of the given FEMForceStrainData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMForceStrainData object fields
    # @see getFEMForceStrainData()
    def addFEMForceStrainData(self, fields):
        row = self.tblFEMForceStrainData.row
        row['id'] = fields.id
        row['Force'] = fields.mForce
        row['ForceAxID'] = fields.mForceAxID
        row['RecordNmb'] = fields.mRecordNmb
        row['Strain'] = fields.mStrain
        row.append()
        self.tblFEMForceStrainData.flush()

    ## Adds a FEMForceStrainData set.
    #
    # The ID field of each FEMForceStrainData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMForceStrainDataFields objects
    # @see FEMForceStrainDataProps
    # @see getFEMForceStrainData()
    def addFEMForceStrainDataList(self, fieldsList):
        row = self.tblFEMForceStrainData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Force'] = fields.mForce
            row['ForceAxID'] = fields.mForceAxID
            row['RecordNmb'] = fields.mRecordNmb
            row['Strain'] = fields.mStrain
            row.append()
        self.tblFEMForceStrainData.flush()

    ## Gets the HDF5 table row of an FEMSkewDOF object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSkewDOFProps
    def getFEMSkewDOF(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSkewDOF.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSkewDOF object.
    #
    # The ID field of the given FEMSkewDOF properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSkewDOF object fields
    # @see getFEMSkewDOF()
    def addFEMSkewDOF(self, fields):
        row = self.tblFEMSkewDOF.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['Node'] = fields.mNode
        row['SkewSystemID'] = fields.mSkewSystemID
        row.append()
        self.tblFEMSkewDOF.flush()

    ## Adds a FEMSkewDOF set.
    #
    # The ID field of each FEMSkewDOF property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSkewDOFFields objects
    # @see FEMSkewDOFProps
    # @see getFEMSkewDOF()
    def addFEMSkewDOFList(self, fieldsList):
        row = self.tblFEMSkewDOF.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['Node'] = fields.mNode
            row['SkewSystemID'] = fields.mSkewSystemID
            row.append()
        self.tblFEMSkewDOF.flush()

    ## Gets the HDF5 table row of an FEMSectionI object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSectionIProps
    def getFEMSectionI(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSectionI.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSectionI object.
    #
    # The ID field of the given FEMSectionI properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSectionI object fields
    # @see getFEMSectionI()
    def addFEMSectionI(self, fields):
        row = self.tblFEMSectionI.row
        row['id'] = fields.id
        row['Height'] = fields.mHeight
        row['SC'] = fields.mSC
        row['SectionID'] = fields.mSectionID
        row['SSarea'] = fields.mSSarea
        row['TC'] = fields.mTC
        row['Thick1'] = fields.mThick1
        row['Thick2'] = fields.mThick2
        row['Thick3'] = fields.mThick3
        row['Torfac'] = fields.mTorfac
        row['TSarea'] = fields.mTSarea
        row['Width1'] = fields.mWidth1
        row['Width2'] = fields.mWidth2
        row.append()
        self.tblFEMSectionI.flush()

    ## Adds a FEMSectionI set.
    #
    # The ID field of each FEMSectionI property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSectionIFields objects
    # @see FEMSectionIProps
    # @see getFEMSectionI()
    def addFEMSectionIList(self, fieldsList):
        row = self.tblFEMSectionI.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Height'] = fields.mHeight
            row['SC'] = fields.mSC
            row['SectionID'] = fields.mSectionID
            row['SSarea'] = fields.mSSarea
            row['TC'] = fields.mTC
            row['Thick1'] = fields.mThick1
            row['Thick2'] = fields.mThick2
            row['Thick3'] = fields.mThick3
            row['Torfac'] = fields.mTorfac
            row['TSarea'] = fields.mTSarea
            row['Width1'] = fields.mWidth1
            row['Width2'] = fields.mWidth2
            row.append()
        self.tblFEMSectionI.flush()

    ## Gets the HDF5 table row of an FEMPlasticBilinearMaterial object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMPlasticBilinearMaterialProps
    def getFEMPlasticBilinearMaterial(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMPlasticBilinearMaterial.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMPlasticBilinearMaterial object.
    #
    # The ID field of the given FEMPlasticBilinearMaterial properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMPlasticBilinearMaterial object fields
    # @see getFEMPlasticBilinearMaterial()
    def addFEMPlasticBilinearMaterial(self, fields):
        row = self.tblFEMPlasticBilinearMaterial.row
        row['id'] = fields.id
        row['Alpha'] = fields.mAlpha
        row['Density'] = fields.mDensity
        row['E'] = fields.mE
        row['EPA'] = fields.mEPA
        row['ET'] = fields.mET
        row['Hardening'] = fields.mHardening
        row['Material'] = fields.mMaterial
        row['NU'] = fields.mNU
        row['TRef'] = fields.mTRef
        row['Yield'] = fields.mYield
        row.append()
        self.tblFEMPlasticBilinearMaterial.flush()

    ## Adds a FEMPlasticBilinearMaterial set.
    #
    # The ID field of each FEMPlasticBilinearMaterial property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMPlasticBilinearMaterialFields objects
    # @see FEMPlasticBilinearMaterialProps
    # @see getFEMPlasticBilinearMaterial()
    def addFEMPlasticBilinearMaterialList(self, fieldsList):
        row = self.tblFEMPlasticBilinearMaterial.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Alpha'] = fields.mAlpha
            row['Density'] = fields.mDensity
            row['E'] = fields.mE
            row['EPA'] = fields.mEPA
            row['ET'] = fields.mET
            row['Hardening'] = fields.mHardening
            row['Material'] = fields.mMaterial
            row['NU'] = fields.mNU
            row['TRef'] = fields.mTRef
            row['Yield'] = fields.mYield
            row.append()
        self.tblFEMPlasticBilinearMaterial.flush()

    ## Gets the HDF5 table row of an FEMMTForceData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMMTForceDataProps
    def getFEMMTForceData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMMTForceData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMMTForceData object.
    #
    # The ID field of the given FEMMTForceData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMMTForceData object fields
    # @see getFEMMTForceData()
    def addFEMMTForceData(self, fields):
        row = self.tblFEMMTForceData.row
        row['id'] = fields.id
        row['Force'] = fields.mForce
        row['MomentRID'] = fields.mMomentRID
        row['RecordNmb'] = fields.mRecordNmb
        row['TwistMomentID'] = fields.mTwistMomentID
        row.append()
        self.tblFEMMTForceData.flush()

    ## Adds a FEMMTForceData set.
    #
    # The ID field of each FEMMTForceData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMMTForceDataFields objects
    # @see FEMMTForceDataProps
    # @see getFEMMTForceData()
    def addFEMMTForceDataList(self, fieldsList):
        row = self.tblFEMMTForceData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Force'] = fields.mForce
            row['MomentRID'] = fields.mMomentRID
            row['RecordNmb'] = fields.mRecordNmb
            row['TwistMomentID'] = fields.mTwistMomentID
            row.append()
        self.tblFEMMTForceData.flush()

    ## Gets the HDF5 table row of an FEMShellPressure object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMShellPressureProps
    def getFEMShellPressure(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMShellPressure.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMShellPressure object.
    #
    # The ID field of the given FEMShellPressure properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMShellPressure object fields
    # @see getFEMShellPressure()
    def addFEMShellPressure(self, fields):
        row = self.tblFEMShellPressure.row
        row['id'] = fields.id
        row['ArrivalTime'] = fields.mArrivalTime
        row['DeformDepend'] = fields.mDeformDepend
        row['Description'] = fields.mDescription
        row['DirectFilter'] = fields.mDirectFilter
        row['ElementID'] = fields.mElementID
        row['Face'] = fields.mFace
        row['Group'] = fields.mGroup
        row['Nodaux'] = fields.mNodaux
        row['P1'] = fields.mP1
        row['P2'] = fields.mP2
        row['P3'] = fields.mP3
        row['P4'] = fields.mP4
        row['RecordNmb'] = fields.mRecordNmb
        row['TimeFunctionID'] = fields.mTimeFunctionID
        row.append()
        self.tblFEMShellPressure.flush()

    ## Adds a FEMShellPressure set.
    #
    # The ID field of each FEMShellPressure property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMShellPressureFields objects
    # @see FEMShellPressureProps
    # @see getFEMShellPressure()
    def addFEMShellPressureList(self, fieldsList):
        row = self.tblFEMShellPressure.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ArrivalTime'] = fields.mArrivalTime
            row['DeformDepend'] = fields.mDeformDepend
            row['Description'] = fields.mDescription
            row['DirectFilter'] = fields.mDirectFilter
            row['ElementID'] = fields.mElementID
            row['Face'] = fields.mFace
            row['Group'] = fields.mGroup
            row['Nodaux'] = fields.mNodaux
            row['P1'] = fields.mP1
            row['P2'] = fields.mP2
            row['P3'] = fields.mP3
            row['P4'] = fields.mP4
            row['RecordNmb'] = fields.mRecordNmb
            row['TimeFunctionID'] = fields.mTimeFunctionID
            row.append()
        self.tblFEMShellPressure.flush()

    ## Gets the HDF5 table row of an FEMMatrices object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMMatricesProps
    def getFEMMatrices(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMMatrices.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMMatrices object.
    #
    # The ID field of the given FEMMatrices properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMMatrices object fields
    # @see getFEMMatrices()
    def addFEMMatrices(self, fields):
        row = self.tblFEMMatrices.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['Lump'] = fields.mLump
        row['MatrixID'] = fields.mMatrixID
        row['MatrixType'] = fields.mMatrixType
        row['ND'] = fields.mND
        row['NS'] = fields.mNS
        row.append()
        self.tblFEMMatrices.flush()

    ## Adds a FEMMatrices set.
    #
    # The ID field of each FEMMatrices property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMMatricesFields objects
    # @see FEMMatricesProps
    # @see getFEMMatrices()
    def addFEMMatricesList(self, fieldsList):
        row = self.tblFEMMatrices.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['Lump'] = fields.mLump
            row['MatrixID'] = fields.mMatrixID
            row['MatrixType'] = fields.mMatrixType
            row['ND'] = fields.mND
            row['NS'] = fields.mNS
            row.append()
        self.tblFEMMatrices.flush()

    ## Gets the HDF5 table row of an FEMDamping object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMDampingProps
    def getFEMDamping(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMDamping.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMDamping object.
    #
    # The ID field of the given FEMDamping properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMDamping object fields
    # @see getFEMDamping()
    def addFEMDamping(self, fields):
        row = self.tblFEMDamping.row
        row['id'] = fields.id
        row['ALPHA'] = fields.mALPHA
        row['BETA'] = fields.mBETA
        row['Group'] = fields.mGroup
        row.append()
        self.tblFEMDamping.flush()

    ## Adds a FEMDamping set.
    #
    # The ID field of each FEMDamping property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMDampingFields objects
    # @see FEMDampingProps
    # @see getFEMDamping()
    def addFEMDampingList(self, fieldsList):
        row = self.tblFEMDamping.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ALPHA'] = fields.mALPHA
            row['BETA'] = fields.mBETA
            row['Group'] = fields.mGroup
            row.append()
        self.tblFEMDamping.flush()

    ## Gets the HDF5 table row of an FEMMaterial object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMMaterialProps
    def getFEMMaterial(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMMaterial.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMMaterial object.
    #
    # The ID field of the given FEMMaterial properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMMaterial object fields
    # @see getFEMMaterial()
    def addFEMMaterial(self, fields):
        row = self.tblFEMMaterial.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['MaterialType'] = self.MaterialTypeEnumList.index(str(fields.mMaterialType))
        row['LocalID'] = fields.mLocalID
        row['Structure'] = fields.mStructure
        row.append()
        self.tblFEMMaterial.flush()

    ## Adds a FEMMaterial set.
    #
    # The ID field of each FEMMaterial property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMMaterialFields objects
    # @see FEMMaterialProps
    # @see getFEMMaterial()
    def addFEMMaterialList(self, fieldsList):
        row = self.tblFEMMaterial.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['MaterialType'] = fields.mMaterialType
            row['LocalID'] = fields.mLocalID
            row['Structure'] = fields.mStructure
            row.append()
        self.tblFEMMaterial.flush()

    ## Gets the HDF5 table row of an FEMMatrixData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMMatrixDataProps
    def getFEMMatrixData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMMatrixData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMMatrixData object.
    #
    # The ID field of the given FEMMatrixData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMMatrixData object fields
    # @see getFEMMatrixData()
    def addFEMMatrixData(self, fields):
        row = self.tblFEMMatrixData.row
        row['id'] = fields.id
        row['Coeff'] = fields.mCoeff
        row['ColumnIndex'] = fields.mColumnIndex
        row['MatrixID'] = fields.mMatrixID
        row['RecordNmb'] = fields.mRecordNmb
        row['RowIndex'] = fields.mRowIndex
        row.append()
        self.tblFEMMatrixData.flush()

    ## Adds a FEMMatrixData set.
    #
    # The ID field of each FEMMatrixData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMMatrixDataFields objects
    # @see FEMMatrixDataProps
    # @see getFEMMatrixData()
    def addFEMMatrixDataList(self, fieldsList):
        row = self.tblFEMMatrixData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Coeff'] = fields.mCoeff
            row['ColumnIndex'] = fields.mColumnIndex
            row['MatrixID'] = fields.mMatrixID
            row['RecordNmb'] = fields.mRecordNmb
            row['RowIndex'] = fields.mRowIndex
            row.append()
        self.tblFEMMatrixData.flush()

    ## Gets the HDF5 table row of an FEMShellAxesOrtho object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMShellAxesOrthoProps
    def getFEMShellAxesOrtho(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMShellAxesOrtho.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMShellAxesOrtho object.
    #
    # The ID field of the given FEMShellAxesOrtho properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMShellAxesOrtho object fields
    # @see getFEMShellAxesOrtho()
    def addFEMShellAxesOrtho(self, fields):
        row = self.tblFEMShellAxesOrtho.row
        row['id'] = fields.id
        row['ALFA'] = fields.mALFA
        row['AxOrthoID'] = fields.mAxOrthoID
        row['Group'] = fields.mGroup
        row['LineID'] = fields.mLineID
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMShellAxesOrtho.flush()

    ## Adds a FEMShellAxesOrtho set.
    #
    # The ID field of each FEMShellAxesOrtho property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMShellAxesOrthoFields objects
    # @see FEMShellAxesOrthoProps
    # @see getFEMShellAxesOrtho()
    def addFEMShellAxesOrthoList(self, fieldsList):
        row = self.tblFEMShellAxesOrtho.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ALFA'] = fields.mALFA
            row['AxOrthoID'] = fields.mAxOrthoID
            row['Group'] = fields.mGroup
            row['LineID'] = fields.mLineID
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMShellAxesOrtho.flush()

    ## Gets the HDF5 table row of an FEMEndreleases object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMEndreleasesProps
    def getFEMEndreleases(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMEndreleases.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMEndreleases object.
    #
    # The ID field of the given FEMEndreleases properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMEndreleases object fields
    # @see getFEMEndreleases()
    def addFEMEndreleases(self, fields):
        row = self.tblFEMEndreleases.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['EndRelease'] = fields.mEndRelease
        row['Moment1'] = fields.mMoment1
        row['Moment2'] = fields.mMoment2
        row['Moment3'] = fields.mMoment3
        row['Moment4'] = fields.mMoment4
        row['Moment5'] = fields.mMoment5
        row['Moment6'] = fields.mMoment6
        row.append()
        self.tblFEMEndreleases.flush()

    ## Adds a FEMEndreleases set.
    #
    # The ID field of each FEMEndreleases property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMEndreleasesFields objects
    # @see FEMEndreleasesProps
    # @see getFEMEndreleases()
    def addFEMEndreleasesList(self, fieldsList):
        row = self.tblFEMEndreleases.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['EndRelease'] = fields.mEndRelease
            row['Moment1'] = fields.mMoment1
            row['Moment2'] = fields.mMoment2
            row['Moment3'] = fields.mMoment3
            row['Moment4'] = fields.mMoment4
            row['Moment5'] = fields.mMoment5
            row['Moment6'] = fields.mMoment6
            row.append()
        self.tblFEMEndreleases.flush()

    ## Gets the HDF5 table row of an FEMTrussGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMTrussGroupProps
    def getFEMTrussGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMTrussGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMTrussGroup object.
    #
    # The ID field of the given FEMTrussGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMTrussGroup object fields
    # @see getFEMTrussGroup()
    def addFEMTrussGroup(self, fields):
        row = self.tblFEMTrussGroup.row
        row['id'] = fields.id
        row['Displacement'] = fields.mDisplacement
        row['GAPS'] = fields.mGAPS
        row['Group'] = fields.mGroup
        row['IniStrain'] = fields.mIniStrain
        row['Material'] = fields.mMaterial
        row['SectionArea'] = fields.mSectionArea
        row.append()
        self.tblFEMTrussGroup.flush()

    ## Adds a FEMTrussGroup set.
    #
    # The ID field of each FEMTrussGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMTrussGroupFields objects
    # @see FEMTrussGroupProps
    # @see getFEMTrussGroup()
    def addFEMTrussGroupList(self, fieldsList):
        row = self.tblFEMTrussGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Displacement'] = fields.mDisplacement
            row['GAPS'] = fields.mGAPS
            row['Group'] = fields.mGroup
            row['IniStrain'] = fields.mIniStrain
            row['Material'] = fields.mMaterial
            row['SectionArea'] = fields.mSectionArea
            row.append()
        self.tblFEMTrussGroup.flush()

    ## Gets the HDF5 table row of an FEMInitialTemperature object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMInitialTemperatureProps
    def getFEMInitialTemperature(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMInitialTemperature.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMInitialTemperature object.
    #
    # The ID field of the given FEMInitialTemperature properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMInitialTemperature object fields
    # @see getFEMInitialTemperature()
    def addFEMInitialTemperature(self, fields):
        row = self.tblFEMInitialTemperature.row
        row['id'] = fields.id
        row['Node'] = fields.mNode
        row['Temperature'] = fields.mTemperature
        row.append()
        self.tblFEMInitialTemperature.flush()

    ## Adds a FEMInitialTemperature set.
    #
    # The ID field of each FEMInitialTemperature property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMInitialTemperatureFields objects
    # @see FEMInitialTemperatureProps
    # @see getFEMInitialTemperature()
    def addFEMInitialTemperatureList(self, fieldsList):
        row = self.tblFEMInitialTemperature.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Node'] = fields.mNode
            row['Temperature'] = fields.mTemperature
            row.append()
        self.tblFEMInitialTemperature.flush()

    ## Gets the HDF5 table row of an FEMThermoIsoMaterials object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMThermoIsoMaterialsProps
    def getFEMThermoIsoMaterials(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMThermoIsoMaterials.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMThermoIsoMaterials object.
    #
    # The ID field of the given FEMThermoIsoMaterials properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMThermoIsoMaterials object fields
    # @see getFEMThermoIsoMaterials()
    def addFEMThermoIsoMaterials(self, fields):
        row = self.tblFEMThermoIsoMaterials.row
        row['id'] = fields.id
        row['Density'] = fields.mDensity
        row['MaterialID'] = fields.mMaterialID
        row['TREF'] = fields.mTREF
        row.append()
        self.tblFEMThermoIsoMaterials.flush()

    ## Adds a FEMThermoIsoMaterials set.
    #
    # The ID field of each FEMThermoIsoMaterials property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMThermoIsoMaterialsFields objects
    # @see FEMThermoIsoMaterialsProps
    # @see getFEMThermoIsoMaterials()
    def addFEMThermoIsoMaterialsList(self, fieldsList):
        row = self.tblFEMThermoIsoMaterials.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Density'] = fields.mDensity
            row['MaterialID'] = fields.mMaterialID
            row['TREF'] = fields.mTREF
            row.append()
        self.tblFEMThermoIsoMaterials.flush()

    ## Gets the HDF5 table row of an FEMThermoIsoData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMThermoIsoDataProps
    def getFEMThermoIsoData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMThermoIsoData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMThermoIsoData object.
    #
    # The ID field of the given FEMThermoIsoData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMThermoIsoData object fields
    # @see getFEMThermoIsoData()
    def addFEMThermoIsoData(self, fields):
        row = self.tblFEMThermoIsoData.row
        row['id'] = fields.id
        row['ALPHA'] = fields.mALPHA
        row['E'] = fields.mE
        row['MaterialID'] = fields.mMaterialID
        row['NU'] = fields.mNU
        row['RecordNmb'] = fields.mRecordNmb
        row['Theta'] = fields.mTheta
        row.append()
        self.tblFEMThermoIsoData.flush()

    ## Adds a FEMThermoIsoData set.
    #
    # The ID field of each FEMThermoIsoData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMThermoIsoDataFields objects
    # @see FEMThermoIsoDataProps
    # @see getFEMThermoIsoData()
    def addFEMThermoIsoDataList(self, fieldsList):
        row = self.tblFEMThermoIsoData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ALPHA'] = fields.mALPHA
            row['E'] = fields.mE
            row['MaterialID'] = fields.mMaterialID
            row['NU'] = fields.mNU
            row['RecordNmb'] = fields.mRecordNmb
            row['Theta'] = fields.mTheta
            row.append()
        self.tblFEMThermoIsoData.flush()

    ## Gets the HDF5 table row of an FEMContactGroup3 object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMContactGroup3Props
    def getFEMContactGroup3(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMContactGroup3.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMContactGroup3 object.
    #
    # The ID field of the given FEMContactGroup3 properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMContactGroup3 object fields
    # @see getFEMContactGroup3()
    def addFEMContactGroup3(self, fields):
        row = self.tblFEMContactGroup3.row
        row['id'] = fields.id
        row['ContGroupID'] = fields.mContGroupID
        row['Depth'] = fields.mDepth
        row['Description'] = fields.mDescription
        row['Forces'] = fields.mForces
        row['Friction'] = fields.mFriction
        row['IniPenetration'] = fields.mIniPenetration
        row['NodeToNode'] = fields.mNodeToNode
        row['Offset'] = fields.mOffset
        row['PenetrAlgorithm'] = fields.mPenetrAlgorithm
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row['Tied'] = fields.mTied
        row['TiedOffset'] = fields.mTiedOffset
        row['Tolerance'] = fields.mTolerance
        row['Tractions'] = fields.mTractions
        row.append()
        self.tblFEMContactGroup3.flush()

    ## Adds a FEMContactGroup3 set.
    #
    # The ID field of each FEMContactGroup3 property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMContactGroup3Fields objects
    # @see FEMContactGroup3Props
    # @see getFEMContactGroup3()
    def addFEMContactGroup3List(self, fieldsList):
        row = self.tblFEMContactGroup3.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ContGroupID'] = fields.mContGroupID
            row['Depth'] = fields.mDepth
            row['Description'] = fields.mDescription
            row['Forces'] = fields.mForces
            row['Friction'] = fields.mFriction
            row['IniPenetration'] = fields.mIniPenetration
            row['NodeToNode'] = fields.mNodeToNode
            row['Offset'] = fields.mOffset
            row['PenetrAlgorithm'] = fields.mPenetrAlgorithm
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row['Tied'] = fields.mTied
            row['TiedOffset'] = fields.mTiedOffset
            row['Tolerance'] = fields.mTolerance
            row['Tractions'] = fields.mTractions
            row.append()
        self.tblFEMContactGroup3.flush()

    ## Gets the HDF5 table row of an FEMNLElasticMaterials object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMNLElasticMaterialsProps
    def getFEMNLElasticMaterials(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMNLElasticMaterials.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMNLElasticMaterials object.
    #
    # The ID field of the given FEMNLElasticMaterials properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMNLElasticMaterials object fields
    # @see getFEMNLElasticMaterials()
    def addFEMNLElasticMaterials(self, fields):
        row = self.tblFEMNLElasticMaterials.row
        row['id'] = fields.id
        row['Dcurve'] = fields.mDcurve
        row['Density'] = fields.mDensity
        row['MaterialID'] = fields.mMaterialID
        row.append()
        self.tblFEMNLElasticMaterials.flush()

    ## Adds a FEMNLElasticMaterials set.
    #
    # The ID field of each FEMNLElasticMaterials property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMNLElasticMaterialsFields objects
    # @see FEMNLElasticMaterialsProps
    # @see getFEMNLElasticMaterials()
    def addFEMNLElasticMaterialsList(self, fieldsList):
        row = self.tblFEMNLElasticMaterials.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Dcurve'] = fields.mDcurve
            row['Density'] = fields.mDensity
            row['MaterialID'] = fields.mMaterialID
            row.append()
        self.tblFEMNLElasticMaterials.flush()

    ## Gets the HDF5 table row of an FEMPlate object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMPlateProps
    def getFEMPlate(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMPlate.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMPlate object.
    #
    # The ID field of the given FEMPlate properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMPlate object fields
    # @see getFEMPlate()
    def addFEMPlate(self, fields):
        row = self.tblFEMPlate.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['Flex11'] = fields.mFlex11
        row['Flex12'] = fields.mFlex12
        row['Flex22'] = fields.mFlex22
        row['Group'] = fields.mGroup
        row['MaterialID'] = fields.mMaterialID
        row['Meps11'] = fields.mMeps11
        row['Meps12'] = fields.mMeps12
        row['Meps22'] = fields.mMeps22
        row['N1'] = fields.mN1
        row['N2'] = fields.mN2
        row['N3'] = fields.mN3
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row['Thick'] = fields.mThick
        row.append()
        self.tblFEMPlate.flush()

    ## Adds a FEMPlate set.
    #
    # The ID field of each FEMPlate property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMPlateFields objects
    # @see FEMPlateProps
    # @see getFEMPlate()
    def addFEMPlateList(self, fieldsList):
        row = self.tblFEMPlate.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['Flex11'] = fields.mFlex11
            row['Flex12'] = fields.mFlex12
            row['Flex22'] = fields.mFlex22
            row['Group'] = fields.mGroup
            row['MaterialID'] = fields.mMaterialID
            row['Meps11'] = fields.mMeps11
            row['Meps12'] = fields.mMeps12
            row['Meps22'] = fields.mMeps22
            row['N1'] = fields.mN1
            row['N2'] = fields.mN2
            row['N3'] = fields.mN3
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row['Thick'] = fields.mThick
            row.append()
        self.tblFEMPlate.flush()

    ## Gets the HDF5 table row of an FEMIsoBeam object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMIsoBeamProps
    def getFEMIsoBeam(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMIsoBeam.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMIsoBeam object.
    #
    # The ID field of the given FEMIsoBeam properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMIsoBeam object fields
    # @see getFEMIsoBeam()
    def addFEMIsoBeam(self, fields):
        row = self.tblFEMIsoBeam.row
        row['id'] = fields.id
        row['AUX'] = fields.mAUX
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['Epaxl'] = fields.mEpaxl
        row['Ephoop'] = fields.mEphoop
        row['Group'] = fields.mGroup
        row['MaterialID'] = fields.mMaterialID
        row['N1'] = fields.mN1
        row['N2'] = fields.mN2
        row['N3'] = fields.mN3
        row['N4'] = fields.mN4
        row['NodeAmount'] = fields.mNodeAmount
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row['SectionID'] = fields.mSectionID
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row.append()
        self.tblFEMIsoBeam.flush()

    ## Adds a FEMIsoBeam set.
    #
    # The ID field of each FEMIsoBeam property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMIsoBeamFields objects
    # @see FEMIsoBeamProps
    # @see getFEMIsoBeam()
    def addFEMIsoBeamList(self, fieldsList):
        row = self.tblFEMIsoBeam.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AUX'] = fields.mAUX
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['Epaxl'] = fields.mEpaxl
            row['Ephoop'] = fields.mEphoop
            row['Group'] = fields.mGroup
            row['MaterialID'] = fields.mMaterialID
            row['N1'] = fields.mN1
            row['N2'] = fields.mN2
            row['N3'] = fields.mN3
            row['N4'] = fields.mN4
            row['NodeAmount'] = fields.mNodeAmount
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row['SectionID'] = fields.mSectionID
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row.append()
        self.tblFEMIsoBeam.flush()

    ## Gets the HDF5 table row of an FEMAppliedConcentratedLoad object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMAppliedConcentratedLoadProps
    def getFEMAppliedConcentratedLoad(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMAppliedConcentratedLoad.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMAppliedConcentratedLoad object.
    #
    # The ID field of the given FEMAppliedConcentratedLoad properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMAppliedConcentratedLoad object fields
    # @see getFEMAppliedConcentratedLoad()
    def addFEMAppliedConcentratedLoad(self, fields):
        row = self.tblFEMAppliedConcentratedLoad.row
        row['id'] = fields.id
        row['ArrivalTime'] = fields.mArrivalTime
        row['Description'] = fields.mDescription
        row['Direction'] = fields.mDirection
        row['Factor'] = fields.mFactor
        row['NodeAux'] = fields.mNodeAux
        row['NodeID'] = fields.mNodeID
        row['RecordNmb'] = fields.mRecordNmb
        row['TimeFunctionID'] = fields.mTimeFunctionID
        row.append()
        self.tblFEMAppliedConcentratedLoad.flush()

    ## Adds a FEMAppliedConcentratedLoad set.
    #
    # The ID field of each FEMAppliedConcentratedLoad property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMAppliedConcentratedLoadFields objects
    # @see FEMAppliedConcentratedLoadProps
    # @see getFEMAppliedConcentratedLoad()
    def addFEMAppliedConcentratedLoadList(self, fieldsList):
        row = self.tblFEMAppliedConcentratedLoad.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ArrivalTime'] = fields.mArrivalTime
            row['Description'] = fields.mDescription
            row['Direction'] = fields.mDirection
            row['Factor'] = fields.mFactor
            row['NodeAux'] = fields.mNodeAux
            row['NodeID'] = fields.mNodeID
            row['RecordNmb'] = fields.mRecordNmb
            row['TimeFunctionID'] = fields.mTimeFunctionID
            row.append()
        self.tblFEMAppliedConcentratedLoad.flush()

    ## Gets the HDF5 table row of an FEMTwoDSolidGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMTwoDSolidGroupProps
    def getFEMTwoDSolidGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMTwoDSolidGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMTwoDSolidGroup object.
    #
    # The ID field of the given FEMTwoDSolidGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMTwoDSolidGroup object fields
    # @see getFEMTwoDSolidGroup()
    def addFEMTwoDSolidGroup(self, fields):
        row = self.tblFEMTwoDSolidGroup.row
        row['id'] = fields.id
        row['AuxNode'] = fields.mAuxNode
        row['Displacement'] = fields.mDisplacement
        row['Group'] = fields.mGroup
        row['MaterialID'] = fields.mMaterialID
        row['Result'] = fields.mResult
        row['Subtype'] = fields.mSubtype
        row.append()
        self.tblFEMTwoDSolidGroup.flush()

    ## Adds a FEMTwoDSolidGroup set.
    #
    # The ID field of each FEMTwoDSolidGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMTwoDSolidGroupFields objects
    # @see FEMTwoDSolidGroupProps
    # @see getFEMTwoDSolidGroup()
    def addFEMTwoDSolidGroupList(self, fieldsList):
        row = self.tblFEMTwoDSolidGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AuxNode'] = fields.mAuxNode
            row['Displacement'] = fields.mDisplacement
            row['Group'] = fields.mGroup
            row['MaterialID'] = fields.mMaterialID
            row['Result'] = fields.mResult
            row['Subtype'] = fields.mSubtype
            row.append()
        self.tblFEMTwoDSolidGroup.flush()

    ## Gets the HDF5 table row of an FEMGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMGroupProps
    def getFEMGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMGroup object.
    #
    # The ID field of the given FEMGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMGroup object fields
    # @see getFEMGroup()
    def addFEMGroup(self, fields):
        row = self.tblFEMGroup.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['GroupType'] = self.GroupTypeEnumList.index(str(fields.mGroupType))
        row['LocalID'] = fields.mLocalID
        row['Structure'] = fields.mStructure
        row.append()
        self.tblFEMGroup.flush()

    ## Adds a FEMGroup set.
    #
    # The ID field of each FEMGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMGroupFields objects
    # @see FEMGroupProps
    # @see getFEMGroup()
    def addFEMGroupList(self, fieldsList):
        row = self.tblFEMGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['GroupType'] = fields.mGroupType
            row['LocalID'] = fields.mLocalID
            row['Structure'] = fields.mStructure
            row.append()
        self.tblFEMGroup.flush()

    ## Gets the HDF5 table row of an FEMProperties object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMPropertiesProps
    def getFEMProperties(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMProperties.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMProperties object.
    #
    # The ID field of the given FEMProperties properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMProperties object fields
    # @see getFEMProperties()
    def addFEMProperties(self, fields):
        row = self.tblFEMProperties.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['PropertyID'] = fields.mPropertyID
        row['PropertyType'] = fields.mPropertyType
        row['Rupture'] = fields.mRupture
        row['XC'] = fields.mXC
        row['XN'] = fields.mXN
        row.append()
        self.tblFEMProperties.flush()

    ## Adds a FEMProperties set.
    #
    # The ID field of each FEMProperties property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMPropertiesFields objects
    # @see FEMPropertiesProps
    # @see getFEMProperties()
    def addFEMPropertiesList(self, fieldsList):
        row = self.tblFEMProperties.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['PropertyID'] = fields.mPropertyID
            row['PropertyType'] = fields.mPropertyType
            row['Rupture'] = fields.mRupture
            row['XC'] = fields.mXC
            row['XN'] = fields.mXN
            row.append()
        self.tblFEMProperties.flush()

    ## Gets the HDF5 table row of an FEMThreeDSolidGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMThreeDSolidGroupProps
    def getFEMThreeDSolidGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMThreeDSolidGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMThreeDSolidGroup object.
    #
    # The ID field of the given FEMThreeDSolidGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMThreeDSolidGroup object fields
    # @see getFEMThreeDSolidGroup()
    def addFEMThreeDSolidGroup(self, fields):
        row = self.tblFEMThreeDSolidGroup.row
        row['id'] = fields.id
        row['Displacement'] = fields.mDisplacement
        row['Group'] = fields.mGroup
        row['MaterialID'] = fields.mMaterialID
        row['Result'] = fields.mResult
        row.append()
        self.tblFEMThreeDSolidGroup.flush()

    ## Adds a FEMThreeDSolidGroup set.
    #
    # The ID field of each FEMThreeDSolidGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMThreeDSolidGroupFields objects
    # @see FEMThreeDSolidGroupProps
    # @see getFEMThreeDSolidGroup()
    def addFEMThreeDSolidGroupList(self, fieldsList):
        row = self.tblFEMThreeDSolidGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Displacement'] = fields.mDisplacement
            row['Group'] = fields.mGroup
            row['MaterialID'] = fields.mMaterialID
            row['Result'] = fields.mResult
            row.append()
        self.tblFEMThreeDSolidGroup.flush()

    ## Gets the HDF5 table row of an FEMThreeDSolid object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMThreeDSolidProps
    def getFEMThreeDSolid(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMThreeDSolid.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMThreeDSolid object.
    #
    # The ID field of the given FEMThreeDSolid properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMThreeDSolid object fields
    # @see getFEMThreeDSolid()
    def addFEMThreeDSolid(self, fields):
        row = self.tblFEMThreeDSolid.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['Group'] = fields.mGroup
        row['MaterialID'] = fields.mMaterialID
        row['Maxes'] = fields.mMaxes
        row['N1'] = fields.mN1
        row['N10'] = fields.mN10
        row['N11'] = fields.mN11
        row['N12'] = fields.mN12
        row['N13'] = fields.mN13
        row['N14'] = fields.mN14
        row['N15'] = fields.mN15
        row['N16'] = fields.mN16
        row['N17'] = fields.mN17
        row['N18'] = fields.mN18
        row['N19'] = fields.mN19
        row['N2'] = fields.mN2
        row['N20'] = fields.mN20
        row['N21'] = fields.mN21
        row['N22'] = fields.mN22
        row['N23'] = fields.mN23
        row['N24'] = fields.mN24
        row['N25'] = fields.mN25
        row['N26'] = fields.mN26
        row['N27'] = fields.mN27
        row['N3'] = fields.mN3
        row['N4'] = fields.mN4
        row['N5'] = fields.mN5
        row['N6'] = fields.mN6
        row['N7'] = fields.mN7
        row['N8'] = fields.mN8
        row['N9'] = fields.mN9
        row['NodeAmount'] = fields.mNodeAmount
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row.append()
        self.tblFEMThreeDSolid.flush()

    ## Adds a FEMThreeDSolid set.
    #
    # The ID field of each FEMThreeDSolid property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMThreeDSolidFields objects
    # @see FEMThreeDSolidProps
    # @see getFEMThreeDSolid()
    def addFEMThreeDSolidList(self, fieldsList):
        row = self.tblFEMThreeDSolid.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['Group'] = fields.mGroup
            row['MaterialID'] = fields.mMaterialID
            row['Maxes'] = fields.mMaxes
            row['N1'] = fields.mN1
            row['N10'] = fields.mN10
            row['N11'] = fields.mN11
            row['N12'] = fields.mN12
            row['N13'] = fields.mN13
            row['N14'] = fields.mN14
            row['N15'] = fields.mN15
            row['N16'] = fields.mN16
            row['N17'] = fields.mN17
            row['N18'] = fields.mN18
            row['N19'] = fields.mN19
            row['N2'] = fields.mN2
            row['N20'] = fields.mN20
            row['N21'] = fields.mN21
            row['N22'] = fields.mN22
            row['N23'] = fields.mN23
            row['N24'] = fields.mN24
            row['N25'] = fields.mN25
            row['N26'] = fields.mN26
            row['N27'] = fields.mN27
            row['N3'] = fields.mN3
            row['N4'] = fields.mN4
            row['N5'] = fields.mN5
            row['N6'] = fields.mN6
            row['N7'] = fields.mN7
            row['N8'] = fields.mN8
            row['N9'] = fields.mN9
            row['NodeAmount'] = fields.mNodeAmount
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row.append()
        self.tblFEMThreeDSolid.flush()

    ## Gets the HDF5 table row of an FEMSectionProp object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSectionPropProps
    def getFEMSectionProp(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSectionProp.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSectionProp object.
    #
    # The ID field of the given FEMSectionProp properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSectionProp object fields
    # @see getFEMSectionProp()
    def addFEMSectionProp(self, fields):
        row = self.tblFEMSectionProp.row
        row['id'] = fields.id
        row['Area'] = fields.mArea
        row['Rinertia'] = fields.mRinertia
        row['Sarea'] = fields.mSarea
        row['SectionID'] = fields.mSectionID
        row['Sinertia'] = fields.mSinertia
        row['Tarea'] = fields.mTarea
        row['Tinertia'] = fields.mTinertia
        row.append()
        self.tblFEMSectionProp.flush()

    ## Adds a FEMSectionProp set.
    #
    # The ID field of each FEMSectionProp property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSectionPropFields objects
    # @see FEMSectionPropProps
    # @see getFEMSectionProp()
    def addFEMSectionPropList(self, fieldsList):
        row = self.tblFEMSectionProp.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Area'] = fields.mArea
            row['Rinertia'] = fields.mRinertia
            row['Sarea'] = fields.mSarea
            row['SectionID'] = fields.mSectionID
            row['Sinertia'] = fields.mSinertia
            row['Tarea'] = fields.mTarea
            row['Tinertia'] = fields.mTinertia
            row.append()
        self.tblFEMSectionProp.flush()

    ## Gets the HDF5 table row of an FEMElasticMaterial object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMElasticMaterialProps
    def getFEMElasticMaterial(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMElasticMaterial.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMElasticMaterial object.
    #
    # The ID field of the given FEMElasticMaterial properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMElasticMaterial object fields
    # @see getFEMElasticMaterial()
    def addFEMElasticMaterial(self, fields):
        row = self.tblFEMElasticMaterial.row
        row['id'] = fields.id
        row['Alpha'] = fields.mAlpha
        row['Density'] = fields.mDensity
        row['E'] = fields.mE
        row['Material'] = fields.mMaterial
        row['NU'] = fields.mNU
        row.append()
        self.tblFEMElasticMaterial.flush()

    ## Adds a FEMElasticMaterial set.
    #
    # The ID field of each FEMElasticMaterial property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMElasticMaterialFields objects
    # @see FEMElasticMaterialProps
    # @see getFEMElasticMaterial()
    def addFEMElasticMaterialList(self, fieldsList):
        row = self.tblFEMElasticMaterial.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Alpha'] = fields.mAlpha
            row['Density'] = fields.mDensity
            row['E'] = fields.mE
            row['Material'] = fields.mMaterial
            row['NU'] = fields.mNU
            row.append()
        self.tblFEMElasticMaterial.flush()

    ## Gets the HDF5 table row of an FEMPoints object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMPointsProps
    def getFEMPoints(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMPoints.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMPoints object.
    #
    # The ID field of the given FEMPoints properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMPoints object fields
    # @see getFEMPoints()
    def addFEMPoints(self, fields):
        row = self.tblFEMPoints.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['PointID'] = fields.mPointID
        row['SystemID'] = fields.mSystemID
        row['X'] = fields.mX
        row['Y'] = fields.mY
        row['Z'] = fields.mZ
        row.append()
        self.tblFEMPoints.flush()

    ## Adds a FEMPoints set.
    #
    # The ID field of each FEMPoints property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMPointsFields objects
    # @see FEMPointsProps
    # @see getFEMPoints()
    def addFEMPointsList(self, fieldsList):
        row = self.tblFEMPoints.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['PointID'] = fields.mPointID
            row['SystemID'] = fields.mSystemID
            row['X'] = fields.mX
            row['Y'] = fields.mY
            row['Z'] = fields.mZ
            row.append()
        self.tblFEMPoints.flush()

    ## Gets the HDF5 table row of an FEMThermoOrthMaterials object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMThermoOrthMaterialsProps
    def getFEMThermoOrthMaterials(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMThermoOrthMaterials.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMThermoOrthMaterials object.
    #
    # The ID field of the given FEMThermoOrthMaterials properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMThermoOrthMaterials object fields
    # @see getFEMThermoOrthMaterials()
    def addFEMThermoOrthMaterials(self, fields):
        row = self.tblFEMThermoOrthMaterials.row
        row['id'] = fields.id
        row['Density'] = fields.mDensity
        row['MaterialID'] = fields.mMaterialID
        row['TREF'] = fields.mTREF
        row.append()
        self.tblFEMThermoOrthMaterials.flush()

    ## Adds a FEMThermoOrthMaterials set.
    #
    # The ID field of each FEMThermoOrthMaterials property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMThermoOrthMaterialsFields objects
    # @see FEMThermoOrthMaterialsProps
    # @see getFEMThermoOrthMaterials()
    def addFEMThermoOrthMaterialsList(self, fieldsList):
        row = self.tblFEMThermoOrthMaterials.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Density'] = fields.mDensity
            row['MaterialID'] = fields.mMaterialID
            row['TREF'] = fields.mTREF
            row.append()
        self.tblFEMThermoOrthMaterials.flush()

    ## Gets the HDF5 table row of an FEMConstraints object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMConstraintsProps
    def getFEMConstraints(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMConstraints.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMConstraints object.
    #
    # The ID field of the given FEMConstraints properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMConstraints object fields
    # @see getFEMConstraints()
    def addFEMConstraints(self, fields):
        row = self.tblFEMConstraints.row
        row['id'] = fields.id
        row['ConstraintID'] = fields.mConstraintID
        row['Description'] = fields.mDescription
        row['SlaveDOF'] = fields.mSlaveDOF
        row['SlaveNode'] = fields.mSlaveNode
        row.append()
        self.tblFEMConstraints.flush()

    ## Adds a FEMConstraints set.
    #
    # The ID field of each FEMConstraints property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMConstraintsFields objects
    # @see FEMConstraintsProps
    # @see getFEMConstraints()
    def addFEMConstraintsList(self, fieldsList):
        row = self.tblFEMConstraints.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ConstraintID'] = fields.mConstraintID
            row['Description'] = fields.mDescription
            row['SlaveDOF'] = fields.mSlaveDOF
            row['SlaveNode'] = fields.mSlaveNode
            row.append()
        self.tblFEMConstraints.flush()

    ## Gets the HDF5 table row of an FEMMCrigidities object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMMCrigiditiesProps
    def getFEMMCrigidities(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMMCrigidities.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMMCrigidities object.
    #
    # The ID field of the given FEMMCrigidities properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMMCrigidities object fields
    # @see getFEMMCrigidities()
    def addFEMMCrigidities(self, fields):
        row = self.tblFEMMCrigidities.row
        row['id'] = fields.id
        row['AcurveType'] = fields.mAcurveType
        row['Alpha'] = fields.mAlpha
        row['AxialCF'] = fields.mAxialCF
        row['BcurveType'] = fields.mBcurveType
        row['BendingCF'] = fields.mBendingCF
        row['Beta'] = fields.mBeta
        row['Density'] = fields.mDensity
        row['ForceAxID'] = fields.mForceAxID
        row['Hardening'] = fields.mHardening
        row['MassArea'] = fields.mMassArea
        row['MassR'] = fields.mMassR
        row['MassS'] = fields.mMassS
        row['MassT'] = fields.mMassT
        row['MomentRID'] = fields.mMomentRID
        row['MomentSID'] = fields.mMomentSID
        row['MomentTID'] = fields.mMomentTID
        row['RigidityID'] = fields.mRigidityID
        row['TcurveType'] = fields.mTcurveType
        row['TorsionCF'] = fields.mTorsionCF
        row.append()
        self.tblFEMMCrigidities.flush()

    ## Adds a FEMMCrigidities set.
    #
    # The ID field of each FEMMCrigidities property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMMCrigiditiesFields objects
    # @see FEMMCrigiditiesProps
    # @see getFEMMCrigidities()
    def addFEMMCrigiditiesList(self, fieldsList):
        row = self.tblFEMMCrigidities.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AcurveType'] = fields.mAcurveType
            row['Alpha'] = fields.mAlpha
            row['AxialCF'] = fields.mAxialCF
            row['BcurveType'] = fields.mBcurveType
            row['BendingCF'] = fields.mBendingCF
            row['Beta'] = fields.mBeta
            row['Density'] = fields.mDensity
            row['ForceAxID'] = fields.mForceAxID
            row['Hardening'] = fields.mHardening
            row['MassArea'] = fields.mMassArea
            row['MassR'] = fields.mMassR
            row['MassS'] = fields.mMassS
            row['MassT'] = fields.mMassT
            row['MomentRID'] = fields.mMomentRID
            row['MomentSID'] = fields.mMomentSID
            row['MomentTID'] = fields.mMomentTID
            row['RigidityID'] = fields.mRigidityID
            row['TcurveType'] = fields.mTcurveType
            row['TorsionCF'] = fields.mTorsionCF
            row.append()
        self.tblFEMMCrigidities.flush()

    ## Gets the HDF5 table row of an FEMSkeySysNode object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSkeySysNodeProps
    def getFEMSkeySysNode(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSkeySysNode.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSkeySysNode object.
    #
    # The ID field of the given FEMSkeySysNode properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSkeySysNode object fields
    # @see getFEMSkeySysNode()
    def addFEMSkeySysNode(self, fields):
        row = self.tblFEMSkeySysNode.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['Node1'] = fields.mNode1
        row['Node2'] = fields.mNode2
        row['Node3'] = fields.mNode3
        row['SkewSystemID'] = fields.mSkewSystemID
        row.append()
        self.tblFEMSkeySysNode.flush()

    ## Adds a FEMSkeySysNode set.
    #
    # The ID field of each FEMSkeySysNode property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSkeySysNodeFields objects
    # @see FEMSkeySysNodeProps
    # @see getFEMSkeySysNode()
    def addFEMSkeySysNodeList(self, fieldsList):
        row = self.tblFEMSkeySysNode.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['Node1'] = fields.mNode1
            row['Node2'] = fields.mNode2
            row['Node3'] = fields.mNode3
            row['SkewSystemID'] = fields.mSkewSystemID
            row.append()
        self.tblFEMSkeySysNode.flush()

    ## Gets the HDF5 table row of an FEMIsoBeamGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMIsoBeamGroupProps
    def getFEMIsoBeamGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMIsoBeamGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMIsoBeamGroup object.
    #
    # The ID field of the given FEMIsoBeamGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMIsoBeamGroup object fields
    # @see getFEMIsoBeamGroup()
    def addFEMIsoBeamGroup(self, fields):
        row = self.tblFEMIsoBeamGroup.row
        row['id'] = fields.id
        row['Displacement'] = fields.mDisplacement
        row['Group'] = fields.mGroup
        row['IniStrain'] = fields.mIniStrain
        row['MaterialID'] = fields.mMaterialID
        row['Result'] = fields.mResult
        row['SectionID'] = fields.mSectionID
        row.append()
        self.tblFEMIsoBeamGroup.flush()

    ## Adds a FEMIsoBeamGroup set.
    #
    # The ID field of each FEMIsoBeamGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMIsoBeamGroupFields objects
    # @see FEMIsoBeamGroupProps
    # @see getFEMIsoBeamGroup()
    def addFEMIsoBeamGroupList(self, fieldsList):
        row = self.tblFEMIsoBeamGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Displacement'] = fields.mDisplacement
            row['Group'] = fields.mGroup
            row['IniStrain'] = fields.mIniStrain
            row['MaterialID'] = fields.mMaterialID
            row['Result'] = fields.mResult
            row['SectionID'] = fields.mSectionID
            row.append()
        self.tblFEMIsoBeamGroup.flush()

    ## Gets the HDF5 table row of an FEMShellDOF object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMShellDOFProps
    def getFEMShellDOF(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMShellDOF.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMShellDOF object.
    #
    # The ID field of the given FEMShellDOF properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMShellDOF object fields
    # @see getFEMShellDOF()
    def addFEMShellDOF(self, fields):
        row = self.tblFEMShellDOF.row
        row['id'] = fields.id
        row['DOFnumber'] = fields.mDOFnumber
        row['Node'] = fields.mNode
        row['VectorID'] = fields.mVectorID
        row.append()
        self.tblFEMShellDOF.flush()

    ## Adds a FEMShellDOF set.
    #
    # The ID field of each FEMShellDOF property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMShellDOFFields objects
    # @see FEMShellDOFProps
    # @see getFEMShellDOF()
    def addFEMShellDOFList(self, fieldsList):
        row = self.tblFEMShellDOF.row
        for fields in fieldsList:
            row[name] = fields.id
            row['DOFnumber'] = fields.mDOFnumber
            row['Node'] = fields.mNode
            row['VectorID'] = fields.mVectorID
            row.append()
        self.tblFEMShellDOF.flush()

    ## Gets the HDF5 table row of an FEMCrossSections object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMCrossSectionsProps
    def getFEMCrossSections(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMCrossSections.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMCrossSections object.
    #
    # The ID field of the given FEMCrossSections properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMCrossSections object fields
    # @see getFEMCrossSections()
    def addFEMCrossSections(self, fields):
        row = self.tblFEMCrossSections.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['SectionID'] = fields.mSectionID
        row['SectionType'] = fields.mSectionType
        row.append()
        self.tblFEMCrossSections.flush()

    ## Adds a FEMCrossSections set.
    #
    # The ID field of each FEMCrossSections property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMCrossSectionsFields objects
    # @see FEMCrossSectionsProps
    # @see getFEMCrossSections()
    def addFEMCrossSectionsList(self, fieldsList):
        row = self.tblFEMCrossSections.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['SectionID'] = fields.mSectionID
            row['SectionType'] = fields.mSectionType
            row.append()
        self.tblFEMCrossSections.flush()

    ## Gets the HDF5 table row of an FEMTwistMomentData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMTwistMomentDataProps
    def getFEMTwistMomentData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMTwistMomentData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMTwistMomentData object.
    #
    # The ID field of the given FEMTwistMomentData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMTwistMomentData object fields
    # @see getFEMTwistMomentData()
    def addFEMTwistMomentData(self, fields):
        row = self.tblFEMTwistMomentData.row
        row['id'] = fields.id
        row['Moment'] = fields.mMoment
        row['RecordNmb'] = fields.mRecordNmb
        row['Twist'] = fields.mTwist
        row['TwistMomentID'] = fields.mTwistMomentID
        row.append()
        self.tblFEMTwistMomentData.flush()

    ## Adds a FEMTwistMomentData set.
    #
    # The ID field of each FEMTwistMomentData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMTwistMomentDataFields objects
    # @see FEMTwistMomentDataProps
    # @see getFEMTwistMomentData()
    def addFEMTwistMomentDataList(self, fieldsList):
        row = self.tblFEMTwistMomentData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Moment'] = fields.mMoment
            row['RecordNmb'] = fields.mRecordNmb
            row['Twist'] = fields.mTwist
            row['TwistMomentID'] = fields.mTwistMomentID
            row.append()
        self.tblFEMTwistMomentData.flush()

    ## Gets the HDF5 table row of an FEMShell object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMShellProps
    def getFEMShell(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMShell.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMShell object.
    #
    # The ID field of the given FEMShell properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMShell object fields
    # @see getFEMShell()
    def addFEMShell(self, fields):
        row = self.tblFEMShell.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['Group'] = fields.mGroup
        row['Material'] = fields.mMaterial
        row['N1'] = fields.mN1
        row['N2'] = fields.mN2
        row['N3'] = fields.mN3
        row['N4'] = fields.mN4
        row['N5'] = fields.mN5
        row['N6'] = fields.mN6
        row['N7'] = fields.mN7
        row['N8'] = fields.mN8
        row['N9'] = fields.mN9
        row['NodeAmount'] = fields.mNodeAmount
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row['T1'] = fields.mT1
        row['T2'] = fields.mT2
        row['T3'] = fields.mT3
        row['T4'] = fields.mT4
        row['T5'] = fields.mT5
        row['T6'] = fields.mT6
        row['T7'] = fields.mT7
        row['T8'] = fields.mT8
        row['T9'] = fields.mT9
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row.append()
        self.tblFEMShell.flush()

    ## Adds a FEMShell set.
    #
    # The ID field of each FEMShell property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMShellFields objects
    # @see FEMShellProps
    # @see getFEMShell()
    def addFEMShellList(self, fieldsList):
        row = self.tblFEMShell.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['Group'] = fields.mGroup
            row['Material'] = fields.mMaterial
            row['N1'] = fields.mN1
            row['N2'] = fields.mN2
            row['N3'] = fields.mN3
            row['N4'] = fields.mN4
            row['N5'] = fields.mN5
            row['N6'] = fields.mN6
            row['N7'] = fields.mN7
            row['N8'] = fields.mN8
            row['N9'] = fields.mN9
            row['NodeAmount'] = fields.mNodeAmount
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row['T1'] = fields.mT1
            row['T2'] = fields.mT2
            row['T3'] = fields.mT3
            row['T4'] = fields.mT4
            row['T5'] = fields.mT5
            row['T6'] = fields.mT6
            row['T7'] = fields.mT7
            row['T8'] = fields.mT8
            row['T9'] = fields.mT9
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row.append()
        self.tblFEMShell.flush()

    ## Gets the HDF5 table row of an FEMNTNContact object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMNTNContactProps
    def getFEMNTNContact(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMNTNContact.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMNTNContact object.
    #
    # The ID field of the given FEMNTNContact properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMNTNContact object fields
    # @see getFEMNTNContact()
    def addFEMNTNContact(self, fields):
        row = self.tblFEMNTNContact.row
        row['id'] = fields.id
        row['ContactorNode'] = fields.mContactorNode
        row['ContGroupID'] = fields.mContGroupID
        row['ContPair'] = fields.mContPair
        row['PrintRes'] = fields.mPrintRes
        row['RecordNmb'] = fields.mRecordNmb
        row['SaveRes'] = fields.mSaveRes
        row['TargetNode'] = fields.mTargetNode
        row['TargetNx'] = fields.mTargetNx
        row['TargetNy'] = fields.mTargetNy
        row['TargetNz'] = fields.mTargetNz
        row.append()
        self.tblFEMNTNContact.flush()

    ## Adds a FEMNTNContact set.
    #
    # The ID field of each FEMNTNContact property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMNTNContactFields objects
    # @see FEMNTNContactProps
    # @see getFEMNTNContact()
    def addFEMNTNContactList(self, fieldsList):
        row = self.tblFEMNTNContact.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ContactorNode'] = fields.mContactorNode
            row['ContGroupID'] = fields.mContGroupID
            row['ContPair'] = fields.mContPair
            row['PrintRes'] = fields.mPrintRes
            row['RecordNmb'] = fields.mRecordNmb
            row['SaveRes'] = fields.mSaveRes
            row['TargetNode'] = fields.mTargetNode
            row['TargetNx'] = fields.mTargetNx
            row['TargetNy'] = fields.mTargetNy
            row['TargetNz'] = fields.mTargetNz
            row.append()
        self.tblFEMNTNContact.flush()

    ## Gets the HDF5 table row of an FEMShellLayer object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMShellLayerProps
    def getFEMShellLayer(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMShellLayer.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMShellLayer object.
    #
    # The ID field of the given FEMShellLayer properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMShellLayer object fields
    # @see getFEMShellLayer()
    def addFEMShellLayer(self, fields):
        row = self.tblFEMShellLayer.row
        row['id'] = fields.id
        row['Group'] = fields.mGroup
        row['LayerNumber'] = fields.mLayerNumber
        row['MaterialID'] = fields.mMaterialID
        row['PThick'] = fields.mPThick
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMShellLayer.flush()

    ## Adds a FEMShellLayer set.
    #
    # The ID field of each FEMShellLayer property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMShellLayerFields objects
    # @see FEMShellLayerProps
    # @see getFEMShellLayer()
    def addFEMShellLayerList(self, fieldsList):
        row = self.tblFEMShellLayer.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Group'] = fields.mGroup
            row['LayerNumber'] = fields.mLayerNumber
            row['MaterialID'] = fields.mMaterialID
            row['PThick'] = fields.mPThick
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMShellLayer.flush()

    ## Gets the HDF5 table row of an FEMSkewSysAngles object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSkewSysAnglesProps
    def getFEMSkewSysAngles(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSkewSysAngles.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSkewSysAngles object.
    #
    # The ID field of the given FEMSkewSysAngles properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSkewSysAngles object fields
    # @see getFEMSkewSysAngles()
    def addFEMSkewSysAngles(self, fields):
        row = self.tblFEMSkewSysAngles.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['PHI'] = fields.mPHI
        row['SkewSystemID'] = fields.mSkewSystemID
        row['THETA'] = fields.mTHETA
        row['XSI'] = fields.mXSI
        row.append()
        self.tblFEMSkewSysAngles.flush()

    ## Adds a FEMSkewSysAngles set.
    #
    # The ID field of each FEMSkewSysAngles property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSkewSysAnglesFields objects
    # @see FEMSkewSysAnglesProps
    # @see getFEMSkewSysAngles()
    def addFEMSkewSysAnglesList(self, fieldsList):
        row = self.tblFEMSkewSysAngles.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['PHI'] = fields.mPHI
            row['SkewSystemID'] = fields.mSkewSystemID
            row['THETA'] = fields.mTHETA
            row['XSI'] = fields.mXSI
            row.append()
        self.tblFEMSkewSysAngles.flush()

    ## Gets the HDF5 table row of an FEMGroundMotionRecord object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMGroundMotionRecordProps
    def getFEMGroundMotionRecord(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMGroundMotionRecord.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMGroundMotionRecord object.
    #
    # The ID field of the given FEMGroundMotionRecord properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMGroundMotionRecord object fields
    # @see getFEMGroundMotionRecord()
    def addFEMGroundMotionRecord(self, fields):
        row = self.tblFEMGroundMotionRecord.row
        row['id'] = fields.id
        row['Description'] = fields.mDescription
        row['GMRecordID'] = fields.mGMRecordID
        row['GMRecordName'] = fields.mGMRecordName
        row.append()
        self.tblFEMGroundMotionRecord.flush()

    ## Adds a FEMGroundMotionRecord set.
    #
    # The ID field of each FEMGroundMotionRecord property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMGroundMotionRecordFields objects
    # @see FEMGroundMotionRecordProps
    # @see getFEMGroundMotionRecord()
    def addFEMGroundMotionRecordList(self, fieldsList):
        row = self.tblFEMGroundMotionRecord.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Description'] = fields.mDescription
            row['GMRecordID'] = fields.mGMRecordID
            row['GMRecordName'] = fields.mGMRecordName
            row.append()
        self.tblFEMGroundMotionRecord.flush()

    ## Gets the HDF5 table row of an FEMGeneralGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMGeneralGroupProps
    def getFEMGeneralGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMGeneralGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMGeneralGroup object.
    #
    # The ID field of the given FEMGeneralGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMGeneralGroup object fields
    # @see getFEMGeneralGroup()
    def addFEMGeneralGroup(self, fields):
        row = self.tblFEMGeneralGroup.row
        row['id'] = fields.id
        row['Group'] = fields.mGroup
        row['MatrixSet'] = fields.mMatrixSet
        row['Result'] = fields.mResult
        row['SkewSystem'] = fields.mSkewSystem
        row.append()
        self.tblFEMGeneralGroup.flush()

    ## Adds a FEMGeneralGroup set.
    #
    # The ID field of each FEMGeneralGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMGeneralGroupFields objects
    # @see FEMGeneralGroupProps
    # @see getFEMGeneralGroup()
    def addFEMGeneralGroupList(self, fieldsList):
        row = self.tblFEMGeneralGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Group'] = fields.mGroup
            row['MatrixSet'] = fields.mMatrixSet
            row['Result'] = fields.mResult
            row['SkewSystem'] = fields.mSkewSystem
            row.append()
        self.tblFEMGeneralGroup.flush()

    ## Gets the HDF5 table row of an FEMTwoDSolid object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMTwoDSolidProps
    def getFEMTwoDSolid(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMTwoDSolid.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMTwoDSolid object.
    #
    # The ID field of the given FEMTwoDSolid properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMTwoDSolid object fields
    # @see getFEMTwoDSolid()
    def addFEMTwoDSolid(self, fields):
        row = self.tblFEMTwoDSolid.row
        row['id'] = fields.id
        row['Bet'] = fields.mBet
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['Group'] = fields.mGroup
        row['MaterialID'] = fields.mMaterialID
        row['N1'] = fields.mN1
        row['N2'] = fields.mN2
        row['N3'] = fields.mN3
        row['N4'] = fields.mN4
        row['N5'] = fields.mN5
        row['N6'] = fields.mN6
        row['N7'] = fields.mN7
        row['N8'] = fields.mN8
        row['N9'] = fields.mN9
        row['NodeAmount'] = fields.mNodeAmount
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row['Thick'] = fields.mThick
        row.append()
        self.tblFEMTwoDSolid.flush()

    ## Adds a FEMTwoDSolid set.
    #
    # The ID field of each FEMTwoDSolid property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMTwoDSolidFields objects
    # @see FEMTwoDSolidProps
    # @see getFEMTwoDSolid()
    def addFEMTwoDSolidList(self, fieldsList):
        row = self.tblFEMTwoDSolid.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Bet'] = fields.mBet
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['Group'] = fields.mGroup
            row['MaterialID'] = fields.mMaterialID
            row['N1'] = fields.mN1
            row['N2'] = fields.mN2
            row['N3'] = fields.mN3
            row['N4'] = fields.mN4
            row['N5'] = fields.mN5
            row['N6'] = fields.mN6
            row['N7'] = fields.mN7
            row['N8'] = fields.mN8
            row['N9'] = fields.mN9
            row['NodeAmount'] = fields.mNodeAmount
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row['Thick'] = fields.mThick
            row.append()
        self.tblFEMTwoDSolid.flush()

    ## Gets the HDF5 table row of an FEMAppliedTemperature object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMAppliedTemperatureProps
    def getFEMAppliedTemperature(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMAppliedTemperature.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMAppliedTemperature object.
    #
    # The ID field of the given FEMAppliedTemperature properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMAppliedTemperature object fields
    # @see getFEMAppliedTemperature()
    def addFEMAppliedTemperature(self, fields):
        row = self.tblFEMAppliedTemperature.row
        row['id'] = fields.id
        row['ArrivalTime'] = fields.mArrivalTime
        row['Factor'] = fields.mFactor
        row['Node'] = fields.mNode
        row['RecordNmbr'] = fields.mRecordNmbr
        row['TimeFunctionID'] = fields.mTimeFunctionID
        row.append()
        self.tblFEMAppliedTemperature.flush()

    ## Adds a FEMAppliedTemperature set.
    #
    # The ID field of each FEMAppliedTemperature property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMAppliedTemperatureFields objects
    # @see FEMAppliedTemperatureProps
    # @see getFEMAppliedTemperature()
    def addFEMAppliedTemperatureList(self, fieldsList):
        row = self.tblFEMAppliedTemperature.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ArrivalTime'] = fields.mArrivalTime
            row['Factor'] = fields.mFactor
            row['Node'] = fields.mNode
            row['RecordNmbr'] = fields.mRecordNmbr
            row['TimeFunctionID'] = fields.mTimeFunctionID
            row.append()
        self.tblFEMAppliedTemperature.flush()

    ## Gets the HDF5 table row of an FEMMatrixSets object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMMatrixSetsProps
    def getFEMMatrixSets(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMMatrixSets.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMMatrixSets object.
    #
    # The ID field of the given FEMMatrixSets properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMMatrixSets object fields
    # @see getFEMMatrixSets()
    def addFEMMatrixSets(self, fields):
        row = self.tblFEMMatrixSets.row
        row['id'] = fields.id
        row['Damping'] = fields.mDamping
        row['Description'] = fields.mDescription
        row['Mass'] = fields.mMass
        row['MatrixSetID'] = fields.mMatrixSetID
        row['Stiffness'] = fields.mStiffness
        row['Stress'] = fields.mStress
        row.append()
        self.tblFEMMatrixSets.flush()

    ## Adds a FEMMatrixSets set.
    #
    # The ID field of each FEMMatrixSets property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMMatrixSetsFields objects
    # @see FEMMatrixSetsProps
    # @see getFEMMatrixSets()
    def addFEMMatrixSetsList(self, fieldsList):
        row = self.tblFEMMatrixSets.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Damping'] = fields.mDamping
            row['Description'] = fields.mDescription
            row['Mass'] = fields.mMass
            row['MatrixSetID'] = fields.mMatrixSetID
            row['Stiffness'] = fields.mStiffness
            row['Stress'] = fields.mStress
            row.append()
        self.tblFEMMatrixSets.flush()

    ## Gets the HDF5 table row of an FEMConstraintCoef object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMConstraintCoefProps
    def getFEMConstraintCoef(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMConstraintCoef.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMConstraintCoef object.
    #
    # The ID field of the given FEMConstraintCoef properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMConstraintCoef object fields
    # @see getFEMConstraintCoef()
    def addFEMConstraintCoef(self, fields):
        row = self.tblFEMConstraintCoef.row
        row['id'] = fields.id
        row['Coefficient'] = fields.mCoefficient
        row['ConstraintID'] = fields.mConstraintID
        row['Description'] = fields.mDescription
        row['MasterDOF'] = fields.mMasterDOF
        row['MasterNode'] = fields.mMasterNode
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMConstraintCoef.flush()

    ## Adds a FEMConstraintCoef set.
    #
    # The ID field of each FEMConstraintCoef property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMConstraintCoefFields objects
    # @see FEMConstraintCoefProps
    # @see getFEMConstraintCoef()
    def addFEMConstraintCoefList(self, fieldsList):
        row = self.tblFEMConstraintCoef.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Coefficient'] = fields.mCoefficient
            row['ConstraintID'] = fields.mConstraintID
            row['Description'] = fields.mDescription
            row['MasterDOF'] = fields.mMasterDOF
            row['MasterNode'] = fields.mMasterNode
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMConstraintCoef.flush()

    ## Gets the HDF5 table row of an FEMSectionBox object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSectionBoxProps
    def getFEMSectionBox(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSectionBox.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSectionBox object.
    #
    # The ID field of the given FEMSectionBox properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSectionBox object fields
    # @see getFEMSectionBox()
    def addFEMSectionBox(self, fields):
        row = self.tblFEMSectionBox.row
        row['id'] = fields.id
        row['Height'] = fields.mHeight
        row['SC'] = fields.mSC
        row['SectionID'] = fields.mSectionID
        row['SSarea'] = fields.mSSarea
        row['TC'] = fields.mTC
        row['Thick1'] = fields.mThick1
        row['Thick2'] = fields.mThick2
        row['Torfac'] = fields.mTorfac
        row['TSarea'] = fields.mTSarea
        row['Width'] = fields.mWidth
        row.append()
        self.tblFEMSectionBox.flush()

    ## Adds a FEMSectionBox set.
    #
    # The ID field of each FEMSectionBox property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSectionBoxFields objects
    # @see FEMSectionBoxProps
    # @see getFEMSectionBox()
    def addFEMSectionBoxList(self, fieldsList):
        row = self.tblFEMSectionBox.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Height'] = fields.mHeight
            row['SC'] = fields.mSC
            row['SectionID'] = fields.mSectionID
            row['SSarea'] = fields.mSSarea
            row['TC'] = fields.mTC
            row['Thick1'] = fields.mThick1
            row['Thick2'] = fields.mThick2
            row['Torfac'] = fields.mTorfac
            row['TSarea'] = fields.mTSarea
            row['Width'] = fields.mWidth
            row.append()
        self.tblFEMSectionBox.flush()

    ## Gets the HDF5 table row of an FEMNKDisplForce object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMNKDisplForceProps
    def getFEMNKDisplForce(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMNKDisplForce.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMNKDisplForce object.
    #
    # The ID field of the given FEMNKDisplForce properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMNKDisplForce object fields
    # @see getFEMNKDisplForce()
    def addFEMNKDisplForce(self, fields):
        row = self.tblFEMNKDisplForce.row
        row['id'] = fields.id
        row['Displacement'] = fields.mDisplacement
        row['Force'] = fields.mForce
        row['PropertyID'] = fields.mPropertyID
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMNKDisplForce.flush()

    ## Adds a FEMNKDisplForce set.
    #
    # The ID field of each FEMNKDisplForce property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMNKDisplForceFields objects
    # @see FEMNKDisplForceProps
    # @see getFEMNKDisplForce()
    def addFEMNKDisplForceList(self, fieldsList):
        row = self.tblFEMNKDisplForce.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Displacement'] = fields.mDisplacement
            row['Force'] = fields.mForce
            row['PropertyID'] = fields.mPropertyID
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMNKDisplForce.flush()

    ## Gets the HDF5 table row of an FEMPlasticStrainStress object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMPlasticStrainStressProps
    def getFEMPlasticStrainStress(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMPlasticStrainStress.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMPlasticStrainStress object.
    #
    # The ID field of the given FEMPlasticStrainStress properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMPlasticStrainStress object fields
    # @see getFEMPlasticStrainStress()
    def addFEMPlasticStrainStress(self, fields):
        row = self.tblFEMPlasticStrainStress.row
        row['id'] = fields.id
        row['MaterialID'] = fields.mMaterialID
        row['RecordNumber'] = fields.mRecordNumber
        row['Strain'] = fields.mStrain
        row['Stress'] = fields.mStress
        row.append()
        self.tblFEMPlasticStrainStress.flush()

    ## Adds a FEMPlasticStrainStress set.
    #
    # The ID field of each FEMPlasticStrainStress property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMPlasticStrainStressFields objects
    # @see FEMPlasticStrainStressProps
    # @see getFEMPlasticStrainStress()
    def addFEMPlasticStrainStressList(self, fieldsList):
        row = self.tblFEMPlasticStrainStress.row
        for fields in fieldsList:
            row[name] = fields.id
            row['MaterialID'] = fields.mMaterialID
            row['RecordNumber'] = fields.mRecordNumber
            row['Strain'] = fields.mStrain
            row['Stress'] = fields.mStress
            row.append()
        self.tblFEMPlasticStrainStress.flush()

    ## Gets the HDF5 table row of an FEMShellAxesOrthoData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMShellAxesOrthoDataProps
    def getFEMShellAxesOrthoData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMShellAxesOrthoData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMShellAxesOrthoData object.
    #
    # The ID field of the given FEMShellAxesOrthoData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMShellAxesOrthoData object fields
    # @see getFEMShellAxesOrthoData()
    def addFEMShellAxesOrthoData(self, fields):
        row = self.tblFEMShellAxesOrthoData.row
        row['id'] = fields.id
        row['AxOrthoID'] = fields.mAxOrthoID
        row['ElementID'] = fields.mElementID
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMShellAxesOrthoData.flush()

    ## Adds a FEMShellAxesOrthoData set.
    #
    # The ID field of each FEMShellAxesOrthoData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMShellAxesOrthoDataFields objects
    # @see FEMShellAxesOrthoDataProps
    # @see getFEMShellAxesOrthoData()
    def addFEMShellAxesOrthoDataList(self, fieldsList):
        row = self.tblFEMShellAxesOrthoData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AxOrthoID'] = fields.mAxOrthoID
            row['ElementID'] = fields.mElementID
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMShellAxesOrthoData.flush()

    ## Gets the HDF5 table row of an FEMGeneralNode object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMGeneralNodeProps
    def getFEMGeneralNode(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMGeneralNode.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMGeneralNode object.
    #
    # The ID field of the given FEMGeneralNode properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMGeneralNode object fields
    # @see getFEMGeneralNode()
    def addFEMGeneralNode(self, fields):
        row = self.tblFEMGeneralNode.row
        row['id'] = fields.id
        row['ElementID'] = fields.mElementID
        row['Group'] = fields.mGroup
        row['LocalNmb'] = fields.mLocalNmb
        row['Node'] = fields.mNode
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMGeneralNode.flush()

    ## Adds a FEMGeneralNode set.
    #
    # The ID field of each FEMGeneralNode property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMGeneralNodeFields objects
    # @see FEMGeneralNodeProps
    # @see getFEMGeneralNode()
    def addFEMGeneralNodeList(self, fieldsList):
        row = self.tblFEMGeneralNode.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ElementID'] = fields.mElementID
            row['Group'] = fields.mGroup
            row['LocalNmb'] = fields.mLocalNmb
            row['Node'] = fields.mNode
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMGeneralNode.flush()

    ## Gets the HDF5 table row of an FEMStrLines object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMStrLinesProps
    def getFEMStrLines(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMStrLines.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMStrLines object.
    #
    # The ID field of the given FEMStrLines properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMStrLines object fields
    # @see getFEMStrLines()
    def addFEMStrLines(self, fields):
        row = self.tblFEMStrLines.row
        row['id'] = fields.id
        row['LineID'] = fields.mLineID
        row['P1'] = fields.mP1
        row['P2'] = fields.mP2
        row.append()
        self.tblFEMStrLines.flush()

    ## Adds a FEMStrLines set.
    #
    # The ID field of each FEMStrLines property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMStrLinesFields objects
    # @see FEMStrLinesProps
    # @see getFEMStrLines()
    def addFEMStrLinesList(self, fieldsList):
        row = self.tblFEMStrLines.row
        for fields in fieldsList:
            row[name] = fields.id
            row['LineID'] = fields.mLineID
            row['P1'] = fields.mP1
            row['P2'] = fields.mP2
            row.append()
        self.tblFEMStrLines.flush()

    ## Gets the HDF5 table row of an FEMContactSurface object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMContactSurfaceProps
    def getFEMContactSurface(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMContactSurface.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMContactSurface object.
    #
    # The ID field of the given FEMContactSurface properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMContactSurface object fields
    # @see getFEMContactSurface()
    def addFEMContactSurface(self, fields):
        row = self.tblFEMContactSurface.row
        row['id'] = fields.id
        row['ContGroupID'] = fields.mContGroupID
        row['ContSegment'] = fields.mContSegment
        row['ContSurface'] = fields.mContSurface
        row['N1'] = fields.mN1
        row['N2'] = fields.mN2
        row['N3'] = fields.mN3
        row['N4'] = fields.mN4
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMContactSurface.flush()

    ## Adds a FEMContactSurface set.
    #
    # The ID field of each FEMContactSurface property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMContactSurfaceFields objects
    # @see FEMContactSurfaceProps
    # @see getFEMContactSurface()
    def addFEMContactSurfaceList(self, fieldsList):
        row = self.tblFEMContactSurface.row
        for fields in fieldsList:
            row[name] = fields.id
            row['ContGroupID'] = fields.mContGroupID
            row['ContSegment'] = fields.mContSegment
            row['ContSurface'] = fields.mContSurface
            row['N1'] = fields.mN1
            row['N2'] = fields.mN2
            row['N3'] = fields.mN3
            row['N4'] = fields.mN4
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMContactSurface.flush()

    ## Gets the HDF5 table row of an FEMMCForceData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMMCForceDataProps
    def getFEMMCForceData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMMCForceData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMMCForceData object.
    #
    # The ID field of the given FEMMCForceData properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMMCForceData object fields
    # @see getFEMMCForceData()
    def addFEMMCForceData(self, fields):
        row = self.tblFEMMCForceData.row
        row['id'] = fields.id
        row['CurvMomentID'] = fields.mCurvMomentID
        row['Force'] = fields.mForce
        row['MomentSTID'] = fields.mMomentSTID
        row['RecordNmb'] = fields.mRecordNmb
        row.append()
        self.tblFEMMCForceData.flush()

    ## Adds a FEMMCForceData set.
    #
    # The ID field of each FEMMCForceData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMMCForceDataFields objects
    # @see FEMMCForceDataProps
    # @see getFEMMCForceData()
    def addFEMMCForceDataList(self, fieldsList):
        row = self.tblFEMMCForceData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['CurvMomentID'] = fields.mCurvMomentID
            row['Force'] = fields.mForce
            row['MomentSTID'] = fields.mMomentSTID
            row['RecordNmb'] = fields.mRecordNmb
            row.append()
        self.tblFEMMCForceData.flush()

    ## Gets the HDF5 table row of an FEMSpring object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSpringProps
    def getFEMSpring(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSpring.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSpring object.
    #
    # The ID field of the given FEMSpring properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSpring object fields
    # @see getFEMSpring()
    def addFEMSpring(self, fields):
        row = self.tblFEMSpring.row
        row['id'] = fields.id
        row['AX'] = fields.mAX
        row['AY'] = fields.mAY
        row['AZ'] = fields.mAZ
        row['Description'] = fields.mDescription
        row['ElementID'] = fields.mElementID
        row['Group'] = fields.mGroup
        row['ID1'] = fields.mID1
        row['ID2'] = fields.mID2
        row['N1'] = fields.mN1
        row['N2'] = fields.mN2
        row['PropertySet'] = fields.mPropertySet
        row['RecordNmb'] = fields.mRecordNmb
        row['Save'] = fields.mSave
        row['TBirth'] = fields.mTBirth
        row['TDeath'] = fields.mTDeath
        row.append()
        self.tblFEMSpring.flush()

    ## Adds a FEMSpring set.
    #
    # The ID field of each FEMSpring property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSpringFields objects
    # @see FEMSpringProps
    # @see getFEMSpring()
    def addFEMSpringList(self, fieldsList):
        row = self.tblFEMSpring.row
        for fields in fieldsList:
            row[name] = fields.id
            row['AX'] = fields.mAX
            row['AY'] = fields.mAY
            row['AZ'] = fields.mAZ
            row['Description'] = fields.mDescription
            row['ElementID'] = fields.mElementID
            row['Group'] = fields.mGroup
            row['ID1'] = fields.mID1
            row['ID2'] = fields.mID2
            row['N1'] = fields.mN1
            row['N2'] = fields.mN2
            row['PropertySet'] = fields.mPropertySet
            row['RecordNmb'] = fields.mRecordNmb
            row['Save'] = fields.mSave
            row['TBirth'] = fields.mTBirth
            row['TDeath'] = fields.mTDeath
            row.append()
        self.tblFEMSpring.flush()

    ## Gets the HDF5 table row of an FEMSpringGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMSpringGroupProps
    def getFEMSpringGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMSpringGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMSpringGroup object.
    #
    # The ID field of the given FEMSpringGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMSpringGroup object fields
    # @see getFEMSpringGroup()
    def addFEMSpringGroup(self, fields):
        row = self.tblFEMSpringGroup.row
        row['id'] = fields.id
        row['Bolt'] = fields.mBolt
        row['Group'] = fields.mGroup
        row['Nonlinear'] = fields.mNonlinear
        row['PropertySet'] = fields.mPropertySet
        row['Result'] = fields.mResult
        row['SkewSystem'] = fields.mSkewSystem
        row.append()
        self.tblFEMSpringGroup.flush()

    ## Adds a FEMSpringGroup set.
    #
    # The ID field of each FEMSpringGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMSpringGroupFields objects
    # @see FEMSpringGroupProps
    # @see getFEMSpringGroup()
    def addFEMSpringGroupList(self, fieldsList):
        row = self.tblFEMSpringGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Bolt'] = fields.mBolt
            row['Group'] = fields.mGroup
            row['Nonlinear'] = fields.mNonlinear
            row['PropertySet'] = fields.mPropertySet
            row['Result'] = fields.mResult
            row['SkewSystem'] = fields.mSkewSystem
            row.append()
        self.tblFEMSpringGroup.flush()

    ## Gets the HDF5 table row of an FEMShellGroup object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see FEMShellGroupProps
    def getFEMShellGroup(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblFEMShellGroup.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an FEMShellGroup object.
    #
    # The ID field of the given FEMShellGroup properties must be set to
    # the SQL object ID.
    #
    # @param fields FEMShellGroup object fields
    # @see getFEMShellGroup()
    def addFEMShellGroup(self, fields):
        row = self.tblFEMShellGroup.row
        row['id'] = fields.id
        row['Displacement'] = fields.mDisplacement
        row['Group'] = fields.mGroup
        row['Material'] = fields.mMaterial
        row['NLayers'] = fields.mNLayers
        row['Result'] = fields.mResult
        row['SectionResult'] = fields.mSectionResult
        row['StressReference'] = fields.mStressReference
        row['Thickness'] = fields.mThickness
        row.append()
        self.tblFEMShellGroup.flush()

    ## Adds a FEMShellGroup set.
    #
    # The ID field of each FEMShellGroup property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of FEMShellGroupFields objects
    # @see FEMShellGroupProps
    # @see getFEMShellGroup()
    def addFEMShellGroupList(self, fieldsList):
        row = self.tblFEMShellGroup.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Displacement'] = fields.mDisplacement
            row['Group'] = fields.mGroup
            row['Material'] = fields.mMaterial
            row['NLayers'] = fields.mNLayers
            row['Result'] = fields.mResult
            row['SectionResult'] = fields.mSectionResult
            row['StressReference'] = fields.mStressReference
            row['Thickness'] = fields.mThickness
            row.append()
        self.tblFEMShellGroup.flush()

    ## Gets the HDF5 table row of an DaqUnit object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see DaqUnitProps
    def getDaqUnit(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblDaqUnit.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an DaqUnit object.
    #
    # The ID field of the given DaqUnit properties must be set to
    # the SQL object ID.
    #
    # @param fields DaqUnit object fields
    # @see getDaqUnit()
    def addDaqUnit(self, fields):
        row = self.tblDaqUnit.row
        row['id'] = fields.id
        row['Model'] = fields.mModel
        row['Identifier'] = fields.mIdentifier
        row.append()
        self.tblDaqUnit.flush()

    ## Adds a DaqUnit set.
    #
    # The ID field of each DaqUnit property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of DaqUnitFields objects
    # @see DaqUnitProps
    # @see getDaqUnit()
    def addDaqUnitList(self, fieldsList):
        row = self.tblDaqUnit.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Model'] = fields.mModel
            row['Identifier'] = fields.mIdentifier
            row.append()
        self.tblDaqUnit.flush()

    ## Gets the HDF5 table row of an DaqUnitChannel object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see DaqUnitChannelProps
    def getDaqUnitChannel(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblDaqUnitChannel.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an DaqUnitChannel object.
    #
    # The ID field of the given DaqUnitChannel properties must be set to
    # the SQL object ID.
    #
    # @param fields DaqUnitChannel object fields
    # @see getDaqUnitChannel()
    def addDaqUnitChannel(self, fields):
        row = self.tblDaqUnitChannel.row
        row['id'] = fields.id
        row['Unit'] = fields.mUnit
        row['Number'] = fields.mNumber
        row.append()
        self.tblDaqUnitChannel.flush()

    ## Adds a DaqUnitChannel set.
    #
    # The ID field of each DaqUnitChannel property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of DaqUnitChannelFields objects
    # @see DaqUnitChannelProps
    # @see getDaqUnitChannel()
    def addDaqUnitChannelList(self, fieldsList):
        row = self.tblDaqUnitChannel.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Unit'] = fields.mUnit
            row['Number'] = fields.mNumber
            row.append()
        self.tblDaqUnitChannel.flush()

    ## Gets the HDF5 table row of an Sensor object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see SensorProps
    def getSensor(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblSensor.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an Sensor object.
    #
    # The ID field of the given Sensor properties must be set to
    # the SQL object ID.
    #
    # @param fields Sensor object fields
    # @see getSensor()
    def addSensor(self, fields):
        row = self.tblSensor.row
        row['id'] = fields.id
        row['Type'] = self.SensorTypeEnumList.index(str(fields.mType))
        row['Model'] = fields.mModel
        row['Identifier'] = fields.mIdentifier
        row.append()
        self.tblSensor.flush()

    ## Adds a Sensor set.
    #
    # The ID field of each Sensor property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of SensorFields objects
    # @see SensorProps
    # @see getSensor()
    def addSensorList(self, fieldsList):
        row = self.tblSensor.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Type'] = fields.mType
            row['Model'] = fields.mModel
            row['Identifier'] = fields.mIdentifier
            row.append()
        self.tblSensor.flush()

    ## Gets the HDF5 table row of an Transducer object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see TransducerProps
    def getTransducer(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblTransducer.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an Transducer object.
    #
    # The ID field of the given Transducer properties must be set to
    # the SQL object ID.
    #
    # @param fields Transducer object fields
    # @see getTransducer()
    def addTransducer(self, fields):
        row = self.tblTransducer.row
        row['id'] = fields.id
        row['Model'] = fields.mModel
        row['Identifier'] = fields.mIdentifier
        row.append()
        self.tblTransducer.flush()

    ## Adds a Transducer set.
    #
    # The ID field of each Transducer property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of TransducerFields objects
    # @see TransducerProps
    # @see getTransducer()
    def addTransducerList(self, fieldsList):
        row = self.tblTransducer.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Model'] = fields.mModel
            row['Identifier'] = fields.mIdentifier
            row.append()
        self.tblTransducer.flush()

    ## Gets the HDF5 table row of an Experiment object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see ExperimentProps
    def getExperiment(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblExperiment.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an Experiment object.
    #
    # The ID field of the given Experiment properties must be set to
    # the SQL object ID.
    #
    # @param fields Experiment object fields
    # @see getExperiment()
    def addExperiment(self, fields):
        row = self.tblExperiment.row
        row['id'] = fields.id
        row['Frequency'] = fields.mFrequency
        row['Duration'] = fields.mDuration
        row['BufferData'] = fields.mBufferData
        row.append()
        self.tblExperiment.flush()

    ## Adds a Experiment set.
    #
    # The ID field of each Experiment property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of ExperimentFields objects
    # @see ExperimentProps
    # @see getExperiment()
    def addExperimentList(self, fieldsList):
        row = self.tblExperiment.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Frequency'] = fields.mFrequency
            row['Duration'] = fields.mDuration
            row['BufferData'] = fields.mBufferData
            row.append()
        self.tblExperiment.flush()

    ## Gets the HDF5 table row of an Cycle object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see CycleProps
    def getCycle(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblCycle.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an Cycle object.
    #
    # The ID field of the given Cycle properties must be set to
    # the SQL object ID.
    #
    # @param fields Cycle object fields
    # @see getCycle()
    def addCycle(self, fields):
        row = self.tblCycle.row
        row['id'] = fields.id
        row['Time'] = fields.mTime
        row['Experiment'] = fields.mExperiment
        row.append()
        self.tblCycle.flush()

    ## Adds a Cycle set.
    #
    # The ID field of each Cycle property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of CycleFields objects
    # @see CycleProps
    # @see getCycle()
    def addCycleList(self, fieldsList):
        row = self.tblCycle.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Time'] = fields.mTime
            row['Experiment'] = fields.mExperiment
            row.append()
        self.tblCycle.flush()

    ## Gets the HDF5 table row of an SensorTimeData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see SensorTimeDataProps
    def getSensorTimeData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblSensorTimeData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an SensorTimeData object.
    #
    # The ID field of the given SensorTimeData properties must be set to
    # the SQL object ID.
    #
    # @param fields SensorTimeData object fields
    # @see getSensorTimeData()
    def addSensorTimeData(self, fields):
        row = self.tblSensorTimeData.row
        row['id'] = fields.id
        row['Structure'] = fields.mStructure
        row['Sensor'] = fields.mSensor
        row['Channel'] = fields.mChannel
        row['StartTime'] = fields.mStartTime
        row['Status'] = fields.mStatus
        row['Node'] = fields.mNode
        row.append()
        self.tblSensorTimeData.flush()

    ## Adds a SensorTimeData set.
    #
    # The ID field of each SensorTimeData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of SensorTimeDataFields objects
    # @see SensorTimeDataProps
    # @see getSensorTimeData()
    def addSensorTimeDataList(self, fieldsList):
        row = self.tblSensorTimeData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Structure'] = fields.mStructure
            row['Sensor'] = fields.mSensor
            row['Channel'] = fields.mChannel
            row['StartTime'] = fields.mStartTime
            row['Status'] = fields.mStatus
            row['Node'] = fields.mNode
            row.append()
        self.tblSensorTimeData.flush()

    ## Gets the HDF5 table row of an TransducerTimeData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see TransducerTimeDataProps
    def getTransducerTimeData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblTransducerTimeData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an TransducerTimeData object.
    #
    # The ID field of the given TransducerTimeData properties must be set to
    # the SQL object ID.
    #
    # @param fields TransducerTimeData object fields
    # @see getTransducerTimeData()
    def addTransducerTimeData(self, fields):
        row = self.tblTransducerTimeData.row
        row['id'] = fields.id
        row['Transducer'] = fields.mTransducer
        row['Sensor'] = fields.mSensor
        row['Node'] = fields.mNode
        row['Orientation'] = fields.mOrientation
        row['Amplification'] = fields.mAmplification
        row['Channel'] = fields.mChannel
        row['Filter'] = fields.mFilter
        row['StartTime'] = fields.mStartTime
        row['Status'] = fields.mStatus
        row.append()
        self.tblTransducerTimeData.flush()

    ## Adds a TransducerTimeData set.
    #
    # The ID field of each TransducerTimeData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of TransducerTimeDataFields objects
    # @see TransducerTimeDataProps
    # @see getTransducerTimeData()
    def addTransducerTimeDataList(self, fieldsList):
        row = self.tblTransducerTimeData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Transducer'] = fields.mTransducer
            row['Sensor'] = fields.mSensor
            row['Node'] = fields.mNode
            row['Orientation'] = fields.mOrientation
            row['Amplification'] = fields.mAmplification
            row['Channel'] = fields.mChannel
            row['Filter'] = fields.mFilter
            row['StartTime'] = fields.mStartTime
            row['Status'] = fields.mStatus
            row.append()
        self.tblTransducerTimeData.flush()

    ## Gets the HDF5 table row of an ExperimentStructureData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see ExperimentStructureDataProps
    def getExperimentStructureData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblExperimentStructureData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an ExperimentStructureData object.
    #
    # The ID field of the given ExperimentStructureData properties must be set to
    # the SQL object ID.
    #
    # @param fields ExperimentStructureData object fields
    # @see getExperimentStructureData()
    def addExperimentStructureData(self, fields):
        row = self.tblExperimentStructureData.row
        row['id'] = fields.id
        row['Experiment'] = fields.mExperiment
        row['Structure'] = fields.mStructure
        row['StartTime'] = fields.mStartTime
        row['Status'] = fields.mStatus
        row.append()
        self.tblExperimentStructureData.flush()

    ## Adds a ExperimentStructureData set.
    #
    # The ID field of each ExperimentStructureData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of ExperimentStructureDataFields objects
    # @see ExperimentStructureDataProps
    # @see getExperimentStructureData()
    def addExperimentStructureDataList(self, fieldsList):
        row = self.tblExperimentStructureData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Experiment'] = fields.mExperiment
            row['Structure'] = fields.mStructure
            row['StartTime'] = fields.mStartTime
            row['Status'] = fields.mStatus
            row.append()
        self.tblExperimentStructureData.flush()

    ## Gets the HDF5 table row of an TransducerCycleData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see TransducerCycleDataProps
    def getTransducerCycleData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblTransducerCycleData.where('id==%d' % id):
            return row
        else:
            return

    ## Adds the fields of an TransducerCycleData object.
    #
    # The ID field of the given TransducerCycleData properties must be set to
    # the SQL object ID.
    #
    # @param fields TransducerCycleData object fields
    # @see getTransducerCycleData()
    def addTransducerCycleData(self, fields):
        row = self.tblTransducerCycleData.row
        row['id'] = fields.id
        row['Transducer'] = fields.mTransducer
        row['Cycle'] = fields.mCycle
        row['SoftwareScaling'] = fields.mSoftwareScaling
        row.append()
        self.tblTransducerCycleData.flush()

    ## Adds a TransducerCycleData set.
    #
    # The ID field of each TransducerCycleData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of TransducerCycleDataFields objects
    # @see TransducerCycleDataProps
    # @see getTransducerCycleData()
    def addTransducerCycleDataList(self, fieldsList):
        row = self.tblTransducerCycleData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Transducer'] = fields.mTransducer
            row['Cycle'] = fields.mCycle
            row['SoftwareScaling'] = fields.mSoftwareScaling
            row.append()
        self.tblTransducerCycleData.flush()

    ## Gets the HDF5 table row of an MappingMatrix object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see MappingMatrixProps
    def getMappingMatrix(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblMappingMatrix.where('id==%d' % id):
            return row
        else:
            return

    ## Gets the MappingMatrix array.
    #
    # @param id  MappingMatrix object ID
    # @return array HDF5 node
    def getMappingMatrixArray(self, id):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            # No data has been set yet
            return None
        return self.h5.getNode(grp, 'data')

    ## Gets the MappingMatrix OutputNode axis by MappingMatrix id.
    #
    # @param id ID of existing MappingMatrix object
    # @return OutputNode axis HDF5 node
    def getMappingMatrixOutputNodeAxis(self, id):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'OutputNodeAxis'):
            # No axis data has been set yet
            return None
        # Get array axes
        return grp.OutputNodeAxis

    ## Gets the MappingMatrix InputNode axis by MappingMatrix id.
    #
    # @param id ID of existing MappingMatrix object
    # @return InputNode axis HDF5 node
    def getMappingMatrixInputNodeAxis(self, id):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'InputNodeAxis'):
            # No axis data has been set yet
            return None
        # Get array axes
        return grp.InputNodeAxis

    ## Gets the array data.
    #
    # @param id array object id
    # @return signal data HDF5 node
    def getMappingMatrixArrayData(self, id):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            return None
        return grp.data

    ## Sets the MappingMatrix OutputNode axis data by MappingMatrix id.
    #
    # @param id ID of existing MappingMatrix object
    # @param data axis data
    def setMappingMatrixOutputNodeAxis(self, id, data):
        # Check for existence of data and compatible axis length
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            raise RuntimeError('Data must be created before setting axis')
        array = self.h5.getNode(grp, 'data')
        dim = array.shape[0]
        if dim != len(data):
            raise ValueError('Axis length must be %d' % dim)
        # Set or update axis
        if nodeExists(grp, 'OutputNodeAxis'):
            # Delete existing array data
            self.h5.removeNode(grp, 'OutputNodeAxis')
        # Store array data
        a = self.h5.createArray(grp, 'OutputNodeAxis', data, 'OutputNode axis')
        a.flush()

    ## Sets the MappingMatrix InputNode axis data by MappingMatrix id.
    #
    # @param id ID of existing MappingMatrix object
    # @param data axis data
    def setMappingMatrixInputNodeAxis(self, id, data):
        # Check for existence of data and compatible axis length
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            raise RuntimeError('Data must be created before setting axis')
        array = self.h5.getNode(grp, 'data')
        dim = array.shape[1]
        if dim != len(data):
            raise ValueError('Axis length must be %d' % dim)
        # Set or update axis
        if nodeExists(grp, 'InputNodeAxis'):
            # Delete existing array data
            self.h5.removeNode(grp, 'InputNodeAxis')
        # Store array data
        a = self.h5.createArray(grp, 'InputNodeAxis', data, 'InputNode axis')
        a.flush()

    ## Sets the MappingMatrix array dimensions.
    #
    # Note that this deletes any existing array data.
    #
    # @param id  MappingMatrix object ID
    # @param dims array dimensions
    def createMappingMatrixArray(self, id, dims):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        if nodeExists(grp, 'data'):
            # Delete existing array data
            self.h5.removeNode(grp, 'data')
        # Create new array
        atom = Float64Atom()
        shape = tuple(dims)
        a = self.h5.createCArray(grp, 'data', atom, shape)
        a.flush()

    ## Sets the MappingMatrix array data of the i'th dimension, at the
    # given indices.
    #
    # @param id      MappingMatrix object ID
    # @param slices  list of slices for each dimension (empty is all)
    # @param data    array data
    def setMappingMatrixArrayData(self, id, slices, data):
        grp = self.h5.getNode(self.grpMappingMatrix, 'id%d' % id)
        array = self.h5.getNode(grp, 'data')
        dataSlices = [Ellipsis]*len(array.shape)
        dataSlices[0:len(slices)] = slices
        for i, slc in enumerate(dataSlices):
            if slc == Ellipsis:
                # PyTables supports at most one Ellipsis
                dataSlices[i] = slice(0,array.shape[i])
        array.__setitem__(tuple(dataSlices), data)
        array.flush()

    ## Adds the fields of an MappingMatrix object.
    #
    # The ID field of the given MappingMatrix properties must be set to
    # the SQL object ID.
    #
    # @param fields MappingMatrix object fields
    # @see getMappingMatrix()
    def addMappingMatrix(self, fields):
        row = self.tblMappingMatrix.row
        row['id'] = fields.id
        row['Structure'] = fields.mStructure
        row['Name'] = fields.mName
        row['Description'] = fields.mDescription
        row['OutputQuantity'] = self.QuantityEnumList.index(str(fields.mOutputQuantity))
        row['InputQuantity'] = self.QuantityEnumList.index(str(fields.mInputQuantity))
        row.append()
        self.tblMappingMatrix.flush()
        # Create group to hold arrays-specific properties
        grp = self.h5.createGroup(self.grpMappingMatrix, 'id%d' % fields.id)

    ## Adds a MappingMatrix set.
    #
    # The ID field of each MappingMatrix property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of MappingMatrixFields objects
    # @see MappingMatrixProps
    # @see getMappingMatrix()
    def addMappingMatrixList(self, fieldsList):
        row = self.tblMappingMatrix.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Structure'] = fields.mStructure
            row['Name'] = fields.mName
            row['Description'] = fields.mDescription
            row['OutputQuantity'] = fields.mOutputQuantity
            row['InputQuantity'] = fields.mInputQuantity
            row.append()
        self.tblMappingMatrix.flush()

    ## Gets the HDF5 table row of an DaqUnitChannelData object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see DaqUnitChannelDataProps
    def getDaqUnitChannelData(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblDaqUnitChannelData.where('id==%d' % id):
            return row
        else:
            return

    ## Gets the DaqUnitChannelData array.
    #
    # @param id  DaqUnitChannelData object ID
    # @return array HDF5 node
    def getDaqUnitChannelDataArray(self, id):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            # No data has been set yet
            return None
        return self.h5.getNode(grp, 'data')

    ## Gets the DaqUnitChannelData SubChannels axis by DaqUnitChannelData id.
    #
    # @param id ID of existing DaqUnitChannelData object
    # @return SubChannels axis HDF5 node
    def getDaqUnitChannelDataSubChannelsAxis(self, id):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if not nodeExists(grp, 'SubChannelsAxis'):
            # No axis data has been set yet
            return None
        # Get array axes
        return grp.SubChannelsAxis

    ## Gets the signal data.
    #
    # @param id signal object id
    # @return signal data HDF5 node
    def getDaqUnitChannelDataSignalData(self, id):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            return None
        return grp.data

    ## Gets the signal time axis.
    #
    # @param id signal object id
    # @return signal time axis HDF5 node
    def getDaqUnitChannelDataSignalTimeAxis(self, id):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if not nodeExists(grp, 'time'):
            return None
        return grp.time

    ## Sets the DaqUnitChannelData SubChannels axis data by DaqUnitChannelData id.
    #
    # @param id ID of existing DaqUnitChannelData object
    # @param data axis data
    def setDaqUnitChannelDataSubChannelsAxis(self, id, data):
        # Check for existence of data and compatible axis length
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            raise RuntimeError('Data must be created before setting axis')
        array = self.h5.getNode(grp, 'data')
        dim = array.shape[1]
        if dim != len(data):
            raise ValueError('Axis length must be %d' % dim)
        # Set or update axis
        if nodeExists(grp, 'SubChannelsAxis'):
            # Delete existing array data
            self.h5.removeNode(grp, 'SubChannelsAxis')
        # Convert enumeration references to indices
        data = [self.QuantityEnumList.index(str(x)) for x in data]
        # Store array data
        a = self.h5.createArray(grp, 'SubChannelsAxis', data, 'SubChannels axis')
        a.flush()

    ## Sets the DaqUnitChannelData signal array dimensions.
    #
    # Note that the array size reflects the size of each axis, not
    # including the time axis.  The signal will grow along the time
    # axis as arrays are added.
    #
    # This method must be called before signal arrays can be added.
    # Any existing data is removed without warning.
    #
    # @param id   DaqUnitChannelData object ID
    # @param dims signal array dimensions
    def createDaqUnitChannelDataSignal(self, id, dims):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        if nodeExists(grp, 'data'):
            # Delete existing array data
            self.h5.removeNode(grp, 'data')
        if nodeExists(grp, 'time'):
            # Delete existing time data
            self.h5.removeNode(grp, 'time')
        # Create new signal data array
        atom = Int32Atom()
        shape = tuple([0] + dims)
        a = self.h5.createEArray(grp, 'data', atom, shape)
        a.flush()
        # Create new signal time array
        atom = Time64Atom()
        shape = (0,)
        a = self.h5.createEArray(grp, 'time', atom, shape)
        a.flush()

    ## Appends a data point to the DaqUnitChannelData signal.
    #
    # The data must be supplied as a vector, which is interpreted as a
    # multi-dimensional array.
    #
    # @param id  DaqUnitChannelData object ID
    # @param t list of timestamps
    # @param data vector data
    def appendToDaqUnitChannelDataSignal(self, id, t, data):
        grp = self.h5.getNode(self.grpDaqUnitChannelData, 'id%d' % id)
        # Add signal data
        array = self.h5.getNode(grp, 'data')
        data = numpy.array(data)
        data = data.reshape(tuple([len(t)] + list(array.shape[1:])))
        array.append(data)
        array.flush()
        # Add time data
        array = self.h5.getNode(grp, 'time')
        array.append(t)
        array.flush()

    ## Adds the fields of an DaqUnitChannelData object.
    #
    # The ID field of the given DaqUnitChannelData properties must be set to
    # the SQL object ID.
    #
    # @param fields DaqUnitChannelData object fields
    # @see getDaqUnitChannelData()
    def addDaqUnitChannelData(self, fields):
        row = self.tblDaqUnitChannelData.row
        row['id'] = fields.id
        row['Channel'] = fields.mChannel
        row.append()
        self.tblDaqUnitChannelData.flush()
        # Create group to hold signal-specific properties
        grp = self.h5.createGroup(self.grpDaqUnitChannelData, 'id%d' % fields.id)

    ## Adds a DaqUnitChannelData set.
    #
    # The ID field of each DaqUnitChannelData property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of DaqUnitChannelDataFields objects
    # @see DaqUnitChannelDataProps
    # @see getDaqUnitChannelData()
    def addDaqUnitChannelDataList(self, fieldsList):
        row = self.tblDaqUnitChannelData.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Channel'] = fields.mChannel
            row.append()
        self.tblDaqUnitChannelData.flush()

    ## Gets the HDF5 table row of an AccelMeas object.
    #
    # The table row holds the regular object members.
    #
    # @param id object id
    # @return corresponding HDF5 table row
    # @see AccelMeasProps
    def getAccelMeas(self, id):
        # TODO: smarter and faster lookup?
        for row in self.tblAccelMeas.where('id==%d' % id):
            return row
        else:
            return

    ## Gets the AccelMeas array.
    #
    # @param id  AccelMeas object ID
    # @return array HDF5 node
    def getAccelMeasArray(self, id):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            # No data has been set yet
            return None
        return self.h5.getNode(grp, 'data')

    ## Gets the AccelMeas Direction axis by AccelMeas id.
    #
    # @param id ID of existing AccelMeas object
    # @return Direction axis HDF5 node
    def getAccelMeasDirectionAxis(self, id):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if not nodeExists(grp, 'DirectionAxis'):
            # No axis data has been set yet
            return None
        # Get array axes
        return grp.DirectionAxis

    ## Gets the signal data.
    #
    # @param id signal object id
    # @return signal data HDF5 node
    def getAccelMeasSignalData(self, id):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            return None
        return grp.data

    ## Gets the signal time axis.
    #
    # @param id signal object id
    # @return signal time axis HDF5 node
    def getAccelMeasSignalTimeAxis(self, id):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if not nodeExists(grp, 'time'):
            return None
        return grp.time

    ## Sets the AccelMeas Direction axis data by AccelMeas id.
    #
    # @param id ID of existing AccelMeas object
    # @param data axis data
    def setAccelMeasDirectionAxis(self, id, data):
        # Check for existence of data and compatible axis length
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if not nodeExists(grp, 'data'):
            raise RuntimeError('Data must be created before setting axis')
        array = self.h5.getNode(grp, 'data')
        dim = array.shape[1]
        if dim != len(data):
            raise ValueError('Axis length must be %d' % dim)
        # Set or update axis
        if nodeExists(grp, 'DirectionAxis'):
            # Delete existing array data
            self.h5.removeNode(grp, 'DirectionAxis')
        # Convert enumeration references to indices
        data = [self.QuantityEnumList.index(str(x)) for x in data]
        # Store array data
        a = self.h5.createArray(grp, 'DirectionAxis', data, 'Direction axis')
        a.flush()

    ## Sets the AccelMeas signal array dimensions.
    #
    # Note that the array size reflects the size of each axis, not
    # including the time axis.  The signal will grow along the time
    # axis as arrays are added.
    #
    # This method must be called before signal arrays can be added.
    # Any existing data is removed without warning.
    #
    # @param id   AccelMeas object ID
    # @param dims signal array dimensions
    def createAccelMeasSignal(self, id, dims):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        if nodeExists(grp, 'data'):
            # Delete existing array data
            self.h5.removeNode(grp, 'data')
        if nodeExists(grp, 'time'):
            # Delete existing time data
            self.h5.removeNode(grp, 'time')
        # Create new signal data array
        atom = Float32Atom()
        shape = tuple([0] + dims)
        a = self.h5.createEArray(grp, 'data', atom, shape)
        a.flush()
        # Create new signal time array
        atom = Time64Atom()
        shape = (0,)
        a = self.h5.createEArray(grp, 'time', atom, shape)
        a.flush()

    ## Appends a data point to the AccelMeas signal.
    #
    # The data must be supplied as a vector, which is interpreted as a
    # multi-dimensional array.
    #
    # @param id  AccelMeas object ID
    # @param t list of timestamps
    # @param data vector data
    def appendToAccelMeasSignal(self, id, t, data):
        grp = self.h5.getNode(self.grpAccelMeas, 'id%d' % id)
        # Add signal data
        array = self.h5.getNode(grp, 'data')
        data = numpy.array(data)
        data = data.reshape(tuple([len(t)] + list(array.shape[1:])))
        array.append(data)
        array.flush()
        # Add time data
        array = self.h5.getNode(grp, 'time')
        array.append(t)
        array.flush()

    ## Adds the fields of an AccelMeas object.
    #
    # The ID field of the given AccelMeas properties must be set to
    # the SQL object ID.
    #
    # @param fields AccelMeas object fields
    # @see getAccelMeas()
    def addAccelMeas(self, fields):
        row = self.tblAccelMeas.row
        row['id'] = fields.id
        row['Sensor'] = fields.mSensor
        row['Name'] = fields.mName
        row['Description'] = fields.mDescription
        row.append()
        self.tblAccelMeas.flush()
        # Create group to hold signal-specific properties
        grp = self.h5.createGroup(self.grpAccelMeas, 'id%d' % fields.id)

    ## Adds a AccelMeas set.
    #
    # The ID field of each AccelMeas property set must be set to the
    # SQL object IDs.
    #
    # @param fieldsList list of AccelMeasFields objects
    # @see AccelMeasProps
    # @see getAccelMeas()
    def addAccelMeasList(self, fieldsList):
        row = self.tblAccelMeas.row
        for fields in fieldsList:
            row[name] = fields.id
            row['Sensor'] = fields.mSensor
            row['Name'] = fields.mName
            row['Description'] = fields.mDescription
            row.append()
        self.tblAccelMeas.flush()

    ## Closes the HDF5 file.
    def close(self):
        self.h5.close()

## Creates a full backup of a database.
#
# Note that the server software version <b>must</b> match the version
# of this software, to avoid problems with enumeration IDs, class
# definition mismatches, etc.
#
# @param config    client configuration
# @param filename  HDF5 backup file name
def backup(config, filename):
    f = BackupFile(filename, 'w')
    # Connect to server with an exclusive access session
    # (this blocks the server for other users!)
    client = SenStoreClient(config, 'SenStore Backup client', exclusive=True)
    mngr = client.getManager()
    logging.info('Connected to SenStore')
    ids = mngr.getStructureIds()
    n = len(ids)
    logging.info('Backing up %d Structure object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getStructureFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addStructure(fields)
        i += d
    ids = mngr.getFEMDofIds()
    n = len(ids)
    logging.info('Backing up %d FEMDof object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMDofFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMDof(fields)
        i += d
    ids = mngr.getFEMNodalMassIds()
    n = len(ids)
    logging.info('Backing up %d FEMNodalMass object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMNodalMassFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMNodalMass(fields)
        i += d
    ids = mngr.getFEMNLElasticStrainStressIds()
    n = len(ids)
    logging.info('Backing up %d FEMNLElasticStrainStress object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMNLElasticStrainStressFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMNLElasticStrainStress(fields)
        i += d
    ids = mngr.getFEMBoundaryIds()
    n = len(ids)
    logging.info('Backing up %d FEMBoundary object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMBoundaryFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMBoundary(fields)
        i += d
    ids = mngr.getFEMSectionPipeIds()
    n = len(ids)
    logging.info('Backing up %d FEMSectionPipe object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSectionPipeFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSectionPipe(fields)
        i += d
    ids = mngr.getFEMCoordSystemIds()
    n = len(ids)
    logging.info('Backing up %d FEMCoordSystem object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMCoordSystemFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMCoordSystem(fields)
        i += d
    ids = mngr.getFEMNodeIds()
    n = len(ids)
    logging.info('Backing up %d FEMNode object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMNodeFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMNode(fields)
        i += d
    ids = mngr.getFEMTrussIds()
    n = len(ids)
    logging.info('Backing up %d FEMTruss object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMTrussFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMTruss(fields)
        i += d
    ids = mngr.getFEMTimeFunctionDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMTimeFunctionData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMTimeFunctionDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMTimeFunctionData(fields)
        i += d
    ids = mngr.getFEMPlasticMlMaterialsIds()
    n = len(ids)
    logging.info('Backing up %d FEMPlasticMlMaterials object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMPlasticMlMaterialsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMPlasticMlMaterials(fields)
        i += d
    ids = mngr.getFEMPlateGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMPlateGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMPlateGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMPlateGroup(fields)
        i += d
    ids = mngr.getFEMBeamIds()
    n = len(ids)
    logging.info('Backing up %d FEMBeam object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMBeamFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMBeam(fields)
        i += d
    ids = mngr.getFEMCurvMomentDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMCurvMomentData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMCurvMomentDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMCurvMomentData(fields)
        i += d
    ids = mngr.getFEMPropertysetsIds()
    n = len(ids)
    logging.info('Backing up %d FEMPropertysets object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMPropertysetsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMPropertysets(fields)
        i += d
    ids = mngr.getFEMOrthotropicMaterialIds()
    n = len(ids)
    logging.info('Backing up %d FEMOrthotropicMaterial object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMOrthotropicMaterialFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMOrthotropicMaterial(fields)
        i += d
    ids = mngr.getFEMAppliedLoadsIds()
    n = len(ids)
    logging.info('Backing up %d FEMAppliedLoads object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMAppliedLoadsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMAppliedLoads(fields)
        i += d
    ids = mngr.getFEMThermoOrthDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMThermoOrthData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMThermoOrthDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMThermoOrthData(fields)
        i += d
    ids = mngr.getFEMContactPairsIds()
    n = len(ids)
    logging.info('Backing up %d FEMContactPairs object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMContactPairsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMContactPairs(fields)
        i += d
    ids = mngr.getFEMGeneralIds()
    n = len(ids)
    logging.info('Backing up %d FEMGeneral object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMGeneralFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMGeneral(fields)
        i += d
    ids = mngr.getFEMBeamNodeIds()
    n = len(ids)
    logging.info('Backing up %d FEMBeamNode object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMBeamNodeFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMBeamNode(fields)
        i += d
    ids = mngr.getFEMSectionRectIds()
    n = len(ids)
    logging.info('Backing up %d FEMSectionRect object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSectionRectFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSectionRect(fields)
        i += d
    ids = mngr.getFEMBeamLoadIds()
    n = len(ids)
    logging.info('Backing up %d FEMBeamLoad object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMBeamLoadFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMBeamLoad(fields)
        i += d
    ids = mngr.getFEMLoadMassProportionalIds()
    n = len(ids)
    logging.info('Backing up %d FEMLoadMassProportional object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMLoadMassProportionalFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMLoadMassProportional(fields)
        i += d
    ids = mngr.getFEMLinkIds()
    n = len(ids)
    logging.info('Backing up %d FEMLink object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMLinkFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMLink(fields)
        i += d
    ids = mngr.getFEMAxesNodeIds()
    n = len(ids)
    logging.info('Backing up %d FEMAxesNode object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMAxesNodeFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMAxesNode(fields)
        i += d
    ids = mngr.getFEMNMTimeMassIds()
    n = len(ids)
    logging.info('Backing up %d FEMNMTimeMass object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMNMTimeMassFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMNMTimeMass(fields)
        i += d
    ids = mngr.getFEMAppliedDisplacementIds()
    n = len(ids)
    logging.info('Backing up %d FEMAppliedDisplacement object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMAppliedDisplacementFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMAppliedDisplacement(fields)
        i += d
    ids = mngr.getFEMTimeFunctionsIds()
    n = len(ids)
    logging.info('Backing up %d FEMTimeFunctions object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMTimeFunctionsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMTimeFunctions(fields)
        i += d
    ids = mngr.getFEMForceStrainDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMForceStrainData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMForceStrainDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMForceStrainData(fields)
        i += d
    ids = mngr.getFEMSkewDOFIds()
    n = len(ids)
    logging.info('Backing up %d FEMSkewDOF object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSkewDOFFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSkewDOF(fields)
        i += d
    ids = mngr.getFEMSectionIIds()
    n = len(ids)
    logging.info('Backing up %d FEMSectionI object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSectionIFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSectionI(fields)
        i += d
    ids = mngr.getFEMPlasticBilinearMaterialIds()
    n = len(ids)
    logging.info('Backing up %d FEMPlasticBilinearMaterial object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMPlasticBilinearMaterialFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMPlasticBilinearMaterial(fields)
        i += d
    ids = mngr.getFEMMTForceDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMMTForceData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMMTForceDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMMTForceData(fields)
        i += d
    ids = mngr.getFEMShellPressureIds()
    n = len(ids)
    logging.info('Backing up %d FEMShellPressure object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMShellPressureFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMShellPressure(fields)
        i += d
    ids = mngr.getFEMMatricesIds()
    n = len(ids)
    logging.info('Backing up %d FEMMatrices object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMMatricesFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMMatrices(fields)
        i += d
    ids = mngr.getFEMDampingIds()
    n = len(ids)
    logging.info('Backing up %d FEMDamping object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMDampingFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMDamping(fields)
        i += d
    ids = mngr.getFEMMaterialIds()
    n = len(ids)
    logging.info('Backing up %d FEMMaterial object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMMaterialFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMMaterial(fields)
        i += d
    ids = mngr.getFEMMatrixDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMMatrixData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMMatrixDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMMatrixData(fields)
        i += d
    ids = mngr.getFEMShellAxesOrthoIds()
    n = len(ids)
    logging.info('Backing up %d FEMShellAxesOrtho object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMShellAxesOrthoFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMShellAxesOrtho(fields)
        i += d
    ids = mngr.getFEMEndreleasesIds()
    n = len(ids)
    logging.info('Backing up %d FEMEndreleases object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMEndreleasesFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMEndreleases(fields)
        i += d
    ids = mngr.getFEMTrussGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMTrussGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMTrussGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMTrussGroup(fields)
        i += d
    ids = mngr.getFEMInitialTemperatureIds()
    n = len(ids)
    logging.info('Backing up %d FEMInitialTemperature object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMInitialTemperatureFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMInitialTemperature(fields)
        i += d
    ids = mngr.getFEMThermoIsoMaterialsIds()
    n = len(ids)
    logging.info('Backing up %d FEMThermoIsoMaterials object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMThermoIsoMaterialsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMThermoIsoMaterials(fields)
        i += d
    ids = mngr.getFEMThermoIsoDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMThermoIsoData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMThermoIsoDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMThermoIsoData(fields)
        i += d
    ids = mngr.getFEMContactGroup3Ids()
    n = len(ids)
    logging.info('Backing up %d FEMContactGroup3 object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMContactGroup3FieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMContactGroup3(fields)
        i += d
    ids = mngr.getFEMNLElasticMaterialsIds()
    n = len(ids)
    logging.info('Backing up %d FEMNLElasticMaterials object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMNLElasticMaterialsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMNLElasticMaterials(fields)
        i += d
    ids = mngr.getFEMPlateIds()
    n = len(ids)
    logging.info('Backing up %d FEMPlate object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMPlateFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMPlate(fields)
        i += d
    ids = mngr.getFEMIsoBeamIds()
    n = len(ids)
    logging.info('Backing up %d FEMIsoBeam object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMIsoBeamFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMIsoBeam(fields)
        i += d
    ids = mngr.getFEMAppliedConcentratedLoadIds()
    n = len(ids)
    logging.info('Backing up %d FEMAppliedConcentratedLoad object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMAppliedConcentratedLoadFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMAppliedConcentratedLoad(fields)
        i += d
    ids = mngr.getFEMTwoDSolidGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMTwoDSolidGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMTwoDSolidGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMTwoDSolidGroup(fields)
        i += d
    ids = mngr.getFEMGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMGroup(fields)
        i += d
    ids = mngr.getFEMPropertiesIds()
    n = len(ids)
    logging.info('Backing up %d FEMProperties object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMPropertiesFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMProperties(fields)
        i += d
    ids = mngr.getFEMThreeDSolidGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMThreeDSolidGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMThreeDSolidGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMThreeDSolidGroup(fields)
        i += d
    ids = mngr.getFEMThreeDSolidIds()
    n = len(ids)
    logging.info('Backing up %d FEMThreeDSolid object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMThreeDSolidFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMThreeDSolid(fields)
        i += d
    ids = mngr.getFEMSectionPropIds()
    n = len(ids)
    logging.info('Backing up %d FEMSectionProp object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSectionPropFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSectionProp(fields)
        i += d
    ids = mngr.getFEMElasticMaterialIds()
    n = len(ids)
    logging.info('Backing up %d FEMElasticMaterial object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMElasticMaterialFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMElasticMaterial(fields)
        i += d
    ids = mngr.getFEMPointsIds()
    n = len(ids)
    logging.info('Backing up %d FEMPoints object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMPointsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMPoints(fields)
        i += d
    ids = mngr.getFEMThermoOrthMaterialsIds()
    n = len(ids)
    logging.info('Backing up %d FEMThermoOrthMaterials object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMThermoOrthMaterialsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMThermoOrthMaterials(fields)
        i += d
    ids = mngr.getFEMConstraintsIds()
    n = len(ids)
    logging.info('Backing up %d FEMConstraints object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMConstraintsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMConstraints(fields)
        i += d
    ids = mngr.getFEMMCrigiditiesIds()
    n = len(ids)
    logging.info('Backing up %d FEMMCrigidities object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMMCrigiditiesFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMMCrigidities(fields)
        i += d
    ids = mngr.getFEMSkeySysNodeIds()
    n = len(ids)
    logging.info('Backing up %d FEMSkeySysNode object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSkeySysNodeFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSkeySysNode(fields)
        i += d
    ids = mngr.getFEMIsoBeamGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMIsoBeamGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMIsoBeamGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMIsoBeamGroup(fields)
        i += d
    ids = mngr.getFEMShellDOFIds()
    n = len(ids)
    logging.info('Backing up %d FEMShellDOF object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMShellDOFFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMShellDOF(fields)
        i += d
    ids = mngr.getFEMCrossSectionsIds()
    n = len(ids)
    logging.info('Backing up %d FEMCrossSections object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMCrossSectionsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMCrossSections(fields)
        i += d
    ids = mngr.getFEMTwistMomentDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMTwistMomentData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMTwistMomentDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMTwistMomentData(fields)
        i += d
    ids = mngr.getFEMShellIds()
    n = len(ids)
    logging.info('Backing up %d FEMShell object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMShellFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMShell(fields)
        i += d
    ids = mngr.getFEMNTNContactIds()
    n = len(ids)
    logging.info('Backing up %d FEMNTNContact object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMNTNContactFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMNTNContact(fields)
        i += d
    ids = mngr.getFEMShellLayerIds()
    n = len(ids)
    logging.info('Backing up %d FEMShellLayer object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMShellLayerFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMShellLayer(fields)
        i += d
    ids = mngr.getFEMSkewSysAnglesIds()
    n = len(ids)
    logging.info('Backing up %d FEMSkewSysAngles object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSkewSysAnglesFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSkewSysAngles(fields)
        i += d
    ids = mngr.getFEMGroundMotionRecordIds()
    n = len(ids)
    logging.info('Backing up %d FEMGroundMotionRecord object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMGroundMotionRecordFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMGroundMotionRecord(fields)
        i += d
    ids = mngr.getFEMGeneralGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMGeneralGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMGeneralGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMGeneralGroup(fields)
        i += d
    ids = mngr.getFEMTwoDSolidIds()
    n = len(ids)
    logging.info('Backing up %d FEMTwoDSolid object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMTwoDSolidFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMTwoDSolid(fields)
        i += d
    ids = mngr.getFEMAppliedTemperatureIds()
    n = len(ids)
    logging.info('Backing up %d FEMAppliedTemperature object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMAppliedTemperatureFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMAppliedTemperature(fields)
        i += d
    ids = mngr.getFEMMatrixSetsIds()
    n = len(ids)
    logging.info('Backing up %d FEMMatrixSets object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMMatrixSetsFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMMatrixSets(fields)
        i += d
    ids = mngr.getFEMConstraintCoefIds()
    n = len(ids)
    logging.info('Backing up %d FEMConstraintCoef object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMConstraintCoefFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMConstraintCoef(fields)
        i += d
    ids = mngr.getFEMSectionBoxIds()
    n = len(ids)
    logging.info('Backing up %d FEMSectionBox object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSectionBoxFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSectionBox(fields)
        i += d
    ids = mngr.getFEMNKDisplForceIds()
    n = len(ids)
    logging.info('Backing up %d FEMNKDisplForce object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMNKDisplForceFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMNKDisplForce(fields)
        i += d
    ids = mngr.getFEMPlasticStrainStressIds()
    n = len(ids)
    logging.info('Backing up %d FEMPlasticStrainStress object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMPlasticStrainStressFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMPlasticStrainStress(fields)
        i += d
    ids = mngr.getFEMShellAxesOrthoDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMShellAxesOrthoData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMShellAxesOrthoDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMShellAxesOrthoData(fields)
        i += d
    ids = mngr.getFEMGeneralNodeIds()
    n = len(ids)
    logging.info('Backing up %d FEMGeneralNode object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMGeneralNodeFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMGeneralNode(fields)
        i += d
    ids = mngr.getFEMStrLinesIds()
    n = len(ids)
    logging.info('Backing up %d FEMStrLines object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMStrLinesFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMStrLines(fields)
        i += d
    ids = mngr.getFEMContactSurfaceIds()
    n = len(ids)
    logging.info('Backing up %d FEMContactSurface object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMContactSurfaceFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMContactSurface(fields)
        i += d
    ids = mngr.getFEMMCForceDataIds()
    n = len(ids)
    logging.info('Backing up %d FEMMCForceData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMMCForceDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMMCForceData(fields)
        i += d
    ids = mngr.getFEMSpringIds()
    n = len(ids)
    logging.info('Backing up %d FEMSpring object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSpringFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSpring(fields)
        i += d
    ids = mngr.getFEMSpringGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMSpringGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMSpringGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMSpringGroup(fields)
        i += d
    ids = mngr.getFEMShellGroupIds()
    n = len(ids)
    logging.info('Backing up %d FEMShellGroup object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getFEMShellGroupFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addFEMShellGroup(fields)
        i += d
    ids = mngr.getDaqUnitIds()
    n = len(ids)
    logging.info('Backing up %d DaqUnit object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getDaqUnitFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addDaqUnit(fields)
        i += d
    ids = mngr.getDaqUnitChannelIds()
    n = len(ids)
    logging.info('Backing up %d DaqUnitChannel object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getDaqUnitChannelFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addDaqUnitChannel(fields)
        i += d
    ids = mngr.getSensorIds()
    n = len(ids)
    logging.info('Backing up %d Sensor object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getSensorFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addSensor(fields)
        i += d
    ids = mngr.getTransducerIds()
    n = len(ids)
    logging.info('Backing up %d Transducer object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getTransducerFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addTransducer(fields)
        i += d
    ids = mngr.getExperimentIds()
    n = len(ids)
    logging.info('Backing up %d Experiment object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getExperimentFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addExperiment(fields)
        i += d
    ids = mngr.getCycleIds()
    n = len(ids)
    logging.info('Backing up %d Cycle object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getCycleFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addCycle(fields)
        i += d
    ids = mngr.getSensorTimeDataIds()
    n = len(ids)
    logging.info('Backing up %d SensorTimeData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getSensorTimeDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addSensorTimeData(fields)
        i += d
    ids = mngr.getTransducerTimeDataIds()
    n = len(ids)
    logging.info('Backing up %d TransducerTimeData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getTransducerTimeDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addTransducerTimeData(fields)
        i += d
    ids = mngr.getExperimentStructureDataIds()
    n = len(ids)
    logging.info('Backing up %d ExperimentStructureData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getExperimentStructureDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addExperimentStructureData(fields)
        i += d
    ids = mngr.getTransducerCycleDataIds()
    n = len(ids)
    logging.info('Backing up %d TransducerCycleData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getTransducerCycleDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addTransducerCycleData(fields)
        i += d
    ids = mngr.getMappingMatrixIds()
    n = len(ids)
    logging.info('Backing up %d MappingMatrix object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getMappingMatrixFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addMappingMatrix(fields)
            # Get proxy
            id = ids[i+j]
            prx = mngr.getMappingMatrix(id)
            # Create array data
            dims = prx.getArraySize()
            if len(dims)==0:
                logging.info('Skipping MappingMatrix[oid=%d] array data (empty)' % id)
                continue
            f.createMappingMatrixArray(id, dims)
            logging.info('Backing up MappingMatrix[oid=%d] array data' % id)
            for slices in slicing.slicer(dims, CHUNK_ELEM_COUNT):
                # Create slice array for proxy call
                prxSlices = slicing.slice2db(SenStore, slices)
                result = prx.getArrayData(prxSlices)
                data = numpy.array(result.data)
                data = data.reshape(result.shape)
                f.setMappingMatrixArrayData(id, slices, data)
            # Copy main axes
            axis = prx.getOutputNodeAxis()
            f.setMappingMatrixOutputNodeAxis(id, axis)
            axis = prx.getInputNodeAxis()
            f.setMappingMatrixInputNodeAxis(id, axis)
        i += d
    ids = mngr.getDaqUnitChannelDataIds()
    n = len(ids)
    logging.info('Backing up %d DaqUnitChannelData object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getDaqUnitChannelDataFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addDaqUnitChannelData(fields)
            # Get proxy
            id = ids[i+j]
            prx = mngr.getDaqUnitChannelData(id)
            logging.info('Backing up DaqUnitChannelData[oid=%d] signal data' % id)
            # Create and copy signal data
            dims = prx.getSignalSize()
            f.createDaqUnitChannelDataSignal(id, dims[1:])
            idxStart = 0
            idxStep = 1000
            while idxStart < dims[0]:
                idxEnd = idxStart + idxStep
                if idxEnd > dims[0]:
                    idxEnd = dims[0]+1
                result = prx.getSignalDataByIndexRange(idxStart, 1, idxEnd, [])
                t = numpy.array(result.t)
                data = numpy.array(result.data)
                f.appendToDaqUnitChannelDataSignal(id, t, data)
                idxStart += idxStep
            # Copy main axes
            axis = prx.getSubChannelsAxis()
            f.setDaqUnitChannelDataSubChannelsAxis(id, axis)
        i += d
    ids = mngr.getAccelMeasIds()
    n = len(ids)
    logging.info('Backing up %d AccelMeas object(s)' % n)
    i = 0
    d = 100
    # Process in batches
    while i < n:
        # Get object proxies
        fieldsList = mngr.getAccelMeasFieldsList(ids[i:i+d])
        for j,fields in enumerate(fieldsList):
            # Copy regular fields
            f.addAccelMeas(fields)
            # Get proxy
            id = ids[i+j]
            prx = mngr.getAccelMeas(id)
            logging.info('Backing up AccelMeas[oid=%d] signal data' % id)
            # Create and copy signal data
            dims = prx.getSignalSize()
            f.createAccelMeasSignal(id, dims[1:])
            idxStart = 0
            idxStep = 1000
            while idxStart < dims[0]:
                idxEnd = idxStart + idxStep
                if idxEnd > dims[0]:
                    idxEnd = dims[0]+1
                result = prx.getSignalDataByIndexRange(idxStart, 1, idxEnd, [])
                t = numpy.array(result.t)
                data = numpy.array(result.data)
                f.appendToAccelMeasSignal(id, t, data)
                idxStart += idxStep
            # Copy main axes
            axis = prx.getDirectionAxis()
            f.setAccelMeasDirectionAxis(id, axis)
        i += d
    # Exit cleanly, so other clients can get access right away
    client.close()
    f.close()


## Performs a full database restore from a backup file.
#
# Note that the server software version <b>must</b> match the version
# of this software, to avoid problems with enumeration IDs, class
# definition mismatches, etc.  The backup file may have been created
# with a different software version.
#
# Typically a restore should be done to an empty database, but this is
# not strictly necessary, as the object IDs are remapped.
#
# @param config    client configuration
# @param filename  HDF5 backup file name
def restore(config, filename):
    f = BackupFile(filename, 'r')
    # Connect to server with an exclusive access session
    # (this blocks the server for other users!)
    client = SenStoreClient(config, 'SenStore Restore client', exclusive=True)
    mngr = client.getManager()
    logging.info('Connected to SenStore')
    # Restore objects (ignoring object references for now)
    if not nodeExists(f.h5.root.objects, 'Structure'):
        logging.info('Skipping Structure objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.Structure
        logging.info('Restoring %d Structure object(s)' % tbl.nrows)
        id2dbid_Structure = {0:0}
        for row in tbl:
            obj = SenStore.StructureFields()
            if 'Name' in tbl.colnames:
                obj.mName = row['Name']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Type' in tbl.colnames:
                obj.mType = row['Type']
            if 'DistanceUnit' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.UnitEnumList[row['DistanceUnit']]
                obj.mDistanceUnit = SenStore.Unit.__dict__[enumLabel]
            if 'ForceUnit' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.UnitEnumList[row['ForceUnit']]
                obj.mForceUnit = SenStore.Unit.__dict__[enumLabel]
            if 'WeightUnit' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.UnitEnumList[row['WeightUnit']]
                obj.mWeightUnit = SenStore.Unit.__dict__[enumLabel]
            obj.id = mngr.addStructure(obj)
            id2dbid_Structure[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMDof'):
        logging.info('Skipping FEMDof objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMDof
        logging.info('Restoring %d FEMDof object(s)' % tbl.nrows)
        id2dbid_FEMDof = {0:0}
        for row in tbl:
            obj = SenStore.FEMDofFields()
            if 'LocalId' in tbl.colnames:
                obj.mLocalId = row['LocalId']
            if 'Direction' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.QuantityEnumList[row['Direction']]
                obj.mDirection = SenStore.Quantity.__dict__[enumLabel]
            obj.id = mngr.addFEMDof(obj)
            id2dbid_FEMDof[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMNodalMass'):
        logging.info('Skipping FEMNodalMass objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMNodalMass
        logging.info('Restoring %d FEMNodalMass object(s)' % tbl.nrows)
        id2dbid_FEMNodalMass = {0:0}
        for row in tbl:
            obj = SenStore.FEMNodalMassFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Mass1' in tbl.colnames:
                obj.mMass1 = row['Mass1']
            if 'Mass2' in tbl.colnames:
                obj.mMass2 = row['Mass2']
            if 'Mass3' in tbl.colnames:
                obj.mMass3 = row['Mass3']
            if 'Mass4' in tbl.colnames:
                obj.mMass4 = row['Mass4']
            if 'Mass5' in tbl.colnames:
                obj.mMass5 = row['Mass5']
            if 'Mass6' in tbl.colnames:
                obj.mMass6 = row['Mass6']
            obj.id = mngr.addFEMNodalMass(obj)
            id2dbid_FEMNodalMass[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMNLElasticStrainStress'):
        logging.info('Skipping FEMNLElasticStrainStress objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMNLElasticStrainStress
        logging.info('Restoring %d FEMNLElasticStrainStress object(s)' % tbl.nrows)
        id2dbid_FEMNLElasticStrainStress = {0:0}
        for row in tbl:
            obj = SenStore.FEMNLElasticStrainStressFields()
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'RecordNumber' in tbl.colnames:
                obj.mRecordNumber = row['RecordNumber']
            if 'Strain' in tbl.colnames:
                obj.mStrain = row['Strain']
            if 'Stress' in tbl.colnames:
                obj.mStress = row['Stress']
            obj.id = mngr.addFEMNLElasticStrainStress(obj)
            id2dbid_FEMNLElasticStrainStress[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMBoundary'):
        logging.info('Skipping FEMBoundary objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMBoundary
        logging.info('Restoring %d FEMBoundary object(s)' % tbl.nrows)
        id2dbid_FEMBoundary = {0:0}
        for row in tbl:
            obj = SenStore.FEMBoundaryFields()
            if 'Ovalization' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.BoundaryTypeEnumList[row['Ovalization']]
                obj.mOvalization = SenStore.BoundaryType.__dict__[enumLabel]
            if 'Phi' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.BoundaryTypeEnumList[row['Phi']]
                obj.mPhi = SenStore.BoundaryType.__dict__[enumLabel]
            if 'Rx' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.BoundaryTypeEnumList[row['Rx']]
                obj.mRx = SenStore.BoundaryType.__dict__[enumLabel]
            if 'Ry' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.BoundaryTypeEnumList[row['Ry']]
                obj.mRy = SenStore.BoundaryType.__dict__[enumLabel]
            if 'Rz' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.BoundaryTypeEnumList[row['Rz']]
                obj.mRz = SenStore.BoundaryType.__dict__[enumLabel]
            if 'Ux' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.BoundaryTypeEnumList[row['Ux']]
                obj.mUx = SenStore.BoundaryType.__dict__[enumLabel]
            if 'Uy' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.BoundaryTypeEnumList[row['Uy']]
                obj.mUy = SenStore.BoundaryType.__dict__[enumLabel]
            if 'Uz' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.BoundaryTypeEnumList[row['Uz']]
                obj.mUz = SenStore.BoundaryType.__dict__[enumLabel]
            if 'Warping' in tbl.colnames:
                obj.mWarping = row['Warping']
            obj.id = mngr.addFEMBoundary(obj)
            id2dbid_FEMBoundary[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSectionPipe'):
        logging.info('Skipping FEMSectionPipe objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSectionPipe
        logging.info('Restoring %d FEMSectionPipe object(s)' % tbl.nrows)
        id2dbid_FEMSectionPipe = {0:0}
        for row in tbl:
            obj = SenStore.FEMSectionPipeFields()
            if 'Diameter' in tbl.colnames:
                obj.mDiameter = row['Diameter']
            if 'SC' in tbl.colnames:
                obj.mSC = row['SC']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'SSarea' in tbl.colnames:
                obj.mSSarea = row['SSarea']
            if 'TC' in tbl.colnames:
                obj.mTC = row['TC']
            if 'Thickness' in tbl.colnames:
                obj.mThickness = row['Thickness']
            if 'Torfac' in tbl.colnames:
                obj.mTorfac = row['Torfac']
            if 'TSarea' in tbl.colnames:
                obj.mTSarea = row['TSarea']
            obj.id = mngr.addFEMSectionPipe(obj)
            id2dbid_FEMSectionPipe[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMCoordSystem'):
        logging.info('Skipping FEMCoordSystem objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMCoordSystem
        logging.info('Restoring %d FEMCoordSystem object(s)' % tbl.nrows)
        id2dbid_FEMCoordSystem = {0:0}
        for row in tbl:
            obj = SenStore.FEMCoordSystemFields()
            if 'AX' in tbl.colnames:
                obj.mAX = row['AX']
            if 'AY' in tbl.colnames:
                obj.mAY = row['AY']
            if 'AZ' in tbl.colnames:
                obj.mAZ = row['AZ']
            if 'BBY' in tbl.colnames:
                obj.mBBY = row['BBY']
            if 'BX' in tbl.colnames:
                obj.mBX = row['BX']
            if 'BZ' in tbl.colnames:
                obj.mBZ = row['BZ']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Mode' in tbl.colnames:
                obj.mMode = row['Mode']
            if 'P1' in tbl.colnames:
                obj.mP1 = row['P1']
            if 'P2' in tbl.colnames:
                obj.mP2 = row['P2']
            if 'P3' in tbl.colnames:
                obj.mP3 = row['P3']
            if 'Type' in tbl.colnames:
                obj.mType = row['Type']
            if 'Xorigin' in tbl.colnames:
                obj.mXorigin = row['Xorigin']
            if 'Yorigin' in tbl.colnames:
                obj.mYorigin = row['Yorigin']
            if 'Zorigin' in tbl.colnames:
                obj.mZorigin = row['Zorigin']
            if 'LocalID' in tbl.colnames:
                obj.mLocalID = row['LocalID']
            obj.id = mngr.addFEMCoordSystem(obj)
            id2dbid_FEMCoordSystem[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMNode'):
        logging.info('Skipping FEMNode objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMNode
        logging.info('Restoring %d FEMNode object(s)' % tbl.nrows)
        id2dbid_FEMNode = {0:0}
        for row in tbl:
            obj = SenStore.FEMNodeFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'X' in tbl.colnames:
                obj.mX = row['X']
            if 'Y' in tbl.colnames:
                obj.mY = row['Y']
            if 'Z' in tbl.colnames:
                obj.mZ = row['Z']
            if 'LocalID' in tbl.colnames:
                obj.mLocalID = row['LocalID']
            obj.id = mngr.addFEMNode(obj)
            id2dbid_FEMNode[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMTruss'):
        logging.info('Skipping FEMTruss objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMTruss
        logging.info('Restoring %d FEMTruss object(s)' % tbl.nrows)
        id2dbid_FEMTruss = {0:0}
        for row in tbl:
            obj = SenStore.FEMTrussFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'Epsin' in tbl.colnames:
                obj.mEpsin = row['Epsin']
            if 'Gapwidth' in tbl.colnames:
                obj.mGapwidth = row['Gapwidth']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            if 'SectionArea' in tbl.colnames:
                obj.mSectionArea = row['SectionArea']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            obj.id = mngr.addFEMTruss(obj)
            id2dbid_FEMTruss[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMTimeFunctionData'):
        logging.info('Skipping FEMTimeFunctionData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMTimeFunctionData
        logging.info('Restoring %d FEMTimeFunctionData object(s)' % tbl.nrows)
        id2dbid_FEMTimeFunctionData = {0:0}
        for row in tbl:
            obj = SenStore.FEMTimeFunctionDataFields()
            if 'DataTime' in tbl.colnames:
                obj.mDataTime = row['DataTime']
            if 'GMRecordID' in tbl.colnames:
                obj.mGMRecordID = row['GMRecordID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'TimeFunctionID' in tbl.colnames:
                obj.mTimeFunctionID = row['TimeFunctionID']
            if 'TimeValue' in tbl.colnames:
                obj.mTimeValue = row['TimeValue']
            obj.id = mngr.addFEMTimeFunctionData(obj)
            id2dbid_FEMTimeFunctionData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMPlasticMlMaterials'):
        logging.info('Skipping FEMPlasticMlMaterials objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMPlasticMlMaterials
        logging.info('Restoring %d FEMPlasticMlMaterials object(s)' % tbl.nrows)
        id2dbid_FEMPlasticMlMaterials = {0:0}
        for row in tbl:
            obj = SenStore.FEMPlasticMlMaterialsFields()
            if 'ALPHA' in tbl.colnames:
                obj.mALPHA = row['ALPHA']
            if 'Density' in tbl.colnames:
                obj.mDensity = row['Density']
            if 'E' in tbl.colnames:
                obj.mE = row['E']
            if 'Hardening' in tbl.colnames:
                obj.mHardening = row['Hardening']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'NU' in tbl.colnames:
                obj.mNU = row['NU']
            if 'TREF' in tbl.colnames:
                obj.mTREF = row['TREF']
            obj.id = mngr.addFEMPlasticMlMaterials(obj)
            id2dbid_FEMPlasticMlMaterials[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMPlateGroup'):
        logging.info('Skipping FEMPlateGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMPlateGroup
        logging.info('Restoring %d FEMPlateGroup object(s)' % tbl.nrows)
        id2dbid_FEMPlateGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMPlateGroupFields()
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'IniStrain' in tbl.colnames:
                obj.mIniStrain = row['IniStrain']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'Result' in tbl.colnames:
                obj.mResult = row['Result']
            obj.id = mngr.addFEMPlateGroup(obj)
            id2dbid_FEMPlateGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMBeam'):
        logging.info('Skipping FEMBeam objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMBeam
        logging.info('Restoring %d FEMBeam object(s)' % tbl.nrows)
        id2dbid_FEMBeam = {0:0}
        for row in tbl:
            obj = SenStore.FEMBeamFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'EndRelease' in tbl.colnames:
                obj.mEndRelease = row['EndRelease']
            if 'Epsin' in tbl.colnames:
                obj.mEpsin = row['Epsin']
            if 'IRigidEnd' in tbl.colnames:
                obj.mIRigidEnd = row['IRigidEnd']
            if 'JRigidEnd' in tbl.colnames:
                obj.mJRigidEnd = row['JRigidEnd']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'Subdivision' in tbl.colnames:
                obj.mSubdivision = row['Subdivision']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            obj.id = mngr.addFEMBeam(obj)
            id2dbid_FEMBeam[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMCurvMomentData'):
        logging.info('Skipping FEMCurvMomentData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMCurvMomentData
        logging.info('Restoring %d FEMCurvMomentData object(s)' % tbl.nrows)
        id2dbid_FEMCurvMomentData = {0:0}
        for row in tbl:
            obj = SenStore.FEMCurvMomentDataFields()
            if 'Curvature' in tbl.colnames:
                obj.mCurvature = row['Curvature']
            if 'CurvMomentID' in tbl.colnames:
                obj.mCurvMomentID = row['CurvMomentID']
            if 'Moment' in tbl.colnames:
                obj.mMoment = row['Moment']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMCurvMomentData(obj)
            id2dbid_FEMCurvMomentData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMPropertysets'):
        logging.info('Skipping FEMPropertysets objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMPropertysets
        logging.info('Restoring %d FEMPropertysets object(s)' % tbl.nrows)
        id2dbid_FEMPropertysets = {0:0}
        for row in tbl:
            obj = SenStore.FEMPropertysetsFields()
            if 'C' in tbl.colnames:
                obj.mC = row['C']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'K' in tbl.colnames:
                obj.mK = row['K']
            if 'M' in tbl.colnames:
                obj.mM = row['M']
            if 'NC' in tbl.colnames:
                obj.mNC = row['NC']
            if 'NK' in tbl.colnames:
                obj.mNK = row['NK']
            if 'NM' in tbl.colnames:
                obj.mNM = row['NM']
            if 'Nonlinear' in tbl.colnames:
                obj.mNonlinear = row['Nonlinear']
            if 'PropertysetID' in tbl.colnames:
                obj.mPropertysetID = row['PropertysetID']
            if 'S' in tbl.colnames:
                obj.mS = row['S']
            obj.id = mngr.addFEMPropertysets(obj)
            id2dbid_FEMPropertysets[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMOrthotropicMaterial'):
        logging.info('Skipping FEMOrthotropicMaterial objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMOrthotropicMaterial
        logging.info('Restoring %d FEMOrthotropicMaterial object(s)' % tbl.nrows)
        id2dbid_FEMOrthotropicMaterial = {0:0}
        for row in tbl:
            obj = SenStore.FEMOrthotropicMaterialFields()
            if 'Density' in tbl.colnames:
                obj.mDensity = row['Density']
            if 'EA' in tbl.colnames:
                obj.mEA = row['EA']
            if 'EB' in tbl.colnames:
                obj.mEB = row['EB']
            if 'EC' in tbl.colnames:
                obj.mEC = row['EC']
            if 'GAB' in tbl.colnames:
                obj.mGAB = row['GAB']
            if 'GAC' in tbl.colnames:
                obj.mGAC = row['GAC']
            if 'GBC' in tbl.colnames:
                obj.mGBC = row['GBC']
            if 'NUAB' in tbl.colnames:
                obj.mNUAB = row['NUAB']
            if 'NUAC' in tbl.colnames:
                obj.mNUAC = row['NUAC']
            if 'NUBC' in tbl.colnames:
                obj.mNUBC = row['NUBC']
            obj.id = mngr.addFEMOrthotropicMaterial(obj)
            id2dbid_FEMOrthotropicMaterial[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMAppliedLoads'):
        logging.info('Skipping FEMAppliedLoads objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMAppliedLoads
        logging.info('Restoring %d FEMAppliedLoads object(s)' % tbl.nrows)
        id2dbid_FEMAppliedLoads = {0:0}
        for row in tbl:
            obj = SenStore.FEMAppliedLoadsFields()
            if 'AppliedLoadNmb' in tbl.colnames:
                obj.mAppliedLoadNmb = row['AppliedLoadNmb']
            if 'ArrivalTime' in tbl.colnames:
                obj.mArrivalTime = row['ArrivalTime']
            if 'LoadID' in tbl.colnames:
                obj.mLoadID = row['LoadID']
            if 'LoadType' in tbl.colnames:
                obj.mLoadType = row['LoadType']
            if 'SiteType' in tbl.colnames:
                obj.mSiteType = row['SiteType']
            if 'TimeFunctionID' in tbl.colnames:
                obj.mTimeFunctionID = row['TimeFunctionID']
            obj.id = mngr.addFEMAppliedLoads(obj)
            id2dbid_FEMAppliedLoads[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMThermoOrthData'):
        logging.info('Skipping FEMThermoOrthData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMThermoOrthData
        logging.info('Restoring %d FEMThermoOrthData object(s)' % tbl.nrows)
        id2dbid_FEMThermoOrthData = {0:0}
        for row in tbl:
            obj = SenStore.FEMThermoOrthDataFields()
            if 'ALPHAA' in tbl.colnames:
                obj.mALPHAA = row['ALPHAA']
            if 'ALPHAB' in tbl.colnames:
                obj.mALPHAB = row['ALPHAB']
            if 'ALPHAC' in tbl.colnames:
                obj.mALPHAC = row['ALPHAC']
            if 'EA' in tbl.colnames:
                obj.mEA = row['EA']
            if 'EB' in tbl.colnames:
                obj.mEB = row['EB']
            if 'EC' in tbl.colnames:
                obj.mEC = row['EC']
            if 'GAB' in tbl.colnames:
                obj.mGAB = row['GAB']
            if 'GAC' in tbl.colnames:
                obj.mGAC = row['GAC']
            if 'GBC' in tbl.colnames:
                obj.mGBC = row['GBC']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'NUAB' in tbl.colnames:
                obj.mNUAB = row['NUAB']
            if 'NUAC' in tbl.colnames:
                obj.mNUAC = row['NUAC']
            if 'NUBC' in tbl.colnames:
                obj.mNUBC = row['NUBC']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Theta' in tbl.colnames:
                obj.mTheta = row['Theta']
            obj.id = mngr.addFEMThermoOrthData(obj)
            id2dbid_FEMThermoOrthData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMContactPairs'):
        logging.info('Skipping FEMContactPairs objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMContactPairs
        logging.info('Restoring %d FEMContactPairs object(s)' % tbl.nrows)
        id2dbid_FEMContactPairs = {0:0}
        for row in tbl:
            obj = SenStore.FEMContactPairsFields()
            if 'ContactorSurf' in tbl.colnames:
                obj.mContactorSurf = row['ContactorSurf']
            if 'ContGroupID' in tbl.colnames:
                obj.mContGroupID = row['ContGroupID']
            if 'ContPair' in tbl.colnames:
                obj.mContPair = row['ContPair']
            if 'FContactor' in tbl.colnames:
                obj.mFContactor = row['FContactor']
            if 'Friction' in tbl.colnames:
                obj.mFriction = row['Friction']
            if 'FTarget' in tbl.colnames:
                obj.mFTarget = row['FTarget']
            if 'HeatTransf' in tbl.colnames:
                obj.mHeatTransf = row['HeatTransf']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'TargetSurf' in tbl.colnames:
                obj.mTargetSurf = row['TargetSurf']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            obj.id = mngr.addFEMContactPairs(obj)
            id2dbid_FEMContactPairs[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMGeneral'):
        logging.info('Skipping FEMGeneral objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMGeneral
        logging.info('Restoring %d FEMGeneral object(s)' % tbl.nrows)
        id2dbid_FEMGeneral = {0:0}
        for row in tbl:
            obj = SenStore.FEMGeneralFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'MatrixSet' in tbl.colnames:
                obj.mMatrixSet = row['MatrixSet']
            if 'NodeAmount' in tbl.colnames:
                obj.mNodeAmount = row['NodeAmount']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            obj.id = mngr.addFEMGeneral(obj)
            id2dbid_FEMGeneral[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMBeamNode'):
        logging.info('Skipping FEMBeamNode objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMBeamNode
        logging.info('Restoring %d FEMBeamNode object(s)' % tbl.nrows)
        id2dbid_FEMBeamNode = {0:0}
        for row in tbl:
            obj = SenStore.FEMBeamNodeFields()
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'IniStrain' in tbl.colnames:
                obj.mIniStrain = row['IniStrain']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'MC' in tbl.colnames:
                obj.mMC = row['MC']
            if 'MCrigidity' in tbl.colnames:
                obj.mMCrigidity = row['MCrigidity']
            if 'REmultiplyer' in tbl.colnames:
                obj.mREmultiplyer = row['REmultiplyer']
            if 'Result' in tbl.colnames:
                obj.mResult = row['Result']
            if 'REtype' in tbl.colnames:
                obj.mREtype = row['REtype']
            if 'RINT' in tbl.colnames:
                obj.mRINT = row['RINT']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'SINT' in tbl.colnames:
                obj.mSINT = row['SINT']
            if 'TINT' in tbl.colnames:
                obj.mTINT = row['TINT']
            obj.id = mngr.addFEMBeamNode(obj)
            id2dbid_FEMBeamNode[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSectionRect'):
        logging.info('Skipping FEMSectionRect objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSectionRect
        logging.info('Restoring %d FEMSectionRect object(s)' % tbl.nrows)
        id2dbid_FEMSectionRect = {0:0}
        for row in tbl:
            obj = SenStore.FEMSectionRectFields()
            if 'Height' in tbl.colnames:
                obj.mHeight = row['Height']
            if 'IShear' in tbl.colnames:
                obj.mIShear = row['IShear']
            if 'SC' in tbl.colnames:
                obj.mSC = row['SC']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'SSarea' in tbl.colnames:
                obj.mSSarea = row['SSarea']
            if 'TC' in tbl.colnames:
                obj.mTC = row['TC']
            if 'Torfac' in tbl.colnames:
                obj.mTorfac = row['Torfac']
            if 'TSarea' in tbl.colnames:
                obj.mTSarea = row['TSarea']
            if 'Width' in tbl.colnames:
                obj.mWidth = row['Width']
            obj.id = mngr.addFEMSectionRect(obj)
            id2dbid_FEMSectionRect[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMBeamLoad'):
        logging.info('Skipping FEMBeamLoad objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMBeamLoad
        logging.info('Restoring %d FEMBeamLoad object(s)' % tbl.nrows)
        id2dbid_FEMBeamLoad = {0:0}
        for row in tbl:
            obj = SenStore.FEMBeamLoadFields()
            if 'ArrivalTime' in tbl.colnames:
                obj.mArrivalTime = row['ArrivalTime']
            if 'DeformDepend' in tbl.colnames:
                obj.mDeformDepend = row['DeformDepend']
            if 'DirectFilter' in tbl.colnames:
                obj.mDirectFilter = row['DirectFilter']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'Face' in tbl.colnames:
                obj.mFace = row['Face']
            if 'P1' in tbl.colnames:
                obj.mP1 = row['P1']
            if 'P2' in tbl.colnames:
                obj.mP2 = row['P2']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'TimeFunctionID' in tbl.colnames:
                obj.mTimeFunctionID = row['TimeFunctionID']
            obj.id = mngr.addFEMBeamLoad(obj)
            id2dbid_FEMBeamLoad[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMLoadMassProportional'):
        logging.info('Skipping FEMLoadMassProportional objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMLoadMassProportional
        logging.info('Restoring %d FEMLoadMassProportional object(s)' % tbl.nrows)
        id2dbid_FEMLoadMassProportional = {0:0}
        for row in tbl:
            obj = SenStore.FEMLoadMassProportionalFields()
            if 'AX' in tbl.colnames:
                obj.mAX = row['AX']
            if 'AY' in tbl.colnames:
                obj.mAY = row['AY']
            if 'AZ' in tbl.colnames:
                obj.mAZ = row['AZ']
            if 'LoadID' in tbl.colnames:
                obj.mLoadID = row['LoadID']
            if 'Magnitude' in tbl.colnames:
                obj.mMagnitude = row['Magnitude']
            obj.id = mngr.addFEMLoadMassProportional(obj)
            id2dbid_FEMLoadMassProportional[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMLink'):
        logging.info('Skipping FEMLink objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMLink
        logging.info('Restoring %d FEMLink object(s)' % tbl.nrows)
        id2dbid_FEMLink = {0:0}
        for row in tbl:
            obj = SenStore.FEMLinkFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'RLID' in tbl.colnames:
                obj.mRLID = row['RLID']
            obj.id = mngr.addFEMLink(obj)
            id2dbid_FEMLink[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMAxesNode'):
        logging.info('Skipping FEMAxesNode objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMAxesNode
        logging.info('Restoring %d FEMAxesNode object(s)' % tbl.nrows)
        id2dbid_FEMAxesNode = {0:0}
        for row in tbl:
            obj = SenStore.FEMAxesNodeFields()
            if 'AxNodeID' in tbl.colnames:
                obj.mAxNodeID = row['AxNodeID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMAxesNode(obj)
            id2dbid_FEMAxesNode[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMNMTimeMass'):
        logging.info('Skipping FEMNMTimeMass objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMNMTimeMass
        logging.info('Restoring %d FEMNMTimeMass object(s)' % tbl.nrows)
        id2dbid_FEMNMTimeMass = {0:0}
        for row in tbl:
            obj = SenStore.FEMNMTimeMassFields()
            if 'Mass' in tbl.colnames:
                obj.mMass = row['Mass']
            if 'PropertyID' in tbl.colnames:
                obj.mPropertyID = row['PropertyID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'TimeValue' in tbl.colnames:
                obj.mTimeValue = row['TimeValue']
            obj.id = mngr.addFEMNMTimeMass(obj)
            id2dbid_FEMNMTimeMass[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMAppliedDisplacement'):
        logging.info('Skipping FEMAppliedDisplacement objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMAppliedDisplacement
        logging.info('Restoring %d FEMAppliedDisplacement object(s)' % tbl.nrows)
        id2dbid_FEMAppliedDisplacement = {0:0}
        for row in tbl:
            obj = SenStore.FEMAppliedDisplacementFields()
            if 'ArrivalTime' in tbl.colnames:
                obj.mArrivalTime = row['ArrivalTime']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Direction' in tbl.colnames:
                obj.mDirection = row['Direction']
            if 'Factor' in tbl.colnames:
                obj.mFactor = row['Factor']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'TimeFunctionID' in tbl.colnames:
                obj.mTimeFunctionID = row['TimeFunctionID']
            obj.id = mngr.addFEMAppliedDisplacement(obj)
            id2dbid_FEMAppliedDisplacement[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMTimeFunctions'):
        logging.info('Skipping FEMTimeFunctions objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMTimeFunctions
        logging.info('Restoring %d FEMTimeFunctions object(s)' % tbl.nrows)
        id2dbid_FEMTimeFunctions = {0:0}
        for row in tbl:
            obj = SenStore.FEMTimeFunctionsFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'TimeFunctionID' in tbl.colnames:
                obj.mTimeFunctionID = row['TimeFunctionID']
            obj.id = mngr.addFEMTimeFunctions(obj)
            id2dbid_FEMTimeFunctions[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMForceStrainData'):
        logging.info('Skipping FEMForceStrainData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMForceStrainData
        logging.info('Restoring %d FEMForceStrainData object(s)' % tbl.nrows)
        id2dbid_FEMForceStrainData = {0:0}
        for row in tbl:
            obj = SenStore.FEMForceStrainDataFields()
            if 'Force' in tbl.colnames:
                obj.mForce = row['Force']
            if 'ForceAxID' in tbl.colnames:
                obj.mForceAxID = row['ForceAxID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Strain' in tbl.colnames:
                obj.mStrain = row['Strain']
            obj.id = mngr.addFEMForceStrainData(obj)
            id2dbid_FEMForceStrainData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSkewDOF'):
        logging.info('Skipping FEMSkewDOF objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSkewDOF
        logging.info('Restoring %d FEMSkewDOF object(s)' % tbl.nrows)
        id2dbid_FEMSkewDOF = {0:0}
        for row in tbl:
            obj = SenStore.FEMSkewDOFFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'SkewSystemID' in tbl.colnames:
                obj.mSkewSystemID = row['SkewSystemID']
            obj.id = mngr.addFEMSkewDOF(obj)
            id2dbid_FEMSkewDOF[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSectionI'):
        logging.info('Skipping FEMSectionI objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSectionI
        logging.info('Restoring %d FEMSectionI object(s)' % tbl.nrows)
        id2dbid_FEMSectionI = {0:0}
        for row in tbl:
            obj = SenStore.FEMSectionIFields()
            if 'Height' in tbl.colnames:
                obj.mHeight = row['Height']
            if 'SC' in tbl.colnames:
                obj.mSC = row['SC']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'SSarea' in tbl.colnames:
                obj.mSSarea = row['SSarea']
            if 'TC' in tbl.colnames:
                obj.mTC = row['TC']
            if 'Thick1' in tbl.colnames:
                obj.mThick1 = row['Thick1']
            if 'Thick2' in tbl.colnames:
                obj.mThick2 = row['Thick2']
            if 'Thick3' in tbl.colnames:
                obj.mThick3 = row['Thick3']
            if 'Torfac' in tbl.colnames:
                obj.mTorfac = row['Torfac']
            if 'TSarea' in tbl.colnames:
                obj.mTSarea = row['TSarea']
            if 'Width1' in tbl.colnames:
                obj.mWidth1 = row['Width1']
            if 'Width2' in tbl.colnames:
                obj.mWidth2 = row['Width2']
            obj.id = mngr.addFEMSectionI(obj)
            id2dbid_FEMSectionI[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMPlasticBilinearMaterial'):
        logging.info('Skipping FEMPlasticBilinearMaterial objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMPlasticBilinearMaterial
        logging.info('Restoring %d FEMPlasticBilinearMaterial object(s)' % tbl.nrows)
        id2dbid_FEMPlasticBilinearMaterial = {0:0}
        for row in tbl:
            obj = SenStore.FEMPlasticBilinearMaterialFields()
            if 'Alpha' in tbl.colnames:
                obj.mAlpha = row['Alpha']
            if 'Density' in tbl.colnames:
                obj.mDensity = row['Density']
            if 'E' in tbl.colnames:
                obj.mE = row['E']
            if 'EPA' in tbl.colnames:
                obj.mEPA = row['EPA']
            if 'ET' in tbl.colnames:
                obj.mET = row['ET']
            if 'Hardening' in tbl.colnames:
                obj.mHardening = row['Hardening']
            if 'NU' in tbl.colnames:
                obj.mNU = row['NU']
            if 'TRef' in tbl.colnames:
                obj.mTRef = row['TRef']
            if 'Yield' in tbl.colnames:
                obj.mYield = row['Yield']
            obj.id = mngr.addFEMPlasticBilinearMaterial(obj)
            id2dbid_FEMPlasticBilinearMaterial[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMMTForceData'):
        logging.info('Skipping FEMMTForceData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMMTForceData
        logging.info('Restoring %d FEMMTForceData object(s)' % tbl.nrows)
        id2dbid_FEMMTForceData = {0:0}
        for row in tbl:
            obj = SenStore.FEMMTForceDataFields()
            if 'Force' in tbl.colnames:
                obj.mForce = row['Force']
            if 'MomentRID' in tbl.colnames:
                obj.mMomentRID = row['MomentRID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'TwistMomentID' in tbl.colnames:
                obj.mTwistMomentID = row['TwistMomentID']
            obj.id = mngr.addFEMMTForceData(obj)
            id2dbid_FEMMTForceData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMShellPressure'):
        logging.info('Skipping FEMShellPressure objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMShellPressure
        logging.info('Restoring %d FEMShellPressure object(s)' % tbl.nrows)
        id2dbid_FEMShellPressure = {0:0}
        for row in tbl:
            obj = SenStore.FEMShellPressureFields()
            if 'ArrivalTime' in tbl.colnames:
                obj.mArrivalTime = row['ArrivalTime']
            if 'DeformDepend' in tbl.colnames:
                obj.mDeformDepend = row['DeformDepend']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'DirectFilter' in tbl.colnames:
                obj.mDirectFilter = row['DirectFilter']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'Face' in tbl.colnames:
                obj.mFace = row['Face']
            if 'Nodaux' in tbl.colnames:
                obj.mNodaux = row['Nodaux']
            if 'P1' in tbl.colnames:
                obj.mP1 = row['P1']
            if 'P2' in tbl.colnames:
                obj.mP2 = row['P2']
            if 'P3' in tbl.colnames:
                obj.mP3 = row['P3']
            if 'P4' in tbl.colnames:
                obj.mP4 = row['P4']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'TimeFunctionID' in tbl.colnames:
                obj.mTimeFunctionID = row['TimeFunctionID']
            obj.id = mngr.addFEMShellPressure(obj)
            id2dbid_FEMShellPressure[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMMatrices'):
        logging.info('Skipping FEMMatrices objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMMatrices
        logging.info('Restoring %d FEMMatrices object(s)' % tbl.nrows)
        id2dbid_FEMMatrices = {0:0}
        for row in tbl:
            obj = SenStore.FEMMatricesFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Lump' in tbl.colnames:
                obj.mLump = row['Lump']
            if 'MatrixID' in tbl.colnames:
                obj.mMatrixID = row['MatrixID']
            if 'MatrixType' in tbl.colnames:
                obj.mMatrixType = row['MatrixType']
            if 'ND' in tbl.colnames:
                obj.mND = row['ND']
            if 'NS' in tbl.colnames:
                obj.mNS = row['NS']
            obj.id = mngr.addFEMMatrices(obj)
            id2dbid_FEMMatrices[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMDamping'):
        logging.info('Skipping FEMDamping objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMDamping
        logging.info('Restoring %d FEMDamping object(s)' % tbl.nrows)
        id2dbid_FEMDamping = {0:0}
        for row in tbl:
            obj = SenStore.FEMDampingFields()
            if 'ALPHA' in tbl.colnames:
                obj.mALPHA = row['ALPHA']
            if 'BETA' in tbl.colnames:
                obj.mBETA = row['BETA']
            obj.id = mngr.addFEMDamping(obj)
            id2dbid_FEMDamping[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMMaterial'):
        logging.info('Skipping FEMMaterial objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMMaterial
        logging.info('Restoring %d FEMMaterial object(s)' % tbl.nrows)
        id2dbid_FEMMaterial = {0:0}
        for row in tbl:
            obj = SenStore.FEMMaterialFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'MaterialType' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.MaterialTypeEnumList[row['MaterialType']]
                obj.mMaterialType = SenStore.MaterialType.__dict__[enumLabel]
            if 'LocalID' in tbl.colnames:
                obj.mLocalID = row['LocalID']
            obj.id = mngr.addFEMMaterial(obj)
            id2dbid_FEMMaterial[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMMatrixData'):
        logging.info('Skipping FEMMatrixData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMMatrixData
        logging.info('Restoring %d FEMMatrixData object(s)' % tbl.nrows)
        id2dbid_FEMMatrixData = {0:0}
        for row in tbl:
            obj = SenStore.FEMMatrixDataFields()
            if 'Coeff' in tbl.colnames:
                obj.mCoeff = row['Coeff']
            if 'ColumnIndex' in tbl.colnames:
                obj.mColumnIndex = row['ColumnIndex']
            if 'MatrixID' in tbl.colnames:
                obj.mMatrixID = row['MatrixID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'RowIndex' in tbl.colnames:
                obj.mRowIndex = row['RowIndex']
            obj.id = mngr.addFEMMatrixData(obj)
            id2dbid_FEMMatrixData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMShellAxesOrtho'):
        logging.info('Skipping FEMShellAxesOrtho objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMShellAxesOrtho
        logging.info('Restoring %d FEMShellAxesOrtho object(s)' % tbl.nrows)
        id2dbid_FEMShellAxesOrtho = {0:0}
        for row in tbl:
            obj = SenStore.FEMShellAxesOrthoFields()
            if 'ALFA' in tbl.colnames:
                obj.mALFA = row['ALFA']
            if 'AxOrthoID' in tbl.colnames:
                obj.mAxOrthoID = row['AxOrthoID']
            if 'LineID' in tbl.colnames:
                obj.mLineID = row['LineID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMShellAxesOrtho(obj)
            id2dbid_FEMShellAxesOrtho[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMEndreleases'):
        logging.info('Skipping FEMEndreleases objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMEndreleases
        logging.info('Restoring %d FEMEndreleases object(s)' % tbl.nrows)
        id2dbid_FEMEndreleases = {0:0}
        for row in tbl:
            obj = SenStore.FEMEndreleasesFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'EndRelease' in tbl.colnames:
                obj.mEndRelease = row['EndRelease']
            if 'Moment1' in tbl.colnames:
                obj.mMoment1 = row['Moment1']
            if 'Moment2' in tbl.colnames:
                obj.mMoment2 = row['Moment2']
            if 'Moment3' in tbl.colnames:
                obj.mMoment3 = row['Moment3']
            if 'Moment4' in tbl.colnames:
                obj.mMoment4 = row['Moment4']
            if 'Moment5' in tbl.colnames:
                obj.mMoment5 = row['Moment5']
            if 'Moment6' in tbl.colnames:
                obj.mMoment6 = row['Moment6']
            obj.id = mngr.addFEMEndreleases(obj)
            id2dbid_FEMEndreleases[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMTrussGroup'):
        logging.info('Skipping FEMTrussGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMTrussGroup
        logging.info('Restoring %d FEMTrussGroup object(s)' % tbl.nrows)
        id2dbid_FEMTrussGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMTrussGroupFields()
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'GAPS' in tbl.colnames:
                obj.mGAPS = row['GAPS']
            if 'IniStrain' in tbl.colnames:
                obj.mIniStrain = row['IniStrain']
            if 'SectionArea' in tbl.colnames:
                obj.mSectionArea = row['SectionArea']
            obj.id = mngr.addFEMTrussGroup(obj)
            id2dbid_FEMTrussGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMInitialTemperature'):
        logging.info('Skipping FEMInitialTemperature objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMInitialTemperature
        logging.info('Restoring %d FEMInitialTemperature object(s)' % tbl.nrows)
        id2dbid_FEMInitialTemperature = {0:0}
        for row in tbl:
            obj = SenStore.FEMInitialTemperatureFields()
            if 'Temperature' in tbl.colnames:
                obj.mTemperature = row['Temperature']
            obj.id = mngr.addFEMInitialTemperature(obj)
            id2dbid_FEMInitialTemperature[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMThermoIsoMaterials'):
        logging.info('Skipping FEMThermoIsoMaterials objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMThermoIsoMaterials
        logging.info('Restoring %d FEMThermoIsoMaterials object(s)' % tbl.nrows)
        id2dbid_FEMThermoIsoMaterials = {0:0}
        for row in tbl:
            obj = SenStore.FEMThermoIsoMaterialsFields()
            if 'Density' in tbl.colnames:
                obj.mDensity = row['Density']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'TREF' in tbl.colnames:
                obj.mTREF = row['TREF']
            obj.id = mngr.addFEMThermoIsoMaterials(obj)
            id2dbid_FEMThermoIsoMaterials[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMThermoIsoData'):
        logging.info('Skipping FEMThermoIsoData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMThermoIsoData
        logging.info('Restoring %d FEMThermoIsoData object(s)' % tbl.nrows)
        id2dbid_FEMThermoIsoData = {0:0}
        for row in tbl:
            obj = SenStore.FEMThermoIsoDataFields()
            if 'ALPHA' in tbl.colnames:
                obj.mALPHA = row['ALPHA']
            if 'E' in tbl.colnames:
                obj.mE = row['E']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'NU' in tbl.colnames:
                obj.mNU = row['NU']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Theta' in tbl.colnames:
                obj.mTheta = row['Theta']
            obj.id = mngr.addFEMThermoIsoData(obj)
            id2dbid_FEMThermoIsoData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMContactGroup3'):
        logging.info('Skipping FEMContactGroup3 objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMContactGroup3
        logging.info('Restoring %d FEMContactGroup3 object(s)' % tbl.nrows)
        id2dbid_FEMContactGroup3 = {0:0}
        for row in tbl:
            obj = SenStore.FEMContactGroup3Fields()
            if 'ContGroupID' in tbl.colnames:
                obj.mContGroupID = row['ContGroupID']
            if 'Depth' in tbl.colnames:
                obj.mDepth = row['Depth']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Forces' in tbl.colnames:
                obj.mForces = row['Forces']
            if 'Friction' in tbl.colnames:
                obj.mFriction = row['Friction']
            if 'IniPenetration' in tbl.colnames:
                obj.mIniPenetration = row['IniPenetration']
            if 'NodeToNode' in tbl.colnames:
                obj.mNodeToNode = row['NodeToNode']
            if 'Offset' in tbl.colnames:
                obj.mOffset = row['Offset']
            if 'PenetrAlgorithm' in tbl.colnames:
                obj.mPenetrAlgorithm = row['PenetrAlgorithm']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            if 'Tied' in tbl.colnames:
                obj.mTied = row['Tied']
            if 'TiedOffset' in tbl.colnames:
                obj.mTiedOffset = row['TiedOffset']
            if 'Tolerance' in tbl.colnames:
                obj.mTolerance = row['Tolerance']
            if 'Tractions' in tbl.colnames:
                obj.mTractions = row['Tractions']
            obj.id = mngr.addFEMContactGroup3(obj)
            id2dbid_FEMContactGroup3[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMNLElasticMaterials'):
        logging.info('Skipping FEMNLElasticMaterials objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMNLElasticMaterials
        logging.info('Restoring %d FEMNLElasticMaterials object(s)' % tbl.nrows)
        id2dbid_FEMNLElasticMaterials = {0:0}
        for row in tbl:
            obj = SenStore.FEMNLElasticMaterialsFields()
            if 'Dcurve' in tbl.colnames:
                obj.mDcurve = row['Dcurve']
            if 'Density' in tbl.colnames:
                obj.mDensity = row['Density']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            obj.id = mngr.addFEMNLElasticMaterials(obj)
            id2dbid_FEMNLElasticMaterials[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMPlate'):
        logging.info('Skipping FEMPlate objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMPlate
        logging.info('Restoring %d FEMPlate object(s)' % tbl.nrows)
        id2dbid_FEMPlate = {0:0}
        for row in tbl:
            obj = SenStore.FEMPlateFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'Flex11' in tbl.colnames:
                obj.mFlex11 = row['Flex11']
            if 'Flex12' in tbl.colnames:
                obj.mFlex12 = row['Flex12']
            if 'Flex22' in tbl.colnames:
                obj.mFlex22 = row['Flex22']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'Meps11' in tbl.colnames:
                obj.mMeps11 = row['Meps11']
            if 'Meps12' in tbl.colnames:
                obj.mMeps12 = row['Meps12']
            if 'Meps22' in tbl.colnames:
                obj.mMeps22 = row['Meps22']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            if 'Thick' in tbl.colnames:
                obj.mThick = row['Thick']
            obj.id = mngr.addFEMPlate(obj)
            id2dbid_FEMPlate[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMIsoBeam'):
        logging.info('Skipping FEMIsoBeam objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMIsoBeam
        logging.info('Restoring %d FEMIsoBeam object(s)' % tbl.nrows)
        id2dbid_FEMIsoBeam = {0:0}
        for row in tbl:
            obj = SenStore.FEMIsoBeamFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'Epaxl' in tbl.colnames:
                obj.mEpaxl = row['Epaxl']
            if 'Ephoop' in tbl.colnames:
                obj.mEphoop = row['Ephoop']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'NodeAmount' in tbl.colnames:
                obj.mNodeAmount = row['NodeAmount']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            obj.id = mngr.addFEMIsoBeam(obj)
            id2dbid_FEMIsoBeam[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMAppliedConcentratedLoad'):
        logging.info('Skipping FEMAppliedConcentratedLoad objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMAppliedConcentratedLoad
        logging.info('Restoring %d FEMAppliedConcentratedLoad object(s)' % tbl.nrows)
        id2dbid_FEMAppliedConcentratedLoad = {0:0}
        for row in tbl:
            obj = SenStore.FEMAppliedConcentratedLoadFields()
            if 'ArrivalTime' in tbl.colnames:
                obj.mArrivalTime = row['ArrivalTime']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Direction' in tbl.colnames:
                obj.mDirection = row['Direction']
            if 'Factor' in tbl.colnames:
                obj.mFactor = row['Factor']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'TimeFunctionID' in tbl.colnames:
                obj.mTimeFunctionID = row['TimeFunctionID']
            obj.id = mngr.addFEMAppliedConcentratedLoad(obj)
            id2dbid_FEMAppliedConcentratedLoad[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMTwoDSolidGroup'):
        logging.info('Skipping FEMTwoDSolidGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMTwoDSolidGroup
        logging.info('Restoring %d FEMTwoDSolidGroup object(s)' % tbl.nrows)
        id2dbid_FEMTwoDSolidGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMTwoDSolidGroupFields()
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'Result' in tbl.colnames:
                obj.mResult = row['Result']
            if 'Subtype' in tbl.colnames:
                obj.mSubtype = row['Subtype']
            obj.id = mngr.addFEMTwoDSolidGroup(obj)
            id2dbid_FEMTwoDSolidGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMGroup'):
        logging.info('Skipping FEMGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMGroup
        logging.info('Restoring %d FEMGroup object(s)' % tbl.nrows)
        id2dbid_FEMGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMGroupFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'GroupType' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.GroupTypeEnumList[row['GroupType']]
                obj.mGroupType = SenStore.GroupType.__dict__[enumLabel]
            if 'LocalID' in tbl.colnames:
                obj.mLocalID = row['LocalID']
            obj.id = mngr.addFEMGroup(obj)
            id2dbid_FEMGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMProperties'):
        logging.info('Skipping FEMProperties objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMProperties
        logging.info('Restoring %d FEMProperties object(s)' % tbl.nrows)
        id2dbid_FEMProperties = {0:0}
        for row in tbl:
            obj = SenStore.FEMPropertiesFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'PropertyID' in tbl.colnames:
                obj.mPropertyID = row['PropertyID']
            if 'PropertyType' in tbl.colnames:
                obj.mPropertyType = row['PropertyType']
            if 'Rupture' in tbl.colnames:
                obj.mRupture = row['Rupture']
            if 'XC' in tbl.colnames:
                obj.mXC = row['XC']
            if 'XN' in tbl.colnames:
                obj.mXN = row['XN']
            obj.id = mngr.addFEMProperties(obj)
            id2dbid_FEMProperties[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMThreeDSolidGroup'):
        logging.info('Skipping FEMThreeDSolidGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMThreeDSolidGroup
        logging.info('Restoring %d FEMThreeDSolidGroup object(s)' % tbl.nrows)
        id2dbid_FEMThreeDSolidGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMThreeDSolidGroupFields()
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'Result' in tbl.colnames:
                obj.mResult = row['Result']
            obj.id = mngr.addFEMThreeDSolidGroup(obj)
            id2dbid_FEMThreeDSolidGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMThreeDSolid'):
        logging.info('Skipping FEMThreeDSolid objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMThreeDSolid
        logging.info('Restoring %d FEMThreeDSolid object(s)' % tbl.nrows)
        id2dbid_FEMThreeDSolid = {0:0}
        for row in tbl:
            obj = SenStore.FEMThreeDSolidFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'Maxes' in tbl.colnames:
                obj.mMaxes = row['Maxes']
            if 'NodeAmount' in tbl.colnames:
                obj.mNodeAmount = row['NodeAmount']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            obj.id = mngr.addFEMThreeDSolid(obj)
            id2dbid_FEMThreeDSolid[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSectionProp'):
        logging.info('Skipping FEMSectionProp objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSectionProp
        logging.info('Restoring %d FEMSectionProp object(s)' % tbl.nrows)
        id2dbid_FEMSectionProp = {0:0}
        for row in tbl:
            obj = SenStore.FEMSectionPropFields()
            if 'Area' in tbl.colnames:
                obj.mArea = row['Area']
            if 'Rinertia' in tbl.colnames:
                obj.mRinertia = row['Rinertia']
            if 'Sarea' in tbl.colnames:
                obj.mSarea = row['Sarea']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'Sinertia' in tbl.colnames:
                obj.mSinertia = row['Sinertia']
            if 'Tarea' in tbl.colnames:
                obj.mTarea = row['Tarea']
            if 'Tinertia' in tbl.colnames:
                obj.mTinertia = row['Tinertia']
            obj.id = mngr.addFEMSectionProp(obj)
            id2dbid_FEMSectionProp[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMElasticMaterial'):
        logging.info('Skipping FEMElasticMaterial objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMElasticMaterial
        logging.info('Restoring %d FEMElasticMaterial object(s)' % tbl.nrows)
        id2dbid_FEMElasticMaterial = {0:0}
        for row in tbl:
            obj = SenStore.FEMElasticMaterialFields()
            if 'Alpha' in tbl.colnames:
                obj.mAlpha = row['Alpha']
            if 'Density' in tbl.colnames:
                obj.mDensity = row['Density']
            if 'E' in tbl.colnames:
                obj.mE = row['E']
            if 'NU' in tbl.colnames:
                obj.mNU = row['NU']
            obj.id = mngr.addFEMElasticMaterial(obj)
            id2dbid_FEMElasticMaterial[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMPoints'):
        logging.info('Skipping FEMPoints objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMPoints
        logging.info('Restoring %d FEMPoints object(s)' % tbl.nrows)
        id2dbid_FEMPoints = {0:0}
        for row in tbl:
            obj = SenStore.FEMPointsFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'PointID' in tbl.colnames:
                obj.mPointID = row['PointID']
            if 'SystemID' in tbl.colnames:
                obj.mSystemID = row['SystemID']
            if 'X' in tbl.colnames:
                obj.mX = row['X']
            if 'Y' in tbl.colnames:
                obj.mY = row['Y']
            if 'Z' in tbl.colnames:
                obj.mZ = row['Z']
            obj.id = mngr.addFEMPoints(obj)
            id2dbid_FEMPoints[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMThermoOrthMaterials'):
        logging.info('Skipping FEMThermoOrthMaterials objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMThermoOrthMaterials
        logging.info('Restoring %d FEMThermoOrthMaterials object(s)' % tbl.nrows)
        id2dbid_FEMThermoOrthMaterials = {0:0}
        for row in tbl:
            obj = SenStore.FEMThermoOrthMaterialsFields()
            if 'Density' in tbl.colnames:
                obj.mDensity = row['Density']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'TREF' in tbl.colnames:
                obj.mTREF = row['TREF']
            obj.id = mngr.addFEMThermoOrthMaterials(obj)
            id2dbid_FEMThermoOrthMaterials[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMConstraints'):
        logging.info('Skipping FEMConstraints objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMConstraints
        logging.info('Restoring %d FEMConstraints object(s)' % tbl.nrows)
        id2dbid_FEMConstraints = {0:0}
        for row in tbl:
            obj = SenStore.FEMConstraintsFields()
            if 'ConstraintID' in tbl.colnames:
                obj.mConstraintID = row['ConstraintID']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'SlaveDOF' in tbl.colnames:
                obj.mSlaveDOF = row['SlaveDOF']
            if 'SlaveNode' in tbl.colnames:
                obj.mSlaveNode = row['SlaveNode']
            obj.id = mngr.addFEMConstraints(obj)
            id2dbid_FEMConstraints[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMMCrigidities'):
        logging.info('Skipping FEMMCrigidities objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMMCrigidities
        logging.info('Restoring %d FEMMCrigidities object(s)' % tbl.nrows)
        id2dbid_FEMMCrigidities = {0:0}
        for row in tbl:
            obj = SenStore.FEMMCrigiditiesFields()
            if 'AcurveType' in tbl.colnames:
                obj.mAcurveType = row['AcurveType']
            if 'Alpha' in tbl.colnames:
                obj.mAlpha = row['Alpha']
            if 'AxialCF' in tbl.colnames:
                obj.mAxialCF = row['AxialCF']
            if 'BcurveType' in tbl.colnames:
                obj.mBcurveType = row['BcurveType']
            if 'BendingCF' in tbl.colnames:
                obj.mBendingCF = row['BendingCF']
            if 'Beta' in tbl.colnames:
                obj.mBeta = row['Beta']
            if 'Density' in tbl.colnames:
                obj.mDensity = row['Density']
            if 'ForceAxID' in tbl.colnames:
                obj.mForceAxID = row['ForceAxID']
            if 'Hardening' in tbl.colnames:
                obj.mHardening = row['Hardening']
            if 'MassArea' in tbl.colnames:
                obj.mMassArea = row['MassArea']
            if 'MassR' in tbl.colnames:
                obj.mMassR = row['MassR']
            if 'MassS' in tbl.colnames:
                obj.mMassS = row['MassS']
            if 'MassT' in tbl.colnames:
                obj.mMassT = row['MassT']
            if 'MomentRID' in tbl.colnames:
                obj.mMomentRID = row['MomentRID']
            if 'MomentSID' in tbl.colnames:
                obj.mMomentSID = row['MomentSID']
            if 'MomentTID' in tbl.colnames:
                obj.mMomentTID = row['MomentTID']
            if 'RigidityID' in tbl.colnames:
                obj.mRigidityID = row['RigidityID']
            if 'TcurveType' in tbl.colnames:
                obj.mTcurveType = row['TcurveType']
            if 'TorsionCF' in tbl.colnames:
                obj.mTorsionCF = row['TorsionCF']
            obj.id = mngr.addFEMMCrigidities(obj)
            id2dbid_FEMMCrigidities[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSkeySysNode'):
        logging.info('Skipping FEMSkeySysNode objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSkeySysNode
        logging.info('Restoring %d FEMSkeySysNode object(s)' % tbl.nrows)
        id2dbid_FEMSkeySysNode = {0:0}
        for row in tbl:
            obj = SenStore.FEMSkeySysNodeFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'SkewSystemID' in tbl.colnames:
                obj.mSkewSystemID = row['SkewSystemID']
            obj.id = mngr.addFEMSkeySysNode(obj)
            id2dbid_FEMSkeySysNode[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMIsoBeamGroup'):
        logging.info('Skipping FEMIsoBeamGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMIsoBeamGroup
        logging.info('Restoring %d FEMIsoBeamGroup object(s)' % tbl.nrows)
        id2dbid_FEMIsoBeamGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMIsoBeamGroupFields()
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'IniStrain' in tbl.colnames:
                obj.mIniStrain = row['IniStrain']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'Result' in tbl.colnames:
                obj.mResult = row['Result']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            obj.id = mngr.addFEMIsoBeamGroup(obj)
            id2dbid_FEMIsoBeamGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMShellDOF'):
        logging.info('Skipping FEMShellDOF objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMShellDOF
        logging.info('Restoring %d FEMShellDOF object(s)' % tbl.nrows)
        id2dbid_FEMShellDOF = {0:0}
        for row in tbl:
            obj = SenStore.FEMShellDOFFields()
            if 'DOFnumber' in tbl.colnames:
                obj.mDOFnumber = row['DOFnumber']
            if 'VectorID' in tbl.colnames:
                obj.mVectorID = row['VectorID']
            obj.id = mngr.addFEMShellDOF(obj)
            id2dbid_FEMShellDOF[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMCrossSections'):
        logging.info('Skipping FEMCrossSections objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMCrossSections
        logging.info('Restoring %d FEMCrossSections object(s)' % tbl.nrows)
        id2dbid_FEMCrossSections = {0:0}
        for row in tbl:
            obj = SenStore.FEMCrossSectionsFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'SectionType' in tbl.colnames:
                obj.mSectionType = row['SectionType']
            obj.id = mngr.addFEMCrossSections(obj)
            id2dbid_FEMCrossSections[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMTwistMomentData'):
        logging.info('Skipping FEMTwistMomentData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMTwistMomentData
        logging.info('Restoring %d FEMTwistMomentData object(s)' % tbl.nrows)
        id2dbid_FEMTwistMomentData = {0:0}
        for row in tbl:
            obj = SenStore.FEMTwistMomentDataFields()
            if 'Moment' in tbl.colnames:
                obj.mMoment = row['Moment']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Twist' in tbl.colnames:
                obj.mTwist = row['Twist']
            if 'TwistMomentID' in tbl.colnames:
                obj.mTwistMomentID = row['TwistMomentID']
            obj.id = mngr.addFEMTwistMomentData(obj)
            id2dbid_FEMTwistMomentData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMShell'):
        logging.info('Skipping FEMShell objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMShell
        logging.info('Restoring %d FEMShell object(s)' % tbl.nrows)
        id2dbid_FEMShell = {0:0}
        for row in tbl:
            obj = SenStore.FEMShellFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'NodeAmount' in tbl.colnames:
                obj.mNodeAmount = row['NodeAmount']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            if 'T1' in tbl.colnames:
                obj.mT1 = row['T1']
            if 'T2' in tbl.colnames:
                obj.mT2 = row['T2']
            if 'T3' in tbl.colnames:
                obj.mT3 = row['T3']
            if 'T4' in tbl.colnames:
                obj.mT4 = row['T4']
            if 'T5' in tbl.colnames:
                obj.mT5 = row['T5']
            if 'T6' in tbl.colnames:
                obj.mT6 = row['T6']
            if 'T7' in tbl.colnames:
                obj.mT7 = row['T7']
            if 'T8' in tbl.colnames:
                obj.mT8 = row['T8']
            if 'T9' in tbl.colnames:
                obj.mT9 = row['T9']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            obj.id = mngr.addFEMShell(obj)
            id2dbid_FEMShell[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMNTNContact'):
        logging.info('Skipping FEMNTNContact objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMNTNContact
        logging.info('Restoring %d FEMNTNContact object(s)' % tbl.nrows)
        id2dbid_FEMNTNContact = {0:0}
        for row in tbl:
            obj = SenStore.FEMNTNContactFields()
            if 'ContactorNode' in tbl.colnames:
                obj.mContactorNode = row['ContactorNode']
            if 'ContGroupID' in tbl.colnames:
                obj.mContGroupID = row['ContGroupID']
            if 'ContPair' in tbl.colnames:
                obj.mContPair = row['ContPair']
            if 'PrintRes' in tbl.colnames:
                obj.mPrintRes = row['PrintRes']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'SaveRes' in tbl.colnames:
                obj.mSaveRes = row['SaveRes']
            if 'TargetNode' in tbl.colnames:
                obj.mTargetNode = row['TargetNode']
            if 'TargetNx' in tbl.colnames:
                obj.mTargetNx = row['TargetNx']
            if 'TargetNy' in tbl.colnames:
                obj.mTargetNy = row['TargetNy']
            if 'TargetNz' in tbl.colnames:
                obj.mTargetNz = row['TargetNz']
            obj.id = mngr.addFEMNTNContact(obj)
            id2dbid_FEMNTNContact[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMShellLayer'):
        logging.info('Skipping FEMShellLayer objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMShellLayer
        logging.info('Restoring %d FEMShellLayer object(s)' % tbl.nrows)
        id2dbid_FEMShellLayer = {0:0}
        for row in tbl:
            obj = SenStore.FEMShellLayerFields()
            if 'LayerNumber' in tbl.colnames:
                obj.mLayerNumber = row['LayerNumber']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'PThick' in tbl.colnames:
                obj.mPThick = row['PThick']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMShellLayer(obj)
            id2dbid_FEMShellLayer[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSkewSysAngles'):
        logging.info('Skipping FEMSkewSysAngles objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSkewSysAngles
        logging.info('Restoring %d FEMSkewSysAngles object(s)' % tbl.nrows)
        id2dbid_FEMSkewSysAngles = {0:0}
        for row in tbl:
            obj = SenStore.FEMSkewSysAnglesFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'PHI' in tbl.colnames:
                obj.mPHI = row['PHI']
            if 'SkewSystemID' in tbl.colnames:
                obj.mSkewSystemID = row['SkewSystemID']
            if 'THETA' in tbl.colnames:
                obj.mTHETA = row['THETA']
            if 'XSI' in tbl.colnames:
                obj.mXSI = row['XSI']
            obj.id = mngr.addFEMSkewSysAngles(obj)
            id2dbid_FEMSkewSysAngles[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMGroundMotionRecord'):
        logging.info('Skipping FEMGroundMotionRecord objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMGroundMotionRecord
        logging.info('Restoring %d FEMGroundMotionRecord object(s)' % tbl.nrows)
        id2dbid_FEMGroundMotionRecord = {0:0}
        for row in tbl:
            obj = SenStore.FEMGroundMotionRecordFields()
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'GMRecordID' in tbl.colnames:
                obj.mGMRecordID = row['GMRecordID']
            if 'GMRecordName' in tbl.colnames:
                obj.mGMRecordName = row['GMRecordName']
            obj.id = mngr.addFEMGroundMotionRecord(obj)
            id2dbid_FEMGroundMotionRecord[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMGeneralGroup'):
        logging.info('Skipping FEMGeneralGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMGeneralGroup
        logging.info('Restoring %d FEMGeneralGroup object(s)' % tbl.nrows)
        id2dbid_FEMGeneralGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMGeneralGroupFields()
            if 'MatrixSet' in tbl.colnames:
                obj.mMatrixSet = row['MatrixSet']
            if 'Result' in tbl.colnames:
                obj.mResult = row['Result']
            if 'SkewSystem' in tbl.colnames:
                obj.mSkewSystem = row['SkewSystem']
            obj.id = mngr.addFEMGeneralGroup(obj)
            id2dbid_FEMGeneralGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMTwoDSolid'):
        logging.info('Skipping FEMTwoDSolid objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMTwoDSolid
        logging.info('Restoring %d FEMTwoDSolid object(s)' % tbl.nrows)
        id2dbid_FEMTwoDSolid = {0:0}
        for row in tbl:
            obj = SenStore.FEMTwoDSolidFields()
            if 'Bet' in tbl.colnames:
                obj.mBet = row['Bet']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'NodeAmount' in tbl.colnames:
                obj.mNodeAmount = row['NodeAmount']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            if 'Thick' in tbl.colnames:
                obj.mThick = row['Thick']
            obj.id = mngr.addFEMTwoDSolid(obj)
            id2dbid_FEMTwoDSolid[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMAppliedTemperature'):
        logging.info('Skipping FEMAppliedTemperature objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMAppliedTemperature
        logging.info('Restoring %d FEMAppliedTemperature object(s)' % tbl.nrows)
        id2dbid_FEMAppliedTemperature = {0:0}
        for row in tbl:
            obj = SenStore.FEMAppliedTemperatureFields()
            if 'ArrivalTime' in tbl.colnames:
                obj.mArrivalTime = row['ArrivalTime']
            if 'Factor' in tbl.colnames:
                obj.mFactor = row['Factor']
            if 'RecordNmbr' in tbl.colnames:
                obj.mRecordNmbr = row['RecordNmbr']
            if 'TimeFunctionID' in tbl.colnames:
                obj.mTimeFunctionID = row['TimeFunctionID']
            obj.id = mngr.addFEMAppliedTemperature(obj)
            id2dbid_FEMAppliedTemperature[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMMatrixSets'):
        logging.info('Skipping FEMMatrixSets objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMMatrixSets
        logging.info('Restoring %d FEMMatrixSets object(s)' % tbl.nrows)
        id2dbid_FEMMatrixSets = {0:0}
        for row in tbl:
            obj = SenStore.FEMMatrixSetsFields()
            if 'Damping' in tbl.colnames:
                obj.mDamping = row['Damping']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'Mass' in tbl.colnames:
                obj.mMass = row['Mass']
            if 'MatrixSetID' in tbl.colnames:
                obj.mMatrixSetID = row['MatrixSetID']
            if 'Stiffness' in tbl.colnames:
                obj.mStiffness = row['Stiffness']
            if 'Stress' in tbl.colnames:
                obj.mStress = row['Stress']
            obj.id = mngr.addFEMMatrixSets(obj)
            id2dbid_FEMMatrixSets[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMConstraintCoef'):
        logging.info('Skipping FEMConstraintCoef objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMConstraintCoef
        logging.info('Restoring %d FEMConstraintCoef object(s)' % tbl.nrows)
        id2dbid_FEMConstraintCoef = {0:0}
        for row in tbl:
            obj = SenStore.FEMConstraintCoefFields()
            if 'Coefficient' in tbl.colnames:
                obj.mCoefficient = row['Coefficient']
            if 'ConstraintID' in tbl.colnames:
                obj.mConstraintID = row['ConstraintID']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'MasterDOF' in tbl.colnames:
                obj.mMasterDOF = row['MasterDOF']
            if 'MasterNode' in tbl.colnames:
                obj.mMasterNode = row['MasterNode']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMConstraintCoef(obj)
            id2dbid_FEMConstraintCoef[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSectionBox'):
        logging.info('Skipping FEMSectionBox objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSectionBox
        logging.info('Restoring %d FEMSectionBox object(s)' % tbl.nrows)
        id2dbid_FEMSectionBox = {0:0}
        for row in tbl:
            obj = SenStore.FEMSectionBoxFields()
            if 'Height' in tbl.colnames:
                obj.mHeight = row['Height']
            if 'SC' in tbl.colnames:
                obj.mSC = row['SC']
            if 'SectionID' in tbl.colnames:
                obj.mSectionID = row['SectionID']
            if 'SSarea' in tbl.colnames:
                obj.mSSarea = row['SSarea']
            if 'TC' in tbl.colnames:
                obj.mTC = row['TC']
            if 'Thick1' in tbl.colnames:
                obj.mThick1 = row['Thick1']
            if 'Thick2' in tbl.colnames:
                obj.mThick2 = row['Thick2']
            if 'Torfac' in tbl.colnames:
                obj.mTorfac = row['Torfac']
            if 'TSarea' in tbl.colnames:
                obj.mTSarea = row['TSarea']
            if 'Width' in tbl.colnames:
                obj.mWidth = row['Width']
            obj.id = mngr.addFEMSectionBox(obj)
            id2dbid_FEMSectionBox[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMNKDisplForce'):
        logging.info('Skipping FEMNKDisplForce objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMNKDisplForce
        logging.info('Restoring %d FEMNKDisplForce object(s)' % tbl.nrows)
        id2dbid_FEMNKDisplForce = {0:0}
        for row in tbl:
            obj = SenStore.FEMNKDisplForceFields()
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'Force' in tbl.colnames:
                obj.mForce = row['Force']
            if 'PropertyID' in tbl.colnames:
                obj.mPropertyID = row['PropertyID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMNKDisplForce(obj)
            id2dbid_FEMNKDisplForce[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMPlasticStrainStress'):
        logging.info('Skipping FEMPlasticStrainStress objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMPlasticStrainStress
        logging.info('Restoring %d FEMPlasticStrainStress object(s)' % tbl.nrows)
        id2dbid_FEMPlasticStrainStress = {0:0}
        for row in tbl:
            obj = SenStore.FEMPlasticStrainStressFields()
            if 'MaterialID' in tbl.colnames:
                obj.mMaterialID = row['MaterialID']
            if 'RecordNumber' in tbl.colnames:
                obj.mRecordNumber = row['RecordNumber']
            if 'Strain' in tbl.colnames:
                obj.mStrain = row['Strain']
            if 'Stress' in tbl.colnames:
                obj.mStress = row['Stress']
            obj.id = mngr.addFEMPlasticStrainStress(obj)
            id2dbid_FEMPlasticStrainStress[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMShellAxesOrthoData'):
        logging.info('Skipping FEMShellAxesOrthoData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMShellAxesOrthoData
        logging.info('Restoring %d FEMShellAxesOrthoData object(s)' % tbl.nrows)
        id2dbid_FEMShellAxesOrthoData = {0:0}
        for row in tbl:
            obj = SenStore.FEMShellAxesOrthoDataFields()
            if 'AxOrthoID' in tbl.colnames:
                obj.mAxOrthoID = row['AxOrthoID']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMShellAxesOrthoData(obj)
            id2dbid_FEMShellAxesOrthoData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMGeneralNode'):
        logging.info('Skipping FEMGeneralNode objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMGeneralNode
        logging.info('Restoring %d FEMGeneralNode object(s)' % tbl.nrows)
        id2dbid_FEMGeneralNode = {0:0}
        for row in tbl:
            obj = SenStore.FEMGeneralNodeFields()
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'LocalNmb' in tbl.colnames:
                obj.mLocalNmb = row['LocalNmb']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMGeneralNode(obj)
            id2dbid_FEMGeneralNode[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMStrLines'):
        logging.info('Skipping FEMStrLines objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMStrLines
        logging.info('Restoring %d FEMStrLines object(s)' % tbl.nrows)
        id2dbid_FEMStrLines = {0:0}
        for row in tbl:
            obj = SenStore.FEMStrLinesFields()
            if 'LineID' in tbl.colnames:
                obj.mLineID = row['LineID']
            if 'P1' in tbl.colnames:
                obj.mP1 = row['P1']
            if 'P2' in tbl.colnames:
                obj.mP2 = row['P2']
            obj.id = mngr.addFEMStrLines(obj)
            id2dbid_FEMStrLines[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMContactSurface'):
        logging.info('Skipping FEMContactSurface objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMContactSurface
        logging.info('Restoring %d FEMContactSurface object(s)' % tbl.nrows)
        id2dbid_FEMContactSurface = {0:0}
        for row in tbl:
            obj = SenStore.FEMContactSurfaceFields()
            if 'ContGroupID' in tbl.colnames:
                obj.mContGroupID = row['ContGroupID']
            if 'ContSegment' in tbl.colnames:
                obj.mContSegment = row['ContSegment']
            if 'ContSurface' in tbl.colnames:
                obj.mContSurface = row['ContSurface']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMContactSurface(obj)
            id2dbid_FEMContactSurface[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMMCForceData'):
        logging.info('Skipping FEMMCForceData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMMCForceData
        logging.info('Restoring %d FEMMCForceData object(s)' % tbl.nrows)
        id2dbid_FEMMCForceData = {0:0}
        for row in tbl:
            obj = SenStore.FEMMCForceDataFields()
            if 'CurvMomentID' in tbl.colnames:
                obj.mCurvMomentID = row['CurvMomentID']
            if 'Force' in tbl.colnames:
                obj.mForce = row['Force']
            if 'MomentSTID' in tbl.colnames:
                obj.mMomentSTID = row['MomentSTID']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            obj.id = mngr.addFEMMCForceData(obj)
            id2dbid_FEMMCForceData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSpring'):
        logging.info('Skipping FEMSpring objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSpring
        logging.info('Restoring %d FEMSpring object(s)' % tbl.nrows)
        id2dbid_FEMSpring = {0:0}
        for row in tbl:
            obj = SenStore.FEMSpringFields()
            if 'AX' in tbl.colnames:
                obj.mAX = row['AX']
            if 'AY' in tbl.colnames:
                obj.mAY = row['AY']
            if 'AZ' in tbl.colnames:
                obj.mAZ = row['AZ']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'ElementID' in tbl.colnames:
                obj.mElementID = row['ElementID']
            if 'ID1' in tbl.colnames:
                obj.mID1 = row['ID1']
            if 'ID2' in tbl.colnames:
                obj.mID2 = row['ID2']
            if 'PropertySet' in tbl.colnames:
                obj.mPropertySet = row['PropertySet']
            if 'RecordNmb' in tbl.colnames:
                obj.mRecordNmb = row['RecordNmb']
            if 'Save' in tbl.colnames:
                obj.mSave = row['Save']
            if 'TBirth' in tbl.colnames:
                obj.mTBirth = row['TBirth']
            if 'TDeath' in tbl.colnames:
                obj.mTDeath = row['TDeath']
            obj.id = mngr.addFEMSpring(obj)
            id2dbid_FEMSpring[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMSpringGroup'):
        logging.info('Skipping FEMSpringGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMSpringGroup
        logging.info('Restoring %d FEMSpringGroup object(s)' % tbl.nrows)
        id2dbid_FEMSpringGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMSpringGroupFields()
            if 'Bolt' in tbl.colnames:
                obj.mBolt = row['Bolt']
            if 'Nonlinear' in tbl.colnames:
                obj.mNonlinear = row['Nonlinear']
            if 'PropertySet' in tbl.colnames:
                obj.mPropertySet = row['PropertySet']
            if 'Result' in tbl.colnames:
                obj.mResult = row['Result']
            if 'SkewSystem' in tbl.colnames:
                obj.mSkewSystem = row['SkewSystem']
            obj.id = mngr.addFEMSpringGroup(obj)
            id2dbid_FEMSpringGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'FEMShellGroup'):
        logging.info('Skipping FEMShellGroup objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.FEMShellGroup
        logging.info('Restoring %d FEMShellGroup object(s)' % tbl.nrows)
        id2dbid_FEMShellGroup = {0:0}
        for row in tbl:
            obj = SenStore.FEMShellGroupFields()
            if 'Displacement' in tbl.colnames:
                obj.mDisplacement = row['Displacement']
            if 'NLayers' in tbl.colnames:
                obj.mNLayers = row['NLayers']
            if 'Result' in tbl.colnames:
                obj.mResult = row['Result']
            if 'SectionResult' in tbl.colnames:
                obj.mSectionResult = row['SectionResult']
            if 'StressReference' in tbl.colnames:
                obj.mStressReference = row['StressReference']
            if 'Thickness' in tbl.colnames:
                obj.mThickness = row['Thickness']
            obj.id = mngr.addFEMShellGroup(obj)
            id2dbid_FEMShellGroup[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'DaqUnit'):
        logging.info('Skipping DaqUnit objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.DaqUnit
        logging.info('Restoring %d DaqUnit object(s)' % tbl.nrows)
        id2dbid_DaqUnit = {0:0}
        for row in tbl:
            obj = SenStore.DaqUnitFields()
            if 'Model' in tbl.colnames:
                obj.mModel = row['Model']
            if 'Identifier' in tbl.colnames:
                obj.mIdentifier = row['Identifier']
            obj.id = mngr.addDaqUnit(obj)
            id2dbid_DaqUnit[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'DaqUnitChannel'):
        logging.info('Skipping DaqUnitChannel objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.DaqUnitChannel
        logging.info('Restoring %d DaqUnitChannel object(s)' % tbl.nrows)
        id2dbid_DaqUnitChannel = {0:0}
        for row in tbl:
            obj = SenStore.DaqUnitChannelFields()
            if 'Number' in tbl.colnames:
                obj.mNumber = row['Number']
            obj.id = mngr.addDaqUnitChannel(obj)
            id2dbid_DaqUnitChannel[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'Sensor'):
        logging.info('Skipping Sensor objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.Sensor
        logging.info('Restoring %d Sensor object(s)' % tbl.nrows)
        id2dbid_Sensor = {0:0}
        for row in tbl:
            obj = SenStore.SensorFields()
            if 'Type' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.SensorTypeEnumList[row['Type']]
                obj.mType = SenStore.SensorType.__dict__[enumLabel]
            if 'Model' in tbl.colnames:
                obj.mModel = row['Model']
            if 'Identifier' in tbl.colnames:
                obj.mIdentifier = row['Identifier']
            obj.id = mngr.addSensor(obj)
            id2dbid_Sensor[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'Transducer'):
        logging.info('Skipping Transducer objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.Transducer
        logging.info('Restoring %d Transducer object(s)' % tbl.nrows)
        id2dbid_Transducer = {0:0}
        for row in tbl:
            obj = SenStore.TransducerFields()
            if 'Model' in tbl.colnames:
                obj.mModel = row['Model']
            if 'Identifier' in tbl.colnames:
                obj.mIdentifier = row['Identifier']
            obj.id = mngr.addTransducer(obj)
            id2dbid_Transducer[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'Experiment'):
        logging.info('Skipping Experiment objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.Experiment
        logging.info('Restoring %d Experiment object(s)' % tbl.nrows)
        id2dbid_Experiment = {0:0}
        for row in tbl:
            obj = SenStore.ExperimentFields()
            if 'Frequency' in tbl.colnames:
                obj.mFrequency = row['Frequency']
            if 'Duration' in tbl.colnames:
                obj.mDuration = row['Duration']
            if 'BufferData' in tbl.colnames:
                obj.mBufferData = row['BufferData']
            obj.id = mngr.addExperiment(obj)
            id2dbid_Experiment[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'Cycle'):
        logging.info('Skipping Cycle objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.Cycle
        logging.info('Restoring %d Cycle object(s)' % tbl.nrows)
        id2dbid_Cycle = {0:0}
        for row in tbl:
            obj = SenStore.CycleFields()
            if 'Time' in tbl.colnames:
                obj.mTime = row['Time']
            obj.id = mngr.addCycle(obj)
            id2dbid_Cycle[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'SensorTimeData'):
        logging.info('Skipping SensorTimeData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.SensorTimeData
        logging.info('Restoring %d SensorTimeData object(s)' % tbl.nrows)
        id2dbid_SensorTimeData = {0:0}
        for row in tbl:
            obj = SenStore.SensorTimeDataFields()
            if 'Channel' in tbl.colnames:
                obj.mChannel = row['Channel']
            if 'StartTime' in tbl.colnames:
                obj.mStartTime = row['StartTime']
            if 'Status' in tbl.colnames:
                obj.mStatus = row['Status']
            obj.id = mngr.addSensorTimeData(obj)
            id2dbid_SensorTimeData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'TransducerTimeData'):
        logging.info('Skipping TransducerTimeData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.TransducerTimeData
        logging.info('Restoring %d TransducerTimeData object(s)' % tbl.nrows)
        id2dbid_TransducerTimeData = {0:0}
        for row in tbl:
            obj = SenStore.TransducerTimeDataFields()
            if 'Orientation' in tbl.colnames:
                obj.mOrientation = row['Orientation']
            if 'Amplification' in tbl.colnames:
                obj.mAmplification = row['Amplification']
            if 'Channel' in tbl.colnames:
                obj.mChannel = row['Channel']
            if 'Filter' in tbl.colnames:
                obj.mFilter = row['Filter']
            if 'StartTime' in tbl.colnames:
                obj.mStartTime = row['StartTime']
            if 'Status' in tbl.colnames:
                obj.mStatus = row['Status']
            obj.id = mngr.addTransducerTimeData(obj)
            id2dbid_TransducerTimeData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'ExperimentStructureData'):
        logging.info('Skipping ExperimentStructureData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.ExperimentStructureData
        logging.info('Restoring %d ExperimentStructureData object(s)' % tbl.nrows)
        id2dbid_ExperimentStructureData = {0:0}
        for row in tbl:
            obj = SenStore.ExperimentStructureDataFields()
            if 'StartTime' in tbl.colnames:
                obj.mStartTime = row['StartTime']
            if 'Status' in tbl.colnames:
                obj.mStatus = row['Status']
            obj.id = mngr.addExperimentStructureData(obj)
            id2dbid_ExperimentStructureData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'TransducerCycleData'):
        logging.info('Skipping TransducerCycleData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.TransducerCycleData
        logging.info('Restoring %d TransducerCycleData object(s)' % tbl.nrows)
        id2dbid_TransducerCycleData = {0:0}
        for row in tbl:
            obj = SenStore.TransducerCycleDataFields()
            if 'SoftwareScaling' in tbl.colnames:
                obj.mSoftwareScaling = row['SoftwareScaling']
            obj.id = mngr.addTransducerCycleData(obj)
            id2dbid_TransducerCycleData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'MappingMatrix'):
        logging.info('Skipping MappingMatrix objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.MappingMatrix
        logging.info('Restoring %d MappingMatrix object(s)' % tbl.nrows)
        id2dbid_MappingMatrix = {0:0}
        for row in tbl:
            obj = SenStore.MappingMatrixFields()
            if 'Name' in tbl.colnames:
                obj.mName = row['Name']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            if 'OutputQuantity' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.QuantityEnumList[row['OutputQuantity']]
                obj.mOutputQuantity = SenStore.Quantity.__dict__[enumLabel]
            if 'InputQuantity' in tbl.colnames:
                # Convert enumeration references to indices
                enumLabel = f.QuantityEnumList[row['InputQuantity']]
                obj.mInputQuantity = SenStore.Quantity.__dict__[enumLabel]
            obj.id = mngr.addMappingMatrix(obj)
            id2dbid_MappingMatrix[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'DaqUnitChannelData'):
        logging.info('Skipping DaqUnitChannelData objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.DaqUnitChannelData
        logging.info('Restoring %d DaqUnitChannelData object(s)' % tbl.nrows)
        id2dbid_DaqUnitChannelData = {0:0}
        for row in tbl:
            obj = SenStore.DaqUnitChannelDataFields()
            obj.id = mngr.addDaqUnitChannelData(obj)
            id2dbid_DaqUnitChannelData[row['id']] = obj.id
    if not nodeExists(f.h5.root.objects, 'AccelMeas'):
        logging.info('Skipping AccelMeas objects (not present in backup file)')
    else:
        tbl = f.h5.root.objects.AccelMeas
        logging.info('Restoring %d AccelMeas object(s)' % tbl.nrows)
        id2dbid_AccelMeas = {0:0}
        for row in tbl:
            obj = SenStore.AccelMeasFields()
            if 'Name' in tbl.colnames:
                obj.mName = row['Name']
            if 'Description' in tbl.colnames:
                obj.mDescription = row['Description']
            obj.id = mngr.addAccelMeas(obj)
            id2dbid_AccelMeas[row['id']] = obj.id
    # Restore objects references
    if nodeExists(f.h5.root.objects, 'Structure'):
        tbl = f.h5.root.objects.Structure
        for row in tbl:
            dbid = id2dbid_Structure[row['id']]
            obj = mngr.getStructureFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setStructureFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMDof'):
        tbl = f.h5.root.objects.FEMDof
        for row in tbl:
            dbid = id2dbid_FEMDof[row['id']]
            obj = mngr.getFEMDofFields(dbid)
            fieldNames = []
            if 'Structure' in tbl.colnames:
                obj.mStructure = id2dbid_Structure[row['Structure']]
                fieldNames.append('Structure')
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMDofFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMNodalMass'):
        tbl = f.h5.root.objects.FEMNodalMass
        for row in tbl:
            dbid = id2dbid_FEMNodalMass[row['id']]
            obj = mngr.getFEMNodalMassFields(dbid)
            fieldNames = []
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMNodalMassFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMNLElasticStrainStress'):
        tbl = f.h5.root.objects.FEMNLElasticStrainStress
        for row in tbl:
            dbid = id2dbid_FEMNLElasticStrainStress[row['id']]
            obj = mngr.getFEMNLElasticStrainStressFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMNLElasticStrainStressFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMBoundary'):
        tbl = f.h5.root.objects.FEMBoundary
        for row in tbl:
            dbid = id2dbid_FEMBoundary[row['id']]
            obj = mngr.getFEMBoundaryFields(dbid)
            fieldNames = []
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMBoundaryFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSectionPipe'):
        tbl = f.h5.root.objects.FEMSectionPipe
        for row in tbl:
            dbid = id2dbid_FEMSectionPipe[row['id']]
            obj = mngr.getFEMSectionPipeFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMSectionPipeFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMCoordSystem'):
        tbl = f.h5.root.objects.FEMCoordSystem
        for row in tbl:
            dbid = id2dbid_FEMCoordSystem[row['id']]
            obj = mngr.getFEMCoordSystemFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMCoordSystemFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMNode'):
        tbl = f.h5.root.objects.FEMNode
        for row in tbl:
            dbid = id2dbid_FEMNode[row['id']]
            obj = mngr.getFEMNodeFields(dbid)
            fieldNames = []
            if 'System' in tbl.colnames:
                obj.mSystem = id2dbid_FEMCoordSystem[row['System']]
                fieldNames.append('System')
            if len(fieldNames) > 0:
                mngr.setFEMNodeFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMTruss'):
        tbl = f.h5.root.objects.FEMTruss
        for row in tbl:
            dbid = id2dbid_FEMTruss[row['id']]
            obj = mngr.getFEMTrussFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'Material' in tbl.colnames:
                obj.mMaterial = id2dbid_FEMMaterial[row['Material']]
                fieldNames.append('Material')
            if 'N1' in tbl.colnames:
                obj.mN1 = id2dbid_FEMNode[row['N1']]
                fieldNames.append('N1')
            if 'N2' in tbl.colnames:
                obj.mN2 = id2dbid_FEMNode[row['N2']]
                fieldNames.append('N2')
            if len(fieldNames) > 0:
                mngr.setFEMTrussFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMTimeFunctionData'):
        tbl = f.h5.root.objects.FEMTimeFunctionData
        for row in tbl:
            dbid = id2dbid_FEMTimeFunctionData[row['id']]
            obj = mngr.getFEMTimeFunctionDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMTimeFunctionDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMPlasticMlMaterials'):
        tbl = f.h5.root.objects.FEMPlasticMlMaterials
        for row in tbl:
            dbid = id2dbid_FEMPlasticMlMaterials[row['id']]
            obj = mngr.getFEMPlasticMlMaterialsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMPlasticMlMaterialsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMPlateGroup'):
        tbl = f.h5.root.objects.FEMPlateGroup
        for row in tbl:
            dbid = id2dbid_FEMPlateGroup[row['id']]
            obj = mngr.getFEMPlateGroupFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMPlateGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMBeam'):
        tbl = f.h5.root.objects.FEMBeam
        for row in tbl:
            dbid = id2dbid_FEMBeam[row['id']]
            obj = mngr.getFEMBeamFields(dbid)
            fieldNames = []
            if 'AuxNode' in tbl.colnames:
                obj.mAuxNode = id2dbid_FEMNode[row['AuxNode']]
                fieldNames.append('AuxNode')
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'Node1' in tbl.colnames:
                obj.mNode1 = id2dbid_FEMNode[row['Node1']]
                fieldNames.append('Node1')
            if 'Node2' in tbl.colnames:
                obj.mNode2 = id2dbid_FEMNode[row['Node2']]
                fieldNames.append('Node2')
            if len(fieldNames) > 0:
                mngr.setFEMBeamFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMCurvMomentData'):
        tbl = f.h5.root.objects.FEMCurvMomentData
        for row in tbl:
            dbid = id2dbid_FEMCurvMomentData[row['id']]
            obj = mngr.getFEMCurvMomentDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMCurvMomentDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMPropertysets'):
        tbl = f.h5.root.objects.FEMPropertysets
        for row in tbl:
            dbid = id2dbid_FEMPropertysets[row['id']]
            obj = mngr.getFEMPropertysetsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMPropertysetsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMOrthotropicMaterial'):
        tbl = f.h5.root.objects.FEMOrthotropicMaterial
        for row in tbl:
            dbid = id2dbid_FEMOrthotropicMaterial[row['id']]
            obj = mngr.getFEMOrthotropicMaterialFields(dbid)
            fieldNames = []
            if 'Material' in tbl.colnames:
                obj.mMaterial = id2dbid_FEMMaterial[row['Material']]
                fieldNames.append('Material')
            if len(fieldNames) > 0:
                mngr.setFEMOrthotropicMaterialFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMAppliedLoads'):
        tbl = f.h5.root.objects.FEMAppliedLoads
        for row in tbl:
            dbid = id2dbid_FEMAppliedLoads[row['id']]
            obj = mngr.getFEMAppliedLoadsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMAppliedLoadsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMThermoOrthData'):
        tbl = f.h5.root.objects.FEMThermoOrthData
        for row in tbl:
            dbid = id2dbid_FEMThermoOrthData[row['id']]
            obj = mngr.getFEMThermoOrthDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMThermoOrthDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMContactPairs'):
        tbl = f.h5.root.objects.FEMContactPairs
        for row in tbl:
            dbid = id2dbid_FEMContactPairs[row['id']]
            obj = mngr.getFEMContactPairsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMContactPairsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMGeneral'):
        tbl = f.h5.root.objects.FEMGeneral
        for row in tbl:
            dbid = id2dbid_FEMGeneral[row['id']]
            obj = mngr.getFEMGeneralFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMGeneralFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMBeamNode'):
        tbl = f.h5.root.objects.FEMBeamNode
        for row in tbl:
            dbid = id2dbid_FEMBeamNode[row['id']]
            obj = mngr.getFEMBeamNodeFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMBeamNodeFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSectionRect'):
        tbl = f.h5.root.objects.FEMSectionRect
        for row in tbl:
            dbid = id2dbid_FEMSectionRect[row['id']]
            obj = mngr.getFEMSectionRectFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMSectionRectFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMBeamLoad'):
        tbl = f.h5.root.objects.FEMBeamLoad
        for row in tbl:
            dbid = id2dbid_FEMBeamLoad[row['id']]
            obj = mngr.getFEMBeamLoadFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMBeamLoadFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMLoadMassProportional'):
        tbl = f.h5.root.objects.FEMLoadMassProportional
        for row in tbl:
            dbid = id2dbid_FEMLoadMassProportional[row['id']]
            obj = mngr.getFEMLoadMassProportionalFields(dbid)
            fieldNames = []
            if 'Structure' in tbl.colnames:
                obj.mStructure = id2dbid_Structure[row['Structure']]
                fieldNames.append('Structure')
            if len(fieldNames) > 0:
                mngr.setFEMLoadMassProportionalFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMLink'):
        tbl = f.h5.root.objects.FEMLink
        for row in tbl:
            dbid = id2dbid_FEMLink[row['id']]
            obj = mngr.getFEMLinkFields(dbid)
            fieldNames = []
            if 'MasterNode' in tbl.colnames:
                obj.mMasterNode = id2dbid_FEMNode[row['MasterNode']]
                fieldNames.append('MasterNode')
            if 'SlaveNode' in tbl.colnames:
                obj.mSlaveNode = id2dbid_FEMNode[row['SlaveNode']]
                fieldNames.append('SlaveNode')
            if len(fieldNames) > 0:
                mngr.setFEMLinkFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMAxesNode'):
        tbl = f.h5.root.objects.FEMAxesNode
        for row in tbl:
            dbid = id2dbid_FEMAxesNode[row['id']]
            obj = mngr.getFEMAxesNodeFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'Node1' in tbl.colnames:
                obj.mNode1 = id2dbid_FEMNode[row['Node1']]
                fieldNames.append('Node1')
            if 'Node2' in tbl.colnames:
                obj.mNode2 = id2dbid_FEMNode[row['Node2']]
                fieldNames.append('Node2')
            if 'Node3' in tbl.colnames:
                obj.mNode3 = id2dbid_FEMNode[row['Node3']]
                fieldNames.append('Node3')
            if len(fieldNames) > 0:
                mngr.setFEMAxesNodeFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMNMTimeMass'):
        tbl = f.h5.root.objects.FEMNMTimeMass
        for row in tbl:
            dbid = id2dbid_FEMNMTimeMass[row['id']]
            obj = mngr.getFEMNMTimeMassFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMNMTimeMassFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMAppliedDisplacement'):
        tbl = f.h5.root.objects.FEMAppliedDisplacement
        for row in tbl:
            dbid = id2dbid_FEMAppliedDisplacement[row['id']]
            obj = mngr.getFEMAppliedDisplacementFields(dbid)
            fieldNames = []
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMAppliedDisplacementFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMTimeFunctions'):
        tbl = f.h5.root.objects.FEMTimeFunctions
        for row in tbl:
            dbid = id2dbid_FEMTimeFunctions[row['id']]
            obj = mngr.getFEMTimeFunctionsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMTimeFunctionsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMForceStrainData'):
        tbl = f.h5.root.objects.FEMForceStrainData
        for row in tbl:
            dbid = id2dbid_FEMForceStrainData[row['id']]
            obj = mngr.getFEMForceStrainDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMForceStrainDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSkewDOF'):
        tbl = f.h5.root.objects.FEMSkewDOF
        for row in tbl:
            dbid = id2dbid_FEMSkewDOF[row['id']]
            obj = mngr.getFEMSkewDOFFields(dbid)
            fieldNames = []
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMSkewDOFFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSectionI'):
        tbl = f.h5.root.objects.FEMSectionI
        for row in tbl:
            dbid = id2dbid_FEMSectionI[row['id']]
            obj = mngr.getFEMSectionIFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMSectionIFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMPlasticBilinearMaterial'):
        tbl = f.h5.root.objects.FEMPlasticBilinearMaterial
        for row in tbl:
            dbid = id2dbid_FEMPlasticBilinearMaterial[row['id']]
            obj = mngr.getFEMPlasticBilinearMaterialFields(dbid)
            fieldNames = []
            if 'Material' in tbl.colnames:
                obj.mMaterial = id2dbid_FEMMaterial[row['Material']]
                fieldNames.append('Material')
            if len(fieldNames) > 0:
                mngr.setFEMPlasticBilinearMaterialFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMMTForceData'):
        tbl = f.h5.root.objects.FEMMTForceData
        for row in tbl:
            dbid = id2dbid_FEMMTForceData[row['id']]
            obj = mngr.getFEMMTForceDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMMTForceDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMShellPressure'):
        tbl = f.h5.root.objects.FEMShellPressure
        for row in tbl:
            dbid = id2dbid_FEMShellPressure[row['id']]
            obj = mngr.getFEMShellPressureFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMShellPressureFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMMatrices'):
        tbl = f.h5.root.objects.FEMMatrices
        for row in tbl:
            dbid = id2dbid_FEMMatrices[row['id']]
            obj = mngr.getFEMMatricesFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMMatricesFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMDamping'):
        tbl = f.h5.root.objects.FEMDamping
        for row in tbl:
            dbid = id2dbid_FEMDamping[row['id']]
            obj = mngr.getFEMDampingFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMDampingFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMMaterial'):
        tbl = f.h5.root.objects.FEMMaterial
        for row in tbl:
            dbid = id2dbid_FEMMaterial[row['id']]
            obj = mngr.getFEMMaterialFields(dbid)
            fieldNames = []
            if 'Structure' in tbl.colnames:
                obj.mStructure = id2dbid_Structure[row['Structure']]
                fieldNames.append('Structure')
            if len(fieldNames) > 0:
                mngr.setFEMMaterialFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMMatrixData'):
        tbl = f.h5.root.objects.FEMMatrixData
        for row in tbl:
            dbid = id2dbid_FEMMatrixData[row['id']]
            obj = mngr.getFEMMatrixDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMMatrixDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMShellAxesOrtho'):
        tbl = f.h5.root.objects.FEMShellAxesOrtho
        for row in tbl:
            dbid = id2dbid_FEMShellAxesOrtho[row['id']]
            obj = mngr.getFEMShellAxesOrthoFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMShellAxesOrthoFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMEndreleases'):
        tbl = f.h5.root.objects.FEMEndreleases
        for row in tbl:
            dbid = id2dbid_FEMEndreleases[row['id']]
            obj = mngr.getFEMEndreleasesFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMEndreleasesFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMTrussGroup'):
        tbl = f.h5.root.objects.FEMTrussGroup
        for row in tbl:
            dbid = id2dbid_FEMTrussGroup[row['id']]
            obj = mngr.getFEMTrussGroupFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'Material' in tbl.colnames:
                obj.mMaterial = id2dbid_FEMMaterial[row['Material']]
                fieldNames.append('Material')
            if len(fieldNames) > 0:
                mngr.setFEMTrussGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMInitialTemperature'):
        tbl = f.h5.root.objects.FEMInitialTemperature
        for row in tbl:
            dbid = id2dbid_FEMInitialTemperature[row['id']]
            obj = mngr.getFEMInitialTemperatureFields(dbid)
            fieldNames = []
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMInitialTemperatureFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMThermoIsoMaterials'):
        tbl = f.h5.root.objects.FEMThermoIsoMaterials
        for row in tbl:
            dbid = id2dbid_FEMThermoIsoMaterials[row['id']]
            obj = mngr.getFEMThermoIsoMaterialsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMThermoIsoMaterialsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMThermoIsoData'):
        tbl = f.h5.root.objects.FEMThermoIsoData
        for row in tbl:
            dbid = id2dbid_FEMThermoIsoData[row['id']]
            obj = mngr.getFEMThermoIsoDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMThermoIsoDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMContactGroup3'):
        tbl = f.h5.root.objects.FEMContactGroup3
        for row in tbl:
            dbid = id2dbid_FEMContactGroup3[row['id']]
            obj = mngr.getFEMContactGroup3Fields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMContactGroup3Fields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMNLElasticMaterials'):
        tbl = f.h5.root.objects.FEMNLElasticMaterials
        for row in tbl:
            dbid = id2dbid_FEMNLElasticMaterials[row['id']]
            obj = mngr.getFEMNLElasticMaterialsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMNLElasticMaterialsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMPlate'):
        tbl = f.h5.root.objects.FEMPlate
        for row in tbl:
            dbid = id2dbid_FEMPlate[row['id']]
            obj = mngr.getFEMPlateFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'N1' in tbl.colnames:
                obj.mN1 = id2dbid_FEMNode[row['N1']]
                fieldNames.append('N1')
            if 'N2' in tbl.colnames:
                obj.mN2 = id2dbid_FEMNode[row['N2']]
                fieldNames.append('N2')
            if 'N3' in tbl.colnames:
                obj.mN3 = id2dbid_FEMNode[row['N3']]
                fieldNames.append('N3')
            if len(fieldNames) > 0:
                mngr.setFEMPlateFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMIsoBeam'):
        tbl = f.h5.root.objects.FEMIsoBeam
        for row in tbl:
            dbid = id2dbid_FEMIsoBeam[row['id']]
            obj = mngr.getFEMIsoBeamFields(dbid)
            fieldNames = []
            if 'AUX' in tbl.colnames:
                obj.mAUX = id2dbid_FEMNode[row['AUX']]
                fieldNames.append('AUX')
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'N1' in tbl.colnames:
                obj.mN1 = id2dbid_FEMNode[row['N1']]
                fieldNames.append('N1')
            if 'N2' in tbl.colnames:
                obj.mN2 = id2dbid_FEMNode[row['N2']]
                fieldNames.append('N2')
            if 'N3' in tbl.colnames:
                obj.mN3 = id2dbid_FEMNode[row['N3']]
                fieldNames.append('N3')
            if 'N4' in tbl.colnames:
                obj.mN4 = id2dbid_FEMNode[row['N4']]
                fieldNames.append('N4')
            if len(fieldNames) > 0:
                mngr.setFEMIsoBeamFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMAppliedConcentratedLoad'):
        tbl = f.h5.root.objects.FEMAppliedConcentratedLoad
        for row in tbl:
            dbid = id2dbid_FEMAppliedConcentratedLoad[row['id']]
            obj = mngr.getFEMAppliedConcentratedLoadFields(dbid)
            fieldNames = []
            if 'NodeAux' in tbl.colnames:
                obj.mNodeAux = id2dbid_FEMNode[row['NodeAux']]
                fieldNames.append('NodeAux')
            if 'NodeID' in tbl.colnames:
                obj.mNodeID = id2dbid_FEMNode[row['NodeID']]
                fieldNames.append('NodeID')
            if len(fieldNames) > 0:
                mngr.setFEMAppliedConcentratedLoadFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMTwoDSolidGroup'):
        tbl = f.h5.root.objects.FEMTwoDSolidGroup
        for row in tbl:
            dbid = id2dbid_FEMTwoDSolidGroup[row['id']]
            obj = mngr.getFEMTwoDSolidGroupFields(dbid)
            fieldNames = []
            if 'AuxNode' in tbl.colnames:
                obj.mAuxNode = id2dbid_FEMNode[row['AuxNode']]
                fieldNames.append('AuxNode')
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMTwoDSolidGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMGroup'):
        tbl = f.h5.root.objects.FEMGroup
        for row in tbl:
            dbid = id2dbid_FEMGroup[row['id']]
            obj = mngr.getFEMGroupFields(dbid)
            fieldNames = []
            if 'Structure' in tbl.colnames:
                obj.mStructure = id2dbid_Structure[row['Structure']]
                fieldNames.append('Structure')
            if len(fieldNames) > 0:
                mngr.setFEMGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMProperties'):
        tbl = f.h5.root.objects.FEMProperties
        for row in tbl:
            dbid = id2dbid_FEMProperties[row['id']]
            obj = mngr.getFEMPropertiesFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMPropertiesFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMThreeDSolidGroup'):
        tbl = f.h5.root.objects.FEMThreeDSolidGroup
        for row in tbl:
            dbid = id2dbid_FEMThreeDSolidGroup[row['id']]
            obj = mngr.getFEMThreeDSolidGroupFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMThreeDSolidGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMThreeDSolid'):
        tbl = f.h5.root.objects.FEMThreeDSolid
        for row in tbl:
            dbid = id2dbid_FEMThreeDSolid[row['id']]
            obj = mngr.getFEMThreeDSolidFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'N1' in tbl.colnames:
                obj.mN1 = id2dbid_FEMNode[row['N1']]
                fieldNames.append('N1')
            if 'N10' in tbl.colnames:
                obj.mN10 = id2dbid_FEMNode[row['N10']]
                fieldNames.append('N10')
            if 'N11' in tbl.colnames:
                obj.mN11 = id2dbid_FEMNode[row['N11']]
                fieldNames.append('N11')
            if 'N12' in tbl.colnames:
                obj.mN12 = id2dbid_FEMNode[row['N12']]
                fieldNames.append('N12')
            if 'N13' in tbl.colnames:
                obj.mN13 = id2dbid_FEMNode[row['N13']]
                fieldNames.append('N13')
            if 'N14' in tbl.colnames:
                obj.mN14 = id2dbid_FEMNode[row['N14']]
                fieldNames.append('N14')
            if 'N15' in tbl.colnames:
                obj.mN15 = id2dbid_FEMNode[row['N15']]
                fieldNames.append('N15')
            if 'N16' in tbl.colnames:
                obj.mN16 = id2dbid_FEMNode[row['N16']]
                fieldNames.append('N16')
            if 'N17' in tbl.colnames:
                obj.mN17 = id2dbid_FEMNode[row['N17']]
                fieldNames.append('N17')
            if 'N18' in tbl.colnames:
                obj.mN18 = id2dbid_FEMNode[row['N18']]
                fieldNames.append('N18')
            if 'N19' in tbl.colnames:
                obj.mN19 = id2dbid_FEMNode[row['N19']]
                fieldNames.append('N19')
            if 'N2' in tbl.colnames:
                obj.mN2 = id2dbid_FEMNode[row['N2']]
                fieldNames.append('N2')
            if 'N20' in tbl.colnames:
                obj.mN20 = id2dbid_FEMNode[row['N20']]
                fieldNames.append('N20')
            if 'N21' in tbl.colnames:
                obj.mN21 = id2dbid_FEMNode[row['N21']]
                fieldNames.append('N21')
            if 'N22' in tbl.colnames:
                obj.mN22 = id2dbid_FEMNode[row['N22']]
                fieldNames.append('N22')
            if 'N23' in tbl.colnames:
                obj.mN23 = id2dbid_FEMNode[row['N23']]
                fieldNames.append('N23')
            if 'N24' in tbl.colnames:
                obj.mN24 = id2dbid_FEMNode[row['N24']]
                fieldNames.append('N24')
            if 'N25' in tbl.colnames:
                obj.mN25 = id2dbid_FEMNode[row['N25']]
                fieldNames.append('N25')
            if 'N26' in tbl.colnames:
                obj.mN26 = id2dbid_FEMNode[row['N26']]
                fieldNames.append('N26')
            if 'N27' in tbl.colnames:
                obj.mN27 = id2dbid_FEMNode[row['N27']]
                fieldNames.append('N27')
            if 'N3' in tbl.colnames:
                obj.mN3 = id2dbid_FEMNode[row['N3']]
                fieldNames.append('N3')
            if 'N4' in tbl.colnames:
                obj.mN4 = id2dbid_FEMNode[row['N4']]
                fieldNames.append('N4')
            if 'N5' in tbl.colnames:
                obj.mN5 = id2dbid_FEMNode[row['N5']]
                fieldNames.append('N5')
            if 'N6' in tbl.colnames:
                obj.mN6 = id2dbid_FEMNode[row['N6']]
                fieldNames.append('N6')
            if 'N7' in tbl.colnames:
                obj.mN7 = id2dbid_FEMNode[row['N7']]
                fieldNames.append('N7')
            if 'N8' in tbl.colnames:
                obj.mN8 = id2dbid_FEMNode[row['N8']]
                fieldNames.append('N8')
            if 'N9' in tbl.colnames:
                obj.mN9 = id2dbid_FEMNode[row['N9']]
                fieldNames.append('N9')
            if len(fieldNames) > 0:
                mngr.setFEMThreeDSolidFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSectionProp'):
        tbl = f.h5.root.objects.FEMSectionProp
        for row in tbl:
            dbid = id2dbid_FEMSectionProp[row['id']]
            obj = mngr.getFEMSectionPropFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMSectionPropFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMElasticMaterial'):
        tbl = f.h5.root.objects.FEMElasticMaterial
        for row in tbl:
            dbid = id2dbid_FEMElasticMaterial[row['id']]
            obj = mngr.getFEMElasticMaterialFields(dbid)
            fieldNames = []
            if 'Material' in tbl.colnames:
                obj.mMaterial = id2dbid_FEMMaterial[row['Material']]
                fieldNames.append('Material')
            if len(fieldNames) > 0:
                mngr.setFEMElasticMaterialFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMPoints'):
        tbl = f.h5.root.objects.FEMPoints
        for row in tbl:
            dbid = id2dbid_FEMPoints[row['id']]
            obj = mngr.getFEMPointsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMPointsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMThermoOrthMaterials'):
        tbl = f.h5.root.objects.FEMThermoOrthMaterials
        for row in tbl:
            dbid = id2dbid_FEMThermoOrthMaterials[row['id']]
            obj = mngr.getFEMThermoOrthMaterialsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMThermoOrthMaterialsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMConstraints'):
        tbl = f.h5.root.objects.FEMConstraints
        for row in tbl:
            dbid = id2dbid_FEMConstraints[row['id']]
            obj = mngr.getFEMConstraintsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMConstraintsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMMCrigidities'):
        tbl = f.h5.root.objects.FEMMCrigidities
        for row in tbl:
            dbid = id2dbid_FEMMCrigidities[row['id']]
            obj = mngr.getFEMMCrigiditiesFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMMCrigiditiesFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSkeySysNode'):
        tbl = f.h5.root.objects.FEMSkeySysNode
        for row in tbl:
            dbid = id2dbid_FEMSkeySysNode[row['id']]
            obj = mngr.getFEMSkeySysNodeFields(dbid)
            fieldNames = []
            if 'Node1' in tbl.colnames:
                obj.mNode1 = id2dbid_FEMNode[row['Node1']]
                fieldNames.append('Node1')
            if 'Node2' in tbl.colnames:
                obj.mNode2 = id2dbid_FEMNode[row['Node2']]
                fieldNames.append('Node2')
            if 'Node3' in tbl.colnames:
                obj.mNode3 = id2dbid_FEMNode[row['Node3']]
                fieldNames.append('Node3')
            if len(fieldNames) > 0:
                mngr.setFEMSkeySysNodeFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMIsoBeamGroup'):
        tbl = f.h5.root.objects.FEMIsoBeamGroup
        for row in tbl:
            dbid = id2dbid_FEMIsoBeamGroup[row['id']]
            obj = mngr.getFEMIsoBeamGroupFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMIsoBeamGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMShellDOF'):
        tbl = f.h5.root.objects.FEMShellDOF
        for row in tbl:
            dbid = id2dbid_FEMShellDOF[row['id']]
            obj = mngr.getFEMShellDOFFields(dbid)
            fieldNames = []
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMShellDOFFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMCrossSections'):
        tbl = f.h5.root.objects.FEMCrossSections
        for row in tbl:
            dbid = id2dbid_FEMCrossSections[row['id']]
            obj = mngr.getFEMCrossSectionsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMCrossSectionsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMTwistMomentData'):
        tbl = f.h5.root.objects.FEMTwistMomentData
        for row in tbl:
            dbid = id2dbid_FEMTwistMomentData[row['id']]
            obj = mngr.getFEMTwistMomentDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMTwistMomentDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMShell'):
        tbl = f.h5.root.objects.FEMShell
        for row in tbl:
            dbid = id2dbid_FEMShell[row['id']]
            obj = mngr.getFEMShellFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'Material' in tbl.colnames:
                obj.mMaterial = id2dbid_FEMMaterial[row['Material']]
                fieldNames.append('Material')
            if 'N1' in tbl.colnames:
                obj.mN1 = id2dbid_FEMNode[row['N1']]
                fieldNames.append('N1')
            if 'N2' in tbl.colnames:
                obj.mN2 = id2dbid_FEMNode[row['N2']]
                fieldNames.append('N2')
            if 'N3' in tbl.colnames:
                obj.mN3 = id2dbid_FEMNode[row['N3']]
                fieldNames.append('N3')
            if 'N4' in tbl.colnames:
                obj.mN4 = id2dbid_FEMNode[row['N4']]
                fieldNames.append('N4')
            if 'N5' in tbl.colnames:
                obj.mN5 = id2dbid_FEMNode[row['N5']]
                fieldNames.append('N5')
            if 'N6' in tbl.colnames:
                obj.mN6 = id2dbid_FEMNode[row['N6']]
                fieldNames.append('N6')
            if 'N7' in tbl.colnames:
                obj.mN7 = id2dbid_FEMNode[row['N7']]
                fieldNames.append('N7')
            if 'N8' in tbl.colnames:
                obj.mN8 = id2dbid_FEMNode[row['N8']]
                fieldNames.append('N8')
            if 'N9' in tbl.colnames:
                obj.mN9 = id2dbid_FEMNode[row['N9']]
                fieldNames.append('N9')
            if len(fieldNames) > 0:
                mngr.setFEMShellFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMNTNContact'):
        tbl = f.h5.root.objects.FEMNTNContact
        for row in tbl:
            dbid = id2dbid_FEMNTNContact[row['id']]
            obj = mngr.getFEMNTNContactFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMNTNContactFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMShellLayer'):
        tbl = f.h5.root.objects.FEMShellLayer
        for row in tbl:
            dbid = id2dbid_FEMShellLayer[row['id']]
            obj = mngr.getFEMShellLayerFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMShellLayerFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSkewSysAngles'):
        tbl = f.h5.root.objects.FEMSkewSysAngles
        for row in tbl:
            dbid = id2dbid_FEMSkewSysAngles[row['id']]
            obj = mngr.getFEMSkewSysAnglesFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMSkewSysAnglesFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMGroundMotionRecord'):
        tbl = f.h5.root.objects.FEMGroundMotionRecord
        for row in tbl:
            dbid = id2dbid_FEMGroundMotionRecord[row['id']]
            obj = mngr.getFEMGroundMotionRecordFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMGroundMotionRecordFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMGeneralGroup'):
        tbl = f.h5.root.objects.FEMGeneralGroup
        for row in tbl:
            dbid = id2dbid_FEMGeneralGroup[row['id']]
            obj = mngr.getFEMGeneralGroupFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMGeneralGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMTwoDSolid'):
        tbl = f.h5.root.objects.FEMTwoDSolid
        for row in tbl:
            dbid = id2dbid_FEMTwoDSolid[row['id']]
            obj = mngr.getFEMTwoDSolidFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'N1' in tbl.colnames:
                obj.mN1 = id2dbid_FEMNode[row['N1']]
                fieldNames.append('N1')
            if 'N2' in tbl.colnames:
                obj.mN2 = id2dbid_FEMNode[row['N2']]
                fieldNames.append('N2')
            if 'N3' in tbl.colnames:
                obj.mN3 = id2dbid_FEMNode[row['N3']]
                fieldNames.append('N3')
            if 'N4' in tbl.colnames:
                obj.mN4 = id2dbid_FEMNode[row['N4']]
                fieldNames.append('N4')
            if 'N5' in tbl.colnames:
                obj.mN5 = id2dbid_FEMNode[row['N5']]
                fieldNames.append('N5')
            if 'N6' in tbl.colnames:
                obj.mN6 = id2dbid_FEMNode[row['N6']]
                fieldNames.append('N6')
            if 'N7' in tbl.colnames:
                obj.mN7 = id2dbid_FEMNode[row['N7']]
                fieldNames.append('N7')
            if 'N8' in tbl.colnames:
                obj.mN8 = id2dbid_FEMNode[row['N8']]
                fieldNames.append('N8')
            if 'N9' in tbl.colnames:
                obj.mN9 = id2dbid_FEMNode[row['N9']]
                fieldNames.append('N9')
            if len(fieldNames) > 0:
                mngr.setFEMTwoDSolidFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMAppliedTemperature'):
        tbl = f.h5.root.objects.FEMAppliedTemperature
        for row in tbl:
            dbid = id2dbid_FEMAppliedTemperature[row['id']]
            obj = mngr.getFEMAppliedTemperatureFields(dbid)
            fieldNames = []
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMAppliedTemperatureFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMMatrixSets'):
        tbl = f.h5.root.objects.FEMMatrixSets
        for row in tbl:
            dbid = id2dbid_FEMMatrixSets[row['id']]
            obj = mngr.getFEMMatrixSetsFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMMatrixSetsFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMConstraintCoef'):
        tbl = f.h5.root.objects.FEMConstraintCoef
        for row in tbl:
            dbid = id2dbid_FEMConstraintCoef[row['id']]
            obj = mngr.getFEMConstraintCoefFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMConstraintCoefFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSectionBox'):
        tbl = f.h5.root.objects.FEMSectionBox
        for row in tbl:
            dbid = id2dbid_FEMSectionBox[row['id']]
            obj = mngr.getFEMSectionBoxFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMSectionBoxFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMNKDisplForce'):
        tbl = f.h5.root.objects.FEMNKDisplForce
        for row in tbl:
            dbid = id2dbid_FEMNKDisplForce[row['id']]
            obj = mngr.getFEMNKDisplForceFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMNKDisplForceFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMPlasticStrainStress'):
        tbl = f.h5.root.objects.FEMPlasticStrainStress
        for row in tbl:
            dbid = id2dbid_FEMPlasticStrainStress[row['id']]
            obj = mngr.getFEMPlasticStrainStressFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMPlasticStrainStressFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMShellAxesOrthoData'):
        tbl = f.h5.root.objects.FEMShellAxesOrthoData
        for row in tbl:
            dbid = id2dbid_FEMShellAxesOrthoData[row['id']]
            obj = mngr.getFEMShellAxesOrthoDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMShellAxesOrthoDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMGeneralNode'):
        tbl = f.h5.root.objects.FEMGeneralNode
        for row in tbl:
            dbid = id2dbid_FEMGeneralNode[row['id']]
            obj = mngr.getFEMGeneralNodeFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setFEMGeneralNodeFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMStrLines'):
        tbl = f.h5.root.objects.FEMStrLines
        for row in tbl:
            dbid = id2dbid_FEMStrLines[row['id']]
            obj = mngr.getFEMStrLinesFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMStrLinesFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMContactSurface'):
        tbl = f.h5.root.objects.FEMContactSurface
        for row in tbl:
            dbid = id2dbid_FEMContactSurface[row['id']]
            obj = mngr.getFEMContactSurfaceFields(dbid)
            fieldNames = []
            if 'N1' in tbl.colnames:
                obj.mN1 = id2dbid_FEMNode[row['N1']]
                fieldNames.append('N1')
            if 'N2' in tbl.colnames:
                obj.mN2 = id2dbid_FEMNode[row['N2']]
                fieldNames.append('N2')
            if 'N3' in tbl.colnames:
                obj.mN3 = id2dbid_FEMNode[row['N3']]
                fieldNames.append('N3')
            if 'N4' in tbl.colnames:
                obj.mN4 = id2dbid_FEMNode[row['N4']]
                fieldNames.append('N4')
            if len(fieldNames) > 0:
                mngr.setFEMContactSurfaceFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMMCForceData'):
        tbl = f.h5.root.objects.FEMMCForceData
        for row in tbl:
            dbid = id2dbid_FEMMCForceData[row['id']]
            obj = mngr.getFEMMCForceDataFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setFEMMCForceDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSpring'):
        tbl = f.h5.root.objects.FEMSpring
        for row in tbl:
            dbid = id2dbid_FEMSpring[row['id']]
            obj = mngr.getFEMSpringFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'N1' in tbl.colnames:
                obj.mN1 = id2dbid_FEMNode[row['N1']]
                fieldNames.append('N1')
            if 'N2' in tbl.colnames:
                obj.mN2 = id2dbid_FEMNode[row['N2']]
                fieldNames.append('N2')
            if len(fieldNames) > 0:
                mngr.setFEMSpringFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMSpringGroup'):
        tbl = f.h5.root.objects.FEMSpringGroup
        for row in tbl:
            dbid = id2dbid_FEMSpringGroup[row['id']]
            obj = mngr.getFEMSpringGroupFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if len(fieldNames) > 0:
                mngr.setFEMSpringGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'FEMShellGroup'):
        tbl = f.h5.root.objects.FEMShellGroup
        for row in tbl:
            dbid = id2dbid_FEMShellGroup[row['id']]
            obj = mngr.getFEMShellGroupFields(dbid)
            fieldNames = []
            if 'Group' in tbl.colnames:
                obj.mGroup = id2dbid_FEMGroup[row['Group']]
                fieldNames.append('Group')
            if 'Material' in tbl.colnames:
                obj.mMaterial = id2dbid_FEMMaterial[row['Material']]
                fieldNames.append('Material')
            if len(fieldNames) > 0:
                mngr.setFEMShellGroupFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'DaqUnit'):
        tbl = f.h5.root.objects.DaqUnit
        for row in tbl:
            dbid = id2dbid_DaqUnit[row['id']]
            obj = mngr.getDaqUnitFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setDaqUnitFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'DaqUnitChannel'):
        tbl = f.h5.root.objects.DaqUnitChannel
        for row in tbl:
            dbid = id2dbid_DaqUnitChannel[row['id']]
            obj = mngr.getDaqUnitChannelFields(dbid)
            fieldNames = []
            if 'Unit' in tbl.colnames:
                obj.mUnit = id2dbid_DaqUnit[row['Unit']]
                fieldNames.append('Unit')
            if len(fieldNames) > 0:
                mngr.setDaqUnitChannelFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'Sensor'):
        tbl = f.h5.root.objects.Sensor
        for row in tbl:
            dbid = id2dbid_Sensor[row['id']]
            obj = mngr.getSensorFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setSensorFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'Transducer'):
        tbl = f.h5.root.objects.Transducer
        for row in tbl:
            dbid = id2dbid_Transducer[row['id']]
            obj = mngr.getTransducerFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setTransducerFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'Experiment'):
        tbl = f.h5.root.objects.Experiment
        for row in tbl:
            dbid = id2dbid_Experiment[row['id']]
            obj = mngr.getExperimentFields(dbid)
            fieldNames = []
            if len(fieldNames) > 0:
                mngr.setExperimentFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'Cycle'):
        tbl = f.h5.root.objects.Cycle
        for row in tbl:
            dbid = id2dbid_Cycle[row['id']]
            obj = mngr.getCycleFields(dbid)
            fieldNames = []
            if 'Experiment' in tbl.colnames:
                obj.mExperiment = id2dbid_Experiment[row['Experiment']]
                fieldNames.append('Experiment')
            if len(fieldNames) > 0:
                mngr.setCycleFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'SensorTimeData'):
        tbl = f.h5.root.objects.SensorTimeData
        for row in tbl:
            dbid = id2dbid_SensorTimeData[row['id']]
            obj = mngr.getSensorTimeDataFields(dbid)
            fieldNames = []
            if 'Structure' in tbl.colnames:
                obj.mStructure = id2dbid_Structure[row['Structure']]
                fieldNames.append('Structure')
            if 'Sensor' in tbl.colnames:
                obj.mSensor = id2dbid_Sensor[row['Sensor']]
                fieldNames.append('Sensor')
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setSensorTimeDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'TransducerTimeData'):
        tbl = f.h5.root.objects.TransducerTimeData
        for row in tbl:
            dbid = id2dbid_TransducerTimeData[row['id']]
            obj = mngr.getTransducerTimeDataFields(dbid)
            fieldNames = []
            if 'Transducer' in tbl.colnames:
                obj.mTransducer = id2dbid_Transducer[row['Transducer']]
                fieldNames.append('Transducer')
            if 'Sensor' in tbl.colnames:
                obj.mSensor = id2dbid_Sensor[row['Sensor']]
                fieldNames.append('Sensor')
            if 'Node' in tbl.colnames:
                obj.mNode = id2dbid_FEMNode[row['Node']]
                fieldNames.append('Node')
            if len(fieldNames) > 0:
                mngr.setTransducerTimeDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'ExperimentStructureData'):
        tbl = f.h5.root.objects.ExperimentStructureData
        for row in tbl:
            dbid = id2dbid_ExperimentStructureData[row['id']]
            obj = mngr.getExperimentStructureDataFields(dbid)
            fieldNames = []
            if 'Experiment' in tbl.colnames:
                obj.mExperiment = id2dbid_Experiment[row['Experiment']]
                fieldNames.append('Experiment')
            if 'Structure' in tbl.colnames:
                obj.mStructure = id2dbid_Structure[row['Structure']]
                fieldNames.append('Structure')
            if len(fieldNames) > 0:
                mngr.setExperimentStructureDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'TransducerCycleData'):
        tbl = f.h5.root.objects.TransducerCycleData
        for row in tbl:
            dbid = id2dbid_TransducerCycleData[row['id']]
            obj = mngr.getTransducerCycleDataFields(dbid)
            fieldNames = []
            if 'Transducer' in tbl.colnames:
                obj.mTransducer = id2dbid_Transducer[row['Transducer']]
                fieldNames.append('Transducer')
            if 'Cycle' in tbl.colnames:
                obj.mCycle = id2dbid_Cycle[row['Cycle']]
                fieldNames.append('Cycle')
            if len(fieldNames) > 0:
                mngr.setTransducerCycleDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'MappingMatrix'):
        tbl = f.h5.root.objects.MappingMatrix
        for row in tbl:
            dbid = id2dbid_MappingMatrix[row['id']]
            obj = mngr.getMappingMatrixFields(dbid)
            fieldNames = []
            if 'Structure' in tbl.colnames:
                obj.mStructure = id2dbid_Structure[row['Structure']]
                fieldNames.append('Structure')
            if len(fieldNames) > 0:
                mngr.setMappingMatrixFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'DaqUnitChannelData'):
        tbl = f.h5.root.objects.DaqUnitChannelData
        for row in tbl:
            dbid = id2dbid_DaqUnitChannelData[row['id']]
            obj = mngr.getDaqUnitChannelDataFields(dbid)
            fieldNames = []
            if 'Channel' in tbl.colnames:
                obj.mChannel = id2dbid_DaqUnitChannel[row['Channel']]
                fieldNames.append('Channel')
            if len(fieldNames) > 0:
                mngr.setDaqUnitChannelDataFields(obj, fieldNames)
    if nodeExists(f.h5.root.objects, 'AccelMeas'):
        tbl = f.h5.root.objects.AccelMeas
        for row in tbl:
            dbid = id2dbid_AccelMeas[row['id']]
            obj = mngr.getAccelMeasFields(dbid)
            fieldNames = []
            if 'Sensor' in tbl.colnames:
                obj.mSensor = id2dbid_Sensor[row['Sensor']]
                fieldNames.append('Sensor')
            if len(fieldNames) > 0:
                mngr.setAccelMeasFields(obj, fieldNames)
    # Restore array and signal data
    if nodeExists(f.h5.root.objects, 'MappingMatrix'):
        tbl = f.h5.root.objects.MappingMatrix
        for row in tbl:
            id = row['id']
            dbid = id2dbid_MappingMatrix[id]
            prx = mngr.getMappingMatrix(dbid)
            # Create array
            data = f.getMappingMatrixArrayData(id)
            dims = data.shape
            logging.info('Restoring MappingMatrix[oid=%d] array data' % id)
            prx.createArray(list(dims))
            for slices in slicing.slicer(dims, CHUNK_ELEM_COUNT):
                dataChunk = data.__getitem__(tuple(slices)).flatten().tolist()
                # Create slice array for proxy call
                prxSlices = slicing.slice2db(SenStore, slices)
                prx.setArrayData(prxSlices, dataChunk)
            # Restore axes
            axis = f.getMappingMatrixOutputNodeAxis(id)
            if axis:
                data = axis.read()
                # Convert local object IDs to database object references
                for i,objId in enumerate(data):
                    data[i] = id2dbid_FEMNode[objId]
                prx.setOutputNodeAxis(data)
            axis = f.getMappingMatrixInputNodeAxis(id)
            if axis:
                data = axis.read()
                # Convert local object IDs to database object references
                for i,objId in enumerate(data):
                    data[i] = id2dbid_FEMNode[objId]
                prx.setInputNodeAxis(data)
    if nodeExists(f.h5.root.objects, 'DaqUnitChannelData'):
        tbl = f.h5.root.objects.DaqUnitChannelData
        for row in tbl:
            id = row['id']
            dbid = id2dbid_DaqUnitChannelData[id]
            prx = mngr.getDaqUnitChannelData(dbid)
            # Create signal
            data = f.getDaqUnitChannelDataSignalData(id)
            t = f.getDaqUnitChannelDataSignalTimeAxis(id)
            logging.info('Restoring DaqUnitChannelData[oid=%d] signal data' % id)
            dims = data.shape
            prx.createSignal(list(dims[1:]))
            idxStart = 0
            idxStep = 1000
            while idxStart < dims[0]:
                idxEnd = idxStart + idxStep
                if idxEnd > dims[0]:
                    idxEnd = dims[0]+1
                dataChunk = data[idxStart:idxEnd].flatten().tolist()
                tChunk = t[idxStart:idxEnd].tolist()
                prx.appendToSignal(tChunk, dataChunk)
                idxStart += idxStep
            # Restore axes
            axis = f.getDaqUnitChannelDataSubChannelsAxis(id)
            if axis:
                data = axis.read()
                # Convert enumeration IDs to enumeration objects
                for i,enumId in enumerate(data):
                    # Convert enumeration references to enums
                    enumLabel = f.QuantityEnumList[enumId]
                    data[i] = SenStore.Quantity.__dict__[enumLabel]
                prx.setSubChannelsAxis(data)
    if nodeExists(f.h5.root.objects, 'AccelMeas'):
        tbl = f.h5.root.objects.AccelMeas
        for row in tbl:
            id = row['id']
            dbid = id2dbid_AccelMeas[id]
            prx = mngr.getAccelMeas(dbid)
            # Create signal
            data = f.getAccelMeasSignalData(id)
            t = f.getAccelMeasSignalTimeAxis(id)
            logging.info('Restoring AccelMeas[oid=%d] signal data' % id)
            dims = data.shape
            prx.createSignal(list(dims[1:]))
            idxStart = 0
            idxStep = 1000
            while idxStart < dims[0]:
                idxEnd = idxStart + idxStep
                if idxEnd > dims[0]:
                    idxEnd = dims[0]+1
                dataChunk = data[idxStart:idxEnd].flatten().tolist()
                tChunk = t[idxStart:idxEnd].tolist()
                prx.appendToSignal(tChunk, dataChunk)
                idxStart += idxStep
            # Restore axes
            axis = f.getAccelMeasDirectionAxis(id)
            if axis:
                data = axis.read()
                # Convert enumeration IDs to enumeration objects
                for i,enumId in enumerate(data):
                    # Convert enumeration references to enums
                    enumLabel = f.QuantityEnumList[enumId]
                    data[i] = SenStore.Quantity.__dict__[enumLabel]
                prx.setDirectionAxis(data)
    # Exit cleanly, so other clients can get access right away
    client.close()
    f.close()
