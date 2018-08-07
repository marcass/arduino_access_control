<template>
  <div class="doors">
    <app-nav></app-nav>
    <h2>Add a user</h2>
    <table class="center">
      <tr>
        <th>
          Existing users
        </th>
        <tr v-for="item in userlist">
          <td class="middle">
            {{ item.username }}
          </td>
        </tr>
      </table>
      <br><br>
      <table class="center">
        <tr>
          <td class="tabLabel">
            Username (if 'burner' it is onetime use)
          </td>
          <td>
            <input v-model="username">
          </td>
        </tr>
        <tr>
          <td class="tabLabel">
            Password
          </td>
          <td>
            <input type="password" v-model="pass1">
          </td>
        </tr>
        <tr>
          <td class="tabLabel">
            Confirm password
          </td>
          <td>
            <input type="password" v-model="pass2">
          </td>
        </tr>
        <tr>
          <td class="tabLabel">
            Keycode
          </td>
          <td>
            <input v-model="keycode">
          </td>
        </tr>
        <tr>
          <td class="tabLabel">
            Role
          </td>
          <td v-for="item in roles">
            <input type="radio" :id="item" :value="item" v-model="role">
            <label for="item">{{ item }}</label>
          </td>
          <!-- <td>
            <input type="radio" id="admin" value="admin" v-model="role">
            <label for="admin">Admin</label>
          </td>
        </tr>
        <tr>
          <td>
          </td>
          <td>
            <input type="radio" id="user" value="user" v-model="role">
            <label for="user">User</label>
          </td> -->
        </tr>
        <tr>
          <td class="tabLabel">
            Enabled
          </td>
          <td>
            <input type="checkbox" id="checkbox" v-model="enabled">
          </td>
        </tr>
        <tr>
          <td class="tabLabel">
            Valid from
          </td>
          <td>
            <div style="position: relative">
              <date-picker v-model="startDateObject" :config="config"></date-picker>
            </div>
          </td>
        </tr>
        <tr>
          <td class="tabLabel">
            Valid until
          </td>
          <td>
            <div style="position: relative">
              <date-picker v-model="endDateObject" :config="config"></date-picker>
            </div>
          </td>
        </tr>
        <tr>
          <th colspan="2"  class="middle">
            <br>
            Select doors that apply
          </th>
        </tr>
        <tr v-for="item in doorlist">
          <td class="tabLabel">
            <label >{{ item }}</label>
          </td>
          <td>
            <input type="checkbox" :id="item" :value="item" v-model="enableddoorlist">
          </td>
        </tr>
        <tr>
          <td colspan="2" class="middle">
            <button v-on:click="verifyPass()">Submit</button>
          </td>
        </tr>
      </table>
      <div v-if="resp != ''">
        <br><br>
        User
        {{ resp.data.Message }}
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
      pass1: '',
      pass2: '',
      role: '',
      roles: ['admin', 'user', 'sensor', 'python'],
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
        this.resp = ret
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
    },
    verifyPass () {
      if (this.pass1 === this.pass2) {
        this.postData(JSON.stringify({'username': this.username, 'password': this.pass1, 'role': this.role, 'keycode': this.keycode, 'enabled': this.enabled, 'timeStart': this.startDateObject, 'timeEnd': this.endDateObject, 'doorlist': this.enableddoorlist}))
        this.getUsers()
      } else {
        this.resp = {'data': {'Status': 'Error', 'Message': 'Passwords do not match'}}
      }
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

td {
  padding: 5px;
}

.middle {
  text-align: center;
}

.justify {
  margin-left: auto;
  margin-right: auto;
  width: 30%;
  text-align: right;
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
