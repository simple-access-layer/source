from abc import ABC, abstractmethod

class Node(ABC):

    def __init__(self, description):

        self.description = description

    @property
    def description(self):
        """
        A string describing the branch contents.
        """
        return self._description

    @description.setter
    def description(self, value):
        if not isinstance(value, str):
            raise ValueError("Description must be a string.")
        if not value:
            raise ValueError("A description cannot be an empty string.")
        self._description = value

    @classmethod
    @abstractmethod
    def from_dict(cls, dict):

        raise NotImplementedError

    @abstractmethod
    def to_dict(self):

        raise NotImplementedError