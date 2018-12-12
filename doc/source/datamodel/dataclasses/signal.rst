Signal Class
============

Signal with N monotonic dimensions. The signal object is a complex object with nested data classes to define components of the signal: e.g. dimensions, errors, status mask, provenance

Core Class
----------

Full Object Definition
~~~~~~~~~~~~~~~~~~~~~~

::

    object: {
        _class: string = "signal"
        _group: string = "signal"
        _type: string = "object"
        _version: uint64 = 1
        description: string = $SIGNAL_DESCRIPTION
        units: string = $SIGNAL_UNITS
        dimensions: {
            count: uint64 = $DIMENSION_COUNT
            0: signal_dimension = $DIMENSION_0_DATA_OBJECT
            1: signal_dimension = $DIMENSION_1_DATA_OBJECT
            .
            .
            .
            $N: signal_dimension = $DIMENSION_N_DATA_OBJECT
        }
        data: array<$TYPE> = $DATA
        [mask: signal_mask = $STATUS_MASK_DATA_OBJECT]
        [error: signal_error = $ERROR_DATA_OBJECT]
    }


Summary Object Definition
~~~~~~~~~~~~~~~~~~~~~~~~~

::

    object: {
        _class: string = "signal"
        _group: string = "signal"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = $SIGNAL_DESCRIPTION
        units: string = $SIGNAL_UNITS
        dimensions: {
            count: uint64 = $DIMENSION_COUNT
            0: signal_dimension = $DIMENSION_0_META_OBJECT
            1: signal_dimension = $DIMENSION_1_META_OBJECT
            .
            .
            .
            $N: signal_dimension = $DIMENSION_N_META_OBJECT
        }
        [mask: signal_mask = $STATUS_MASK_META_OBJECT]
        [error: signal_error = $ERROR_META_OBJECT]
    }


Dimension Classes
-----------------

Calculated Dimension
~~~~~~~~~~~~~~~~~~~~

Full Object Definition
++++++++++++++++++++++

::

    object: {
        _class: string = "dimension_calculated"
        _group: string = "signal_dimension"
        _type: string = "object"
        _version: uint64 = 1
        description: string = $DESCRIPTION
        units: string = $UNITS
        start: fp64 = $START
        increment: fp64 = $INCREMENT
        length: uint64 = $DIMENSION_LENGTH
        temporal: bool = $TIME_DIMENSION
        [error: signal_error = $ERROR_DATA_OBJECT]
    }

Summary Object Definition
+++++++++++++++++++++++++

::

    object: {
        _class: string = "dimension_calculated"
        _group: string = "signal_dimension"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = $DESCRIPTION
        units: string = $UNITS
        length: uint64 = $DIMENSION_LENGTH
        temporal: bool = $TIME_DIMENSION
        [error: signal_error = $ERROR_META_OBJECT]
    }

Array Dimension Sub-Class
~~~~~~~~~~~~~~~~~~~~~~~~~

Full Object Definition
++++++++++++++++++++++

::

    object: {
        _class: string = "dimension_array"
        _group: string = "signal_dimension"
        _type: string = "object"
        _version: uint64 = 1
        description: string = $DESCRIPTION
        units: string = $UNITS
        length: uint64 = $DIMENSION_LENGTH
        data: array<$TYPE> = $DATA
        temporal: bool = $TIME_DIMENSION
        [error: signal_error = $ERROR_DATA_OBJECT]
    }

Summary Object Definition
+++++++++++++++++++++++++

::

    object: {
        _class: string = "dimension_array"
        _group: string = "signal_dimension"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = $DESCRIPTION
        units: string = $UNITS
        length: uint64 = $DIMENSION_LENGTH
        temporal: bool = $TIME_DIMENSION
        [error: signal_error = $ERROR_META_OBJECT]
    }

Error Classes
-------------

Constant Error
~~~~~~~~~~~~~~

Full Object Definition
++++++++++++++++++++++

