module.exports = {
  request: function (req, token) {
    this.options.http._setHeaders.call(this, req, {Authorization: 'Bearer ' + token})
  },
  response: function (res) {
    // var refresh_token = res.data.refresh_token
    // how the fuck do I return a refresh token and place in local storage as well???
    // Check this:
    // https://github.com/websanova/vue-auth/issues/226
    var token = res.data.access_token
    return token
  }
}
