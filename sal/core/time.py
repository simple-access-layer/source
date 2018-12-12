import sys
from datetime import datetime


def new_timestamp():
    """
    Generates a timestamp for the current time.
    
    :return: A datetime object.
    """
    return datetime.today()


def encode_timestamp(timestamp):
    """
    Encodes a timestamp as an ISO 8601 string.
    
    :param timestamp: A datetime object.
    :return: An ISO 8601 string.
    """

    return timestamp.strftime("%Y-%m-%dT%H:%M:%S.%f")


def decode_timestamp(timestamp):
    """
    Decodes a timestamp from an ISO 8601 string.

    :param timestamp: An ISO 8601 string. 
    :return: A datetime object.
    """
    return datetime.strptime(timestamp, "%Y-%m-%dT%H:%M:%S.%f")


def validate_timestamp(timestamp):
    """
    
    :param timestamp: An ISO 8601 string or datetime object. 
    :return: A datetime object.
    """
    if isinstance(timestamp, str):
        timestamp = decode_timestamp(timestamp)
    elif not isinstance(timestamp, datetime):
        raise ValueError('Timestamp must be a datetime object or an ISO 8601 compliant string representation.')
    return timestamp



