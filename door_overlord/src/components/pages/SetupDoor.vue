<template>
  <div class="doors">
    <app-nav></app-nav>
    <h1>Door Table</h1>
   <div class="col-md-5" v-for="item in doors">
     <ul>
       <li>{{ item }}</li>
     </ul>
   </div>
  </div>
</template>

<script>
import { getDoors, postDoor } from '../../../utils/door-api'
import AppNav from '../AppNav'
export default {
  name: 'setupdoor',
  data () {
    return {
      doors: [],
      resp: ''
    }
  },
  components: {
    AppNav
  },
  methods: {
    getDoors () {
      getDoors().then((ret) => {
        this.doors = ret
      })
    },
    addDoor (door) {
      postDoor().then((ret) => {
        this.resp = ret
      })
    }
  },
  mounted () {
    this.getDoors()
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
