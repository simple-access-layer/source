from abc import ABC, abstractmethod

class Node(ABC):

    def __init__(self, description):

        self.description = str(description)

    @abstractmethod
    def serialise(self, additional):

        raise NotImplementedError

    @classmethod
    @abstractmethod
    def from_dict(cls, dict):

        raise NotImplementedError

    @abstractmethod
    def to_dict(self):

        raise NotImplementedError