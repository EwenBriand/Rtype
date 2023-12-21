<template>
  <slashrLauncher />
</template>

<script>
import slashrLauncher from './components/Launcher.vue';

export default {
  name: 'App',
  components: {
    slashrLauncher,
  }
}

</script>

<style>
@import url('https://fonts.googleapis.com/css2?family=Bebas+Neue&display=swap');
#app {
  font-family: 'Roboto', sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
}
</style>
