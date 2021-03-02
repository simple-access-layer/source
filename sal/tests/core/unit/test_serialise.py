from datetime import datetime
from inspect import isclass
from unittest.mock import Mock

import numpy as np
import pytest

from sal.core.serialise import (serialise, deserialise, encode_types,
                                decode_types)
from sal.core.object import (Branch, BranchReport, LeafReport, ObjectReport,
                             DataObject, DataSummary, register)



@pytest.mark.parametrize('value, dict_type, encoded_type',
                         [(1, int, 'int64'),
                          (1., float, 'float64'),
                          ('string', str, 'string'),
                          (1, np.int8, 'int8'),
                          (1, np.int16, 'int16'),
                          (1, np.int32, 'int32'),
                          (1, np.int64, 'int64'),
                          (1, np.uint8, 'uint8'),
                          (1, np.uint16, 'uint16'),
                          (1, np.uint32, 'uint32'),
                          (1, np.uint64, 'uint64'),
                          (1., np.float32, 'float32'),
                          (1., np.float64, 'float64')])
def test_encode_scalar(value, dict_type, encoded_type):

    """
    GIVEN
        A dictionary containing scalar data
    WHEN
        The scalar data is encoded
    THEN
        Correctly encoded data is returned
    """

    data_dict = {'data': dict_type(value)}

    encoded_data = encode_types(data_dict)['data']
    
    assert encoded_data == {'type': encoded_type, 'value': value}


def test_encode_dict():

    """
    GIVEN
        A dictionary containing dictionary data
    WHEN
        The dictionary data is encoded
    THEN
        Correctly encoded data is returned
    """

    val1 = 5
    val2 = np.float32(1.5)
    val3 = 'A string'
    val4 = {'dval': np.uint64(9)}
    data_dict = {'data': {'val1': val1,
                          'val2': val2,
                          'val3': val3,
                          'val4': val4}}

    encoded_data = encode_types(data_dict)['data']

    assert encoded_data == {
        'type': 'branch',
        'value': {'val1': {'type': 'int64',
                           'value': val1},
                  'val2': {'type': 'float32',
                           'value': val2},
                  'val3': {'type': 'string',
                           'value': val3},
                  'val4': {'type': 'branch',
                           'value': {'dval': {'type': 'uint64',
                                              'value': val4['dval']}}}}}


@pytest.mark.parametrize('array, encoded_type, shape, encoding, data',
                         [(np.array(['Some', 'string', 'data']),
                           'string',
                           (3, ),
                           'list',
                           ['Some', 'string', 'data']),
                          (np.zeros((2, 2), dtype=np.int8),
                           'int8',
                           (2, 2),
                           'base64',
                           'AAAAAA=='),
                          (np.zeros((2, 2), dtype=np.int16),
                           'int16',
                           (2, 2),
                           'base64',
                           'AAAAAAAAAAA=')])
def test_encode_array(array, encoded_type, shape, data, encoding):

    """
    GIVEN
        A dictionary containing array data
    WHEN
        The array data is encoded
    THEN
        Correctly encoded data is returned
    """

    data_dict = {'data': array}
    encoded_data = encode_types(data_dict)['data']
    assert encoded_data == {'type': 'array',
                            'value': {'type': encoded_type,
                                      'shape': shape,
                                      'encoding': encoding,
                                      'data': data}}

