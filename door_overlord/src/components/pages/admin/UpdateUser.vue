<template>
  <div class="doors">
    <app-nav></app-nav>
    <h2>Select user to update/delete</h2>
    <select v-model="username">
      <option disabled value="">Select user</option>
      <option v-for="(item, key, index) in this.userlist" v-bind:key="item.username">{{ item.username }}</option>
    </select>
    <button v-on:click="amendUser(username)">Do stuff to this user</button>
    <br><br>
    <div v-if="disp">
      <h4>Username: {{ this.userData.username }}</h4>
      <table class="center">
        <tr>
          <td>
            <button v-on:click="amendKeycode()">Change keycode</button>
            <button v-on:click="amendDoors()">Change door permissions</button>
            <button v-on:click="amendValiddates()">Change dates allowed</button>
          </td>
        </tr>
        <tr>
          <td>
            <button v-on:click="amendEnabled()">Enable or disable</button>
            <button v-on:click="amendPass()">Change password</button>
            <button v-on:click="amendRole()">Change role</button>
            <button v-on:click="amendEnabled()">Enable or disable</button>
          </td>
        </tr>
        <tr>
          <td colspan="3">
            <button v-on:click="sendDelete(userData.username)">Delete user</button>
          </td>
        </tr>
      </table>
      <br><br>
      <div v-if="edType == 'role'" v-model="role">
        Current role is: {{ userData.role }}
        <table class="center">
          <tr>
            <td class="tabLabel">
              <label for="admin">Admin</label>
            </td>
            <td>
              <input type="radio" id="admin" value="admin">
            </td>
          </tr>
          <tr>
            <td class="tabLabel">
              <label for="user">User</label>
            </td>
            <td>
              <input type="radio" id="user" value="user">
            </td>
          </tr>
        </table>
        <br>
        <button v-on:click="changeRole()">Change role</button>
      </div>
      <div v-if="edType == 'key'">
        Keycode: <input v-model="key" :placeholder="this.userData.keycode">
        <button v-on:click="changeattr(userData.username, 'keycode', key)">Change keycode</button>
      </div>
      <div v-if="edType == 'doors'">
        <table class="center">
          <tr>
            <th>
              Currently enabled doors
            </th>
          </tr>
          <tr v-for="item in this.userData.doors">
            <td>
              {{ item }}
            </td>
          </tr>
          <tr>
            <th>
              Check required doors:
            </th>
          </tr>
          <tr v-for="x in doorlist">
            <td>
              <input type="checkbox" :id="x" :value="x" v-model="doors">
              <label >{{ x }}</label>
            </td>
          </tr>
          <tr>
            <td>
              <button v-on:click="changeattr(userData.username, 'doors', doors)">Change doors</button>
            </td>
          </tr>
        </table>
      </div>
      <div v-if="edType == 'dates'" style="position: relative">
        Valid from: <date-picker v-model="newStart" :config="config" :placeholder="String(this.userData.times.start)"></date-picker>
        <br>
        <button v-on:click="changeattr(userData.username, 'timeStart', newStart)">Change start date</button>
        <br><br>
        Expires: <date-picker v-model="newEnd" :config="config" :placeholder="String(this.userData.times.end)"></date-picker>
        <br>
        <button v-on:click="changeattr(userData.username, 'timeEnd', newEnd)">Change end date</button>
      </div>
      <div v-if="edType == 'enabled'">
        Enabled: <input type="checkbox" id="checkbox" v-model="userData.enabled">
        <br>
        <button v-on:click="changeattr(userData.username, 'enabled', userData.enabled)">Change status</button>
      </div>
      <div v-if="edType == 'pass'">
          New password: <input type="password" v-model="pass1">
          Confirm password: <input type="password" v-model="pass2">
          <br>
          <button v-on:click="changePass(pass1, pass2)">Change password</button>
      </div>
    </div>
    <div v-if="response != ''">
      {{ response.data.Message }}
    </div>
  </div>
</template>

<script>
import { getUsers, getDoors, putUserData, putAllUserData, deleteUser, userData } from '../../../../utils/door-api'
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
      userData: '',
      username: '',
      key: '',
      newStart: '',
      newEnd: '',
      doors: [],
      enabled: '',
      role: '',
      pass1: '',
      pass2: '',
      disp: false,
      edType: '',
      config: {
        format: 'ddd, MMM DD YYYY, HH:mm'
      },
      response: ''
    }
  },
  components: {
    datePicker,
    AppNav
  },
  methods: {
    sendData (payload) {
      putAllUserData(payload).then((ret) => {
        this.response = ret
      })
    },
    amendKeycode () {
      this.edType = 'key'
    },
    amendDoors () {
      this.edType = 'doors'
    },
    amendValiddates () {
      this.edType = 'dates'
    },
    amendEnabled () {
      this.edType = 'enabled'
    },
    amendPass () {
      this.edType = 'pass'
    },
    amendRole () {
      this.edType = 'role'
    },
    amendUser (username) {
      userData(username).then((ret) => {
        this.userData = ret
        console.log(ret)
        // Change date format so it can be read
        this.userData.times.start = new Date(ret.times.start)
        this.userData.times.end = new Date(ret.times.end)
      })
      this.disp = true
    },
    sendDelete (payload) {
      deleteUser(payload).then((ret) => {
        this.response = ret
      })
      // refesh list of users
      this.getUsers()
    },
    changePass () {
      if (this.pass1 === this.pass2) {
        this.changeattr(this.userData.username, 'password', this.pass1)
      } else {
        this.response = {'Status': 'Error', 'Message': 'Passwords do not match'}
      }
    },
    changeRole () {
      this.changeattr(this.userData.username, 'role', this.role)
    },
    changeattr (userin, attr, val) {
      const payload = JSON.stringify({'username': userin, [attr]: val})
      putUserData(payload, attr).then((ret) => {
        this.response = ret
        console.log(this.response)
      })
    },
    getDoors () {
      getDoors().then((ret) => {
        this.doorlist = ret
      })
    },
    getUsers () {
      getUsers().then((ret) => {
        this.userlist = ret.map(function (el) {
          var o = Object.assign({}, el)
          o.startDateObject = new Date(o.times.start)
          o.endDateObject = new Date(o.times.end)
          return o
        })
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
h1, h2 {
  font-weight: normal;
}

td {
  padding: 5px;
  text-align: center;
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

.radio {
  text-align: left;
}

th {
  text-align: center;
}
</style>
