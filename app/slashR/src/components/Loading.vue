<template>
  <div class="card">
    <div v-if="installStatus">
      <h1 class="title"> Welcome back </h1>
      <div class="loadingBox">
        <p v-if="percentage > 0" class="percentage">{{ percentage }}%</p>
        <div v-if="percentage > 0" class="loadingBar">
          <div class="loadingBarProgress" :style="{'width': percentage + '%'}"></div>
        </div>
        <p v-if="percentage > 0">{{ loadingMessage }}</p>
      </div>
    </div>
    <div v-else>
      <h1 class="title"> Hello new user</h1>
      <div class="message" v-if="percentage < 1">
        <p>It seem that you don't have installed SlashR yet</p>
        <p>Clic on the button below to install the dependencies</p>
        <button @click="sendToInstaller()" class="installButton">Install dependencies</button>
      </div>

      <div class="loadingBox">
        <p v-if="percentage > 0" class="percentage">{{ percentage }}%</p>
        <div v-if="percentage > 0" class="loadingBar">
          <div class="loadingBarProgress" :style="{'width': percentage + '%'}"></div>
        </div>
        <p v-if="percentage > 0">{{ loadingMessage }}</p>
      </div>
    </div>
  </div>
</template>

<script setup>
  import { ref } from "vue";
  import { getAppPaths } from '../scripts/getPaths'
  import { isInstalled, installer } from '../scripts/engineManagment'
  import { delay } from '../scripts/utils'
  import { useRouter } from 'vue-router'

  const percentage = ref(0);
  const loadingMessage = ref("Loading...");
  const installStatus = ref(false);
  const router = useRouter();
  const paths = getAppPaths();

  installStatus.value = isInstalled(paths.homePath);

  const sendToInstaller = async () => {
    moveLoadingBarTo(95);
    loadingMessage.value = "Installing dependencies..";
    try {
      await installer(paths.homePath, paths.appPath);
      loadingMessage.value = "Install done";
      percentage.value = 100 ;
    } catch (error) {
      console.error(error)
    }
    await delay(1000)
    location.reload();
  }

  const moveLoadingBarTo = (value) => {
    var interval = setInterval(() => {
      if (percentage.value >= value || percentage.value >= 100) {
        clearInterval(interval);
      } else {
        percentage.value += 1;
      }
    }, 10);
  }

  const loadLauncher = async () => {
    moveLoadingBarTo(100);
    await delay(1000)
    router.push('/launcher');
  }

  if (installStatus.value) {
    console.log("Dependencies are installed");
    loadingMessage.value = "Loading data..";
    loadLauncher();
  } else {
    console.log("Dependencies are not installed");
    loadingMessage.value = "Installing dependencies..";
  }

</script>

<script>
export default {
  name: "LoadingPage",
};
</script>

<style>

  p {
    color: white;
  }
  .card {
    display: flex;
    flex-wrap: nowrap;
    flex-direction: column;
    justify-content: center;
    margin-left: auto;
    margin-right: auto;
    margin-top: 8%;
    align-items: center;
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    border-radius: 30px;
    box-shadow: inset 0px 4px 20px #ffffff33;
    height: 578px;
    width: 928px;
  }

  .message {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
  }

  .installButton {
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    border-radius: 20px;
    box-shadow: inset 2px 2px 10px #ffffff33;
    border: none;
    width: 150px;
    height: 35px;
    background-color: transparent;
    color: white;

  }

  .installButton:hover {
    box-shadow: inset 2px 2px 10px #AD2A2A;
  }

  .installButton:active {
    box-shadow: inset 2px 2px 10px #4D4D4D;
    color: #4D4D4D;
  }

  .title {
    font-size: 40px;
    font-family: "Roboto Mono", monospace;
    color: white;
    position: absolute;
    top: 40px;
    left: 60px;
  }
  .loadingBox {
    width: 800px;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
  }

  .percentage {
    font-size: 20px;
    font-family: "Roboto Mono", monospace;
    color: white;
    margin: 0;
    padding: 0;
  }

  .loadingBar {
    width: 300px;
    height: 20px;
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    box-shadow: inset 0px 4px 10px #ffffff33;
    border-radius: 10px;
    margin-top: 10px;
  }
  .loadingBarProgress {
    height: 100%;
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    box-shadow: inset 0px 4px 14px #AD2A2A;
    border-radius: 10px;
  }
</style>
