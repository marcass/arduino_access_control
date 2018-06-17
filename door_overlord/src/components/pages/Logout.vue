<template>
  <div>
    <app-nav></app-nav>
    <h1>Logging your bitch-ass out becuase you are NOT AUTHORISED</h1>
  </div>
</template>

<script>
import AppNav from '../AppNav'
export default {
  components: {
    AppNav
  },
  methods: {
  },
  mounted () {
    this.$auth.logout({
      makeRequest: false,
      success () {
        // console.log('success ' + this.context)
        this.$router.push({name: 'Login'})
      },
      error () {
        // console.log('error ' + this.context)
      }
    })
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
