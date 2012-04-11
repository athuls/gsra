# Copyright (c) 2010 SC Solutions, Inc.
# See LICENCE.TXT for licensing terms.
# $Id: dbdef.py,v 1.5 2010/07/27 00:10:02 glinden Exp $
import logging
from yeanpypa import *

## @package buildtools.dbdef
#
# This package provides a tool for converting a database definition
# (using the domain specific database definition language) to an
# in-memory variable definition tree.
#
# The database definition language resembles C++, and allows the
# definition of enumerations and classes.  <b>Enumerations</b> allow
# the definition of database fields that have a predefined number of
# options.  The documentation strings (inspired by JavaDoc / Doxygen)
# are scanned as well, and can be used in the template.
#
# \verbatim
# enum SomeName {
#  /** Documentation string goes here. */
#  LABEL1,
#  /** Documentation string goes here. */
#  LABEL2
# };\endverbatim
#
# <b>Classes</b> allow the definition of objects that contain
# variables and references to other objects.
#
# \verbatim
# /** Some base class. */
# class SomeBaseClass {
#   /** Some integer variable (to be stored in HDF5). */
#   @hdf int32 count;
# };
#
# /** Some derived class. */
# class SomeClass(SomeBaseClass) {
#   /** Some string variable. */
#   string name;
# };
#
# /** Some other class. */
# class SomeOtherClass {
#   /** A link to an object of another class. */
#   SomeClass *buddy;
# };\endverbatim
#
# The database code generation will automatically add an id variable
# to each base class, and use that in uniquely identifying an object
# (a primary key in SQL), and to link to other objects (a foreign key
# in SQL).  The variable types can be anything in theory, but must be
# supported by the template and any type mapping function.
# Reference variables are set as <tt>'class' = 'ref'</tt>, and normal
# variable as <tt>'class' = 'var'</tt>, which can be used in the
# template to differentiate between them.  Members can be tagged to
# indicate where they will be stored.  Currently all regular fields are
# stored in the SQL database by default, unless they are tagged by
# '\@hdf', in which case they are stored in HDF5.
#
# Members can also be tagged with '\@parent' to indicate that the member
# refers to a parent object.  This can be used in the code generation
# to generate methods for getting the children of parent object types.
# Note that this tag is only relevant for object link fields.
#
# <b>Arrays</b> allow the definition of multi-dimensional arrays
# that map variables, <i>e.g.</i> the force-to-displacement mapping
# (elasticity).  The array data itself is stored in HDF5.
#
# \verbatim
# /** Linear mapping between two node-related quantities. */
# array NodeMapping(float64) {
#   /** Parent structure */
#   Structure *struct;
#   /** Short name */
#   string name;
#   /** Description */
#   string description;
#   /** Output quantity */
#   Quantity *outputQuantity;
#   /** Input quantity */
#   Quantity *inputQuantity;
#   axes {
#     /** Output node */
#     Node *outputNode;
#     /** Input node */
#     Node *inputNode;
#   };
# };\endverbatim
#
# <b>Signals</b> allow the definition of multi-dimensional arrays with
# an additional time dimension, <i>e.g.</i> strain measurements over
# time.  The signal data itself is stored in HDF5.
#
# \verbatim
# /** Strain sensor readings. */
# signal StrainMeasurement(float32) {
#   /** Parent structure */
#   Structure *struct;
#   /** Short name */
#   string name;
#   /** Description */
#   string description;
#   /** Output quantity */
#   axes {
#     /** Sensor. */
#     Sensor *sensor;
#   };
# };\endverbatim


