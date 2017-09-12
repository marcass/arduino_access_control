import Vue from 'vue'
import Router from 'vue-router'
import Hello from '@/components/Hello'
import Users from '@/components/Users'
import Doors from '@/components/Doors'
import UpdateUser from '@/components/UpdateUser'

Vue.use(Router)

export default new Router({
  routes: [
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
    }
  ]
})
