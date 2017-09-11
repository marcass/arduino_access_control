import axios from 'axios';

const BASE_URL = 'http://localhost:5000';

export {getUsers, getDoors, doorStatus, getAllowedUsers};

function getUsers() {
  const url = `${BASE_URL}/users`;
  return axios.get(url).then(response => response.data);
}

function getDoors() {
  const url = `${BASE_URL}/doors`;
  return axios.get(url).then(response => response.data);
}

function doorStatus() {
  const url = `${BASE_URL}/door/status`;
  return axios.get(url).then(response => response.data);
}

function getAllowedUsers() {
  const url = `${BASE_URL}/listallowed`;
  return axios.get(url).then(response => response.data);
}
