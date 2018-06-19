<template>
  <div class="doors">
    <app-nav></app-nav>
    <h2>Add a user</h2>
    <div class="col-2">
      <h3>Existing users</h3>
      <p v-for="item in userlist"> {{ item.username }} </p>
    </div>
    <div class="col-2">
     <li><h3>Username (if burner it is onetime use)</h3>
       <input v-model="username">
     </li>
   </div>
   <div class="col-2">
     <li><h3>Password</h3>
       <input v-model="password">
     </li>
   </div>
   <div class="col-2">
     <li><h3>Keycode</h3>
        Must consist of at least four of 1-9,A-D:
       <input v-model="keycode">
     </li>
   </div>
   <div class="col-2">
      <li>
        <h3>
          Enabled doors
        </h3>
        <div class='radio' v-for="x in doorlist">
          <input type="checkbox" :id="x" :value="x" v-model="enableddoorlist">
          <label >{{ x }}</label>
        </div>
      </li>
    </div>
    <div class="col-2">
      <li>
        <div class='radio'>
          <h3>Role</h3>
          <input type="radio" id="admin" value="admin" v-model="role">
          <label for="admin">Admin</label>
          <br>
          <input type="radio" id="user" value="user" v-model="role">
          <label for="user">User</label>
        </div>
      </li>
    </div>
    <div class="col-2">
      <li>
        <h3>
          Tick to mark enabled
        </h3>
        <input type="checkbox" id="checkbox" v-model="enabled">
      </li>
    </div>
    <div class="col-5 hack">
      <li><h3>Valid from</h3>
        <date-picker v-model="startDateObject" :config="config"></date-picker>
      </li>
    </div>
    <div class="col-5 hack">
      <li><h3>Expires</h3>
        <date-picker v-model="endDateObject" :config="config"></date-picker>
       </li>
    </div>
    <div class="col-12">
      <li>
        <button v-on:click="postData(JSON.stringify({'username':username, 'password':password, 'role':role, 'keycode': keycode, 'enabled': enabled, 'timeStart': startDateObject, 'timeEnd': endDateObject, 'doorlist': enableddoorlist}))">Submit</button>
      </li>
    </div>
    <div class="col-12">
      <li class="response">
        Result: {{ this.resp }}
      </li>
    </div>
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
        format: 'ddd, MMM DD YYYY, HH:mm'
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
      console.log(payload)
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
