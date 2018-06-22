<template>
  <div class="doors">
    <app-nav></app-nav>
    <h1>Manage a door</h1>
    <button v-on:click="displayAdd()">Add a door</button>
    <button v-on:click="displayDel()">Remove a door</button>

   <!-- <div class="col-md-5" v-for="item in doors"> -->
   <div v-if="this.thingy == 'edit'">
     <div class="col-md-5">
       <p>
         The new door name must correspond to an entry in an arduino script that is manipulating the controller of the door.
       </p>
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
         <tr v-if="this.mess != ''">
           <td>
             {{ this.mess.Messsage }}
           </td>
         </tr>
       </table>
     </div>
   </div>
   <div v-if="this.thingy == 'del'">
     <br><br>
     <table class="center">
       <tr>
         <th>
           Delete a door
         </th>
       <tr>
         <td>
          <select v-model="DoorName">
            <option disabled value="">Select Door to Delete</option>
            <option v-for="item in doors" v-bind:key="item">{{ item }}</option>
          </select>
        </td>
      </tr>
      <tr>
        <td>
          <br>
          <button v-on:click="delADoor(DoorName)">Delete door now</button>
        </td>
      </tr>
      <tr v-if="this.mess != ''">
        <td>
          {{ this.mess.Message }}
        </td>
      </tr>
    </table>
   </div>
  </div>
</template>

<script>
import { getDoors, postDoor, delDoor } from '../../../utils/door-api'
import AppNav from '../AppNav'
export default {
  name: 'setupdoor',
  data () {
    return {
      doors: [],
      resp: '',
      newdoor: '',
      thingy: '',
      DoorName: '',
      mess: ''
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
      // console.log('newdoor is ' + this.newdoor)
      postDoor(door).then((ret) => {
        this.resp = ret
        this.getDoors()
      })
    },
    displayAdd () {
      this.thingy = 'edit'
    },
    displayDel () {
      this.thingy = 'del'
    },
    delADoor (door) {
      delDoor(door).then((ret) => {
        this.mess = ret
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
/*.center {
  margin: auto;
}

td {
  text-align: left;
}*/
</style>
