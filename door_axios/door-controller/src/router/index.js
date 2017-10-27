import Vue from 'vue'
import Router from 'vue-router'
import HelloWorld from '@/components/HelloWorld'
import Users from '@/components/pages/admin/Users'
import Login from '@/components/pages/Login'

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
      meta: {auth: true}
    },
    {
      path: '/auth/login',
      name: 'Login',
      component: Login,
      meta: {auth: false}
    }
  ]
})
