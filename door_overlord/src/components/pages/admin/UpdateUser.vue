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
      <button v-on:click="amendKeycode()">Change keycode</button>
      <button v-on:click="amendDoors()">Change door permissions</button>
      <button v-on:click="amendValiddates()">Change dates allowed</button>
      <button v-on:click="amendEnabled()">Enable or disable</button>
      <button v-on:click="amendPass()">Change password</button>
      <button v-on:click="delUser()">Delete user</button>
      <br><br>
      <div v-if="edType == 'keycode'">
        Keycode: <input v-model="key" :placeholder="this.userData.keycode" v-on:keyup.enter="changeattr(this.userData.username, 'keycode', key)">
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
        </table>
      </div>
      <div v-if="edType == 'dates'" style="position: relative">
        Valid from: <date-picker v-model="newStart" :config="config" :placeholder="String(this.userData.times.start)"></date-picker>
        Expires: <date-picker v-model="newEnd" :config="config" :placeholder="String(this.userData.times.end)"></date-picker>
      </div>
      <div v-if="edType == 'enabled'">
        Enabled: <input type="checkbox" id="checkbox" v-model="this.userData.enabled">
      </div>
      <div v-if="edType == 'pass'">
        pass stuff
      </div>
    </div>




    <!-- <table v-if="disp" class="center">
      <tr>
        <th colspan="5">
          Username: {{ this.userData.username }}
        </th>
      </tr>
      <tr>
        <td>
          Keycode: <input v-model="key" :placeholder="this.userData.keycode" v-on:keyup.enter="changeattr(this.userData.username, 'keycode', key)">
        </td>
        <tr>
          <td>
            <br> -->
            <!-- <div class="radio" id="enabled-doors" v-model="doors" >
              <div v-for="x in this.doors">
                <input type="checkbox" :id="x" :value="x">
                <label >{{ x }}</label>
              </div>
              <div v-for="i in availDoors">
                <input type="checkbox" :id="i" :value="i">
                <label >{{ i }}</label>
              </div>
            </div> -->
            <!-- Currently enabled doors:<br>
            <div v-for="item in this.userData.doors">
              <li>
                {{ item }}
              </li>
            </div>
            <div class="radio" id='enabled-doors' v-for="x in doorlist">
              <input type="checkbox" :id="x" :value="x" v-model="doors">
              <label >{{ x }}</label>
            </div>
          </td>
        <tr>
          <td class="radio">
            Enabled: <input type="checkbox" id="checkbox" v-model="enabled">
          </td>
        </tr> -->
        <!-- <td>
          <div>
            Valid from: <date-picker v-model="item.startDateObject" :config="config" :placeholder="String(item.startDateObject)"></date-picker>
          </div>
        </td>
        <td>
          Expires: <date-picker v-model="item.endDateObject" :config="config" :placeholder="String(item.endDateObject)"></date-picker>
        </td>
      </tr>
      <tr>
        <td>
          <div class="radio" id='enabled-doors' v-for="x in doorlist">
            <input type="checkbox" :id="x" :value="x" v-model="item.doors">
            <label >{{ x }}</label>
          </div>
        </td>
        <td class="radio">
          Enabled: <input type="checkbox" id="checkbox" v-model="item.enabled">
        </td>
        <td>
          <button v-on:click="sendData(JSON.stringify({'username': item.username, 'keycode': item.keycode, 'enabled': item.enabled, 'timeStart': item.startDateObject, 'timeEnd': item.endDateObject, 'doorlist': item.doors}))">Submit user data</button>
          <button v-on:click="sendDelete(item.username)">Delete all user data</button>
        </td> -->
    <!-- </table> -->
    <!-- <div class="response">
     Result: {{ this.resp }}
    </div> -->

    <!-- <div class="col-5 user" v-for="(item, key, index) in userlist" :key="item.name">
      <div class="col-3"><h3>Username: {{ item.username }} </h3>
        Keycode: <input v-model="item.keycode" :placeholder="item.keycode" v-on:keyup.enter="changeattr(item.username, 'keycode', item.keycode)">
      </div>
      <div class="col-3 hack" ><h4>Valid from:</h4> <date-picker v-model="item.startDateObject" :config="config" :placeholder="String(item.startDateObject)"></date-picker>
        <br>
        <h4>Expires: </h4><date-picker v-model="item.endDateObject" :config="config" :placeholder="String(item.endDateObject)"></date-picker>
      </div>
      <div class="col-2">
        <div class="radio" id='enabled-doors' v-for="x in doorlist">
          <input type="checkbox" :id="x" :value="x" v-model="item.doors">
          <label >{{ x }}</label>
        </div>
      </div>
      <div class="col-2 radio">
        <h4>Enabled: <input type="checkbox" id="checkbox" v-model="item.enabled"></h4>
        <button v-on:click="sendData(JSON.stringify({'username': item.username, 'keycode': item.keycode, 'enabled': item.enabled, 'timeStart': item.startDateObject, 'timeEnd': item.endDateObject, 'doorlist': item.doors}))">Submit user data</button>
        <button v-on:click="sendDelete(item.username)">Delete all user data</button>
      </div>
    </div>
    <div class="response">
     Result: {{ this.resp }}
    </div> -->
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
      message: '',
      key: '',
      resp: '',
      enableddoorlist: [],
      newStart: '',
      newEnd: '',
      disp: false,
      edType: '',
      doors: [],
      enabled: '',
      config: {
        format: 'ddd, MMM DD YYYY, HH:mm'
      }
    }
  },
  components: {
    datePicker,
    AppNav
  },
  methods: {
    sendData (payload) {
      putAllUserData(payload).then((ret) => {
        this.resp = ret.data.status
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
    delUser () {
      this.edType = 'key'
    },
    amendUser (username) {
      userData(username).then((ret) => {
        this.userData = ret
        // Change date format so it can be read
        this.userData.times.start = new Date(ret.times.start)
        this.userData.times.end = new Date(ret.times.end)
      })
      this.disp = true
    },
    sendDelete (payload) {
      // console.log(payload)
      deleteUser(payload)
    },
    changeattr (userin, attr, val) {
      // var key = attr
      // axios.put(url,{'username': this.username, 'keycode': message})
      const payload = JSON.stringify({'username': userin, [attr]: val})
      // const pl = {username: item.username, keycode: message}
      putUserData(payload, attr)
      // console.log(payload)
      return 1
    },
    // doorIsEnabled () {
    //   this.enableddoorlist.includes(this.doors)
    // },
    changestatus (x) {
      this.x = !this.x
      return this.x
    },
    enabled (x) {
      if (x === 1) {
        return true
      } else {
        return false
      }
    },
    getDoors () {
      getDoors().then((ret) => {
        this.doorlist = ret
      })
    },
    getUsers () {
      getUsers().then((ret) => {
        console.log(ret)
        this.userlist = ret.map(function (el) {
          var o = Object.assign({}, el)
          o.startDateObject = new Date(o.times.start)
          o.endDateObject = new Date(o.times.end)
          // console.log(o.startDateObject)
          return o
        //   // instead of `Object.assign(this.someObject, { a: 1, b: 2 })`
        //   // this.someObject = Object.assign({}, this.someObject, { a: 1, b: 2 })
        })
        // this.userlist = ret
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
