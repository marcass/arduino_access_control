import axios from 'axios';

const BASE_URL = 'https://skibo.duckdns.org/api';
axios.defaults.headers.post['Content-Type'] = 'application/json';
axios.defaults.headers.put['Content-Type'] = 'application/json';
axios.defaults.headers.delete['Content-Type'] = 'application/json';

export {getUsers, getDoors, getDoorStatus, getAllowedUsers, putUserData, putAllUserData, postUserData, postKeycode, deleteUser};

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

function getDoors() {
  const url = BASE_URL+'/doors'
  return simple_get(url)
}

function getDoorStatus() {
  const url = BASE_URL+'/door/status'
  return simple_get(url)
}

function getAllowedUsers() {
  const url = BASE_URL+'/listallowed'
  return simple_get(url)
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

function deleteUser(user) {
  const url = BASE_URL+'/user/'
  console.log(user)
  return axios.delete(url+user)
  .then(function (response) {
      return response.data
  });
}
