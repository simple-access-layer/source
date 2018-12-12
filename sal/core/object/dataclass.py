import pkgutil
import importlib
import numpy as np

# todo: consider making DataObject etc.. Abstract Base Classes (ABCs)

__all__ = ['register', 'build', 'resolve', 'list', 'DataClass', 'DataObject', 'DataSummary', 'TYPE_OBJECT', 'TYPE_SUMMARY']

# dictionaries holding the data object and data summary registry
# the key name is the class name (id) and the item is equivalent python class
_DATACLASS_OBJECT_REGISTRY = {}
_DATACLASS_SUMMARY_REGISTRY = {}

TYPE_OBJECT = 'object'
TYPE_SUMMARY = 'summary'


def register(cls):
    """
    Decorator that adds a data class to the class registry.

    :param cls: Class to register.
    :return: Class.
    """

    if not issubclass(cls, DataObject):
        raise TypeError('Only DataObject classes may be registered.')

    name = cls.CLASS.lower()
    if name in _DATACLASS_OBJECT_REGISTRY:
        raise ValueError('Data class ID \'{}\' is already registered.'.format(name))

    if not cls.SUMMARY_CLASS or not issubclass(cls.SUMMARY_CLASS, DataSummary):
        raise TypeError('The data summary class specified by the data object is not a subclass of DataSummary.')

    _DATACLASS_OBJECT_REGISTRY[name] = cls
    _DATACLASS_SUMMARY_REGISTRY[name] = cls.SUMMARY_CLASS

    return cls


def build(d):
    """
    Identifies and builds the data or summary class defined by the supplied dictionary.

    This function will automatically inspect the _type and _class keys of the dictionary
    and look up the relevant class in the class registry. The dictionary will be
    de-serialised into an instance of the identified class.

    If the dictionary does not contain the right header keys a ValueError exception is raised.

    :param d: Dictionary containing serialised class data.
    :return: A populated data/summary object.
    """

    try:
        type = d['_type']
        name = d['_class']
    except KeyError:
        raise ValueError('The supplied data does not define a data class object.')

    return resolve(name, type).from_dict(d)


def resolve(name, type):
    """
    Returns the Python class that implements the data class specified by the string class/type names.

    If the class is not recognised, a ValueError is raised.

    :param name: String name of data class.
    :param type: String type of data class.
    :return: Class.
    """

    # select appropriate registry
    if type == TYPE_OBJECT:
        registry = _DATACLASS_OBJECT_REGISTRY
    elif type == TYPE_SUMMARY:
        registry = _DATACLASS_SUMMARY_REGISTRY
    else:
        raise ValueError('Unsupported class type: {}'.format(type))

    # obtain class
    try:
        return registry[name.lower()]
    except KeyError:
        raise ValueError(
            'Specified data class \'{}\' could not be identified, has it been registered?'.format(name))


def list():
    """
    Returns a dictionary listing the registered data classes.

    The registered data classes are listed by group and then class in a nested
    dictionary:

        registerd = {
            'core': ['string', 'scalar', 'dictionary'],
            'signal': ['signal', 'envelope']
        }

    :return: A dictionary listing the registered data classes.
    """
    d = {}
    for cls in _DATACLASS_OBJECT_REGISTRY.values():
        if cls.GROUP not in d:
            d[cls.GROUP] = []
        d[cls.GROUP].append(cls.CLASS)
    return d


class DataClass:
    """
    Abstract base class representing a SAL data class.

    This class should not be extended from directly, developers wishing to add
    a new data classes to SAL should use the DataObject and DataSummary base
    classes.
    """

    #: The class id string.
    CLASS = None

    #: The group id string.
    GROUP = None

    #: The data-class version.
    VERSION = -1

    def __init__(self, description):

        #: A string description.
        self.description = description

    def __repr__(self):

        return '<{} (class=\'{}\', group=\'{}\', version={}) at {}>'.format(
            self.__class__.__name__,
            self.CLASS,
            self.GROUP,
            self.VERSION,
            str(hex(id(self)))
        )

    @classmethod
    def from_dict(cls, d):
        """
        Instances the class from a dictionary representation.
        
        :param d: Dictionary containing a serialised object. 
        :return: An object instance.
        """

        raise NotImplementedError('This method must be implemented by sub-classes.')

    def to_dict(self):
        """
        Returns a dictionary representation of the data object.
         
        :return: A data object dictionary. 
        """

        raise NotImplementedError('This method must be implemented by sub-classes.')

    @classmethod
    def is_compatible(cls, d):
        """
        Checks the dictionary contains the correct header information for the data class.

        Returns True if the correct class, group, version and type attributes are defined,
        False is returned otherwise.

        :param d: Dictionary containing serialised class data.
        :returns: True if compatible, False otherwise.
        """

        try:
            data_class = d['_class']
            group = d['_group']
            version = d['_version']
        except KeyError:
            return False

        return data_class == cls.CLASS and group == cls.GROUP and version == cls.VERSION

    def _new_dict(self):
        """
        Creates a new data object dictionary populated with common header data.
        
        :return: A data object dictionary. 
        """

        return {
            "_class": self.CLASS,
            "_group": self.GROUP,
            "_version": np.uint64(self.VERSION),
            "description": str(self.description)
        }


