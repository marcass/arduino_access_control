import axios from 'axios';

const BASE_URL = 'http://localhost:5000';

export {getUsers, getDoors};

function getUsers() {
  const url = `${BASE_URL}/users`;
  return axios.get(url).then(response => response.data);
}

function getDoors() {
  const url = `${BASE_URL}/doors`;
  return axios.get(url).then(response => response.data);
}
