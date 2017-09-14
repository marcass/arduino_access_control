<template>
  <div class="keypad">
    <h2>Input your keycode, then press #</h2>
    <p>
      <div id='enabled-doors' v-for="x in doorlist">
        <input type="radio" :id="x" :value="x" v-model="doorselected">
        <label for="x">{{ x }}</label>
     </div>
    </p>
    <keyboard layouts="123A|456B|789C|*0{#:enter}D" v-model="keycode" @enter="postkey"></keyboard>
    <p> Keycode = {{ keycode }}</p>
  </div>
</template>

<script>
import { postKeycode, getDoors } from '../../utils/door-api'
import keyboard from 'vue-keyboard'
export default {
  name: 'updateuser',
  data () {
    return {
      doorlist: [],
      keycode: '',
      doorselected: ''
    }
  },
  components: {
    keyboard
  },
  methods: {
    postkey () {
      postKeycode(JSON.stringify({'door': this.doorselected, 'pincode': this.keycode}))
      this.keycode = ''
    },
    getDoors () {
      getDoors().then((ret) => {
        this.doorlist = ret
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
