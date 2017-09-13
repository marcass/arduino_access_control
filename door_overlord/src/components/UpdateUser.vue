<template>
  <div class="doors">
    <h1>Door Users for updating</h1>
   <div class="col-md-5" v-for="(item, key, index) in userlist" :key="item.name">
     <li>Username: {{ item.username }}</li>
     <li>Existing keycode is:
       <input v-model="item.keycode" :placeholder="item.keycode" v-on:keyup.enter="changeattr(item.username, 'keycode', item.keycode)">
     </li>
     <li>Enabled:
       <!-- <input type="checkbox" id="checkbox" v-model="checked">
       <label for="checkbox">{{ checked }}</label> -->
       <!-- :checked="enabled(item.enabled)" -->
       <!-- http://www.davidebarranca.com/2016/08/vue-js-binding-a-component-in-a-v-for-loop-to-the-parent-model/ -->
       <input type="checkbox" id="checkbox" v-model="item.enabled"  @click="changeattr(item.username, 'enabled', !item.enabled)">
       <!-- <label for="checkbox">{{ item.enabled }}</label> -->
     </li>
     <li>Valid from:
       <date-picker v-model="item.times.start" :placeholder="Date(item.times.start)"></date-picker>
       <button v-on:click="changeattr(item.username, 'timeStart', item.times.start)">Change time</button>
       <!-- <input v-model="starttime" :placeholder="item.times.start"> -->
       <p>New start time is: {{ item.times.start }}</p>
     </li>
     <li>Expires:
       <date-picker v-model="item.times.start" :placeholder="Date(item.times.end)" v-on:click="changeattr(item.username, 'timeStart', item.times.end)"></date-picker>
       <!-- <input v-model="endtime" :placeholder="item.times.end"> -->
       <p>New expiry time is: {{ item.times.end }}</p>
      </li>
   </div>
  </div>
</template>

<script>
import { getUsers, getDoors, putUserData } from '../../utils/door-api'
import 'bootstrap/dist/css/bootstrap.css'
import datePicker from 'vue-bootstrap-datetimepicker'
import 'eonasdan-bootstrap-datetimepicker/build/css/bootstrap-datetimepicker.css'
export default {
  name: 'updateuser',
  data () {
    return {
      doors: [],
      userlist: [],
      message: '',
      key: ''
    }
  },
  components: {
    datePicker
  },
  methods: {
    changeattr (userin, attr, val) {
      // var key = attr
      // axios.put(url,{'username': this.username, 'keycode': message})
      const payload = JSON.stringify({'username': userin, [attr]: val})
      // const pl = {username: item.username, keycode: message}
      putUserData(payload, attr)
      console.log(payload)
      return 1
    },
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
        this.doors = ret
      })
    },
    getUsers () {
      getUsers().then((ret) => {
        // this.userlist = ret.map(function (el) {
        // var o = Object.assign({}, el)
        //   o.startDateObject = new Date(o.times.start)
        //   o.endDateObject = new Date(o.times.end)
        //   return o
        //   // instead of `Object.assign(this.someObject, { a: 1, b: 2 })`
        //   // this.someObject = Object.assign({}, this.someObject, { a: 1, b: 2 })
        // })
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
