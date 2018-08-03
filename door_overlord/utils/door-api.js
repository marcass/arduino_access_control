//following block for jwt (see https://github.com/websanova/vue-auth/blob/master/docs/StepByStepGuide.md)

import axios from 'axios'
import VueAxios from 'vue-axios'
import Vue from 'vue'

Vue.use(VueAxios, axios)

const BASE_URL = 'https://skibo.duckdns.org/api';
Vue.axios.defaults.baseURL = BASE_URL;
axios.defaults.headers.post['Content-Type'] = 'application/json';
axios.defaults.headers.put['Content-Type'] = 'application/json';
axios.defaults.headers.delete['Content-Type'] = 'application/json';

// access axios with Vue or use the 'this' reference in components
// Vue.axios.post(...).then(res => {
//     console.log('RES', res);
// });

export {getBoilerState, getBoilerData, getBoilerValues, postCustomData, getADoorStatus, getUsers, getUser, getDoors, getDoorStatus, getAllowedUsers, postCreds, putUserData, putAllUserData, postUserData, postKeycode, deleteDoorUser, postAuth, getVerifyUser, getLog, postDoor, delDoor, userData};

function simple_get(url) {
  return axios.get(url)
  .then(function (response) {
      return response.data
  });
}

function getUsers() {
  const url = BASE_URL+'/users'
  return simple_get(url)
}

function getBoilerData() {
  const url = BASE_URL+'/boiler'
  return simple_get(url)
}

function getBoilerState() {
  const url = BASE_URL+'/boiler/state'
  return simple_get(url)
}

function getBoilerValues() {
  const url = BASE_URL+'/boiler/values'
  return simple_get(url)
}

function postCustomData(payload) {
  const url = BASE_URL+'/boiler'
  return axios.post(url, payload);
}

function getUser(user) {
  const url = BASE_URL+'/user/data/'+user
  return simple_get(url)
}

function userData(user) {
  const url = BASE_URL+'/user/data/'+user
  return simple_get(url)
}

function getVerifyUser(user, payload) {
  const url = BASE_URL+'/auth/user/'
  return axios.post(url+user, payload)
  .then(function (response) {
      return response.data
  });
}

function getDoors() {
  const url = BASE_URL+'/doors'
  return simple_get(url)
}

function getDoorStatus() {
  const url = BASE_URL+'/door/status'
  return simple_get(url)
}

function getADoorStatus(door) {
  const url = BASE_URL+'/door/status/'
  return simple_get(url+door)
}

function getLog(door, payload) {
  const url = BASE_URL+'/door/log/'
  return axios.post(url+door, payload)
  .then(function (response) {
      return response.data
  });
}

function getAllowedUsers() {
  const url = BASE_URL+'/listallowed'
  return simple_get(url)
}

function login(payload) {
  // console.log(payload)
  const url = BASE_URL+'/auth'
  return axios.post(url, payload)
  // .then(res => {
  //   console.log('RES', res)
  // })
}
function putUserData(payload, route) {
  const url = BASE_URL+'/user/'
  return axios.put(url+route, payload);
}

function putAllUserData(payload) {
  const url = BASE_URL+'/user'
  return axios.put(url, payload);
}

function postUserData(payload) {
  const url = BASE_URL+'/user'
  return axios.post(url, payload);
}

function postKeycode(payload) {
  const url = BASE_URL+'/usekey'
  return axios.post(url, payload)
  .then(function (response) {
      return response.data
  });
}

function postDoor(payload) {
  const url = BASE_URL+'/door/setup'
  return axios.post(url, payload)
  .then(function (response) {
      return response.data
  });
}

function deleteDoorUser(user) {
  const url = BASE_URL+'/user/'
  // console.log(user)
  return axios.delete(url+user)
  .then(function (response) {
      return response.data
  });
}

function delDoor(door) {
  const url = BASE_URL+'/door/'
  // console.log(user)
  return axios.delete(url+door)
  .then(function (response) {
      return response.data
  });
}

function postAuth(user, pass) {
  const url = BASE_URL+'/auth'
  return axios.post(user, pass)
  .then(function (response) {
      return response.data
  });
}
