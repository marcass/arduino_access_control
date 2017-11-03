<template>
  <nav class="navbar navbar-default">
    <div class="navbar-header">
      <!-- <router-link to="/" class="navbar-brand"> fuck you by mw</router-link>
    </div>
    <ul class="nav navbar-nav navbar-right">
      <li>
        <button class="auth button" v-show="isLoggedIn()" @click="handleLogout()">Log out </button>
        <button class="auth button" v-show="!isLoggedIn()" @click="handleLogin()">Log In</button>
      </li>
    </ul> -->
      <ul>
        <li>
          <!-- <a v-link="'login'">Login</a> -->
          <span v-show="!$auth.check()">
            <router-link :to="{name: 'login'}">login</router-link>
          </span>
          <span v-show="$auth.check('admin')">
            <!-- <span v-if="$auth.user('admin')"> -->
              <router-link :to="{name: 'Users'}">All Users</router-link> &bull;
              <router-link :to="{name: 'usekey'}">Open a door</router-link> &bull;
              <router-link :to="{name: 'listallowed'}">Allowed users</router-link> &bull;
              <router-link :to="{name: 'adduser'}">Add a user</router-link> &bull;
              <router-link :to="{name: 'edituser'}">Edit users</router-link> &bull;
              <router-link :to="{name: 'doors'}">Door status</router-link> &bull;
            <!-- </span>
            <span v-else> -->
            </span>
            <span v-show="$auth.check()">
              <router-link :to="{name: 'Hello'}">Hello</router-link> &bull;
              <router-link :to="{name: 'userupdate'}">Update keycode</router-link> &bull;
              <a v-on:click='logout()' href="javascript:void(0);">Logout</a> &bull;
            </span>
          </span>
        </li>
      </ul>
    </div>
  </nav>
</template>

<script>
// import { isLoggedIn, login, logout } from '../../utils/auth'
//
// import role from './pages/Login'
export default {
  name: 'app-nav',
  methods: {
    logout () {
      this.$auth.logout({
        makeRequest: false,
        success () {
          console.log('success ' + this.context)
        },
        error () {
          console.log('error ' + this.context)
        }
      })
    }
    // checkRole () {
    //   if (this.role === 'admin') {
    //     return 'admin'
    //   } else {
    //     return 'user'
    //   }
    // }
  }
  // components: {
  //   role
  // }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.navbar-right { margin-right: 0px !important}

.log {
  margin: 5px 10px 0 0;
}
</style>
