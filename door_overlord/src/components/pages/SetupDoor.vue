<template>
  <div class="doors">
    <app-nav></app-nav>
    <h1>Add a door</h1>
    <p>
      The new door name must correxpond to an entry in an arduino script that is manipulating the controller of the door.
    </p>
   <!-- <div class="col-md-5" v-for="item in doors"> -->
   <div class="col-md-5">
     <table class='center'>
       <tr>
         <th>
           Door Table
         </th>
       </tr>
       <tr  v-for="item in doors">
         <td>
           {{ item }}
         </td>
       </tr>
     </table>
   </div>
   <div class="col-md-5">
      <table class='center'>
       <tr>
         <th colspan="2">
           New door name
         </th>
       </tr>
       <tr>
         <td>
           <input v-model="newdoor">
         </td>
         <td>
           <button v-on:click="addDoor({'door': newdoor})">Submit</button>
         </td>
       </tr>
     </table>
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
      resp: '',
      newdoor: ''
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
      console.log('newdoor is ' + this.newdoor)
      postDoor(door).then((ret) => {
        this.resp = ret
        this.getDoors()
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
.center {
  margin: auto;
}

td {
  text-align: left;
}
</style>
