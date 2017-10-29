<template>
    <div>
        <h1>Admin</h1>

        <router-link :to="{name: 'users-list'}">User list</router-link>

        <hr/>

        <router-view></router-view>
    </div>
</template>
