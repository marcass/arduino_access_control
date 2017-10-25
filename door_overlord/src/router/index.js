import Vue from 'vue'
import Router from 'vue-router'
import Hello from '@/components/Hello'
import Users from '@/components/Users'
import Doors from '@/components/Doors'
import UpdateUser from '@/components/UpdateUser'
import AddUser from '@/components/AddUser'
import UseKey from '@/components/UseKey'

// Vue.use(Router)
// for jwt:
Vue.router = Router
Vue.use(require('@websanova/vue-auth'), {
  auth: require('@websanova/vue-auth/drivers/auth/bearer.js'),
  http: require('@websanova/vue-auth/drivers/http/axios.1.x.js'),
  router: require('@websanova/vue-auth/drivers/router/vue-router.2.x.js')
})

export default new Router({
  routes: [
    {
      path: '/auth',
      name: 'Auth',
      component: Auth
    },
    {
      path: '/hello',
      name: 'Hello',
      component: Hello
    },
    {
      path: '/',
      name: 'Users',
      component: Users
    },
    {
      path: '/updateuser',
      name: 'UpdateUser',
      component: UpdateUser
    },
    {
      path: '/doors',
      name: 'Doors',
      component: Doors
    },
    {
      path: '/usekey',
      name: 'UseKey',
      component: UseKey
    },
    {
      path: '/adduser',
      name: 'AddUser',
      component: AddUser
    }
  ]
})
