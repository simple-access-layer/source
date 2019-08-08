import unittest
import numpy as np
from sal.dataclass.core.dictionary import Dictionary, DictionarySummary


class TestDictionary(unittest.TestCase):

    def setUp(self):

        self.valid_content = {
            'uint8_value': np.uint8(5),
            'uint16_value': np.uint16(10),
            'uint32_value': np.uint32(15),
            'uint64_value': np.uint64(20),
            'int_value': int(0),
            'int8_value': np.int8(-5),
            'int16_value': np.int16(-10),
            'int32_value': np.int32(-15),
            'int64_value': np.int64(-20),
            'float_value': float(0.0),
            'fp32_value': np.float32(1.0),
            'fp64_value': np.float64(2.0),
            'numpy_bool_value': np.bool(True),
            'bool_value': bool(True),
            'string_value': str('Test string.')
        }

        self.valid_content_promoted = {
            'uint8_value': np.uint8(5),
            'uint16_value': np.uint16(10),
            'uint32_value': np.uint32(15),
            'uint64_value': np.uint64(20),
            'int_value': np.int64(0),
            'int8_value': np.int8(-5),
            'int16_value': np.int16(-10),
            'int32_value': np.int32(-15),
            'int64_value': np.int64(-20),
            'float_value': np.float64(0.0),
            'fp32_value': np.float32(1.0),
            'fp64_value': np.float64(2.0),
            'numpy_bool_value': np.bool(True),
            'bool_value': np.bool(True),
            'string_value': str('Test string.')
        }

        self.valid_content_serialised = {
            '_group': 'core',
            '_class': 'dictionary',
            '_type': 'object',
            '_version': 1,
            'description': 'A dictionary.',
            'items': {
                'uint8_value': np.uint8(5),
                'uint16_value': np.uint16(10),
                'uint32_value': np.uint32(15),
                'uint64_value': np.uint64(20),
                'int_value': np.int64(0),
                'int8_value': np.int8(-5),
                'int16_value': np.int16(-10),
                'int32_value': np.int32(-15),
                'int64_value': np.int64(-20),
                'float_value': np.float64(0.0),
                'fp32_value': np.float32(1.0),
                'fp64_value': np.float64(2.0),
                'numpy_bool_value': np.bool(True),
                'bool_value': np.bool(True),
                'string_value': str('Test string.')
            }
        }

    def test_dataclass_properties(self):

        d = Dictionary()
        self.assertEqual(d.CLASS, 'dictionary')
        self.assertEqual(d.GROUP, 'core')
        self.assertEqual(d.VERSION, 1)

    def test_init_default(self):

        d = Dictionary()
        self.assertEqual(d.description, 'A dictionary.')
        self.assertEqual(d._items, {})

    def test_init_items(self):

        d = Dictionary(items=self.valid_content)
        self.assertEqual(d.description, 'A dictionary.')
        self.assertEqual(d._items, self.valid_content_promoted)

    def test_init_description(self):

        d = Dictionary(description='A test dictionary.')
        self.assertEqual(d.description, 'A test dictionary.')
        self.assertEqual(d._items, {})

    def test_getitem(self):

        d = Dictionary(items=self.valid_content)

        # valid keys
        for key, value in self.valid_content_promoted.items():
            self.assertEqual(d[key], value)

        # invalid key
        with self.assertRaises(KeyError):
            v = d['abc']

    def test_setitem(self):

        d = Dictionary()

        # valid values
        d['alpha'] = 'apple'
        d['beta'] = np.int8(1)
        d['abcdefghijklmnopqrstuvwxyz0123456789_.-'] = np.float64(1.0)

        self.assertEqual(d._items['alpha'], 'apple')
        self.assertEqual(d._items['beta'], np.int8(1))
        self.assertEqual(d._items['abcdefghijklmnopqrstuvwxyz0123456789_.-'], np.float64(1.0))

        # invalid key
        with self.assertRaises(ValueError):
            d['ABC'] = 'blah'

        # invalid value
        with self.assertRaises(TypeError):
            d['abc'] = []

    def test_delitem(self):

        d = Dictionary(items=self.valid_content)
        self.assertTrue('int8_value' in d._items)
        del d['int8_value']
        self.assertFalse('int8_value' in d._items)

    def test_iter(self):

        d = Dictionary(self.valid_content)
        for key in iter(d):
            self.assertTrue(key in d._items)

    def test_len(self):

        d = Dictionary(self.valid_content)
        self.assertEqual(len(d), 15)

    def test_contains(self):

        d = Dictionary(self.valid_content)

        # valid keys
        for key in self.valid_content.keys():
            self.assertTrue(key in d)

        # invalid keys
        self.assertFalse('non_existent' in d)

    def test_keys(self):

        d = Dictionary(self.valid_content)
        keys_original = set(self.valid_content.keys())
        keys_parsed = set(d.keys())
        self.assertFalse(keys_original - keys_parsed)

    def test_values(self):

        d = Dictionary(self.valid_content)
        values_original = set(self.valid_content_promoted.values())
        values_parsed = set(d.values())
        self.assertFalse(values_original - values_parsed)

    def test_items(self):

        d = Dictionary(self.valid_content)
        r = dict(d.items())
        self.assertEqual(r, self.valid_content_promoted)

    def test_clear(self):

        d = Dictionary(self.valid_content)
        d.clear()
        self.assertEqual(d._items, {})

    def test_get(self):

        d = Dictionary(items=self.valid_content)

        # valid keys
        for key, value in self.valid_content_promoted.items():
            self.assertEqual(d.get(key), value)

        # invalid key
        r = d.get('abc')
        self.assertEqual(r, None)

        # invalid key, alternate default
        r = d.get('abc', 'spoon')
        self.assertEqual(r, 'spoon')

    def test_update(self):

        d = Dictionary()

        # valid content
        d.update(self.valid_content, apple=90, banana=7.8)
        for key, value in self.valid_content_promoted.items():
            self.assertEqual(d._items[key], value)
        self.assertEqual(d._items['apple'], np.int64(90))
        self.assertEqual(d._items['banana'], np.float64(7.8))

        # invalid content via dictionary
        with self.assertRaises(ValueError):
            d.update({'ABC': 5})

        with self.assertRaises(TypeError):
            d.update({'abc': []})

        # invalid argument via kwargs
        with self.assertRaises(ValueError):
            d.update(ABC=5)

        with self.assertRaises(TypeError):
            d.update(abc=[])

    def test_summary(self):

        d = Dictionary(items=self.valid_content, description='A test dictionary.')
        s = d.summary()

        # a DictionarySummary must be returned
        self.assertIsInstance(s, DictionarySummary)

        # has description correctly propagated
        self.assertEqual(s.description, 'A test dictionary.')

    def test_to_dict(self):

        d = Dictionary(self.valid_content)
        self.assertEqual(d.to_dict(), self.valid_content_serialised)

    def test_from_dict(self):

        # valid data
        d = Dictionary.from_dict(self.valid_content_serialised)

        # invalid data
        with self.assertRaises(ValueError):
            d = Dictionary.from_dict({})

        with self.assertRaises(ValueError):
            d = Dictionary.from_dict(
                {
                    '_group': 'invalid',
                    '_class': 'wrong',
                    '_type': 'object',
                    '_version': 1,
                    'description': 'A dictionary.',
                    'items': {}
                }
            )


