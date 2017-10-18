from flask import Flask, jsonify, request
from flask_jwt_extended import JWTManager

from flask_cors import CORS, cross_origin


import sql
import mqtt

sql.setup_db()

app = Flask(__name__)
CORS(app)

import views_users
import views_auth

if __name__ == '__main__':
    app.run()
