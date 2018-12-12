Dictionary Class
==================

A list of key-value pairs. Values can be any atomic data type, but not an array.

Full Object Definition
----------------------

::

    object: {
        _class: string = "dictionary"
        _group: string = "core"
        _type: string = "object"
        _version: uint64 = 1
        description: string = $DOC_STRING
        items: {
            $KEY0: $ATOMIC_TYPE_0 = $VALUE_0
            $KEY1: $ATOMIC_TYPE_1 = $VALUE_1
            .
            .
            .
            $KEYN: $ATOMIC_TYPE_N = $VALUE_N
        }
    }

Summary Object Definition
-------------------------

::

    object: {
        _class: string = "dictionary"
        _group: string = "core"
        _type: string = "summary"
        _version: uint64 = 1
        description: string = $DOC_STRING
    }