class TestDictionarySummary(unittest.TestCase):

    def setUp(self):

        self.valid_content_serialised = {
            '_group': 'core',
            '_class': 'dictionary',
            '_type': 'summary',
            '_version': 1,
            'description': 'A test dictionary.',
        }

    def test_dataclass_properties(self):

        d = DictionarySummary()
        self.assertEqual(d.CLASS, 'dictionary')
        self.assertEqual(d.GROUP, 'core')
        self.assertEqual(d.VERSION, 1)

    def test_init_default(self):

        d = DictionarySummary()
        self.assertEqual(d.description, 'A dictionary.')

    def test_init_description(self):

        d = DictionarySummary(description='A test dictionary.')
        self.assertEqual(d.description, 'A test dictionary.')

    def test_to_dict(self):

        d = DictionarySummary(description='A test dictionary.')
        self.assertEqual(d.to_dict(), self.valid_content_serialised)

    def test_from_dict(self):

        # valid data
        d = DictionarySummary.from_dict(self.valid_content_serialised)
        self.assertEqual(d.description, 'A test dictionary.')

        # invalid data
        with self.assertRaises(ValueError):
            d = DictionarySummary.from_dict({})

        with self.assertRaises(ValueError):
            d = DictionarySummary.from_dict(
                {
                    '_group': 'invalid',
                    '_class': 'wrong',
                    '_type': 'summary',
                    '_version': 1,
                    'description': 'A dictionary.',
                }
            )


if __name__ == '__main__':
    unittest.main()
