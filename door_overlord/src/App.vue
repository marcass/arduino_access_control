<template>
  <div id="app">
    <div v-if="$auth.ready()">
      <router-view></router-view>
    </div>
    <div v-if="!$auth.ready()">
        <div style="text-align:center; padding-top:50px;">
            Loading site...
        </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'app',
  data () {
    return {
      context: 'app context'
    }
  },
  methods: {
    logout () {
      this.$auth.logout({
        makeRequest: true,
        success () {
          console.log('success ' + this.context)
        },
        error () {
          console.log('error ' + this.context)
        }
      })
    }
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
@import './assets/css/main.css'
</style>
