# Mostly obtained from:
# https://github.com/vimalloc/flask-jwt-extended
# also check out: https://gist.github.com/jslvtr/139cf76db7132b53f2b20c5b6a9fa7ad
import sql
from flask import Flask, request, jsonify
from flask_jwt_extended import jwt_required, \
    create_access_token, jwt_refresh_token_required, \
    create_refresh_token, get_jwt_identity, get_jwt_claims

from init import app, jwt
import creds

app.secret_key = creds.jwt_secret  # Change this!
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

# http://flask-jwt-extended.readthedocs.io/en/latest/tokens_from_complex_object.html
# Create a function that will be called whenever create_access_token
# is used. It will take whatever object is passed into the
# create_access_token method, and lets us define what custom claims
# should be added to the access token.
@jwt.user_claims_loader
def add_claims_to_access_token(user):
    return {'role': user['role']}


# Create a function that will be called whenever create_access_token
# is used. It will take whatever object is passed into the
# create_access_token method, and lets us define what the identity
# of the access token should be.
@jwt.user_identity_loader
def user_identity_lookup(user):
    return user['username']

@app.route('/auth/login', methods=['POST'])
def auth():
    try:
        username = request.json.get('username', None)
        password = request.json.get('password', None)
        # print username
        # print password
        content = sql.auth_user(username, password)
        # print content
        if content['status'] == 'passed':
            # Create user and role object
            user = {'username':username, 'role':content['role']}
            # Use create_access_token() and create_refresh_token() to create our
            # access and refresh tokens
            ret = {
                'access_token': create_access_token(identity=user),
                'refresh_token': create_refresh_token(identity=user), 'data':{
                'role': content['role']}
            }
            # print ret
            return jsonify(ret), 200
        else:
            # print 'fucked up with a bad username'
	    return jsonify({"msg": "Bad username or password"}), 401
    except:
        print 'empty request'
	return jsonify({'Status':'Error', 'Message':'Empty request'})

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
