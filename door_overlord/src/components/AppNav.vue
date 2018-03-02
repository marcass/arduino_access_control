<template>
  <!-- <nav class="navbar navbar-default"> -->
    <div class="navbar-header">
      <expand-ball class="ball-pos">
        <span slot="menu" @click="clicked('M1')">M1</span>
        <span slot="menu" @click="clicked('M2')">M2</span>
      </expand-ball>
      <!-- <span v-show="!$auth.check()">
        <quick-menu :menu-count=nonauthcount :icon-class=nonauthicons :menu-url-list=nonauthlist></quick-menu>
      </span> -->
      <ul class="menu">
        <span v-show="!$auth.check()">
          <li class="menu">
            <router-link :to="{name: 'login'}">login</router-link>
          </li>
        </span>
        <span v-show="$auth.check()">
          <span v-show="$auth.check('admin')">
            <quick-menu :menu-count=authcount :icon-class=authicons :menu-url-list=authlist></quick-menu>
            <!-- <span v-if="$auth.user('admin')"> -->
            <li class="menu">
              <router-link :to="{name: 'Users'}">All Users</router-link>
            </li>
            <li class="menu">
              <router-link :to="{name: 'usekey'}">Actuate a door</router-link>
            </li>
            <li class="menu">
              <router-link :to="{name: 'listallowed'}">Allowed users</router-link>
            </li>
            <li class="menu">
              <router-link :to="{name: 'adduser'}">Add a user</router-link>
            </li>
            <li class="menu">
              <router-link :to="{name: 'edituser'}">Edit users</router-link>
            </li>
            <li class="menu">
              <router-link :to="{name: 'doors'}">Door status</router-link>
            </li>
            <li class="menu">
              <router-link :to="{name: 'statuslog'}">Door status log</router-link>
            </li>
          </span>
          <li class="menu">
            <router-link :to="{name: 'Hello'}">Hello</router-link>
          </li>
        <span v-show="$auth.check('user')">
          <li class="menu">
            <router-link :to="{name: 'userupdate'}">Update user details</router-link>
          </li>
        </span>
          <li class="menu">
            <a v-on:click='logout()' href="javascript:void(0);">Logout</a>
          </li>
          </span>
      </ul>
    </div>
  <!-- </nav> -->
</template>

<script>
// import VueExpandBall from 'vue-expand-ball'
// import quickMenu from 'vue-quick-menu'
// import log-in from 'vue-icon';
// import log-out from 'vue-icon';
// import user-check from 'vue-icon';
// import feather from 'vue-icon'
// import user-plus from 'vue-icon';
// import zap from 'vue-icon'
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
    },
    clicked (str) {
      alert(str + ' is just clicked!!')
    }
  },
  components: {
    // quickMenu
    // VueExpandBall
    // 'icons': require('vue-icons')
  }
  // data () {
  //   return {
  //     authcount: 4,
  //     nonauthcount: 2,
  //     authicons: ['icon-feather', 'icon-feather', 'icon', feather],
  //     // authicons: ['../assets/003-logout.png', '002-unlocked.png', '004-people.png', '005-profile.png'],
  //     authlist: ['userupdate', 'doors', 'adduser', 'listallowed'],
  //     // authlist:["<a v-on:click='logout()' href="javascript:void(0);">Logout</a>","<router-link :to="{name: 'usekey'}">Actuate a door</router-link>","<router-link :to="{name: 'edituser'}">Edit users</router-link>","<router-link :to="{name: 'adduser'}">Add a user</router-link>"],
  //     nonauthicons: ['001-login.png', '002-unlocked.png'],
  //     nonauthlist: ['https://google.com', 'https://google.com'],
  //     // nonauthlist:["<router-link :to="{name: 'login'}">login</router-link>","<router-link :to="{name: 'Hello'}">Hello</router-link>"],
  //     backgroundColor: '#17c4c5',
  //     color: '#ffffff',
  //     position: 'top-left'
  //   }
  // }
}
</script>

<script type="text/javascript" src="node_modules/vue/dist/vue.min.js"></script>

<script type="text/javascript" src="node_modules/vue-expand-ball/dist/vue-expand-ball.min.js"></script>

<script type="text/javascript">var VueExpandBall = window.VueExpandBall</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.navbar-right { margin-right: 0px !important}

.ball-pos {
    bottom: 200px;
    right: 200px;
}

.log {
  margin: 5px 10px 0 0;
}
</style>
