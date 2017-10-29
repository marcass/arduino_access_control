<template>
  <div class="doors">
    <app-nav></app-nav>
    <h1>Door Users for updating</h1>
   <!-- <div class="col-md-5" v-for="(item, key, index) in userlist" :key="item.name"> -->
   <div class="col-lg-7" v-for="(item, key, index) in userlist" :key="item.name">
      <div class="col-lg-3">
       <div class="col-sm-3"><h4>Username: {{ item.username }} </h4>
         Keycode: <input v-model="item.keycode" :placeholder="item.keycode" v-on:keyup.enter="changeattr(item.username, 'keycode', item.keycode)">
       </div>
       <div class="col-sm-3" ><h4>Valid from:</h4> <date-picker v-model="item.startDateObject" :config="config" :placeholder="String(item.startDateObject)"></date-picker>
         <br>
       <h4>Expires: </h4><date-picker v-model="item.endDateObject" :config="config" :placeholder="String(item.endDateObject)"></date-picker>
      </div>
      </div>
       <div class="col-lg-3">
        <div class="col-sm-3">
          <div id='enabled-doors' v-for="x in doorlist">
            <input type="checkbox" :id="x" :value="x" v-model="item.doors">
            <label >{{ x }}</label>
          </div>
        </div>
        <div class="col-sm-3">
          <h4>Enabled: <input type="checkbox" id="checkbox" v-model="item.enabled"></h4>
          <button v-on:click="sendData(JSON.stringify({'username': item.username, 'keycode': item.keycode, 'enabled': item.enabled, 'timeStart': item.startDateObject, 'timeEnd': item.endDateObject, 'doorlist': item.doors}))">Submit user data</button>
          <button v-on:click="sendDelete(item.username)">Delete all user data</button>
          <!-- <button v-on:click="deleteUser(JSON.stringify({'username': item.username}))">Delete all user data</button> -->
        </div>
      </div>
   </div>
  </div>
</template>

<script>
import { getUsers, getDoors, putUserData, putAllUserData, deleteUser } from '../../../../utils/door-api'
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
      key: '',
      enableddoorlist: [],
      config: {
        format: 'ddd, MMM Do YYYY, HH:mm'
      }
    }
  },
  components: {
    datePicker,
    AppNav
  },
  methods: {
    sendData (payload) {
      putAllUserData(payload)
    },
    sendDelete (payload) {
      console.log(payload)
      deleteUser(payload)
    },
    changeattr (userin, attr, val) {
      // var key = attr
      // axios.put(url,{'username': this.username, 'keycode': message})
      const payload = JSON.stringify({'username': userin, [attr]: val})
      // const pl = {username: item.username, keycode: message}
      putUserData(payload, attr)
      console.log(payload)
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
        this.userlist = ret.map(function (el) {
          var o = Object.assign({}, el)
          o.startDateObject = new Date(o.times.start)
          o.endDateObject = new Date(o.times.end)
          console.log(o.startDateObject)
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
</style>
