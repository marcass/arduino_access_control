<template>
  <div class="doors">
    <app-nav></app-nav>
    <table class="center">
      <tr v-for="item in doors">
        <td>
          <input type="radio" :id="item" :value="item" v-bind:value="item" v-model="door">
        </td>
        <td>
          <label >{{ item }}</label>
        </td>
      </tr>
    </table>

    <!-- <h1>Please choose a door</h1>
    <div class='radio'>
      <p v-for="item in doors">
        <input type="radio" :id="door" v-bind:value="item" v-model="door">
        <label for="door">{{ item }}</label>
        <br>
      </p>
    </div> -->
    <div style="position:relative">
      <li>Log Start (default is 1d ago):
        <date-picker v-model="startDateObject" :config="config"></date-picker>
      </li>
      <li>Log end time (default is now):
        <date-picker v-model="endDateObject" :config="config"></date-picker>

       </li>
       <br><br>
       <li>
        <button v-on:click="postData(door, JSON.stringify({'timeStart': startDateObject, 'timeEnd': endDateObject}))">Submit</button>
      </li>
      <!-- <li class="response">
        Actions:
        <div v-for="item in this.resp.actions">
          {{ item }}
        </div>
        Door states:
        <div v-for="item in this.resp.states">
          {{ item }}
        </div>
      </li> -->
      <br><br>
      <div v-if="resp != ''">
        <h3>Door log for {{this.door}}</h3>
        <table class="center">
          <tr>
            <th colspan="2">
              States
            </th>
          </tr>
          <tr v-for="(item, index) in this.resp.states.state">
            <td>
              {{ item }} at {{resp.states.time[index]}}
            </td>
          </tr>
        </table>
        <table class="center">
          <tr>
            <th colspan="2">
              Actions
            </th>
          </tr>
          <tr v-for="(item, index) in this.resp.actions.action">
            <td>
              {{ resp.actions.mesage[index] }} {{ item }} on {{ resp.actions.time[index] }}
            </td>
          </tr>
        </table>
      </div>
    </div>
  </div>
</template>

<script>
import { getDoorStatus, getDoors, getLog } from '../../../utils/door-api'
import 'bootstrap/dist/css/bootstrap.css'
import datePicker from 'vue-bootstrap-datetimepicker'
import AppNav from '../AppNav'
import 'eonasdan-bootstrap-datetimepicker/build/css/bootstrap-datetimepicker.css'
export default {
  name: 'statuslog',
  data () {
    return {
      doors: [],
      doorstatus: [],
      resp: '',
      door: '',
      config: {
        format: 'ddd, MMM DD YYYY, HH:mm'
      },
      endDateObject: '',
      startDateObject: ''
    }
  },
  components: {
    AppNav,
    datePicker
  },
  methods: {
    getDoorStatus () {
      getDoorStatus().then((ret) => {
        this.doorstatus = ret
      })
    },
    getDoors () {
      getDoors().then((ret) => {
        this.doors = ret
      })
    },
    postData (door, payload) {
      // console.log(payload)
      // console.log(door)
      getLog(door, payload).then((ret) => {
        // console.log(ret)
        // console.log(ret.actions)
        this.resp = ret
      })
    }
  },
  mounted () {
    this.getDoorStatus()
    this.getDoors()
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1, h2 {
  font-weight: normal;
}

td {
  padding: 5px;
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

/*th {
  text-align: center;
}*/
</style>
