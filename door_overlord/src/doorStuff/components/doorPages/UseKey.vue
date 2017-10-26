<template>
  <div class="keypad">
    <h2>Input your keycode, then press #</h2>
    <p>
      <div id='doors' v-for="x in doorstatus">
        <input type="radio" :id="x.door" :value="x.door" v-model="doorselected">
      <label for="x.door">{{ x.door }} currently {{ x.status }}</label>
     </div>
    </p>
    <keyboard layouts="123A|456B|789C|*0{#:enter}D" v-model="keycode" @enter="postkey"></keyboard>
    <p> Keycode = {{ keycode }}</p>
    <div class="statusgood" v-if="resp.pin_correct === true">
     <p>Status</p>
    </div>
    <!-- <div class="statusbad" v-if="resp.pin_correct === false"> -->
    <div class="statusbad" v-else>
     <p>Status</p>
    </div>
     <!-- <div v-else>
      <p>Status</p>
     </div> -->
  </div>
</template>

<script>
import { postKeycode, getDoors, getDoorStatus } from '../../utils/door-api'
import keyboard from 'vue-keyboard'
export default {
  name: 'updateuser',
  data () {
    return {
      doorlist: [],
      keycode: '',
      doorselected: '',
      doorstatus: [],
      resp: []
    }
  },
  components: {
    keyboard
  },
  methods: {
    postkey () {
      postKeycode(JSON.stringify({'door': this.doorselected, 'pincode': this.keycode})).then((ret) => {
        this.resp = ret
      })
      this.keycode = ''
    },
    getDoors () {
      getDoors().then((ret) => {
        this.doorlist = ret
      })
    },
    getDoorStatus () {
      getDoorStatus().then((ret) => {
        this.doorstatus = ret
      })
    }
  },
  mounted () {
    this.getDoors()
    this.getDoorStatus()
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
div.statusgood {
    background-color: green;
    width: 50px;
    margin:0 auto;
}
div.statusbad {
    background-color: red;
    margin:0 auto;
    width: 50px;
}
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
