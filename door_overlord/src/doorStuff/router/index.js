import Vue from 'vue'
import Router from 'vue-router'
import Hello from '@/components/Hello'
import Users from '@/components/Users'
import Doors from '@/components/Doors'
import UpdateUser from '@/components/UpdateUser'
import AddUser from '@/components/AddUser'
import UseKey from '@/components/UseKey'
import Register from '@/components/Register'

// Vue.use(Router)
// for jwt:
Vue.router = Router

Vue.use(require('@websanova/vue-auth'), {
  auth: require('@websanova/vue-auth/drivers/auth/bearer.js'),
  http: require('@websanova/vue-auth/drivers/http/axios.1.x.js'),
  Router: require('@websanova/vue-auth/drivers/router/vue-router.2.x.js')
})

export default new Router({
  routes: [
    {
      path: '/auth',
      name: 'Register',
      component: Register,
      meta: {auth: false}
    },
    {
      path: '/hello',
      name: 'Hello',
      component: Hello,
      meta: {auth: false}
    },
    {
      path: '/',
      name: 'Users',
      component: Users,
      meta: {auth: ['admin']}
    },
    {
      path: '/updateuser',
      name: 'UpdateUser',
      component: UpdateUser,
      meta: {auth: true}
    },
    {
      path: '/doors',
      name: 'Doors',
      component: Doors,
      meta: {auth: true}
    },
    {
      path: '/usekey',
      name: 'UseKey',
      component: UseKey,
      meta: {auth: true}
    },
    {
      path: '/adduser',
      name: 'AddUser',
      component: AddUser,
      meta: {auth: ['admin']}
    }
  ]
})
