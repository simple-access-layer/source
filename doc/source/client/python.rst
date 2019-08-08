Python Client
=============

The SAL Python client provides all the SAL operations and a set of Python classes that wrap the SAL data-classes.

To ease user interaction with the data system, the client uses Python objects to represent the objects in the data-tree. client side validation of data.

SALClient Class
---------------

.. autoclass:: sal.client.SALClient
   :members:

Report Classes
--------------

These classes are returned by :meth:`~sal.client.SALClient.list` operations and provide information on the data-tree nodes. To simplify working interactively, the :meth:`__repr__` method has been overridden in these classes to print a summary of the report to the console.

.. autoclass:: sal.core.object.BranchReport
   :members:

.. autoclass:: sal.core.object.LeafReport
   :members:

.. autoclass:: sal.core.object.ObjectReport
   :members:

Branch Class
------------

The branch class represents a branch object, it is returned by a :meth:`~sal.client.SALClient.get` operation on a branch node. The same object is passed to :meth:`~sal.client.SALClient.put` to create a branch node in the data-tree.

.. autoclass:: sal.core.object.Branch
   :members:

Data Classes
------------

The Python client contains a set of Python objects that represent the SAL data classes. These objects provide richer access to the data stored in a SAL data-object, typically providing calculated attributes and automatic validation of data.

Base Classes
~~~~~~~~~~~~

All the Python data-classes are derived from a common set of base classes.

**Full Object:**

.. autoclass:: sal.core.object.DataObject
   :members:
   :inherited-members:

   .. autoattribute:: sal.core.object.DataObject.CLASS
   .. autoattribute:: sal.core.object.DataObject.GROUP
   .. autoattribute:: sal.core.object.DataObject.VERSION

**Summary Object:**

.. autoclass:: sal.core.object.DataSummary
   :members:
   :inherited-members:

   .. autoattribute:: sal.core.object.DataSummary.CLASS
   .. autoattribute:: sal.core.object.DataSummary.GROUP
   .. autoattribute:: sal.core.object.DataSummary.VERSION


Scalar Class
~~~~~~~~~~~~~~

**Full Object:**

.. autoclass:: sal.dataclass.Scalar
   :members:

**Summary Object:**

.. autoclass:: sal.dataclass.ScalarSummary
   :members:


String Class
~~~~~~~~~~~~~~

**Full Object:**

.. autoclass:: sal.dataclass.String
   :members:

**Summary Object:**

.. autoclass:: sal.dataclass.StringSummary
   :members:


Dictionary Class
~~~~~~~~~~~~~~~~

**Full Object:**

.. autoclass:: sal.dataclass.Dictionary
   :members:

**Summary Object:**

.. autoclass:: sal.dataclass.DictionarySummary
   :members:


Array Class
~~~~~~~~~~~

**Full Object:**

.. autoclass:: sal.dataclass.Array
   :members:

**Summary Object:**

.. autoclass:: sal.dataclass.ArraySummary
   :members:


Signal Class
~~~~~~~~~~~~

The signal class is built from a main Signal object that contains sub-objects. The sub-objects define the signal dimensions, errors and masks.

**Full Object:**

.. autoclass:: sal.dataclass.Signal
   :members:

**Summary Object:**

.. autoclass:: sal.dataclass.SignalSummary
   :members:

Signal Dimensions
+++++++++++++++++

Dimensions are defined with one or more of the following dimension classes.

**Full Objects:**

.. autoclass:: sal.dataclass.CalculatedDimension
   :members:

.. autoclass:: sal.dataclass.ArrayDimension
   :members:

**Summary Objects:**

.. autoclass:: sal.dataclass.CalculatedDimensionSummary
   :members:

.. autoclass:: sal.dataclass.ArrayDimensionSummary
   :members:

Signal Errors
+++++++++++++

Signal data and dimension errors are defined using one of the following error classes.

**Full Objects:**

.. autoclass:: sal.dataclass.ConstantError
   :members:

.. autoclass:: sal.dataclass.SymmetricArrayError
   :members:

.. autoclass:: sal.dataclass.AsymmetricArrayError
   :members:

**Summary Objects:**

.. autoclass:: sal.dataclass.ConstantErrorSummary
   :members:

.. autoclass:: sal.dataclass.SymmetricArrayErrorSummary
   :members:

.. autoclass:: sal.dataclass.AsymmetricArrayErrorSummary
   :members:

Signal Masks
++++++++++++

Signal masks (for example quality indicators for the signal timepoints) are defined using one of the following classes.

**Full Objects:**

.. autoclass:: sal.dataclass.ScalarStatus
   :members:

.. autoclass:: sal.dataclass.ArrayStatus
   :members:

**Summary Objects:**

.. autoclass:: sal.dataclass.ScalarStatusSummary
   :members:


.. autoclass:: sal.dataclass.ArrayStatusSummary
   :members:




