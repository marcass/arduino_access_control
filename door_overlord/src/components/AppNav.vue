<template>
    <!-- <div class="navbar-header menu"> -->
    <div class="menu">

      <!-- Using v-if: -->

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
        <!-- <span v-else>
          <button @click="logout">Logout</button>
        </span> -->
      </span>
      <span v-if="!$auth.check()">
        <tree
          :data="nonauthtreeData"
          @node:selected="onNodeSelected"
        />
      </span>
      <!-- <span v-else>
          <button @click="logout">Logout</button>
      </span> -->


      <!-- Using v-show: -->

      <!-- <span v-show="!$auth.check()">
        <tree
          :data="nonauthtreeData"
          @node:selected="onNodeSelected"
        />
      </span>
      <span v-show="$auth.check()">
        <span v-show="$auth.check('admin')">
          <tree
            :data="authtreeData"
            class="tree--small"
            @node:selected="onNodeSelected"
          />
        </span>
        <span v-show="$auth.check('user')">
          <tree
            :data="usertreeData"
            @node:selected="onNodeSelected"
          />
        </span>
      </span> -->


      <!-- Using mounted method: -->

      <!-- <span>
        <tree
          :data="this.datatree"
          class="tree--small"
          @node:selected="onNodeSelected"
        />
      </span> -->


      <!-- Using text menu: -->

      <!-- <div> -->
        <!-- <h2>Door Overlord</h2> -->
      <!-- </div> -->

      <!-- <ul class="menu">
          <span v-show="!$auth.check()">
            <tree
              :data="nonauthtreeData"
              @node:selected="onNodeSelected"
            />
            <li class="menu">
              <router-link :to="{name: 'login'}">login</router-link>
            </li>
          </span>
          <span v-show="$auth.check()">
            <span v-show="$auth.check('admin')">
              <tree
                :data="authtreeData"
                class="tree--small"
                @node:selected="onNodeSelected"
              />
              <li class="menu">
                <router-link :to="{name: 'Users'}">All Users</router-link>
              </li>
              <li class="menu">
                <router-link :to="{name: 'usekey'}">Actuate a door</router-link>
              </li>
              <li class="menu">
                <router-link :to="{name: 'listallowed'}">Allowed users</router-link>
              </li>
              <li class="menu">
                <router-link :to="{name: 'adduser'}">Add a user</router-link>
              </li>
              <li class="menu">
                <router-link :to="{name: 'edituser'}">Edit users</router-link>
              </li>
              <li class="menu">
                <router-link :to="{name: 'doors'}">Door status</router-link>
              </li>
              <li class="menu">
                <router-link :to="{name: 'statuslog'}">Door status log</router-link>
              </li>
            </span>
              <li class="menu">
                <router-link :to="{name: 'Hello'}">Hello</router-link>
              </li>
              <span v-show="$auth.check('user')">
                <tree
                  :data="usertreeData"
                  @node:selected="onNodeSelected"
                />
                <li class="menu">
                  <router-link :to="{name: 'userupdate'}">Update user details</router-link>
                </li>
              </span>
              <li class="menu">
                <a v-on:click='logout()' href="javascript:void(0);">Logout</a>
              </li>
            </span>
          </span>
        </li>
      </ul> -->
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
