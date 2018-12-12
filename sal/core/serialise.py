"""
Serialise persistence layer objects for transmission using JSON.
"""

# TODO: add unit tests

import base64

import numpy as _np

from sal.core.object import DataClass, build
from sal.core.exception import InternalError
from sal.core.object import Branch, BranchReport, LeafReport

# supported numpy types
_NUMPY_INTEGER_TYPES = (_np.int8, _np.int16, _np.int32, _np.int64, _np.uint8, _np.uint16, _np.uint32, _np.uint64)
_NUMPY_FLOAT_TYPES = (_np.float32, _np.float64)

# tables for converting numpy->type id and vice versa
_TYPES_NUMPY_TO_ID = {
    _np.int8: 'int8',
    _np.int16: 'int16',
    _np.int32: 'int32',
    _np.int64: 'int64',
    _np.uint8: 'uint8',
    _np.uint16: 'uint16',
    _np.uint32: 'uint32',
    _np.uint64: 'uint64',
    _np.float32: 'float32',
    _np.float64: 'float64',
    bool: 'bool',
    str: 'string',
    _np.str_: 'string',
    _np.ndarray: 'array',
    dict: 'branch'
}

_TYPES_ID_TO_NUMPY = {
    'int8': _np.int8,
    'int16': _np.int16,
    'int32': _np.int32,
    'int64': _np.int64,
    'uint8': _np.uint8,
    'uint16': _np.uint16,
    'uint32': _np.uint32,
    'uint64': _np.uint64,
    'float32': _np.float32,
    'float64': _np.float64,
    'bool': bool,
    'string': str,
    'array': _np.ndarray,
    'branch': dict
}


def serialise(obj):
    """
    Encodes a persistence layer object in a json compatible serialised representation.

    :param obj: Persistence layer object.
    :return: A dictionary containing the serialised object.
    """

    if isinstance(obj, BranchReport):
        return {
            'content': 'report',
            'type': 'branch',
            'object': obj.to_dict()
        }

    if isinstance(obj, LeafReport):
        return {
            'content': 'report',
            'type': 'leaf',
            'object': obj.to_dict()
        }

    if isinstance(obj, Branch):
        return {
            'content': 'object',
            'type': 'branch',
            'object': obj.to_dict()
        }

    if isinstance(obj, DataClass):
        return {
            'content': 'object',
            'type': 'leaf',
            'object': encode_types(obj.to_dict())
        }


def deserialise(d):
    """
    Decodes a persistence layer object from a serialised representation.

    :param d: A dictionary containing the serialised object.
    :return: Persistence layer object.
    """

    try:
        content = d['content']
        type = d['type']
        obj = d['object']
    except KeyError:
        raise InternalError('Dictionary does not contain a serialised object.')

    if content == 'report':

        if type == 'branch':
            return BranchReport.from_dict(obj)

        if type == 'leaf':
            return LeafReport.from_dict(obj)

    elif content == 'object':

        if type == 'branch':
            return Branch.from_dict(obj)

        if type == 'leaf':
            # data class requires decoding of types
            return build(decode_types(obj))

    raise InternalError('Unrecognised class type.')


def encode_types(d):
    """
    Encodes python/numpy types for transmission over json transport.

    :param d: Dictionary containing typed data.
    :return: Encoded data.
    """

    packed = {}
    for key, item in d.items():

        if item is None:
            packed[key] = None

        elif isinstance(item, dict):
            packed[key] = _encode_branch(item)

        elif isinstance(item, _np.ndarray):
            packed[key] = _encode_array(item)

        else:
            packed[key] = _encode_scalar(item)

    return packed


def _encode_branch(d):
    """
    Encodes branch nodes for transmission over json transport.

    :param d: Dictionary containing typed data.
    :return: Encoded data.
    """

    return {
        'type': _TYPES_NUMPY_TO_ID[dict],
        'value': encode_types(d)
    }


def _encode_array(d):
    """
    Encodes arrays for transmission over json transport.

    :param d: Dictionary containing typed data.
    :return: Encoded data.
    """

    try:
        dtype = _TYPES_NUMPY_TO_ID[_np.ndarray]
        array_dtype = _TYPES_NUMPY_TO_ID[d.dtype.type]
        shape = d.shape
    except KeyError:
        raise InternalError('Object serialisation failed during type encoding.')

    if array_dtype == 'string':

        # simply list strings
        return {
            'type': dtype,
            'value': {
                'type': array_dtype,
                'shape': shape,
                'encoding': 'list',
                'data': d.tolist()
            }
        }

    else:

        # base64 encode
        data = d.tostring()
        encoded = base64.b64encode(data).decode('utf-8')
        return {
            'type': dtype,
            'value': {
                'type': array_dtype,
                'shape': shape,
                'encoding': 'base64',
                'data': encoded
            }
        }


def _encode_scalar(d):
    """
    Encodes scalars for transmission over json transport.

    :param d: Dictionary containing typed data.
    :return: Encoded data.
    """

    # python types are coerced to equivalent numpy types
    if isinstance(d, bool):
        dtype = _TYPES_NUMPY_TO_ID[bool]
        value = d

    elif isinstance(d, float):
        dtype = _TYPES_NUMPY_TO_ID[_np.float64]
        value = d

    elif isinstance(d, int):
        dtype = _TYPES_NUMPY_TO_ID[_np.int64]
        value = d

    elif isinstance(d, _NUMPY_INTEGER_TYPES):
        dtype = _TYPES_NUMPY_TO_ID[type(d)]
        value = int(d)

    elif isinstance(d, _NUMPY_FLOAT_TYPES):
        dtype = _TYPES_NUMPY_TO_ID[type(d)]
        value = float(d)

    else:
        try:
            dtype = _TYPES_NUMPY_TO_ID[type(d)]
            value = d
        except KeyError:
            raise InternalError('Object serialisation failed during type encoding.')

    return {
        'type': dtype,
        'value': value
    }


def decode_types(d):
    """
    Decodes python/numpy types from json encoding.

    :param d: Dictionary containing encoded type data.
    :return: Decoded data.
    """

    decoded = {}
    for key, item in d.items():

        if item is None:
            decoded[key] = None
            continue

        try:
            dtype = _TYPES_ID_TO_NUMPY[item['type']]
            value = item['value']
        except KeyError:
            raise InternalError('Malformed type data found during de-serialisation.')

        if dtype is dict:
            decoded[key] = decode_types(value)
        elif dtype is _np.ndarray:
            decoded[key] = _decode_array(value)
        else:
            decoded[key] = dtype(value)

    return decoded


def _decode_array(d):
    """
    Decodes arrays from json encoding.

    :param d: Dictionary containing encoded type data.
    :return: Decoded data.
    """

    try:
        encoding = d['encoding']
        dtype = _TYPES_ID_TO_NUMPY[d['type']]
        data = d['data']
        shape = d['shape']
    except KeyError:
        raise InternalError('Malformed array data found during de-serialisation.')

    if encoding == 'base64':
        return _np.fromstring(base64.b64decode(data), dtype=dtype).reshape(shape)

    if encoding == 'list':
        return _np.array(data, dtype=dtype)

    raise InternalError('Unrecognised array encoding type found during de-serialisation.')