::

    object: {
        _class: string = "error_constant"
        _group: string = "signal_error"
        _type: string = "object"
        _version: uint64 = 1
        description: string = "Constant error."
        lower: fp64 = $LOWER
        upper: fp64 = $UPPER
        relative: boolean = $RELATIVE
    }

Summary Object Definition
+++++++++++++++++++++++++

::

    object: {
        _class: string = "error_constant"
        _group: string = "signal_error"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = "Constant error."
    }

Symmetrical Error Array
~~~~~~~~~~~~~~~~~~~~~~~

Full Object Definition
++++++++++++++++++++++

::

    object: {
        _class: string = "error_symmetrical"
        _group: string = "signal_error"
        _type: string = "object"
        _version: uint64 = 1
        description: string = "Symmetrical error per point."
        data: array<$TYPE> = $ERROR_DATA
    }

Summary Object Definition
+++++++++++++++++++++++++

::

    object: {
        _class: string = "error_symmetrical"
        _group: string = "signal_error"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = "Symmetrical error per point."
    }

Asymmetric Error Array
~~~~~~~~~~~~~~~~~~~~~~

Full Object Definition
++++++++++++++++++++++

::

    object: {
        _class: string = "error_asymmetric"
        _group: string = "signal_error"
        _type: string = "object"
        _version: uint64 = 1
        description: string = "Asymmetric error per point."
        lower: array<$TYPE> = $LOWER_ERROR_DATA
        upper: array<$TYPE> = $UPPER_ERROR_DATA
    }

Summary Object Definition
+++++++++++++++++++++++++

::

    object: {
        _class: string = "error_asymmetric"
        _group: string = "signal_error"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = "Asymmetric error per point."
    }

Mask Classes
------------

Scalar Status Mask
~~~~~~~~~~~~~~~~~~

Full Object Definition
++++++++++++++++++++++

::

    object: {
        _class: string = "mask_scalar_status"
        _group: string = "signal_mask"
        _type: string = "object"
        _version: uint64 = 1
        description: string = "Scalar status mask."
        status: uint8 = $STATUS_VALUE
        key: array<string> = $STATUS_DESCRIPTION_TABLE
    }

Summary Object Definition
+++++++++++++++++++++++++

::

    object: {
        _class: string = "mask_scalar_status"
        _group: string = "signal_mask"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = "Scalar status mask."
    }

Example
+++++++

::

    object: {
        _class: string = "mask_scalar_status"
        _group: string = "signal_mask"
        _type: string = "object"
        _version: uint64 = 1
        description: string = "Scalar status mask."
        status: uint8 = 0
        key: array<string> = ["unvalidated", "good", "medium", "poor", "unusable"]
    }

Array Status Mask
~~~~~~~~~~~~~~~~~

Full Object Definition
++++++++++++++++++++++

::

    object: {
        _class: string = "mask_status_array"
        _group: string = "signal_mask"
        _type: string = "object"
        _version: uint64 = 1
        description: string = "Array status mask."
        status: array<uint8> = $STATUS_VALUES
        key: array<string> = $STATUS_DESCRIPTION_TABLE
    }

Summary Object Definition
+++++++++++++++++++++++++

::

    object: {
        _class: string = "mask_status_array"
        _group: string = "signal_mask"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = "Array status mask."
    }

Example
+++++++

::

    object: {
        _class: string = "mask_status_array"
        _group: string = "signal_mask"
        _type: string = "object"
        _version: uint64 = 1
        description: string = "Array status mask."
        status: array<uint8> = [
            [4,4,4,4,3,3,2,2,1,1,1,1,1,1,1,1,4,4,1,1,1,1,2,3,3,4,4],
            [4,2,2,2,3,2,2,2,1,1,1,4,4,3,2,1,1,1,1,1,1,1,2,3,4,4,4]
        ]
        key: array<string> = ["unvalidated", "good", "medium", "poor", "unusable"]
        }



