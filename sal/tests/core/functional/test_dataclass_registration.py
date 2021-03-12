import pytest

from sal import dataclass as dc 
from sal.core.object import dataclass


@pytest.mark.parametrize('class_',
                         [dc.String,
                          dc.Scalar,
                          dc.Array,
                          dc.Dictionary,
                          dc.Signal])
def test_dataobject_registered(class_):

    assert (dataclass._DATACLASS_OBJECT_REGISTRY[getattr(class_, 'CLASS')] is
            class_)

@pytest.mark.parametrize('class_',
                         [dc.StringSummary,
                          dc.ScalarSummary,
                          dc.ArraySummary,
                          dc.DictionarySummary,
                          dc.SignalSummary])
def test_datasummary_registered(class_):

    assert (dataclass._DATACLASS_SUMMARY_REGISTRY[getattr(class_, 'CLASS')] is
            class_)


def test_register():

    class DummySummary(dc.DataSummary):
        CLASS = 'dummy'
        GROUP = 'dummy'
        VERSION = 1

    @dataclass.register
    class Dummy(dc.DataObject):
        CLASS = 'dummy'
        GROUP = 'dummy'
        VERSION = 1
        SUMMARY_CLASS = DummySummary

    assert dataclass._DATACLASS_OBJECT_REGISTRY[Dummy.CLASS] is Dummy
    assert (dataclass._DATACLASS_SUMMARY_REGISTRY[DummySummary.CLASS] is
            DummySummary)

