// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import AppNav from './components/AppNav'
import router from './router'
import VueAuth from '@websanova/vue-auth'

Vue.router = router

Vue.use(VueAuth, {
  auth: require('@websanova/vue-auth/drivers/auth/bearer.js'),
  http: require('@websanova/vue-auth/drivers/http/axios.1.x.js'),
  router: require('@websanova/vue-auth/drivers/router/vue-router.2.x.js'),
  authRedirect: {path: '/users'},
  refreshData: {enabled: false},
  rolesVar: 'role',
  fetchData: {
    url: '/auth/user',
    method: 'GET',
    enabled: true
  // },
  // _parseUserData: function (data) {
  //   console.log(data.data)
  //   return data.data
  }
})

Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  template: '<App/>',
  components: { App, AppNav }
})
