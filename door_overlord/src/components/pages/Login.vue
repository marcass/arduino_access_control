<template>
  <div>
    <h1>Login</h1>
    <hr/>
    <form v-on:submit.prevent="login()">
      <table><tr>
        <td>Username:</td>
        <td><input v-model="data.body.username" /></td>
      </tr><tr>
        <td>Password:</td>
        <td><input v-model="data.body.password" type="password" /></td>
      </tr><tr>
        <td></td>
        <td><label><input v-model="data.rememberMe" type="checkbox" /> Remember Me</label></td>
      </tr><tr>
        <td></td>
        <!-- <td><label><input v-model="data.fetchUser" type="checkbox" /> Fetch User (test)</label></td> -->
      </tr><tr>
        <td></td>
        <td><button v-on:click="login()">Login</button></td>
        <!-- <td><button type="submit">Login</button></td> -->
      </tr></table>

      <hr/>

      <div v-show="error" style="color:red; word-wrap:break-word;">{{ error | json }}</div>
    </form>
  </div>
</template>

<script>
  // import { postCreds } from '../../../utils/door-api'
  export default {
    data () {
      return {
        // context: 'login context',
        token: '',
        refresh_token: '',
        role: '',
        data: {
          body: {
            username: '',
            password: ''
          },
          rememberMe: true,
          fetchUser: false
          // redirect: '/users'
        },

        error: null
      }
    },
    // components: {
    //   role
    // },
    mounted () {
      console.log(this.$auth.redirect())

      // Can set query parameter here for auth redirect or just do it silently in login redirect.
    },

    methods: {
      login () {
        var redirect = this.$auth.redirect()
        this.$auth.login({
          // url: 'https://skibo.duckdns.org/tanktestapi/auth/login',
          data: this.data.body, // Axios
          rememberMe: this.data.rememberMe,
          redirect: {name: redirect ? redirect.from.name : 'DoorOverlord'},
          fetchUser: this.data.fetchUser,
          success (res) {
            // console.log(res)
            var roleIn = res.data.data.role
            // this.$auth.refresh({ data: this.data })
            this.$auth.user({'role': roleIn, 'username': this.data.body.username})
            // console.log(res)
            // console.log('user = ' + this.$auth.user().username + ' role = ' + this.$auth.user().role)
          },
          error (res) {
            this.message = {'Status': 'Error', 'Message': 'Incorrect username or password'}
          }
        })
      }
    }
  }
</script>
