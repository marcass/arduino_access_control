<template>
  <div class="doors">
    <h1>Existing users</h1>
    <p v-for="item in userlist"> {{ item.username }} </p>
   <div class="col-md-5">
     <li>Username:
       <input v-model="username">
     </li>
     <li>Keycode:
       <input v-model="keycode">
     </li>
     <li>Enabled:
       <input type="checkbox" id="checkbox" v-model="enabled">
     </li>
     <li>Valid from:
       <date-picker v-model="startDateObject" :config="config"></date-picker>
     </li>
     <li>Expires:
       <date-picker v-model="endDateObject" :config="config"></date-picker>
      </li>
      <li>
        <div id='enabled-doors' v-for="x in doorlist">
          <input type="checkbox" :id="x" :value="x" v-model="enableddoorlist">
          <label >{{ x }}</label>
        </div>
      </li>
      <li>
        <button v-on:click="blah(JSON.stringify({'username':username, 'keycode': keycode, 'enabled': enabled, 'timeStart': startDateObject, 'timeEnd': endDateObject, 'doorlist': enableddoorlist}))">Submit</button>
      </li>
   </div>
  </div>
</template>

<script>
import { getUsers, getDoors, postUserData } from '../../../../utils/door-api'
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
      keycode: '',
      enableddoorlist: [],
      config: {
        format: 'ddd, MMM Do YYYY, HH:mm'
      },
      username: '',
      endDateObject: '',
      startDateObject: '',
      enabled: ''
    }
  },
  components: {
    datePicker
  },
  methods: {
    blah (payload) {
      postUserData(payload)
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
