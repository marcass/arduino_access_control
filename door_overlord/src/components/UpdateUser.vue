<template>
  <div class="doors">
    <h1>Door Users for updateing</h1>
   <div class="col-md-5" v-for="item in userlist">
     <li>Username: {{ item.username }}</li>
     <li>Keycode: {{ item.keycode }}</li>
     <li>Enabled: {{ item.enabled }}</li>
     <li>Valid from: {{ item.times.start }}</li>
     <li>Expires: {{ item.times.end }}</li>
   </div>
  </div>
</template>

<script>
import { getUsers, getDoors } from '../../utils/door-api'
import 'bootstrap/dist/css/bootstrap.css'
import datePicker from 'vue-bootstrap-datetimepicker'
import 'eonasdan-bootstrap-datetimepicker/build/css/bootstrap-datetimepicker.css'
export default {
  name: 'updateuser',
  data () {
    return {
      doors: [],
      userlist: []
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
          o.keycode = new Keycode(o.keycode)
          o.enabled = new Enabled(o.enabled)
          o.startDateObject = new Date(o.times.start)
          o.endDateObject = new Date(o.times.end)
          return o
          // instead of `Object.assign(this.someObject, { a: 1, b: 2 })`
          // this.someObject = Object.assign({}, this.someObject, { a: 1, b: 2 })
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