# TODO: add tests
class DataObject(DataClass):
    """
    Abstract base class representing a SAL data object.

    A DataObject represents a discrete chunk of data stored by the SAL system, such as
    a signal, image, or audio recording. A SAL leaf node contains a single DataObject. 

    A new SAL data class is defined by extending DataObject and defining an associated
    DataSummary class. The data object is then registered with SAL using the register
    decorator found in this module::

        @register
        class MyNewType(DataObject):
            etc...

    Concrete classes derived from DataObject must populate four class attributes::
     
        CLASS: A string that uniquely identifies the class to the SAL server and client.
        GROUP: A string that specifies the data class groups the class belongs to.
        VERSION: An integer specifying the version of the data class.
        SUMMARY_CLASS: The summary class associated with the data class.  

    The string attributes CLASS and GROUP must be lower-case and contain only alphanumeric
    characters, '_', '-' and no whitespace. e.g. CLASS = 'signal', GROUP = 'signal_dimension'

    VERSION must be an integer greater than zero. The version of a data class should be
    incremented if the storage representation of the class changes. This ensures it is simple
    for developers to identify different representations of a data class stored in the data
    system.
    
    The SUMMARY_CLASS attribute must be a reference to the DataSummary class returned by the
    DataObject when summary() is called. e.g. SUMMARY_CLASS = MyNewTypeSummary
    
    The from_dict() and to_dict() methods must be implemented. To simplify development of
    to_dict() a helper method _new_dict() is provided. The _new_dict() method will return a
    dictionary pre-populated with the data class metadata keys.
    """

    #: The summary class associated with this data class.
    SUMMARY_CLASS = None

    def summary(self):
        """
        Returns a summary of the data object.

        :return: A DataSummary object.
        """

        raise NotImplementedError('This method must be implemented by sub-classes.')

    @classmethod
    def is_compatible(cls, d):
        """
        Checks the dictionary contains the correct header information for the data class.

        Returns True if the correct class, group, version and type attributes are defined,
        False is returned otherwise.

        :param d: Dictionary containing serialised class data.
        :returns: True if compatible, False otherwise.
        """

        try:
            class_type = d['_type']
        except KeyError:
            return False

        return super().is_compatible(d) and class_type == TYPE_OBJECT

    def _new_dict(self):
        """
        Creates a new data object dictionary populated with common header data.
        
        :return: A data object dictionary. 
        """

        d = super()._new_dict()
        d['_type'] = TYPE_OBJECT
        return d


# TODO: add tests
class DataSummary(DataClass):
    """
    Abstract base class representing a SAL data summary.

    A DataSummary object is a reduced representation, or summary, of the data stored in a
    DataObject. A summary object will typically contain the DataObject metadata, but not
    the data arrays.

    The values of the CLASS, GROUP and VERSION attributes of the summary class must match the
    values in the associated DataObject.
    """

    @classmethod
    def is_compatible(cls, d):
        """
        Checks the dictionary contains the correct header information for the data class.

        Returns True if the correct class, group, version and type attributes are defined,
        False is returned otherwise.

        :param d: Dictionary containing serialised class data.
        :returns: True if compatible, False otherwise.
        """

        try:
            class_type = d['_type']
        except KeyError:
            return False

        return super().is_compatible(d) and class_type == TYPE_SUMMARY

    def _new_dict(self):
        """
        Creates a new data object dictionary populated with common header data.
        
        :return: A data object dictionary. 
        """

        d = super()._new_dict()
        d['_type'] = TYPE_SUMMARY
        return d
