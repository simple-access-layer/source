Array Class
===========

An n-dimensional array object.

Full Object Definition
----------------------

::

    object: {
        _class: string = "array"
        _group: string = "core"
        _type: string = "object"
        _version: uint64 = 1
        description: string = $DOC_STRING
        data: array<$TYPE> = $DATA
    }

Summary Object Definition
-------------------------

::

    object: {
        _class: string = "array"
        _group: string = "core"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = $DOC_STRING
        shape: array<uint64> = $SHAPE
    }


