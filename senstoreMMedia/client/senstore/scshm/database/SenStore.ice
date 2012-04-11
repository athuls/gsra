/* -*- c++ -*- */

/*
 * Copyright (c) 2010 SC Solutions, Inc.
 * See LICENSE.txt.
 * $Id: server-tpl.ice,v 1.11 2010/09/02 00:33:14 glinden Exp $
 *
 * Notice: ZeroC Ice is licensed under GPL, and requires a license when used
 * in closed-source applications.
 */

/**
 * SenStore database
 *
 * ZeroC Ice interface to SenStore cyber-infrastructure server.
 *
 * The [Inspector] interface provides a database schema neutral interface, to
 * retrieve the database definition.  The [SenStoreMngr] interface provides
 * read/write access to the database objects.
 */
module SenStore {
  /** Data type for holding list of dimension sizes or indices. */
  sequence <long> DimensionList;

  /** Data type for holding list of large indices. */
  sequence <long> IndexList;

  /** Supported database object class types. */
  enum ClassType {
    /* Generic object class. */
    ClassTypeCLASS,
    /** Objects including a multi-dimensional array. */
    ClassTypeARRAY,
    /** Objects including a multi-dimensional time signal. */
    ClassTypeSIGNAL,
  };

  /** List of class types. */
  sequence <ClassType> ClassTypeList;

  /** Module for inspecting the database schema. */
  module Info {
    /** Data type for holding list of names. */
    sequence <string> NameList;

    /** Variable (class data member) meta-data. */
    struct VarInfo {
      /** Variable name. */
      string name;
      /** Variable description. */
      string descr;
      /** Variable data type. */
      string varType;
      /** Variable meta type (var, ref or enumref). */
      string className;
    };

    /** List of meta-data of variables. */
    sequence <VarInfo> VarInfoList;

    /** Class meta-data. */
    class ClassInfo {
      /** Class type (class, array or signal). */
      ClassType clsType;
      /** Class name. */
      string name;
      /** Class description. */
      string descr;
      /** Member variables meta-data. */
      VarInfoList vars;
    };

    /** Array meta-data. */
    class ArrayInfo extends ClassInfo {
      /** Data type of array. */
      string dataType;
      /** Array axes meta-data. */
      VarInfoList axes;
      /** Array axes dimensions. */
      DimensionList dims;
    };

    /** Signal meta-data. */
    class SignalInfo extends ArrayInfo {
      /** Dummy member, because extended classes must have at least on additional member. */
      int dummy;
    };

    /**
     * Interface to the database schema.
     *
     * Use this interface to query a database about its defined classes and variables.
     */
    interface Inspector {
      /**
       * Gets the names of the database classes.
       *
       * By specifying the list of class types, a subset of the
       * class types can be retrieved.
       *
       * @param clsTypes list of class types (empty is all)
       */
      idempotent NameList getClassNames(ClassTypeList clsTypes);
      /**
       * Gets the info of the given class.
       *
       * @return class/array/signal info
       */
      idempotent ClassInfo getClassInfo(string name);
    };
  };

  /** Array slice. */
  struct ArraySlice {
    /** Start index (0-based). */
    int start;
    /** Step size (0 indicates all elements and start/end are ignored). */
    int step;
    /** End index (0-based). */
    int end;
  };

  /** Data type for holding list of array slices. */
  sequence <ArraySlice> ArraySliceList;

  /** Data type for holding list of field names. */
  sequence <string> FieldNameList;

  /** Data type for holding list of object IDs. */
  sequence <long> IdList;

  /** Data type for holding list of int32s. */
  sequence <int> Int32List;

  /** Data type for holding list of int16s. */
  sequence <short> Int16List;

  /** Data type for holding list of bools. */
  sequence <bool> BoolList;

  /** Data type for holding list of strings. */
  sequence <string> StringList;

  /** Data type for holding list of float64s. */
  sequence <double> Float64List;

  /** Data type for holding list of timestamps. */
  sequence <double> TimestampList;

  /** Data type for holding list of int64s. */
  sequence <long> Int64List;

  /** Data type for holding list of float32s. */
  sequence <float> Float32List;

  /**
   * Multi-dimensional array data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a = a.reshape(myStruct.shape);
   */
  struct Int32Array {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Int32List data;
  };

  /**
   * Multi-dimensional array data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a = a.reshape(myStruct.shape);
   */
  struct Int16Array {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Int16List data;
  };

  /**
   * Multi-dimensional array data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a = a.reshape(myStruct.shape);
   */
  struct BoolArray {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    BoolList data;
  };

  /**
   * Multi-dimensional array data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a = a.reshape(myStruct.shape);
   */
  struct StringArray {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    StringList data;
  };

  /**
   * Multi-dimensional array data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a = a.reshape(myStruct.shape);
   */
  struct Float64Array {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Float64List data;
  };

  /**
   * Multi-dimensional array data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a = a.reshape(myStruct.shape);
   */
  struct TimestampArray {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    TimestampList data;
  };

  /**
   * Multi-dimensional array data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a = a.reshape(myStruct.shape);
   */
  struct Int64Array {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Int64List data;
  };

  /**
   * Multi-dimensional array data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a = a.reshape(myStruct.shape);
   */
  struct Float32Array {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Float32List data;
  };
  /**
   * Multi-dimensional signal data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a.reshape(myStruct.shape);
   */
  struct Int32Signal {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Int32List data;
    /** Time axis data. */
    TimestampList t;
  };
  /**
   * Multi-dimensional signal data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a.reshape(myStruct.shape);
   */
  struct Int16Signal {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Int16List data;
    /** Time axis data. */
    TimestampList t;
  };
  /**
   * Multi-dimensional signal data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a.reshape(myStruct.shape);
   */
  struct BoolSignal {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    BoolList data;
    /** Time axis data. */
    TimestampList t;
  };
  /**
   * Multi-dimensional signal data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a.reshape(myStruct.shape);
   */
  struct StringSignal {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    StringList data;
    /** Time axis data. */
    TimestampList t;
  };
  /**
   * Multi-dimensional signal data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a.reshape(myStruct.shape);
   */
  struct Float64Signal {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Float64List data;
    /** Time axis data. */
    TimestampList t;
  };
  /**
   * Multi-dimensional signal data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a.reshape(myStruct.shape);
   */
  struct TimestampSignal {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    TimestampList data;
    /** Time axis data. */
    TimestampList t;
  };
  /**
   * Multi-dimensional signal data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a.reshape(myStruct.shape);
   */
  struct Int64Signal {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Int64List data;
    /** Time axis data. */
    TimestampList t;
  };
  /**
   * Multi-dimensional signal data.
   *
   * Note that the vector data must be reshaped to get the
   * multi-dimensional array.  In Python, use:
   * a = numpy.array(myStruct.data);
   * a.reshape(myStruct.shape);
   */
  struct Float32Signal {
    /** Array dimensions (shape). */
    DimensionList shape;
    /** Array data, stored as a vector. */
    Float32List data;
    /** Time axis data. */
    TimestampList t;
  };

  /**
   * Supported material types. 
   */
  enum MaterialType {
    /** Elastic material (default).  */
    MaterialTypeELASTIC,
    /** Orthotropic material.  */
    MaterialTypeORTHOTROPIC,
    /** Nonlinear-elastic material.  */
    MaterialTypeNONLINEARELASTIC,
    /** Plastic bilinear material.  */
    MaterialTypePLASTICBILINEAR,
    /** Plastic muli-linear material.  */
    MaterialTypePLASTICMULTILINEAR,
    /** Thermo isotropic material.  */
    MaterialTypeTHERMOISOTROPIC,
    /** Thermo orthotropic material.  */
    MaterialTypeTHERMOORTHOTROPIC,
  };

  /** Data type for holding list of MaterialTypes. */
  sequence <MaterialType> MaterialTypeList;

  /**
   * Supported units. 
   */
  enum Unit {
    /** Meter (SI) (default).  */
    UnitMETER,
    /** Kilogram (SI).  */
    UnitKILOGRAM,
    /** Newton (SI).  */
    UnitNEWTON,
    /** Foot (1200/3937 meters; imperial).  */
    UnitFOOT,
    /** Pound (international avoirdupois pound; 0.45359237 kilogram).  */
    UnitPOUND,
    /** 1000 Pounds (imperial).  */
    UnitKILOPOUND,
    /** Mass that accelerates by 1 ft/s^2 (imperial)  */
    UnitSLUG,
    /** 1000 Slugs (imperial).  */
    UnitKILOSLUG,
  };

  /** Data type for holding list of Units. */
  sequence <Unit> UnitList;

  /**
   * Available quantities (degrees of freedom). 
   */
  enum Quantity {
    /** X displacement (default).  */
    QuantityXDISPLACEMENT,
    /** Y displacement.  */
    QuantityYDISPLACEMENT,
    /** Z displacement.  */
    QuantityZDISPLACEMENT,
    /** X rotation  */
    QuantityXROTATION,
    /** Y rotation  */
    QuantityYROTATION,
    /** Z rotation  */
    QuantityZROTATION,
    /** X force  */
    QuantityXFORCE,
    /** Y force  */
    QuantityYFORCE,
    /** Z force  */
    QuantityZFORCE,
    /** Raw (undefined).  */
    QuantityRAW,
  };

  /** Data type for holding list of Quantitys. */
  sequence <Quantity> QuantityList;

  /**
   * Supported coordinate system types. 
   */
  enum CoordinateSystemType {
    /** Cartesian coordinate system (default)  */
    CoordinateSystemTypeCARTESIAN,
    /** Spherical coordinate system  */
    CoordinateSystemTypeSPHERICAL,
    /** Cylindrical coordinate system  */
    CoordinateSystemTypeCYLINDRICAL,
  };

  /** Data type for holding list of CoordinateSystemTypes. */
  sequence <CoordinateSystemType> CoordinateSystemTypeList;

  /**
   * Supported boundary types. 
   */
  enum BoundaryType {
    /** Free to move (default).  */
    BoundaryTypeFREE,
    /** Fixed.  */
    BoundaryTypeFIXED,
  };

  /** Data type for holding list of BoundaryTypes. */
  sequence <BoundaryType> BoundaryTypeList;

  /**
   * Supported cross sections. 
   */
  enum SectionType {
    /**  */
    SectionTypeBOX,
    /**  */
    SectionTypePROPERTIES,
    /**  */
    SectionTypeRECTANGULAR,
    /**  */
    SectionTypePIPE,
    /**  */
    SectionTypeI,
    /**  */
    SectionTypeL,
    /**  */
    SectionTypeU,
  };

  /** Data type for holding list of SectionTypes. */
  sequence <SectionType> SectionTypeList;

  /**
   * Supported group types. 
   */
  enum GroupType {
    /**  */
    GroupTypeBEAM,
    /**  */
    GroupTypeTRUSS,
    /**  */
    GroupTypeGENERAL,
    /**  */
    GroupTypeISOBEAM,
    /**  */
    GroupTypePLATE,
    /**  */
    GroupTypeSHELL,
    /**  */
    GroupTypeSPRING,
    /**  */
    GroupTypeTHREEDSOLID,
    /**  */
    GroupTypeTWODSOLID,
  };

  /** Data type for holding list of GroupTypes. */
  sequence <GroupType> GroupTypeList;

  /**
   * Supported sensor types. 
   */
  enum SensorType {
    /** Accelerometer, measuring linear acceleration.  */
    SensorTypeACCELEROMETER,
    /** Anemometer, measuring wind speed.  */
    SensorTypeANEMOMETER,
    /** Wind vane, measuring wind direction.  */
    SensorTypeWINDVANE,
    /** Strain gauge.  */
    SensorTypeSTRAINGAUGE,
  };

  /** Data type for holding list of SensorTypes. */
  sequence <SensorType> SensorTypeList;

  /** Time axis info. */
  struct TimeAxisInfo {
    /** Time vector \[s]. */
    TimestampList t;
    /** Index vector. */
    IndexList idx;
  };

  /** Structure object proxy. */
  interface Structure;
  /** Data type for holding a list of Structure proxies. */
  sequence<Structure*> StructureList;

  /** FEMDof object proxy. */
  interface FEMDof;
  /** Data type for holding a list of FEMDof proxies. */
  sequence<FEMDof*> FEMDofList;

  /** FEMNodalMass object proxy. */
  interface FEMNodalMass;
  /** Data type for holding a list of FEMNodalMass proxies. */
  sequence<FEMNodalMass*> FEMNodalMassList;

  /** FEMNLElasticStrainStress object proxy. */
  interface FEMNLElasticStrainStress;
  /** Data type for holding a list of FEMNLElasticStrainStress proxies. */
  sequence<FEMNLElasticStrainStress*> FEMNLElasticStrainStressList;

  /** FEMBoundary object proxy. */
  interface FEMBoundary;
  /** Data type for holding a list of FEMBoundary proxies. */
  sequence<FEMBoundary*> FEMBoundaryList;

  /** FEMSectionPipe object proxy. */
  interface FEMSectionPipe;
  /** Data type for holding a list of FEMSectionPipe proxies. */
  sequence<FEMSectionPipe*> FEMSectionPipeList;

  /** FEMCoordSystem object proxy. */
  interface FEMCoordSystem;
  /** Data type for holding a list of FEMCoordSystem proxies. */
  sequence<FEMCoordSystem*> FEMCoordSystemList;

  /** FEMNode object proxy. */
  interface FEMNode;
  /** Data type for holding a list of FEMNode proxies. */
  sequence<FEMNode*> FEMNodeList;

  /** FEMTruss object proxy. */
  interface FEMTruss;
  /** Data type for holding a list of FEMTruss proxies. */
  sequence<FEMTruss*> FEMTrussList;

  /** FEMTimeFunctionData object proxy. */
  interface FEMTimeFunctionData;
  /** Data type for holding a list of FEMTimeFunctionData proxies. */
  sequence<FEMTimeFunctionData*> FEMTimeFunctionDataList;

  /** FEMPlasticMlMaterials object proxy. */
  interface FEMPlasticMlMaterials;
  /** Data type for holding a list of FEMPlasticMlMaterials proxies. */
  sequence<FEMPlasticMlMaterials*> FEMPlasticMlMaterialsList;

  /** FEMPlateGroup object proxy. */
  interface FEMPlateGroup;
  /** Data type for holding a list of FEMPlateGroup proxies. */
  sequence<FEMPlateGroup*> FEMPlateGroupList;

  /** FEMBeam object proxy. */
  interface FEMBeam;
  /** Data type for holding a list of FEMBeam proxies. */
  sequence<FEMBeam*> FEMBeamList;

  /** FEMCurvMomentData object proxy. */
  interface FEMCurvMomentData;
  /** Data type for holding a list of FEMCurvMomentData proxies. */
  sequence<FEMCurvMomentData*> FEMCurvMomentDataList;

  /** FEMPropertysets object proxy. */
  interface FEMPropertysets;
  /** Data type for holding a list of FEMPropertysets proxies. */
  sequence<FEMPropertysets*> FEMPropertysetsList;

  /** FEMOrthotropicMaterial object proxy. */
  interface FEMOrthotropicMaterial;
  /** Data type for holding a list of FEMOrthotropicMaterial proxies. */
  sequence<FEMOrthotropicMaterial*> FEMOrthotropicMaterialList;

  /** FEMAppliedLoads object proxy. */
  interface FEMAppliedLoads;
  /** Data type for holding a list of FEMAppliedLoads proxies. */
  sequence<FEMAppliedLoads*> FEMAppliedLoadsList;

  /** FEMThermoOrthData object proxy. */
  interface FEMThermoOrthData;
  /** Data type for holding a list of FEMThermoOrthData proxies. */
  sequence<FEMThermoOrthData*> FEMThermoOrthDataList;

  /** FEMContactPairs object proxy. */
  interface FEMContactPairs;
  /** Data type for holding a list of FEMContactPairs proxies. */
  sequence<FEMContactPairs*> FEMContactPairsList;

  /** FEMGeneral object proxy. */
  interface FEMGeneral;
  /** Data type for holding a list of FEMGeneral proxies. */
  sequence<FEMGeneral*> FEMGeneralList;

  /** FEMBeamNode object proxy. */
  interface FEMBeamNode;
  /** Data type for holding a list of FEMBeamNode proxies. */
  sequence<FEMBeamNode*> FEMBeamNodeList;

  /** FEMSectionRect object proxy. */
  interface FEMSectionRect;
  /** Data type for holding a list of FEMSectionRect proxies. */
  sequence<FEMSectionRect*> FEMSectionRectList;

  /** FEMBeamLoad object proxy. */
  interface FEMBeamLoad;
  /** Data type for holding a list of FEMBeamLoad proxies. */
  sequence<FEMBeamLoad*> FEMBeamLoadList;

  /** FEMLoadMassProportional object proxy. */
  interface FEMLoadMassProportional;
  /** Data type for holding a list of FEMLoadMassProportional proxies. */
  sequence<FEMLoadMassProportional*> FEMLoadMassProportionalList;

  /** FEMLink object proxy. */
  interface FEMLink;
  /** Data type for holding a list of FEMLink proxies. */
  sequence<FEMLink*> FEMLinkList;

  /** FEMAxesNode object proxy. */
  interface FEMAxesNode;
  /** Data type for holding a list of FEMAxesNode proxies. */
  sequence<FEMAxesNode*> FEMAxesNodeList;

  /** FEMNMTimeMass object proxy. */
  interface FEMNMTimeMass;
  /** Data type for holding a list of FEMNMTimeMass proxies. */
  sequence<FEMNMTimeMass*> FEMNMTimeMassList;

  /** FEMAppliedDisplacement object proxy. */
  interface FEMAppliedDisplacement;
  /** Data type for holding a list of FEMAppliedDisplacement proxies. */
  sequence<FEMAppliedDisplacement*> FEMAppliedDisplacementList;

  /** FEMTimeFunctions object proxy. */
  interface FEMTimeFunctions;
  /** Data type for holding a list of FEMTimeFunctions proxies. */
  sequence<FEMTimeFunctions*> FEMTimeFunctionsList;

  /** FEMForceStrainData object proxy. */
  interface FEMForceStrainData;
  /** Data type for holding a list of FEMForceStrainData proxies. */
  sequence<FEMForceStrainData*> FEMForceStrainDataList;

  /** FEMSkewDOF object proxy. */
  interface FEMSkewDOF;
  /** Data type for holding a list of FEMSkewDOF proxies. */
  sequence<FEMSkewDOF*> FEMSkewDOFList;

  /** FEMSectionI object proxy. */
  interface FEMSectionI;
  /** Data type for holding a list of FEMSectionI proxies. */
  sequence<FEMSectionI*> FEMSectionIList;

  /** FEMPlasticBilinearMaterial object proxy. */
  interface FEMPlasticBilinearMaterial;
  /** Data type for holding a list of FEMPlasticBilinearMaterial proxies. */
  sequence<FEMPlasticBilinearMaterial*> FEMPlasticBilinearMaterialList;

  /** FEMMTForceData object proxy. */
  interface FEMMTForceData;
  /** Data type for holding a list of FEMMTForceData proxies. */
  sequence<FEMMTForceData*> FEMMTForceDataList;

  /** FEMShellPressure object proxy. */
  interface FEMShellPressure;
  /** Data type for holding a list of FEMShellPressure proxies. */
  sequence<FEMShellPressure*> FEMShellPressureList;

  /** FEMMatrices object proxy. */
  interface FEMMatrices;
  /** Data type for holding a list of FEMMatrices proxies. */
  sequence<FEMMatrices*> FEMMatricesList;

  /** FEMDamping object proxy. */
  interface FEMDamping;
  /** Data type for holding a list of FEMDamping proxies. */
  sequence<FEMDamping*> FEMDampingList;

  /** FEMMaterial object proxy. */
  interface FEMMaterial;
  /** Data type for holding a list of FEMMaterial proxies. */
  sequence<FEMMaterial*> FEMMaterialList;

  /** FEMMatrixData object proxy. */
  interface FEMMatrixData;
  /** Data type for holding a list of FEMMatrixData proxies. */
  sequence<FEMMatrixData*> FEMMatrixDataList;

  /** FEMShellAxesOrtho object proxy. */
  interface FEMShellAxesOrtho;
  /** Data type for holding a list of FEMShellAxesOrtho proxies. */
  sequence<FEMShellAxesOrtho*> FEMShellAxesOrthoList;

  /** FEMEndreleases object proxy. */
  interface FEMEndreleases;
  /** Data type for holding a list of FEMEndreleases proxies. */
  sequence<FEMEndreleases*> FEMEndreleasesList;

  /** FEMTrussGroup object proxy. */
  interface FEMTrussGroup;
  /** Data type for holding a list of FEMTrussGroup proxies. */
  sequence<FEMTrussGroup*> FEMTrussGroupList;

  /** FEMInitialTemperature object proxy. */
  interface FEMInitialTemperature;
  /** Data type for holding a list of FEMInitialTemperature proxies. */
  sequence<FEMInitialTemperature*> FEMInitialTemperatureList;

  /** FEMThermoIsoMaterials object proxy. */
  interface FEMThermoIsoMaterials;
  /** Data type for holding a list of FEMThermoIsoMaterials proxies. */
  sequence<FEMThermoIsoMaterials*> FEMThermoIsoMaterialsList;

  /** FEMThermoIsoData object proxy. */
  interface FEMThermoIsoData;
  /** Data type for holding a list of FEMThermoIsoData proxies. */
  sequence<FEMThermoIsoData*> FEMThermoIsoDataList;

  /** FEMContactGroup3 object proxy. */
  interface FEMContactGroup3;
  /** Data type for holding a list of FEMContactGroup3 proxies. */
  sequence<FEMContactGroup3*> FEMContactGroup3List;

  /** FEMNLElasticMaterials object proxy. */
  interface FEMNLElasticMaterials;
  /** Data type for holding a list of FEMNLElasticMaterials proxies. */
  sequence<FEMNLElasticMaterials*> FEMNLElasticMaterialsList;

  /** FEMPlate object proxy. */
  interface FEMPlate;
  /** Data type for holding a list of FEMPlate proxies. */
  sequence<FEMPlate*> FEMPlateList;

  /** FEMIsoBeam object proxy. */
  interface FEMIsoBeam;
  /** Data type for holding a list of FEMIsoBeam proxies. */
  sequence<FEMIsoBeam*> FEMIsoBeamList;

  /** FEMAppliedConcentratedLoad object proxy. */
  interface FEMAppliedConcentratedLoad;
  /** Data type for holding a list of FEMAppliedConcentratedLoad proxies. */
  sequence<FEMAppliedConcentratedLoad*> FEMAppliedConcentratedLoadList;

  /** FEMTwoDSolidGroup object proxy. */
  interface FEMTwoDSolidGroup;
  /** Data type for holding a list of FEMTwoDSolidGroup proxies. */
  sequence<FEMTwoDSolidGroup*> FEMTwoDSolidGroupList;

  /** FEMGroup object proxy. */
  interface FEMGroup;
  /** Data type for holding a list of FEMGroup proxies. */
  sequence<FEMGroup*> FEMGroupList;

  /** FEMProperties object proxy. */
  interface FEMProperties;
  /** Data type for holding a list of FEMProperties proxies. */
  sequence<FEMProperties*> FEMPropertiesList;

  /** FEMThreeDSolidGroup object proxy. */
  interface FEMThreeDSolidGroup;
  /** Data type for holding a list of FEMThreeDSolidGroup proxies. */
  sequence<FEMThreeDSolidGroup*> FEMThreeDSolidGroupList;

  /** FEMThreeDSolid object proxy. */
  interface FEMThreeDSolid;
  /** Data type for holding a list of FEMThreeDSolid proxies. */
  sequence<FEMThreeDSolid*> FEMThreeDSolidList;

  /** FEMSectionProp object proxy. */
  interface FEMSectionProp;
  /** Data type for holding a list of FEMSectionProp proxies. */
  sequence<FEMSectionProp*> FEMSectionPropList;

  /** FEMElasticMaterial object proxy. */
  interface FEMElasticMaterial;
  /** Data type for holding a list of FEMElasticMaterial proxies. */
  sequence<FEMElasticMaterial*> FEMElasticMaterialList;

  /** FEMPoints object proxy. */
  interface FEMPoints;
  /** Data type for holding a list of FEMPoints proxies. */
  sequence<FEMPoints*> FEMPointsList;

  /** FEMThermoOrthMaterials object proxy. */
  interface FEMThermoOrthMaterials;
  /** Data type for holding a list of FEMThermoOrthMaterials proxies. */
  sequence<FEMThermoOrthMaterials*> FEMThermoOrthMaterialsList;

  /** FEMConstraints object proxy. */
  interface FEMConstraints;
  /** Data type for holding a list of FEMConstraints proxies. */
  sequence<FEMConstraints*> FEMConstraintsList;

  /** FEMMCrigidities object proxy. */
  interface FEMMCrigidities;
  /** Data type for holding a list of FEMMCrigidities proxies. */
  sequence<FEMMCrigidities*> FEMMCrigiditiesList;

  /** FEMSkeySysNode object proxy. */
  interface FEMSkeySysNode;
  /** Data type for holding a list of FEMSkeySysNode proxies. */
  sequence<FEMSkeySysNode*> FEMSkeySysNodeList;

  /** FEMIsoBeamGroup object proxy. */
  interface FEMIsoBeamGroup;
  /** Data type for holding a list of FEMIsoBeamGroup proxies. */
  sequence<FEMIsoBeamGroup*> FEMIsoBeamGroupList;

  /** FEMShellDOF object proxy. */
  interface FEMShellDOF;
  /** Data type for holding a list of FEMShellDOF proxies. */
  sequence<FEMShellDOF*> FEMShellDOFList;

  /** FEMCrossSections object proxy. */
  interface FEMCrossSections;
  /** Data type for holding a list of FEMCrossSections proxies. */
  sequence<FEMCrossSections*> FEMCrossSectionsList;

  /** FEMTwistMomentData object proxy. */
  interface FEMTwistMomentData;
  /** Data type for holding a list of FEMTwistMomentData proxies. */
  sequence<FEMTwistMomentData*> FEMTwistMomentDataList;

  /** FEMShell object proxy. */
  interface FEMShell;
  /** Data type for holding a list of FEMShell proxies. */
  sequence<FEMShell*> FEMShellList;

  /** FEMNTNContact object proxy. */
  interface FEMNTNContact;
  /** Data type for holding a list of FEMNTNContact proxies. */
  sequence<FEMNTNContact*> FEMNTNContactList;

  /** FEMShellLayer object proxy. */
  interface FEMShellLayer;
  /** Data type for holding a list of FEMShellLayer proxies. */
  sequence<FEMShellLayer*> FEMShellLayerList;

  /** FEMSkewSysAngles object proxy. */
  interface FEMSkewSysAngles;
  /** Data type for holding a list of FEMSkewSysAngles proxies. */
  sequence<FEMSkewSysAngles*> FEMSkewSysAnglesList;

  /** FEMGroundMotionRecord object proxy. */
  interface FEMGroundMotionRecord;
  /** Data type for holding a list of FEMGroundMotionRecord proxies. */
  sequence<FEMGroundMotionRecord*> FEMGroundMotionRecordList;

  /** FEMGeneralGroup object proxy. */
  interface FEMGeneralGroup;
  /** Data type for holding a list of FEMGeneralGroup proxies. */
  sequence<FEMGeneralGroup*> FEMGeneralGroupList;

  /** FEMTwoDSolid object proxy. */
  interface FEMTwoDSolid;
  /** Data type for holding a list of FEMTwoDSolid proxies. */
  sequence<FEMTwoDSolid*> FEMTwoDSolidList;

  /** FEMAppliedTemperature object proxy. */
  interface FEMAppliedTemperature;
  /** Data type for holding a list of FEMAppliedTemperature proxies. */
  sequence<FEMAppliedTemperature*> FEMAppliedTemperatureList;

  /** FEMMatrixSets object proxy. */
  interface FEMMatrixSets;
  /** Data type for holding a list of FEMMatrixSets proxies. */
  sequence<FEMMatrixSets*> FEMMatrixSetsList;

  /** FEMConstraintCoef object proxy. */
  interface FEMConstraintCoef;
  /** Data type for holding a list of FEMConstraintCoef proxies. */
  sequence<FEMConstraintCoef*> FEMConstraintCoefList;

  /** FEMSectionBox object proxy. */
  interface FEMSectionBox;
  /** Data type for holding a list of FEMSectionBox proxies. */
  sequence<FEMSectionBox*> FEMSectionBoxList;

  /** FEMNKDisplForce object proxy. */
  interface FEMNKDisplForce;
  /** Data type for holding a list of FEMNKDisplForce proxies. */
  sequence<FEMNKDisplForce*> FEMNKDisplForceList;

  /** FEMPlasticStrainStress object proxy. */
  interface FEMPlasticStrainStress;
  /** Data type for holding a list of FEMPlasticStrainStress proxies. */
  sequence<FEMPlasticStrainStress*> FEMPlasticStrainStressList;

  /** FEMShellAxesOrthoData object proxy. */
  interface FEMShellAxesOrthoData;
  /** Data type for holding a list of FEMShellAxesOrthoData proxies. */
  sequence<FEMShellAxesOrthoData*> FEMShellAxesOrthoDataList;

  /** FEMGeneralNode object proxy. */
  interface FEMGeneralNode;
  /** Data type for holding a list of FEMGeneralNode proxies. */
  sequence<FEMGeneralNode*> FEMGeneralNodeList;

  /** FEMStrLines object proxy. */
  interface FEMStrLines;
  /** Data type for holding a list of FEMStrLines proxies. */
  sequence<FEMStrLines*> FEMStrLinesList;

  /** FEMContactSurface object proxy. */
  interface FEMContactSurface;
  /** Data type for holding a list of FEMContactSurface proxies. */
  sequence<FEMContactSurface*> FEMContactSurfaceList;

  /** FEMMCForceData object proxy. */
  interface FEMMCForceData;
  /** Data type for holding a list of FEMMCForceData proxies. */
  sequence<FEMMCForceData*> FEMMCForceDataList;

  /** FEMSpring object proxy. */
  interface FEMSpring;
  /** Data type for holding a list of FEMSpring proxies. */
  sequence<FEMSpring*> FEMSpringList;

  /** FEMSpringGroup object proxy. */
  interface FEMSpringGroup;
  /** Data type for holding a list of FEMSpringGroup proxies. */
  sequence<FEMSpringGroup*> FEMSpringGroupList;

  /** FEMShellGroup object proxy. */
  interface FEMShellGroup;
  /** Data type for holding a list of FEMShellGroup proxies. */
  sequence<FEMShellGroup*> FEMShellGroupList;

  /** DaqUnit object proxy. */
  interface DaqUnit;
  /** Data type for holding a list of DaqUnit proxies. */
  sequence<DaqUnit*> DaqUnitList;

  /** DaqUnitChannel object proxy. */
  interface DaqUnitChannel;
  /** Data type for holding a list of DaqUnitChannel proxies. */
  sequence<DaqUnitChannel*> DaqUnitChannelList;

  /** Sensor object proxy. */
  interface Sensor;
  /** Data type for holding a list of Sensor proxies. */
  sequence<Sensor*> SensorList;

  /** Transducer object proxy. */
  interface Transducer;
  /** Data type for holding a list of Transducer proxies. */
  sequence<Transducer*> TransducerList;

  /** Experiment object proxy. */
  interface Experiment;
  /** Data type for holding a list of Experiment proxies. */
  sequence<Experiment*> ExperimentList;

  /** Cycle object proxy. */
  interface Cycle;
  /** Data type for holding a list of Cycle proxies. */
  sequence<Cycle*> CycleList;

  /** SensorTimeData object proxy. */
  interface SensorTimeData;
  /** Data type for holding a list of SensorTimeData proxies. */
  sequence<SensorTimeData*> SensorTimeDataList;

  /** TransducerTimeData object proxy. */
  interface TransducerTimeData;
  /** Data type for holding a list of TransducerTimeData proxies. */
  sequence<TransducerTimeData*> TransducerTimeDataList;

  /** ExperimentStructureData object proxy. */
  interface ExperimentStructureData;
  /** Data type for holding a list of ExperimentStructureData proxies. */
  sequence<ExperimentStructureData*> ExperimentStructureDataList;

  /** TransducerCycleData object proxy. */
  interface TransducerCycleData;
  /** Data type for holding a list of TransducerCycleData proxies. */
  sequence<TransducerCycleData*> TransducerCycleDataList;

  /** MappingMatrix object proxy. */
  interface MappingMatrix;
  /** Data type for holding a list of MappingMatrix proxies. */
  sequence<MappingMatrix*> MappingMatrixList;

  /** DaqUnitChannelData object proxy. */
  interface DaqUnitChannelData;
  /** Data type for holding a list of DaqUnitChannelData proxies. */
  sequence<DaqUnitChannelData*> DaqUnitChannelDataList;

  /** AccelMeas object proxy. */
  interface AccelMeas;
  /** Data type for holding a list of AccelMeas proxies. */
  sequence<AccelMeas*> AccelMeasList;

  /**
   * Structure fields structure.
   */
  struct StructureFields {
    /** Unique identifier (for internal use only). */
    long id;
    /** Short name.  */
    string mName;
    /** One-line description.  */
    string mDescription;
    /** Type of structure, e.g. suspension bridge.  */
    string mType;
    /** Unit of distance  */
    Unit mDistanceUnit;
    /** Unit of force  */
    Unit mForceUnit;
    /** Unit of weight  */
    Unit mWeightUnit;
  };
  /** Data type for holding a list of Structure fields structures. */
  sequence<StructureFields> StructureFieldsList;

  /**
   * FEMDof fields structure.
   */
  struct FEMDofFields {
    /** Unique identifier (for internal use only). */
    long id;
    /** Structure  */
    long mStructure;
    /** Local DOF number.  */
    int mLocalId;
    /** Finite element node.  */
    long mNode;
    /** Direction of motion.  */
    Quantity mDirection;
  };
  /** Data type for holding a list of FEMDof fields structures. */
  sequence<FEMDofFields> FEMDofFieldsList;

