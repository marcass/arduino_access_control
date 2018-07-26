<template>
  <div class='wrapper'>
    <div class='main-head'>
      <app-nav></app-nav>
      <h1>Boiler behaviour</h1>
    </div>
    <div class='side'>
      <select v-model="graph_items" multiple>
        <option disabled value="">Select attribute(s) to graph</option>
        <option v-for="item in values" v-bind:key="item">{{ item }}</option>
      </select>
      <select v-model="range">
        <option disabled value="">Select graph range</option>
        <option value="hours">Hours</option>
        <option value="days">Days</option>
      </select>
      <select v-model="period">
        <option disabled value="">Select graph period</option>
        <option v-for="n in 365" v-bind:key="n">{{ n }}</option>
      </select>
      <button v-on:click="graph({'items':graph_items, 'range':range, 'period':period})">Make the graph</button>
    </div>
    <div class='content'>
      <vue-plotly :data="this.data" :layout="this.layout" :options="options"/>
    <!-- <vue-plotly :data="data[1]" :layout="layout" :options="options"/> -->
    </div>
  </div>
</template>

<script>
import { getBoilerData, getBoilerValues, postCustomData } from '../../../utils/door-api'
import AppNav from '../AppNav'
import VuePlotly from '@statnett/vue-plotly'
// import Plotly from 'plotly.js/dist/plotly'
export default {
  name: 'doors',
  data () {
    return {
      data: [],
      period: 1,
      range: 'days',
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
        // console.log(ret)
        this.data = ret
      })
    },
    getValues () {
      getBoilerValues().then((ret) => {
        // console.log(ret)
        this.values = ret
      })
    },
    graph (payload) {
      postCustomData(payload).then((ret) => {
        this.data = ret
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
