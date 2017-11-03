<template>
  <div class="doors">
    <app-nav></app-nav>
    <h1>Update keycode for {{ this.username }}</h1>
    <div class="col-lg-7">
         Keycode: <input v-model="keycode" :placeholder="this.keycode" v-on:keyup.enter="changepin({'username':username, 'keycode': keycode})">
         <h4>Status: {{ status }}</h4>
   </div>
  </div>
</template>

<script>
import { putUserData, getUser } from '../../../utils/door-api'
import AppNav from '../AppNav'
export default {
  name: 'userupdate',
  data () {
    return {
      // user: this.auth.user().username,
      keycode: '',
      username: '',
      status: 'Pending'
    }
  },
  components: {
    AppNav
  },
  methods: {
    changepin (payload) {
      putUserData(JSON.stringify(payload), 'keycode')
      this.status = 'Success'
      return 1
    },
    getUserKey () {
      getUser(this.$auth.user().username).then((ret) => {
        this.keycode = ret.keycode
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
