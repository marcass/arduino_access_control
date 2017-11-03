<template>
  <div class="users">
    <app-nav></app-nav>
    <h1>User Table</h1>
      <div v-for="item in userlist">
        <div class="col-md-5">
            <div  v-if="enabled(item.enabled)">
                <h3>
                  {{ item.username }}  is ENABLED
                </h3>
                <h4>
                  Keycode:  {{ item.keycode }}
                </h4>
                <h4>
                  Valid from: {{ item.startDateReadable }}
                </h4>
                <h4>
                  Expires: {{ item.endDateReadable }}
                </h4>
                Permitted doors: <p v-for="x in item.doors">
                                    {{ x }}
                                  </p>
            </div>
            <div v-else>
                <h3>{{ item.username }}  |  {{ item.keycode }} is disabled</h3>
            </div>
        </div>
      </div>
   <!-- <div v-for="item in doorstatus">
     <li>{{ item.door }} was {{ item.status }} at {{ item.time }}</li>

  </div> -->
  </div>
</template>

<script>
import { getUsers, getDoors, getDoorStatus, getAllowedUsers } from '../../../../utils/door-api'
import 'bootstrap/dist/css/bootstrap.css'
// Import this component
import datePicker from 'vue-bootstrap-datetimepicker'
// Import date picker css
import 'eonasdan-bootstrap-datetimepicker/build/css/bootstrap-datetimepicker.css'
import AppNav from '../../AppNav'
// import { isLoggedIn } from '../../../../utils/auth'

export default {
  name: 'users',
  data () {
    return {
      userlist: [],
      doors: [],
      doorstatus: [],
      getallowedusers: []
    }
  },
  components: {
    datePicker,
    AppNav
  },
  methods: {
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
        this.userlist = ret.map(function (el) {
          var o = Object.assign({}, el)
          o.startDateObject = new Date(o.times.start)
          o.endDateObject = new Date(o.times.end)
          o.startDateReadable = o.startDateObject.toDateString()
          o.endDateReadable = o.endDateObject.toDateString()
          return o
        })
      })
    },
    getAllowedUsers () {
      getAllowedUsers().then((ret) => {
        this.getallowedusers = ret
      })
    },
    getDoorStatus () {
      getDoorStatus().then((ret) => {
        this.doorstatus = ret
      })
    }
  },
  mounted () {
    this.getUsers()
    this.getDoors()
    this.getDoorStatus()
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
