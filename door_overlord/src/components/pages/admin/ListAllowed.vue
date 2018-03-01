<template>
  <div class="users">
    <app-nav></app-nav>
     <h2>Currently Allowed Users</h2>
     <div class="col-5" v-for="item in allowedusers">
       <li>{{ item.door }} has the following user(s) enabled:
         <div v-for="x in item.users">
            {{ x }}
         </div>
       </li>
     </div>
  </div>
</template>

<script>
import { getAllowedUsers } from '../../../../utils/door-api'
import AppNav from '../../AppNav'
export default {
  name: 'allowedusers',
  data () {
    return {
      allowedusers: []
    }
  },
  components: {
    AppNav
  },
  methods: {
    getAllowedUsers () {
      getAllowedUsers().then((ret) => {
        this.allowedusers = ret
      })
    }
  },
  mounted () {
    this.getAllowedUsers()
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
