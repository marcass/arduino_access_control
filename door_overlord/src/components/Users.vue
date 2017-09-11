<template>
  <div class="users">
    <h1>User Table</h1>
      <div class="col-md-5" v-for="item in userlist">
        <h3>{{ item.username }}  |  {{ item.keycode }}</h3>
   Enabled: {{ item.enabled }}
           <div class="col-lg-4">
            <date-picker v-model="item.startDateObject"></date-picker> <date-picker v-model="item.endDateObject"></date-picker>
         </div>
         {{ doors }} {{ obj.usrs }}
         {{ item.doors }}

    </div>
     {{ doorstatus }}
     <!-- {{ getallowedusers }} -->
  </div>

  <!-- <div class="col-md-5" v-for="obj in doors">
    {{ obj.doors }}
  </div> -->
</template>

<script>
import { getUsers, getDoors, doorStatus, getAllowedUsers } from '../../utils/door-api'
import 'bootstrap/dist/css/bootstrap.css'
// Import this component
import datePicker from 'vue-bootstrap-datetimepicker'
// Import date picker css
import 'eonasdan-bootstrap-datetimepicker/build/css/bootstrap-datetimepicker.css'

export default {
  name: 'users',
  data () {
    return {
      userlist: [],
      doors: [],
      doorstatus: [],
      getallowedusers: [],
      getmoredoors: []
    }
  },
  components: {
    datePicker
  },
  methods: {
    getDoors () {
      getDoors().then((ret) => {
        this.doors = ret
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
    },
    getAllowedUsers () {
      getAllowedUsers().then((ret) => {
        this.getallowedusers = ret
      })
    },
    doorStatus () {
      doorStatus().then((ret) => {
        this.doorstatus = ret
      })
    },
    getmoreDoors () {
      getDoors().then((ret) => {
        this.doors = ret
      })
    }
  },
  mounted () {
    this.getUsers()
    this.getDoors()
    this.doorStatus()
    this.getmoreDoors()
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
