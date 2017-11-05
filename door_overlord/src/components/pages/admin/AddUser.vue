<template>
  <div class="doors">
    <app-nav></app-nav>
    <h1>Existing users</h1>
    <p v-for="item in userlist"> {{ item.username }} </p>
   <div class="col-md-5">
     <li>Username:
       <input v-model="username">
     </li>
     <li>Password:
       <input v-model="password">
     </li>
     <li>Keycode. Must consist of at least four of 1-9,A-D:
       <input v-model="keycode">
     </li>
     <li>Valid from:
       <date-picker v-model="startDateObject" :config="config"></date-picker>
     </li>
     <li>Expires:
       <date-picker v-model="endDateObject" :config="config"></date-picker>
      </li>
      <li>
        <h4>
          Enabled doors:
        </h4>
        <div class='radio' v-for="x in doorlist">
          <input type="checkbox" :id="x" :value="x" v-model="enableddoorlist">
          <label >{{ x }}</label>
        </div>
      </li>
      <li>
        <div class='radio'>
          <h4>Role:</h4>
          <input type="radio" id="admin" value="admin" v-model="role">
          <label for="admin">Admin</label>
          <br>
          <input type="radio" id="user" value="user" v-model="role">
          <label for="user">User</label>
        </div>
      </li>
      <li>
        <h4>
          Enabled:
        </h4>
        <input type="checkbox" id="checkbox" v-model="enabled">
      </li>
      <li>
        <button v-on:click="postData(JSON.stringify({'username':username, 'password':password, 'role':role, 'keycode': keycode, 'enabled': enabled, 'timeStart': startDateObject, 'timeEnd': endDateObject, 'doorlist': enableddoorlist}))">Submit</button>
      </li>
      <li class="response">
        Result: {{ this.resp }}
      </li>
   </div>
  </div>
</template>

<script>
import { getUsers, getDoors, postUserData } from '../../../../utils/door-api'
import 'bootstrap/dist/css/bootstrap.css'
import datePicker from 'vue-bootstrap-datetimepicker'
import AppNav from '../../AppNav'
import 'eonasdan-bootstrap-datetimepicker/build/css/bootstrap-datetimepicker.css'
export default {
  name: 'updateuser',
  data () {
    return {
      doorlist: [],
      userlist: [],
      message: '',
      keycode: '',
      resp: '',
      enableddoorlist: [],
      config: {
        format: 'ddd, MMM Do YYYY, HH:mm'
      },
      username: '',
      password: '',
      role: '',
      endDateObject: '',
      startDateObject: '',
      enabled: ''
    }
  },
  components: {
    datePicker,
    AppNav
  },
  methods: {
    postData (payload) {
      postUserData(payload).then((ret) => {
        this.resp = ret.data.status
      })
      // this.$router.push('/users')
    },
    getDoors () {
      getDoors().then((ret) => {
        this.doorlist = ret
      })
    },
    getUsers () {
      getUsers().then((ret) => {
        this.userlist = ret
      })
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
.radio {
  text-align: left;
}

/*span {
  vertical-align: top;
}*/

h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  vertical-align: top;
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b283;
}
</style>
