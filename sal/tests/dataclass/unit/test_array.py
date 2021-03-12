import unittest
import numpy as np
from sal.dataclass.core.array import Array, ArraySummary


class TestArray(unittest.TestCase):

    def setUp(self):

        self.data = [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]

        self.dtypes = (
            np.int8,
            np.int16,
            np.int32,
            np.int64,
            np.uint8,
            np.uint16,
            np.uint32,
            np.uint64,
            np.float32,
            np.float64,
        )

        self.promoted = (
            (int, np.int64),
            (float, np.float64)
        )

        self.valid_header_serialised = {
            '_group': 'core',
            '_class': 'array',
            '_type': 'object',
            '_version': 1,
            'description': 'An array.',
        }

    def test_dataclass_properties(self):

        d = Array(shape=(1,))
        self.assertEqual(d.CLASS, 'array')
        self.assertEqual(d.GROUP, 'core')
        self.assertEqual(d.VERSION, 1)

    def test_init_default(self):

        d = Array(shape=(1,))
        self.assertEqual(d.description, 'An array.')
        self.assertEqual(d.data.dtype, np.float64)

    def test_init_description(self):

        d = Array(shape=(1,), description='A new data array.')
        self.assertEqual(d.description, 'A new data array.')

    def test_init_data(self):

        # data consistent with shape
        d = Array(shape=(3, 4), data=self.data)
        self.assertEqual(d.data.dtype, np.float64)
        self.assertTrue((d.data == np.array(self.data, dtype=np.float64)).all())

        # data size inconsistent with data
        with self.assertRaises(ValueError):
            Array(shape=(10,), data=self.data)

    def test_shape(self):

        shapes = (
            (1,),
            (100,),
            (20, 30),
            (5, 10, 3),
            (2, 8, 9, 2),
        )

        for shape in shapes:
            d = Array(shape=shape)
            self.assertEqual(d.shape, shape)
            self.assertEqual(d.data.shape, shape)

    def test_dtype(self):

        for dtype in self.dtypes:
            d = Array(shape=(3, 4), data=self.data, dtype=dtype)
            self.assertEqual(d.data.dtype, dtype)

        for python_type, np_type in self.promoted:
            d = Array(shape=(3, 4), data=self.data, dtype=python_type)
            self.assertEqual(d.data.dtype, np_type)

    def test_summary(self):

        d = Array(shape=(3, 4), data=self.data, description='A test array.')
        s = d.summary()

        # an ArraySummary must be returned
        self.assertIsInstance(s, ArraySummary)

        # has description correctly propagated
        self.assertEqual(s.description, 'A test array.')
        self.assertEqual(s.shape, (3, 4))

    def test_to_dict(self):

        for dtype in self.dtypes:

            d = Array(shape=(3, 4), data=self.data, dtype=dtype)
            v = d.to_dict()
            r = self.valid_header_serialised.copy()

            # check data item
            self.assertTrue((v['data'] == np.array(self.data, dtype)).all())

            # check other items
            del v['data']
            self.assertEqual(v, r)

        for python_type, np_type in self.promoted:

            d = Array(shape=(3, 4), data=self.data, dtype=python_type)
            v = d.to_dict()
            r = self.valid_header_serialised.copy()

            # check data item
            self.assertTrue((v['data'] == np.array(self.data, dtype)).all())

            # check other items
            del v['data']
            self.assertEqual(v, r)

    def test_from_dict(self):

        # valid data
        for dtype in self.dtypes:

            v = self.valid_header_serialised.copy()
            v['data'] = np.array(self.data, dtype)

            d = Array.from_dict(v)
            self.assertEqual(d.shape, (3, 4))
            self.assertTrue((d.data == np.array(self.data, dtype)).all())

        # invalid data
        with self.assertRaises(ValueError):
            Array.from_dict({})

        with self.assertRaises(ValueError):
            Array.from_dict(
                {
                    '_group': 'invalid',
                    '_class': 'wrong',
                    '_type': 'object',
                    '_version': 1,
                }
            )


class TestArraySummary(unittest.TestCase):

    def setUp(self):

        self.valid_header_serialised = {
            '_group': 'core',
            '_class': 'array',
            '_type': 'summary',
            '_version': 1,
            'description': 'A test array.',
        }

    def test_dataclass_properties(self):

        d = ArraySummary(shape=(1,))
        self.assertEqual(d.CLASS, 'array')
        self.assertEqual(d.GROUP, 'core')
        self.assertEqual(d.VERSION, 1)

    def test_init_default(self):

        d = ArraySummary((1,))
        self.assertEqual(d.description, 'An array.')

    def test_init_description(self):

        d = ArraySummary(shape=(1,), description='A new data array.')
        self.assertEqual(d.description, 'A new data array.')

    def test_shape(self):

        shapes = (
            (1,),
            (100,),
            (20, 30),
            (5, 10, 3),
            (2, 8, 9, 2),
        )

        for shape in shapes:
            d = ArraySummary(shape=shape)
            self.assertEqual(d.shape, shape)

    def test_to_dict(self):

        d = ArraySummary(shape=(3, 4), description='A test array.')
        v = d.to_dict()
        r = self.valid_header_serialised.copy()

        # check data item
        self.assertTrue((v['shape'] == np.array([3, 4], np.uint64)).all())

        # check other items
        del v['shape']
        self.assertEqual(v, r)

    def test_from_dict(self):

        # valid data
        v = self.valid_header_serialised.copy()
        v['shape'] = np.array([3, 4], dtype=np.uint64)

        d = ArraySummary.from_dict(v)
        self.assertEqual(d.shape, (3, 4))

        # invalid data
        with self.assertRaises(ValueError):
            ArraySummary.from_dict({})

        with self.assertRaises(ValueError):
            ArraySummary.from_dict(
                {
                    '_group': 'invalid',
                    '_class': 'wrong',
                    '_type': 'summary',
                    '_version': 1,
                }
            )


if __name__ == '__main__':
    unittest.main()
