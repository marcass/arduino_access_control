<template>
  <div class="doors">
    <h1>Door Users for updating</h1>
   <div class="col-md-5" v-for="(item, key, index) in userlist" :key="item.name">
     <li>Username: {{ item.username }}</li>
     <li>Existing keycode is:
       <input v-model="item.keycode" :placeholder="item.keycode" v-on:keyup.enter="changeattr(item.username, 'keycode', item.keycode)">
     </li>
     <li>Enabled:
      <input type="checkbox" id="checkbox" v-model="item.enabled">
     </li>
     <li>Valid from:
       <date-picker v-model="item.startDateObject" :config="config" :placeholder="String(item.startDateObject)"></date-picker>
     </li>
     <li>Expires:
       <date-picker v-model="item.endDateObject" :config="config" :placeholder="String(item.endDateObject)"></date-picker>
      </li>
      <li>
        <div id='enabled-doors' v-for="x in doorlist">
          <input type="checkbox" :id="x" :value="x" v-model="item.doors">
          <label >{{ x }}</label>
        </div>
      </li>
      <li>
        <button v-on:click="blah(JSON.stringify({'username': item.username, 'keycode': item.keycode, 'enabled': item.enabled, 'timeStart': item.startDateObject, 'timeEnd': item.endDateObject, 'doorlist': item.doors}))">Submit</button>
      </li>
   </div>
  </div>
</template>

<script>
import { getUsers, getDoors, putUserData, putAllUserData } from '../../utils/door-api'
import 'bootstrap/dist/css/bootstrap.css'
import datePicker from 'vue-bootstrap-datetimepicker'
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
    datePicker
  },
  methods: {
    blah (payload) {
      putAllUserData(payload)
    },
    // putUserDataAll () {
    //   console.log()
    //   const stuff = JSON.stringify({'username': this.username, 'keycode': this.keycode, 'enabled': this.enabled, 'timeStart': this.startDateObject, 'timeEnd': this.endDateObject, 'doorlist': this.doors})
    //   console.log(stuff)
    //   putAllUserData(stuff)
    // },
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
