Scalar
======

Represents a single numerical value.

Full Object Definition
----------------------

::

    object: {
        _class: string = "scalar"
        _group: string = "core"
        _type: string = "object"
        _version: uint64 = 1
        description: string = $DOC_STRING
        value: $TYPE = $VALUE
    }

Summary Object Definition
-------------------------

::

    object: {
        _class: string = "scalar"
        _group: string = "core"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = $DOC_STRING
    }