# Mostly obtained from:
# https://github.com/vimalloc/flask-jwt-extended
# also check out: https://gist.github.com/jslvtr/139cf76db7132b53f2b20c5b6a9fa7ad
import sql
from flask import Flask, request, jsonify
from flask_jwt_extended import jwt_required, \
    create_access_token, jwt_refresh_token_required, \
    create_refresh_token, get_jwt_identity

from init import app, jwt

app.secret_key = 'ksajdkhsadulaulkj1092830983no1y24'  # Change this!
app.config['JWT_HEADER_TYPE'] = 'Bearer'
#jwt = JWTManager(app)

# import os
# if os.environ.has_key('LOGIN_PW'):
#   pw = os.environ['LOGIN_PW']
# else:
#   pw = 'password'

# @app.route('/auth/login', methods=['POST'])
# def auth():
#     username = request.json.get('username', None)
#     password = request.json.get('password', None)
#     if username != 'admin' or password != pw:
#         return jsonify({"msg": "Bad username or password"}), 401
#
#     # Use create_access_token() and create_refresh_token() to create our
#     # access and refresh tokens
#     ret = {
#         'access_token': create_access_token(identity=username),
#         'refresh_token': create_refresh_token(identity=username)
#     }
#     print ret
#     return jsonify(ret), 200

@app.route('/auth/login', methods=['POST'])
def auth():
    username = request.json.get('username', None)
    password = request.json.get('password', None)
    print username
    print password
    if sql.get_user(username, password):
        # Use create_access_token() and create_refresh_token() to create our
        # access and refresh tokens
        ret = {
            'access_token': create_access_token(identity=username),
            'refresh_token': create_refresh_token(identity=username)
        }
        return jsonify(ret), 200
    else:
        return jsonify({"msg": "Bad username or password"}), 401

# The jwt_refresh_token_required decorator insures a valid refresh
# token is present in the request before calling this endpoint. We
# can use the get_jwt_identity() function to get the identity of
# the refresh token, and use the create_access_token() function again
# to make a new access token for this identity.
@app.route('/refresh', methods=['POST'])
@jwt_refresh_token_required
def refresh():
    current_user = get_jwt_identity()
    ret = {
        'access_token': create_access_token(identity=current_user)
    }
    return jsonify(ret), 200

@jwt.expired_token_loader
def my_expired_token_callback():
    return jsonify({
        'status': 401,
        'sub_status': 101,
        'msg': 'The token has expired'
    }), 401
