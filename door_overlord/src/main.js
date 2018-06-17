// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import AppNav from './components/AppNav'
import router from './router'
import VueAuth from '@websanova/vue-auth'
const BASE_URL = 'https://skibo.duckdns.org/api'

Vue.router = router

Vue.use(VueAuth, {
  auth: require('./bearer.js'),
  // http: require('./axios.1.x.js'),
  // Path specific stuff in http so need to keep modified files in original path (update for git!)
  http: require('@websanova/vue-auth/drivers/http/axios.1.x.js'),
  router: require('@websanova/vue-auth/drivers/router/vue-router.2.x.js'),
  loginData: {url: BASE_URL + '/auth/login', method: 'POST', redirect: {name: 'DoorOverlord'}},
  authRedirect: {path: '/'},
  fetchData: {url: BASE_URL + '/auth/login', method: 'POST', enabled: false},
  refreshData: {enabled: false},
  rolesVar: 'role',
  logoutData: {url: 'auth/logout', method: 'POST', redirect: '/', makeRequest: false}
  // fetchData: {
  //   // url: '/auth/user/blah',
  //   // method: 'GET',
  //   enabled: false
  // }
  // _parseUserData: function (data) {
  //   console.log(data.data)
  //   return data.data
  // }
})

Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  template: '<App/>',
  components: { App, AppNav }
})
