from flask.json import jsonify
from flask_restful import Api

class ErrorHandlerApi(Api):

    """
    Subclasses flask_restul.Api to add error handling 
    """

    def handle_error(self, e):
        """
        Converts the error (including any custom messages/description) to JSON

        :param e: The exception to handle
        :type e: werkzeug.exceptions.HTTPException
        :return: Tuple of JSON equivalent of `e` and its status code
        """

        return jsonify({"message":str(e)}), e.code