import Vue from 'vue'
import Router from 'vue-router'
// import Hello from '@/components/pages/Hello'
import Users from '@/components/pages/admin/Users'
import AddUser from '@/components/pages/admin/AddUser'
import Listallowed from '@/components/pages/admin/ListAllowed'
import Login from '@/components/pages/Login'
import Doors from '@/components/pages/Doors'
import DoorOverlord from '@/components/pages/UseKey'
import Updateuser from '@/components/pages/admin/UpdateUser'
import Userupdate from '@/components/pages/UserUpdate'
import StatusLog from '@/components/pages/StatusLog'
import Three from '@/components/pages/403'
import Four from '@/components/pages/404'
import Logout from '@/components/pages/Logout'
import SetupDoor from '@/components/pages/SetupDoor'
import Boiler from '@/components/pages/Boiler'

// Vue.router = Router
Vue.use(Router)

export default new Router({
  // mode: 'history',
  routes: [
    {
      path: '/auth/logout',
      name: 'Logout',
      component: Logout
    },
    {
      path: '/door/setup',
      name: 'SetupDoor',
      component: SetupDoor
    },
    {
      path: '/users',
      name: 'Users',
      component: Users,
      meta: {auth: {roles: 'admin'}}
      // meta: {auth: true}
    },
    {
      path: '/',
      name: 'Login',
      component: Login
      // meta: {auth: false}
    },
    {
      path: '/adduser',
      name: 'AddUser',
      component: AddUser,
      meta: {auth: {roles: 'admin'}}
      // meta: {auth: true}
    },
    {
      path: '/listallowed',
      name: 'ListAllowed',
      component: Listallowed,
      meta: {auth: {roles: 'admin'}}
      // meta: {auth: true}
    },
    {
      path: '/doors',
      name: 'Doors',
      component: Doors,
      meta: {auth: true}
    },
    {
      path: '/usekey',
      name: 'DoorOverlord',
      component: DoorOverlord,
      meta: {auth: true}
    },
    {
      path: '/updateuser',
      name: 'EditUser',
      component: Updateuser,
      meta: {auth: {roles: 'admin'}}
      // meta: {auth: true}
    },
    {
      path: '/userupdate',
      name: 'UserUpdate',
      component: Userupdate,
      meta: {auth: ['admin', 'user']}
      // meta: {auth: true}
    },
    {
      path: '/statuslog',
      name: 'StatusLog',
      component: StatusLog,
      meta: {auth: ['admin']}
      // meta: {auth: true}
    },
    {
      path: '/boiler',
      name: 'Boiler',
      component: Boiler,
      meta: {auth: ['admin', 'user']}
    },
    {
      path: '/403',
      name: '403',
      component: Three
    },
    {
      path: '/404',
      name: '404',
      component: Four
    }
  ]
})
