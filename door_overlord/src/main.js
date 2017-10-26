import App from './components/App.vue';

// Router
Vue.router = new VueRouter({
    hashbang: false,
    linkActiveClass: 'active',
    mode: 'history',
    base: __dirname,
    routes: [{
        path: '/',
        name: 'default',
        component: require('./components/pages/Hello.vue')
    }, {
        path: '/login',
        name: 'login',
        component: require('./components/pages/Login.vue'),
        meta: {auth: false}
    }, {
        path: '/admin',
        name: 'admin',
        component: require('./components/pages/Admin.vue'),
        meta: {auth: {roles: 'admin', redirect: {name: 'login'}, forbiddenRedirect: '/403'}},
        children: [{
            path: 'users',
            name: 'users-list',
            component: require('./components/pages/admin/Users.vue'),
        },
        {
          path: 'adduser',
          name: 'add-user',
          component: require('./components/pages/admin/AddUser.vue')
        }]
    }, {
        path: '/404',
        name: 'error-404',
        component: require('./components/pages/404.vue')
    }, {
        path: '/403',
        name: 'error-403',
        component: require('./components/pages/403.vue')
    }, {
        path: '/502',
        name: 'error-502',
        component: require('./components/pages/502.vue')
    }]
});

import axios from 'axios';
import VueAxios from 'vue-axios';
Vue.use(VueAxios, axios);
Vue.axios.defaults.baseURL = 'https://skibo.duckdns.org/api';

// Http
// Vue.http.options.root = 'https://api-demo.websanova.com/api/v1';
//
// Vue.http.interceptors.push(function(request, next) {
//     next(function (res) {
//         if (res.status === 401) {
//             Vue.auth.logout();
//         }
//     });
// });

// Vue Auth
// Vue.use(require('../../../src/index.js'), {
//     auth: require('../../../drivers/auth/bearer.js'),
//     http: require('../../../drivers/http/vue-resource.1.x.js'),
//     // http: require('../../../drivers/http/axios.1.x.js'),
//     router: require('../../../drivers/router/vue-router.2.x.js'),
//     rolesVar: 'role'
// });
Vue.use(require('@websanova/vue-auth'), {
  auth: require('@websanova/vue-auth/drivers/auth/bearer.js'),
  http: require('@websanova/vue-auth/drivers/http/axios.1.x.js'),
  router: require('@websanova/vue-auth/drivers/router/vue-router.2.x.js'),
  rolesVar: 'role'
})

// Start
var component = require('./components/App.vue');

component.router = Vue.router;

new Vue(component).$mount('#app');