## Dictionary class for holding variables.
#
# This class can be used to allow conditional expressions in
# templates, such as "p.name" rather than "p['name']".
#
# Note that the constructor is not overridden, so an instance can be
# created using constructs such as:
#
# <tt>vars = Vars({'name':'Some name'})</tt>
class Vars(dict):
    def __getattr__(self, key):
        return self[key]
    def __setattr__(self, key, value):
        self[key] = value
    def addGroup(self, key, value=None):
        if value == None:
            value = []
        self[key] = value
        return value
    def setParent(self):
        for items in self.values():
            # Walk variable tree recursively 
            if isinstance(items, list):
                for item in items:
                    if isinstance(item, Vars):
                        item.__parent = self
                        item.setParent()
    def setRoot(self, root = None):
        if root == None:
            # I am root variable
            root = self
        self.__root = root
        for items in self.values():
            # Walk variable tree recursively 
            if isinstance(items, list):
                for item in items:
                    if isinstance(item, Vars):
                        item.setRoot(root)
    def find(self, group, key, value):
        return find(self[group], key, value)
    def root(self):
        try:
            return self.__root
        except:
            return None
    def parent(self):
        try:
            return self.__parent
        except:
            return None


## Variable data set for the storing database definition.
class DatabaseVars(Vars):
    ## Creates the data vars object.
    #
    # @param tokens list of tokens as output by the parser
    def __init__(self, tokens):
        Vars.__init__(self)
        enumDefs      = self.addGroup('enumDefs')
        classDefs     = self.addGroup('classDefs')
        arrayDefs     = self.addGroup('arrayDefs')
        signalDefs     = self.addGroup('signalDefs')
        docString = ''
        for token in tokens:
            if isinstance(token, DocString):
                docString = token.text
                continue
            if isinstance(token, Comment):
                continue
            if isinstance(token, EnumDef):
                token.update()
                enumDefs.append(token)
            elif isinstance(token, ClassDef):
                token.update()
                classDefs.append(token)
            elif isinstance(token, SignalDef):
                token.update()
                signalDefs.append(token)
            elif isinstance(token, ArrayDef):
                token.update()
                arrayDefs.append(token)
            token.descr = docString
            docString = ''
        self.cleanup()
    ## Removes temporary variables.
    def cleanup(self):
        for child in self.values():
            if isinstance(child, SyntaxObject):
                child.cleanup()
            if isinstance(child, list):
                for grandchild in child:
                    grandchild.cleanup()


## Abstract base class for a syntax object.
#
# A syntax object captures a (possibly complex) token in a template
# file.
class SyntaxObject(Vars):
    ## Initializes internal variables.
    def __init__(self, match, pos):
        Vars.__init__(self)
        self.match = match
        self.pos = pos
        self.descr = ''
    ## Removes temporary variables.
    def cleanup(self):
        if self.has_key('pos'):
            del self['pos']
        if self.has_key('match'):
            del self['match']
        for child in self.values():
            if isinstance(child, SyntaxObject):
                child.cleanup()
            if isinstance(child, list):
                for grandchild in child:
                    grandchild.cleanup()

    ## Shows a short description of this object.
    def __repr__(self):
        return '<%s %s>' % (self.__class__.__name__, dict.__repr__(self))


