<template>
    <div class="menu">
      <span v-if="$auth.check()">
        <span v-if="$auth.check('admin')">
          <tree
            :data="authtreeData"
            class="tree--small"
            @node:selected="onNodeSelected"
          />
        </span>
        <span v-if="$auth.check('user')">
          <tree
            :data="usertreeData"
            @node:selected="onNodeSelected"
          />
        </span>
      </span>
      <span v-if="!$auth.check()">
        <tree
          :data="nonauthtreeData"
          @node:selected="onNodeSelected"
        />
      </span>
    </div>
</template>

<script>
import Vue from 'vue'
import LiquorTree from 'liquor-tree'
// global registration
Vue.use(LiquorTree)
export default {
  name: 'app-nav',
  methods: {
    logout () {
      this.$auth.logout({
        makeRequest: false,
        success () {
          console.log('logout success ' + this.context)
        },
        error () {
          console.log('logout error ' + this.context)
        }
      })
      // this.$router.push({name: 'login'})
    },
    onNodeSelected (node) {
      // var data = this.data
      console.log('data ' + node.text)
      if (node.text === 'Logout') {
        this.$auth.logout({
          makeRequest: false,
          success () {
            console.log('node selected success ' + this.context)
          },
          error () {
            console.log(' node selected error ' + this.context)
          }
        })
      } else {
        console.log('push route = ' + node.text)
        this.$router.push({name: node.text})
        // this.$router.push('/auth/login')
        // router.push({name: node.text})
      }
    }
  },
  components: {
    // [tree-nav.name]: LiquorTree
  },
  data: () => ({
    datatree: '',
    authtreeData: [
      {text: 'dooroverlord',
        children: [
          {text: 'Logout'},
          {text: 'Users',
            children: [
              {text: 'edituser'},
              {text: 'listallowed'},
              {text: 'adduser'}
            ]
          },
          {text: 'doors',
            children: [
              {text: 'statuslog'}
            ]
          }
        ]
      }
    ],
    usertreeData: [
      {text: 'dooroverlord',
        children: [
          {text: 'UpdateUser'},
          {text: 'Logout'}
        ]
      }
    ],
    nonauthtreeData: [
      {text: 'login'}
    ]
  }),
  mounted: function () {
    console.log('nav user = ' + this.$auth.user().username)
    if (this.$auth.check()) {
      if (this.$auth.check('admin') || this.$auth.check('user')) {
        console.log('nav user OK')
      } else {
        console.log('logging out')
        console.log('logout user = ' + this.$auth.user().username)
        this.logout()
      }
    }
  }
  // created: function () {
  //   console.log('nav user = ' + this.$auth.user().username)
  //   if (this.$auth.check()) {
  //     if (this.$auth.check('admin')) {
  //       this.datatree = this.authtreeData
  //     }
  //     if (this.$auth.check('user')) {
  //       this.datatree = this.usertreeData
  //     } else {
  //       this.logout()
  //     }
  //   } else {
  //     this.datatree = this.nonauthtreeData
  //   }
  // }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
/*.navbar-right { margin-right: 0px !important}

.log {
  margin: 5px 10px 0 0;
}*/
.menu {
  font-size: 200%;
}
</style>
