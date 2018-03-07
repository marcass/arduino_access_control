<template>
  <div class="hello">
    <app-nav></app-nav>
    <h1>{{ msg }}</h1>
    <!-- <h2>Essential Links</h2> -->
  </div>
</template>

<script>
import AppNav from '../AppNav'
export default {
  name: 'hello',
  data () {
    return {
      msg: 'Welcome. What orifice can I open for you?'
    }
  },
  components: {
    AppNav
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
  color: #42b983;
}
</style>
