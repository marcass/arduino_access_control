<template>
  <div class="doors">
    <h1>Door Table</h1>
   <div class="col-md-5" v-for="item in doorstatus">
     <li>{{ item.door }} was {{ item.status }} at {{ item.time }}</li>

   </div>
  </div>
</template>

<script>
import { getDoorStatus } from '../../utils/door-api'
// import 'bootstrap/dist/css/bootstrap.css'
export default {
  name: 'doors',
  data () {
    return {
      doors: [],
      doorstatus: []
    }
  },
  components: {
    // datePicker
  },
  methods: {
    getDoorStatus () {
      getDoorStatus().then((ret) => {
        this.doorstatus = ret
      })
    }
  },
  mounted () {
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
