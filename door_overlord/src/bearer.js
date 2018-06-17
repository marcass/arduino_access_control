module.exports = {
  request: function (req, token) {
    // parse the token from local storage after it was stringify'ed with the refresh token
    // console.log('REfresh = ' + token.split(';')[1])
    // token = token.split(';')[0]
    // console.log('Refresh token = ' + refreshToken)
    // token = token[0]
    // console.log('Actual token = ' + token)
    this.options.http._setHeaders.call(this, req, {Authorization: 'Bearer ' + token})
  },
  response: function (res) {
    // how the fuck do I return a refresh token and place in local storage as well???
    // Check this:
    // https://github.com/websanova/vue-auth/issues/226
    // console.log(res)
    // var refreshToken = res.data.refresh_token
    var token = res.data.access_token
    // stringify token and refresh for later use
    // return token + ';' + refreshToken
    return token
  }
}
