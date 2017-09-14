import axios from 'axios';

const BASE_URL = 'http://localhost:5000';
axios.defaults.headers.post['Content-Type'] = 'application/json';
axios.defaults.headers.put['Content-Type'] = 'application/json';
axios.defaults.headers.delete['Content-Type'] = 'application/json';

export {getUsers, getDoors, getDoorStatus, getAllowedUsers, putUserData, putAllUserData, postUserData, postKeycode};

function getUsers() {
  const url = `${BASE_URL}/users`;
  return axios.get(url).then(response => response.data);
}

function getDoors() {
  const url = `${BASE_URL}/doors`;
  return axios.get(url).then(response => response.data);
}

function getDoorStatus() {
  const url = `${BASE_URL}/door/status`;
  return axios.get(url).then(response => response.data);
}

function getAllowedUsers() {
  const url = `${BASE_URL}/listallowed`;
  return axios.get(url).then(response => response.data);
}

function putUserData(payload, route) {
  const url = `${BASE_URL}/user/`;
  return axios.put(url+route, payload);
}

function putAllUserData(payload) {
  const url = `${BASE_URL}/user`;
  return axios.put(url, payload);
}

function postUserData(payload) {
  const url = `${BASE_URL}/user`;
  return axios.post(url, payload);
}

function postKeycode(payload) {
  const url = `${BASE_URL}/usekey`;
  return axios.post(url, payload);
}
