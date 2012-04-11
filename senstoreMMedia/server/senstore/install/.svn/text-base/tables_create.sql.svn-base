CREATE TABLE ENUM_MaterialType (
    ObjectID SERIAL,
    Label TEXT,
    PRIMARY KEY (ObjectID),
    UNIQUE (Label)
);
CREATE TABLE ENUM_Unit (
    ObjectID SERIAL,
    Label TEXT,
    PRIMARY KEY (ObjectID),
    UNIQUE (Label)
);
CREATE TABLE ENUM_Quantity (
    ObjectID SERIAL,
    Label TEXT,
    PRIMARY KEY (ObjectID),
    UNIQUE (Label)
);
CREATE TABLE ENUM_CoordinateSystemType (
    ObjectID SERIAL,
    Label TEXT,
    PRIMARY KEY (ObjectID),
    UNIQUE (Label)
);
CREATE TABLE ENUM_BoundaryType (
    ObjectID SERIAL,
    Label TEXT,
    PRIMARY KEY (ObjectID),
    UNIQUE (Label)
);
CREATE TABLE ENUM_SectionType (
    ObjectID SERIAL,
    Label TEXT,
    PRIMARY KEY (ObjectID),
    UNIQUE (Label)
);
CREATE TABLE ENUM_GroupType (
    ObjectID SERIAL,
    Label TEXT,
    PRIMARY KEY (ObjectID),
    UNIQUE (Label)
);
CREATE TABLE ENUM_SensorType (
    ObjectID SERIAL,
    Label TEXT,
    PRIMARY KEY (ObjectID),
    UNIQUE (Label)
);
INSERT INTO ENUM_MaterialType (Label) VALUES ('ELASTIC');
INSERT INTO ENUM_MaterialType (Label) VALUES ('ORTHOTROPIC');
INSERT INTO ENUM_MaterialType (Label) VALUES ('NONLINEARELASTIC');
INSERT INTO ENUM_MaterialType (Label) VALUES ('PLASTICBILINEAR');
INSERT INTO ENUM_MaterialType (Label) VALUES ('PLASTICMULTILINEAR');
INSERT INTO ENUM_MaterialType (Label) VALUES ('THERMOISOTROPIC');
INSERT INTO ENUM_MaterialType (Label) VALUES ('THERMOORTHOTROPIC');
INSERT INTO ENUM_Unit (Label) VALUES ('METER');
INSERT INTO ENUM_Unit (Label) VALUES ('KILOGRAM');
INSERT INTO ENUM_Unit (Label) VALUES ('NEWTON');
INSERT INTO ENUM_Unit (Label) VALUES ('FOOT');
INSERT INTO ENUM_Unit (Label) VALUES ('POUND');
INSERT INTO ENUM_Unit (Label) VALUES ('KILOPOUND');
INSERT INTO ENUM_Unit (Label) VALUES ('SLUG');
INSERT INTO ENUM_Unit (Label) VALUES ('KILOSLUG');
INSERT INTO ENUM_Quantity (Label) VALUES ('XDISPLACEMENT');
INSERT INTO ENUM_Quantity (Label) VALUES ('YDISPLACEMENT');
INSERT INTO ENUM_Quantity (Label) VALUES ('ZDISPLACEMENT');
INSERT INTO ENUM_Quantity (Label) VALUES ('XROTATION');
INSERT INTO ENUM_Quantity (Label) VALUES ('YROTATION');
INSERT INTO ENUM_Quantity (Label) VALUES ('ZROTATION');
INSERT INTO ENUM_Quantity (Label) VALUES ('XFORCE');
INSERT INTO ENUM_Quantity (Label) VALUES ('YFORCE');
INSERT INTO ENUM_Quantity (Label) VALUES ('ZFORCE');
INSERT INTO ENUM_Quantity (Label) VALUES ('RAW');
INSERT INTO ENUM_CoordinateSystemType (Label) VALUES ('CARTESIAN');
INSERT INTO ENUM_CoordinateSystemType (Label) VALUES ('SPHERICAL');
INSERT INTO ENUM_CoordinateSystemType (Label) VALUES ('CYLINDRICAL');
INSERT INTO ENUM_BoundaryType (Label) VALUES ('FREE');
INSERT INTO ENUM_BoundaryType (Label) VALUES ('FIXED');
INSERT INTO ENUM_SectionType (Label) VALUES ('BOX');
INSERT INTO ENUM_SectionType (Label) VALUES ('PROPERTIES');
INSERT INTO ENUM_SectionType (Label) VALUES ('RECTANGULAR');
INSERT INTO ENUM_SectionType (Label) VALUES ('PIPE');
INSERT INTO ENUM_SectionType (Label) VALUES ('I');
INSERT INTO ENUM_SectionType (Label) VALUES ('L');
INSERT INTO ENUM_SectionType (Label) VALUES ('U');
INSERT INTO ENUM_GroupType (Label) VALUES ('BEAM');
INSERT INTO ENUM_GroupType (Label) VALUES ('TRUSS');
INSERT INTO ENUM_GroupType (Label) VALUES ('GENERAL');
INSERT INTO ENUM_GroupType (Label) VALUES ('ISOBEAM');
INSERT INTO ENUM_GroupType (Label) VALUES ('PLATE');
INSERT INTO ENUM_GroupType (Label) VALUES ('SHELL');
INSERT INTO ENUM_GroupType (Label) VALUES ('SPRING');
INSERT INTO ENUM_GroupType (Label) VALUES ('THREEDSOLID');
INSERT INTO ENUM_GroupType (Label) VALUES ('TWODSOLID');
INSERT INTO ENUM_SensorType (Label) VALUES ('ACCELEROMETER');
INSERT INTO ENUM_SensorType (Label) VALUES ('ANEMOMETER');
INSERT INTO ENUM_SensorType (Label) VALUES ('WINDVANE');
INSERT INTO ENUM_SensorType (Label) VALUES ('STRAINGAUGE');
CREATE TABLE CLASS_Structure (
    ObjectID SERIAL,
    var_Name TEXT,
    var_Description TEXT,
    var_Type TEXT,
    ref_DistanceUnit INT4,
    ref_ForceUnit INT4,
    ref_WeightUnit INT4,
    FOREIGN KEY (ref_DistanceUnit) REFERENCES ENUM_Unit (ObjectID),
    FOREIGN KEY (ref_ForceUnit) REFERENCES ENUM_Unit (ObjectID),
    FOREIGN KEY (ref_WeightUnit) REFERENCES ENUM_Unit (ObjectID),
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMDof (
    ObjectID SERIAL,
    ref_Structure INT4,
    var_LocalId INT4,
    ref_Node INT4,
    ref_Direction INT4,
    FOREIGN KEY (ref_Direction) REFERENCES ENUM_Quantity (ObjectID),
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMNodalMass (
    ObjectID SERIAL,
    var_Description TEXT,
    var_Mass1 FLOAT8,
    var_Mass2 FLOAT8,
    var_Mass3 FLOAT8,
    var_Mass4 FLOAT8,
    var_Mass5 FLOAT8,
    var_Mass6 FLOAT8,
    ref_Node INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMNLElasticStrainStress (
    ObjectID SERIAL,
    var_MaterialID INT4,
    var_RecordNumber INT4,
    var_Strain FLOAT8,
    var_Stress FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMBoundary (
    ObjectID SERIAL,
    ref_Node INT4,
    ref_Ovalization INT4,
    ref_Phi INT4,
    ref_Rx INT4,
    ref_Ry INT4,
    ref_Rz INT4,
    ref_Ux INT4,
    ref_Uy INT4,
    ref_Uz INT4,
    var_Warping TEXT,
    FOREIGN KEY (ref_Ovalization) REFERENCES ENUM_BoundaryType (ObjectID),
    FOREIGN KEY (ref_Phi) REFERENCES ENUM_BoundaryType (ObjectID),
    FOREIGN KEY (ref_Rx) REFERENCES ENUM_BoundaryType (ObjectID),
    FOREIGN KEY (ref_Ry) REFERENCES ENUM_BoundaryType (ObjectID),
    FOREIGN KEY (ref_Rz) REFERENCES ENUM_BoundaryType (ObjectID),
    FOREIGN KEY (ref_Ux) REFERENCES ENUM_BoundaryType (ObjectID),
    FOREIGN KEY (ref_Uy) REFERENCES ENUM_BoundaryType (ObjectID),
    FOREIGN KEY (ref_Uz) REFERENCES ENUM_BoundaryType (ObjectID),
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSectionPipe (
    ObjectID SERIAL,
    var_Diameter FLOAT8,
    var_SC FLOAT8,
    var_SectionID INT4,
    var_SSarea FLOAT8,
    var_TC FLOAT8,
    var_Thickness FLOAT8,
    var_Torfac FLOAT8,
    var_TSarea FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMCoordSystem (
    ObjectID SERIAL,
    var_AX FLOAT8,
    var_AY FLOAT8,
    var_AZ FLOAT8,
    var_BBY FLOAT8,
    var_BX FLOAT8,
    var_BZ FLOAT8,
    var_Description TEXT,
    var_Mode INT2,
    var_P1 INT4,
    var_P2 INT4,
    var_P3 INT4,
    var_Type TEXT,
    var_Xorigin FLOAT8,
    var_Yorigin FLOAT8,
    var_Zorigin FLOAT8,
    var_LocalID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMNode (
    ObjectID SERIAL,
    var_Description TEXT,
    ref_System INT4,
    var_X FLOAT8,
    var_Y FLOAT8,
    var_Z FLOAT8,
    var_LocalID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMTruss (
    ObjectID SERIAL,
    var_Description TEXT,
    var_ElementID INT4,
    var_Epsin FLOAT8,
    var_Gapwidth FLOAT8,
    ref_Group INT4,
    ref_Material INT4,
    ref_N1 INT4,
    ref_N2 INT4,
    var_RecordNmb INT4,
    var_Save TEXT,
    var_SectionArea FLOAT8,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMTimeFunctionData (
    ObjectID SERIAL,
    var_DataTime FLOAT8,
    var_GMRecordID INT2,
    var_RecordNmb INT4,
    var_TimeFunctionID INT4,
    var_TimeValue FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMPlasticMlMaterials (
    ObjectID SERIAL,
    var_ALPHA FLOAT8,
    var_Density FLOAT8,
    var_E FLOAT8,
    var_Hardening TEXT,
    var_MaterialID INT4,
    var_NU FLOAT8,
    var_TREF FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMPlateGroup (
    ObjectID SERIAL,
    var_Displacement TEXT,
    ref_Group INT4,
    var_IniStrain TEXT,
    var_MaterialID INT4,
    var_Result TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMBeam (
    ObjectID SERIAL,
    ref_AuxNode INT4,
    var_Description TEXT,
    var_ElementID INT4,
    var_EndRelease INT4,
    var_Epsin FLOAT8,
    ref_Group INT4,
    var_IRigidEnd FLOAT8,
    var_JRigidEnd FLOAT8,
    var_MaterialID INT4,
    ref_Node1 INT4,
    ref_Node2 INT4,
    var_RecordNmb INT4,
    var_Save TEXT,
    var_SectionID INT4,
    var_Subdivision INT4,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMCurvMomentData (
    ObjectID SERIAL,
    var_Curvature FLOAT8,
    var_CurvMomentID INT4,
    var_Moment FLOAT8,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMPropertysets (
    ObjectID SERIAL,
    var_C FLOAT8,
    var_Description TEXT,
    var_K FLOAT8,
    var_M FLOAT8,
    var_NC INT4,
    var_NK INT4,
    var_NM INT4,
    var_Nonlinear TEXT,
    var_PropertysetID INT4,
    var_S FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMOrthotropicMaterial (
    ObjectID SERIAL,
    var_Density FLOAT8,
    var_EA FLOAT8,
    var_EB FLOAT8,
    var_EC FLOAT8,
    var_GAB FLOAT8,
    var_GAC FLOAT8,
    var_GBC FLOAT8,
    ref_Material INT4,
    var_NUAB FLOAT8,
    var_NUAC FLOAT8,
    var_NUBC FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMAppliedLoads (
    ObjectID SERIAL,
    var_AppliedLoadNmb INT4,
    var_ArrivalTime FLOAT8,
    var_LoadID INT2,
    var_LoadType TEXT,
    var_SiteType TEXT,
    var_TimeFunctionID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMThermoOrthData (
    ObjectID SERIAL,
    var_ALPHAA FLOAT8,
    var_ALPHAB FLOAT8,
    var_ALPHAC FLOAT8,
    var_EA FLOAT8,
    var_EB FLOAT8,
    var_EC FLOAT8,
    var_GAB FLOAT8,
    var_GAC FLOAT8,
    var_GBC FLOAT8,
    var_MaterialID INT4,
    var_NUAB FLOAT8,
    var_NUAC FLOAT8,
    var_NUBC FLOAT8,
    var_RecordNmb INT4,
    var_Theta FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMContactPairs (
    ObjectID SERIAL,
    var_ContactorSurf INT4,
    var_ContGroupID INT4,
    var_ContPair INT4,
    var_FContactor FLOAT8,
    var_Friction FLOAT8,
    var_FTarget FLOAT8,
    var_HeatTransf FLOAT8,
    var_RecordNmb INT4,
    var_TargetSurf INT4,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMGeneral (
    ObjectID SERIAL,
    var_Description TEXT,
    var_ElementID INT4,
    ref_Group INT4,
    var_MatrixSet INT4,
    var_NodeAmount INT2,
    var_RecordNmb INT4,
    var_Save TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMBeamNode (
    ObjectID SERIAL,
    var_Displacement TEXT,
    ref_Group INT4,
    var_IniStrain TEXT,
    var_MaterialID INT4,
    var_MC TEXT,
    var_MCrigidity INT4,
    var_REmultiplyer FLOAT8,
    var_Result TEXT,
    var_REtype TEXT,
    var_RINT INT2,
    var_SectionID INT4,
    var_SINT INT2,
    var_TINT INT2,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSectionRect (
    ObjectID SERIAL,
    var_Height FLOAT8,
    var_IShear TEXT,
    var_SC FLOAT8,
    var_SectionID INT4,
    var_SSarea FLOAT8,
    var_TC FLOAT8,
    var_Torfac FLOAT8,
    var_TSarea FLOAT8,
    var_Width FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMBeamLoad (
    ObjectID SERIAL,
    var_ArrivalTime FLOAT8,
    var_DeformDepend INT2,
    var_DirectFilter INT2,
    var_ElementID INT4,
    var_Face INT2,
    ref_Group INT4,
    var_P1 FLOAT8,
    var_P2 FLOAT8,
    var_RecordNmb INT4,
    var_TimeFunctionID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMLoadMassProportional (
    ObjectID SERIAL,
    var_AX FLOAT8,
    var_AY FLOAT8,
    var_AZ FLOAT8,
    var_LoadID INT2,
    var_Magnitude FLOAT8,
    ref_Structure INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMLink (
    ObjectID SERIAL,
    var_Description TEXT,
    var_Displacement TEXT,
    ref_MasterNode INT4,
    var_RLID INT4,
    ref_SlaveNode INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMAxesNode (
    ObjectID SERIAL,
    var_AxNodeID INT4,
    ref_Group INT4,
    ref_Node1 INT4,
    ref_Node2 INT4,
    ref_Node3 INT4,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMNMTimeMass (
    ObjectID SERIAL,
    var_Mass FLOAT8,
    var_PropertyID INT4,
    var_RecordNmb INT4,
    var_TimeValue FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMAppliedDisplacement (
    ObjectID SERIAL,
    var_ArrivalTime FLOAT8,
    var_Description TEXT,
    var_Direction INT2,
    var_Factor FLOAT8,
    ref_Node INT4,
    var_RecordNmb INT4,
    var_TimeFunctionID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMTimeFunctions (
    ObjectID SERIAL,
    var_Description TEXT,
    var_TimeFunctionID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMForceStrainData (
    ObjectID SERIAL,
    var_Force FLOAT8,
    var_ForceAxID INT4,
    var_RecordNmb INT4,
    var_Strain FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSkewDOF (
    ObjectID SERIAL,
    var_Description TEXT,
    ref_Node INT4,
    var_SkewSystemID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSectionI (
    ObjectID SERIAL,
    var_Height FLOAT8,
    var_SC FLOAT8,
    var_SectionID INT4,
    var_SSarea FLOAT8,
    var_TC FLOAT8,
    var_Thick1 FLOAT8,
    var_Thick2 FLOAT8,
    var_Thick3 FLOAT8,
    var_Torfac FLOAT8,
    var_TSarea FLOAT8,
    var_Width1 FLOAT8,
    var_Width2 FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMPlasticBilinearMaterial (
    ObjectID SERIAL,
    var_Alpha FLOAT8,
    var_Density FLOAT8,
    var_E FLOAT8,
    var_EPA FLOAT8,
    var_ET FLOAT8,
    var_Hardening TEXT,
    ref_Material INT4,
    var_NU FLOAT8,
    var_TRef FLOAT8,
    var_Yield FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMMTForceData (
    ObjectID SERIAL,
    var_Force FLOAT8,
    var_MomentRID INT4,
    var_RecordNmb INT4,
    var_TwistMomentID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMShellPressure (
    ObjectID SERIAL,
    var_ArrivalTime FLOAT8,
    var_DeformDepend INT2,
    var_Description TEXT,
    var_DirectFilter INT2,
    var_ElementID INT4,
    var_Face INT2,
    ref_Group INT4,
    var_Nodaux INT4,
    var_P1 FLOAT8,
    var_P2 FLOAT8,
    var_P3 FLOAT8,
    var_P4 FLOAT8,
    var_RecordNmb INT4,
    var_TimeFunctionID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMMatrices (
    ObjectID SERIAL,
    var_Description TEXT,
    var_Lump INT4,
    var_MatrixID INT4,
    var_MatrixType TEXT,
    var_ND INT4,
    var_NS INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMDamping (
    ObjectID SERIAL,
    var_ALPHA FLOAT8,
    var_BETA FLOAT8,
    ref_Group INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMMaterial (
    ObjectID SERIAL,
    var_Description TEXT,
    ref_MaterialType INT4,
    var_LocalID INT4,
    ref_Structure INT4,
    FOREIGN KEY (ref_MaterialType) REFERENCES ENUM_MaterialType (ObjectID),
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMMatrixData (
    ObjectID SERIAL,
    var_Coeff FLOAT8,
    var_ColumnIndex INT4,
    var_MatrixID INT4,
    var_RecordNmb INT4,
    var_RowIndex INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMShellAxesOrtho (
    ObjectID SERIAL,
    var_ALFA FLOAT8,
    var_AxOrthoID INT4,
    ref_Group INT4,
    var_LineID INT4,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMEndreleases (
    ObjectID SERIAL,
    var_Description TEXT,
    var_EndRelease INT4,
    var_Moment1 INT2,
    var_Moment2 INT2,
    var_Moment3 INT2,
    var_Moment4 INT2,
    var_Moment5 INT2,
    var_Moment6 INT2,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMTrussGroup (
    ObjectID SERIAL,
    var_Displacement TEXT,
    var_GAPS TEXT,
    ref_Group INT4,
    var_IniStrain TEXT,
    ref_Material INT4,
    var_SectionArea FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMInitialTemperature (
    ObjectID SERIAL,
    ref_Node INT4,
    var_Temperature FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMThermoIsoMaterials (
    ObjectID SERIAL,
    var_Density FLOAT8,
    var_MaterialID INT4,
    var_TREF FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMThermoIsoData (
    ObjectID SERIAL,
    var_ALPHA FLOAT8,
    var_E FLOAT8,
    var_MaterialID INT4,
    var_NU FLOAT8,
    var_RecordNmb INT4,
    var_Theta FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMContactGroup3 (
    ObjectID SERIAL,
    var_ContGroupID INT4,
    var_Depth FLOAT8,
    var_Description TEXT,
    var_Forces TEXT,
    var_Friction FLOAT8,
    var_IniPenetration TEXT,
    var_NodeToNode TEXT,
    var_Offset FLOAT8,
    var_PenetrAlgorithm TEXT,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    var_Tied TEXT,
    var_TiedOffset FLOAT8,
    var_Tolerance FLOAT8,
    var_Tractions TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMNLElasticMaterials (
    ObjectID SERIAL,
    var_Dcurve INT4,
    var_Density FLOAT8,
    var_MaterialID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMPlate (
    ObjectID SERIAL,
    var_Description TEXT,
    var_ElementID INT4,
    var_Flex11 FLOAT8,
    var_Flex12 FLOAT8,
    var_Flex22 FLOAT8,
    ref_Group INT4,
    var_MaterialID INT4,
    var_Meps11 FLOAT8,
    var_Meps12 FLOAT8,
    var_Meps22 FLOAT8,
    ref_N1 INT4,
    ref_N2 INT4,
    ref_N3 INT4,
    var_RecordNmb INT4,
    var_Save TEXT,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    var_Thick FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMIsoBeam (
    ObjectID SERIAL,
    ref_AUX INT4,
    var_Description TEXT,
    var_ElementID INT4,
    var_Epaxl FLOAT8,
    var_Ephoop FLOAT8,
    ref_Group INT4,
    var_MaterialID INT4,
    ref_N1 INT4,
    ref_N2 INT4,
    ref_N3 INT4,
    ref_N4 INT4,
    var_NodeAmount INT2,
    var_RecordNmb INT4,
    var_Save TEXT,
    var_SectionID INT4,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMAppliedConcentratedLoad (
    ObjectID SERIAL,
    var_ArrivalTime FLOAT8,
    var_Description TEXT,
    var_Direction INT2,
    var_Factor FLOAT8,
    ref_NodeAux INT4,
    ref_NodeID INT4,
    var_RecordNmb INT4,
    var_TimeFunctionID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMTwoDSolidGroup (
    ObjectID SERIAL,
    ref_AuxNode INT4,
    var_Displacement TEXT,
    ref_Group INT4,
    var_MaterialID INT4,
    var_Result TEXT,
    var_Subtype TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMGroup (
    ObjectID SERIAL,
    var_Description TEXT,
    ref_GroupType INT4,
    var_LocalID INT4,
    ref_Structure INT4,
    FOREIGN KEY (ref_GroupType) REFERENCES ENUM_GroupType (ObjectID),
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMProperties (
    ObjectID SERIAL,
    var_Description TEXT,
    var_PropertyID INT4,
    var_PropertyType TEXT,
    var_Rupture TEXT,
    var_XC FLOAT8,
    var_XN FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMThreeDSolidGroup (
    ObjectID SERIAL,
    var_Displacement TEXT,
    ref_Group INT4,
    var_MaterialID INT4,
    var_Result TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMThreeDSolid (
    ObjectID SERIAL,
    var_Description TEXT,
    var_ElementID INT4,
    ref_Group INT4,
    var_MaterialID INT4,
    var_Maxes INT4,
    ref_N1 INT4,
    ref_N10 INT4,
    ref_N11 INT4,
    ref_N12 INT4,
    ref_N13 INT4,
    ref_N14 INT4,
    ref_N15 INT4,
    ref_N16 INT4,
    ref_N17 INT4,
    ref_N18 INT4,
    ref_N19 INT4,
    ref_N2 INT4,
    ref_N20 INT4,
    ref_N21 INT4,
    ref_N22 INT4,
    ref_N23 INT4,
    ref_N24 INT4,
    ref_N25 INT4,
    ref_N26 INT4,
    ref_N27 INT4,
    ref_N3 INT4,
    ref_N4 INT4,
    ref_N5 INT4,
    ref_N6 INT4,
    ref_N7 INT4,
    ref_N8 INT4,
    ref_N9 INT4,
    var_NodeAmount INT2,
    var_RecordNmb INT4,
    var_Save TEXT,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSectionProp (
    ObjectID SERIAL,
    var_Area FLOAT8,
    var_Rinertia FLOAT8,
    var_Sarea FLOAT8,
    var_SectionID INT4,
    var_Sinertia FLOAT8,
    var_Tarea FLOAT8,
    var_Tinertia FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMElasticMaterial (
    ObjectID SERIAL,
    var_Alpha FLOAT8,
    var_Density FLOAT8,
    var_E FLOAT8,
    ref_Material INT4,
    var_NU FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMPoints (
    ObjectID SERIAL,
    var_Description TEXT,
    var_PointID INT4,
    var_SystemID INT4,
    var_X FLOAT8,
    var_Y FLOAT8,
    var_Z FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMThermoOrthMaterials (
    ObjectID SERIAL,
    var_Density FLOAT8,
    var_MaterialID INT4,
    var_TREF FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMConstraints (
    ObjectID SERIAL,
    var_ConstraintID INT4,
    var_Description TEXT,
    var_SlaveDOF TEXT,
    var_SlaveNode INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMMCrigidities (
    ObjectID SERIAL,
    var_AcurveType TEXT,
    var_Alpha INT4,
    var_AxialCF FLOAT8,
    var_BcurveType TEXT,
    var_BendingCF FLOAT8,
    var_Beta INT4,
    var_Density FLOAT8,
    var_ForceAxID INT4,
    var_Hardening TEXT,
    var_MassArea FLOAT8,
    var_MassR FLOAT8,
    var_MassS FLOAT8,
    var_MassT FLOAT8,
    var_MomentRID INT4,
    var_MomentSID INT4,
    var_MomentTID INT4,
    var_RigidityID INT4,
    var_TcurveType TEXT,
    var_TorsionCF FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSkeySysNode (
    ObjectID SERIAL,
    var_Description TEXT,
    ref_Node1 INT4,
    ref_Node2 INT4,
    ref_Node3 INT4,
    var_SkewSystemID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMIsoBeamGroup (
    ObjectID SERIAL,
    var_Displacement TEXT,
    ref_Group INT4,
    var_IniStrain TEXT,
    var_MaterialID INT4,
    var_Result TEXT,
    var_SectionID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMShellDOF (
    ObjectID SERIAL,
    var_DOFnumber TEXT,
    ref_Node INT4,
    var_VectorID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMCrossSections (
    ObjectID SERIAL,
    var_Description TEXT,
    var_SectionID INT4,
    var_SectionType TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMTwistMomentData (
    ObjectID SERIAL,
    var_Moment FLOAT8,
    var_RecordNmb INT4,
    var_Twist FLOAT8,
    var_TwistMomentID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMShell (
    ObjectID SERIAL,
    var_Description TEXT,
    var_ElementID INT4,
    ref_Group INT4,
    ref_Material INT4,
    ref_N1 INT4,
    ref_N2 INT4,
    ref_N3 INT4,
    ref_N4 INT4,
    ref_N5 INT4,
    ref_N6 INT4,
    ref_N7 INT4,
    ref_N8 INT4,
    ref_N9 INT4,
    var_NodeAmount INT2,
    var_RecordNmb INT4,
    var_Save TEXT,
    var_T1 FLOAT8,
    var_T2 FLOAT8,
    var_T3 FLOAT8,
    var_T4 FLOAT8,
    var_T5 FLOAT8,
    var_T6 FLOAT8,
    var_T7 FLOAT8,
    var_T8 FLOAT8,
    var_T9 FLOAT8,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMNTNContact (
    ObjectID SERIAL,
    var_ContactorNode INT4,
    var_ContGroupID INT4,
    var_ContPair INT4,
    var_PrintRes TEXT,
    var_RecordNmb INT4,
    var_SaveRes TEXT,
    var_TargetNode INT4,
    var_TargetNx FLOAT8,
    var_TargetNy FLOAT8,
    var_TargetNz FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMShellLayer (
    ObjectID SERIAL,
    ref_Group INT4,
    var_LayerNumber INT4,
    var_MaterialID INT4,
    var_PThick FLOAT8,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSkewSysAngles (
    ObjectID SERIAL,
    var_Description TEXT,
    var_PHI FLOAT8,
    var_SkewSystemID INT4,
    var_THETA FLOAT8,
    var_XSI FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMGroundMotionRecord (
    ObjectID SERIAL,
    var_Description TEXT,
    var_GMRecordID INT2,
    var_GMRecordName TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMGeneralGroup (
    ObjectID SERIAL,
    ref_Group INT4,
    var_MatrixSet INT4,
    var_Result TEXT,
    var_SkewSystem TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMTwoDSolid (
    ObjectID SERIAL,
    var_Bet FLOAT8,
    var_Description TEXT,
    var_ElementID INT4,
    ref_Group INT4,
    var_MaterialID INT4,
    ref_N1 INT4,
    ref_N2 INT4,
    ref_N3 INT4,
    ref_N4 INT4,
    ref_N5 INT4,
    ref_N6 INT4,
    ref_N7 INT4,
    ref_N8 INT4,
    ref_N9 INT4,
    var_NodeAmount INT2,
    var_RecordNmb INT4,
    var_Save TEXT,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    var_Thick FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMAppliedTemperature (
    ObjectID SERIAL,
    var_ArrivalTime FLOAT8,
    var_Factor FLOAT8,
    ref_Node INT4,
    var_RecordNmbr INT4,
    var_TimeFunctionID INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMMatrixSets (
    ObjectID SERIAL,
    var_Damping INT4,
    var_Description TEXT,
    var_Mass INT4,
    var_MatrixSetID INT4,
    var_Stiffness INT4,
    var_Stress INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMConstraintCoef (
    ObjectID SERIAL,
    var_Coefficient FLOAT8,
    var_ConstraintID INT4,
    var_Description TEXT,
    var_MasterDOF TEXT,
    var_MasterNode INT4,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSectionBox (
    ObjectID SERIAL,
    var_Height FLOAT8,
    var_SC FLOAT8,
    var_SectionID INT4,
    var_SSarea FLOAT8,
    var_TC FLOAT8,
    var_Thick1 FLOAT8,
    var_Thick2 FLOAT8,
    var_Torfac FLOAT8,
    var_TSarea FLOAT8,
    var_Width FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMNKDisplForce (
    ObjectID SERIAL,
    var_Displacement FLOAT8,
    var_Force FLOAT8,
    var_PropertyID INT4,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMPlasticStrainStress (
    ObjectID SERIAL,
    var_MaterialID INT4,
    var_RecordNumber INT4,
    var_Strain FLOAT8,
    var_Stress FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMShellAxesOrthoData (
    ObjectID SERIAL,
    var_AxOrthoID INT4,
    var_ElementID INT4,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMGeneralNode (
    ObjectID SERIAL,
    var_ElementID INT4,
    ref_Group INT4,
    var_LocalNmb INT2,
    ref_Node INT4,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMStrLines (
    ObjectID SERIAL,
    var_LineID INT4,
    var_P1 INT4,
    var_P2 INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMContactSurface (
    ObjectID SERIAL,
    var_ContGroupID INT4,
    var_ContSegment INT4,
    var_ContSurface INT4,
    ref_N1 INT4,
    ref_N2 INT4,
    ref_N3 INT4,
    ref_N4 INT4,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMMCForceData (
    ObjectID SERIAL,
    var_CurvMomentID INT4,
    var_Force FLOAT8,
    var_MomentSTID INT4,
    var_RecordNmb INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSpring (
    ObjectID SERIAL,
    var_AX FLOAT8,
    var_AY FLOAT8,
    var_AZ FLOAT8,
    var_Description TEXT,
    var_ElementID INT4,
    ref_Group INT4,
    var_ID1 INT2,
    var_ID2 INT2,
    ref_N1 INT4,
    ref_N2 INT4,
    var_PropertySet INT4,
    var_RecordNmb INT4,
    var_Save TEXT,
    var_TBirth FLOAT8,
    var_TDeath FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMSpringGroup (
    ObjectID SERIAL,
    var_Bolt TEXT,
    ref_Group INT4,
    var_Nonlinear TEXT,
    var_PropertySet INT4,
    var_Result TEXT,
    var_SkewSystem TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_FEMShellGroup (
    ObjectID SERIAL,
    var_Displacement TEXT,
    ref_Group INT4,
    ref_Material INT4,
    var_NLayers INT4,
    var_Result TEXT,
    var_SectionResult INT2,
    var_StressReference TEXT,
    var_Thickness FLOAT8,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_DaqUnit (
    ObjectID SERIAL,
    var_Model TEXT,
    var_Identifier TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_DaqUnitChannel (
    ObjectID SERIAL,
    ref_Unit INT4,
    var_Number INT2,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_Sensor (
    ObjectID SERIAL,
    ref_Type INT4,
    var_Model TEXT,
    var_Identifier TEXT,
    FOREIGN KEY (ref_Type) REFERENCES ENUM_SensorType (ObjectID),
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_Transducer (
    ObjectID SERIAL,
    var_Model TEXT,
    var_Identifier TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_Experiment (
    ObjectID SERIAL,
    var_Frequency INT2,
    var_Duration INT4,
    var_BufferData INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_Cycle (
    ObjectID SERIAL,
    var_Time FLOAT8,
    ref_Experiment INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_SensorTimeData (
    ObjectID SERIAL,
    ref_Structure INT4,
    ref_Sensor INT4,
    var_Channel INT2,
    var_StartTime FLOAT8,
    var_Status TEXT,
    ref_Node INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_TransducerTimeData (
    ObjectID SERIAL,
    ref_Transducer INT4,
    ref_Sensor INT4,
    ref_Node INT4,
    var_Orientation INT2,
    var_Amplification INT4,
    var_Channel INT2,
    var_Filter TEXT,
    var_StartTime FLOAT8,
    var_Status TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_ExperimentStructureData (
    ObjectID SERIAL,
    ref_Experiment INT4,
    ref_Structure INT4,
    var_StartTime FLOAT8,
    var_Status TEXT,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_TransducerCycleData (
    ObjectID SERIAL,
    ref_Transducer INT4,
    ref_Cycle INT4,
    var_SoftwareScaling FLOAT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_MappingMatrix (
    ObjectID SERIAL,
    ref_Structure INT4,
    var_Name TEXT,
    var_Description TEXT,
    ref_OutputQuantity INT4,
    ref_InputQuantity INT4,
    FOREIGN KEY (ref_OutputQuantity) REFERENCES ENUM_Quantity (ObjectID),
    FOREIGN KEY (ref_InputQuantity) REFERENCES ENUM_Quantity (ObjectID),
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_DaqUnitChannelData (
    ObjectID SERIAL,
    ref_Channel INT4,
    PRIMARY KEY (ObjectID)
);
CREATE TABLE CLASS_AccelMeas (
    ObjectID SERIAL,
    ref_Sensor INT4,
    var_Name TEXT,
    var_Description TEXT,
    PRIMARY KEY (ObjectID)
);
ALTER TABLE CLASS_FEMDof ADD FOREIGN KEY (ref_Structure) REFERENCES CLASS_Structure (ObjectID);
ALTER TABLE CLASS_FEMDof ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMNodalMass ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMBoundary ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMNode ADD FOREIGN KEY (ref_System) REFERENCES CLASS_FEMCoordSystem (ObjectID);
ALTER TABLE CLASS_FEMTruss ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMTruss ADD FOREIGN KEY (ref_Material) REFERENCES CLASS_FEMMaterial (ObjectID);
ALTER TABLE CLASS_FEMTruss ADD FOREIGN KEY (ref_N1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTruss ADD FOREIGN KEY (ref_N2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMPlateGroup ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMBeam ADD FOREIGN KEY (ref_AuxNode) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMBeam ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMBeam ADD FOREIGN KEY (ref_Node1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMBeam ADD FOREIGN KEY (ref_Node2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMOrthotropicMaterial ADD FOREIGN KEY (ref_Material) REFERENCES CLASS_FEMMaterial (ObjectID);
ALTER TABLE CLASS_FEMGeneral ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMBeamNode ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMBeamLoad ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMLoadMassProportional ADD FOREIGN KEY (ref_Structure) REFERENCES CLASS_Structure (ObjectID);
ALTER TABLE CLASS_FEMLink ADD FOREIGN KEY (ref_MasterNode) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMLink ADD FOREIGN KEY (ref_SlaveNode) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMAxesNode ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMAxesNode ADD FOREIGN KEY (ref_Node1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMAxesNode ADD FOREIGN KEY (ref_Node2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMAxesNode ADD FOREIGN KEY (ref_Node3) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMAppliedDisplacement ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMSkewDOF ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMPlasticBilinearMaterial ADD FOREIGN KEY (ref_Material) REFERENCES CLASS_FEMMaterial (ObjectID);
ALTER TABLE CLASS_FEMShellPressure ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMDamping ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMMaterial ADD FOREIGN KEY (ref_Structure) REFERENCES CLASS_Structure (ObjectID);
ALTER TABLE CLASS_FEMShellAxesOrtho ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMTrussGroup ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMTrussGroup ADD FOREIGN KEY (ref_Material) REFERENCES CLASS_FEMMaterial (ObjectID);
ALTER TABLE CLASS_FEMInitialTemperature ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMPlate ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMPlate ADD FOREIGN KEY (ref_N1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMPlate ADD FOREIGN KEY (ref_N2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMPlate ADD FOREIGN KEY (ref_N3) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMIsoBeam ADD FOREIGN KEY (ref_AUX) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMIsoBeam ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMIsoBeam ADD FOREIGN KEY (ref_N1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMIsoBeam ADD FOREIGN KEY (ref_N2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMIsoBeam ADD FOREIGN KEY (ref_N3) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMIsoBeam ADD FOREIGN KEY (ref_N4) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMAppliedConcentratedLoad ADD FOREIGN KEY (ref_NodeAux) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMAppliedConcentratedLoad ADD FOREIGN KEY (ref_NodeID) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolidGroup ADD FOREIGN KEY (ref_AuxNode) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolidGroup ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMGroup ADD FOREIGN KEY (ref_Structure) REFERENCES CLASS_Structure (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolidGroup ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N10) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N11) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N12) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N13) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N14) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N15) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N16) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N17) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N18) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N19) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N20) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N21) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N22) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N23) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N24) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N25) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N26) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N27) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N3) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N4) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N5) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N6) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N7) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N8) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMThreeDSolid ADD FOREIGN KEY (ref_N9) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMElasticMaterial ADD FOREIGN KEY (ref_Material) REFERENCES CLASS_FEMMaterial (ObjectID);
ALTER TABLE CLASS_FEMSkeySysNode ADD FOREIGN KEY (ref_Node1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMSkeySysNode ADD FOREIGN KEY (ref_Node2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMSkeySysNode ADD FOREIGN KEY (ref_Node3) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMIsoBeamGroup ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMShellDOF ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_Material) REFERENCES CLASS_FEMMaterial (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N3) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N4) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N5) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N6) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N7) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N8) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShell ADD FOREIGN KEY (ref_N9) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMShellLayer ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMGeneralGroup ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N3) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N4) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N5) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N6) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N7) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N8) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMTwoDSolid ADD FOREIGN KEY (ref_N9) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMAppliedTemperature ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMGeneralNode ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMGeneralNode ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMContactSurface ADD FOREIGN KEY (ref_N1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMContactSurface ADD FOREIGN KEY (ref_N2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMContactSurface ADD FOREIGN KEY (ref_N3) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMContactSurface ADD FOREIGN KEY (ref_N4) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMSpring ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMSpring ADD FOREIGN KEY (ref_N1) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMSpring ADD FOREIGN KEY (ref_N2) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_FEMSpringGroup ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMShellGroup ADD FOREIGN KEY (ref_Group) REFERENCES CLASS_FEMGroup (ObjectID);
ALTER TABLE CLASS_FEMShellGroup ADD FOREIGN KEY (ref_Material) REFERENCES CLASS_FEMMaterial (ObjectID);
ALTER TABLE CLASS_DaqUnitChannel ADD FOREIGN KEY (ref_Unit) REFERENCES CLASS_DaqUnit (ObjectID);
ALTER TABLE CLASS_Cycle ADD FOREIGN KEY (ref_Experiment) REFERENCES CLASS_Experiment (ObjectID);
ALTER TABLE CLASS_SensorTimeData ADD FOREIGN KEY (ref_Structure) REFERENCES CLASS_Structure (ObjectID);
ALTER TABLE CLASS_SensorTimeData ADD FOREIGN KEY (ref_Sensor) REFERENCES CLASS_Sensor (ObjectID);
ALTER TABLE CLASS_SensorTimeData ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_TransducerTimeData ADD FOREIGN KEY (ref_Transducer) REFERENCES CLASS_Transducer (ObjectID);
ALTER TABLE CLASS_TransducerTimeData ADD FOREIGN KEY (ref_Sensor) REFERENCES CLASS_Sensor (ObjectID);
ALTER TABLE CLASS_TransducerTimeData ADD FOREIGN KEY (ref_Node) REFERENCES CLASS_FEMNode (ObjectID);
ALTER TABLE CLASS_ExperimentStructureData ADD FOREIGN KEY (ref_Experiment) REFERENCES CLASS_Experiment (ObjectID);
ALTER TABLE CLASS_ExperimentStructureData ADD FOREIGN KEY (ref_Structure) REFERENCES CLASS_Structure (ObjectID);
ALTER TABLE CLASS_TransducerCycleData ADD FOREIGN KEY (ref_Transducer) REFERENCES CLASS_Transducer (ObjectID);
ALTER TABLE CLASS_TransducerCycleData ADD FOREIGN KEY (ref_Cycle) REFERENCES CLASS_Cycle (ObjectID);
ALTER TABLE CLASS_MappingMatrix ADD FOREIGN KEY (ref_Structure) REFERENCES CLASS_Structure (ObjectID);
ALTER TABLE CLASS_DaqUnitChannelData ADD FOREIGN KEY (ref_Channel) REFERENCES CLASS_DaqUnitChannel (ObjectID);
ALTER TABLE CLASS_AccelMeas ADD FOREIGN KEY (ref_Sensor) REFERENCES CLASS_Sensor (ObjectID);

CREATE TABLE EXT_Multimedia (
	id             serial    PRIMARY KEY,

	file_name      text      NOT NULL,
	file_type      text      NOT NULL,
  desc_short     text      NOT NULL,
	desc_long      text      NOT NULL
);