  /**
   * FEMNodalMass fields structure.
   */
  struct FEMNodalMassFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    double mMass1;
    /**  */
    double mMass2;
    /**  */
    double mMass3;
    /**  */
    double mMass4;
    /**  */
    double mMass5;
    /**  */
    double mMass6;
    /**  */
    long mNode;
  };
  /** Data type for holding a list of FEMNodalMass fields structures. */
  sequence<FEMNodalMassFields> FEMNodalMassFieldsList;

  /**
   * FEMNLElasticStrainStress fields structure.
   */
  struct FEMNLElasticStrainStressFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mMaterialID;
    /**  */
    int mRecordNumber;
    /**  */
    double mStrain;
    /**  */
    double mStress;
  };
  /** Data type for holding a list of FEMNLElasticStrainStress fields structures. */
  sequence<FEMNLElasticStrainStressFields> FEMNLElasticStrainStressFieldsList;

  /**
   * FEMBoundary fields structure.
   */
  struct FEMBoundaryFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mNode;
    /**  */
    BoundaryType mOvalization;
    /**  */
    BoundaryType mPhi;
    /**  */
    BoundaryType mRx;
    /**  */
    BoundaryType mRy;
    /**  */
    BoundaryType mRz;
    /**  */
    BoundaryType mUx;
    /**  */
    BoundaryType mUy;
    /**  */
    BoundaryType mUz;
    /**  */
    string mWarping;
  };
  /** Data type for holding a list of FEMBoundary fields structures. */
  sequence<FEMBoundaryFields> FEMBoundaryFieldsList;

  /**
   * FEMSectionPipe fields structure.
   */
  struct FEMSectionPipeFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mDiameter;
    /**  */
    double mSC;
    /**  */
    int mSectionID;
    /**  */
    double mSSarea;
    /**  */
    double mTC;
    /**  */
    double mThickness;
    /**  */
    double mTorfac;
    /**  */
    double mTSarea;
  };
  /** Data type for holding a list of FEMSectionPipe fields structures. */
  sequence<FEMSectionPipeFields> FEMSectionPipeFieldsList;

  /**
   * FEMCoordSystem fields structure.
   */
  struct FEMCoordSystemFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mAX;
    /**  */
    double mAY;
    /**  */
    double mAZ;
    /**  */
    double mBBY;
    /**  */
    double mBX;
    /**  */
    double mBZ;
    /**  */
    string mDescription;
    /**  */
    short mMode;
    /**  */
    int mP1;
    /**  */
    int mP2;
    /**  */
    int mP3;
    /**  */
    string mType;
    /**  */
    double mXorigin;
    /**  */
    double mYorigin;
    /**  */
    double mZorigin;
    /**  */
    int mLocalID;
  };
  /** Data type for holding a list of FEMCoordSystem fields structures. */
  sequence<FEMCoordSystemFields> FEMCoordSystemFieldsList;

  /**
   * FEMNode fields structure.
   */
  struct FEMNodeFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    long mSystem;
    /**  */
    double mX;
    /**  */
    double mY;
    /**  */
    double mZ;
    /**  */
    int mLocalID;
  };
  /** Data type for holding a list of FEMNode fields structures. */
  sequence<FEMNodeFields> FEMNodeFieldsList;

  /**
   * FEMTruss fields structure.
   */
  struct FEMTrussFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    double mEpsin;
    /**  */
    double mGapwidth;
    /**  */
    long mGroup;
    /**  */
    long mMaterial;
    /**  */
    long mN1;
    /**  */
    long mN2;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
    /**  */
    double mSectionArea;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
  };
  /** Data type for holding a list of FEMTruss fields structures. */
  sequence<FEMTrussFields> FEMTrussFieldsList;

  /**
   * FEMTimeFunctionData fields structure.
   */
  struct FEMTimeFunctionDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mDataTime;
    /**  */
    short mGMRecordID;
    /**  */
    int mRecordNmb;
    /**  */
    int mTimeFunctionID;
    /**  */
    double mTimeValue;
  };
  /** Data type for holding a list of FEMTimeFunctionData fields structures. */
  sequence<FEMTimeFunctionDataFields> FEMTimeFunctionDataFieldsList;

  /**
   * FEMPlasticMlMaterials fields structure.
   */
  struct FEMPlasticMlMaterialsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mALPHA;
    /**  */
    double mDensity;
    /**  */
    double mE;
    /**  */
    string mHardening;
    /**  */
    int mMaterialID;
    /**  */
    double mNU;
    /**  */
    double mTREF;
  };
  /** Data type for holding a list of FEMPlasticMlMaterials fields structures. */
  sequence<FEMPlasticMlMaterialsFields> FEMPlasticMlMaterialsFieldsList;

  /**
   * FEMPlateGroup fields structure.
   */
  struct FEMPlateGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDisplacement;
    /**  */
    long mGroup;
    /**  */
    string mIniStrain;
    /**  */
    int mMaterialID;
    /**  */
    string mResult;
  };
  /** Data type for holding a list of FEMPlateGroup fields structures. */
  sequence<FEMPlateGroupFields> FEMPlateGroupFieldsList;

  /**
   * FEMBeam fields structure.
   */
  struct FEMBeamFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mAuxNode;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    int mEndRelease;
    /**  */
    double mEpsin;
    /**  */
    long mGroup;
    /**  */
    double mIRigidEnd;
    /**  */
    double mJRigidEnd;
    /**  */
    int mMaterialID;
    /**  */
    long mNode1;
    /**  */
    long mNode2;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
    /**  */
    int mSectionID;
    /**  */
    int mSubdivision;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
  };
  /** Data type for holding a list of FEMBeam fields structures. */
  sequence<FEMBeamFields> FEMBeamFieldsList;

  /**
   * FEMCurvMomentData fields structure.
   */
  struct FEMCurvMomentDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mCurvature;
    /**  */
    int mCurvMomentID;
    /**  */
    double mMoment;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMCurvMomentData fields structures. */
  sequence<FEMCurvMomentDataFields> FEMCurvMomentDataFieldsList;

  /**
   * FEMPropertysets fields structure.
   */
  struct FEMPropertysetsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mC;
    /**  */
    string mDescription;
    /**  */
    double mK;
    /**  */
    double mM;
    /**  */
    int mNC;
    /**  */
    int mNK;
    /**  */
    int mNM;
    /**  */
    string mNonlinear;
    /**  */
    int mPropertysetID;
    /**  */
    double mS;
  };
  /** Data type for holding a list of FEMPropertysets fields structures. */
  sequence<FEMPropertysetsFields> FEMPropertysetsFieldsList;

  /**
   * FEMOrthotropicMaterial fields structure.
   */
  struct FEMOrthotropicMaterialFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mDensity;
    /**  */
    double mEA;
    /**  */
    double mEB;
    /**  */
    double mEC;
    /**  */
    double mGAB;
    /**  */
    double mGAC;
    /**  */
    double mGBC;
    /**  */
    long mMaterial;
    /**  */
    double mNUAB;
    /**  */
    double mNUAC;
    /**  */
    double mNUBC;
  };
  /** Data type for holding a list of FEMOrthotropicMaterial fields structures. */
  sequence<FEMOrthotropicMaterialFields> FEMOrthotropicMaterialFieldsList;

  /**
   * FEMAppliedLoads fields structure.
   */
  struct FEMAppliedLoadsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mAppliedLoadNmb;
    /**  */
    double mArrivalTime;
    /**  */
    short mLoadID;
    /**  */
    string mLoadType;
    /**  */
    string mSiteType;
    /**  */
    int mTimeFunctionID;
  };
  /** Data type for holding a list of FEMAppliedLoads fields structures. */
  sequence<FEMAppliedLoadsFields> FEMAppliedLoadsFieldsList;

  /**
   * FEMThermoOrthData fields structure.
   */
  struct FEMThermoOrthDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mALPHAA;
    /**  */
    double mALPHAB;
    /**  */
    double mALPHAC;
    /**  */
    double mEA;
    /**  */
    double mEB;
    /**  */
    double mEC;
    /**  */
    double mGAB;
    /**  */
    double mGAC;
    /**  */
    double mGBC;
    /**  */
    int mMaterialID;
    /**  */
    double mNUAB;
    /**  */
    double mNUAC;
    /**  */
    double mNUBC;
    /**  */
    int mRecordNmb;
    /**  */
    double mTheta;
  };
  /** Data type for holding a list of FEMThermoOrthData fields structures. */
  sequence<FEMThermoOrthDataFields> FEMThermoOrthDataFieldsList;

  /**
   * FEMContactPairs fields structure.
   */
  struct FEMContactPairsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mContactorSurf;
    /**  */
    int mContGroupID;
    /**  */
    int mContPair;
    /**  */
    double mFContactor;
    /**  */
    double mFriction;
    /**  */
    double mFTarget;
    /**  */
    double mHeatTransf;
    /**  */
    int mRecordNmb;
    /**  */
    int mTargetSurf;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
  };
  /** Data type for holding a list of FEMContactPairs fields structures. */
  sequence<FEMContactPairsFields> FEMContactPairsFieldsList;

  /**
   * FEMGeneral fields structure.
   */
  struct FEMGeneralFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    long mGroup;
    /**  */
    int mMatrixSet;
    /**  */
    short mNodeAmount;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
  };
  /** Data type for holding a list of FEMGeneral fields structures. */
  sequence<FEMGeneralFields> FEMGeneralFieldsList;

  /**
   * FEMBeamNode fields structure.
   */
  struct FEMBeamNodeFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDisplacement;
    /**  */
    long mGroup;
    /**  */
    string mIniStrain;
    /**  */
    int mMaterialID;
    /**  */
    string mMC;
    /**  */
    int mMCrigidity;
    /**  */
    double mREmultiplyer;
    /**  */
    string mResult;
    /**  */
    string mREtype;
    /**  */
    short mRINT;
    /**  */
    int mSectionID;
    /**  */
    short mSINT;
    /**  */
    short mTINT;
  };
  /** Data type for holding a list of FEMBeamNode fields structures. */
  sequence<FEMBeamNodeFields> FEMBeamNodeFieldsList;

  /**
   * FEMSectionRect fields structure.
   */
  struct FEMSectionRectFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mHeight;
    /**  */
    string mIShear;
    /**  */
    double mSC;
    /**  */
    int mSectionID;
    /**  */
    double mSSarea;
    /**  */
    double mTC;
    /**  */
    double mTorfac;
    /**  */
    double mTSarea;
    /**  */
    double mWidth;
  };
  /** Data type for holding a list of FEMSectionRect fields structures. */
  sequence<FEMSectionRectFields> FEMSectionRectFieldsList;

  /**
   * FEMBeamLoad fields structure.
   */
  struct FEMBeamLoadFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mArrivalTime;
    /**  */
    short mDeformDepend;
    /**  */
    short mDirectFilter;
    /**  */
    int mElementID;
    /**  */
    short mFace;
    /**  */
    long mGroup;
    /**  */
    double mP1;
    /**  */
    double mP2;
    /**  */
    int mRecordNmb;
    /**  */
    int mTimeFunctionID;
  };
  /** Data type for holding a list of FEMBeamLoad fields structures. */
  sequence<FEMBeamLoadFields> FEMBeamLoadFieldsList;

  /**
   * FEMLoadMassProportional fields structure.
   */
  struct FEMLoadMassProportionalFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mAX;
    /**  */
    double mAY;
    /**  */
    double mAZ;
    /**  */
    short mLoadID;
    /**  */
    double mMagnitude;
    /**  */
    long mStructure;
  };
  /** Data type for holding a list of FEMLoadMassProportional fields structures. */
  sequence<FEMLoadMassProportionalFields> FEMLoadMassProportionalFieldsList;

  /**
   * FEMLink fields structure.
   */
  struct FEMLinkFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    string mDisplacement;
    /**  */
    long mMasterNode;
    /**  */
    int mRLID;
    /**  */
    long mSlaveNode;
  };
  /** Data type for holding a list of FEMLink fields structures. */
  sequence<FEMLinkFields> FEMLinkFieldsList;

  /**
   * FEMAxesNode fields structure.
   */
  struct FEMAxesNodeFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mAxNodeID;
    /**  */
    long mGroup;
    /**  */
    long mNode1;
    /**  */
    long mNode2;
    /**  */
    long mNode3;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMAxesNode fields structures. */
  sequence<FEMAxesNodeFields> FEMAxesNodeFieldsList;

  /**
   * FEMNMTimeMass fields structure.
   */
  struct FEMNMTimeMassFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mMass;
    /**  */
    int mPropertyID;
    /**  */
    int mRecordNmb;
    /**  */
    double mTimeValue;
  };
  /** Data type for holding a list of FEMNMTimeMass fields structures. */
  sequence<FEMNMTimeMassFields> FEMNMTimeMassFieldsList;

  /**
   * FEMAppliedDisplacement fields structure.
   */
  struct FEMAppliedDisplacementFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mArrivalTime;
    /**  */
    string mDescription;
    /**  */
    short mDirection;
    /**  */
    double mFactor;
    /**  */
    long mNode;
    /**  */
    int mRecordNmb;
    /**  */
    int mTimeFunctionID;
  };
  /** Data type for holding a list of FEMAppliedDisplacement fields structures. */
  sequence<FEMAppliedDisplacementFields> FEMAppliedDisplacementFieldsList;

  /**
   * FEMTimeFunctions fields structure.
   */
  struct FEMTimeFunctionsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mTimeFunctionID;
  };
  /** Data type for holding a list of FEMTimeFunctions fields structures. */
  sequence<FEMTimeFunctionsFields> FEMTimeFunctionsFieldsList;

  /**
   * FEMForceStrainData fields structure.
   */
  struct FEMForceStrainDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mForce;
    /**  */
    int mForceAxID;
    /**  */
    int mRecordNmb;
    /**  */
    double mStrain;
  };
  /** Data type for holding a list of FEMForceStrainData fields structures. */
  sequence<FEMForceStrainDataFields> FEMForceStrainDataFieldsList;

  /**
   * FEMSkewDOF fields structure.
   */
  struct FEMSkewDOFFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    long mNode;
    /**  */
    int mSkewSystemID;
  };
  /** Data type for holding a list of FEMSkewDOF fields structures. */
  sequence<FEMSkewDOFFields> FEMSkewDOFFieldsList;

  /**
   * FEMSectionI fields structure.
   */
  struct FEMSectionIFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mHeight;
    /**  */
    double mSC;
    /**  */
    int mSectionID;
    /**  */
    double mSSarea;
    /**  */
    double mTC;
    /**  */
    double mThick1;
    /**  */
    double mThick2;
    /**  */
    double mThick3;
    /**  */
    double mTorfac;
    /**  */
    double mTSarea;
    /**  */
    double mWidth1;
    /**  */
    double mWidth2;
  };
  /** Data type for holding a list of FEMSectionI fields structures. */
  sequence<FEMSectionIFields> FEMSectionIFieldsList;

  /**
   * FEMPlasticBilinearMaterial fields structure.
   */
  struct FEMPlasticBilinearMaterialFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mAlpha;
    /**  */
    double mDensity;
    /**  */
    double mE;
    /**  */
    double mEPA;
    /**  */
    double mET;
    /**  */
    string mHardening;
    /**  */
    long mMaterial;
    /**  */
    double mNU;
    /**  */
    double mTRef;
    /**  */
    double mYield;
  };
  /** Data type for holding a list of FEMPlasticBilinearMaterial fields structures. */
  sequence<FEMPlasticBilinearMaterialFields> FEMPlasticBilinearMaterialFieldsList;

  /**
   * FEMMTForceData fields structure.
   */
  struct FEMMTForceDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mForce;
    /**  */
    int mMomentRID;
    /**  */
    int mRecordNmb;
    /**  */
    int mTwistMomentID;
  };
  /** Data type for holding a list of FEMMTForceData fields structures. */
  sequence<FEMMTForceDataFields> FEMMTForceDataFieldsList;

  /**
   * FEMShellPressure fields structure.
   */
  struct FEMShellPressureFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mArrivalTime;
    /**  */
    short mDeformDepend;
    /**  */
    string mDescription;
    /**  */
    short mDirectFilter;
    /**  */
    int mElementID;
    /**  */
    short mFace;
    /**  */
    long mGroup;
    /**  */
    int mNodaux;
    /**  */
    double mP1;
    /**  */
    double mP2;
    /**  */
    double mP3;
    /**  */
    double mP4;
    /**  */
    int mRecordNmb;
    /**  */
    int mTimeFunctionID;
  };
  /** Data type for holding a list of FEMShellPressure fields structures. */
  sequence<FEMShellPressureFields> FEMShellPressureFieldsList;

  /**
   * FEMMatrices fields structure.
   */
  struct FEMMatricesFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mLump;
    /**  */
    int mMatrixID;
    /**  */
    string mMatrixType;
    /**  */
    int mND;
    /**  */
    int mNS;
  };
  /** Data type for holding a list of FEMMatrices fields structures. */
  sequence<FEMMatricesFields> FEMMatricesFieldsList;

  /**
   * FEMDamping fields structure.
   */
  struct FEMDampingFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mALPHA;
    /**  */
    double mBETA;
    /**  */
    long mGroup;
  };
  /** Data type for holding a list of FEMDamping fields structures. */
  sequence<FEMDampingFields> FEMDampingFieldsList;

  /**
   * FEMMaterial fields structure.
   */
  struct FEMMaterialFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    MaterialType mMaterialType;
    /**  */
    int mLocalID;
    /**  */
    long mStructure;
  };
  /** Data type for holding a list of FEMMaterial fields structures. */
  sequence<FEMMaterialFields> FEMMaterialFieldsList;

  /**
   * FEMMatrixData fields structure.
   */
  struct FEMMatrixDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mCoeff;
    /**  */
    int mColumnIndex;
    /**  */
    int mMatrixID;
    /**  */
    int mRecordNmb;
    /**  */
    int mRowIndex;
  };
  /** Data type for holding a list of FEMMatrixData fields structures. */
  sequence<FEMMatrixDataFields> FEMMatrixDataFieldsList;

  /**
   * FEMShellAxesOrtho fields structure.
   */
  struct FEMShellAxesOrthoFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mALFA;
    /**  */
    int mAxOrthoID;
    /**  */
    long mGroup;
    /**  */
    int mLineID;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMShellAxesOrtho fields structures. */
  sequence<FEMShellAxesOrthoFields> FEMShellAxesOrthoFieldsList;

  /**
   * FEMEndreleases fields structure.
   */
  struct FEMEndreleasesFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mEndRelease;
    /**  */
    short mMoment1;
    /**  */
    short mMoment2;
    /**  */
    short mMoment3;
    /**  */
    short mMoment4;
    /**  */
    short mMoment5;
    /**  */
    short mMoment6;
  };
  /** Data type for holding a list of FEMEndreleases fields structures. */
  sequence<FEMEndreleasesFields> FEMEndreleasesFieldsList;

  /**
   * FEMTrussGroup fields structure.
   */
  struct FEMTrussGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDisplacement;
    /**  */
    string mGAPS;
    /**  */
    long mGroup;
    /**  */
    string mIniStrain;
    /**  */
    long mMaterial;
    /**  */
    double mSectionArea;
  };
  /** Data type for holding a list of FEMTrussGroup fields structures. */
  sequence<FEMTrussGroupFields> FEMTrussGroupFieldsList;

  /**
   * FEMInitialTemperature fields structure.
   */
  struct FEMInitialTemperatureFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mNode;
    /**  */
    double mTemperature;
  };
  /** Data type for holding a list of FEMInitialTemperature fields structures. */
  sequence<FEMInitialTemperatureFields> FEMInitialTemperatureFieldsList;

  /**
   * FEMThermoIsoMaterials fields structure.
   */
  struct FEMThermoIsoMaterialsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mDensity;
    /**  */
    int mMaterialID;
    /**  */
    double mTREF;
  };
  /** Data type for holding a list of FEMThermoIsoMaterials fields structures. */
  sequence<FEMThermoIsoMaterialsFields> FEMThermoIsoMaterialsFieldsList;

  /**
   * FEMThermoIsoData fields structure.
   */
  struct FEMThermoIsoDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mALPHA;
    /**  */
    double mE;
    /**  */
    int mMaterialID;
    /**  */
    double mNU;
    /**  */
    int mRecordNmb;
    /**  */
    double mTheta;
  };
  /** Data type for holding a list of FEMThermoIsoData fields structures. */
  sequence<FEMThermoIsoDataFields> FEMThermoIsoDataFieldsList;

  /**
   * FEMContactGroup3 fields structure.
   */
  struct FEMContactGroup3Fields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mContGroupID;
    /**  */
    double mDepth;
    /**  */
    string mDescription;
    /**  */
    string mForces;
    /**  */
    double mFriction;
    /**  */
    string mIniPenetration;
    /**  */
    string mNodeToNode;
    /**  */
    double mOffset;
    /**  */
    string mPenetrAlgorithm;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
    /**  */
    string mTied;
    /**  */
    double mTiedOffset;
    /**  */
    double mTolerance;
    /**  */
    string mTractions;
  };
  /** Data type for holding a list of FEMContactGroup3 fields structures. */
  sequence<FEMContactGroup3Fields> FEMContactGroup3FieldsList;

  /**
   * FEMNLElasticMaterials fields structure.
   */
  struct FEMNLElasticMaterialsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mDcurve;
    /**  */
    double mDensity;
    /**  */
    int mMaterialID;
  };
  /** Data type for holding a list of FEMNLElasticMaterials fields structures. */
  sequence<FEMNLElasticMaterialsFields> FEMNLElasticMaterialsFieldsList;

  /**
   * FEMPlate fields structure.
   */
  struct FEMPlateFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    double mFlex11;
    /**  */
    double mFlex12;
    /**  */
    double mFlex22;
    /**  */
    long mGroup;
    /**  */
    int mMaterialID;
    /**  */
    double mMeps11;
    /**  */
    double mMeps12;
    /**  */
    double mMeps22;
    /**  */
    long mN1;
    /**  */
    long mN2;
    /**  */
    long mN3;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
    /**  */
    double mThick;
  };
  /** Data type for holding a list of FEMPlate fields structures. */
  sequence<FEMPlateFields> FEMPlateFieldsList;

  /**
   * FEMIsoBeam fields structure.
   */
  struct FEMIsoBeamFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mAUX;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    double mEpaxl;
    /**  */
    double mEphoop;
    /**  */
    long mGroup;
    /**  */
    int mMaterialID;
    /**  */
    long mN1;
    /**  */
    long mN2;
    /**  */
    long mN3;
    /**  */
    long mN4;
    /**  */
    short mNodeAmount;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
    /**  */
    int mSectionID;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
  };
  /** Data type for holding a list of FEMIsoBeam fields structures. */
  sequence<FEMIsoBeamFields> FEMIsoBeamFieldsList;

  /**
   * FEMAppliedConcentratedLoad fields structure.
   */
  struct FEMAppliedConcentratedLoadFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mArrivalTime;
    /**  */
    string mDescription;
    /**  */
    short mDirection;
    /**  */
    double mFactor;
    /**  */
    long mNodeAux;
    /**  */
    long mNodeID;
    /**  */
    int mRecordNmb;
    /**  */
    int mTimeFunctionID;
  };
  /** Data type for holding a list of FEMAppliedConcentratedLoad fields structures. */
  sequence<FEMAppliedConcentratedLoadFields> FEMAppliedConcentratedLoadFieldsList;

  /**
   * FEMTwoDSolidGroup fields structure.
   */
  struct FEMTwoDSolidGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mAuxNode;
    /**  */
    string mDisplacement;
    /**  */
    long mGroup;
    /**  */
    int mMaterialID;
    /**  */
    string mResult;
    /**  */
    string mSubtype;
  };
  /** Data type for holding a list of FEMTwoDSolidGroup fields structures. */
  sequence<FEMTwoDSolidGroupFields> FEMTwoDSolidGroupFieldsList;

  /**
   * FEMGroup fields structure.
   */
  struct FEMGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    GroupType mGroupType;
    /**  */
    int mLocalID;
    /**  */
    long mStructure;
  };
  /** Data type for holding a list of FEMGroup fields structures. */
  sequence<FEMGroupFields> FEMGroupFieldsList;

  /**
   * FEMProperties fields structure.
   */
  struct FEMPropertiesFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mPropertyID;
    /**  */
    string mPropertyType;
    /**  */
    string mRupture;
    /**  */
    double mXC;
    /**  */
    double mXN;
  };
  /** Data type for holding a list of FEMProperties fields structures. */
  sequence<FEMPropertiesFields> FEMPropertiesFieldsList;

  /**
   * FEMThreeDSolidGroup fields structure.
   */
  struct FEMThreeDSolidGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDisplacement;
    /**  */
    long mGroup;
    /**  */
    int mMaterialID;
    /**  */
    string mResult;
  };
  /** Data type for holding a list of FEMThreeDSolidGroup fields structures. */
  sequence<FEMThreeDSolidGroupFields> FEMThreeDSolidGroupFieldsList;

  /**
   * FEMThreeDSolid fields structure.
   */
  struct FEMThreeDSolidFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    long mGroup;
    /**  */
    int mMaterialID;
    /**  */
    int mMaxes;
    /**  */
    long mN1;
    /**  */
    long mN10;
    /**  */
    long mN11;
    /**  */
    long mN12;
    /**  */
    long mN13;
    /**  */
    long mN14;
    /**  */
    long mN15;
    /**  */
    long mN16;
    /**  */
    long mN17;
    /**  */
    long mN18;
    /**  */
    long mN19;
    /**  */
    long mN2;
    /**  */
    long mN20;
    /**  */
    long mN21;
    /**  */
    long mN22;
    /**  */
    long mN23;
    /**  */
    long mN24;
    /**  */
    long mN25;
    /**  */
    long mN26;
    /**  */
    long mN27;
    /**  */
    long mN3;
    /**  */
    long mN4;
    /**  */
    long mN5;
    /**  */
    long mN6;
    /**  */
    long mN7;
    /**  */
    long mN8;
    /**  */
    long mN9;
    /**  */
    short mNodeAmount;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
  };
  /** Data type for holding a list of FEMThreeDSolid fields structures. */
  sequence<FEMThreeDSolidFields> FEMThreeDSolidFieldsList;

  /**
   * FEMSectionProp fields structure.
   */
  struct FEMSectionPropFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mArea;
    /**  */
    double mRinertia;
    /**  */
    double mSarea;
    /**  */
    int mSectionID;
    /**  */
    double mSinertia;
    /**  */
    double mTarea;
    /**  */
    double mTinertia;
  };
  /** Data type for holding a list of FEMSectionProp fields structures. */
  sequence<FEMSectionPropFields> FEMSectionPropFieldsList;

  /**
   * FEMElasticMaterial fields structure.
   */
  struct FEMElasticMaterialFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mAlpha;
    /**  */
    double mDensity;
    /**  */
    double mE;
    /**  */
    long mMaterial;
    /**  */
    double mNU;
  };
  /** Data type for holding a list of FEMElasticMaterial fields structures. */
  sequence<FEMElasticMaterialFields> FEMElasticMaterialFieldsList;

  /**
   * FEMPoints fields structure.
   */
  struct FEMPointsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mPointID;
    /**  */
    int mSystemID;
    /**  */
    double mX;
    /**  */
    double mY;
    /**  */
    double mZ;
  };
  /** Data type for holding a list of FEMPoints fields structures. */
  sequence<FEMPointsFields> FEMPointsFieldsList;

  /**
   * FEMThermoOrthMaterials fields structure.
   */
  struct FEMThermoOrthMaterialsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mDensity;
    /**  */
    int mMaterialID;
    /**  */
    double mTREF;
  };
  /** Data type for holding a list of FEMThermoOrthMaterials fields structures. */
  sequence<FEMThermoOrthMaterialsFields> FEMThermoOrthMaterialsFieldsList;

  /**
   * FEMConstraints fields structure.
   */
  struct FEMConstraintsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mConstraintID;
    /**  */
    string mDescription;
    /**  */
    string mSlaveDOF;
    /**  */
    int mSlaveNode;
  };
  /** Data type for holding a list of FEMConstraints fields structures. */
  sequence<FEMConstraintsFields> FEMConstraintsFieldsList;

  /**
   * FEMMCrigidities fields structure.
   */
  struct FEMMCrigiditiesFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mAcurveType;
    /**  */
    int mAlpha;
    /**  */
    double mAxialCF;
    /**  */
    string mBcurveType;
    /**  */
    double mBendingCF;
    /**  */
    int mBeta;
    /**  */
    double mDensity;
    /**  */
    int mForceAxID;
    /**  */
    string mHardening;
    /**  */
    double mMassArea;
    /**  */
    double mMassR;
    /**  */
    double mMassS;
    /**  */
    double mMassT;
    /**  */
    int mMomentRID;
    /**  */
    int mMomentSID;
    /**  */
    int mMomentTID;
    /**  */
    int mRigidityID;
    /**  */
    string mTcurveType;
    /**  */
    double mTorsionCF;
  };
  /** Data type for holding a list of FEMMCrigidities fields structures. */
  sequence<FEMMCrigiditiesFields> FEMMCrigiditiesFieldsList;

  /**
   * FEMSkeySysNode fields structure.
   */
  struct FEMSkeySysNodeFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    long mNode1;
    /**  */
    long mNode2;
    /**  */
    long mNode3;
    /**  */
    int mSkewSystemID;
  };
  /** Data type for holding a list of FEMSkeySysNode fields structures. */
  sequence<FEMSkeySysNodeFields> FEMSkeySysNodeFieldsList;

  /**
   * FEMIsoBeamGroup fields structure.
   */
  struct FEMIsoBeamGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDisplacement;
    /**  */
    long mGroup;
    /**  */
    string mIniStrain;
    /**  */
    int mMaterialID;
    /**  */
    string mResult;
    /**  */
    int mSectionID;
  };
  /** Data type for holding a list of FEMIsoBeamGroup fields structures. */
  sequence<FEMIsoBeamGroupFields> FEMIsoBeamGroupFieldsList;

  /**
   * FEMShellDOF fields structure.
   */
  struct FEMShellDOFFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDOFnumber;
    /**  */
    long mNode;
    /**  */
    int mVectorID;
  };
  /** Data type for holding a list of FEMShellDOF fields structures. */
  sequence<FEMShellDOFFields> FEMShellDOFFieldsList;

  /**
   * FEMCrossSections fields structure.
   */
  struct FEMCrossSectionsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mSectionID;
    /**  */
    string mSectionType;
  };
  /** Data type for holding a list of FEMCrossSections fields structures. */
  sequence<FEMCrossSectionsFields> FEMCrossSectionsFieldsList;

  /**
   * FEMTwistMomentData fields structure.
   */
  struct FEMTwistMomentDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mMoment;
    /**  */
    int mRecordNmb;
    /**  */
    double mTwist;
    /**  */
    int mTwistMomentID;
  };
  /** Data type for holding a list of FEMTwistMomentData fields structures. */
  sequence<FEMTwistMomentDataFields> FEMTwistMomentDataFieldsList;

  /**
   * FEMShell fields structure.
   */
  struct FEMShellFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    long mGroup;
    /**  */
    long mMaterial;
    /**  */
    long mN1;
    /**  */
    long mN2;
    /**  */
    long mN3;
    /**  */
    long mN4;
    /**  */
    long mN5;
    /**  */
    long mN6;
    /**  */
    long mN7;
    /**  */
    long mN8;
    /**  */
    long mN9;
    /**  */
    short mNodeAmount;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
    /**  */
    double mT1;
    /**  */
    double mT2;
    /**  */
    double mT3;
    /**  */
    double mT4;
    /**  */
    double mT5;
    /**  */
    double mT6;
    /**  */
    double mT7;
    /**  */
    double mT8;
    /**  */
    double mT9;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
  };
  /** Data type for holding a list of FEMShell fields structures. */
  sequence<FEMShellFields> FEMShellFieldsList;

  /**
   * FEMNTNContact fields structure.
   */
  struct FEMNTNContactFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mContactorNode;
    /**  */
    int mContGroupID;
    /**  */
    int mContPair;
    /**  */
    string mPrintRes;
    /**  */
    int mRecordNmb;
    /**  */
    string mSaveRes;
    /**  */
    int mTargetNode;
    /**  */
    double mTargetNx;
    /**  */
    double mTargetNy;
    /**  */
    double mTargetNz;
  };
  /** Data type for holding a list of FEMNTNContact fields structures. */
  sequence<FEMNTNContactFields> FEMNTNContactFieldsList;

  /**
   * FEMShellLayer fields structure.
   */
  struct FEMShellLayerFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mGroup;
    /**  */
    int mLayerNumber;
    /**  */
    int mMaterialID;
    /**  */
    double mPThick;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMShellLayer fields structures. */
  sequence<FEMShellLayerFields> FEMShellLayerFieldsList;

  /**
   * FEMSkewSysAngles fields structure.
   */
  struct FEMSkewSysAnglesFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    double mPHI;
    /**  */
    int mSkewSystemID;
    /**  */
    double mTHETA;
    /**  */
    double mXSI;
  };
  /** Data type for holding a list of FEMSkewSysAngles fields structures. */
  sequence<FEMSkewSysAnglesFields> FEMSkewSysAnglesFieldsList;

  /**
   * FEMGroundMotionRecord fields structure.
   */
  struct FEMGroundMotionRecordFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDescription;
    /**  */
    short mGMRecordID;
    /**  */
    string mGMRecordName;
  };
  /** Data type for holding a list of FEMGroundMotionRecord fields structures. */
  sequence<FEMGroundMotionRecordFields> FEMGroundMotionRecordFieldsList;

  /**
   * FEMGeneralGroup fields structure.
   */
  struct FEMGeneralGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mGroup;
    /**  */
    int mMatrixSet;
    /**  */
    string mResult;
    /**  */
    string mSkewSystem;
  };
  /** Data type for holding a list of FEMGeneralGroup fields structures. */
  sequence<FEMGeneralGroupFields> FEMGeneralGroupFieldsList;

  /**
   * FEMTwoDSolid fields structure.
   */
  struct FEMTwoDSolidFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mBet;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    long mGroup;
    /**  */
    int mMaterialID;
    /**  */
    long mN1;
    /**  */
    long mN2;
    /**  */
    long mN3;
    /**  */
    long mN4;
    /**  */
    long mN5;
    /**  */
    long mN6;
    /**  */
    long mN7;
    /**  */
    long mN8;
    /**  */
    long mN9;
    /**  */
    short mNodeAmount;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
    /**  */
    double mThick;
  };
  /** Data type for holding a list of FEMTwoDSolid fields structures. */
  sequence<FEMTwoDSolidFields> FEMTwoDSolidFieldsList;

  /**
   * FEMAppliedTemperature fields structure.
   */
  struct FEMAppliedTemperatureFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mArrivalTime;
    /**  */
    double mFactor;
    /**  */
    long mNode;
    /**  */
    int mRecordNmbr;
    /**  */
    int mTimeFunctionID;
  };
  /** Data type for holding a list of FEMAppliedTemperature fields structures. */
  sequence<FEMAppliedTemperatureFields> FEMAppliedTemperatureFieldsList;

  /**
   * FEMMatrixSets fields structure.
   */
  struct FEMMatrixSetsFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mDamping;
    /**  */
    string mDescription;
    /**  */
    int mMass;
    /**  */
    int mMatrixSetID;
    /**  */
    int mStiffness;
    /**  */
    int mStress;
  };
  /** Data type for holding a list of FEMMatrixSets fields structures. */
  sequence<FEMMatrixSetsFields> FEMMatrixSetsFieldsList;

  /**
   * FEMConstraintCoef fields structure.
   */
  struct FEMConstraintCoefFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mCoefficient;
    /**  */
    int mConstraintID;
    /**  */
    string mDescription;
    /**  */
    string mMasterDOF;
    /**  */
    int mMasterNode;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMConstraintCoef fields structures. */
  sequence<FEMConstraintCoefFields> FEMConstraintCoefFieldsList;

  /**
   * FEMSectionBox fields structure.
   */
  struct FEMSectionBoxFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mHeight;
    /**  */
    double mSC;
    /**  */
    int mSectionID;
    /**  */
    double mSSarea;
    /**  */
    double mTC;
    /**  */
    double mThick1;
    /**  */
    double mThick2;
    /**  */
    double mTorfac;
    /**  */
    double mTSarea;
    /**  */
    double mWidth;
  };
  /** Data type for holding a list of FEMSectionBox fields structures. */
  sequence<FEMSectionBoxFields> FEMSectionBoxFieldsList;

  /**
   * FEMNKDisplForce fields structure.
   */
  struct FEMNKDisplForceFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mDisplacement;
    /**  */
    double mForce;
    /**  */
    int mPropertyID;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMNKDisplForce fields structures. */
  sequence<FEMNKDisplForceFields> FEMNKDisplForceFieldsList;

  /**
   * FEMPlasticStrainStress fields structure.
   */
  struct FEMPlasticStrainStressFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mMaterialID;
    /**  */
    int mRecordNumber;
    /**  */
    double mStrain;
    /**  */
    double mStress;
  };
  /** Data type for holding a list of FEMPlasticStrainStress fields structures. */
  sequence<FEMPlasticStrainStressFields> FEMPlasticStrainStressFieldsList;

  /**
   * FEMShellAxesOrthoData fields structure.
   */
  struct FEMShellAxesOrthoDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mAxOrthoID;
    /**  */
    int mElementID;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMShellAxesOrthoData fields structures. */
  sequence<FEMShellAxesOrthoDataFields> FEMShellAxesOrthoDataFieldsList;

  /**
   * FEMGeneralNode fields structure.
   */
  struct FEMGeneralNodeFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mElementID;
    /**  */
    long mGroup;
    /**  */
    short mLocalNmb;
    /**  */
    long mNode;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMGeneralNode fields structures. */
  sequence<FEMGeneralNodeFields> FEMGeneralNodeFieldsList;

  /**
   * FEMStrLines fields structure.
   */
  struct FEMStrLinesFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mLineID;
    /**  */
    int mP1;
    /**  */
    int mP2;
  };
  /** Data type for holding a list of FEMStrLines fields structures. */
  sequence<FEMStrLinesFields> FEMStrLinesFieldsList;

  /**
   * FEMContactSurface fields structure.
   */
  struct FEMContactSurfaceFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mContGroupID;
    /**  */
    int mContSegment;
    /**  */
    int mContSurface;
    /**  */
    long mN1;
    /**  */
    long mN2;
    /**  */
    long mN3;
    /**  */
    long mN4;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMContactSurface fields structures. */
  sequence<FEMContactSurfaceFields> FEMContactSurfaceFieldsList;

  /**
   * FEMMCForceData fields structure.
   */
  struct FEMMCForceDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    int mCurvMomentID;
    /**  */
    double mForce;
    /**  */
    int mMomentSTID;
    /**  */
    int mRecordNmb;
  };
  /** Data type for holding a list of FEMMCForceData fields structures. */
  sequence<FEMMCForceDataFields> FEMMCForceDataFieldsList;

  /**
   * FEMSpring fields structure.
   */
  struct FEMSpringFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mAX;
    /**  */
    double mAY;
    /**  */
    double mAZ;
    /**  */
    string mDescription;
    /**  */
    int mElementID;
    /**  */
    long mGroup;
    /**  */
    short mID1;
    /**  */
    short mID2;
    /**  */
    long mN1;
    /**  */
    long mN2;
    /**  */
    int mPropertySet;
    /**  */
    int mRecordNmb;
    /**  */
    string mSave;
    /**  */
    double mTBirth;
    /**  */
    double mTDeath;
  };
  /** Data type for holding a list of FEMSpring fields structures. */
  sequence<FEMSpringFields> FEMSpringFieldsList;

  /**
   * FEMSpringGroup fields structure.
   */
  struct FEMSpringGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mBolt;
    /**  */
    long mGroup;
    /**  */
    string mNonlinear;
    /**  */
    int mPropertySet;
    /**  */
    string mResult;
    /**  */
    string mSkewSystem;
  };
  /** Data type for holding a list of FEMSpringGroup fields structures. */
  sequence<FEMSpringGroupFields> FEMSpringGroupFieldsList;

  /**
   * FEMShellGroup fields structure.
   */
  struct FEMShellGroupFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mDisplacement;
    /**  */
    long mGroup;
    /**  */
    long mMaterial;
    /**  */
    int mNLayers;
    /**  */
    string mResult;
    /**  */
    short mSectionResult;
    /**  */
    string mStressReference;
    /**  */
    double mThickness;
  };
  /** Data type for holding a list of FEMShellGroup fields structures. */
  sequence<FEMShellGroupFields> FEMShellGroupFieldsList;

  /**
   * DaqUnit fields structure.
   */
  struct DaqUnitFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mModel;
    /**  */
    string mIdentifier;
  };
  /** Data type for holding a list of DaqUnit fields structures. */
  sequence<DaqUnitFields> DaqUnitFieldsList;

  /**
   * DaqUnitChannel fields structure.
   */
  struct DaqUnitChannelFields {
    /** Unique identifier (for internal use only). */
    long id;
    /** Corresponding data acquisition unit.  */
    long mUnit;
    /** Local channel number.  */
    short mNumber;
  };
  /** Data type for holding a list of DaqUnitChannel fields structures. */
  sequence<DaqUnitChannelFields> DaqUnitChannelFieldsList;

  /**
   * Sensor fields structure.
   */
  struct SensorFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    SensorType mType;
    /**  */
    string mModel;
    /**  */
    string mIdentifier;
  };
  /** Data type for holding a list of Sensor fields structures. */
  sequence<SensorFields> SensorFieldsList;

  /**
   * Transducer fields structure.
   */
  struct TransducerFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    string mModel;
    /**  */
    string mIdentifier;
  };
  /** Data type for holding a list of Transducer fields structures. */
  sequence<TransducerFields> TransducerFieldsList;

  /**
   * Experiment fields structure.
   */
  struct ExperimentFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    short mFrequency;
    /**  */
    int mDuration;
    /**  */
    int mBufferData;
  };
  /** Data type for holding a list of Experiment fields structures. */
  sequence<ExperimentFields> ExperimentFieldsList;

  /**
   * Cycle fields structure.
   */
  struct CycleFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    double mTime;
    /**  */
    long mExperiment;
  };
  /** Data type for holding a list of Cycle fields structures. */
  sequence<CycleFields> CycleFieldsList;

  /**
   * SensorTimeData fields structure.
   */
  struct SensorTimeDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mStructure;
    /**  */
    long mSensor;
    /**  */
    short mChannel;
    /**  */
    double mStartTime;
    /**  */
    string mStatus;
    /**  */
    long mNode;
  };
  /** Data type for holding a list of SensorTimeData fields structures. */
  sequence<SensorTimeDataFields> SensorTimeDataFieldsList;

  /**
   * TransducerTimeData fields structure.
   */
  struct TransducerTimeDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mTransducer;
    /**  */
    long mSensor;
    /**  */
    long mNode;
    /**  */
    short mOrientation;
    /**  */
    int mAmplification;
    /**  */
    short mChannel;
    /**  */
    string mFilter;
    /**  */
    double mStartTime;
    /**  */
    string mStatus;
  };
  /** Data type for holding a list of TransducerTimeData fields structures. */
  sequence<TransducerTimeDataFields> TransducerTimeDataFieldsList;

  /**
   * ExperimentStructureData fields structure.
   */
  struct ExperimentStructureDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mExperiment;
    /**  */
    long mStructure;
    /**  */
    double mStartTime;
    /**  */
    string mStatus;
  };
  /** Data type for holding a list of ExperimentStructureData fields structures. */
  sequence<ExperimentStructureDataFields> ExperimentStructureDataFieldsList;

  /**
   * TransducerCycleData fields structure.
   */
  struct TransducerCycleDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /**  */
    long mTransducer;
    /**  */
    long mCycle;
    /**  */
    float mSoftwareScaling;
  };
  /** Data type for holding a list of TransducerCycleData fields structures. */
  sequence<TransducerCycleDataFields> TransducerCycleDataFieldsList;

  /**
   * MappingMatrix fields structure.
   *
   * Note that the array data is not included here.
   * See [MappingMatrix] to access the array data.
   */
  struct MappingMatrixFields {
    /** Unique identifier (for internal use only). */
    long id;
    /** Parent structure  */
    long mStructure;
    /** Short name  */
    string mName;
    /** Description  */
    string mDescription;
    /** Output quantity  */
    Quantity mOutputQuantity;
    /** Input quantity  */
    Quantity mInputQuantity;
  };
  /** Data type for holding a list of MappingMatrix fields structures. */
  sequence<MappingMatrixFields> MappingMatrixFieldsList;

  /**
   * DaqUnitChannelData fields structure.
   *
   * Note that the signal data is not included here.
   * See [DaqUnitChannelData] to access the signal data.
   */
  struct DaqUnitChannelDataFields {
    /** Unique identifier (for internal use only). */
    long id;
    /** Parent sensor  */
    long mChannel;
  };
  /** Data type for holding a list of DaqUnitChannelData fields structures. */
  sequence<DaqUnitChannelDataFields> DaqUnitChannelDataFieldsList;

  /**
   * AccelMeas fields structure.
   *
   * Note that the signal data is not included here.
   * See [AccelMeas] to access the signal data.
   */
  struct AccelMeasFields {
    /** Unique identifier (for internal use only). */
    long id;
    /** Parent sensor  */
    long mSensor;
    /** Short name  */
    string mName;
    /** Description  */
    string mDescription;
  };
  /** Data type for holding a list of AccelMeas fields structures. */
  sequence<AccelMeasFields> AccelMeasFieldsList;

  /**
   * Structure, such as a bridge. 
   *
   */
  interface Structure {
    /**
     * Gets the fields of this Structure object.
     *
     * @return Structure object fields
     */
    idempotent StructureFields getFields();

    /**
     * Updates the fields of this Structure object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Name</li>
     * <li>Description</li>
     * <li>Type</li>
     * <li>DistanceUnit</li>
     * <li>ForceUnit</li>
     * <li>WeightUnit</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(StructureFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the IDs of the [FEMLoadMassProportional] child objects.
     *
     * The child objects refer to this object.
     *
     * @return list of [FEMLoadMassProportional] child object IDs
     */
    idempotent IdList getFEMLoadMassProportionalChildIds();

    /**
     * Gets the IDs of the [FEMMaterial] child objects.
     *
     * The child objects refer to this object.
     *
     * @return list of [FEMMaterial] child object IDs
     */
    idempotent IdList getFEMMaterialChildIds();

    /**
     * Gets the IDs of the [FEMGroup] child objects.
     *
     * The child objects refer to this object.
     *
     * @return list of [FEMGroup] child object IDs
     */
    idempotent IdList getFEMGroupChildIds();
  };

  /**
   * Finite element model degrees of freedom. 
   *
   */
  interface FEMDof {
    /**
     * Gets the fields of this FEMDof object.
     *
     * @return FEMDof object fields
     */
    idempotent FEMDofFields getFields();

    /**
     * Updates the fields of this FEMDof object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Structure</li>
     * <li>LocalId</li>
     * <li>Node</li>
     * <li>Direction</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMDofFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Structure] object refered to by the
     * Structure field.
     *
     * @return Structure interface
     */
    idempotent Structure* getStructure();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMNodalMass {
    /**
     * Gets the fields of this FEMNodalMass object.
     *
     * @return FEMNodalMass object fields
     */
    idempotent FEMNodalMassFields getFields();

    /**
     * Updates the fields of this FEMNodalMass object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>Mass1</li>
     * <li>Mass2</li>
     * <li>Mass3</li>
     * <li>Mass4</li>
     * <li>Mass5</li>
     * <li>Mass6</li>
     * <li>Node</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMNodalMassFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMNLElasticStrainStress {
    /**
     * Gets the fields of this FEMNLElasticStrainStress object.
     *
     * @return FEMNLElasticStrainStress object fields
     */
    idempotent FEMNLElasticStrainStressFields getFields();

    /**
     * Updates the fields of this FEMNLElasticStrainStress object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>MaterialID</li>
     * <li>RecordNumber</li>
     * <li>Strain</li>
     * <li>Stress</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMNLElasticStrainStressFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMBoundary {
    /**
     * Gets the fields of this FEMBoundary object.
     *
     * @return FEMBoundary object fields
     */
    idempotent FEMBoundaryFields getFields();

    /**
     * Updates the fields of this FEMBoundary object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Node</li>
     * <li>Ovalization</li>
     * <li>Phi</li>
     * <li>Rx</li>
     * <li>Ry</li>
     * <li>Rz</li>
     * <li>Ux</li>
     * <li>Uy</li>
     * <li>Uz</li>
     * <li>Warping</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMBoundaryFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMSectionPipe {
    /**
     * Gets the fields of this FEMSectionPipe object.
     *
     * @return FEMSectionPipe object fields
     */
    idempotent FEMSectionPipeFields getFields();

    /**
     * Updates the fields of this FEMSectionPipe object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Diameter</li>
     * <li>SC</li>
     * <li>SectionID</li>
     * <li>SSarea</li>
     * <li>TC</li>
     * <li>Thickness</li>
     * <li>Torfac</li>
     * <li>TSarea</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSectionPipeFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMCoordSystem {
    /**
     * Gets the fields of this FEMCoordSystem object.
     *
     * @return FEMCoordSystem object fields
     */
    idempotent FEMCoordSystemFields getFields();

    /**
     * Updates the fields of this FEMCoordSystem object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AX</li>
     * <li>AY</li>
     * <li>AZ</li>
     * <li>BBY</li>
     * <li>BX</li>
     * <li>BZ</li>
     * <li>Description</li>
     * <li>Mode</li>
     * <li>P1</li>
     * <li>P2</li>
     * <li>P3</li>
     * <li>Type</li>
     * <li>Xorigin</li>
     * <li>Yorigin</li>
     * <li>Zorigin</li>
     * <li>LocalID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMCoordSystemFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMNode {
    /**
     * Gets the fields of this FEMNode object.
     *
     * @return FEMNode object fields
     */
    idempotent FEMNodeFields getFields();

    /**
     * Updates the fields of this FEMNode object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>System</li>
     * <li>X</li>
     * <li>Y</li>
     * <li>Z</li>
     * <li>LocalID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMCoordSystem] object refered to by the
     * System field.
     *
     * @return System interface
     */
    idempotent FEMCoordSystem* getSystem();
  };

  /**
   * 
   *
   */
  interface FEMTruss {
    /**
     * Gets the fields of this FEMTruss object.
     *
     * @return FEMTruss object fields
     */
    idempotent FEMTrussFields getFields();

    /**
     * Updates the fields of this FEMTruss object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>Epsin</li>
     * <li>Gapwidth</li>
     * <li>Group</li>
     * <li>Material</li>
     * <li>N1</li>
     * <li>N2</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * <li>SectionArea</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMTrussFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMMaterial] object refered to by the
     * Material field.
     *
     * @return Material interface
     */
    idempotent FEMMaterial* getMaterial();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N1 field.
     *
     * @return N1 interface
     */
    idempotent FEMNode* getN1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N2 field.
     *
     * @return N2 interface
     */
    idempotent FEMNode* getN2();
  };

  /**
   * 
   *
   */
  interface FEMTimeFunctionData {
    /**
     * Gets the fields of this FEMTimeFunctionData object.
     *
     * @return FEMTimeFunctionData object fields
     */
    idempotent FEMTimeFunctionDataFields getFields();

    /**
     * Updates the fields of this FEMTimeFunctionData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>DataTime</li>
     * <li>GMRecordID</li>
     * <li>RecordNmb</li>
     * <li>TimeFunctionID</li>
     * <li>TimeValue</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMTimeFunctionDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMPlasticMlMaterials {
    /**
     * Gets the fields of this FEMPlasticMlMaterials object.
     *
     * @return FEMPlasticMlMaterials object fields
     */
    idempotent FEMPlasticMlMaterialsFields getFields();

    /**
     * Updates the fields of this FEMPlasticMlMaterials object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ALPHA</li>
     * <li>Density</li>
     * <li>E</li>
     * <li>Hardening</li>
     * <li>MaterialID</li>
     * <li>NU</li>
     * <li>TREF</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMPlasticMlMaterialsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMPlateGroup {
    /**
     * Gets the fields of this FEMPlateGroup object.
     *
     * @return FEMPlateGroup object fields
     */
    idempotent FEMPlateGroupFields getFields();

    /**
     * Updates the fields of this FEMPlateGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Displacement</li>
     * <li>Group</li>
     * <li>IniStrain</li>
     * <li>MaterialID</li>
     * <li>Result</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMPlateGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMBeam {
    /**
     * Gets the fields of this FEMBeam object.
     *
     * @return FEMBeam object fields
     */
    idempotent FEMBeamFields getFields();

    /**
     * Updates the fields of this FEMBeam object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AuxNode</li>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>EndRelease</li>
     * <li>Epsin</li>
     * <li>Group</li>
     * <li>IRigidEnd</li>
     * <li>JRigidEnd</li>
     * <li>MaterialID</li>
     * <li>Node1</li>
     * <li>Node2</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * <li>SectionID</li>
     * <li>Subdivision</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMBeamFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * AuxNode field.
     *
     * @return AuxNode interface
     */
    idempotent FEMNode* getAuxNode();

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node1 field.
     *
     * @return Node1 interface
     */
    idempotent FEMNode* getNode1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node2 field.
     *
     * @return Node2 interface
     */
    idempotent FEMNode* getNode2();
  };

  /**
   * 
   *
   */
  interface FEMCurvMomentData {
    /**
     * Gets the fields of this FEMCurvMomentData object.
     *
     * @return FEMCurvMomentData object fields
     */
    idempotent FEMCurvMomentDataFields getFields();

    /**
     * Updates the fields of this FEMCurvMomentData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Curvature</li>
     * <li>CurvMomentID</li>
     * <li>Moment</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMCurvMomentDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMPropertysets {
    /**
     * Gets the fields of this FEMPropertysets object.
     *
     * @return FEMPropertysets object fields
     */
    idempotent FEMPropertysetsFields getFields();

    /**
     * Updates the fields of this FEMPropertysets object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>C</li>
     * <li>Description</li>
     * <li>K</li>
     * <li>M</li>
     * <li>NC</li>
     * <li>NK</li>
     * <li>NM</li>
     * <li>Nonlinear</li>
     * <li>PropertysetID</li>
     * <li>S</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMPropertysetsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMOrthotropicMaterial {
    /**
     * Gets the fields of this FEMOrthotropicMaterial object.
     *
     * @return FEMOrthotropicMaterial object fields
     */
    idempotent FEMOrthotropicMaterialFields getFields();

    /**
     * Updates the fields of this FEMOrthotropicMaterial object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Density</li>
     * <li>EA</li>
     * <li>EB</li>
     * <li>EC</li>
     * <li>GAB</li>
     * <li>GAC</li>
     * <li>GBC</li>
     * <li>Material</li>
     * <li>NUAB</li>
     * <li>NUAC</li>
     * <li>NUBC</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMOrthotropicMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMMaterial] object refered to by the
     * Material field.
     *
     * @return Material interface
     */
    idempotent FEMMaterial* getMaterial();
  };

  /**
   * 
   *
   */
  interface FEMAppliedLoads {
    /**
     * Gets the fields of this FEMAppliedLoads object.
     *
     * @return FEMAppliedLoads object fields
     */
    idempotent FEMAppliedLoadsFields getFields();

    /**
     * Updates the fields of this FEMAppliedLoads object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AppliedLoadNmb</li>
     * <li>ArrivalTime</li>
     * <li>LoadID</li>
     * <li>LoadType</li>
     * <li>SiteType</li>
     * <li>TimeFunctionID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMAppliedLoadsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMThermoOrthData {
    /**
     * Gets the fields of this FEMThermoOrthData object.
     *
     * @return FEMThermoOrthData object fields
     */
    idempotent FEMThermoOrthDataFields getFields();

    /**
     * Updates the fields of this FEMThermoOrthData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ALPHAA</li>
     * <li>ALPHAB</li>
     * <li>ALPHAC</li>
     * <li>EA</li>
     * <li>EB</li>
     * <li>EC</li>
     * <li>GAB</li>
     * <li>GAC</li>
     * <li>GBC</li>
     * <li>MaterialID</li>
     * <li>NUAB</li>
     * <li>NUAC</li>
     * <li>NUBC</li>
     * <li>RecordNmb</li>
     * <li>Theta</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMThermoOrthDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMContactPairs {
    /**
     * Gets the fields of this FEMContactPairs object.
     *
     * @return FEMContactPairs object fields
     */
    idempotent FEMContactPairsFields getFields();

    /**
     * Updates the fields of this FEMContactPairs object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ContactorSurf</li>
     * <li>ContGroupID</li>
     * <li>ContPair</li>
     * <li>FContactor</li>
     * <li>Friction</li>
     * <li>FTarget</li>
     * <li>HeatTransf</li>
     * <li>RecordNmb</li>
     * <li>TargetSurf</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMContactPairsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMGeneral {
    /**
     * Gets the fields of this FEMGeneral object.
     *
     * @return FEMGeneral object fields
     */
    idempotent FEMGeneralFields getFields();

    /**
     * Updates the fields of this FEMGeneral object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>Group</li>
     * <li>MatrixSet</li>
     * <li>NodeAmount</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMGeneralFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMBeamNode {
    /**
     * Gets the fields of this FEMBeamNode object.
     *
     * @return FEMBeamNode object fields
     */
    idempotent FEMBeamNodeFields getFields();

    /**
     * Updates the fields of this FEMBeamNode object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Displacement</li>
     * <li>Group</li>
     * <li>IniStrain</li>
     * <li>MaterialID</li>
     * <li>MC</li>
     * <li>MCrigidity</li>
     * <li>REmultiplyer</li>
     * <li>Result</li>
     * <li>REtype</li>
     * <li>RINT</li>
     * <li>SectionID</li>
     * <li>SINT</li>
     * <li>TINT</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMBeamNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMSectionRect {
    /**
     * Gets the fields of this FEMSectionRect object.
     *
     * @return FEMSectionRect object fields
     */
    idempotent FEMSectionRectFields getFields();

    /**
     * Updates the fields of this FEMSectionRect object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Height</li>
     * <li>IShear</li>
     * <li>SC</li>
     * <li>SectionID</li>
     * <li>SSarea</li>
     * <li>TC</li>
     * <li>Torfac</li>
     * <li>TSarea</li>
     * <li>Width</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSectionRectFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMBeamLoad {
    /**
     * Gets the fields of this FEMBeamLoad object.
     *
     * @return FEMBeamLoad object fields
     */
    idempotent FEMBeamLoadFields getFields();

    /**
     * Updates the fields of this FEMBeamLoad object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ArrivalTime</li>
     * <li>DeformDepend</li>
     * <li>DirectFilter</li>
     * <li>ElementID</li>
     * <li>Face</li>
     * <li>Group</li>
     * <li>P1</li>
     * <li>P2</li>
     * <li>RecordNmb</li>
     * <li>TimeFunctionID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMBeamLoadFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMLoadMassProportional {
    /**
     * Gets the fields of this FEMLoadMassProportional object.
     *
     * @return FEMLoadMassProportional object fields
     */
    idempotent FEMLoadMassProportionalFields getFields();

    /**
     * Updates the fields of this FEMLoadMassProportional object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AX</li>
     * <li>AY</li>
     * <li>AZ</li>
     * <li>LoadID</li>
     * <li>Magnitude</li>
     * <li>Structure</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMLoadMassProportionalFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Structure] object refered to by the
     * Structure field.
     *
     * @return Structure interface
     */
    idempotent Structure* getStructure();
  };

  /**
   * 
   *
   */
  interface FEMLink {
    /**
     * Gets the fields of this FEMLink object.
     *
     * @return FEMLink object fields
     */
    idempotent FEMLinkFields getFields();

    /**
     * Updates the fields of this FEMLink object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>Displacement</li>
     * <li>MasterNode</li>
     * <li>RLID</li>
     * <li>SlaveNode</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMLinkFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * MasterNode field.
     *
     * @return MasterNode interface
     */
    idempotent FEMNode* getMasterNode();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * SlaveNode field.
     *
     * @return SlaveNode interface
     */
    idempotent FEMNode* getSlaveNode();
  };

  /**
   * 
   *
   */
  interface FEMAxesNode {
    /**
     * Gets the fields of this FEMAxesNode object.
     *
     * @return FEMAxesNode object fields
     */
    idempotent FEMAxesNodeFields getFields();

    /**
     * Updates the fields of this FEMAxesNode object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AxNodeID</li>
     * <li>Group</li>
     * <li>Node1</li>
     * <li>Node2</li>
     * <li>Node3</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMAxesNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node1 field.
     *
     * @return Node1 interface
     */
    idempotent FEMNode* getNode1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node2 field.
     *
     * @return Node2 interface
     */
    idempotent FEMNode* getNode2();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node3 field.
     *
     * @return Node3 interface
     */
    idempotent FEMNode* getNode3();
  };

  /**
   * 
   *
   */
  interface FEMNMTimeMass {
    /**
     * Gets the fields of this FEMNMTimeMass object.
     *
     * @return FEMNMTimeMass object fields
     */
    idempotent FEMNMTimeMassFields getFields();

    /**
     * Updates the fields of this FEMNMTimeMass object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Mass</li>
     * <li>PropertyID</li>
     * <li>RecordNmb</li>
     * <li>TimeValue</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMNMTimeMassFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMAppliedDisplacement {
    /**
     * Gets the fields of this FEMAppliedDisplacement object.
     *
     * @return FEMAppliedDisplacement object fields
     */
    idempotent FEMAppliedDisplacementFields getFields();

    /**
     * Updates the fields of this FEMAppliedDisplacement object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ArrivalTime</li>
     * <li>Description</li>
     * <li>Direction</li>
     * <li>Factor</li>
     * <li>Node</li>
     * <li>RecordNmb</li>
     * <li>TimeFunctionID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMAppliedDisplacementFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMTimeFunctions {
    /**
     * Gets the fields of this FEMTimeFunctions object.
     *
     * @return FEMTimeFunctions object fields
     */
    idempotent FEMTimeFunctionsFields getFields();

    /**
     * Updates the fields of this FEMTimeFunctions object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>TimeFunctionID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMTimeFunctionsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMForceStrainData {
    /**
     * Gets the fields of this FEMForceStrainData object.
     *
     * @return FEMForceStrainData object fields
     */
    idempotent FEMForceStrainDataFields getFields();

    /**
     * Updates the fields of this FEMForceStrainData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Force</li>
     * <li>ForceAxID</li>
     * <li>RecordNmb</li>
     * <li>Strain</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMForceStrainDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMSkewDOF {
    /**
     * Gets the fields of this FEMSkewDOF object.
     *
     * @return FEMSkewDOF object fields
     */
    idempotent FEMSkewDOFFields getFields();

    /**
     * Updates the fields of this FEMSkewDOF object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>Node</li>
     * <li>SkewSystemID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSkewDOFFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMSectionI {
    /**
     * Gets the fields of this FEMSectionI object.
     *
     * @return FEMSectionI object fields
     */
    idempotent FEMSectionIFields getFields();

    /**
     * Updates the fields of this FEMSectionI object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Height</li>
     * <li>SC</li>
     * <li>SectionID</li>
     * <li>SSarea</li>
     * <li>TC</li>
     * <li>Thick1</li>
     * <li>Thick2</li>
     * <li>Thick3</li>
     * <li>Torfac</li>
     * <li>TSarea</li>
     * <li>Width1</li>
     * <li>Width2</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSectionIFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMPlasticBilinearMaterial {
    /**
     * Gets the fields of this FEMPlasticBilinearMaterial object.
     *
     * @return FEMPlasticBilinearMaterial object fields
     */
    idempotent FEMPlasticBilinearMaterialFields getFields();

    /**
     * Updates the fields of this FEMPlasticBilinearMaterial object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Alpha</li>
     * <li>Density</li>
     * <li>E</li>
     * <li>EPA</li>
     * <li>ET</li>
     * <li>Hardening</li>
     * <li>Material</li>
     * <li>NU</li>
     * <li>TRef</li>
     * <li>Yield</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMPlasticBilinearMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMMaterial] object refered to by the
     * Material field.
     *
     * @return Material interface
     */
    idempotent FEMMaterial* getMaterial();
  };

  /**
   * 
   *
   */
  interface FEMMTForceData {
    /**
     * Gets the fields of this FEMMTForceData object.
     *
     * @return FEMMTForceData object fields
     */
    idempotent FEMMTForceDataFields getFields();

    /**
     * Updates the fields of this FEMMTForceData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Force</li>
     * <li>MomentRID</li>
     * <li>RecordNmb</li>
     * <li>TwistMomentID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMMTForceDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMShellPressure {
    /**
     * Gets the fields of this FEMShellPressure object.
     *
     * @return FEMShellPressure object fields
     */
    idempotent FEMShellPressureFields getFields();

    /**
     * Updates the fields of this FEMShellPressure object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ArrivalTime</li>
     * <li>DeformDepend</li>
     * <li>Description</li>
     * <li>DirectFilter</li>
     * <li>ElementID</li>
     * <li>Face</li>
     * <li>Group</li>
     * <li>Nodaux</li>
     * <li>P1</li>
     * <li>P2</li>
     * <li>P3</li>
     * <li>P4</li>
     * <li>RecordNmb</li>
     * <li>TimeFunctionID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMShellPressureFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMMatrices {
    /**
     * Gets the fields of this FEMMatrices object.
     *
     * @return FEMMatrices object fields
     */
    idempotent FEMMatricesFields getFields();

    /**
     * Updates the fields of this FEMMatrices object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>Lump</li>
     * <li>MatrixID</li>
     * <li>MatrixType</li>
     * <li>ND</li>
     * <li>NS</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMMatricesFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMDamping {
    /**
     * Gets the fields of this FEMDamping object.
     *
     * @return FEMDamping object fields
     */
    idempotent FEMDampingFields getFields();

    /**
     * Updates the fields of this FEMDamping object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ALPHA</li>
     * <li>BETA</li>
     * <li>Group</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMDampingFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMMaterial {
    /**
     * Gets the fields of this FEMMaterial object.
     *
     * @return FEMMaterial object fields
     */
    idempotent FEMMaterialFields getFields();

    /**
     * Updates the fields of this FEMMaterial object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>MaterialType</li>
     * <li>LocalID</li>
     * <li>Structure</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Structure] object refered to by the
     * Structure field.
     *
     * @return Structure interface
     */
    idempotent Structure* getStructure();
  };

  /**
   * 
   *
   */
  interface FEMMatrixData {
    /**
     * Gets the fields of this FEMMatrixData object.
     *
     * @return FEMMatrixData object fields
     */
    idempotent FEMMatrixDataFields getFields();

    /**
     * Updates the fields of this FEMMatrixData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Coeff</li>
     * <li>ColumnIndex</li>
     * <li>MatrixID</li>
     * <li>RecordNmb</li>
     * <li>RowIndex</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMMatrixDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMShellAxesOrtho {
    /**
     * Gets the fields of this FEMShellAxesOrtho object.
     *
     * @return FEMShellAxesOrtho object fields
     */
    idempotent FEMShellAxesOrthoFields getFields();

    /**
     * Updates the fields of this FEMShellAxesOrtho object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ALFA</li>
     * <li>AxOrthoID</li>
     * <li>Group</li>
     * <li>LineID</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMShellAxesOrthoFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMEndreleases {
    /**
     * Gets the fields of this FEMEndreleases object.
     *
     * @return FEMEndreleases object fields
     */
    idempotent FEMEndreleasesFields getFields();

    /**
     * Updates the fields of this FEMEndreleases object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>EndRelease</li>
     * <li>Moment1</li>
     * <li>Moment2</li>
     * <li>Moment3</li>
     * <li>Moment4</li>
     * <li>Moment5</li>
     * <li>Moment6</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMEndreleasesFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMTrussGroup {
    /**
     * Gets the fields of this FEMTrussGroup object.
     *
     * @return FEMTrussGroup object fields
     */
    idempotent FEMTrussGroupFields getFields();

    /**
     * Updates the fields of this FEMTrussGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Displacement</li>
     * <li>GAPS</li>
     * <li>Group</li>
     * <li>IniStrain</li>
     * <li>Material</li>
     * <li>SectionArea</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMTrussGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMMaterial] object refered to by the
     * Material field.
     *
     * @return Material interface
     */
    idempotent FEMMaterial* getMaterial();
  };

  /**
   * 
   *
   */
  interface FEMInitialTemperature {
    /**
     * Gets the fields of this FEMInitialTemperature object.
     *
     * @return FEMInitialTemperature object fields
     */
    idempotent FEMInitialTemperatureFields getFields();

    /**
     * Updates the fields of this FEMInitialTemperature object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Node</li>
     * <li>Temperature</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMInitialTemperatureFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMThermoIsoMaterials {
    /**
     * Gets the fields of this FEMThermoIsoMaterials object.
     *
     * @return FEMThermoIsoMaterials object fields
     */
    idempotent FEMThermoIsoMaterialsFields getFields();

    /**
     * Updates the fields of this FEMThermoIsoMaterials object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Density</li>
     * <li>MaterialID</li>
     * <li>TREF</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMThermoIsoMaterialsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMThermoIsoData {
    /**
     * Gets the fields of this FEMThermoIsoData object.
     *
     * @return FEMThermoIsoData object fields
     */
    idempotent FEMThermoIsoDataFields getFields();

    /**
     * Updates the fields of this FEMThermoIsoData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ALPHA</li>
     * <li>E</li>
     * <li>MaterialID</li>
     * <li>NU</li>
     * <li>RecordNmb</li>
     * <li>Theta</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMThermoIsoDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMContactGroup3 {
    /**
     * Gets the fields of this FEMContactGroup3 object.
     *
     * @return FEMContactGroup3 object fields
     */
    idempotent FEMContactGroup3Fields getFields();

    /**
     * Updates the fields of this FEMContactGroup3 object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ContGroupID</li>
     * <li>Depth</li>
     * <li>Description</li>
     * <li>Forces</li>
     * <li>Friction</li>
     * <li>IniPenetration</li>
     * <li>NodeToNode</li>
     * <li>Offset</li>
     * <li>PenetrAlgorithm</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * <li>Tied</li>
     * <li>TiedOffset</li>
     * <li>Tolerance</li>
     * <li>Tractions</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMContactGroup3Fields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMNLElasticMaterials {
    /**
     * Gets the fields of this FEMNLElasticMaterials object.
     *
     * @return FEMNLElasticMaterials object fields
     */
    idempotent FEMNLElasticMaterialsFields getFields();

    /**
     * Updates the fields of this FEMNLElasticMaterials object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Dcurve</li>
     * <li>Density</li>
     * <li>MaterialID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMNLElasticMaterialsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMPlate {
    /**
     * Gets the fields of this FEMPlate object.
     *
     * @return FEMPlate object fields
     */
    idempotent FEMPlateFields getFields();

    /**
     * Updates the fields of this FEMPlate object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>Flex11</li>
     * <li>Flex12</li>
     * <li>Flex22</li>
     * <li>Group</li>
     * <li>MaterialID</li>
     * <li>Meps11</li>
     * <li>Meps12</li>
     * <li>Meps22</li>
     * <li>N1</li>
     * <li>N2</li>
     * <li>N3</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * <li>Thick</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMPlateFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N1 field.
     *
     * @return N1 interface
     */
    idempotent FEMNode* getN1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N2 field.
     *
     * @return N2 interface
     */
    idempotent FEMNode* getN2();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N3 field.
     *
     * @return N3 interface
     */
    idempotent FEMNode* getN3();
  };

  /**
   * 
   *
   */
  interface FEMIsoBeam {
    /**
     * Gets the fields of this FEMIsoBeam object.
     *
     * @return FEMIsoBeam object fields
     */
    idempotent FEMIsoBeamFields getFields();

    /**
     * Updates the fields of this FEMIsoBeam object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AUX</li>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>Epaxl</li>
     * <li>Ephoop</li>
     * <li>Group</li>
     * <li>MaterialID</li>
     * <li>N1</li>
     * <li>N2</li>
     * <li>N3</li>
     * <li>N4</li>
     * <li>NodeAmount</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * <li>SectionID</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMIsoBeamFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * AUX field.
     *
     * @return AUX interface
     */
    idempotent FEMNode* getAUX();

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N1 field.
     *
     * @return N1 interface
     */
    idempotent FEMNode* getN1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N2 field.
     *
     * @return N2 interface
     */
    idempotent FEMNode* getN2();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N3 field.
     *
     * @return N3 interface
     */
    idempotent FEMNode* getN3();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N4 field.
     *
     * @return N4 interface
     */
    idempotent FEMNode* getN4();
  };

  /**
   * 
   *
   */
  interface FEMAppliedConcentratedLoad {
    /**
     * Gets the fields of this FEMAppliedConcentratedLoad object.
     *
     * @return FEMAppliedConcentratedLoad object fields
     */
    idempotent FEMAppliedConcentratedLoadFields getFields();

    /**
     * Updates the fields of this FEMAppliedConcentratedLoad object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ArrivalTime</li>
     * <li>Description</li>
     * <li>Direction</li>
     * <li>Factor</li>
     * <li>NodeAux</li>
     * <li>NodeID</li>
     * <li>RecordNmb</li>
     * <li>TimeFunctionID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMAppliedConcentratedLoadFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * NodeAux field.
     *
     * @return NodeAux interface
     */
    idempotent FEMNode* getNodeAux();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * NodeID field.
     *
     * @return NodeID interface
     */
    idempotent FEMNode* getNodeID();
  };

  /**
   * 
   *
   */
  interface FEMTwoDSolidGroup {
    /**
     * Gets the fields of this FEMTwoDSolidGroup object.
     *
     * @return FEMTwoDSolidGroup object fields
     */
    idempotent FEMTwoDSolidGroupFields getFields();

    /**
     * Updates the fields of this FEMTwoDSolidGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AuxNode</li>
     * <li>Displacement</li>
     * <li>Group</li>
     * <li>MaterialID</li>
     * <li>Result</li>
     * <li>Subtype</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMTwoDSolidGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * AuxNode field.
     *
     * @return AuxNode interface
     */
    idempotent FEMNode* getAuxNode();

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMGroup {
    /**
     * Gets the fields of this FEMGroup object.
     *
     * @return FEMGroup object fields
     */
    idempotent FEMGroupFields getFields();

    /**
     * Updates the fields of this FEMGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>GroupType</li>
     * <li>LocalID</li>
     * <li>Structure</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Structure] object refered to by the
     * Structure field.
     *
     * @return Structure interface
     */
    idempotent Structure* getStructure();
  };

  /**
   * 
   *
   */
  interface FEMProperties {
    /**
     * Gets the fields of this FEMProperties object.
     *
     * @return FEMProperties object fields
     */
    idempotent FEMPropertiesFields getFields();

    /**
     * Updates the fields of this FEMProperties object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>PropertyID</li>
     * <li>PropertyType</li>
     * <li>Rupture</li>
     * <li>XC</li>
     * <li>XN</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMPropertiesFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMThreeDSolidGroup {
    /**
     * Gets the fields of this FEMThreeDSolidGroup object.
     *
     * @return FEMThreeDSolidGroup object fields
     */
    idempotent FEMThreeDSolidGroupFields getFields();

    /**
     * Updates the fields of this FEMThreeDSolidGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Displacement</li>
     * <li>Group</li>
     * <li>MaterialID</li>
     * <li>Result</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMThreeDSolidGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMThreeDSolid {
    /**
     * Gets the fields of this FEMThreeDSolid object.
     *
     * @return FEMThreeDSolid object fields
     */
    idempotent FEMThreeDSolidFields getFields();

    /**
     * Updates the fields of this FEMThreeDSolid object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>Group</li>
     * <li>MaterialID</li>
     * <li>Maxes</li>
     * <li>N1</li>
     * <li>N10</li>
     * <li>N11</li>
     * <li>N12</li>
     * <li>N13</li>
     * <li>N14</li>
     * <li>N15</li>
     * <li>N16</li>
     * <li>N17</li>
     * <li>N18</li>
     * <li>N19</li>
     * <li>N2</li>
     * <li>N20</li>
     * <li>N21</li>
     * <li>N22</li>
     * <li>N23</li>
     * <li>N24</li>
     * <li>N25</li>
     * <li>N26</li>
     * <li>N27</li>
     * <li>N3</li>
     * <li>N4</li>
     * <li>N5</li>
     * <li>N6</li>
     * <li>N7</li>
     * <li>N8</li>
     * <li>N9</li>
     * <li>NodeAmount</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMThreeDSolidFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N1 field.
     *
     * @return N1 interface
     */
    idempotent FEMNode* getN1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N10 field.
     *
     * @return N10 interface
     */
    idempotent FEMNode* getN10();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N11 field.
     *
     * @return N11 interface
     */
    idempotent FEMNode* getN11();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N12 field.
     *
     * @return N12 interface
     */
    idempotent FEMNode* getN12();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N13 field.
     *
     * @return N13 interface
     */
    idempotent FEMNode* getN13();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N14 field.
     *
     * @return N14 interface
     */
    idempotent FEMNode* getN14();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N15 field.
     *
     * @return N15 interface
     */
    idempotent FEMNode* getN15();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N16 field.
     *
     * @return N16 interface
     */
    idempotent FEMNode* getN16();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N17 field.
     *
     * @return N17 interface
     */
    idempotent FEMNode* getN17();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N18 field.
     *
     * @return N18 interface
     */
    idempotent FEMNode* getN18();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N19 field.
     *
     * @return N19 interface
     */
    idempotent FEMNode* getN19();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N2 field.
     *
     * @return N2 interface
     */
    idempotent FEMNode* getN2();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N20 field.
     *
     * @return N20 interface
     */
    idempotent FEMNode* getN20();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N21 field.
     *
     * @return N21 interface
     */
    idempotent FEMNode* getN21();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N22 field.
     *
     * @return N22 interface
     */
    idempotent FEMNode* getN22();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N23 field.
     *
     * @return N23 interface
     */
    idempotent FEMNode* getN23();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N24 field.
     *
     * @return N24 interface
     */
    idempotent FEMNode* getN24();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N25 field.
     *
     * @return N25 interface
     */
    idempotent FEMNode* getN25();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N26 field.
     *
     * @return N26 interface
     */
    idempotent FEMNode* getN26();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N27 field.
     *
     * @return N27 interface
     */
    idempotent FEMNode* getN27();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N3 field.
     *
     * @return N3 interface
     */
    idempotent FEMNode* getN3();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N4 field.
     *
     * @return N4 interface
     */
    idempotent FEMNode* getN4();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N5 field.
     *
     * @return N5 interface
     */
    idempotent FEMNode* getN5();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N6 field.
     *
     * @return N6 interface
     */
    idempotent FEMNode* getN6();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N7 field.
     *
     * @return N7 interface
     */
    idempotent FEMNode* getN7();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N8 field.
     *
     * @return N8 interface
     */
    idempotent FEMNode* getN8();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N9 field.
     *
     * @return N9 interface
     */
    idempotent FEMNode* getN9();
  };

  /**
   * 
   *
   */
  interface FEMSectionProp {
    /**
     * Gets the fields of this FEMSectionProp object.
     *
     * @return FEMSectionProp object fields
     */
    idempotent FEMSectionPropFields getFields();

    /**
     * Updates the fields of this FEMSectionProp object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Area</li>
     * <li>Rinertia</li>
     * <li>Sarea</li>
     * <li>SectionID</li>
     * <li>Sinertia</li>
     * <li>Tarea</li>
     * <li>Tinertia</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSectionPropFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMElasticMaterial {
    /**
     * Gets the fields of this FEMElasticMaterial object.
     *
     * @return FEMElasticMaterial object fields
     */
    idempotent FEMElasticMaterialFields getFields();

    /**
     * Updates the fields of this FEMElasticMaterial object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Alpha</li>
     * <li>Density</li>
     * <li>E</li>
     * <li>Material</li>
     * <li>NU</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMElasticMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMMaterial] object refered to by the
     * Material field.
     *
     * @return Material interface
     */
    idempotent FEMMaterial* getMaterial();
  };

  /**
   * 
   *
   */
  interface FEMPoints {
    /**
     * Gets the fields of this FEMPoints object.
     *
     * @return FEMPoints object fields
     */
    idempotent FEMPointsFields getFields();

    /**
     * Updates the fields of this FEMPoints object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>PointID</li>
     * <li>SystemID</li>
     * <li>X</li>
     * <li>Y</li>
     * <li>Z</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMPointsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMThermoOrthMaterials {
    /**
     * Gets the fields of this FEMThermoOrthMaterials object.
     *
     * @return FEMThermoOrthMaterials object fields
     */
    idempotent FEMThermoOrthMaterialsFields getFields();

    /**
     * Updates the fields of this FEMThermoOrthMaterials object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Density</li>
     * <li>MaterialID</li>
     * <li>TREF</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMThermoOrthMaterialsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMConstraints {
    /**
     * Gets the fields of this FEMConstraints object.
     *
     * @return FEMConstraints object fields
     */
    idempotent FEMConstraintsFields getFields();

    /**
     * Updates the fields of this FEMConstraints object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ConstraintID</li>
     * <li>Description</li>
     * <li>SlaveDOF</li>
     * <li>SlaveNode</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMConstraintsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMMCrigidities {
    /**
     * Gets the fields of this FEMMCrigidities object.
     *
     * @return FEMMCrigidities object fields
     */
    idempotent FEMMCrigiditiesFields getFields();

    /**
     * Updates the fields of this FEMMCrigidities object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AcurveType</li>
     * <li>Alpha</li>
     * <li>AxialCF</li>
     * <li>BcurveType</li>
     * <li>BendingCF</li>
     * <li>Beta</li>
     * <li>Density</li>
     * <li>ForceAxID</li>
     * <li>Hardening</li>
     * <li>MassArea</li>
     * <li>MassR</li>
     * <li>MassS</li>
     * <li>MassT</li>
     * <li>MomentRID</li>
     * <li>MomentSID</li>
     * <li>MomentTID</li>
     * <li>RigidityID</li>
     * <li>TcurveType</li>
     * <li>TorsionCF</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMMCrigiditiesFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMSkeySysNode {
    /**
     * Gets the fields of this FEMSkeySysNode object.
     *
     * @return FEMSkeySysNode object fields
     */
    idempotent FEMSkeySysNodeFields getFields();

    /**
     * Updates the fields of this FEMSkeySysNode object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>Node1</li>
     * <li>Node2</li>
     * <li>Node3</li>
     * <li>SkewSystemID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSkeySysNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node1 field.
     *
     * @return Node1 interface
     */
    idempotent FEMNode* getNode1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node2 field.
     *
     * @return Node2 interface
     */
    idempotent FEMNode* getNode2();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node3 field.
     *
     * @return Node3 interface
     */
    idempotent FEMNode* getNode3();
  };

  /**
   * 
   *
   */
  interface FEMIsoBeamGroup {
    /**
     * Gets the fields of this FEMIsoBeamGroup object.
     *
     * @return FEMIsoBeamGroup object fields
     */
    idempotent FEMIsoBeamGroupFields getFields();

    /**
     * Updates the fields of this FEMIsoBeamGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Displacement</li>
     * <li>Group</li>
     * <li>IniStrain</li>
     * <li>MaterialID</li>
     * <li>Result</li>
     * <li>SectionID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMIsoBeamGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMShellDOF {
    /**
     * Gets the fields of this FEMShellDOF object.
     *
     * @return FEMShellDOF object fields
     */
    idempotent FEMShellDOFFields getFields();

    /**
     * Updates the fields of this FEMShellDOF object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>DOFnumber</li>
     * <li>Node</li>
     * <li>VectorID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMShellDOFFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMCrossSections {
    /**
     * Gets the fields of this FEMCrossSections object.
     *
     * @return FEMCrossSections object fields
     */
    idempotent FEMCrossSectionsFields getFields();

    /**
     * Updates the fields of this FEMCrossSections object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>SectionID</li>
     * <li>SectionType</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMCrossSectionsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMTwistMomentData {
    /**
     * Gets the fields of this FEMTwistMomentData object.
     *
     * @return FEMTwistMomentData object fields
     */
    idempotent FEMTwistMomentDataFields getFields();

    /**
     * Updates the fields of this FEMTwistMomentData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Moment</li>
     * <li>RecordNmb</li>
     * <li>Twist</li>
     * <li>TwistMomentID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMTwistMomentDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMShell {
    /**
     * Gets the fields of this FEMShell object.
     *
     * @return FEMShell object fields
     */
    idempotent FEMShellFields getFields();

    /**
     * Updates the fields of this FEMShell object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>Group</li>
     * <li>Material</li>
     * <li>N1</li>
     * <li>N2</li>
     * <li>N3</li>
     * <li>N4</li>
     * <li>N5</li>
     * <li>N6</li>
     * <li>N7</li>
     * <li>N8</li>
     * <li>N9</li>
     * <li>NodeAmount</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * <li>T1</li>
     * <li>T2</li>
     * <li>T3</li>
     * <li>T4</li>
     * <li>T5</li>
     * <li>T6</li>
     * <li>T7</li>
     * <li>T8</li>
     * <li>T9</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMShellFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMMaterial] object refered to by the
     * Material field.
     *
     * @return Material interface
     */
    idempotent FEMMaterial* getMaterial();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N1 field.
     *
     * @return N1 interface
     */
    idempotent FEMNode* getN1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N2 field.
     *
     * @return N2 interface
     */
    idempotent FEMNode* getN2();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N3 field.
     *
     * @return N3 interface
     */
    idempotent FEMNode* getN3();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N4 field.
     *
     * @return N4 interface
     */
    idempotent FEMNode* getN4();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N5 field.
     *
     * @return N5 interface
     */
    idempotent FEMNode* getN5();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N6 field.
     *
     * @return N6 interface
     */
    idempotent FEMNode* getN6();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N7 field.
     *
     * @return N7 interface
     */
    idempotent FEMNode* getN7();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N8 field.
     *
     * @return N8 interface
     */
    idempotent FEMNode* getN8();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N9 field.
     *
     * @return N9 interface
     */
    idempotent FEMNode* getN9();
  };

  /**
   * 
   *
   */
  interface FEMNTNContact {
    /**
     * Gets the fields of this FEMNTNContact object.
     *
     * @return FEMNTNContact object fields
     */
    idempotent FEMNTNContactFields getFields();

    /**
     * Updates the fields of this FEMNTNContact object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ContactorNode</li>
     * <li>ContGroupID</li>
     * <li>ContPair</li>
     * <li>PrintRes</li>
     * <li>RecordNmb</li>
     * <li>SaveRes</li>
     * <li>TargetNode</li>
     * <li>TargetNx</li>
     * <li>TargetNy</li>
     * <li>TargetNz</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMNTNContactFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMShellLayer {
    /**
     * Gets the fields of this FEMShellLayer object.
     *
     * @return FEMShellLayer object fields
     */
    idempotent FEMShellLayerFields getFields();

    /**
     * Updates the fields of this FEMShellLayer object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Group</li>
     * <li>LayerNumber</li>
     * <li>MaterialID</li>
     * <li>PThick</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMShellLayerFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMSkewSysAngles {
    /**
     * Gets the fields of this FEMSkewSysAngles object.
     *
     * @return FEMSkewSysAngles object fields
     */
    idempotent FEMSkewSysAnglesFields getFields();

    /**
     * Updates the fields of this FEMSkewSysAngles object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>PHI</li>
     * <li>SkewSystemID</li>
     * <li>THETA</li>
     * <li>XSI</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSkewSysAnglesFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMGroundMotionRecord {
    /**
     * Gets the fields of this FEMGroundMotionRecord object.
     *
     * @return FEMGroundMotionRecord object fields
     */
    idempotent FEMGroundMotionRecordFields getFields();

    /**
     * Updates the fields of this FEMGroundMotionRecord object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Description</li>
     * <li>GMRecordID</li>
     * <li>GMRecordName</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMGroundMotionRecordFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMGeneralGroup {
    /**
     * Gets the fields of this FEMGeneralGroup object.
     *
     * @return FEMGeneralGroup object fields
     */
    idempotent FEMGeneralGroupFields getFields();

    /**
     * Updates the fields of this FEMGeneralGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Group</li>
     * <li>MatrixSet</li>
     * <li>Result</li>
     * <li>SkewSystem</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMGeneralGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMTwoDSolid {
    /**
     * Gets the fields of this FEMTwoDSolid object.
     *
     * @return FEMTwoDSolid object fields
     */
    idempotent FEMTwoDSolidFields getFields();

    /**
     * Updates the fields of this FEMTwoDSolid object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Bet</li>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>Group</li>
     * <li>MaterialID</li>
     * <li>N1</li>
     * <li>N2</li>
     * <li>N3</li>
     * <li>N4</li>
     * <li>N5</li>
     * <li>N6</li>
     * <li>N7</li>
     * <li>N8</li>
     * <li>N9</li>
     * <li>NodeAmount</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * <li>Thick</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMTwoDSolidFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N1 field.
     *
     * @return N1 interface
     */
    idempotent FEMNode* getN1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N2 field.
     *
     * @return N2 interface
     */
    idempotent FEMNode* getN2();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N3 field.
     *
     * @return N3 interface
     */
    idempotent FEMNode* getN3();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N4 field.
     *
     * @return N4 interface
     */
    idempotent FEMNode* getN4();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N5 field.
     *
     * @return N5 interface
     */
    idempotent FEMNode* getN5();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N6 field.
     *
     * @return N6 interface
     */
    idempotent FEMNode* getN6();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N7 field.
     *
     * @return N7 interface
     */
    idempotent FEMNode* getN7();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N8 field.
     *
     * @return N8 interface
     */
    idempotent FEMNode* getN8();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N9 field.
     *
     * @return N9 interface
     */
    idempotent FEMNode* getN9();
  };

  /**
   * 
   *
   */
  interface FEMAppliedTemperature {
    /**
     * Gets the fields of this FEMAppliedTemperature object.
     *
     * @return FEMAppliedTemperature object fields
     */
    idempotent FEMAppliedTemperatureFields getFields();

    /**
     * Updates the fields of this FEMAppliedTemperature object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ArrivalTime</li>
     * <li>Factor</li>
     * <li>Node</li>
     * <li>RecordNmbr</li>
     * <li>TimeFunctionID</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMAppliedTemperatureFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMMatrixSets {
    /**
     * Gets the fields of this FEMMatrixSets object.
     *
     * @return FEMMatrixSets object fields
     */
    idempotent FEMMatrixSetsFields getFields();

    /**
     * Updates the fields of this FEMMatrixSets object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Damping</li>
     * <li>Description</li>
     * <li>Mass</li>
     * <li>MatrixSetID</li>
     * <li>Stiffness</li>
     * <li>Stress</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMMatrixSetsFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMConstraintCoef {
    /**
     * Gets the fields of this FEMConstraintCoef object.
     *
     * @return FEMConstraintCoef object fields
     */
    idempotent FEMConstraintCoefFields getFields();

    /**
     * Updates the fields of this FEMConstraintCoef object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Coefficient</li>
     * <li>ConstraintID</li>
     * <li>Description</li>
     * <li>MasterDOF</li>
     * <li>MasterNode</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMConstraintCoefFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMSectionBox {
    /**
     * Gets the fields of this FEMSectionBox object.
     *
     * @return FEMSectionBox object fields
     */
    idempotent FEMSectionBoxFields getFields();

    /**
     * Updates the fields of this FEMSectionBox object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Height</li>
     * <li>SC</li>
     * <li>SectionID</li>
     * <li>SSarea</li>
     * <li>TC</li>
     * <li>Thick1</li>
     * <li>Thick2</li>
     * <li>Torfac</li>
     * <li>TSarea</li>
     * <li>Width</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSectionBoxFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMNKDisplForce {
    /**
     * Gets the fields of this FEMNKDisplForce object.
     *
     * @return FEMNKDisplForce object fields
     */
    idempotent FEMNKDisplForceFields getFields();

    /**
     * Updates the fields of this FEMNKDisplForce object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Displacement</li>
     * <li>Force</li>
     * <li>PropertyID</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMNKDisplForceFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMPlasticStrainStress {
    /**
     * Gets the fields of this FEMPlasticStrainStress object.
     *
     * @return FEMPlasticStrainStress object fields
     */
    idempotent FEMPlasticStrainStressFields getFields();

    /**
     * Updates the fields of this FEMPlasticStrainStress object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>MaterialID</li>
     * <li>RecordNumber</li>
     * <li>Strain</li>
     * <li>Stress</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMPlasticStrainStressFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMShellAxesOrthoData {
    /**
     * Gets the fields of this FEMShellAxesOrthoData object.
     *
     * @return FEMShellAxesOrthoData object fields
     */
    idempotent FEMShellAxesOrthoDataFields getFields();

    /**
     * Updates the fields of this FEMShellAxesOrthoData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AxOrthoID</li>
     * <li>ElementID</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMShellAxesOrthoDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMGeneralNode {
    /**
     * Gets the fields of this FEMGeneralNode object.
     *
     * @return FEMGeneralNode object fields
     */
    idempotent FEMGeneralNodeFields getFields();

    /**
     * Updates the fields of this FEMGeneralNode object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ElementID</li>
     * <li>Group</li>
     * <li>LocalNmb</li>
     * <li>Node</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMGeneralNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface FEMStrLines {
    /**
     * Gets the fields of this FEMStrLines object.
     *
     * @return FEMStrLines object fields
     */
    idempotent FEMStrLinesFields getFields();

    /**
     * Updates the fields of this FEMStrLines object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>LineID</li>
     * <li>P1</li>
     * <li>P2</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMStrLinesFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMContactSurface {
    /**
     * Gets the fields of this FEMContactSurface object.
     *
     * @return FEMContactSurface object fields
     */
    idempotent FEMContactSurfaceFields getFields();

    /**
     * Updates the fields of this FEMContactSurface object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>ContGroupID</li>
     * <li>ContSegment</li>
     * <li>ContSurface</li>
     * <li>N1</li>
     * <li>N2</li>
     * <li>N3</li>
     * <li>N4</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMContactSurfaceFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N1 field.
     *
     * @return N1 interface
     */
    idempotent FEMNode* getN1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N2 field.
     *
     * @return N2 interface
     */
    idempotent FEMNode* getN2();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N3 field.
     *
     * @return N3 interface
     */
    idempotent FEMNode* getN3();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N4 field.
     *
     * @return N4 interface
     */
    idempotent FEMNode* getN4();
  };

  /**
   * 
   *
   */
  interface FEMMCForceData {
    /**
     * Gets the fields of this FEMMCForceData object.
     *
     * @return FEMMCForceData object fields
     */
    idempotent FEMMCForceDataFields getFields();

    /**
     * Updates the fields of this FEMMCForceData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>CurvMomentID</li>
     * <li>Force</li>
     * <li>MomentSTID</li>
     * <li>RecordNmb</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMMCForceDataFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface FEMSpring {
    /**
     * Gets the fields of this FEMSpring object.
     *
     * @return FEMSpring object fields
     */
    idempotent FEMSpringFields getFields();

    /**
     * Updates the fields of this FEMSpring object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>AX</li>
     * <li>AY</li>
     * <li>AZ</li>
     * <li>Description</li>
     * <li>ElementID</li>
     * <li>Group</li>
     * <li>ID1</li>
     * <li>ID2</li>
     * <li>N1</li>
     * <li>N2</li>
     * <li>PropertySet</li>
     * <li>RecordNmb</li>
     * <li>Save</li>
     * <li>TBirth</li>
     * <li>TDeath</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSpringFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N1 field.
     *
     * @return N1 interface
     */
    idempotent FEMNode* getN1();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * N2 field.
     *
     * @return N2 interface
     */
    idempotent FEMNode* getN2();
  };

  /**
   * 
   *
   */
  interface FEMSpringGroup {
    /**
     * Gets the fields of this FEMSpringGroup object.
     *
     * @return FEMSpringGroup object fields
     */
    idempotent FEMSpringGroupFields getFields();

    /**
     * Updates the fields of this FEMSpringGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Bolt</li>
     * <li>Group</li>
     * <li>Nonlinear</li>
     * <li>PropertySet</li>
     * <li>Result</li>
     * <li>SkewSystem</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMSpringGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();
  };

  /**
   * 
   *
   */
  interface FEMShellGroup {
    /**
     * Gets the fields of this FEMShellGroup object.
     *
     * @return FEMShellGroup object fields
     */
    idempotent FEMShellGroupFields getFields();

    /**
     * Updates the fields of this FEMShellGroup object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Displacement</li>
     * <li>Group</li>
     * <li>Material</li>
     * <li>NLayers</li>
     * <li>Result</li>
     * <li>SectionResult</li>
     * <li>StressReference</li>
     * <li>Thickness</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(FEMShellGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [FEMGroup] object refered to by the
     * Group field.
     *
     * @return Group interface
     */
    idempotent FEMGroup* getGroup();

    /**
     * Gets the proxy to the [FEMMaterial] object refered to by the
     * Material field.
     *
     * @return Material interface
     */
    idempotent FEMMaterial* getMaterial();
  };

  /**
   * Data acquisition unit (e.g. Narada node). 
   *
   */
  interface DaqUnit {
    /**
     * Gets the fields of this DaqUnit object.
     *
     * @return DaqUnit object fields
     */
    idempotent DaqUnitFields getFields();

    /**
     * Updates the fields of this DaqUnit object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Model</li>
     * <li>Identifier</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(DaqUnitFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the IDs of the [DaqUnitChannel] child objects.
     *
     * The child objects refer to this object.
     *
     * @return list of [DaqUnitChannel] child object IDs
     */
    idempotent IdList getDaqUnitChannelChildIds();
  };

  /**
   * Data acquisition unit channel. 
   *
   */
  interface DaqUnitChannel {
    /**
     * Gets the fields of this DaqUnitChannel object.
     *
     * @return DaqUnitChannel object fields
     */
    idempotent DaqUnitChannelFields getFields();

    /**
     * Updates the fields of this DaqUnitChannel object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Unit</li>
     * <li>Number</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(DaqUnitChannelFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [DaqUnit] object refered to by the
     * Unit field.
     *
     * @return Unit interface
     */
    idempotent DaqUnit* getUnit();

    /**
     * Gets the IDs of the [DaqUnitChannelData] child objects.
     *
     * The child objects refer to this object.
     *
     * @return list of [DaqUnitChannelData] child object IDs
     */
    idempotent IdList getDaqUnitChannelDataChildIds();
  };

  /**
   * 
   *
   */
  interface Sensor {
    /**
     * Gets the fields of this Sensor object.
     *
     * @return Sensor object fields
     */
    idempotent SensorFields getFields();

    /**
     * Updates the fields of this Sensor object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Type</li>
     * <li>Model</li>
     * <li>Identifier</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(SensorFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface Transducer {
    /**
     * Gets the fields of this Transducer object.
     *
     * @return Transducer object fields
     */
    idempotent TransducerFields getFields();

    /**
     * Updates the fields of this Transducer object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Model</li>
     * <li>Identifier</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(TransducerFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface Experiment {
    /**
     * Gets the fields of this Experiment object.
     *
     * @return Experiment object fields
     */
    idempotent ExperimentFields getFields();

    /**
     * Updates the fields of this Experiment object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Frequency</li>
     * <li>Duration</li>
     * <li>BufferData</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(ExperimentFields fields,
        FieldNameList fieldNames);
  };

  /**
   * 
   *
   */
  interface Cycle {
    /**
     * Gets the fields of this Cycle object.
     *
     * @return Cycle object fields
     */
    idempotent CycleFields getFields();

    /**
     * Updates the fields of this Cycle object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Time</li>
     * <li>Experiment</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(CycleFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Experiment] object refered to by the
     * Experiment field.
     *
     * @return Experiment interface
     */
    idempotent Experiment* getExperiment();
  };

  /**
   * 
   *
   */
  interface SensorTimeData {
    /**
     * Gets the fields of this SensorTimeData object.
     *
     * @return SensorTimeData object fields
     */
    idempotent SensorTimeDataFields getFields();

    /**
     * Updates the fields of this SensorTimeData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Structure</li>
     * <li>Sensor</li>
     * <li>Channel</li>
     * <li>StartTime</li>
     * <li>Status</li>
     * <li>Node</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(SensorTimeDataFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Structure] object refered to by the
     * Structure field.
     *
     * @return Structure interface
     */
    idempotent Structure* getStructure();

    /**
     * Gets the proxy to the [Sensor] object refered to by the
     * Sensor field.
     *
     * @return Sensor interface
     */
    idempotent Sensor* getSensor();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface TransducerTimeData {
    /**
     * Gets the fields of this TransducerTimeData object.
     *
     * @return TransducerTimeData object fields
     */
    idempotent TransducerTimeDataFields getFields();

    /**
     * Updates the fields of this TransducerTimeData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Transducer</li>
     * <li>Sensor</li>
     * <li>Node</li>
     * <li>Orientation</li>
     * <li>Amplification</li>
     * <li>Channel</li>
     * <li>Filter</li>
     * <li>StartTime</li>
     * <li>Status</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(TransducerTimeDataFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Transducer] object refered to by the
     * Transducer field.
     *
     * @return Transducer interface
     */
    idempotent Transducer* getTransducer();

    /**
     * Gets the proxy to the [Sensor] object refered to by the
     * Sensor field.
     *
     * @return Sensor interface
     */
    idempotent Sensor* getSensor();

    /**
     * Gets the proxy to the [FEMNode] object refered to by the
     * Node field.
     *
     * @return Node interface
     */
    idempotent FEMNode* getNode();
  };

  /**
   * 
   *
   */
  interface ExperimentStructureData {
    /**
     * Gets the fields of this ExperimentStructureData object.
     *
     * @return ExperimentStructureData object fields
     */
    idempotent ExperimentStructureDataFields getFields();

    /**
     * Updates the fields of this ExperimentStructureData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Experiment</li>
     * <li>Structure</li>
     * <li>StartTime</li>
     * <li>Status</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(ExperimentStructureDataFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Experiment] object refered to by the
     * Experiment field.
     *
     * @return Experiment interface
     */
    idempotent Experiment* getExperiment();

    /**
     * Gets the proxy to the [Structure] object refered to by the
     * Structure field.
     *
     * @return Structure interface
     */
    idempotent Structure* getStructure();
  };

  /**
   * 
   *
   */
  interface TransducerCycleData {
    /**
     * Gets the fields of this TransducerCycleData object.
     *
     * @return TransducerCycleData object fields
     */
    idempotent TransducerCycleDataFields getFields();

    /**
     * Updates the fields of this TransducerCycleData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Transducer</li>
     * <li>Cycle</li>
     * <li>SoftwareScaling</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(TransducerCycleDataFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Transducer] object refered to by the
     * Transducer field.
     *
     * @return Transducer interface
     */
    idempotent Transducer* getTransducer();

    /**
     * Gets the proxy to the [Cycle] object refered to by the
     * Cycle field.
     *
     * @return Cycle interface
     */
    idempotent Cycle* getCycle();
  };

  /**
   * Linear mapping between two node-related quantities. 
   *
   * The axis order is:
   * <ul>
   * <li>OutputNode</li>
   * <li>InputNode</li>
   * </ul>
   */
  interface MappingMatrix {
    /**
     * Gets the fields of this MappingMatrix object.
     *
     * @return MappingMatrix object fields
     */
    idempotent MappingMatrixFields getFields();

    /**
     * Updates the fields of this MappingMatrix object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Structure</li>
     * <li>Name</li>
     * <li>Description</li>
     * <li>OutputQuantity</li>
     * <li>InputQuantity</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(MappingMatrixFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Structure] object refered to by the
     * Structure field.
     *
     * @return Structure interface
     */
    idempotent Structure* getStructure();

    /**
     * Gets the OutputNode axis.
     *
     * @return list of [FEMNode] IDs
     */
    idempotent IdList getOutputNodeAxis();

    /**
     * Sets the OutputNode axis.
     *
     * @param data list of [FEMNode] IDs
     */
    idempotent void setOutputNodeAxis(IdList data);

    /**
     * Gets the InputNode axis.
     *
     * @return list of [FEMNode] IDs
     */
    idempotent IdList getInputNodeAxis();

    /**
     * Sets the InputNode axis.
     *
     * @param data list of [FEMNode] IDs
     */
    idempotent void setInputNodeAxis(IdList data);

    /**
     * Gets the array dimensions.
     *
     * @return array dimensions
     */
    idempotent DimensionList getArraySize();

    /**
     * Creates the array.
     *
     * This method must be called before data can be set, or axes
     * defined.
     *
     * @param dims array dimensions
     */
    idempotent void createArray(DimensionList dims);

    /**
     * Gets a slice of the array data.
     *
     * @param slices  list of slices for each dimension (empty is all)
     * @return array data
     */
    idempotent Float64Array getArrayData(ArraySliceList slices);

    /**
     * Sets the array data at the given slice.
     *
     * @param slices  list of slices for each dimension (empty is all)
     * @param data vector data
     */
    idempotent void setArrayData(ArraySliceList slices,
                                 Float64List data);
  };

  /**
   * Data acquisition unit raw channel data. 
   *
   * The axis order is:
   * <ul>
   * <li>SubChannels</li>
   * </ul>
   */
  interface DaqUnitChannelData {
    /**
     * Gets the fields of this DaqUnitChannelData object.
     *
     * @return DaqUnitChannelData object fields
     */
    idempotent DaqUnitChannelDataFields getFields();

    /**
     * Updates the fields of this DaqUnitChannelData object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Channel</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(DaqUnitChannelDataFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [DaqUnitChannel] object refered to by the
     * Channel field.
     *
     * @return Channel interface
     */
    idempotent DaqUnitChannel* getChannel();

    /**
     * Gets the SubChannels axis.
     *
     * @return list of [Quantity]s
     */
    idempotent QuantityList getSubChannelsAxis();

    /**
     * Gets the SubChannels axis.
     *
     * @param data list of [Quantity]s
     */
    idempotent void setSubChannelsAxis(QuantityList data);

    /**
     * Gets the signal dimensions.
     *
     * Note that the number of dimensions equals the number of axes
     * plus one (for the time axis).  The first element of the
     * dimension list is the time dimension.
     *
     * @return signal dimensions
     */
    idempotent DimensionList getSignalSize();

    /**
     * Creates the signal array.
     *
     * Note that the array size reflects the size of each axis, not
     * including the time axis.  The signal will grow along the time
     * axis as arrays are added.
     *
     * This method must be called before signal samples can be added,
     * or axes can be set.
     *
     * @param dims signal array dimensions
     */
    idempotent void createSignal(DimensionList dims);

    /**
     * Gets the time axis information.
     *
     * Returns a data structure with information on how many samples
     * of data are available in the given time range.  The returned
     * time range will be trimmed to the range for which data is
     * available.
     *
     * If the time vector is empty, the beginning and end of the
     * available data are returned.
     *
     * @param t  vector with time stamps \[s]
     * @return time axis information structure
     */
    idempotent TimeAxisInfo getTimeAxisInfo(TimestampList t);

    /**
     * Gets the signal data.
     *
     * The returned signal data will have time stamps tStart <= t <=
     * tEnd, with no two time stamps closer than tDelta.  No interpolation
     * performed, only available samples are returned.
     *
     * @param tStart  start time
     * @param tDelta  minimum time interval \[s]
     * @param tEnd    end time
     * @param slices  list of slices for each dimension (empty is all)
     * @return signal structure
     */
    idempotent Int32Signal getSignalData(double tStart, float tStep, double tEnd,
        ArraySliceList slices);

    /**
     * Gets the signal data by index range.
     *
     * The returned signal data will contain samples
     * \[idxStart,  idxStart+idxStep, idxStart+2*idxStep, ...],
     * where idx < idxEnd.
     *
     * @param idxStart  start index (included in data)
     * @param idxStep   index step size
     * @param idxEnd    end index (not included in data)
     * @param slices    list of slices for each dimension (empty is all)
     * @return signal structure
     */
    idempotent Int32Signal getSignalDataByIndexRange(long idxStart, long idxStep, long idxEnd,
        ArraySliceList slices);

    /**
     * Appends a set of data points to the signal.
     *
     * The data vector is interpreted as an array of compatible
     * dimension.  The storage order is last dimension (axis) moves
     * fastest.
     *
     * @param t     ordered list of time stamps
     * @param data  signal array data
     */
    void appendToSignal(TimestampList t, Int32List data);
  };

  /**
   * Multi-axis accelerometer measurement data. 
   *
   * The axis order is:
   * <ul>
   * <li>Direction</li>
   * </ul>
   */
  interface AccelMeas {
    /**
     * Gets the fields of this AccelMeas object.
     *
     * @return AccelMeas object fields
     */
    idempotent AccelMeasFields getFields();

    /**
     * Updates the fields of this AccelMeas object.
     *
     * If one or more fields are specified, only the fields listed
     * will be updated, the other fields will be left unchanged.  If
     * the field list is empty ALL fields will be updated.
     * The available fields are:
     * <ul>
     * <li>Sensor</li>
     * <li>Name</li>
     * <li>Description</li>
     * </ul>
     *
     * @param fields      updated fields structure
     * @param fieldNames  a string list with field names to be updated
     */
    idempotent void setFields(AccelMeasFields fields,
        FieldNameList fieldNames);

    /**
     * Gets the proxy to the [Sensor] object refered to by the
     * Sensor field.
     *
     * @return Sensor interface
     */
    idempotent Sensor* getSensor();

    /**
     * Gets the Direction axis.
     *
     * @return list of [Quantity]s
     */
    idempotent QuantityList getDirectionAxis();

    /**
     * Gets the Direction axis.
     *
     * @param data list of [Quantity]s
     */
    idempotent void setDirectionAxis(QuantityList data);

    /**
     * Gets the signal dimensions.
     *
     * Note that the number of dimensions equals the number of axes
     * plus one (for the time axis).  The first element of the
     * dimension list is the time dimension.
     *
     * @return signal dimensions
     */
    idempotent DimensionList getSignalSize();

    /**
     * Creates the signal array.
     *
     * Note that the array size reflects the size of each axis, not
     * including the time axis.  The signal will grow along the time
     * axis as arrays are added.
     *
     * This method must be called before signal samples can be added,
     * or axes can be set.
     *
     * @param dims signal array dimensions
     */
    idempotent void createSignal(DimensionList dims);

    /**
     * Gets the time axis information.
     *
     * Returns a data structure with information on how many samples
     * of data are available in the given time range.  The returned
     * time range will be trimmed to the range for which data is
     * available.
     *
     * If the time vector is empty, the beginning and end of the
     * available data are returned.
     *
     * @param t  vector with time stamps \[s]
     * @return time axis information structure
     */
    idempotent TimeAxisInfo getTimeAxisInfo(TimestampList t);

    /**
     * Gets the signal data.
     *
     * The returned signal data will have time stamps tStart <= t <=
     * tEnd, with no two time stamps closer than tDelta.  No interpolation
     * performed, only available samples are returned.
     *
     * @param tStart  start time
     * @param tDelta  minimum time interval \[s]
     * @param tEnd    end time
     * @param slices  list of slices for each dimension (empty is all)
     * @return signal structure
     */
    idempotent Float32Signal getSignalData(double tStart, float tStep, double tEnd,
        ArraySliceList slices);

    /**
     * Gets the signal data by index range.
     *
     * The returned signal data will contain samples
     * \[idxStart,  idxStart+idxStep, idxStart+2*idxStep, ...],
     * where idx < idxEnd.
     *
     * @param idxStart  start index (included in data)
     * @param idxStep   index step size
     * @param idxEnd    end index (not included in data)
     * @param slices    list of slices for each dimension (empty is all)
     * @return signal structure
     */
    idempotent Float32Signal getSignalDataByIndexRange(long idxStart, long idxStep, long idxEnd,
        ArraySliceList slices);

    /**
     * Appends a set of data points to the signal.
     *
     * The data vector is interpreted as an array of compatible
     * dimension.  The storage order is last dimension (axis) moves
     * fastest.
     *
     * @param t     ordered list of time stamps
     * @param data  signal array data
     */
    void appendToSignal(TimestampList t, Float32List data);
  };

  interface SenStoreMngr {

    /**
     * Adds a Structure object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addStructure(StructureFields fields);

    /**
     * Adds a set of Structure objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addStructureList(StructureFieldsList fieldsList);

    /**
     * Gets the Structure object proxy.
     *
     * @param id  Structure object ID
     * @return Structure object proxy
     */
    idempotent Structure* getStructure(long id);

    /**
     * Gets a list of all Structure object IDs.
     *
     * @return list of Structure object IDs
     */
    idempotent IdList getStructureIds();

    /**
     * Gets a list of Structure object proxies.
     *
     * @param ids list of Structure object IDs
     * @return list of Structure object proxies
     */
    idempotent StructureList getStructureList(IdList ids);

    /**
     * Gets the Structure object fields.
     *
     * @param id Structure object ID
     * @return Structure object fields
     */
    idempotent StructureFields getStructureFields(long id);

    /**
     * Gets a list of Structure object fields.
     *
     * @param ids list of Structure object IDs
     * @return list of Structure object fields
     */
    idempotent StructureFieldsList getStructureFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all Structure objects matching the given
     * reference fields.
     *
     * @param fields Structure object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching Structure objects
     */
    idempotent IdList findEqualStructure(StructureFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named Structure object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields Structure object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setStructureFields(StructureFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMDof object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMDof(FEMDofFields fields);

    /**
     * Adds a set of FEMDof objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMDofList(FEMDofFieldsList fieldsList);

    /**
     * Gets the FEMDof object proxy.
     *
     * @param id  FEMDof object ID
     * @return FEMDof object proxy
     */
    idempotent FEMDof* getFEMDof(long id);

    /**
     * Gets a list of all FEMDof object IDs.
     *
     * @return list of FEMDof object IDs
     */
    idempotent IdList getFEMDofIds();

    /**
     * Gets a list of FEMDof object proxies.
     *
     * @param ids list of FEMDof object IDs
     * @return list of FEMDof object proxies
     */
    idempotent FEMDofList getFEMDofList(IdList ids);

    /**
     * Gets the FEMDof object fields.
     *
     * @param id FEMDof object ID
     * @return FEMDof object fields
     */
    idempotent FEMDofFields getFEMDofFields(long id);

    /**
     * Gets a list of FEMDof object fields.
     *
     * @param ids list of FEMDof object IDs
     * @return list of FEMDof object fields
     */
    idempotent FEMDofFieldsList getFEMDofFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMDof objects matching the given
     * reference fields.
     *
     * @param fields FEMDof object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMDof objects
     */
    idempotent IdList findEqualFEMDof(FEMDofFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMDof object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMDof object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMDofFields(FEMDofFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMNodalMass object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMNodalMass(FEMNodalMassFields fields);

    /**
     * Adds a set of FEMNodalMass objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMNodalMassList(FEMNodalMassFieldsList fieldsList);

    /**
     * Gets the FEMNodalMass object proxy.
     *
     * @param id  FEMNodalMass object ID
     * @return FEMNodalMass object proxy
     */
    idempotent FEMNodalMass* getFEMNodalMass(long id);

    /**
     * Gets a list of all FEMNodalMass object IDs.
     *
     * @return list of FEMNodalMass object IDs
     */
    idempotent IdList getFEMNodalMassIds();

    /**
     * Gets a list of FEMNodalMass object proxies.
     *
     * @param ids list of FEMNodalMass object IDs
     * @return list of FEMNodalMass object proxies
     */
    idempotent FEMNodalMassList getFEMNodalMassList(IdList ids);

    /**
     * Gets the FEMNodalMass object fields.
     *
     * @param id FEMNodalMass object ID
     * @return FEMNodalMass object fields
     */
    idempotent FEMNodalMassFields getFEMNodalMassFields(long id);

    /**
     * Gets a list of FEMNodalMass object fields.
     *
     * @param ids list of FEMNodalMass object IDs
     * @return list of FEMNodalMass object fields
     */
    idempotent FEMNodalMassFieldsList getFEMNodalMassFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMNodalMass objects matching the given
     * reference fields.
     *
     * @param fields FEMNodalMass object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMNodalMass objects
     */
    idempotent IdList findEqualFEMNodalMass(FEMNodalMassFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMNodalMass object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMNodalMass object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMNodalMassFields(FEMNodalMassFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMNLElasticStrainStress object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMNLElasticStrainStress(FEMNLElasticStrainStressFields fields);

    /**
     * Adds a set of FEMNLElasticStrainStress objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMNLElasticStrainStressList(FEMNLElasticStrainStressFieldsList fieldsList);

    /**
     * Gets the FEMNLElasticStrainStress object proxy.
     *
     * @param id  FEMNLElasticStrainStress object ID
     * @return FEMNLElasticStrainStress object proxy
     */
    idempotent FEMNLElasticStrainStress* getFEMNLElasticStrainStress(long id);

    /**
     * Gets a list of all FEMNLElasticStrainStress object IDs.
     *
     * @return list of FEMNLElasticStrainStress object IDs
     */
    idempotent IdList getFEMNLElasticStrainStressIds();

    /**
     * Gets a list of FEMNLElasticStrainStress object proxies.
     *
     * @param ids list of FEMNLElasticStrainStress object IDs
     * @return list of FEMNLElasticStrainStress object proxies
     */
    idempotent FEMNLElasticStrainStressList getFEMNLElasticStrainStressList(IdList ids);

    /**
     * Gets the FEMNLElasticStrainStress object fields.
     *
     * @param id FEMNLElasticStrainStress object ID
     * @return FEMNLElasticStrainStress object fields
     */
    idempotent FEMNLElasticStrainStressFields getFEMNLElasticStrainStressFields(long id);

    /**
     * Gets a list of FEMNLElasticStrainStress object fields.
     *
     * @param ids list of FEMNLElasticStrainStress object IDs
     * @return list of FEMNLElasticStrainStress object fields
     */
    idempotent FEMNLElasticStrainStressFieldsList getFEMNLElasticStrainStressFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMNLElasticStrainStress objects matching the given
     * reference fields.
     *
     * @param fields FEMNLElasticStrainStress object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMNLElasticStrainStress objects
     */
    idempotent IdList findEqualFEMNLElasticStrainStress(FEMNLElasticStrainStressFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMNLElasticStrainStress object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMNLElasticStrainStress object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMNLElasticStrainStressFields(FEMNLElasticStrainStressFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMBoundary object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMBoundary(FEMBoundaryFields fields);

    /**
     * Adds a set of FEMBoundary objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMBoundaryList(FEMBoundaryFieldsList fieldsList);

    /**
     * Gets the FEMBoundary object proxy.
     *
     * @param id  FEMBoundary object ID
     * @return FEMBoundary object proxy
     */
    idempotent FEMBoundary* getFEMBoundary(long id);

    /**
     * Gets a list of all FEMBoundary object IDs.
     *
     * @return list of FEMBoundary object IDs
     */
    idempotent IdList getFEMBoundaryIds();

    /**
     * Gets a list of FEMBoundary object proxies.
     *
     * @param ids list of FEMBoundary object IDs
     * @return list of FEMBoundary object proxies
     */
    idempotent FEMBoundaryList getFEMBoundaryList(IdList ids);

    /**
     * Gets the FEMBoundary object fields.
     *
     * @param id FEMBoundary object ID
     * @return FEMBoundary object fields
     */
    idempotent FEMBoundaryFields getFEMBoundaryFields(long id);

    /**
     * Gets a list of FEMBoundary object fields.
     *
     * @param ids list of FEMBoundary object IDs
     * @return list of FEMBoundary object fields
     */
    idempotent FEMBoundaryFieldsList getFEMBoundaryFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMBoundary objects matching the given
     * reference fields.
     *
     * @param fields FEMBoundary object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMBoundary objects
     */
    idempotent IdList findEqualFEMBoundary(FEMBoundaryFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMBoundary object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMBoundary object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMBoundaryFields(FEMBoundaryFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSectionPipe object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSectionPipe(FEMSectionPipeFields fields);

    /**
     * Adds a set of FEMSectionPipe objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSectionPipeList(FEMSectionPipeFieldsList fieldsList);

    /**
     * Gets the FEMSectionPipe object proxy.
     *
     * @param id  FEMSectionPipe object ID
     * @return FEMSectionPipe object proxy
     */
    idempotent FEMSectionPipe* getFEMSectionPipe(long id);

    /**
     * Gets a list of all FEMSectionPipe object IDs.
     *
     * @return list of FEMSectionPipe object IDs
     */
    idempotent IdList getFEMSectionPipeIds();

    /**
     * Gets a list of FEMSectionPipe object proxies.
     *
     * @param ids list of FEMSectionPipe object IDs
     * @return list of FEMSectionPipe object proxies
     */
    idempotent FEMSectionPipeList getFEMSectionPipeList(IdList ids);

    /**
     * Gets the FEMSectionPipe object fields.
     *
     * @param id FEMSectionPipe object ID
     * @return FEMSectionPipe object fields
     */
    idempotent FEMSectionPipeFields getFEMSectionPipeFields(long id);

    /**
     * Gets a list of FEMSectionPipe object fields.
     *
     * @param ids list of FEMSectionPipe object IDs
     * @return list of FEMSectionPipe object fields
     */
    idempotent FEMSectionPipeFieldsList getFEMSectionPipeFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSectionPipe objects matching the given
     * reference fields.
     *
     * @param fields FEMSectionPipe object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSectionPipe objects
     */
    idempotent IdList findEqualFEMSectionPipe(FEMSectionPipeFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSectionPipe object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSectionPipe object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSectionPipeFields(FEMSectionPipeFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMCoordSystem object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMCoordSystem(FEMCoordSystemFields fields);

    /**
     * Adds a set of FEMCoordSystem objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMCoordSystemList(FEMCoordSystemFieldsList fieldsList);

    /**
     * Gets the FEMCoordSystem object proxy.
     *
     * @param id  FEMCoordSystem object ID
     * @return FEMCoordSystem object proxy
     */
    idempotent FEMCoordSystem* getFEMCoordSystem(long id);

    /**
     * Gets a list of all FEMCoordSystem object IDs.
     *
     * @return list of FEMCoordSystem object IDs
     */
    idempotent IdList getFEMCoordSystemIds();

    /**
     * Gets a list of FEMCoordSystem object proxies.
     *
     * @param ids list of FEMCoordSystem object IDs
     * @return list of FEMCoordSystem object proxies
     */
    idempotent FEMCoordSystemList getFEMCoordSystemList(IdList ids);

    /**
     * Gets the FEMCoordSystem object fields.
     *
     * @param id FEMCoordSystem object ID
     * @return FEMCoordSystem object fields
     */
    idempotent FEMCoordSystemFields getFEMCoordSystemFields(long id);

    /**
     * Gets a list of FEMCoordSystem object fields.
     *
     * @param ids list of FEMCoordSystem object IDs
     * @return list of FEMCoordSystem object fields
     */
    idempotent FEMCoordSystemFieldsList getFEMCoordSystemFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMCoordSystem objects matching the given
     * reference fields.
     *
     * @param fields FEMCoordSystem object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMCoordSystem objects
     */
    idempotent IdList findEqualFEMCoordSystem(FEMCoordSystemFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMCoordSystem object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMCoordSystem object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMCoordSystemFields(FEMCoordSystemFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMNode object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMNode(FEMNodeFields fields);

    /**
     * Adds a set of FEMNode objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMNodeList(FEMNodeFieldsList fieldsList);

    /**
     * Gets the FEMNode object proxy.
     *
     * @param id  FEMNode object ID
     * @return FEMNode object proxy
     */
    idempotent FEMNode* getFEMNode(long id);

    /**
     * Gets a list of all FEMNode object IDs.
     *
     * @return list of FEMNode object IDs
     */
    idempotent IdList getFEMNodeIds();

    /**
     * Gets a list of FEMNode object proxies.
     *
     * @param ids list of FEMNode object IDs
     * @return list of FEMNode object proxies
     */
    idempotent FEMNodeList getFEMNodeList(IdList ids);

    /**
     * Gets the FEMNode object fields.
     *
     * @param id FEMNode object ID
     * @return FEMNode object fields
     */
    idempotent FEMNodeFields getFEMNodeFields(long id);

    /**
     * Gets a list of FEMNode object fields.
     *
     * @param ids list of FEMNode object IDs
     * @return list of FEMNode object fields
     */
    idempotent FEMNodeFieldsList getFEMNodeFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMNode objects matching the given
     * reference fields.
     *
     * @param fields FEMNode object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMNode objects
     */
    idempotent IdList findEqualFEMNode(FEMNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMNode object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMNode object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMNodeFields(FEMNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMTruss object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMTruss(FEMTrussFields fields);

    /**
     * Adds a set of FEMTruss objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMTrussList(FEMTrussFieldsList fieldsList);

    /**
     * Gets the FEMTruss object proxy.
     *
     * @param id  FEMTruss object ID
     * @return FEMTruss object proxy
     */
    idempotent FEMTruss* getFEMTruss(long id);

    /**
     * Gets a list of all FEMTruss object IDs.
     *
     * @return list of FEMTruss object IDs
     */
    idempotent IdList getFEMTrussIds();

    /**
     * Gets a list of FEMTruss object proxies.
     *
     * @param ids list of FEMTruss object IDs
     * @return list of FEMTruss object proxies
     */
    idempotent FEMTrussList getFEMTrussList(IdList ids);

    /**
     * Gets the FEMTruss object fields.
     *
     * @param id FEMTruss object ID
     * @return FEMTruss object fields
     */
    idempotent FEMTrussFields getFEMTrussFields(long id);

    /**
     * Gets a list of FEMTruss object fields.
     *
     * @param ids list of FEMTruss object IDs
     * @return list of FEMTruss object fields
     */
    idempotent FEMTrussFieldsList getFEMTrussFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMTruss objects matching the given
     * reference fields.
     *
     * @param fields FEMTruss object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMTruss objects
     */
    idempotent IdList findEqualFEMTruss(FEMTrussFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMTruss object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMTruss object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMTrussFields(FEMTrussFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMTimeFunctionData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMTimeFunctionData(FEMTimeFunctionDataFields fields);

    /**
     * Adds a set of FEMTimeFunctionData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMTimeFunctionDataList(FEMTimeFunctionDataFieldsList fieldsList);

    /**
     * Gets the FEMTimeFunctionData object proxy.
     *
     * @param id  FEMTimeFunctionData object ID
     * @return FEMTimeFunctionData object proxy
     */
    idempotent FEMTimeFunctionData* getFEMTimeFunctionData(long id);

    /**
     * Gets a list of all FEMTimeFunctionData object IDs.
     *
     * @return list of FEMTimeFunctionData object IDs
     */
    idempotent IdList getFEMTimeFunctionDataIds();

    /**
     * Gets a list of FEMTimeFunctionData object proxies.
     *
     * @param ids list of FEMTimeFunctionData object IDs
     * @return list of FEMTimeFunctionData object proxies
     */
    idempotent FEMTimeFunctionDataList getFEMTimeFunctionDataList(IdList ids);

    /**
     * Gets the FEMTimeFunctionData object fields.
     *
     * @param id FEMTimeFunctionData object ID
     * @return FEMTimeFunctionData object fields
     */
    idempotent FEMTimeFunctionDataFields getFEMTimeFunctionDataFields(long id);

    /**
     * Gets a list of FEMTimeFunctionData object fields.
     *
     * @param ids list of FEMTimeFunctionData object IDs
     * @return list of FEMTimeFunctionData object fields
     */
    idempotent FEMTimeFunctionDataFieldsList getFEMTimeFunctionDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMTimeFunctionData objects matching the given
     * reference fields.
     *
     * @param fields FEMTimeFunctionData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMTimeFunctionData objects
     */
    idempotent IdList findEqualFEMTimeFunctionData(FEMTimeFunctionDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMTimeFunctionData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMTimeFunctionData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMTimeFunctionDataFields(FEMTimeFunctionDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMPlasticMlMaterials object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMPlasticMlMaterials(FEMPlasticMlMaterialsFields fields);

    /**
     * Adds a set of FEMPlasticMlMaterials objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMPlasticMlMaterialsList(FEMPlasticMlMaterialsFieldsList fieldsList);

    /**
     * Gets the FEMPlasticMlMaterials object proxy.
     *
     * @param id  FEMPlasticMlMaterials object ID
     * @return FEMPlasticMlMaterials object proxy
     */
    idempotent FEMPlasticMlMaterials* getFEMPlasticMlMaterials(long id);

    /**
     * Gets a list of all FEMPlasticMlMaterials object IDs.
     *
     * @return list of FEMPlasticMlMaterials object IDs
     */
    idempotent IdList getFEMPlasticMlMaterialsIds();

    /**
     * Gets a list of FEMPlasticMlMaterials object proxies.
     *
     * @param ids list of FEMPlasticMlMaterials object IDs
     * @return list of FEMPlasticMlMaterials object proxies
     */
    idempotent FEMPlasticMlMaterialsList getFEMPlasticMlMaterialsList(IdList ids);

    /**
     * Gets the FEMPlasticMlMaterials object fields.
     *
     * @param id FEMPlasticMlMaterials object ID
     * @return FEMPlasticMlMaterials object fields
     */
    idempotent FEMPlasticMlMaterialsFields getFEMPlasticMlMaterialsFields(long id);

    /**
     * Gets a list of FEMPlasticMlMaterials object fields.
     *
     * @param ids list of FEMPlasticMlMaterials object IDs
     * @return list of FEMPlasticMlMaterials object fields
     */
    idempotent FEMPlasticMlMaterialsFieldsList getFEMPlasticMlMaterialsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMPlasticMlMaterials objects matching the given
     * reference fields.
     *
     * @param fields FEMPlasticMlMaterials object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMPlasticMlMaterials objects
     */
    idempotent IdList findEqualFEMPlasticMlMaterials(FEMPlasticMlMaterialsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMPlasticMlMaterials object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMPlasticMlMaterials object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMPlasticMlMaterialsFields(FEMPlasticMlMaterialsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMPlateGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMPlateGroup(FEMPlateGroupFields fields);

    /**
     * Adds a set of FEMPlateGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMPlateGroupList(FEMPlateGroupFieldsList fieldsList);

    /**
     * Gets the FEMPlateGroup object proxy.
     *
     * @param id  FEMPlateGroup object ID
     * @return FEMPlateGroup object proxy
     */
    idempotent FEMPlateGroup* getFEMPlateGroup(long id);

    /**
     * Gets a list of all FEMPlateGroup object IDs.
     *
     * @return list of FEMPlateGroup object IDs
     */
    idempotent IdList getFEMPlateGroupIds();

    /**
     * Gets a list of FEMPlateGroup object proxies.
     *
     * @param ids list of FEMPlateGroup object IDs
     * @return list of FEMPlateGroup object proxies
     */
    idempotent FEMPlateGroupList getFEMPlateGroupList(IdList ids);

    /**
     * Gets the FEMPlateGroup object fields.
     *
     * @param id FEMPlateGroup object ID
     * @return FEMPlateGroup object fields
     */
    idempotent FEMPlateGroupFields getFEMPlateGroupFields(long id);

    /**
     * Gets a list of FEMPlateGroup object fields.
     *
     * @param ids list of FEMPlateGroup object IDs
     * @return list of FEMPlateGroup object fields
     */
    idempotent FEMPlateGroupFieldsList getFEMPlateGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMPlateGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMPlateGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMPlateGroup objects
     */
    idempotent IdList findEqualFEMPlateGroup(FEMPlateGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMPlateGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMPlateGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMPlateGroupFields(FEMPlateGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMBeam object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMBeam(FEMBeamFields fields);

    /**
     * Adds a set of FEMBeam objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMBeamList(FEMBeamFieldsList fieldsList);

    /**
     * Gets the FEMBeam object proxy.
     *
     * @param id  FEMBeam object ID
     * @return FEMBeam object proxy
     */
    idempotent FEMBeam* getFEMBeam(long id);

    /**
     * Gets a list of all FEMBeam object IDs.
     *
     * @return list of FEMBeam object IDs
     */
    idempotent IdList getFEMBeamIds();

    /**
     * Gets a list of FEMBeam object proxies.
     *
     * @param ids list of FEMBeam object IDs
     * @return list of FEMBeam object proxies
     */
    idempotent FEMBeamList getFEMBeamList(IdList ids);

    /**
     * Gets the FEMBeam object fields.
     *
     * @param id FEMBeam object ID
     * @return FEMBeam object fields
     */
    idempotent FEMBeamFields getFEMBeamFields(long id);

    /**
     * Gets a list of FEMBeam object fields.
     *
     * @param ids list of FEMBeam object IDs
     * @return list of FEMBeam object fields
     */
    idempotent FEMBeamFieldsList getFEMBeamFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMBeam objects matching the given
     * reference fields.
     *
     * @param fields FEMBeam object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMBeam objects
     */
    idempotent IdList findEqualFEMBeam(FEMBeamFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMBeam object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMBeam object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMBeamFields(FEMBeamFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMCurvMomentData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMCurvMomentData(FEMCurvMomentDataFields fields);

    /**
     * Adds a set of FEMCurvMomentData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMCurvMomentDataList(FEMCurvMomentDataFieldsList fieldsList);

    /**
     * Gets the FEMCurvMomentData object proxy.
     *
     * @param id  FEMCurvMomentData object ID
     * @return FEMCurvMomentData object proxy
     */
    idempotent FEMCurvMomentData* getFEMCurvMomentData(long id);

    /**
     * Gets a list of all FEMCurvMomentData object IDs.
     *
     * @return list of FEMCurvMomentData object IDs
     */
    idempotent IdList getFEMCurvMomentDataIds();

    /**
     * Gets a list of FEMCurvMomentData object proxies.
     *
     * @param ids list of FEMCurvMomentData object IDs
     * @return list of FEMCurvMomentData object proxies
     */
    idempotent FEMCurvMomentDataList getFEMCurvMomentDataList(IdList ids);

    /**
     * Gets the FEMCurvMomentData object fields.
     *
     * @param id FEMCurvMomentData object ID
     * @return FEMCurvMomentData object fields
     */
    idempotent FEMCurvMomentDataFields getFEMCurvMomentDataFields(long id);

    /**
     * Gets a list of FEMCurvMomentData object fields.
     *
     * @param ids list of FEMCurvMomentData object IDs
     * @return list of FEMCurvMomentData object fields
     */
    idempotent FEMCurvMomentDataFieldsList getFEMCurvMomentDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMCurvMomentData objects matching the given
     * reference fields.
     *
     * @param fields FEMCurvMomentData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMCurvMomentData objects
     */
    idempotent IdList findEqualFEMCurvMomentData(FEMCurvMomentDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMCurvMomentData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMCurvMomentData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMCurvMomentDataFields(FEMCurvMomentDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMPropertysets object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMPropertysets(FEMPropertysetsFields fields);

    /**
     * Adds a set of FEMPropertysets objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMPropertysetsList(FEMPropertysetsFieldsList fieldsList);

    /**
     * Gets the FEMPropertysets object proxy.
     *
     * @param id  FEMPropertysets object ID
     * @return FEMPropertysets object proxy
     */
    idempotent FEMPropertysets* getFEMPropertysets(long id);

    /**
     * Gets a list of all FEMPropertysets object IDs.
     *
     * @return list of FEMPropertysets object IDs
     */
    idempotent IdList getFEMPropertysetsIds();

    /**
     * Gets a list of FEMPropertysets object proxies.
     *
     * @param ids list of FEMPropertysets object IDs
     * @return list of FEMPropertysets object proxies
     */
    idempotent FEMPropertysetsList getFEMPropertysetsList(IdList ids);

    /**
     * Gets the FEMPropertysets object fields.
     *
     * @param id FEMPropertysets object ID
     * @return FEMPropertysets object fields
     */
    idempotent FEMPropertysetsFields getFEMPropertysetsFields(long id);

    /**
     * Gets a list of FEMPropertysets object fields.
     *
     * @param ids list of FEMPropertysets object IDs
     * @return list of FEMPropertysets object fields
     */
    idempotent FEMPropertysetsFieldsList getFEMPropertysetsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMPropertysets objects matching the given
     * reference fields.
     *
     * @param fields FEMPropertysets object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMPropertysets objects
     */
    idempotent IdList findEqualFEMPropertysets(FEMPropertysetsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMPropertysets object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMPropertysets object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMPropertysetsFields(FEMPropertysetsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMOrthotropicMaterial object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMOrthotropicMaterial(FEMOrthotropicMaterialFields fields);

    /**
     * Adds a set of FEMOrthotropicMaterial objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMOrthotropicMaterialList(FEMOrthotropicMaterialFieldsList fieldsList);

    /**
     * Gets the FEMOrthotropicMaterial object proxy.
     *
     * @param id  FEMOrthotropicMaterial object ID
     * @return FEMOrthotropicMaterial object proxy
     */
    idempotent FEMOrthotropicMaterial* getFEMOrthotropicMaterial(long id);

    /**
     * Gets a list of all FEMOrthotropicMaterial object IDs.
     *
     * @return list of FEMOrthotropicMaterial object IDs
     */
    idempotent IdList getFEMOrthotropicMaterialIds();

    /**
     * Gets a list of FEMOrthotropicMaterial object proxies.
     *
     * @param ids list of FEMOrthotropicMaterial object IDs
     * @return list of FEMOrthotropicMaterial object proxies
     */
    idempotent FEMOrthotropicMaterialList getFEMOrthotropicMaterialList(IdList ids);

    /**
     * Gets the FEMOrthotropicMaterial object fields.
     *
     * @param id FEMOrthotropicMaterial object ID
     * @return FEMOrthotropicMaterial object fields
     */
    idempotent FEMOrthotropicMaterialFields getFEMOrthotropicMaterialFields(long id);

    /**
     * Gets a list of FEMOrthotropicMaterial object fields.
     *
     * @param ids list of FEMOrthotropicMaterial object IDs
     * @return list of FEMOrthotropicMaterial object fields
     */
    idempotent FEMOrthotropicMaterialFieldsList getFEMOrthotropicMaterialFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMOrthotropicMaterial objects matching the given
     * reference fields.
     *
     * @param fields FEMOrthotropicMaterial object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMOrthotropicMaterial objects
     */
    idempotent IdList findEqualFEMOrthotropicMaterial(FEMOrthotropicMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMOrthotropicMaterial object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMOrthotropicMaterial object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMOrthotropicMaterialFields(FEMOrthotropicMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMAppliedLoads object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMAppliedLoads(FEMAppliedLoadsFields fields);

    /**
     * Adds a set of FEMAppliedLoads objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMAppliedLoadsList(FEMAppliedLoadsFieldsList fieldsList);

    /**
     * Gets the FEMAppliedLoads object proxy.
     *
     * @param id  FEMAppliedLoads object ID
     * @return FEMAppliedLoads object proxy
     */
    idempotent FEMAppliedLoads* getFEMAppliedLoads(long id);

    /**
     * Gets a list of all FEMAppliedLoads object IDs.
     *
     * @return list of FEMAppliedLoads object IDs
     */
    idempotent IdList getFEMAppliedLoadsIds();

    /**
     * Gets a list of FEMAppliedLoads object proxies.
     *
     * @param ids list of FEMAppliedLoads object IDs
     * @return list of FEMAppliedLoads object proxies
     */
    idempotent FEMAppliedLoadsList getFEMAppliedLoadsList(IdList ids);

    /**
     * Gets the FEMAppliedLoads object fields.
     *
     * @param id FEMAppliedLoads object ID
     * @return FEMAppliedLoads object fields
     */
    idempotent FEMAppliedLoadsFields getFEMAppliedLoadsFields(long id);

    /**
     * Gets a list of FEMAppliedLoads object fields.
     *
     * @param ids list of FEMAppliedLoads object IDs
     * @return list of FEMAppliedLoads object fields
     */
    idempotent FEMAppliedLoadsFieldsList getFEMAppliedLoadsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMAppliedLoads objects matching the given
     * reference fields.
     *
     * @param fields FEMAppliedLoads object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMAppliedLoads objects
     */
    idempotent IdList findEqualFEMAppliedLoads(FEMAppliedLoadsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMAppliedLoads object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMAppliedLoads object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMAppliedLoadsFields(FEMAppliedLoadsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMThermoOrthData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMThermoOrthData(FEMThermoOrthDataFields fields);

    /**
     * Adds a set of FEMThermoOrthData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMThermoOrthDataList(FEMThermoOrthDataFieldsList fieldsList);

    /**
     * Gets the FEMThermoOrthData object proxy.
     *
     * @param id  FEMThermoOrthData object ID
     * @return FEMThermoOrthData object proxy
     */
    idempotent FEMThermoOrthData* getFEMThermoOrthData(long id);

    /**
     * Gets a list of all FEMThermoOrthData object IDs.
     *
     * @return list of FEMThermoOrthData object IDs
     */
    idempotent IdList getFEMThermoOrthDataIds();

    /**
     * Gets a list of FEMThermoOrthData object proxies.
     *
     * @param ids list of FEMThermoOrthData object IDs
     * @return list of FEMThermoOrthData object proxies
     */
    idempotent FEMThermoOrthDataList getFEMThermoOrthDataList(IdList ids);

    /**
     * Gets the FEMThermoOrthData object fields.
     *
     * @param id FEMThermoOrthData object ID
     * @return FEMThermoOrthData object fields
     */
    idempotent FEMThermoOrthDataFields getFEMThermoOrthDataFields(long id);

    /**
     * Gets a list of FEMThermoOrthData object fields.
     *
     * @param ids list of FEMThermoOrthData object IDs
     * @return list of FEMThermoOrthData object fields
     */
    idempotent FEMThermoOrthDataFieldsList getFEMThermoOrthDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMThermoOrthData objects matching the given
     * reference fields.
     *
     * @param fields FEMThermoOrthData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMThermoOrthData objects
     */
    idempotent IdList findEqualFEMThermoOrthData(FEMThermoOrthDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMThermoOrthData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMThermoOrthData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMThermoOrthDataFields(FEMThermoOrthDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMContactPairs object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMContactPairs(FEMContactPairsFields fields);

    /**
     * Adds a set of FEMContactPairs objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMContactPairsList(FEMContactPairsFieldsList fieldsList);

    /**
     * Gets the FEMContactPairs object proxy.
     *
     * @param id  FEMContactPairs object ID
     * @return FEMContactPairs object proxy
     */
    idempotent FEMContactPairs* getFEMContactPairs(long id);

    /**
     * Gets a list of all FEMContactPairs object IDs.
     *
     * @return list of FEMContactPairs object IDs
     */
    idempotent IdList getFEMContactPairsIds();

    /**
     * Gets a list of FEMContactPairs object proxies.
     *
     * @param ids list of FEMContactPairs object IDs
     * @return list of FEMContactPairs object proxies
     */
    idempotent FEMContactPairsList getFEMContactPairsList(IdList ids);

    /**
     * Gets the FEMContactPairs object fields.
     *
     * @param id FEMContactPairs object ID
     * @return FEMContactPairs object fields
     */
    idempotent FEMContactPairsFields getFEMContactPairsFields(long id);

    /**
     * Gets a list of FEMContactPairs object fields.
     *
     * @param ids list of FEMContactPairs object IDs
     * @return list of FEMContactPairs object fields
     */
    idempotent FEMContactPairsFieldsList getFEMContactPairsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMContactPairs objects matching the given
     * reference fields.
     *
     * @param fields FEMContactPairs object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMContactPairs objects
     */
    idempotent IdList findEqualFEMContactPairs(FEMContactPairsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMContactPairs object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMContactPairs object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMContactPairsFields(FEMContactPairsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMGeneral object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMGeneral(FEMGeneralFields fields);

    /**
     * Adds a set of FEMGeneral objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMGeneralList(FEMGeneralFieldsList fieldsList);

    /**
     * Gets the FEMGeneral object proxy.
     *
     * @param id  FEMGeneral object ID
     * @return FEMGeneral object proxy
     */
    idempotent FEMGeneral* getFEMGeneral(long id);

    /**
     * Gets a list of all FEMGeneral object IDs.
     *
     * @return list of FEMGeneral object IDs
     */
    idempotent IdList getFEMGeneralIds();

    /**
     * Gets a list of FEMGeneral object proxies.
     *
     * @param ids list of FEMGeneral object IDs
     * @return list of FEMGeneral object proxies
     */
    idempotent FEMGeneralList getFEMGeneralList(IdList ids);

    /**
     * Gets the FEMGeneral object fields.
     *
     * @param id FEMGeneral object ID
     * @return FEMGeneral object fields
     */
    idempotent FEMGeneralFields getFEMGeneralFields(long id);

    /**
     * Gets a list of FEMGeneral object fields.
     *
     * @param ids list of FEMGeneral object IDs
     * @return list of FEMGeneral object fields
     */
    idempotent FEMGeneralFieldsList getFEMGeneralFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMGeneral objects matching the given
     * reference fields.
     *
     * @param fields FEMGeneral object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMGeneral objects
     */
    idempotent IdList findEqualFEMGeneral(FEMGeneralFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMGeneral object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMGeneral object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMGeneralFields(FEMGeneralFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMBeamNode object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMBeamNode(FEMBeamNodeFields fields);

    /**
     * Adds a set of FEMBeamNode objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMBeamNodeList(FEMBeamNodeFieldsList fieldsList);

    /**
     * Gets the FEMBeamNode object proxy.
     *
     * @param id  FEMBeamNode object ID
     * @return FEMBeamNode object proxy
     */
    idempotent FEMBeamNode* getFEMBeamNode(long id);

    /**
     * Gets a list of all FEMBeamNode object IDs.
     *
     * @return list of FEMBeamNode object IDs
     */
    idempotent IdList getFEMBeamNodeIds();

    /**
     * Gets a list of FEMBeamNode object proxies.
     *
     * @param ids list of FEMBeamNode object IDs
     * @return list of FEMBeamNode object proxies
     */
    idempotent FEMBeamNodeList getFEMBeamNodeList(IdList ids);

    /**
     * Gets the FEMBeamNode object fields.
     *
     * @param id FEMBeamNode object ID
     * @return FEMBeamNode object fields
     */
    idempotent FEMBeamNodeFields getFEMBeamNodeFields(long id);

    /**
     * Gets a list of FEMBeamNode object fields.
     *
     * @param ids list of FEMBeamNode object IDs
     * @return list of FEMBeamNode object fields
     */
    idempotent FEMBeamNodeFieldsList getFEMBeamNodeFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMBeamNode objects matching the given
     * reference fields.
     *
     * @param fields FEMBeamNode object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMBeamNode objects
     */
    idempotent IdList findEqualFEMBeamNode(FEMBeamNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMBeamNode object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMBeamNode object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMBeamNodeFields(FEMBeamNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSectionRect object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSectionRect(FEMSectionRectFields fields);

    /**
     * Adds a set of FEMSectionRect objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSectionRectList(FEMSectionRectFieldsList fieldsList);

    /**
     * Gets the FEMSectionRect object proxy.
     *
     * @param id  FEMSectionRect object ID
     * @return FEMSectionRect object proxy
     */
    idempotent FEMSectionRect* getFEMSectionRect(long id);

    /**
     * Gets a list of all FEMSectionRect object IDs.
     *
     * @return list of FEMSectionRect object IDs
     */
    idempotent IdList getFEMSectionRectIds();

    /**
     * Gets a list of FEMSectionRect object proxies.
     *
     * @param ids list of FEMSectionRect object IDs
     * @return list of FEMSectionRect object proxies
     */
    idempotent FEMSectionRectList getFEMSectionRectList(IdList ids);

    /**
     * Gets the FEMSectionRect object fields.
     *
     * @param id FEMSectionRect object ID
     * @return FEMSectionRect object fields
     */
    idempotent FEMSectionRectFields getFEMSectionRectFields(long id);

    /**
     * Gets a list of FEMSectionRect object fields.
     *
     * @param ids list of FEMSectionRect object IDs
     * @return list of FEMSectionRect object fields
     */
    idempotent FEMSectionRectFieldsList getFEMSectionRectFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSectionRect objects matching the given
     * reference fields.
     *
     * @param fields FEMSectionRect object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSectionRect objects
     */
    idempotent IdList findEqualFEMSectionRect(FEMSectionRectFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSectionRect object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSectionRect object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSectionRectFields(FEMSectionRectFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMBeamLoad object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMBeamLoad(FEMBeamLoadFields fields);

    /**
     * Adds a set of FEMBeamLoad objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMBeamLoadList(FEMBeamLoadFieldsList fieldsList);

    /**
     * Gets the FEMBeamLoad object proxy.
     *
     * @param id  FEMBeamLoad object ID
     * @return FEMBeamLoad object proxy
     */
    idempotent FEMBeamLoad* getFEMBeamLoad(long id);

    /**
     * Gets a list of all FEMBeamLoad object IDs.
     *
     * @return list of FEMBeamLoad object IDs
     */
    idempotent IdList getFEMBeamLoadIds();

    /**
     * Gets a list of FEMBeamLoad object proxies.
     *
     * @param ids list of FEMBeamLoad object IDs
     * @return list of FEMBeamLoad object proxies
     */
    idempotent FEMBeamLoadList getFEMBeamLoadList(IdList ids);

    /**
     * Gets the FEMBeamLoad object fields.
     *
     * @param id FEMBeamLoad object ID
     * @return FEMBeamLoad object fields
     */
    idempotent FEMBeamLoadFields getFEMBeamLoadFields(long id);

    /**
     * Gets a list of FEMBeamLoad object fields.
     *
     * @param ids list of FEMBeamLoad object IDs
     * @return list of FEMBeamLoad object fields
     */
    idempotent FEMBeamLoadFieldsList getFEMBeamLoadFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMBeamLoad objects matching the given
     * reference fields.
     *
     * @param fields FEMBeamLoad object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMBeamLoad objects
     */
    idempotent IdList findEqualFEMBeamLoad(FEMBeamLoadFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMBeamLoad object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMBeamLoad object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMBeamLoadFields(FEMBeamLoadFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMLoadMassProportional object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMLoadMassProportional(FEMLoadMassProportionalFields fields);

    /**
     * Adds a set of FEMLoadMassProportional objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMLoadMassProportionalList(FEMLoadMassProportionalFieldsList fieldsList);

    /**
     * Gets the FEMLoadMassProportional object proxy.
     *
     * @param id  FEMLoadMassProportional object ID
     * @return FEMLoadMassProportional object proxy
     */
    idempotent FEMLoadMassProportional* getFEMLoadMassProportional(long id);

    /**
     * Gets a list of all FEMLoadMassProportional object IDs.
     *
     * @return list of FEMLoadMassProportional object IDs
     */
    idempotent IdList getFEMLoadMassProportionalIds();

    /**
     * Gets a list of FEMLoadMassProportional object proxies.
     *
     * @param ids list of FEMLoadMassProportional object IDs
     * @return list of FEMLoadMassProportional object proxies
     */
    idempotent FEMLoadMassProportionalList getFEMLoadMassProportionalList(IdList ids);

    /**
     * Gets the FEMLoadMassProportional object fields.
     *
     * @param id FEMLoadMassProportional object ID
     * @return FEMLoadMassProportional object fields
     */
    idempotent FEMLoadMassProportionalFields getFEMLoadMassProportionalFields(long id);

    /**
     * Gets a list of FEMLoadMassProportional object fields.
     *
     * @param ids list of FEMLoadMassProportional object IDs
     * @return list of FEMLoadMassProportional object fields
     */
    idempotent FEMLoadMassProportionalFieldsList getFEMLoadMassProportionalFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMLoadMassProportional objects matching the given
     * reference fields.
     *
     * @param fields FEMLoadMassProportional object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMLoadMassProportional objects
     */
    idempotent IdList findEqualFEMLoadMassProportional(FEMLoadMassProportionalFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMLoadMassProportional object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMLoadMassProportional object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMLoadMassProportionalFields(FEMLoadMassProportionalFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMLink object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMLink(FEMLinkFields fields);

    /**
     * Adds a set of FEMLink objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMLinkList(FEMLinkFieldsList fieldsList);

    /**
     * Gets the FEMLink object proxy.
     *
     * @param id  FEMLink object ID
     * @return FEMLink object proxy
     */
    idempotent FEMLink* getFEMLink(long id);

    /**
     * Gets a list of all FEMLink object IDs.
     *
     * @return list of FEMLink object IDs
     */
    idempotent IdList getFEMLinkIds();

    /**
     * Gets a list of FEMLink object proxies.
     *
     * @param ids list of FEMLink object IDs
     * @return list of FEMLink object proxies
     */
    idempotent FEMLinkList getFEMLinkList(IdList ids);

    /**
     * Gets the FEMLink object fields.
     *
     * @param id FEMLink object ID
     * @return FEMLink object fields
     */
    idempotent FEMLinkFields getFEMLinkFields(long id);

    /**
     * Gets a list of FEMLink object fields.
     *
     * @param ids list of FEMLink object IDs
     * @return list of FEMLink object fields
     */
    idempotent FEMLinkFieldsList getFEMLinkFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMLink objects matching the given
     * reference fields.
     *
     * @param fields FEMLink object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMLink objects
     */
    idempotent IdList findEqualFEMLink(FEMLinkFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMLink object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMLink object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMLinkFields(FEMLinkFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMAxesNode object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMAxesNode(FEMAxesNodeFields fields);

    /**
     * Adds a set of FEMAxesNode objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMAxesNodeList(FEMAxesNodeFieldsList fieldsList);

    /**
     * Gets the FEMAxesNode object proxy.
     *
     * @param id  FEMAxesNode object ID
     * @return FEMAxesNode object proxy
     */
    idempotent FEMAxesNode* getFEMAxesNode(long id);

    /**
     * Gets a list of all FEMAxesNode object IDs.
     *
     * @return list of FEMAxesNode object IDs
     */
    idempotent IdList getFEMAxesNodeIds();

    /**
     * Gets a list of FEMAxesNode object proxies.
     *
     * @param ids list of FEMAxesNode object IDs
     * @return list of FEMAxesNode object proxies
     */
    idempotent FEMAxesNodeList getFEMAxesNodeList(IdList ids);

    /**
     * Gets the FEMAxesNode object fields.
     *
     * @param id FEMAxesNode object ID
     * @return FEMAxesNode object fields
     */
    idempotent FEMAxesNodeFields getFEMAxesNodeFields(long id);

    /**
     * Gets a list of FEMAxesNode object fields.
     *
     * @param ids list of FEMAxesNode object IDs
     * @return list of FEMAxesNode object fields
     */
    idempotent FEMAxesNodeFieldsList getFEMAxesNodeFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMAxesNode objects matching the given
     * reference fields.
     *
     * @param fields FEMAxesNode object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMAxesNode objects
     */
    idempotent IdList findEqualFEMAxesNode(FEMAxesNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMAxesNode object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMAxesNode object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMAxesNodeFields(FEMAxesNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMNMTimeMass object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMNMTimeMass(FEMNMTimeMassFields fields);

    /**
     * Adds a set of FEMNMTimeMass objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMNMTimeMassList(FEMNMTimeMassFieldsList fieldsList);

    /**
     * Gets the FEMNMTimeMass object proxy.
     *
     * @param id  FEMNMTimeMass object ID
     * @return FEMNMTimeMass object proxy
     */
    idempotent FEMNMTimeMass* getFEMNMTimeMass(long id);

    /**
     * Gets a list of all FEMNMTimeMass object IDs.
     *
     * @return list of FEMNMTimeMass object IDs
     */
    idempotent IdList getFEMNMTimeMassIds();

    /**
     * Gets a list of FEMNMTimeMass object proxies.
     *
     * @param ids list of FEMNMTimeMass object IDs
     * @return list of FEMNMTimeMass object proxies
     */
    idempotent FEMNMTimeMassList getFEMNMTimeMassList(IdList ids);

    /**
     * Gets the FEMNMTimeMass object fields.
     *
     * @param id FEMNMTimeMass object ID
     * @return FEMNMTimeMass object fields
     */
    idempotent FEMNMTimeMassFields getFEMNMTimeMassFields(long id);

    /**
     * Gets a list of FEMNMTimeMass object fields.
     *
     * @param ids list of FEMNMTimeMass object IDs
     * @return list of FEMNMTimeMass object fields
     */
    idempotent FEMNMTimeMassFieldsList getFEMNMTimeMassFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMNMTimeMass objects matching the given
     * reference fields.
     *
     * @param fields FEMNMTimeMass object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMNMTimeMass objects
     */
    idempotent IdList findEqualFEMNMTimeMass(FEMNMTimeMassFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMNMTimeMass object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMNMTimeMass object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMNMTimeMassFields(FEMNMTimeMassFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMAppliedDisplacement object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMAppliedDisplacement(FEMAppliedDisplacementFields fields);

    /**
     * Adds a set of FEMAppliedDisplacement objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMAppliedDisplacementList(FEMAppliedDisplacementFieldsList fieldsList);

    /**
     * Gets the FEMAppliedDisplacement object proxy.
     *
     * @param id  FEMAppliedDisplacement object ID
     * @return FEMAppliedDisplacement object proxy
     */
    idempotent FEMAppliedDisplacement* getFEMAppliedDisplacement(long id);

    /**
     * Gets a list of all FEMAppliedDisplacement object IDs.
     *
     * @return list of FEMAppliedDisplacement object IDs
     */
    idempotent IdList getFEMAppliedDisplacementIds();

    /**
     * Gets a list of FEMAppliedDisplacement object proxies.
     *
     * @param ids list of FEMAppliedDisplacement object IDs
     * @return list of FEMAppliedDisplacement object proxies
     */
    idempotent FEMAppliedDisplacementList getFEMAppliedDisplacementList(IdList ids);

    /**
     * Gets the FEMAppliedDisplacement object fields.
     *
     * @param id FEMAppliedDisplacement object ID
     * @return FEMAppliedDisplacement object fields
     */
    idempotent FEMAppliedDisplacementFields getFEMAppliedDisplacementFields(long id);

    /**
     * Gets a list of FEMAppliedDisplacement object fields.
     *
     * @param ids list of FEMAppliedDisplacement object IDs
     * @return list of FEMAppliedDisplacement object fields
     */
    idempotent FEMAppliedDisplacementFieldsList getFEMAppliedDisplacementFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMAppliedDisplacement objects matching the given
     * reference fields.
     *
     * @param fields FEMAppliedDisplacement object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMAppliedDisplacement objects
     */
    idempotent IdList findEqualFEMAppliedDisplacement(FEMAppliedDisplacementFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMAppliedDisplacement object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMAppliedDisplacement object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMAppliedDisplacementFields(FEMAppliedDisplacementFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMTimeFunctions object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMTimeFunctions(FEMTimeFunctionsFields fields);

    /**
     * Adds a set of FEMTimeFunctions objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMTimeFunctionsList(FEMTimeFunctionsFieldsList fieldsList);

    /**
     * Gets the FEMTimeFunctions object proxy.
     *
     * @param id  FEMTimeFunctions object ID
     * @return FEMTimeFunctions object proxy
     */
    idempotent FEMTimeFunctions* getFEMTimeFunctions(long id);

    /**
     * Gets a list of all FEMTimeFunctions object IDs.
     *
     * @return list of FEMTimeFunctions object IDs
     */
    idempotent IdList getFEMTimeFunctionsIds();

    /**
     * Gets a list of FEMTimeFunctions object proxies.
     *
     * @param ids list of FEMTimeFunctions object IDs
     * @return list of FEMTimeFunctions object proxies
     */
    idempotent FEMTimeFunctionsList getFEMTimeFunctionsList(IdList ids);

    /**
     * Gets the FEMTimeFunctions object fields.
     *
     * @param id FEMTimeFunctions object ID
     * @return FEMTimeFunctions object fields
     */
    idempotent FEMTimeFunctionsFields getFEMTimeFunctionsFields(long id);

    /**
     * Gets a list of FEMTimeFunctions object fields.
     *
     * @param ids list of FEMTimeFunctions object IDs
     * @return list of FEMTimeFunctions object fields
     */
    idempotent FEMTimeFunctionsFieldsList getFEMTimeFunctionsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMTimeFunctions objects matching the given
     * reference fields.
     *
     * @param fields FEMTimeFunctions object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMTimeFunctions objects
     */
    idempotent IdList findEqualFEMTimeFunctions(FEMTimeFunctionsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMTimeFunctions object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMTimeFunctions object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMTimeFunctionsFields(FEMTimeFunctionsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMForceStrainData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMForceStrainData(FEMForceStrainDataFields fields);

    /**
     * Adds a set of FEMForceStrainData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMForceStrainDataList(FEMForceStrainDataFieldsList fieldsList);

    /**
     * Gets the FEMForceStrainData object proxy.
     *
     * @param id  FEMForceStrainData object ID
     * @return FEMForceStrainData object proxy
     */
    idempotent FEMForceStrainData* getFEMForceStrainData(long id);

    /**
     * Gets a list of all FEMForceStrainData object IDs.
     *
     * @return list of FEMForceStrainData object IDs
     */
    idempotent IdList getFEMForceStrainDataIds();

    /**
     * Gets a list of FEMForceStrainData object proxies.
     *
     * @param ids list of FEMForceStrainData object IDs
     * @return list of FEMForceStrainData object proxies
     */
    idempotent FEMForceStrainDataList getFEMForceStrainDataList(IdList ids);

    /**
     * Gets the FEMForceStrainData object fields.
     *
     * @param id FEMForceStrainData object ID
     * @return FEMForceStrainData object fields
     */
    idempotent FEMForceStrainDataFields getFEMForceStrainDataFields(long id);

    /**
     * Gets a list of FEMForceStrainData object fields.
     *
     * @param ids list of FEMForceStrainData object IDs
     * @return list of FEMForceStrainData object fields
     */
    idempotent FEMForceStrainDataFieldsList getFEMForceStrainDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMForceStrainData objects matching the given
     * reference fields.
     *
     * @param fields FEMForceStrainData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMForceStrainData objects
     */
    idempotent IdList findEqualFEMForceStrainData(FEMForceStrainDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMForceStrainData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMForceStrainData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMForceStrainDataFields(FEMForceStrainDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSkewDOF object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSkewDOF(FEMSkewDOFFields fields);

    /**
     * Adds a set of FEMSkewDOF objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSkewDOFList(FEMSkewDOFFieldsList fieldsList);

    /**
     * Gets the FEMSkewDOF object proxy.
     *
     * @param id  FEMSkewDOF object ID
     * @return FEMSkewDOF object proxy
     */
    idempotent FEMSkewDOF* getFEMSkewDOF(long id);

    /**
     * Gets a list of all FEMSkewDOF object IDs.
     *
     * @return list of FEMSkewDOF object IDs
     */
    idempotent IdList getFEMSkewDOFIds();

    /**
     * Gets a list of FEMSkewDOF object proxies.
     *
     * @param ids list of FEMSkewDOF object IDs
     * @return list of FEMSkewDOF object proxies
     */
    idempotent FEMSkewDOFList getFEMSkewDOFList(IdList ids);

    /**
     * Gets the FEMSkewDOF object fields.
     *
     * @param id FEMSkewDOF object ID
     * @return FEMSkewDOF object fields
     */
    idempotent FEMSkewDOFFields getFEMSkewDOFFields(long id);

    /**
     * Gets a list of FEMSkewDOF object fields.
     *
     * @param ids list of FEMSkewDOF object IDs
     * @return list of FEMSkewDOF object fields
     */
    idempotent FEMSkewDOFFieldsList getFEMSkewDOFFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSkewDOF objects matching the given
     * reference fields.
     *
     * @param fields FEMSkewDOF object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSkewDOF objects
     */
    idempotent IdList findEqualFEMSkewDOF(FEMSkewDOFFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSkewDOF object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSkewDOF object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSkewDOFFields(FEMSkewDOFFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSectionI object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSectionI(FEMSectionIFields fields);

    /**
     * Adds a set of FEMSectionI objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSectionIList(FEMSectionIFieldsList fieldsList);

    /**
     * Gets the FEMSectionI object proxy.
     *
     * @param id  FEMSectionI object ID
     * @return FEMSectionI object proxy
     */
    idempotent FEMSectionI* getFEMSectionI(long id);

    /**
     * Gets a list of all FEMSectionI object IDs.
     *
     * @return list of FEMSectionI object IDs
     */
    idempotent IdList getFEMSectionIIds();

    /**
     * Gets a list of FEMSectionI object proxies.
     *
     * @param ids list of FEMSectionI object IDs
     * @return list of FEMSectionI object proxies
     */
    idempotent FEMSectionIList getFEMSectionIList(IdList ids);

    /**
     * Gets the FEMSectionI object fields.
     *
     * @param id FEMSectionI object ID
     * @return FEMSectionI object fields
     */
    idempotent FEMSectionIFields getFEMSectionIFields(long id);

    /**
     * Gets a list of FEMSectionI object fields.
     *
     * @param ids list of FEMSectionI object IDs
     * @return list of FEMSectionI object fields
     */
    idempotent FEMSectionIFieldsList getFEMSectionIFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSectionI objects matching the given
     * reference fields.
     *
     * @param fields FEMSectionI object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSectionI objects
     */
    idempotent IdList findEqualFEMSectionI(FEMSectionIFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSectionI object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSectionI object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSectionIFields(FEMSectionIFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMPlasticBilinearMaterial object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMPlasticBilinearMaterial(FEMPlasticBilinearMaterialFields fields);

    /**
     * Adds a set of FEMPlasticBilinearMaterial objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMPlasticBilinearMaterialList(FEMPlasticBilinearMaterialFieldsList fieldsList);

    /**
     * Gets the FEMPlasticBilinearMaterial object proxy.
     *
     * @param id  FEMPlasticBilinearMaterial object ID
     * @return FEMPlasticBilinearMaterial object proxy
     */
    idempotent FEMPlasticBilinearMaterial* getFEMPlasticBilinearMaterial(long id);

    /**
     * Gets a list of all FEMPlasticBilinearMaterial object IDs.
     *
     * @return list of FEMPlasticBilinearMaterial object IDs
     */
    idempotent IdList getFEMPlasticBilinearMaterialIds();

    /**
     * Gets a list of FEMPlasticBilinearMaterial object proxies.
     *
     * @param ids list of FEMPlasticBilinearMaterial object IDs
     * @return list of FEMPlasticBilinearMaterial object proxies
     */
    idempotent FEMPlasticBilinearMaterialList getFEMPlasticBilinearMaterialList(IdList ids);

    /**
     * Gets the FEMPlasticBilinearMaterial object fields.
     *
     * @param id FEMPlasticBilinearMaterial object ID
     * @return FEMPlasticBilinearMaterial object fields
     */
    idempotent FEMPlasticBilinearMaterialFields getFEMPlasticBilinearMaterialFields(long id);

    /**
     * Gets a list of FEMPlasticBilinearMaterial object fields.
     *
     * @param ids list of FEMPlasticBilinearMaterial object IDs
     * @return list of FEMPlasticBilinearMaterial object fields
     */
    idempotent FEMPlasticBilinearMaterialFieldsList getFEMPlasticBilinearMaterialFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMPlasticBilinearMaterial objects matching the given
     * reference fields.
     *
     * @param fields FEMPlasticBilinearMaterial object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMPlasticBilinearMaterial objects
     */
    idempotent IdList findEqualFEMPlasticBilinearMaterial(FEMPlasticBilinearMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMPlasticBilinearMaterial object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMPlasticBilinearMaterial object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMPlasticBilinearMaterialFields(FEMPlasticBilinearMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMMTForceData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMMTForceData(FEMMTForceDataFields fields);

    /**
     * Adds a set of FEMMTForceData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMMTForceDataList(FEMMTForceDataFieldsList fieldsList);

    /**
     * Gets the FEMMTForceData object proxy.
     *
     * @param id  FEMMTForceData object ID
     * @return FEMMTForceData object proxy
     */
    idempotent FEMMTForceData* getFEMMTForceData(long id);

    /**
     * Gets a list of all FEMMTForceData object IDs.
     *
     * @return list of FEMMTForceData object IDs
     */
    idempotent IdList getFEMMTForceDataIds();

    /**
     * Gets a list of FEMMTForceData object proxies.
     *
     * @param ids list of FEMMTForceData object IDs
     * @return list of FEMMTForceData object proxies
     */
    idempotent FEMMTForceDataList getFEMMTForceDataList(IdList ids);

    /**
     * Gets the FEMMTForceData object fields.
     *
     * @param id FEMMTForceData object ID
     * @return FEMMTForceData object fields
     */
    idempotent FEMMTForceDataFields getFEMMTForceDataFields(long id);

    /**
     * Gets a list of FEMMTForceData object fields.
     *
     * @param ids list of FEMMTForceData object IDs
     * @return list of FEMMTForceData object fields
     */
    idempotent FEMMTForceDataFieldsList getFEMMTForceDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMMTForceData objects matching the given
     * reference fields.
     *
     * @param fields FEMMTForceData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMMTForceData objects
     */
    idempotent IdList findEqualFEMMTForceData(FEMMTForceDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMMTForceData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMMTForceData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMMTForceDataFields(FEMMTForceDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMShellPressure object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMShellPressure(FEMShellPressureFields fields);

    /**
     * Adds a set of FEMShellPressure objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMShellPressureList(FEMShellPressureFieldsList fieldsList);

    /**
     * Gets the FEMShellPressure object proxy.
     *
     * @param id  FEMShellPressure object ID
     * @return FEMShellPressure object proxy
     */
    idempotent FEMShellPressure* getFEMShellPressure(long id);

    /**
     * Gets a list of all FEMShellPressure object IDs.
     *
     * @return list of FEMShellPressure object IDs
     */
    idempotent IdList getFEMShellPressureIds();

    /**
     * Gets a list of FEMShellPressure object proxies.
     *
     * @param ids list of FEMShellPressure object IDs
     * @return list of FEMShellPressure object proxies
     */
    idempotent FEMShellPressureList getFEMShellPressureList(IdList ids);

    /**
     * Gets the FEMShellPressure object fields.
     *
     * @param id FEMShellPressure object ID
     * @return FEMShellPressure object fields
     */
    idempotent FEMShellPressureFields getFEMShellPressureFields(long id);

    /**
     * Gets a list of FEMShellPressure object fields.
     *
     * @param ids list of FEMShellPressure object IDs
     * @return list of FEMShellPressure object fields
     */
    idempotent FEMShellPressureFieldsList getFEMShellPressureFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMShellPressure objects matching the given
     * reference fields.
     *
     * @param fields FEMShellPressure object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMShellPressure objects
     */
    idempotent IdList findEqualFEMShellPressure(FEMShellPressureFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMShellPressure object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMShellPressure object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMShellPressureFields(FEMShellPressureFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMMatrices object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMMatrices(FEMMatricesFields fields);

    /**
     * Adds a set of FEMMatrices objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMMatricesList(FEMMatricesFieldsList fieldsList);

    /**
     * Gets the FEMMatrices object proxy.
     *
     * @param id  FEMMatrices object ID
     * @return FEMMatrices object proxy
     */
    idempotent FEMMatrices* getFEMMatrices(long id);

    /**
     * Gets a list of all FEMMatrices object IDs.
     *
     * @return list of FEMMatrices object IDs
     */
    idempotent IdList getFEMMatricesIds();

    /**
     * Gets a list of FEMMatrices object proxies.
     *
     * @param ids list of FEMMatrices object IDs
     * @return list of FEMMatrices object proxies
     */
    idempotent FEMMatricesList getFEMMatricesList(IdList ids);

    /**
     * Gets the FEMMatrices object fields.
     *
     * @param id FEMMatrices object ID
     * @return FEMMatrices object fields
     */
    idempotent FEMMatricesFields getFEMMatricesFields(long id);

    /**
     * Gets a list of FEMMatrices object fields.
     *
     * @param ids list of FEMMatrices object IDs
     * @return list of FEMMatrices object fields
     */
    idempotent FEMMatricesFieldsList getFEMMatricesFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMMatrices objects matching the given
     * reference fields.
     *
     * @param fields FEMMatrices object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMMatrices objects
     */
    idempotent IdList findEqualFEMMatrices(FEMMatricesFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMMatrices object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMMatrices object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMMatricesFields(FEMMatricesFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMDamping object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMDamping(FEMDampingFields fields);

    /**
     * Adds a set of FEMDamping objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMDampingList(FEMDampingFieldsList fieldsList);

    /**
     * Gets the FEMDamping object proxy.
     *
     * @param id  FEMDamping object ID
     * @return FEMDamping object proxy
     */
    idempotent FEMDamping* getFEMDamping(long id);

    /**
     * Gets a list of all FEMDamping object IDs.
     *
     * @return list of FEMDamping object IDs
     */
    idempotent IdList getFEMDampingIds();

    /**
     * Gets a list of FEMDamping object proxies.
     *
     * @param ids list of FEMDamping object IDs
     * @return list of FEMDamping object proxies
     */
    idempotent FEMDampingList getFEMDampingList(IdList ids);

    /**
     * Gets the FEMDamping object fields.
     *
     * @param id FEMDamping object ID
     * @return FEMDamping object fields
     */
    idempotent FEMDampingFields getFEMDampingFields(long id);

    /**
     * Gets a list of FEMDamping object fields.
     *
     * @param ids list of FEMDamping object IDs
     * @return list of FEMDamping object fields
     */
    idempotent FEMDampingFieldsList getFEMDampingFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMDamping objects matching the given
     * reference fields.
     *
     * @param fields FEMDamping object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMDamping objects
     */
    idempotent IdList findEqualFEMDamping(FEMDampingFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMDamping object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMDamping object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMDampingFields(FEMDampingFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMMaterial object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMMaterial(FEMMaterialFields fields);

    /**
     * Adds a set of FEMMaterial objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMMaterialList(FEMMaterialFieldsList fieldsList);

    /**
     * Gets the FEMMaterial object proxy.
     *
     * @param id  FEMMaterial object ID
     * @return FEMMaterial object proxy
     */
    idempotent FEMMaterial* getFEMMaterial(long id);

    /**
     * Gets a list of all FEMMaterial object IDs.
     *
     * @return list of FEMMaterial object IDs
     */
    idempotent IdList getFEMMaterialIds();

    /**
     * Gets a list of FEMMaterial object proxies.
     *
     * @param ids list of FEMMaterial object IDs
     * @return list of FEMMaterial object proxies
     */
    idempotent FEMMaterialList getFEMMaterialList(IdList ids);

    /**
     * Gets the FEMMaterial object fields.
     *
     * @param id FEMMaterial object ID
     * @return FEMMaterial object fields
     */
    idempotent FEMMaterialFields getFEMMaterialFields(long id);

    /**
     * Gets a list of FEMMaterial object fields.
     *
     * @param ids list of FEMMaterial object IDs
     * @return list of FEMMaterial object fields
     */
    idempotent FEMMaterialFieldsList getFEMMaterialFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMMaterial objects matching the given
     * reference fields.
     *
     * @param fields FEMMaterial object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMMaterial objects
     */
    idempotent IdList findEqualFEMMaterial(FEMMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMMaterial object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMMaterial object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMMaterialFields(FEMMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMMatrixData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMMatrixData(FEMMatrixDataFields fields);

    /**
     * Adds a set of FEMMatrixData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMMatrixDataList(FEMMatrixDataFieldsList fieldsList);

    /**
     * Gets the FEMMatrixData object proxy.
     *
     * @param id  FEMMatrixData object ID
     * @return FEMMatrixData object proxy
     */
    idempotent FEMMatrixData* getFEMMatrixData(long id);

    /**
     * Gets a list of all FEMMatrixData object IDs.
     *
     * @return list of FEMMatrixData object IDs
     */
    idempotent IdList getFEMMatrixDataIds();

    /**
     * Gets a list of FEMMatrixData object proxies.
     *
     * @param ids list of FEMMatrixData object IDs
     * @return list of FEMMatrixData object proxies
     */
    idempotent FEMMatrixDataList getFEMMatrixDataList(IdList ids);

    /**
     * Gets the FEMMatrixData object fields.
     *
     * @param id FEMMatrixData object ID
     * @return FEMMatrixData object fields
     */
    idempotent FEMMatrixDataFields getFEMMatrixDataFields(long id);

    /**
     * Gets a list of FEMMatrixData object fields.
     *
     * @param ids list of FEMMatrixData object IDs
     * @return list of FEMMatrixData object fields
     */
    idempotent FEMMatrixDataFieldsList getFEMMatrixDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMMatrixData objects matching the given
     * reference fields.
     *
     * @param fields FEMMatrixData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMMatrixData objects
     */
    idempotent IdList findEqualFEMMatrixData(FEMMatrixDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMMatrixData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMMatrixData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMMatrixDataFields(FEMMatrixDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMShellAxesOrtho object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMShellAxesOrtho(FEMShellAxesOrthoFields fields);

    /**
     * Adds a set of FEMShellAxesOrtho objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMShellAxesOrthoList(FEMShellAxesOrthoFieldsList fieldsList);

    /**
     * Gets the FEMShellAxesOrtho object proxy.
     *
     * @param id  FEMShellAxesOrtho object ID
     * @return FEMShellAxesOrtho object proxy
     */
    idempotent FEMShellAxesOrtho* getFEMShellAxesOrtho(long id);

    /**
     * Gets a list of all FEMShellAxesOrtho object IDs.
     *
     * @return list of FEMShellAxesOrtho object IDs
     */
    idempotent IdList getFEMShellAxesOrthoIds();

    /**
     * Gets a list of FEMShellAxesOrtho object proxies.
     *
     * @param ids list of FEMShellAxesOrtho object IDs
     * @return list of FEMShellAxesOrtho object proxies
     */
    idempotent FEMShellAxesOrthoList getFEMShellAxesOrthoList(IdList ids);

    /**
     * Gets the FEMShellAxesOrtho object fields.
     *
     * @param id FEMShellAxesOrtho object ID
     * @return FEMShellAxesOrtho object fields
     */
    idempotent FEMShellAxesOrthoFields getFEMShellAxesOrthoFields(long id);

    /**
     * Gets a list of FEMShellAxesOrtho object fields.
     *
     * @param ids list of FEMShellAxesOrtho object IDs
     * @return list of FEMShellAxesOrtho object fields
     */
    idempotent FEMShellAxesOrthoFieldsList getFEMShellAxesOrthoFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMShellAxesOrtho objects matching the given
     * reference fields.
     *
     * @param fields FEMShellAxesOrtho object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMShellAxesOrtho objects
     */
    idempotent IdList findEqualFEMShellAxesOrtho(FEMShellAxesOrthoFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMShellAxesOrtho object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMShellAxesOrtho object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMShellAxesOrthoFields(FEMShellAxesOrthoFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMEndreleases object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMEndreleases(FEMEndreleasesFields fields);

    /**
     * Adds a set of FEMEndreleases objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMEndreleasesList(FEMEndreleasesFieldsList fieldsList);

    /**
     * Gets the FEMEndreleases object proxy.
     *
     * @param id  FEMEndreleases object ID
     * @return FEMEndreleases object proxy
     */
    idempotent FEMEndreleases* getFEMEndreleases(long id);

    /**
     * Gets a list of all FEMEndreleases object IDs.
     *
     * @return list of FEMEndreleases object IDs
     */
    idempotent IdList getFEMEndreleasesIds();

    /**
     * Gets a list of FEMEndreleases object proxies.
     *
     * @param ids list of FEMEndreleases object IDs
     * @return list of FEMEndreleases object proxies
     */
    idempotent FEMEndreleasesList getFEMEndreleasesList(IdList ids);

    /**
     * Gets the FEMEndreleases object fields.
     *
     * @param id FEMEndreleases object ID
     * @return FEMEndreleases object fields
     */
    idempotent FEMEndreleasesFields getFEMEndreleasesFields(long id);

    /**
     * Gets a list of FEMEndreleases object fields.
     *
     * @param ids list of FEMEndreleases object IDs
     * @return list of FEMEndreleases object fields
     */
    idempotent FEMEndreleasesFieldsList getFEMEndreleasesFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMEndreleases objects matching the given
     * reference fields.
     *
     * @param fields FEMEndreleases object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMEndreleases objects
     */
    idempotent IdList findEqualFEMEndreleases(FEMEndreleasesFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMEndreleases object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMEndreleases object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMEndreleasesFields(FEMEndreleasesFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMTrussGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMTrussGroup(FEMTrussGroupFields fields);

    /**
     * Adds a set of FEMTrussGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMTrussGroupList(FEMTrussGroupFieldsList fieldsList);

    /**
     * Gets the FEMTrussGroup object proxy.
     *
     * @param id  FEMTrussGroup object ID
     * @return FEMTrussGroup object proxy
     */
    idempotent FEMTrussGroup* getFEMTrussGroup(long id);

    /**
     * Gets a list of all FEMTrussGroup object IDs.
     *
     * @return list of FEMTrussGroup object IDs
     */
    idempotent IdList getFEMTrussGroupIds();

    /**
     * Gets a list of FEMTrussGroup object proxies.
     *
     * @param ids list of FEMTrussGroup object IDs
     * @return list of FEMTrussGroup object proxies
     */
    idempotent FEMTrussGroupList getFEMTrussGroupList(IdList ids);

    /**
     * Gets the FEMTrussGroup object fields.
     *
     * @param id FEMTrussGroup object ID
     * @return FEMTrussGroup object fields
     */
    idempotent FEMTrussGroupFields getFEMTrussGroupFields(long id);

    /**
     * Gets a list of FEMTrussGroup object fields.
     *
     * @param ids list of FEMTrussGroup object IDs
     * @return list of FEMTrussGroup object fields
     */
    idempotent FEMTrussGroupFieldsList getFEMTrussGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMTrussGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMTrussGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMTrussGroup objects
     */
    idempotent IdList findEqualFEMTrussGroup(FEMTrussGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMTrussGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMTrussGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMTrussGroupFields(FEMTrussGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMInitialTemperature object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMInitialTemperature(FEMInitialTemperatureFields fields);

    /**
     * Adds a set of FEMInitialTemperature objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMInitialTemperatureList(FEMInitialTemperatureFieldsList fieldsList);

    /**
     * Gets the FEMInitialTemperature object proxy.
     *
     * @param id  FEMInitialTemperature object ID
     * @return FEMInitialTemperature object proxy
     */
    idempotent FEMInitialTemperature* getFEMInitialTemperature(long id);

    /**
     * Gets a list of all FEMInitialTemperature object IDs.
     *
     * @return list of FEMInitialTemperature object IDs
     */
    idempotent IdList getFEMInitialTemperatureIds();

    /**
     * Gets a list of FEMInitialTemperature object proxies.
     *
     * @param ids list of FEMInitialTemperature object IDs
     * @return list of FEMInitialTemperature object proxies
     */
    idempotent FEMInitialTemperatureList getFEMInitialTemperatureList(IdList ids);

    /**
     * Gets the FEMInitialTemperature object fields.
     *
     * @param id FEMInitialTemperature object ID
     * @return FEMInitialTemperature object fields
     */
    idempotent FEMInitialTemperatureFields getFEMInitialTemperatureFields(long id);

    /**
     * Gets a list of FEMInitialTemperature object fields.
     *
     * @param ids list of FEMInitialTemperature object IDs
     * @return list of FEMInitialTemperature object fields
     */
    idempotent FEMInitialTemperatureFieldsList getFEMInitialTemperatureFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMInitialTemperature objects matching the given
     * reference fields.
     *
     * @param fields FEMInitialTemperature object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMInitialTemperature objects
     */
    idempotent IdList findEqualFEMInitialTemperature(FEMInitialTemperatureFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMInitialTemperature object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMInitialTemperature object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMInitialTemperatureFields(FEMInitialTemperatureFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMThermoIsoMaterials object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMThermoIsoMaterials(FEMThermoIsoMaterialsFields fields);

    /**
     * Adds a set of FEMThermoIsoMaterials objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMThermoIsoMaterialsList(FEMThermoIsoMaterialsFieldsList fieldsList);

    /**
     * Gets the FEMThermoIsoMaterials object proxy.
     *
     * @param id  FEMThermoIsoMaterials object ID
     * @return FEMThermoIsoMaterials object proxy
     */
    idempotent FEMThermoIsoMaterials* getFEMThermoIsoMaterials(long id);

    /**
     * Gets a list of all FEMThermoIsoMaterials object IDs.
     *
     * @return list of FEMThermoIsoMaterials object IDs
     */
    idempotent IdList getFEMThermoIsoMaterialsIds();

    /**
     * Gets a list of FEMThermoIsoMaterials object proxies.
     *
     * @param ids list of FEMThermoIsoMaterials object IDs
     * @return list of FEMThermoIsoMaterials object proxies
     */
    idempotent FEMThermoIsoMaterialsList getFEMThermoIsoMaterialsList(IdList ids);

    /**
     * Gets the FEMThermoIsoMaterials object fields.
     *
     * @param id FEMThermoIsoMaterials object ID
     * @return FEMThermoIsoMaterials object fields
     */
    idempotent FEMThermoIsoMaterialsFields getFEMThermoIsoMaterialsFields(long id);

    /**
     * Gets a list of FEMThermoIsoMaterials object fields.
     *
     * @param ids list of FEMThermoIsoMaterials object IDs
     * @return list of FEMThermoIsoMaterials object fields
     */
    idempotent FEMThermoIsoMaterialsFieldsList getFEMThermoIsoMaterialsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMThermoIsoMaterials objects matching the given
     * reference fields.
     *
     * @param fields FEMThermoIsoMaterials object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMThermoIsoMaterials objects
     */
    idempotent IdList findEqualFEMThermoIsoMaterials(FEMThermoIsoMaterialsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMThermoIsoMaterials object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMThermoIsoMaterials object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMThermoIsoMaterialsFields(FEMThermoIsoMaterialsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMThermoIsoData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMThermoIsoData(FEMThermoIsoDataFields fields);

    /**
     * Adds a set of FEMThermoIsoData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMThermoIsoDataList(FEMThermoIsoDataFieldsList fieldsList);

    /**
     * Gets the FEMThermoIsoData object proxy.
     *
     * @param id  FEMThermoIsoData object ID
     * @return FEMThermoIsoData object proxy
     */
    idempotent FEMThermoIsoData* getFEMThermoIsoData(long id);

    /**
     * Gets a list of all FEMThermoIsoData object IDs.
     *
     * @return list of FEMThermoIsoData object IDs
     */
    idempotent IdList getFEMThermoIsoDataIds();

    /**
     * Gets a list of FEMThermoIsoData object proxies.
     *
     * @param ids list of FEMThermoIsoData object IDs
     * @return list of FEMThermoIsoData object proxies
     */
    idempotent FEMThermoIsoDataList getFEMThermoIsoDataList(IdList ids);

    /**
     * Gets the FEMThermoIsoData object fields.
     *
     * @param id FEMThermoIsoData object ID
     * @return FEMThermoIsoData object fields
     */
    idempotent FEMThermoIsoDataFields getFEMThermoIsoDataFields(long id);

    /**
     * Gets a list of FEMThermoIsoData object fields.
     *
     * @param ids list of FEMThermoIsoData object IDs
     * @return list of FEMThermoIsoData object fields
     */
    idempotent FEMThermoIsoDataFieldsList getFEMThermoIsoDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMThermoIsoData objects matching the given
     * reference fields.
     *
     * @param fields FEMThermoIsoData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMThermoIsoData objects
     */
    idempotent IdList findEqualFEMThermoIsoData(FEMThermoIsoDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMThermoIsoData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMThermoIsoData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMThermoIsoDataFields(FEMThermoIsoDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMContactGroup3 object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMContactGroup3(FEMContactGroup3Fields fields);

    /**
     * Adds a set of FEMContactGroup3 objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMContactGroup3List(FEMContactGroup3FieldsList fieldsList);

    /**
     * Gets the FEMContactGroup3 object proxy.
     *
     * @param id  FEMContactGroup3 object ID
     * @return FEMContactGroup3 object proxy
     */
    idempotent FEMContactGroup3* getFEMContactGroup3(long id);

    /**
     * Gets a list of all FEMContactGroup3 object IDs.
     *
     * @return list of FEMContactGroup3 object IDs
     */
    idempotent IdList getFEMContactGroup3Ids();

    /**
     * Gets a list of FEMContactGroup3 object proxies.
     *
     * @param ids list of FEMContactGroup3 object IDs
     * @return list of FEMContactGroup3 object proxies
     */
    idempotent FEMContactGroup3List getFEMContactGroup3List(IdList ids);

    /**
     * Gets the FEMContactGroup3 object fields.
     *
     * @param id FEMContactGroup3 object ID
     * @return FEMContactGroup3 object fields
     */
    idempotent FEMContactGroup3Fields getFEMContactGroup3Fields(long id);

    /**
     * Gets a list of FEMContactGroup3 object fields.
     *
     * @param ids list of FEMContactGroup3 object IDs
     * @return list of FEMContactGroup3 object fields
     */
    idempotent FEMContactGroup3FieldsList getFEMContactGroup3FieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMContactGroup3 objects matching the given
     * reference fields.
     *
     * @param fields FEMContactGroup3 object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMContactGroup3 objects
     */
    idempotent IdList findEqualFEMContactGroup3(FEMContactGroup3Fields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMContactGroup3 object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMContactGroup3 object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMContactGroup3Fields(FEMContactGroup3Fields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMNLElasticMaterials object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMNLElasticMaterials(FEMNLElasticMaterialsFields fields);

    /**
     * Adds a set of FEMNLElasticMaterials objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMNLElasticMaterialsList(FEMNLElasticMaterialsFieldsList fieldsList);

    /**
     * Gets the FEMNLElasticMaterials object proxy.
     *
     * @param id  FEMNLElasticMaterials object ID
     * @return FEMNLElasticMaterials object proxy
     */
    idempotent FEMNLElasticMaterials* getFEMNLElasticMaterials(long id);

    /**
     * Gets a list of all FEMNLElasticMaterials object IDs.
     *
     * @return list of FEMNLElasticMaterials object IDs
     */
    idempotent IdList getFEMNLElasticMaterialsIds();

    /**
     * Gets a list of FEMNLElasticMaterials object proxies.
     *
     * @param ids list of FEMNLElasticMaterials object IDs
     * @return list of FEMNLElasticMaterials object proxies
     */
    idempotent FEMNLElasticMaterialsList getFEMNLElasticMaterialsList(IdList ids);

    /**
     * Gets the FEMNLElasticMaterials object fields.
     *
     * @param id FEMNLElasticMaterials object ID
     * @return FEMNLElasticMaterials object fields
     */
    idempotent FEMNLElasticMaterialsFields getFEMNLElasticMaterialsFields(long id);

    /**
     * Gets a list of FEMNLElasticMaterials object fields.
     *
     * @param ids list of FEMNLElasticMaterials object IDs
     * @return list of FEMNLElasticMaterials object fields
     */
    idempotent FEMNLElasticMaterialsFieldsList getFEMNLElasticMaterialsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMNLElasticMaterials objects matching the given
     * reference fields.
     *
     * @param fields FEMNLElasticMaterials object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMNLElasticMaterials objects
     */
    idempotent IdList findEqualFEMNLElasticMaterials(FEMNLElasticMaterialsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMNLElasticMaterials object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMNLElasticMaterials object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMNLElasticMaterialsFields(FEMNLElasticMaterialsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMPlate object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMPlate(FEMPlateFields fields);

    /**
     * Adds a set of FEMPlate objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMPlateList(FEMPlateFieldsList fieldsList);

    /**
     * Gets the FEMPlate object proxy.
     *
     * @param id  FEMPlate object ID
     * @return FEMPlate object proxy
     */
    idempotent FEMPlate* getFEMPlate(long id);

    /**
     * Gets a list of all FEMPlate object IDs.
     *
     * @return list of FEMPlate object IDs
     */
    idempotent IdList getFEMPlateIds();

    /**
     * Gets a list of FEMPlate object proxies.
     *
     * @param ids list of FEMPlate object IDs
     * @return list of FEMPlate object proxies
     */
    idempotent FEMPlateList getFEMPlateList(IdList ids);

    /**
     * Gets the FEMPlate object fields.
     *
     * @param id FEMPlate object ID
     * @return FEMPlate object fields
     */
    idempotent FEMPlateFields getFEMPlateFields(long id);

    /**
     * Gets a list of FEMPlate object fields.
     *
     * @param ids list of FEMPlate object IDs
     * @return list of FEMPlate object fields
     */
    idempotent FEMPlateFieldsList getFEMPlateFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMPlate objects matching the given
     * reference fields.
     *
     * @param fields FEMPlate object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMPlate objects
     */
    idempotent IdList findEqualFEMPlate(FEMPlateFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMPlate object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMPlate object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMPlateFields(FEMPlateFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMIsoBeam object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMIsoBeam(FEMIsoBeamFields fields);

    /**
     * Adds a set of FEMIsoBeam objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMIsoBeamList(FEMIsoBeamFieldsList fieldsList);

    /**
     * Gets the FEMIsoBeam object proxy.
     *
     * @param id  FEMIsoBeam object ID
     * @return FEMIsoBeam object proxy
     */
    idempotent FEMIsoBeam* getFEMIsoBeam(long id);

    /**
     * Gets a list of all FEMIsoBeam object IDs.
     *
     * @return list of FEMIsoBeam object IDs
     */
    idempotent IdList getFEMIsoBeamIds();

    /**
     * Gets a list of FEMIsoBeam object proxies.
     *
     * @param ids list of FEMIsoBeam object IDs
     * @return list of FEMIsoBeam object proxies
     */
    idempotent FEMIsoBeamList getFEMIsoBeamList(IdList ids);

    /**
     * Gets the FEMIsoBeam object fields.
     *
     * @param id FEMIsoBeam object ID
     * @return FEMIsoBeam object fields
     */
    idempotent FEMIsoBeamFields getFEMIsoBeamFields(long id);

    /**
     * Gets a list of FEMIsoBeam object fields.
     *
     * @param ids list of FEMIsoBeam object IDs
     * @return list of FEMIsoBeam object fields
     */
    idempotent FEMIsoBeamFieldsList getFEMIsoBeamFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMIsoBeam objects matching the given
     * reference fields.
     *
     * @param fields FEMIsoBeam object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMIsoBeam objects
     */
    idempotent IdList findEqualFEMIsoBeam(FEMIsoBeamFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMIsoBeam object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMIsoBeam object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMIsoBeamFields(FEMIsoBeamFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMAppliedConcentratedLoad object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMAppliedConcentratedLoad(FEMAppliedConcentratedLoadFields fields);

    /**
     * Adds a set of FEMAppliedConcentratedLoad objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMAppliedConcentratedLoadList(FEMAppliedConcentratedLoadFieldsList fieldsList);

    /**
     * Gets the FEMAppliedConcentratedLoad object proxy.
     *
     * @param id  FEMAppliedConcentratedLoad object ID
     * @return FEMAppliedConcentratedLoad object proxy
     */
    idempotent FEMAppliedConcentratedLoad* getFEMAppliedConcentratedLoad(long id);

    /**
     * Gets a list of all FEMAppliedConcentratedLoad object IDs.
     *
     * @return list of FEMAppliedConcentratedLoad object IDs
     */
    idempotent IdList getFEMAppliedConcentratedLoadIds();

    /**
     * Gets a list of FEMAppliedConcentratedLoad object proxies.
     *
     * @param ids list of FEMAppliedConcentratedLoad object IDs
     * @return list of FEMAppliedConcentratedLoad object proxies
     */
    idempotent FEMAppliedConcentratedLoadList getFEMAppliedConcentratedLoadList(IdList ids);

    /**
     * Gets the FEMAppliedConcentratedLoad object fields.
     *
     * @param id FEMAppliedConcentratedLoad object ID
     * @return FEMAppliedConcentratedLoad object fields
     */
    idempotent FEMAppliedConcentratedLoadFields getFEMAppliedConcentratedLoadFields(long id);

    /**
     * Gets a list of FEMAppliedConcentratedLoad object fields.
     *
     * @param ids list of FEMAppliedConcentratedLoad object IDs
     * @return list of FEMAppliedConcentratedLoad object fields
     */
    idempotent FEMAppliedConcentratedLoadFieldsList getFEMAppliedConcentratedLoadFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMAppliedConcentratedLoad objects matching the given
     * reference fields.
     *
     * @param fields FEMAppliedConcentratedLoad object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMAppliedConcentratedLoad objects
     */
    idempotent IdList findEqualFEMAppliedConcentratedLoad(FEMAppliedConcentratedLoadFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMAppliedConcentratedLoad object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMAppliedConcentratedLoad object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMAppliedConcentratedLoadFields(FEMAppliedConcentratedLoadFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMTwoDSolidGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMTwoDSolidGroup(FEMTwoDSolidGroupFields fields);

    /**
     * Adds a set of FEMTwoDSolidGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMTwoDSolidGroupList(FEMTwoDSolidGroupFieldsList fieldsList);

    /**
     * Gets the FEMTwoDSolidGroup object proxy.
     *
     * @param id  FEMTwoDSolidGroup object ID
     * @return FEMTwoDSolidGroup object proxy
     */
    idempotent FEMTwoDSolidGroup* getFEMTwoDSolidGroup(long id);

    /**
     * Gets a list of all FEMTwoDSolidGroup object IDs.
     *
     * @return list of FEMTwoDSolidGroup object IDs
     */
    idempotent IdList getFEMTwoDSolidGroupIds();

    /**
     * Gets a list of FEMTwoDSolidGroup object proxies.
     *
     * @param ids list of FEMTwoDSolidGroup object IDs
     * @return list of FEMTwoDSolidGroup object proxies
     */
    idempotent FEMTwoDSolidGroupList getFEMTwoDSolidGroupList(IdList ids);

    /**
     * Gets the FEMTwoDSolidGroup object fields.
     *
     * @param id FEMTwoDSolidGroup object ID
     * @return FEMTwoDSolidGroup object fields
     */
    idempotent FEMTwoDSolidGroupFields getFEMTwoDSolidGroupFields(long id);

    /**
     * Gets a list of FEMTwoDSolidGroup object fields.
     *
     * @param ids list of FEMTwoDSolidGroup object IDs
     * @return list of FEMTwoDSolidGroup object fields
     */
    idempotent FEMTwoDSolidGroupFieldsList getFEMTwoDSolidGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMTwoDSolidGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMTwoDSolidGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMTwoDSolidGroup objects
     */
    idempotent IdList findEqualFEMTwoDSolidGroup(FEMTwoDSolidGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMTwoDSolidGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMTwoDSolidGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMTwoDSolidGroupFields(FEMTwoDSolidGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMGroup(FEMGroupFields fields);

    /**
     * Adds a set of FEMGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMGroupList(FEMGroupFieldsList fieldsList);

    /**
     * Gets the FEMGroup object proxy.
     *
     * @param id  FEMGroup object ID
     * @return FEMGroup object proxy
     */
    idempotent FEMGroup* getFEMGroup(long id);

    /**
     * Gets a list of all FEMGroup object IDs.
     *
     * @return list of FEMGroup object IDs
     */
    idempotent IdList getFEMGroupIds();

    /**
     * Gets a list of FEMGroup object proxies.
     *
     * @param ids list of FEMGroup object IDs
     * @return list of FEMGroup object proxies
     */
    idempotent FEMGroupList getFEMGroupList(IdList ids);

    /**
     * Gets the FEMGroup object fields.
     *
     * @param id FEMGroup object ID
     * @return FEMGroup object fields
     */
    idempotent FEMGroupFields getFEMGroupFields(long id);

    /**
     * Gets a list of FEMGroup object fields.
     *
     * @param ids list of FEMGroup object IDs
     * @return list of FEMGroup object fields
     */
    idempotent FEMGroupFieldsList getFEMGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMGroup objects
     */
    idempotent IdList findEqualFEMGroup(FEMGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMGroupFields(FEMGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMProperties object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMProperties(FEMPropertiesFields fields);

    /**
     * Adds a set of FEMProperties objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMPropertiesList(FEMPropertiesFieldsList fieldsList);

    /**
     * Gets the FEMProperties object proxy.
     *
     * @param id  FEMProperties object ID
     * @return FEMProperties object proxy
     */
    idempotent FEMProperties* getFEMProperties(long id);

    /**
     * Gets a list of all FEMProperties object IDs.
     *
     * @return list of FEMProperties object IDs
     */
    idempotent IdList getFEMPropertiesIds();

    /**
     * Gets a list of FEMProperties object proxies.
     *
     * @param ids list of FEMProperties object IDs
     * @return list of FEMProperties object proxies
     */
    idempotent FEMPropertiesList getFEMPropertiesList(IdList ids);

    /**
     * Gets the FEMProperties object fields.
     *
     * @param id FEMProperties object ID
     * @return FEMProperties object fields
     */
    idempotent FEMPropertiesFields getFEMPropertiesFields(long id);

    /**
     * Gets a list of FEMProperties object fields.
     *
     * @param ids list of FEMProperties object IDs
     * @return list of FEMProperties object fields
     */
    idempotent FEMPropertiesFieldsList getFEMPropertiesFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMProperties objects matching the given
     * reference fields.
     *
     * @param fields FEMProperties object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMProperties objects
     */
    idempotent IdList findEqualFEMProperties(FEMPropertiesFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMProperties object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMProperties object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMPropertiesFields(FEMPropertiesFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMThreeDSolidGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMThreeDSolidGroup(FEMThreeDSolidGroupFields fields);

    /**
     * Adds a set of FEMThreeDSolidGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMThreeDSolidGroupList(FEMThreeDSolidGroupFieldsList fieldsList);

    /**
     * Gets the FEMThreeDSolidGroup object proxy.
     *
     * @param id  FEMThreeDSolidGroup object ID
     * @return FEMThreeDSolidGroup object proxy
     */
    idempotent FEMThreeDSolidGroup* getFEMThreeDSolidGroup(long id);

    /**
     * Gets a list of all FEMThreeDSolidGroup object IDs.
     *
     * @return list of FEMThreeDSolidGroup object IDs
     */
    idempotent IdList getFEMThreeDSolidGroupIds();

    /**
     * Gets a list of FEMThreeDSolidGroup object proxies.
     *
     * @param ids list of FEMThreeDSolidGroup object IDs
     * @return list of FEMThreeDSolidGroup object proxies
     */
    idempotent FEMThreeDSolidGroupList getFEMThreeDSolidGroupList(IdList ids);

    /**
     * Gets the FEMThreeDSolidGroup object fields.
     *
     * @param id FEMThreeDSolidGroup object ID
     * @return FEMThreeDSolidGroup object fields
     */
    idempotent FEMThreeDSolidGroupFields getFEMThreeDSolidGroupFields(long id);

    /**
     * Gets a list of FEMThreeDSolidGroup object fields.
     *
     * @param ids list of FEMThreeDSolidGroup object IDs
     * @return list of FEMThreeDSolidGroup object fields
     */
    idempotent FEMThreeDSolidGroupFieldsList getFEMThreeDSolidGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMThreeDSolidGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMThreeDSolidGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMThreeDSolidGroup objects
     */
    idempotent IdList findEqualFEMThreeDSolidGroup(FEMThreeDSolidGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMThreeDSolidGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMThreeDSolidGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMThreeDSolidGroupFields(FEMThreeDSolidGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMThreeDSolid object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMThreeDSolid(FEMThreeDSolidFields fields);

    /**
     * Adds a set of FEMThreeDSolid objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMThreeDSolidList(FEMThreeDSolidFieldsList fieldsList);

    /**
     * Gets the FEMThreeDSolid object proxy.
     *
     * @param id  FEMThreeDSolid object ID
     * @return FEMThreeDSolid object proxy
     */
    idempotent FEMThreeDSolid* getFEMThreeDSolid(long id);

    /**
     * Gets a list of all FEMThreeDSolid object IDs.
     *
     * @return list of FEMThreeDSolid object IDs
     */
    idempotent IdList getFEMThreeDSolidIds();

    /**
     * Gets a list of FEMThreeDSolid object proxies.
     *
     * @param ids list of FEMThreeDSolid object IDs
     * @return list of FEMThreeDSolid object proxies
     */
    idempotent FEMThreeDSolidList getFEMThreeDSolidList(IdList ids);

    /**
     * Gets the FEMThreeDSolid object fields.
     *
     * @param id FEMThreeDSolid object ID
     * @return FEMThreeDSolid object fields
     */
    idempotent FEMThreeDSolidFields getFEMThreeDSolidFields(long id);

    /**
     * Gets a list of FEMThreeDSolid object fields.
     *
     * @param ids list of FEMThreeDSolid object IDs
     * @return list of FEMThreeDSolid object fields
     */
    idempotent FEMThreeDSolidFieldsList getFEMThreeDSolidFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMThreeDSolid objects matching the given
     * reference fields.
     *
     * @param fields FEMThreeDSolid object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMThreeDSolid objects
     */
    idempotent IdList findEqualFEMThreeDSolid(FEMThreeDSolidFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMThreeDSolid object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMThreeDSolid object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMThreeDSolidFields(FEMThreeDSolidFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSectionProp object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSectionProp(FEMSectionPropFields fields);

    /**
     * Adds a set of FEMSectionProp objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSectionPropList(FEMSectionPropFieldsList fieldsList);

    /**
     * Gets the FEMSectionProp object proxy.
     *
     * @param id  FEMSectionProp object ID
     * @return FEMSectionProp object proxy
     */
    idempotent FEMSectionProp* getFEMSectionProp(long id);

    /**
     * Gets a list of all FEMSectionProp object IDs.
     *
     * @return list of FEMSectionProp object IDs
     */
    idempotent IdList getFEMSectionPropIds();

    /**
     * Gets a list of FEMSectionProp object proxies.
     *
     * @param ids list of FEMSectionProp object IDs
     * @return list of FEMSectionProp object proxies
     */
    idempotent FEMSectionPropList getFEMSectionPropList(IdList ids);

    /**
     * Gets the FEMSectionProp object fields.
     *
     * @param id FEMSectionProp object ID
     * @return FEMSectionProp object fields
     */
    idempotent FEMSectionPropFields getFEMSectionPropFields(long id);

    /**
     * Gets a list of FEMSectionProp object fields.
     *
     * @param ids list of FEMSectionProp object IDs
     * @return list of FEMSectionProp object fields
     */
    idempotent FEMSectionPropFieldsList getFEMSectionPropFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSectionProp objects matching the given
     * reference fields.
     *
     * @param fields FEMSectionProp object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSectionProp objects
     */
    idempotent IdList findEqualFEMSectionProp(FEMSectionPropFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSectionProp object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSectionProp object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSectionPropFields(FEMSectionPropFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMElasticMaterial object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMElasticMaterial(FEMElasticMaterialFields fields);

    /**
     * Adds a set of FEMElasticMaterial objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMElasticMaterialList(FEMElasticMaterialFieldsList fieldsList);

    /**
     * Gets the FEMElasticMaterial object proxy.
     *
     * @param id  FEMElasticMaterial object ID
     * @return FEMElasticMaterial object proxy
     */
    idempotent FEMElasticMaterial* getFEMElasticMaterial(long id);

    /**
     * Gets a list of all FEMElasticMaterial object IDs.
     *
     * @return list of FEMElasticMaterial object IDs
     */
    idempotent IdList getFEMElasticMaterialIds();

    /**
     * Gets a list of FEMElasticMaterial object proxies.
     *
     * @param ids list of FEMElasticMaterial object IDs
     * @return list of FEMElasticMaterial object proxies
     */
    idempotent FEMElasticMaterialList getFEMElasticMaterialList(IdList ids);

    /**
     * Gets the FEMElasticMaterial object fields.
     *
     * @param id FEMElasticMaterial object ID
     * @return FEMElasticMaterial object fields
     */
    idempotent FEMElasticMaterialFields getFEMElasticMaterialFields(long id);

    /**
     * Gets a list of FEMElasticMaterial object fields.
     *
     * @param ids list of FEMElasticMaterial object IDs
     * @return list of FEMElasticMaterial object fields
     */
    idempotent FEMElasticMaterialFieldsList getFEMElasticMaterialFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMElasticMaterial objects matching the given
     * reference fields.
     *
     * @param fields FEMElasticMaterial object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMElasticMaterial objects
     */
    idempotent IdList findEqualFEMElasticMaterial(FEMElasticMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMElasticMaterial object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMElasticMaterial object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMElasticMaterialFields(FEMElasticMaterialFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMPoints object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMPoints(FEMPointsFields fields);

    /**
     * Adds a set of FEMPoints objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMPointsList(FEMPointsFieldsList fieldsList);

    /**
     * Gets the FEMPoints object proxy.
     *
     * @param id  FEMPoints object ID
     * @return FEMPoints object proxy
     */
    idempotent FEMPoints* getFEMPoints(long id);

    /**
     * Gets a list of all FEMPoints object IDs.
     *
     * @return list of FEMPoints object IDs
     */
    idempotent IdList getFEMPointsIds();

    /**
     * Gets a list of FEMPoints object proxies.
     *
     * @param ids list of FEMPoints object IDs
     * @return list of FEMPoints object proxies
     */
    idempotent FEMPointsList getFEMPointsList(IdList ids);

    /**
     * Gets the FEMPoints object fields.
     *
     * @param id FEMPoints object ID
     * @return FEMPoints object fields
     */
    idempotent FEMPointsFields getFEMPointsFields(long id);

    /**
     * Gets a list of FEMPoints object fields.
     *
     * @param ids list of FEMPoints object IDs
     * @return list of FEMPoints object fields
     */
    idempotent FEMPointsFieldsList getFEMPointsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMPoints objects matching the given
     * reference fields.
     *
     * @param fields FEMPoints object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMPoints objects
     */
    idempotent IdList findEqualFEMPoints(FEMPointsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMPoints object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMPoints object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMPointsFields(FEMPointsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMThermoOrthMaterials object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMThermoOrthMaterials(FEMThermoOrthMaterialsFields fields);

    /**
     * Adds a set of FEMThermoOrthMaterials objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMThermoOrthMaterialsList(FEMThermoOrthMaterialsFieldsList fieldsList);

    /**
     * Gets the FEMThermoOrthMaterials object proxy.
     *
     * @param id  FEMThermoOrthMaterials object ID
     * @return FEMThermoOrthMaterials object proxy
     */
    idempotent FEMThermoOrthMaterials* getFEMThermoOrthMaterials(long id);

    /**
     * Gets a list of all FEMThermoOrthMaterials object IDs.
     *
     * @return list of FEMThermoOrthMaterials object IDs
     */
    idempotent IdList getFEMThermoOrthMaterialsIds();

    /**
     * Gets a list of FEMThermoOrthMaterials object proxies.
     *
     * @param ids list of FEMThermoOrthMaterials object IDs
     * @return list of FEMThermoOrthMaterials object proxies
     */
    idempotent FEMThermoOrthMaterialsList getFEMThermoOrthMaterialsList(IdList ids);

    /**
     * Gets the FEMThermoOrthMaterials object fields.
     *
     * @param id FEMThermoOrthMaterials object ID
     * @return FEMThermoOrthMaterials object fields
     */
    idempotent FEMThermoOrthMaterialsFields getFEMThermoOrthMaterialsFields(long id);

    /**
     * Gets a list of FEMThermoOrthMaterials object fields.
     *
     * @param ids list of FEMThermoOrthMaterials object IDs
     * @return list of FEMThermoOrthMaterials object fields
     */
    idempotent FEMThermoOrthMaterialsFieldsList getFEMThermoOrthMaterialsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMThermoOrthMaterials objects matching the given
     * reference fields.
     *
     * @param fields FEMThermoOrthMaterials object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMThermoOrthMaterials objects
     */
    idempotent IdList findEqualFEMThermoOrthMaterials(FEMThermoOrthMaterialsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMThermoOrthMaterials object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMThermoOrthMaterials object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMThermoOrthMaterialsFields(FEMThermoOrthMaterialsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMConstraints object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMConstraints(FEMConstraintsFields fields);

    /**
     * Adds a set of FEMConstraints objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMConstraintsList(FEMConstraintsFieldsList fieldsList);

    /**
     * Gets the FEMConstraints object proxy.
     *
     * @param id  FEMConstraints object ID
     * @return FEMConstraints object proxy
     */
    idempotent FEMConstraints* getFEMConstraints(long id);

    /**
     * Gets a list of all FEMConstraints object IDs.
     *
     * @return list of FEMConstraints object IDs
     */
    idempotent IdList getFEMConstraintsIds();

    /**
     * Gets a list of FEMConstraints object proxies.
     *
     * @param ids list of FEMConstraints object IDs
     * @return list of FEMConstraints object proxies
     */
    idempotent FEMConstraintsList getFEMConstraintsList(IdList ids);

    /**
     * Gets the FEMConstraints object fields.
     *
     * @param id FEMConstraints object ID
     * @return FEMConstraints object fields
     */
    idempotent FEMConstraintsFields getFEMConstraintsFields(long id);

    /**
     * Gets a list of FEMConstraints object fields.
     *
     * @param ids list of FEMConstraints object IDs
     * @return list of FEMConstraints object fields
     */
    idempotent FEMConstraintsFieldsList getFEMConstraintsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMConstraints objects matching the given
     * reference fields.
     *
     * @param fields FEMConstraints object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMConstraints objects
     */
    idempotent IdList findEqualFEMConstraints(FEMConstraintsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMConstraints object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMConstraints object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMConstraintsFields(FEMConstraintsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMMCrigidities object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMMCrigidities(FEMMCrigiditiesFields fields);

    /**
     * Adds a set of FEMMCrigidities objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMMCrigiditiesList(FEMMCrigiditiesFieldsList fieldsList);

    /**
     * Gets the FEMMCrigidities object proxy.
     *
     * @param id  FEMMCrigidities object ID
     * @return FEMMCrigidities object proxy
     */
    idempotent FEMMCrigidities* getFEMMCrigidities(long id);

    /**
     * Gets a list of all FEMMCrigidities object IDs.
     *
     * @return list of FEMMCrigidities object IDs
     */
    idempotent IdList getFEMMCrigiditiesIds();

    /**
     * Gets a list of FEMMCrigidities object proxies.
     *
     * @param ids list of FEMMCrigidities object IDs
     * @return list of FEMMCrigidities object proxies
     */
    idempotent FEMMCrigiditiesList getFEMMCrigiditiesList(IdList ids);

    /**
     * Gets the FEMMCrigidities object fields.
     *
     * @param id FEMMCrigidities object ID
     * @return FEMMCrigidities object fields
     */
    idempotent FEMMCrigiditiesFields getFEMMCrigiditiesFields(long id);

    /**
     * Gets a list of FEMMCrigidities object fields.
     *
     * @param ids list of FEMMCrigidities object IDs
     * @return list of FEMMCrigidities object fields
     */
    idempotent FEMMCrigiditiesFieldsList getFEMMCrigiditiesFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMMCrigidities objects matching the given
     * reference fields.
     *
     * @param fields FEMMCrigidities object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMMCrigidities objects
     */
    idempotent IdList findEqualFEMMCrigidities(FEMMCrigiditiesFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMMCrigidities object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMMCrigidities object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMMCrigiditiesFields(FEMMCrigiditiesFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSkeySysNode object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSkeySysNode(FEMSkeySysNodeFields fields);

    /**
     * Adds a set of FEMSkeySysNode objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSkeySysNodeList(FEMSkeySysNodeFieldsList fieldsList);

    /**
     * Gets the FEMSkeySysNode object proxy.
     *
     * @param id  FEMSkeySysNode object ID
     * @return FEMSkeySysNode object proxy
     */
    idempotent FEMSkeySysNode* getFEMSkeySysNode(long id);

    /**
     * Gets a list of all FEMSkeySysNode object IDs.
     *
     * @return list of FEMSkeySysNode object IDs
     */
    idempotent IdList getFEMSkeySysNodeIds();

    /**
     * Gets a list of FEMSkeySysNode object proxies.
     *
     * @param ids list of FEMSkeySysNode object IDs
     * @return list of FEMSkeySysNode object proxies
     */
    idempotent FEMSkeySysNodeList getFEMSkeySysNodeList(IdList ids);

    /**
     * Gets the FEMSkeySysNode object fields.
     *
     * @param id FEMSkeySysNode object ID
     * @return FEMSkeySysNode object fields
     */
    idempotent FEMSkeySysNodeFields getFEMSkeySysNodeFields(long id);

    /**
     * Gets a list of FEMSkeySysNode object fields.
     *
     * @param ids list of FEMSkeySysNode object IDs
     * @return list of FEMSkeySysNode object fields
     */
    idempotent FEMSkeySysNodeFieldsList getFEMSkeySysNodeFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSkeySysNode objects matching the given
     * reference fields.
     *
     * @param fields FEMSkeySysNode object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSkeySysNode objects
     */
    idempotent IdList findEqualFEMSkeySysNode(FEMSkeySysNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSkeySysNode object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSkeySysNode object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSkeySysNodeFields(FEMSkeySysNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMIsoBeamGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMIsoBeamGroup(FEMIsoBeamGroupFields fields);

    /**
     * Adds a set of FEMIsoBeamGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMIsoBeamGroupList(FEMIsoBeamGroupFieldsList fieldsList);

    /**
     * Gets the FEMIsoBeamGroup object proxy.
     *
     * @param id  FEMIsoBeamGroup object ID
     * @return FEMIsoBeamGroup object proxy
     */
    idempotent FEMIsoBeamGroup* getFEMIsoBeamGroup(long id);

    /**
     * Gets a list of all FEMIsoBeamGroup object IDs.
     *
     * @return list of FEMIsoBeamGroup object IDs
     */
    idempotent IdList getFEMIsoBeamGroupIds();

    /**
     * Gets a list of FEMIsoBeamGroup object proxies.
     *
     * @param ids list of FEMIsoBeamGroup object IDs
     * @return list of FEMIsoBeamGroup object proxies
     */
    idempotent FEMIsoBeamGroupList getFEMIsoBeamGroupList(IdList ids);

    /**
     * Gets the FEMIsoBeamGroup object fields.
     *
     * @param id FEMIsoBeamGroup object ID
     * @return FEMIsoBeamGroup object fields
     */
    idempotent FEMIsoBeamGroupFields getFEMIsoBeamGroupFields(long id);

    /**
     * Gets a list of FEMIsoBeamGroup object fields.
     *
     * @param ids list of FEMIsoBeamGroup object IDs
     * @return list of FEMIsoBeamGroup object fields
     */
    idempotent FEMIsoBeamGroupFieldsList getFEMIsoBeamGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMIsoBeamGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMIsoBeamGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMIsoBeamGroup objects
     */
    idempotent IdList findEqualFEMIsoBeamGroup(FEMIsoBeamGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMIsoBeamGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMIsoBeamGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMIsoBeamGroupFields(FEMIsoBeamGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMShellDOF object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMShellDOF(FEMShellDOFFields fields);

    /**
     * Adds a set of FEMShellDOF objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMShellDOFList(FEMShellDOFFieldsList fieldsList);

    /**
     * Gets the FEMShellDOF object proxy.
     *
     * @param id  FEMShellDOF object ID
     * @return FEMShellDOF object proxy
     */
    idempotent FEMShellDOF* getFEMShellDOF(long id);

    /**
     * Gets a list of all FEMShellDOF object IDs.
     *
     * @return list of FEMShellDOF object IDs
     */
    idempotent IdList getFEMShellDOFIds();

    /**
     * Gets a list of FEMShellDOF object proxies.
     *
     * @param ids list of FEMShellDOF object IDs
     * @return list of FEMShellDOF object proxies
     */
    idempotent FEMShellDOFList getFEMShellDOFList(IdList ids);

    /**
     * Gets the FEMShellDOF object fields.
     *
     * @param id FEMShellDOF object ID
     * @return FEMShellDOF object fields
     */
    idempotent FEMShellDOFFields getFEMShellDOFFields(long id);

    /**
     * Gets a list of FEMShellDOF object fields.
     *
     * @param ids list of FEMShellDOF object IDs
     * @return list of FEMShellDOF object fields
     */
    idempotent FEMShellDOFFieldsList getFEMShellDOFFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMShellDOF objects matching the given
     * reference fields.
     *
     * @param fields FEMShellDOF object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMShellDOF objects
     */
    idempotent IdList findEqualFEMShellDOF(FEMShellDOFFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMShellDOF object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMShellDOF object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMShellDOFFields(FEMShellDOFFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMCrossSections object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMCrossSections(FEMCrossSectionsFields fields);

    /**
     * Adds a set of FEMCrossSections objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMCrossSectionsList(FEMCrossSectionsFieldsList fieldsList);

    /**
     * Gets the FEMCrossSections object proxy.
     *
     * @param id  FEMCrossSections object ID
     * @return FEMCrossSections object proxy
     */
    idempotent FEMCrossSections* getFEMCrossSections(long id);

    /**
     * Gets a list of all FEMCrossSections object IDs.
     *
     * @return list of FEMCrossSections object IDs
     */
    idempotent IdList getFEMCrossSectionsIds();

    /**
     * Gets a list of FEMCrossSections object proxies.
     *
     * @param ids list of FEMCrossSections object IDs
     * @return list of FEMCrossSections object proxies
     */
    idempotent FEMCrossSectionsList getFEMCrossSectionsList(IdList ids);

    /**
     * Gets the FEMCrossSections object fields.
     *
     * @param id FEMCrossSections object ID
     * @return FEMCrossSections object fields
     */
    idempotent FEMCrossSectionsFields getFEMCrossSectionsFields(long id);

    /**
     * Gets a list of FEMCrossSections object fields.
     *
     * @param ids list of FEMCrossSections object IDs
     * @return list of FEMCrossSections object fields
     */
    idempotent FEMCrossSectionsFieldsList getFEMCrossSectionsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMCrossSections objects matching the given
     * reference fields.
     *
     * @param fields FEMCrossSections object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMCrossSections objects
     */
    idempotent IdList findEqualFEMCrossSections(FEMCrossSectionsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMCrossSections object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMCrossSections object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMCrossSectionsFields(FEMCrossSectionsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMTwistMomentData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMTwistMomentData(FEMTwistMomentDataFields fields);

    /**
     * Adds a set of FEMTwistMomentData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMTwistMomentDataList(FEMTwistMomentDataFieldsList fieldsList);

    /**
     * Gets the FEMTwistMomentData object proxy.
     *
     * @param id  FEMTwistMomentData object ID
     * @return FEMTwistMomentData object proxy
     */
    idempotent FEMTwistMomentData* getFEMTwistMomentData(long id);

    /**
     * Gets a list of all FEMTwistMomentData object IDs.
     *
     * @return list of FEMTwistMomentData object IDs
     */
    idempotent IdList getFEMTwistMomentDataIds();

    /**
     * Gets a list of FEMTwistMomentData object proxies.
     *
     * @param ids list of FEMTwistMomentData object IDs
     * @return list of FEMTwistMomentData object proxies
     */
    idempotent FEMTwistMomentDataList getFEMTwistMomentDataList(IdList ids);

    /**
     * Gets the FEMTwistMomentData object fields.
     *
     * @param id FEMTwistMomentData object ID
     * @return FEMTwistMomentData object fields
     */
    idempotent FEMTwistMomentDataFields getFEMTwistMomentDataFields(long id);

    /**
     * Gets a list of FEMTwistMomentData object fields.
     *
     * @param ids list of FEMTwistMomentData object IDs
     * @return list of FEMTwistMomentData object fields
     */
    idempotent FEMTwistMomentDataFieldsList getFEMTwistMomentDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMTwistMomentData objects matching the given
     * reference fields.
     *
     * @param fields FEMTwistMomentData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMTwistMomentData objects
     */
    idempotent IdList findEqualFEMTwistMomentData(FEMTwistMomentDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMTwistMomentData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMTwistMomentData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMTwistMomentDataFields(FEMTwistMomentDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMShell object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMShell(FEMShellFields fields);

    /**
     * Adds a set of FEMShell objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMShellList(FEMShellFieldsList fieldsList);

    /**
     * Gets the FEMShell object proxy.
     *
     * @param id  FEMShell object ID
     * @return FEMShell object proxy
     */
    idempotent FEMShell* getFEMShell(long id);

    /**
     * Gets a list of all FEMShell object IDs.
     *
     * @return list of FEMShell object IDs
     */
    idempotent IdList getFEMShellIds();

    /**
     * Gets a list of FEMShell object proxies.
     *
     * @param ids list of FEMShell object IDs
     * @return list of FEMShell object proxies
     */
    idempotent FEMShellList getFEMShellList(IdList ids);

    /**
     * Gets the FEMShell object fields.
     *
     * @param id FEMShell object ID
     * @return FEMShell object fields
     */
    idempotent FEMShellFields getFEMShellFields(long id);

    /**
     * Gets a list of FEMShell object fields.
     *
     * @param ids list of FEMShell object IDs
     * @return list of FEMShell object fields
     */
    idempotent FEMShellFieldsList getFEMShellFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMShell objects matching the given
     * reference fields.
     *
     * @param fields FEMShell object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMShell objects
     */
    idempotent IdList findEqualFEMShell(FEMShellFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMShell object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMShell object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMShellFields(FEMShellFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMNTNContact object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMNTNContact(FEMNTNContactFields fields);

    /**
     * Adds a set of FEMNTNContact objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMNTNContactList(FEMNTNContactFieldsList fieldsList);

    /**
     * Gets the FEMNTNContact object proxy.
     *
     * @param id  FEMNTNContact object ID
     * @return FEMNTNContact object proxy
     */
    idempotent FEMNTNContact* getFEMNTNContact(long id);

    /**
     * Gets a list of all FEMNTNContact object IDs.
     *
     * @return list of FEMNTNContact object IDs
     */
    idempotent IdList getFEMNTNContactIds();

    /**
     * Gets a list of FEMNTNContact object proxies.
     *
     * @param ids list of FEMNTNContact object IDs
     * @return list of FEMNTNContact object proxies
     */
    idempotent FEMNTNContactList getFEMNTNContactList(IdList ids);

    /**
     * Gets the FEMNTNContact object fields.
     *
     * @param id FEMNTNContact object ID
     * @return FEMNTNContact object fields
     */
    idempotent FEMNTNContactFields getFEMNTNContactFields(long id);

    /**
     * Gets a list of FEMNTNContact object fields.
     *
     * @param ids list of FEMNTNContact object IDs
     * @return list of FEMNTNContact object fields
     */
    idempotent FEMNTNContactFieldsList getFEMNTNContactFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMNTNContact objects matching the given
     * reference fields.
     *
     * @param fields FEMNTNContact object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMNTNContact objects
     */
    idempotent IdList findEqualFEMNTNContact(FEMNTNContactFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMNTNContact object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMNTNContact object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMNTNContactFields(FEMNTNContactFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMShellLayer object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMShellLayer(FEMShellLayerFields fields);

    /**
     * Adds a set of FEMShellLayer objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMShellLayerList(FEMShellLayerFieldsList fieldsList);

    /**
     * Gets the FEMShellLayer object proxy.
     *
     * @param id  FEMShellLayer object ID
     * @return FEMShellLayer object proxy
     */
    idempotent FEMShellLayer* getFEMShellLayer(long id);

    /**
     * Gets a list of all FEMShellLayer object IDs.
     *
     * @return list of FEMShellLayer object IDs
     */
    idempotent IdList getFEMShellLayerIds();

    /**
     * Gets a list of FEMShellLayer object proxies.
     *
     * @param ids list of FEMShellLayer object IDs
     * @return list of FEMShellLayer object proxies
     */
    idempotent FEMShellLayerList getFEMShellLayerList(IdList ids);

    /**
     * Gets the FEMShellLayer object fields.
     *
     * @param id FEMShellLayer object ID
     * @return FEMShellLayer object fields
     */
    idempotent FEMShellLayerFields getFEMShellLayerFields(long id);

    /**
     * Gets a list of FEMShellLayer object fields.
     *
     * @param ids list of FEMShellLayer object IDs
     * @return list of FEMShellLayer object fields
     */
    idempotent FEMShellLayerFieldsList getFEMShellLayerFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMShellLayer objects matching the given
     * reference fields.
     *
     * @param fields FEMShellLayer object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMShellLayer objects
     */
    idempotent IdList findEqualFEMShellLayer(FEMShellLayerFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMShellLayer object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMShellLayer object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMShellLayerFields(FEMShellLayerFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSkewSysAngles object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSkewSysAngles(FEMSkewSysAnglesFields fields);

    /**
     * Adds a set of FEMSkewSysAngles objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSkewSysAnglesList(FEMSkewSysAnglesFieldsList fieldsList);

    /**
     * Gets the FEMSkewSysAngles object proxy.
     *
     * @param id  FEMSkewSysAngles object ID
     * @return FEMSkewSysAngles object proxy
     */
    idempotent FEMSkewSysAngles* getFEMSkewSysAngles(long id);

    /**
     * Gets a list of all FEMSkewSysAngles object IDs.
     *
     * @return list of FEMSkewSysAngles object IDs
     */
    idempotent IdList getFEMSkewSysAnglesIds();

    /**
     * Gets a list of FEMSkewSysAngles object proxies.
     *
     * @param ids list of FEMSkewSysAngles object IDs
     * @return list of FEMSkewSysAngles object proxies
     */
    idempotent FEMSkewSysAnglesList getFEMSkewSysAnglesList(IdList ids);

    /**
     * Gets the FEMSkewSysAngles object fields.
     *
     * @param id FEMSkewSysAngles object ID
     * @return FEMSkewSysAngles object fields
     */
    idempotent FEMSkewSysAnglesFields getFEMSkewSysAnglesFields(long id);

    /**
     * Gets a list of FEMSkewSysAngles object fields.
     *
     * @param ids list of FEMSkewSysAngles object IDs
     * @return list of FEMSkewSysAngles object fields
     */
    idempotent FEMSkewSysAnglesFieldsList getFEMSkewSysAnglesFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSkewSysAngles objects matching the given
     * reference fields.
     *
     * @param fields FEMSkewSysAngles object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSkewSysAngles objects
     */
    idempotent IdList findEqualFEMSkewSysAngles(FEMSkewSysAnglesFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSkewSysAngles object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSkewSysAngles object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSkewSysAnglesFields(FEMSkewSysAnglesFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMGroundMotionRecord object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMGroundMotionRecord(FEMGroundMotionRecordFields fields);

    /**
     * Adds a set of FEMGroundMotionRecord objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMGroundMotionRecordList(FEMGroundMotionRecordFieldsList fieldsList);

    /**
     * Gets the FEMGroundMotionRecord object proxy.
     *
     * @param id  FEMGroundMotionRecord object ID
     * @return FEMGroundMotionRecord object proxy
     */
    idempotent FEMGroundMotionRecord* getFEMGroundMotionRecord(long id);

    /**
     * Gets a list of all FEMGroundMotionRecord object IDs.
     *
     * @return list of FEMGroundMotionRecord object IDs
     */
    idempotent IdList getFEMGroundMotionRecordIds();

    /**
     * Gets a list of FEMGroundMotionRecord object proxies.
     *
     * @param ids list of FEMGroundMotionRecord object IDs
     * @return list of FEMGroundMotionRecord object proxies
     */
    idempotent FEMGroundMotionRecordList getFEMGroundMotionRecordList(IdList ids);

    /**
     * Gets the FEMGroundMotionRecord object fields.
     *
     * @param id FEMGroundMotionRecord object ID
     * @return FEMGroundMotionRecord object fields
     */
    idempotent FEMGroundMotionRecordFields getFEMGroundMotionRecordFields(long id);

    /**
     * Gets a list of FEMGroundMotionRecord object fields.
     *
     * @param ids list of FEMGroundMotionRecord object IDs
     * @return list of FEMGroundMotionRecord object fields
     */
    idempotent FEMGroundMotionRecordFieldsList getFEMGroundMotionRecordFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMGroundMotionRecord objects matching the given
     * reference fields.
     *
     * @param fields FEMGroundMotionRecord object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMGroundMotionRecord objects
     */
    idempotent IdList findEqualFEMGroundMotionRecord(FEMGroundMotionRecordFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMGroundMotionRecord object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMGroundMotionRecord object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMGroundMotionRecordFields(FEMGroundMotionRecordFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMGeneralGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMGeneralGroup(FEMGeneralGroupFields fields);

    /**
     * Adds a set of FEMGeneralGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMGeneralGroupList(FEMGeneralGroupFieldsList fieldsList);

    /**
     * Gets the FEMGeneralGroup object proxy.
     *
     * @param id  FEMGeneralGroup object ID
     * @return FEMGeneralGroup object proxy
     */
    idempotent FEMGeneralGroup* getFEMGeneralGroup(long id);

    /**
     * Gets a list of all FEMGeneralGroup object IDs.
     *
     * @return list of FEMGeneralGroup object IDs
     */
    idempotent IdList getFEMGeneralGroupIds();

    /**
     * Gets a list of FEMGeneralGroup object proxies.
     *
     * @param ids list of FEMGeneralGroup object IDs
     * @return list of FEMGeneralGroup object proxies
     */
    idempotent FEMGeneralGroupList getFEMGeneralGroupList(IdList ids);

    /**
     * Gets the FEMGeneralGroup object fields.
     *
     * @param id FEMGeneralGroup object ID
     * @return FEMGeneralGroup object fields
     */
    idempotent FEMGeneralGroupFields getFEMGeneralGroupFields(long id);

    /**
     * Gets a list of FEMGeneralGroup object fields.
     *
     * @param ids list of FEMGeneralGroup object IDs
     * @return list of FEMGeneralGroup object fields
     */
    idempotent FEMGeneralGroupFieldsList getFEMGeneralGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMGeneralGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMGeneralGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMGeneralGroup objects
     */
    idempotent IdList findEqualFEMGeneralGroup(FEMGeneralGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMGeneralGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMGeneralGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMGeneralGroupFields(FEMGeneralGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMTwoDSolid object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMTwoDSolid(FEMTwoDSolidFields fields);

    /**
     * Adds a set of FEMTwoDSolid objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMTwoDSolidList(FEMTwoDSolidFieldsList fieldsList);

    /**
     * Gets the FEMTwoDSolid object proxy.
     *
     * @param id  FEMTwoDSolid object ID
     * @return FEMTwoDSolid object proxy
     */
    idempotent FEMTwoDSolid* getFEMTwoDSolid(long id);

    /**
     * Gets a list of all FEMTwoDSolid object IDs.
     *
     * @return list of FEMTwoDSolid object IDs
     */
    idempotent IdList getFEMTwoDSolidIds();

    /**
     * Gets a list of FEMTwoDSolid object proxies.
     *
     * @param ids list of FEMTwoDSolid object IDs
     * @return list of FEMTwoDSolid object proxies
     */
    idempotent FEMTwoDSolidList getFEMTwoDSolidList(IdList ids);

    /**
     * Gets the FEMTwoDSolid object fields.
     *
     * @param id FEMTwoDSolid object ID
     * @return FEMTwoDSolid object fields
     */
    idempotent FEMTwoDSolidFields getFEMTwoDSolidFields(long id);

    /**
     * Gets a list of FEMTwoDSolid object fields.
     *
     * @param ids list of FEMTwoDSolid object IDs
     * @return list of FEMTwoDSolid object fields
     */
    idempotent FEMTwoDSolidFieldsList getFEMTwoDSolidFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMTwoDSolid objects matching the given
     * reference fields.
     *
     * @param fields FEMTwoDSolid object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMTwoDSolid objects
     */
    idempotent IdList findEqualFEMTwoDSolid(FEMTwoDSolidFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMTwoDSolid object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMTwoDSolid object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMTwoDSolidFields(FEMTwoDSolidFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMAppliedTemperature object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMAppliedTemperature(FEMAppliedTemperatureFields fields);

    /**
     * Adds a set of FEMAppliedTemperature objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMAppliedTemperatureList(FEMAppliedTemperatureFieldsList fieldsList);

    /**
     * Gets the FEMAppliedTemperature object proxy.
     *
     * @param id  FEMAppliedTemperature object ID
     * @return FEMAppliedTemperature object proxy
     */
    idempotent FEMAppliedTemperature* getFEMAppliedTemperature(long id);

    /**
     * Gets a list of all FEMAppliedTemperature object IDs.
     *
     * @return list of FEMAppliedTemperature object IDs
     */
    idempotent IdList getFEMAppliedTemperatureIds();

    /**
     * Gets a list of FEMAppliedTemperature object proxies.
     *
     * @param ids list of FEMAppliedTemperature object IDs
     * @return list of FEMAppliedTemperature object proxies
     */
    idempotent FEMAppliedTemperatureList getFEMAppliedTemperatureList(IdList ids);

    /**
     * Gets the FEMAppliedTemperature object fields.
     *
     * @param id FEMAppliedTemperature object ID
     * @return FEMAppliedTemperature object fields
     */
    idempotent FEMAppliedTemperatureFields getFEMAppliedTemperatureFields(long id);

    /**
     * Gets a list of FEMAppliedTemperature object fields.
     *
     * @param ids list of FEMAppliedTemperature object IDs
     * @return list of FEMAppliedTemperature object fields
     */
    idempotent FEMAppliedTemperatureFieldsList getFEMAppliedTemperatureFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMAppliedTemperature objects matching the given
     * reference fields.
     *
     * @param fields FEMAppliedTemperature object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMAppliedTemperature objects
     */
    idempotent IdList findEqualFEMAppliedTemperature(FEMAppliedTemperatureFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMAppliedTemperature object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMAppliedTemperature object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMAppliedTemperatureFields(FEMAppliedTemperatureFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMMatrixSets object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMMatrixSets(FEMMatrixSetsFields fields);

    /**
     * Adds a set of FEMMatrixSets objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMMatrixSetsList(FEMMatrixSetsFieldsList fieldsList);

    /**
     * Gets the FEMMatrixSets object proxy.
     *
     * @param id  FEMMatrixSets object ID
     * @return FEMMatrixSets object proxy
     */
    idempotent FEMMatrixSets* getFEMMatrixSets(long id);

    /**
     * Gets a list of all FEMMatrixSets object IDs.
     *
     * @return list of FEMMatrixSets object IDs
     */
    idempotent IdList getFEMMatrixSetsIds();

    /**
     * Gets a list of FEMMatrixSets object proxies.
     *
     * @param ids list of FEMMatrixSets object IDs
     * @return list of FEMMatrixSets object proxies
     */
    idempotent FEMMatrixSetsList getFEMMatrixSetsList(IdList ids);

    /**
     * Gets the FEMMatrixSets object fields.
     *
     * @param id FEMMatrixSets object ID
     * @return FEMMatrixSets object fields
     */
    idempotent FEMMatrixSetsFields getFEMMatrixSetsFields(long id);

    /**
     * Gets a list of FEMMatrixSets object fields.
     *
     * @param ids list of FEMMatrixSets object IDs
     * @return list of FEMMatrixSets object fields
     */
    idempotent FEMMatrixSetsFieldsList getFEMMatrixSetsFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMMatrixSets objects matching the given
     * reference fields.
     *
     * @param fields FEMMatrixSets object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMMatrixSets objects
     */
    idempotent IdList findEqualFEMMatrixSets(FEMMatrixSetsFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMMatrixSets object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMMatrixSets object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMMatrixSetsFields(FEMMatrixSetsFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMConstraintCoef object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMConstraintCoef(FEMConstraintCoefFields fields);

    /**
     * Adds a set of FEMConstraintCoef objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMConstraintCoefList(FEMConstraintCoefFieldsList fieldsList);

    /**
     * Gets the FEMConstraintCoef object proxy.
     *
     * @param id  FEMConstraintCoef object ID
     * @return FEMConstraintCoef object proxy
     */
    idempotent FEMConstraintCoef* getFEMConstraintCoef(long id);

    /**
     * Gets a list of all FEMConstraintCoef object IDs.
     *
     * @return list of FEMConstraintCoef object IDs
     */
    idempotent IdList getFEMConstraintCoefIds();

    /**
     * Gets a list of FEMConstraintCoef object proxies.
     *
     * @param ids list of FEMConstraintCoef object IDs
     * @return list of FEMConstraintCoef object proxies
     */
    idempotent FEMConstraintCoefList getFEMConstraintCoefList(IdList ids);

    /**
     * Gets the FEMConstraintCoef object fields.
     *
     * @param id FEMConstraintCoef object ID
     * @return FEMConstraintCoef object fields
     */
    idempotent FEMConstraintCoefFields getFEMConstraintCoefFields(long id);

    /**
     * Gets a list of FEMConstraintCoef object fields.
     *
     * @param ids list of FEMConstraintCoef object IDs
     * @return list of FEMConstraintCoef object fields
     */
    idempotent FEMConstraintCoefFieldsList getFEMConstraintCoefFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMConstraintCoef objects matching the given
     * reference fields.
     *
     * @param fields FEMConstraintCoef object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMConstraintCoef objects
     */
    idempotent IdList findEqualFEMConstraintCoef(FEMConstraintCoefFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMConstraintCoef object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMConstraintCoef object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMConstraintCoefFields(FEMConstraintCoefFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSectionBox object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSectionBox(FEMSectionBoxFields fields);

    /**
     * Adds a set of FEMSectionBox objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSectionBoxList(FEMSectionBoxFieldsList fieldsList);

    /**
     * Gets the FEMSectionBox object proxy.
     *
     * @param id  FEMSectionBox object ID
     * @return FEMSectionBox object proxy
     */
    idempotent FEMSectionBox* getFEMSectionBox(long id);

    /**
     * Gets a list of all FEMSectionBox object IDs.
     *
     * @return list of FEMSectionBox object IDs
     */
    idempotent IdList getFEMSectionBoxIds();

    /**
     * Gets a list of FEMSectionBox object proxies.
     *
     * @param ids list of FEMSectionBox object IDs
     * @return list of FEMSectionBox object proxies
     */
    idempotent FEMSectionBoxList getFEMSectionBoxList(IdList ids);

    /**
     * Gets the FEMSectionBox object fields.
     *
     * @param id FEMSectionBox object ID
     * @return FEMSectionBox object fields
     */
    idempotent FEMSectionBoxFields getFEMSectionBoxFields(long id);

    /**
     * Gets a list of FEMSectionBox object fields.
     *
     * @param ids list of FEMSectionBox object IDs
     * @return list of FEMSectionBox object fields
     */
    idempotent FEMSectionBoxFieldsList getFEMSectionBoxFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSectionBox objects matching the given
     * reference fields.
     *
     * @param fields FEMSectionBox object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSectionBox objects
     */
    idempotent IdList findEqualFEMSectionBox(FEMSectionBoxFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSectionBox object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSectionBox object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSectionBoxFields(FEMSectionBoxFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMNKDisplForce object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMNKDisplForce(FEMNKDisplForceFields fields);

    /**
     * Adds a set of FEMNKDisplForce objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMNKDisplForceList(FEMNKDisplForceFieldsList fieldsList);

    /**
     * Gets the FEMNKDisplForce object proxy.
     *
     * @param id  FEMNKDisplForce object ID
     * @return FEMNKDisplForce object proxy
     */
    idempotent FEMNKDisplForce* getFEMNKDisplForce(long id);

    /**
     * Gets a list of all FEMNKDisplForce object IDs.
     *
     * @return list of FEMNKDisplForce object IDs
     */
    idempotent IdList getFEMNKDisplForceIds();

    /**
     * Gets a list of FEMNKDisplForce object proxies.
     *
     * @param ids list of FEMNKDisplForce object IDs
     * @return list of FEMNKDisplForce object proxies
     */
    idempotent FEMNKDisplForceList getFEMNKDisplForceList(IdList ids);

    /**
     * Gets the FEMNKDisplForce object fields.
     *
     * @param id FEMNKDisplForce object ID
     * @return FEMNKDisplForce object fields
     */
    idempotent FEMNKDisplForceFields getFEMNKDisplForceFields(long id);

    /**
     * Gets a list of FEMNKDisplForce object fields.
     *
     * @param ids list of FEMNKDisplForce object IDs
     * @return list of FEMNKDisplForce object fields
     */
    idempotent FEMNKDisplForceFieldsList getFEMNKDisplForceFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMNKDisplForce objects matching the given
     * reference fields.
     *
     * @param fields FEMNKDisplForce object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMNKDisplForce objects
     */
    idempotent IdList findEqualFEMNKDisplForce(FEMNKDisplForceFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMNKDisplForce object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMNKDisplForce object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMNKDisplForceFields(FEMNKDisplForceFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMPlasticStrainStress object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMPlasticStrainStress(FEMPlasticStrainStressFields fields);

    /**
     * Adds a set of FEMPlasticStrainStress objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMPlasticStrainStressList(FEMPlasticStrainStressFieldsList fieldsList);

    /**
     * Gets the FEMPlasticStrainStress object proxy.
     *
     * @param id  FEMPlasticStrainStress object ID
     * @return FEMPlasticStrainStress object proxy
     */
    idempotent FEMPlasticStrainStress* getFEMPlasticStrainStress(long id);

    /**
     * Gets a list of all FEMPlasticStrainStress object IDs.
     *
     * @return list of FEMPlasticStrainStress object IDs
     */
    idempotent IdList getFEMPlasticStrainStressIds();

    /**
     * Gets a list of FEMPlasticStrainStress object proxies.
     *
     * @param ids list of FEMPlasticStrainStress object IDs
     * @return list of FEMPlasticStrainStress object proxies
     */
    idempotent FEMPlasticStrainStressList getFEMPlasticStrainStressList(IdList ids);

    /**
     * Gets the FEMPlasticStrainStress object fields.
     *
     * @param id FEMPlasticStrainStress object ID
     * @return FEMPlasticStrainStress object fields
     */
    idempotent FEMPlasticStrainStressFields getFEMPlasticStrainStressFields(long id);

    /**
     * Gets a list of FEMPlasticStrainStress object fields.
     *
     * @param ids list of FEMPlasticStrainStress object IDs
     * @return list of FEMPlasticStrainStress object fields
     */
    idempotent FEMPlasticStrainStressFieldsList getFEMPlasticStrainStressFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMPlasticStrainStress objects matching the given
     * reference fields.
     *
     * @param fields FEMPlasticStrainStress object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMPlasticStrainStress objects
     */
    idempotent IdList findEqualFEMPlasticStrainStress(FEMPlasticStrainStressFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMPlasticStrainStress object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMPlasticStrainStress object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMPlasticStrainStressFields(FEMPlasticStrainStressFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMShellAxesOrthoData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMShellAxesOrthoData(FEMShellAxesOrthoDataFields fields);

    /**
     * Adds a set of FEMShellAxesOrthoData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMShellAxesOrthoDataList(FEMShellAxesOrthoDataFieldsList fieldsList);

    /**
     * Gets the FEMShellAxesOrthoData object proxy.
     *
     * @param id  FEMShellAxesOrthoData object ID
     * @return FEMShellAxesOrthoData object proxy
     */
    idempotent FEMShellAxesOrthoData* getFEMShellAxesOrthoData(long id);

    /**
     * Gets a list of all FEMShellAxesOrthoData object IDs.
     *
     * @return list of FEMShellAxesOrthoData object IDs
     */
    idempotent IdList getFEMShellAxesOrthoDataIds();

    /**
     * Gets a list of FEMShellAxesOrthoData object proxies.
     *
     * @param ids list of FEMShellAxesOrthoData object IDs
     * @return list of FEMShellAxesOrthoData object proxies
     */
    idempotent FEMShellAxesOrthoDataList getFEMShellAxesOrthoDataList(IdList ids);

    /**
     * Gets the FEMShellAxesOrthoData object fields.
     *
     * @param id FEMShellAxesOrthoData object ID
     * @return FEMShellAxesOrthoData object fields
     */
    idempotent FEMShellAxesOrthoDataFields getFEMShellAxesOrthoDataFields(long id);

    /**
     * Gets a list of FEMShellAxesOrthoData object fields.
     *
     * @param ids list of FEMShellAxesOrthoData object IDs
     * @return list of FEMShellAxesOrthoData object fields
     */
    idempotent FEMShellAxesOrthoDataFieldsList getFEMShellAxesOrthoDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMShellAxesOrthoData objects matching the given
     * reference fields.
     *
     * @param fields FEMShellAxesOrthoData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMShellAxesOrthoData objects
     */
    idempotent IdList findEqualFEMShellAxesOrthoData(FEMShellAxesOrthoDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMShellAxesOrthoData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMShellAxesOrthoData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMShellAxesOrthoDataFields(FEMShellAxesOrthoDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMGeneralNode object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMGeneralNode(FEMGeneralNodeFields fields);

    /**
     * Adds a set of FEMGeneralNode objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMGeneralNodeList(FEMGeneralNodeFieldsList fieldsList);

    /**
     * Gets the FEMGeneralNode object proxy.
     *
     * @param id  FEMGeneralNode object ID
     * @return FEMGeneralNode object proxy
     */
    idempotent FEMGeneralNode* getFEMGeneralNode(long id);

    /**
     * Gets a list of all FEMGeneralNode object IDs.
     *
     * @return list of FEMGeneralNode object IDs
     */
    idempotent IdList getFEMGeneralNodeIds();

    /**
     * Gets a list of FEMGeneralNode object proxies.
     *
     * @param ids list of FEMGeneralNode object IDs
     * @return list of FEMGeneralNode object proxies
     */
    idempotent FEMGeneralNodeList getFEMGeneralNodeList(IdList ids);

    /**
     * Gets the FEMGeneralNode object fields.
     *
     * @param id FEMGeneralNode object ID
     * @return FEMGeneralNode object fields
     */
    idempotent FEMGeneralNodeFields getFEMGeneralNodeFields(long id);

    /**
     * Gets a list of FEMGeneralNode object fields.
     *
     * @param ids list of FEMGeneralNode object IDs
     * @return list of FEMGeneralNode object fields
     */
    idempotent FEMGeneralNodeFieldsList getFEMGeneralNodeFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMGeneralNode objects matching the given
     * reference fields.
     *
     * @param fields FEMGeneralNode object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMGeneralNode objects
     */
    idempotent IdList findEqualFEMGeneralNode(FEMGeneralNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMGeneralNode object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMGeneralNode object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMGeneralNodeFields(FEMGeneralNodeFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMStrLines object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMStrLines(FEMStrLinesFields fields);

    /**
     * Adds a set of FEMStrLines objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMStrLinesList(FEMStrLinesFieldsList fieldsList);

    /**
     * Gets the FEMStrLines object proxy.
     *
     * @param id  FEMStrLines object ID
     * @return FEMStrLines object proxy
     */
    idempotent FEMStrLines* getFEMStrLines(long id);

    /**
     * Gets a list of all FEMStrLines object IDs.
     *
     * @return list of FEMStrLines object IDs
     */
    idempotent IdList getFEMStrLinesIds();

    /**
     * Gets a list of FEMStrLines object proxies.
     *
     * @param ids list of FEMStrLines object IDs
     * @return list of FEMStrLines object proxies
     */
    idempotent FEMStrLinesList getFEMStrLinesList(IdList ids);

    /**
     * Gets the FEMStrLines object fields.
     *
     * @param id FEMStrLines object ID
     * @return FEMStrLines object fields
     */
    idempotent FEMStrLinesFields getFEMStrLinesFields(long id);

    /**
     * Gets a list of FEMStrLines object fields.
     *
     * @param ids list of FEMStrLines object IDs
     * @return list of FEMStrLines object fields
     */
    idempotent FEMStrLinesFieldsList getFEMStrLinesFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMStrLines objects matching the given
     * reference fields.
     *
     * @param fields FEMStrLines object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMStrLines objects
     */
    idempotent IdList findEqualFEMStrLines(FEMStrLinesFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMStrLines object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMStrLines object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMStrLinesFields(FEMStrLinesFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMContactSurface object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMContactSurface(FEMContactSurfaceFields fields);

    /**
     * Adds a set of FEMContactSurface objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMContactSurfaceList(FEMContactSurfaceFieldsList fieldsList);

    /**
     * Gets the FEMContactSurface object proxy.
     *
     * @param id  FEMContactSurface object ID
     * @return FEMContactSurface object proxy
     */
    idempotent FEMContactSurface* getFEMContactSurface(long id);

    /**
     * Gets a list of all FEMContactSurface object IDs.
     *
     * @return list of FEMContactSurface object IDs
     */
    idempotent IdList getFEMContactSurfaceIds();

    /**
     * Gets a list of FEMContactSurface object proxies.
     *
     * @param ids list of FEMContactSurface object IDs
     * @return list of FEMContactSurface object proxies
     */
    idempotent FEMContactSurfaceList getFEMContactSurfaceList(IdList ids);

    /**
     * Gets the FEMContactSurface object fields.
     *
     * @param id FEMContactSurface object ID
     * @return FEMContactSurface object fields
     */
    idempotent FEMContactSurfaceFields getFEMContactSurfaceFields(long id);

    /**
     * Gets a list of FEMContactSurface object fields.
     *
     * @param ids list of FEMContactSurface object IDs
     * @return list of FEMContactSurface object fields
     */
    idempotent FEMContactSurfaceFieldsList getFEMContactSurfaceFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMContactSurface objects matching the given
     * reference fields.
     *
     * @param fields FEMContactSurface object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMContactSurface objects
     */
    idempotent IdList findEqualFEMContactSurface(FEMContactSurfaceFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMContactSurface object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMContactSurface object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMContactSurfaceFields(FEMContactSurfaceFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMMCForceData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMMCForceData(FEMMCForceDataFields fields);

    /**
     * Adds a set of FEMMCForceData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMMCForceDataList(FEMMCForceDataFieldsList fieldsList);

    /**
     * Gets the FEMMCForceData object proxy.
     *
     * @param id  FEMMCForceData object ID
     * @return FEMMCForceData object proxy
     */
    idempotent FEMMCForceData* getFEMMCForceData(long id);

    /**
     * Gets a list of all FEMMCForceData object IDs.
     *
     * @return list of FEMMCForceData object IDs
     */
    idempotent IdList getFEMMCForceDataIds();

    /**
     * Gets a list of FEMMCForceData object proxies.
     *
     * @param ids list of FEMMCForceData object IDs
     * @return list of FEMMCForceData object proxies
     */
    idempotent FEMMCForceDataList getFEMMCForceDataList(IdList ids);

    /**
     * Gets the FEMMCForceData object fields.
     *
     * @param id FEMMCForceData object ID
     * @return FEMMCForceData object fields
     */
    idempotent FEMMCForceDataFields getFEMMCForceDataFields(long id);

    /**
     * Gets a list of FEMMCForceData object fields.
     *
     * @param ids list of FEMMCForceData object IDs
     * @return list of FEMMCForceData object fields
     */
    idempotent FEMMCForceDataFieldsList getFEMMCForceDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMMCForceData objects matching the given
     * reference fields.
     *
     * @param fields FEMMCForceData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMMCForceData objects
     */
    idempotent IdList findEqualFEMMCForceData(FEMMCForceDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMMCForceData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMMCForceData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMMCForceDataFields(FEMMCForceDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSpring object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSpring(FEMSpringFields fields);

    /**
     * Adds a set of FEMSpring objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSpringList(FEMSpringFieldsList fieldsList);

    /**
     * Gets the FEMSpring object proxy.
     *
     * @param id  FEMSpring object ID
     * @return FEMSpring object proxy
     */
    idempotent FEMSpring* getFEMSpring(long id);

    /**
     * Gets a list of all FEMSpring object IDs.
     *
     * @return list of FEMSpring object IDs
     */
    idempotent IdList getFEMSpringIds();

    /**
     * Gets a list of FEMSpring object proxies.
     *
     * @param ids list of FEMSpring object IDs
     * @return list of FEMSpring object proxies
     */
    idempotent FEMSpringList getFEMSpringList(IdList ids);

    /**
     * Gets the FEMSpring object fields.
     *
     * @param id FEMSpring object ID
     * @return FEMSpring object fields
     */
    idempotent FEMSpringFields getFEMSpringFields(long id);

    /**
     * Gets a list of FEMSpring object fields.
     *
     * @param ids list of FEMSpring object IDs
     * @return list of FEMSpring object fields
     */
    idempotent FEMSpringFieldsList getFEMSpringFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSpring objects matching the given
     * reference fields.
     *
     * @param fields FEMSpring object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSpring objects
     */
    idempotent IdList findEqualFEMSpring(FEMSpringFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSpring object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSpring object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSpringFields(FEMSpringFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMSpringGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMSpringGroup(FEMSpringGroupFields fields);

    /**
     * Adds a set of FEMSpringGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMSpringGroupList(FEMSpringGroupFieldsList fieldsList);

    /**
     * Gets the FEMSpringGroup object proxy.
     *
     * @param id  FEMSpringGroup object ID
     * @return FEMSpringGroup object proxy
     */
    idempotent FEMSpringGroup* getFEMSpringGroup(long id);

    /**
     * Gets a list of all FEMSpringGroup object IDs.
     *
     * @return list of FEMSpringGroup object IDs
     */
    idempotent IdList getFEMSpringGroupIds();

    /**
     * Gets a list of FEMSpringGroup object proxies.
     *
     * @param ids list of FEMSpringGroup object IDs
     * @return list of FEMSpringGroup object proxies
     */
    idempotent FEMSpringGroupList getFEMSpringGroupList(IdList ids);

    /**
     * Gets the FEMSpringGroup object fields.
     *
     * @param id FEMSpringGroup object ID
     * @return FEMSpringGroup object fields
     */
    idempotent FEMSpringGroupFields getFEMSpringGroupFields(long id);

    /**
     * Gets a list of FEMSpringGroup object fields.
     *
     * @param ids list of FEMSpringGroup object IDs
     * @return list of FEMSpringGroup object fields
     */
    idempotent FEMSpringGroupFieldsList getFEMSpringGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMSpringGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMSpringGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMSpringGroup objects
     */
    idempotent IdList findEqualFEMSpringGroup(FEMSpringGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMSpringGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMSpringGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMSpringGroupFields(FEMSpringGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a FEMShellGroup object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addFEMShellGroup(FEMShellGroupFields fields);

    /**
     * Adds a set of FEMShellGroup objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addFEMShellGroupList(FEMShellGroupFieldsList fieldsList);

    /**
     * Gets the FEMShellGroup object proxy.
     *
     * @param id  FEMShellGroup object ID
     * @return FEMShellGroup object proxy
     */
    idempotent FEMShellGroup* getFEMShellGroup(long id);

    /**
     * Gets a list of all FEMShellGroup object IDs.
     *
     * @return list of FEMShellGroup object IDs
     */
    idempotent IdList getFEMShellGroupIds();

    /**
     * Gets a list of FEMShellGroup object proxies.
     *
     * @param ids list of FEMShellGroup object IDs
     * @return list of FEMShellGroup object proxies
     */
    idempotent FEMShellGroupList getFEMShellGroupList(IdList ids);

    /**
     * Gets the FEMShellGroup object fields.
     *
     * @param id FEMShellGroup object ID
     * @return FEMShellGroup object fields
     */
    idempotent FEMShellGroupFields getFEMShellGroupFields(long id);

    /**
     * Gets a list of FEMShellGroup object fields.
     *
     * @param ids list of FEMShellGroup object IDs
     * @return list of FEMShellGroup object fields
     */
    idempotent FEMShellGroupFieldsList getFEMShellGroupFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all FEMShellGroup objects matching the given
     * reference fields.
     *
     * @param fields FEMShellGroup object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching FEMShellGroup objects
     */
    idempotent IdList findEqualFEMShellGroup(FEMShellGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named FEMShellGroup object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields FEMShellGroup object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setFEMShellGroupFields(FEMShellGroupFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a DaqUnit object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addDaqUnit(DaqUnitFields fields);

    /**
     * Adds a set of DaqUnit objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addDaqUnitList(DaqUnitFieldsList fieldsList);

    /**
     * Gets the DaqUnit object proxy.
     *
     * @param id  DaqUnit object ID
     * @return DaqUnit object proxy
     */
    idempotent DaqUnit* getDaqUnit(long id);

    /**
     * Gets a list of all DaqUnit object IDs.
     *
     * @return list of DaqUnit object IDs
     */
    idempotent IdList getDaqUnitIds();

    /**
     * Gets a list of DaqUnit object proxies.
     *
     * @param ids list of DaqUnit object IDs
     * @return list of DaqUnit object proxies
     */
    idempotent DaqUnitList getDaqUnitList(IdList ids);

    /**
     * Gets the DaqUnit object fields.
     *
     * @param id DaqUnit object ID
     * @return DaqUnit object fields
     */
    idempotent DaqUnitFields getDaqUnitFields(long id);

    /**
     * Gets a list of DaqUnit object fields.
     *
     * @param ids list of DaqUnit object IDs
     * @return list of DaqUnit object fields
     */
    idempotent DaqUnitFieldsList getDaqUnitFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all DaqUnit objects matching the given
     * reference fields.
     *
     * @param fields DaqUnit object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching DaqUnit objects
     */
    idempotent IdList findEqualDaqUnit(DaqUnitFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named DaqUnit object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields DaqUnit object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setDaqUnitFields(DaqUnitFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a DaqUnitChannel object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addDaqUnitChannel(DaqUnitChannelFields fields);

    /**
     * Adds a set of DaqUnitChannel objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addDaqUnitChannelList(DaqUnitChannelFieldsList fieldsList);

    /**
     * Gets the DaqUnitChannel object proxy.
     *
     * @param id  DaqUnitChannel object ID
     * @return DaqUnitChannel object proxy
     */
    idempotent DaqUnitChannel* getDaqUnitChannel(long id);

    /**
     * Gets a list of all DaqUnitChannel object IDs.
     *
     * @return list of DaqUnitChannel object IDs
     */
    idempotent IdList getDaqUnitChannelIds();

    /**
     * Gets a list of DaqUnitChannel object proxies.
     *
     * @param ids list of DaqUnitChannel object IDs
     * @return list of DaqUnitChannel object proxies
     */
    idempotent DaqUnitChannelList getDaqUnitChannelList(IdList ids);

    /**
     * Gets the DaqUnitChannel object fields.
     *
     * @param id DaqUnitChannel object ID
     * @return DaqUnitChannel object fields
     */
    idempotent DaqUnitChannelFields getDaqUnitChannelFields(long id);

    /**
     * Gets a list of DaqUnitChannel object fields.
     *
     * @param ids list of DaqUnitChannel object IDs
     * @return list of DaqUnitChannel object fields
     */
    idempotent DaqUnitChannelFieldsList getDaqUnitChannelFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all DaqUnitChannel objects matching the given
     * reference fields.
     *
     * @param fields DaqUnitChannel object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching DaqUnitChannel objects
     */
    idempotent IdList findEqualDaqUnitChannel(DaqUnitChannelFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named DaqUnitChannel object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields DaqUnitChannel object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setDaqUnitChannelFields(DaqUnitChannelFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a Sensor object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addSensor(SensorFields fields);

    /**
     * Adds a set of Sensor objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addSensorList(SensorFieldsList fieldsList);

    /**
     * Gets the Sensor object proxy.
     *
     * @param id  Sensor object ID
     * @return Sensor object proxy
     */
    idempotent Sensor* getSensor(long id);

    /**
     * Gets a list of all Sensor object IDs.
     *
     * @return list of Sensor object IDs
     */
    idempotent IdList getSensorIds();

    /**
     * Gets a list of Sensor object proxies.
     *
     * @param ids list of Sensor object IDs
     * @return list of Sensor object proxies
     */
    idempotent SensorList getSensorList(IdList ids);

    /**
     * Gets the Sensor object fields.
     *
     * @param id Sensor object ID
     * @return Sensor object fields
     */
    idempotent SensorFields getSensorFields(long id);

    /**
     * Gets a list of Sensor object fields.
     *
     * @param ids list of Sensor object IDs
     * @return list of Sensor object fields
     */
    idempotent SensorFieldsList getSensorFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all Sensor objects matching the given
     * reference fields.
     *
     * @param fields Sensor object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching Sensor objects
     */
    idempotent IdList findEqualSensor(SensorFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named Sensor object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields Sensor object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setSensorFields(SensorFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a Transducer object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addTransducer(TransducerFields fields);

    /**
     * Adds a set of Transducer objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addTransducerList(TransducerFieldsList fieldsList);

    /**
     * Gets the Transducer object proxy.
     *
     * @param id  Transducer object ID
     * @return Transducer object proxy
     */
    idempotent Transducer* getTransducer(long id);

    /**
     * Gets a list of all Transducer object IDs.
     *
     * @return list of Transducer object IDs
     */
    idempotent IdList getTransducerIds();

    /**
     * Gets a list of Transducer object proxies.
     *
     * @param ids list of Transducer object IDs
     * @return list of Transducer object proxies
     */
    idempotent TransducerList getTransducerList(IdList ids);

    /**
     * Gets the Transducer object fields.
     *
     * @param id Transducer object ID
     * @return Transducer object fields
     */
    idempotent TransducerFields getTransducerFields(long id);

    /**
     * Gets a list of Transducer object fields.
     *
     * @param ids list of Transducer object IDs
     * @return list of Transducer object fields
     */
    idempotent TransducerFieldsList getTransducerFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all Transducer objects matching the given
     * reference fields.
     *
     * @param fields Transducer object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching Transducer objects
     */
    idempotent IdList findEqualTransducer(TransducerFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named Transducer object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields Transducer object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setTransducerFields(TransducerFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a Experiment object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addExperiment(ExperimentFields fields);

    /**
     * Adds a set of Experiment objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addExperimentList(ExperimentFieldsList fieldsList);

    /**
     * Gets the Experiment object proxy.
     *
     * @param id  Experiment object ID
     * @return Experiment object proxy
     */
    idempotent Experiment* getExperiment(long id);

    /**
     * Gets a list of all Experiment object IDs.
     *
     * @return list of Experiment object IDs
     */
    idempotent IdList getExperimentIds();

    /**
     * Gets a list of Experiment object proxies.
     *
     * @param ids list of Experiment object IDs
     * @return list of Experiment object proxies
     */
    idempotent ExperimentList getExperimentList(IdList ids);

    /**
     * Gets the Experiment object fields.
     *
     * @param id Experiment object ID
     * @return Experiment object fields
     */
    idempotent ExperimentFields getExperimentFields(long id);

    /**
     * Gets a list of Experiment object fields.
     *
     * @param ids list of Experiment object IDs
     * @return list of Experiment object fields
     */
    idempotent ExperimentFieldsList getExperimentFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all Experiment objects matching the given
     * reference fields.
     *
     * @param fields Experiment object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching Experiment objects
     */
    idempotent IdList findEqualExperiment(ExperimentFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named Experiment object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields Experiment object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setExperimentFields(ExperimentFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a Cycle object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addCycle(CycleFields fields);

    /**
     * Adds a set of Cycle objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addCycleList(CycleFieldsList fieldsList);

    /**
     * Gets the Cycle object proxy.
     *
     * @param id  Cycle object ID
     * @return Cycle object proxy
     */
    idempotent Cycle* getCycle(long id);

    /**
     * Gets a list of all Cycle object IDs.
     *
     * @return list of Cycle object IDs
     */
    idempotent IdList getCycleIds();

    /**
     * Gets a list of Cycle object proxies.
     *
     * @param ids list of Cycle object IDs
     * @return list of Cycle object proxies
     */
    idempotent CycleList getCycleList(IdList ids);

    /**
     * Gets the Cycle object fields.
     *
     * @param id Cycle object ID
     * @return Cycle object fields
     */
    idempotent CycleFields getCycleFields(long id);

    /**
     * Gets a list of Cycle object fields.
     *
     * @param ids list of Cycle object IDs
     * @return list of Cycle object fields
     */
    idempotent CycleFieldsList getCycleFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all Cycle objects matching the given
     * reference fields.
     *
     * @param fields Cycle object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching Cycle objects
     */
    idempotent IdList findEqualCycle(CycleFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named Cycle object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields Cycle object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setCycleFields(CycleFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a SensorTimeData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addSensorTimeData(SensorTimeDataFields fields);

    /**
     * Adds a set of SensorTimeData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addSensorTimeDataList(SensorTimeDataFieldsList fieldsList);

    /**
     * Gets the SensorTimeData object proxy.
     *
     * @param id  SensorTimeData object ID
     * @return SensorTimeData object proxy
     */
    idempotent SensorTimeData* getSensorTimeData(long id);

    /**
     * Gets a list of all SensorTimeData object IDs.
     *
     * @return list of SensorTimeData object IDs
     */
    idempotent IdList getSensorTimeDataIds();

    /**
     * Gets a list of SensorTimeData object proxies.
     *
     * @param ids list of SensorTimeData object IDs
     * @return list of SensorTimeData object proxies
     */
    idempotent SensorTimeDataList getSensorTimeDataList(IdList ids);

    /**
     * Gets the SensorTimeData object fields.
     *
     * @param id SensorTimeData object ID
     * @return SensorTimeData object fields
     */
    idempotent SensorTimeDataFields getSensorTimeDataFields(long id);

    /**
     * Gets a list of SensorTimeData object fields.
     *
     * @param ids list of SensorTimeData object IDs
     * @return list of SensorTimeData object fields
     */
    idempotent SensorTimeDataFieldsList getSensorTimeDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all SensorTimeData objects matching the given
     * reference fields.
     *
     * @param fields SensorTimeData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching SensorTimeData objects
     */
    idempotent IdList findEqualSensorTimeData(SensorTimeDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named SensorTimeData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields SensorTimeData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setSensorTimeDataFields(SensorTimeDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a TransducerTimeData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addTransducerTimeData(TransducerTimeDataFields fields);

    /**
     * Adds a set of TransducerTimeData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addTransducerTimeDataList(TransducerTimeDataFieldsList fieldsList);

    /**
     * Gets the TransducerTimeData object proxy.
     *
     * @param id  TransducerTimeData object ID
     * @return TransducerTimeData object proxy
     */
    idempotent TransducerTimeData* getTransducerTimeData(long id);

    /**
     * Gets a list of all TransducerTimeData object IDs.
     *
     * @return list of TransducerTimeData object IDs
     */
    idempotent IdList getTransducerTimeDataIds();

    /**
     * Gets a list of TransducerTimeData object proxies.
     *
     * @param ids list of TransducerTimeData object IDs
     * @return list of TransducerTimeData object proxies
     */
    idempotent TransducerTimeDataList getTransducerTimeDataList(IdList ids);

    /**
     * Gets the TransducerTimeData object fields.
     *
     * @param id TransducerTimeData object ID
     * @return TransducerTimeData object fields
     */
    idempotent TransducerTimeDataFields getTransducerTimeDataFields(long id);

    /**
     * Gets a list of TransducerTimeData object fields.
     *
     * @param ids list of TransducerTimeData object IDs
     * @return list of TransducerTimeData object fields
     */
    idempotent TransducerTimeDataFieldsList getTransducerTimeDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all TransducerTimeData objects matching the given
     * reference fields.
     *
     * @param fields TransducerTimeData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching TransducerTimeData objects
     */
    idempotent IdList findEqualTransducerTimeData(TransducerTimeDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named TransducerTimeData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields TransducerTimeData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setTransducerTimeDataFields(TransducerTimeDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a ExperimentStructureData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addExperimentStructureData(ExperimentStructureDataFields fields);

    /**
     * Adds a set of ExperimentStructureData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addExperimentStructureDataList(ExperimentStructureDataFieldsList fieldsList);

    /**
     * Gets the ExperimentStructureData object proxy.
     *
     * @param id  ExperimentStructureData object ID
     * @return ExperimentStructureData object proxy
     */
    idempotent ExperimentStructureData* getExperimentStructureData(long id);

    /**
     * Gets a list of all ExperimentStructureData object IDs.
     *
     * @return list of ExperimentStructureData object IDs
     */
    idempotent IdList getExperimentStructureDataIds();

    /**
     * Gets a list of ExperimentStructureData object proxies.
     *
     * @param ids list of ExperimentStructureData object IDs
     * @return list of ExperimentStructureData object proxies
     */
    idempotent ExperimentStructureDataList getExperimentStructureDataList(IdList ids);

    /**
     * Gets the ExperimentStructureData object fields.
     *
     * @param id ExperimentStructureData object ID
     * @return ExperimentStructureData object fields
     */
    idempotent ExperimentStructureDataFields getExperimentStructureDataFields(long id);

    /**
     * Gets a list of ExperimentStructureData object fields.
     *
     * @param ids list of ExperimentStructureData object IDs
     * @return list of ExperimentStructureData object fields
     */
    idempotent ExperimentStructureDataFieldsList getExperimentStructureDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all ExperimentStructureData objects matching the given
     * reference fields.
     *
     * @param fields ExperimentStructureData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching ExperimentStructureData objects
     */
    idempotent IdList findEqualExperimentStructureData(ExperimentStructureDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named ExperimentStructureData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields ExperimentStructureData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setExperimentStructureDataFields(ExperimentStructureDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a TransducerCycleData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addTransducerCycleData(TransducerCycleDataFields fields);

    /**
     * Adds a set of TransducerCycleData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addTransducerCycleDataList(TransducerCycleDataFieldsList fieldsList);

    /**
     * Gets the TransducerCycleData object proxy.
     *
     * @param id  TransducerCycleData object ID
     * @return TransducerCycleData object proxy
     */
    idempotent TransducerCycleData* getTransducerCycleData(long id);

    /**
     * Gets a list of all TransducerCycleData object IDs.
     *
     * @return list of TransducerCycleData object IDs
     */
    idempotent IdList getTransducerCycleDataIds();

    /**
     * Gets a list of TransducerCycleData object proxies.
     *
     * @param ids list of TransducerCycleData object IDs
     * @return list of TransducerCycleData object proxies
     */
    idempotent TransducerCycleDataList getTransducerCycleDataList(IdList ids);

    /**
     * Gets the TransducerCycleData object fields.
     *
     * @param id TransducerCycleData object ID
     * @return TransducerCycleData object fields
     */
    idempotent TransducerCycleDataFields getTransducerCycleDataFields(long id);

    /**
     * Gets a list of TransducerCycleData object fields.
     *
     * @param ids list of TransducerCycleData object IDs
     * @return list of TransducerCycleData object fields
     */
    idempotent TransducerCycleDataFieldsList getTransducerCycleDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all TransducerCycleData objects matching the given
     * reference fields.
     *
     * @param fields TransducerCycleData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching TransducerCycleData objects
     */
    idempotent IdList findEqualTransducerCycleData(TransducerCycleDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named TransducerCycleData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields TransducerCycleData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setTransducerCycleDataFields(TransducerCycleDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a MappingMatrix object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addMappingMatrix(MappingMatrixFields fields);

    /**
     * Adds a set of MappingMatrix objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addMappingMatrixList(MappingMatrixFieldsList fieldsList);

    /**
     * Gets the MappingMatrix object proxy.
     *
     * @param id  MappingMatrix object ID
     * @return MappingMatrix object proxy
     */
    idempotent MappingMatrix* getMappingMatrix(long id);

    /**
     * Gets a list of all MappingMatrix object IDs.
     *
     * @return list of MappingMatrix object IDs
     */
    idempotent IdList getMappingMatrixIds();

    /**
     * Gets a list of MappingMatrix object proxies.
     *
     * @param ids list of MappingMatrix object IDs
     * @return list of MappingMatrix object proxies
     */
    idempotent MappingMatrixList getMappingMatrixList(IdList ids);

    /**
     * Gets the MappingMatrix object fields.
     *
     * @param id MappingMatrix object ID
     * @return MappingMatrix object fields
     */
    idempotent MappingMatrixFields getMappingMatrixFields(long id);

    /**
     * Gets a list of MappingMatrix object fields.
     *
     * @param ids list of MappingMatrix object IDs
     * @return list of MappingMatrix object fields
     */
    idempotent MappingMatrixFieldsList getMappingMatrixFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all MappingMatrix objects matching the given
     * reference fields.
     *
     * @param fields MappingMatrix object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching MappingMatrix objects
     */
    idempotent IdList findEqualMappingMatrix(MappingMatrixFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named MappingMatrix object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields MappingMatrix object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setMappingMatrixFields(MappingMatrixFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a DaqUnitChannelData object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addDaqUnitChannelData(DaqUnitChannelDataFields fields);

    /**
     * Adds a set of DaqUnitChannelData objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addDaqUnitChannelDataList(DaqUnitChannelDataFieldsList fieldsList);

    /**
     * Gets the DaqUnitChannelData object proxy.
     *
     * @param id  DaqUnitChannelData object ID
     * @return DaqUnitChannelData object proxy
     */
    idempotent DaqUnitChannelData* getDaqUnitChannelData(long id);

    /**
     * Gets a list of all DaqUnitChannelData object IDs.
     *
     * @return list of DaqUnitChannelData object IDs
     */
    idempotent IdList getDaqUnitChannelDataIds();

    /**
     * Gets a list of DaqUnitChannelData object proxies.
     *
     * @param ids list of DaqUnitChannelData object IDs
     * @return list of DaqUnitChannelData object proxies
     */
    idempotent DaqUnitChannelDataList getDaqUnitChannelDataList(IdList ids);

    /**
     * Gets the DaqUnitChannelData object fields.
     *
     * @param id DaqUnitChannelData object ID
     * @return DaqUnitChannelData object fields
     */
    idempotent DaqUnitChannelDataFields getDaqUnitChannelDataFields(long id);

    /**
     * Gets a list of DaqUnitChannelData object fields.
     *
     * @param ids list of DaqUnitChannelData object IDs
     * @return list of DaqUnitChannelData object fields
     */
    idempotent DaqUnitChannelDataFieldsList getDaqUnitChannelDataFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all DaqUnitChannelData objects matching the given
     * reference fields.
     *
     * @param fields DaqUnitChannelData object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching DaqUnitChannelData objects
     */
    idempotent IdList findEqualDaqUnitChannelData(DaqUnitChannelDataFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named DaqUnitChannelData object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields DaqUnitChannelData object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setDaqUnitChannelDataFields(DaqUnitChannelDataFields fields,
        FieldNameList fieldNames);

    /**
     * Adds a AccelMeas object to the store.
     *
     * @param fields object fields
     * @return newly assigned ID
     */
    long addAccelMeas(AccelMeasFields fields);

    /**
     * Adds a set of AccelMeas objects to the store.
     *
     * @param fieldsList list of object fields
     * @return list of newly assigned IDs
     */
    IdList addAccelMeasList(AccelMeasFieldsList fieldsList);

    /**
     * Gets the AccelMeas object proxy.
     *
     * @param id  AccelMeas object ID
     * @return AccelMeas object proxy
     */
    idempotent AccelMeas* getAccelMeas(long id);

    /**
     * Gets a list of all AccelMeas object IDs.
     *
     * @return list of AccelMeas object IDs
     */
    idempotent IdList getAccelMeasIds();

    /**
     * Gets a list of AccelMeas object proxies.
     *
     * @param ids list of AccelMeas object IDs
     * @return list of AccelMeas object proxies
     */
    idempotent AccelMeasList getAccelMeasList(IdList ids);

    /**
     * Gets the AccelMeas object fields.
     *
     * @param id AccelMeas object ID
     * @return AccelMeas object fields
     */
    idempotent AccelMeasFields getAccelMeasFields(long id);

    /**
     * Gets a list of AccelMeas object fields.
     *
     * @param ids list of AccelMeas object IDs
     * @return list of AccelMeas object fields
     */
    idempotent AccelMeasFieldsList getAccelMeasFieldsList(IdList ids);

    /**
     * Gets a list of IDs of all AccelMeas objects matching the given
     * reference fields.
     *
     * @param fields AccelMeas object fields to compare to
     * @param fieldNames list of names of fields to compare (empty is all)
     * @return list of IDs of matching AccelMeas objects
     */
    idempotent IdList findEqualAccelMeas(AccelMeasFields fields,
        FieldNameList fieldNames);

    /**
     * Updates the named AccelMeas object fields.
     *
     * The 'fields' variable must have a valid id field. 
     *
     * @param fields AccelMeas object fields 
     * @param fieldNames list of names of fields to update (empty is all)
     */
    idempotent void setAccelMeasFields(AccelMeasFields fields,
        FieldNameList fieldNames);
  };
  
  /**
   * Session interface.
   *
   * @see SessionFactory
   */
  interface Session {
    /**
     * Gets the SenStore manager.
     * @return [SenStoreMngr] proxy
     */
    SenStoreMngr* getManager();
    /**
     * Gets the name of this session.
     *
     * @return session name
     */
    idempotent string getName();
    /**
     * Gets the session time-out setting.
     *
     * @return session time-out [s]
     * @see refresh()
     */
    idempotent float getTimeout();
    /**
     * Destroys this session.
     *
     * Call this method when the session is finished, for a speedy release
     * of resources on the server.
     */
    void destroy();
    /**
     * Refreshes this session, resetting the time-out counter on the server.
     *
     * This session must be refreshed at an interval shorter than the
     * time-out value to stay alive.  Otherwise the server may destroy
     * the session, and the session will no longer be valid.
     */
    idempotent void refresh();
  };
  
  /**
   * Session factory singleton.
   *
   * Use this interface to create a session, needed to interface with the
   * SenStore database.
   */
  interface SessionFactory {
    /**
     * Creates a normal session, allowing access by multiple clients.
     *
     * @param name session name
     * @return [Session] proxy
     */
    Session* createSession(string name);
    /**
     * Creates an exclusive-access session, temporarily blocking access by
     * other clients.
     *
     * @param name session name
     * @return [Session] proxy
     */
    Session* createSessionExclusive(string name);
  };
};
