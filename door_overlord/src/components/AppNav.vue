<template>
    <div class="navbar-header menu">
      <!-- <span v-show="!$auth.check()"> -->
      <!-- <span>
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




      <ul class="menu">
          <span v-show="!$auth.check()">
            <tree
              :data="nonauthtreeData"
              @node:selected="onNodeSelected"
            />
            <!-- <li class="menu">
              <router-link :to="{name: 'login'}">login</router-link>
            </li> -->
          </span>
          <span v-show="$auth.check()">
            <span v-show="$auth.check('admin')">
              <tree
                :data="authtreeData"
                class="tree--small"
                @node:selected="onNodeSelected"
              />
              <!-- <li class="menu">
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
              </li> -->
            </span>
              <!-- <li class="menu">
                <router-link :to="{name: 'Hello'}">Hello</router-link>
              </li> -->
              <span v-show="$auth.check('user')">
                <tree
                  :data="usertreeData"
                  @node:selected="onNodeSelected"
                />
                <!-- <li class="menu">
                  <router-link :to="{name: 'userupdate'}">Update user details</router-link>
                </li> -->
              </span>
              <!-- <li class="menu">
                <a v-on:click='logout()' href="javascript:void(0);">Logout</a>
              </li> -->
            </span>
          </span>
        </li>
      </ul>
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
          console.log('success ' + this.context)
        },
        error () {
          console.log('error ' + this.context)
        }
      })
    },
    onNodeSelected (node) {
      // var data = this.data
      console.log('data ' + node.text)
      if (node.text === 'Logout') {
        this.$auth.logout({
          makeRequest: false,
          success () {
            console.log('success ' + this.context)
          },
          error () {
            console.log('error ' + this.context)
          }
        })
      } else {
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
    authtreeData: [
      {text: 'Menu',
        children: [
          {text: 'Logout'},
          {text: 'Users',
            children: [
              {text: 'edituser'},
              {text: 'listallowed'},
              {text: 'adduser'}
            ]
          },
          {text: 'Doors',
            children: [
              {text: 'statuslog'},
              {text: 'usekey'},
              {text: 'doors'}
            ]
          }
        ]
      }
    ],
    usertreeData: [
      {text: 'Menu',
        children: [
          {text: 'UpdateUser'},
          {text: 'Logout'}
        ]
      }
    ],
    nonauthtreeData: [
      {text: 'login'}
    ]
  })
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.navbar-right { margin-right: 0px !important}

.log {
  margin: 5px 10px 0 0;
}
</style>
