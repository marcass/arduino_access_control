<template>
  <div class="doors">
    <app-nav></app-nav>
    <h1>Update keycode for {{ this.username }}</h1>
    <div class="col-lg-7">
      <ul>
        <li>
          Keycode: <input v-model="keycode" :placeholder="this.keycode">
        </li>
        <li>
          Password: <input type="password" v-model="password">
        </li>
        <li>
          <button v-on:click="passwordCheck(password, keycode)">Submit</button>
        </li>
      </ul>
   </div>
   <div v-if="this.resp != ''">
     {{ this.resp.data.Message }}
   </div>
  </div>
</template>

<script>
import { putUserData, getUser, getVerifyUser } from '../../../utils/door-api'
import AppNav from '../AppNav'
export default {
  name: 'userupdate',
  data () {
    return {
      // user: this.auth.user().username,
      keycode: '',
      username: '',
      status: 'Pending',
      verified: '',
      password: '',
      resp: ''
    }
  },
  components: {
    AppNav
  },
  methods: {
    changepin (payload) {
      putUserData(JSON.stringify(payload), 'keycode').then((ret) => {
        this.resp = ret
      })
    },

    getUserKey () {
      getUser(this.$auth.user().username).then((ret) => {
        this.keycode = ret.keycode
      })
    },
    passwordCheck (pass, key) {
      getVerifyUser(this.username, {'password': pass}).then((ret) => {
      // getVerifyUser(this.$auth.user().username, password).then((ret) => {
        this.verified = ret
        if (this.verified.status === 'passed') {
          this.changepin({'username': this.username, 'keycode': key})
        } else {
          this.resp = {'data': {'Status': 'Error', 'Message': 'Password check failed'}}
        }
        // console.log(this.resp)
      })
    }
  },
  mounted () {
    this.getUserKey()
    this.username = this.$auth.user().username
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1, h2 {
  font-weight: normal;
}

div.col-sm-3 {
  text-align: left;
    padding-top: 40px;
}

div.col-lg-7 {
  clear: both;;
  margin: 15px;
}

div.col-lg-3 {
  clear: right;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b283;
}
</style>
