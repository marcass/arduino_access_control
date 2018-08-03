<template>
  <div class='wrapper'>
    <div class='main-head'>
      <app-nav></app-nav>
      <h1>Boiler behaviour</h1>
      <h3>Boiler state is {{ state }}</h3>
    </div>
    <div class='side'>
      <select v-model="graph_items" multiple>
        <option disabled value="">Select attribute(s) to graph</option>
        <option v-for="item in values" v-bind:key="item">{{ item }}</option>
      </select>
      <select v-model="range">
        <option disabled value="a">Select graph range</option>
        <option v-for="item, index in label" :value="val[index]">{{ item }}</option>
        <!-- <option v-for="item in table" :value="item.value", v-bind:key="item.label">{{ item.label }}</option> -->
        <!-- <option value="days">Days</option> -->
      </select>
      <select v-model="period" v-if="range == '24_hours'">
        <option disabled value="">Select graph period in hours</option>
        <option v-for="n in 24" v-bind:key="n">{{ n }}</option>
      </select>
      <select v-model="period" v-if="range == '7_days'">
        <option disabled value="">Select graph period in days</option>
        <option v-for="n in 7" v-bind:key="n">{{ n }}</option>
      </select>
      <select v-model="period" v-if="range == '2_months'">
        <option disabled value="">Select graph period in days</option>
        <option v-for="n in 60" v-bind:key="n">{{ n }}</option>
      </select>
      <select v-model="period" v-if="range == '1_year'">
        <option disabled value="">Select graph period in Months</option>
        <option v-for="n in 12" v-bind:key="n">{{ n }}</option>
      </select>
      <select v-model="period" v-if="range == '5_years'">
        <option disabled value="">Select graph period in Years</option>
        <option v-for="n in 5" v-bind:key="n">{{ n }}</option>
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
import { getBoilerData, getBoilerValues, postCustomData, getBoilerState } from '../../../utils/door-api'
import AppNav from '../AppNav'
import VuePlotly from '@statnett/vue-plotly'
// import Plotly from 'plotly.js/dist/plotly'
export default {
  name: 'doors',
  data () {
    return {
      data: [],
      period: 1,
      range: '7_days',
      table: [{'label': 'Hours', 'val': '24_hours'}, {'label': 'Days', 'val': '7_days'}, {'label': 'Months', 'val': '2_months'}, {'label': 'Year', 'val': '1_year'}, {'label': 'Years', 'val': '5_years'}],
      val: ['24_hours', '7_days', '2_months', '1_year', '5_years'],
      label: ['Hours', 'Days', 'Months', 'Year', 'Years'],
      // label: ['a', 'b', 'c'],
      values: [],
      graph_items: [],
      layout: {
        'title': 'Boiler data',
        'yaxis': {'title': 'Temperature'},
        'yaxis2': {'title': 'Percent', 'overlaying': 'y', 'side': 'right'}
      },
      options: {},
      timeRes: '',
      state: ''
    }
  },
  components: {
    AppNav,
    VuePlotly
  },
  methods: {
    getData () {
      getBoilerData().then((ret) => {
        this.data = this.convTime(ret)
      })
    },
    getValues () {
      getBoilerValues().then((ret) => {
        console.log(ret)
        this.values = ret.values
      })
    },
    graph (payload) {
      // console.log(payload)
      postCustomData(payload).then((ret) => {
        // console.log(ret)
        this.data = this.convTime(ret.data)
      })
    },
    convTime (data) {
      var i
      for (i in data) {
        var arr = data[i].x
        var dates = function (element, index, arr) {
          element = new Date(element)
          return element
        }
        const result = arr.map(dates)
        data[i].x = result
      }
      return data
    },
    getState () {
      getBoilerState().then((ret) => {
        console.log(ret)
        this.state = ret.state
      })
    }
  },
  mounted () {
    this.getData()
    this.getValues()
    this.getState()
    // this.dateConvert('2018-08-01T10:02:07.862211072Z')
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
