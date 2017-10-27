<template>
  <div>
    <h1>Login</h1>

    <b>Test users:</b> (admin / password)

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
        <td><button type="submit">Login</button></td>
      </tr></table>

      <hr/>

      <div v-show="error" style="color:red; word-wrap:break-word;">{{ error | json }}</div>
    </form>
  </div>
</template>

<script>
  import { postCreds } from '../../../utils/door-api'
  export default {
    data () {
      return {
        // context: 'login context',
        token: '',
        refresh_token: '',
        data: {
          body: {
            username: 'admin',
            password: 'password'
          },
          rememberMe: false,
          fetchUser: true
        },

        error: null
      }
    },

    mounted () {
      console.log(this.$auth.redirect())

      // Can set query parameter here for auth redirect or just do it silently in login redirect.
    },

    methods: {
      login () {
        // console.log(JSON.stringify(this.data.body))
        postCreds(JSON.stringify(this.data.body)).then((res) => {
          console.log('RES', res)
          console.log('access token', res.data.access_token)
          console.log('refresh token', res.data.refresh_token)
          this.token = res.data.access_token
          this.refresh_token = res.data.refresh_token
        })
        console.log(this.token)
        console.log(this.refresh_token)
        // var redirect = this.$auth.redirect()
        // this.$auth.login({
        //   // body: this.data.body, // Vue-resource
        //   data: this.data.body, // Axios
        //   rememberMe: this.data.rememberMe,
        //   redirect: {name: redirect ? redirect.from.name : 'users'},
        //   fetchUser: this.data.fetchUser,
        //   success () {
        //     console.log('success ' + this.context)
        //   },
        //   error (res) {
        //     console.log('error ' + this.context)
        //
        //     this.error = res.data
        //   }
        // })
      }
    }
  }
</script>
