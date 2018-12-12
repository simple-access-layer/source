from flask_restful import Resource, request, current_app
from sal.core.version import VERSION
from sal.core.object import dataclass
from sal.server.auth import auth_required


class ServerInfo(Resource):
    """
    Handler for the server root resource.
    """

    def get(self):
        """
        Returns server configuration information.
        """

        api_version = current_app.config['SAL']['API_VERSION']

        # authentication required
        requires_auth = auth_required()

        # build list of resources
        resources = ['data']
        if requires_auth:
            resources.append('auth')

        return {
            'host': request.base_url,
            'api': {
                'version': api_version,
                'requires_auth': requires_auth,
                'resources': resources,
                'classes': dataclass.list()
            },
            'service': {
                'name': 'Simple Access Layer (SAL) Server',
                'version': VERSION
            },
            'request': {
                'url': request.url
            }
        }
