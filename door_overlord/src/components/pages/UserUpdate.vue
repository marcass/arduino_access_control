<template>
  <div class="doors">
    <app-nav></app-nav>
    <h1>Update keycode for {{ $auth.user().username }}</h1>
    <div class="col-lg-7">
         Keycode: <input v-model="keycode" :placeholder="keycode" v-on:keyup.enter="changeattr($auth.user().username, 'keycode', keycode)">
        <button v-on:click="sendData(JSON.stringify({'username': $auth.user().username, 'keycode': item.keycode, 'enabled': item.enabled, 'timeStart': item.startDateObject, 'timeEnd': item.endDateObject, 'doorlist': item.doors}))">Submit user data</button>
   </div>
  </div>
</template>

<script>
import { getUsers, putUserData, putAllUserData } from '../../../utils/door-api'
import AppNav from '../../AppNav'
export default {
  name: 'updateuser',
  data () {
    return {
      user: '',
      keycode: '',
    }
  },
  components: {
    AppNav
  },
  methods: {
    sendData (payload) {
      putAllUserData(payload)
    },
    changeattr (userin, attr, val) {
      // var key = attr
      // axios.put(url,{'username': this.username, 'keycode': message})
      const payload = JSON.stringify({'username': userin, [attr]: val})
      // const pl = {username: item.username, keycode: message}
      putUserData(payload, attr)
      console.log(payload)
      return 1
    }
  },
  mounted () {
    this.getDoors()
    this.getUsers()
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
