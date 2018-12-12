from sal.core.object import DataObject, DataSummary, TYPE_OBJECT, TYPE_SUMMARY

# The signal object is pluggable, parts of the object can be swapped out for alternate versions e.g. dimensions.
# For simplicity the sub-objects are SAL DataClass objects.
# The signal class therefore has it's own (parallel) system for registering classes. This is basically a copy
# of the data class registry system used to manage and instance SAL DataClass objects in the SAL server and SAL client.
# The separate registry is necessary as adding the sub objects to the main registry would allow users to put and get
# the sub-objects directly as SAL leaves.

_SIGNAL_OBJECT_REGISTRY = {}
_SIGNAL_SUMMARY_REGISTRY = {}


def register(cls):
    """
    Decorator that adds a data class to the signal sub-object class ID lookup table.

    :param cls: Class to register.
    :return: Class.
    """

    if not issubclass(cls, DataObject):
        raise TypeError('Only DataObject classes may be registered as signal sub-objects.')

    name = cls.CLASS.lower()
    if name in _SIGNAL_OBJECT_REGISTRY:
        raise ValueError('Data class ID \'{}\' is already registered as a signal sub-object.'.format(name))

    if not cls.SUMMARY_CLASS or not issubclass(cls.SUMMARY_CLASS, DataSummary):
        raise TypeError('The data summary class specified by the data object is not a subclass of DataSummary.')

    _SIGNAL_OBJECT_REGISTRY[name] = cls
    _SIGNAL_SUMMARY_REGISTRY[name] = cls.SUMMARY_CLASS

    return cls


def build(d):
    """
    Identifies and builds the data/summary class defined by the supplied dictionary.

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
    Returns the Python class that implements the data class specified by the string class/type ids.

    If the class is not recognised, a ValueError is raised.

    :param name: String ID of data class.
    :param type: String type of data class.
    :return: Class.
    """

    # select appropriate registry
    if type == TYPE_OBJECT:
        registry = _SIGNAL_OBJECT_REGISTRY
    elif type == TYPE_SUMMARY:
        registry = _SIGNAL_SUMMARY_REGISTRY
    else:
        raise ValueError('Unsupported class type: {}'.format(type))

    # obtain class
    try:
        return registry[name.lower()]
    except KeyError:
        raise ValueError('Specified data class \'{}\' could not be identified, has it been registered?'.format(name))