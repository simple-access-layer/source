from flask_restful import Resource, request, reqparse, current_app

from sal.core.serialise import serialise, deserialise
from sal.core.object import Branch, DataObject
from sal.core.exception import InvalidRequest
from sal.server.auth import authenticated_endpoint


# query argument validators
def _object_arg(value):
    """
    Validates the value of the optional object argument.

    :raises ValueError: If value is not None, 'full' or 'summary'.
    :param value: Argument value.
    :return: Validated value.
    """

    if value in [None, 'full', 'summary']:
        return value
    raise ValueError('Must be either \'full\' or \'summary\'.')


def _revision_arg(value):
    """
    Validates the value of the optional revision argument.

    :raises ValueError: If revision number is invalid.
    :param value: Argument value.
    :return: Validated value.
    """

    if value == 'head':
        return 0
    value = int(value)
    if value >= 0:
        return value
    raise ValueError('Must be \'head\' or an integer >= 0.')


# argument parsers for each method
get_parser = reqparse.RequestParser()
get_parser.add_argument('object', type=_object_arg, case_sensitive=False, default=None)
get_parser.add_argument('revision', type=_revision_arg, case_sensitive=False, default=0)

post_parser = reqparse.RequestParser()
post_parser.add_argument('source', type=str, case_sensitive=False, default=None)
post_parser.add_argument('source_revision', type=_revision_arg, case_sensitive=False, default=0)


class DataTree(Resource):

    decorators = [authenticated_endpoint]

    def __init__(self):
        self.persistence_provider = current_app.config['SAL']['PERSISTENCE']
        self.authorisation_provider = current_app.config['SAL']['AUTHORISATION']

    def get(self, path='', user=None):
        """
        List operation:

            GET http://<hostpath>/data/<path>?[revision=<revision/head>]

        Get operation:

            GET http://<hostpath>/data/<path>?object=<full/summary>[&revision=<revision/head>]

        """

        # todo: requests groups for user from authorisation provider and pass to persistence layer

        # parse query string options
        args = get_parser.parse_args()
        object_request = args['object']
        revision = args['revision']

        # construct node path
        path = '/{}:{}'.format(path, revision)

        if object_request:

            # request object
            summary = (object_request == 'summary')
            obj = self.persistence_provider.get(path, summary)

        else:

            # request report
            obj = self.persistence_provider.list(path)

        # generate response
        response = serialise(obj)
        response["request"] = {"url": request.url}
        return response

    def post(self, path='', user=None):
        """
        Put operation:

            POST http://<hostpath>/data/<path>

            branch and leaf content

        Copy operation:

            POST http://<hostpath>/data/<path>?source=<source_path>[&source_revision=<revision/head>]

        """

        # todo: requests groups for user from authorisation provider and pass to persistence layer

        # parse query string options
        args = post_parser.parse_args()
        source = args['source']
        source_revision = args['source_revision']

        # construct target path
        target = '/' + path

        if source:

            # new content is to be coped from a source path
            source = '{}:{}'.format(source, source_revision)
            self.persistence_provider.copy(target, source)

        else:

            # new content
            content = request.json

            try:
                obj = deserialise(content)
            except:
                raise InvalidRequest('Could not de-serialise content.')

            if not isinstance(obj, (Branch, DataObject)):
                # invalid content
                return InvalidRequest('Content does not describe a Branch or DataObject.')

            self.persistence_provider.put(target, obj)

        # return no content
        return '', 204

    def delete(self, path='', user=None):
        """
        Delete operation:

            DELETE http://<hostpath>/data/<path>

        """

        # todo: requests groups for user from authorisation provider and pass to persistence layer

        # request deletion
        self.persistence_provider.delete('/' + path)

        # return no content
        return '', 204
