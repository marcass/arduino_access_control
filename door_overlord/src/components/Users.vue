<template>
  <div class="users">
    <h1>User Table</h1>
      <div class="col-md-5" v-for="item in userlist">
        <h3>{{ item.username }}  |  {{ item.keycode }}</h3>
   Enabled: {{ item.enabled }}
           <div class="col-lg-4">
            <date-picker v-model="item.startDateObject"></date-picker> <date-picker v-model="item.endDateObject"></date-picker>
         </div>
         {{ doors }}
         {{ item.doors }}
    </div>
  </div>
</template>

<script>
import { getUsers, getDoors } from '../../utils/door-api'
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
      doors: []
    }
  },
  components: {
    datePicker
  },
  methods: {
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
    getDoors () {
      getDoors().then((ret) => {
        this.doors = ret
      })
    }
  },
  mounted () {
    this.getUsers()
    this.getDoors()
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
