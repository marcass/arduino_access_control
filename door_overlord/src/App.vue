<template>
  <div id="app">
    <div v-if="$auth.ready()">
     <!-- <img src="./assets/logo.png"> -->
     <router-view></router-view>
    </div>
    <div v-else>
      Loading ...
    </div>
  </div>
</template>

<script>
export default {
  name: 'app'
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
</style>