## Comment syntax element object class.
class Comment(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.text = match[0]


## Document string syntax element object class.
class DocString(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.text = match[0]


## Enumeration name syntax element object class.
class EnumName(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.name = match[0]


## Class name syntax element object class.
class ClassName(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.name = match[0]


## Base class name syntax element object class.
class BaseClassName(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.name = match[0]


## Tag name syntax element object class.
class TagName(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.name = match[0]


## Variable name syntax element object class.
class VarName(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.name = match[0]


## Object reference name syntax element object class.
class RefName(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.name = match[0]


## Variable or reference type syntax element object class.
class TypeName(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)
        self.name = match[0]


## Variable definition syntax element object class.
class VarDef(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) in [2,3])

    ## Updates the variable properties.
    def update(self):
        self.className = 'var'
        n = len(self.match)
        self.tags = {}
        if n == 3:
            # Process tags
            for tag in self.match[0]:
                self.tags[tag.name] = True
        self.dtype = self.match[-2].name
        self.name = self.match[-1].name


## Object reference definition syntax element object class.
class RefDef(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) in [2,3])

    ## Updates the variable properties.
    def update(self):
        self.tags = {}
        self.className = 'ref'
        n = len(self.match)
        self.tags = {}
        if n == 3:
            # Process tags
            for tag in self.match[0]:
                self.tags[tag.name] = True
        self.dtype = self.match[-2].name
        self.name = self.match[-1].name


## Class definition syntax element object class.
class ClassDef(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) >= 1)

    ## Updates the variable properties.
    def update(self):
        self.name = self.match[0].name
        baseClassName = None
        if len(self.match) > 1:
            # Get optional base class name
            if isinstance(self.match[1][0], BaseClassName):
                baseClassName = self.match[1][0].name
                if len(self.match) > 2:
                    items = self.match[2]
                else:
                    items = []
            else:
                items = self.match[1]
        else:
            items = []
        self.baseclass = baseClassName
        self.members = self.addGroup('members')
        docString = ''
        for item in items:
            if isinstance(item, Comment):
                continue
            if isinstance(item, DocString):
                docString = item.text
                continue
            if isinstance(item, VarDef) or isinstance(item, RefDef):
                item.descr = docString
                item.update()
                self.members.append(item)
            docString = ''


## Enumeration element definition syntax element object class.
class EnumElem(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 1)

    ## Updates the variable properties.
    def update(self):
        self.name = self.match[0].name


## Enumeration definition syntax element object class.
class EnumDef(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 2)

    ## Updates the variable properties.
    def update(self):
        self.name = self.match[0].name
        self.members = self.addGroup('members')
        docString = ''
        for item in self.match[1]:
            if isinstance(item, Comment):
                continue
            if isinstance(item, DocString):
                docString = item.text
                continue
            if isinstance(item, EnumElem):
                item.update()
                item.descr = docString
                self.members.append(item)
            docString = ''


## Array definition syntax element object class.
class ArrayDef(SyntaxObject):
    ## Initializes the internal variables.
    def __init__(self, match, pos):
        SyntaxObject.__init__(self, match, pos)
        assert(len(match) == 4)
        # Arrays don't support base classes
        self.baseclass = None

    ## Updates the variable properties.
    def update(self):
        self.name = self.match[0].name
        self.dtype = self.match[1].name
        self.members = self.addGroup('members')
        self.axes = self.addGroup('axes')
        docString = ''
        for item in self.match[2]:
            if isinstance(item, Comment):
                continue
            if isinstance(item, DocString):
                docString = item.text
                continue
            if isinstance(item, VarDef) or isinstance(item, RefDef):
                item.descr = docString
                item.update()
                self.members.append(item)
            docString = ''
        docString = ''
        for item in self.match[3]:
            if isinstance(item, Comment):
                continue
            if isinstance(item, DocString):
                docString = item.text
                continue
            if isinstance(item, VarDef) or isinstance(item, RefDef):
                item.descr = docString
                item.update()
                self.axes.append(item)
            docString = ''


## Signal definition syntax element object class.
class SignalDef(ArrayDef):
    pass


## Builds the Database-definition language syntax.
def getSyntax():
    digit       = AnyOf('0123456789')
    lowercase   = AnyOf('abcdefghijklmnopqrstuvwxyz')
    uppercase   = AnyOf('ABCDEFGHIJKLMNOPQRSTUVWXYZ')
    white       = ZeroOrMore(AnyOf('\n\r\t '))

    word = Word(lowercase | uppercase | digit)

    enumName       = Combine(word)
    enumName.setAction(lambda x,y: EnumName(x,y))
    className      = Combine(word)
    className.setAction(lambda x,y: ClassName(x,y))
    baseClassName  = Combine(word)
    baseClassName.setAction(lambda x,y: BaseClassName(x,y))
    tagName     = Literal('@').hide() + Combine(word)
    tagName.setAction(lambda x,y: TagName(x,y))
    varName        = Combine(word)
    varName.setAction(lambda x,y: VarName(x,y))
    refName        = Combine(word)
    refName.setAction(lambda x,y: RefName(x,y))
    typeName       = Combine(word)
    typeName.setAction(lambda x,y: TypeName(x,y))

    docString = Combine(
        Literal('/**').hide() + MatchWhite(ZeroOrMore(
            OneOrMore(NoneOf('*')) |
            OneOrMore(Literal('*') + NoneOf('/'))
        )) + Literal('*/').hide())
    docString.setAction(lambda x,y: DocString(x,y))

    comment = Combine(
        Literal('/*').hide() + MatchWhite(ZeroOrMore(
            OneOrMore(NoneOf('*')) |
            OneOrMore(Literal('*') + NoneOf('/'))
        )) + Literal('*/').hide())
    comment.setAction(lambda x,y: Comment(x,y))

    varDef = ZeroOrMore(tagName) + typeName + varName + Literal(';').hide()
    varDef.setAction(lambda x,y: VarDef(x,y))

    refDef = ZeroOrMore(tagName) + typeName + Literal('*').hide() + varName + Literal(';').hide()
    refDef.setAction(lambda x,y: RefDef(x,y))

    enumElem = typeName + Optional(Literal(',')).hide()
    enumElem.setAction(lambda x,y: EnumElem(x,y))

    enumDef = Literal('enum').hide() + enumName + Literal('{').hide() + \
        ZeroOrMore(enumElem | docString | comment) + \
        Literal('}').hide() + Literal(';').hide()
    enumDef.setAction(lambda x,y: EnumDef(x,y))

    classDef = Literal('class').hide() + className + \
        Optional(Literal('(').hide() + baseClassName + Literal(')').hide()) + \
        Literal('{').hide() + \
        ZeroOrMore(varDef | refDef | docString | comment) + \
        Literal('}').hide() + Literal(';').hide()
    classDef.setAction(lambda x,y: ClassDef(x,y))

    arrayDef = Literal('array').hide() + className + \
        Literal('(').hide() + typeName + Literal(')').hide() + \
        Literal('{').hide() + \
        ZeroOrMore(varDef | refDef | docString | comment) + \
        Literal('axes').hide() + Literal('{').hide() + \
        ZeroOrMore(varDef | refDef | docString | comment) + \
        Literal('}').hide() + Literal(';').hide() + \
        Literal('}').hide() + Literal(';').hide()
    arrayDef.setAction(lambda x,y: ArrayDef(x,y))

    signalDef = Literal('signal').hide() + className + \
        Literal('(').hide() + typeName + Literal(')').hide() + \
        Literal('{').hide() + \
        ZeroOrMore(varDef | refDef | docString | comment) + \
        Literal('axes').hide() + Literal('{').hide() + \
        ZeroOrMore(varDef | refDef | docString | comment) + \
        Literal('}').hide() + Literal(';').hide() + \
        Literal('}').hide() + Literal(';').hide()
    signalDef.setAction(lambda x,y: SignalDef(x,y))

    syntax = ZeroOrMore(docString | comment | varDef | enumDef | 
                        classDef | arrayDef | signalDef )
    return syntax


## Pre-build syntax
_syntax = getSyntax()


## Parses the database definition and returns the corresponding
# variable tree definition.
#
# @param dbdFilename database file name (input)
# @return variable tree definition
# @sa Vars
# @sa read()
def process(dbdFilename):
    # Parse database definition
    logging.info('Parsing database definition: %s' % dbdFilename)
    f = open(dbdFilename, 'rt')
    text = f.read()
    f.close()
    result = parse(_syntax, text, ignore_white=True)
    if result.full():
        tokens = result.getTokens()
    else:
        print '...', str(result.getTokens())[-100:]
        raise RuntimeError('Unexpected input')
    vars = DatabaseVars(tokens)
    #
    # Inspect variable tree to determine additional settings
    #
    enums = [enumDef.name for enumDef in vars.enumDefs]
    for enumDef in vars.enumDefs:
        enumDef.sqlName = 'ENUM_' + enumDef.name
    vars.hdfClassDefs = []
    for classDef in vars.classDefs:
        classDef.sqlName = 'CLASS_' + classDef.name
        inHdf = False
        # Update references to enumerations
        for member in classDef.members:
            if member.className == 'ref'  and member.dtype in enums:
                member.className = 'enumref'
        # Check for HDF5 / SQL members of class definitions
        classDef.hdfMembers = []
        classDef.sqlMembers = []
        for member in classDef.members:
            if 'hdf' in member.tags:
                classDef.hdfMembers.append(member)
                inHdf = True
            else:
                if member.className in ['ref', 'enumref']:
                    member.sqlName = 'ref_' + member.name
                else:
                    member.sqlName = 'var_' + member.name
                classDef.sqlMembers.append(member)
        if inHdf:
            # Keep track classes that have HDF5 fields
            vars.hdfClassDefs.append(classDef)
    for arrayDef in vars.arrayDefs:
        arrayDef.sqlName = 'CLASS_' + arrayDef.name
        # Update references to enumerations
        for member in arrayDef.members:
            if member.className == 'ref'  and member.dtype in enums:
                member.className = 'enumref'
        for axis in arrayDef.axes:
            if axis.className == 'ref'  and axis.dtype in enums:
                axis.className = 'enumref'
        # Check for HDF5 / SQL members of array definitions.  Note
        # that array definitions always have HDF5 data (the array
        # itself)
        arrayDef.hdfMembers = []
        arrayDef.sqlMembers = []
        for member in arrayDef.members:
            if 'hdf' in member.tags:
                arrayDef.hdfMembers.append(member)
            else:
                if member.className in ['ref', 'enumref']:
                    member.sqlName = 'ref_' + member.name
                else:
                    member.sqlName = 'var_' + member.name
                arrayDef.sqlMembers.append(member)
    for signalDef in vars.signalDefs:
        signalDef.sqlName = 'CLASS_' + signalDef.name
        # Update references to enumerations
        for member in signalDef.members:
            if member.className == 'ref'  and member.dtype in enums:
                member.className = 'enumref'
        for axis in signalDef.axes:
            if axis.className == 'ref'  and axis.dtype in enums:
                axis.className = 'enumref'
        # Check for HDF5 / SQL members of signal definitions.  Note
        # that signal definitions always have HDF5 data (the signal
        # itself)
        signalDef.hdfMembers = []
        signalDef.sqlMembers = []
        for member in signalDef.members:
            if 'hdf' in member.tags:
                signalDef.hdfMembers.append(member)
            else:
                if member.className in ['ref', 'enumref']:
                    member.sqlName = 'ref_' + member.name
                else:
                    member.sqlName = 'var_' + member.name
                signalDef.sqlMembers.append(member)
                
    return vars


## Parses the variable definition file and adds internal variables.
#
# The resulting variable definition contains all the database variables and
# code generatation variables.
# The variable definition is a tree of Python Vars() objects and Python
# lists.  The Vars() objects act like dictionaries, with added functionality
# of accessing keys as members: <code>vars.enumDefs</code> is a short-hand for
# <code>vars['enumDefs']</code>.  For example, looping over all enumeration definitions
# can be done as follows:
#
# \verbatim
# for enumDef in vars.enumDefs:
#   print enumDef.sqlName
#   for member in enumDef.members:
#      print '  ' + member.name\endverbatim
#
# The variable definition contains, among others, the following data:
#
# Database definition lists:
# - \c vars.enumDefs: list of database enumerations
# - \c vars.classDefs: list of database classes
# - \c vars.arrayDefs: list of database arrays
# - \c vars.signalDefs: list of database signals
#
# Names of generated Python packages:
# - \c vars.hdf5PyPackage:  HDF5 database access
# - \c vars.pgdbPyPackage:  PostgreSQL database access 
# - \c vars.icePyPackage:   ZeroC Ice database interface
# - \c vars.serverPyPackage:  Transparent access to HDF5 and PostgreSQL databases
# - \c vars.clientPyPackage:  Utility functions to connect to database
#
# @param defFile   variable definition file name
# @param basename  base name of database (Capitalized)
# @return variable definition
# @see process()
def read(defFile, basename='SenStore'):
    # Get database definition
    dbVars = process(defFile)

    # Set naming and file variables
    dbVars.name     = basename
    dbVars.descr    = '%s database' % basename
    dbVars.hdf5PyPackage = '%s_h5db' % basename
    dbVars.pgdbPyPackage = '%s_pgdb' % basename
    dbVars.iceDefFile = '%s.ice' % basename
    dbVars.icePyPackage  = basename
    dbVars.serverPyPackage  = '%sServer' % basename
    dbVars.clientPyPackage  = '%sClient' % basename

    return dbVars
