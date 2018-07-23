<template>
  <div>
    <app-nav></app-nav>
    <h1>Boiler behaviour</h1>
    <div>
      <select v-model="graph_items" multiple>
        <option disabled value="">Select attribute(s) to graph</option>
        <option v-for="item in values" v-bind:key="item">{{ item }}</option>
      </select>
    </div>
    <vue-plotly :data="data" :layout="layout" :options="options"/>
    <!-- <vue-plotly :data="data[1]" :layout="layout" :options="options"/> -->
  </div>
</template>

<script>
import { getBoilerData, getBoilerValues } from '../../../utils/door-api'
import AppNav from '../AppNav'
import VuePlotly from '@statnett/vue-plotly'
export default {
  name: 'doors',
  data () {
    return {
      data: [],
      period: 1,
      range: '',
      values: [],
      graph_items: [],
      layout: {
        'title': 'Boiler data',
        'yaxis': {'title': 'Temperature'},
        'yaxis2': {'title': 'Percent', 'overlaying': 'y', 'side': 'right'}
      },
      options: {}
    }
  },
  components: {
    AppNav,
    VuePlotly
  },
  methods: {
    getData () {
      getBoilerData().then((ret) => {
        console.log(ret)
        this.data = ret
      })
    },
    getValues () {
      getBoilerValues().then((ret) => {
        console.log(ret)
        this.values = ret
      })
    }
  },
  mounted () {
    this.getData()
    this.getValues()
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
