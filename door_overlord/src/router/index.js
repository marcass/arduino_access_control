import Vue from 'vue'
import Router from 'vue-router'
import HelloWorld from '@/components/HelloWorld'
import Users from '@/components/pages/admin/Users'
import Adduser from '@/components/pages/admin/AddUser'
import Listallowed from '@/components/pages/admin/ListAllowed'
import Login from '@/components/pages/Login'
import Doors from '@/components/pages/Doors'
import Usekey from '@/components/pages/UseKey'
import Updateuser from '@/components/pages/admin/UpdateUser'

// Vue.router = Router
Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'Hello',
      component: HelloWorld
    },
    {
      path: '/users',
      name: 'Users',
      component: Users,
      // meta: {auth: {roles: 'admin'}}
      meta: {auth: true}
    },
    {
      path: '/auth/login',
      name: 'login',
      component: Login,
      meta: {auth: false}
    },
    {
      path: '/adduser',
      name: 'adduser',
      component: Adduser,
      meta: {auth: true}
    },
    {
      path: '/listallowed',
      name: 'listallowed',
      component: Listallowed,
      meta: {auth: true}
    },
    {
      path: '/doors',
      name: 'doors',
      component: Doors,
      meta: {auth: true}
    },
    {
      path: '/usekey',
      name: 'usekey',
      component: Usekey,
      meta: {auth: true}
    },
    {
      path: '/updateuser',
      name: 'edituser',
      component: Updateuser,
      meta: {auth: true}
    }
  ]
})
