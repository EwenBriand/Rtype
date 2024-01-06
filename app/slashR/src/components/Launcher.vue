
<template>
  <div class="desktop">
    <div class="overlap">
      <div class="red-circle-1" />
      <div class="red-circle-2" />
      <div class="transparent-circle-1" />
      <div class="transparent-circle-2" />
      <div class="purple-circle" />
      <!-- <div class="button-circle-shadow" />
      <div class="button-circle-shadow-2" />
      <div class="image-circle-shadow" /> -->
      <div class="card">
        <h1 class="title-wrapper"> Welcome to SlashR</h1>
        <div class="project-section">
          <h3 class="project-section-title">Recent projects</h3>
          <ul class="project-list">
            <img class="line" alt="Line" src="../assets/separator.svg" />
            <template v-for="(item, index) in projectList" :key="index">
              <li @click="router.push({ name: 'workspace', params: { projectName: item } });" class="project" > {{ item }} </li>
              <img alt="Line" src="../assets/separator.svg" />
            </template>
          </ul>
          <div class="button-section">
            <input v-model="projectName" @keyup.enter="createNewProject(projectName)" class="input" type="text" placeholder="New project name"/>
            <button @click="createNewProject(projectName)" class="button">Create new</button>
          </div>
        </div>
        <div class="image-section">
          <img class="spaceship-image" alt="Spaceship-image" src="../assets/preview-image.png" />
          <div class="image-wrapper">
            <h3 class="image-title">Edit you game</h3>
            <p class="image-text">Create and edit your games as wanted</p>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
  import { ref, onMounted } from 'vue'
  import { getConfigData, getDirs, getList } from '../scripts/getPaths';
  import { createProject } from '../scripts/engineManagment';
  import { useRouter } from 'vue-router'

  let paths = ref({})
  const projectList = ref([])
  const projectName = ref('')
  const router = useRouter();
  const goToProjectName = ref('')

  const createNewProject = async (name) => {
    if (name === undefined || name == "") {
      goToProjectName.value = await createProject("NewProject")
      router.push({ name: 'workspace', params: { projectName: goToProjectName.value } });
    } else {
      goToProjectName.value = await createProject(name)
      router.push({ name: 'workspace', params: { projectName: goToProjectName.value } });
    }
  }

  onMounted(async () => {
    const data = await getConfigData();
    paths.value = getDirs(data);
    projectList.value = await getList(paths.value.projectsdir);
  });
</script>

<script>
  export default {
    name: "slashrLauncher",
  };
</script>

<style>

.desktop .overlap {
    display: flex;
    justify-content: center;
    height: 100%;
    width: 100%;
  }
  .desktop .card {
    display: flex;
    top: 3%;
    flex-wrap: nowrap;
    flex-direction: row;
    justify-content: center;
    align-items: center;
    column-gap: 150px;
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    background-color: #27272780;
    border-radius: 30px;
    box-shadow: inset 0px 4px 20px #ffffff33;
    height: 578px;
    position: absolute;
    width: 928px;
  }

  .desktop .title-wrapper {
    color: #ffffff;
    font-family: "Roboto-Bold", Helvetica;
    font-size: 32px;
    font-weight: 700;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    left: 40px;
    top: 20px;
  }

  .desktop .project-section {
    color: #ffffff;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
  }

  .desktop .project-section-title {
    color: #ffffff;
    font-family: "Roboto-Medium", Helvetica;
    font-size: 24px;
    letter-spacing: 0;
    line-height: normal;
  }

  .desktop .project-list {
    list-style-type: none;
    margin: 0;
    padding: 0;
    overflow-y: scroll;
    height: 200px;
  }

  .desktop .project {
    color: #ffffff;
    font-family: "Roboto-Light", Helvetica;
    font-size: 14px;
    display: flex;
    flex-direction: row;
    justify-content: flex-start;
    padding-left: 30px;
    padding-top: 10px;
  }

  .project:hover {
    color: #AD2A2A;
    cursor: pointer;
  }

  .desktop .button-section {
    display: flex;
    flex-direction: row;
    justify-content: center;
    align-items: center;
    width: 100%;
    margin-top: 50px;
    gap: 50px;
  }

  .button {
    align-items: center;
    background-color: #AD2A2A;
    border-radius: 25px;
    height: 40px;
    width: 140px;
    border: 0;
    cursor: pointer;
    font-family: "Roboto-Medium", Helvetica;
    color: #ffffff;
    font-size: 13px;
    font-weight: 600;
    justify-content: center;
    text-align: center;

  }

  .button:hover {
    background: #8d2121;
    cursor: pointer;
  }

  .desktop .image-section {
    display: flex;
    flex-direction: column;
  }

  .desktop .spaceship-image {
    width: 280px;
    height: 300px;
  }

  .desktop .image-wrapper {
    display: flex;
    flex-direction: column;
    justify-content: flex-start;
    align-items: start;
    background-color: #363636;
    border-bottom-left-radius: 15px;
    border-bottom-right-radius: 15px;
    padding-left: 20px;
  }

  .desktop .image-title {
    color: #ffffff;
    font-family: "Roboto", Helvetica;
    font-size: 16px;
    margin-bottom: 0px;
  }

  .desktop .image-text {
    color: #ffffff;
    font-family: "Roboto-Light", Helvetica;
    font-size: 13px;
    font-weight: 200;
  }

  .desktop .transparent-circle-1 {
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    background-color: #cc1e1e0d;
    border-radius: 44px/44.5px;
    box-shadow: inset 0px 4px 12px #ffffff66, inset 0px -40px 40px #00000033, inset 0px -4px 4px #ffffff40,
    inset 0px 20px 40px #fffbfb40, inset 0px 1px 2px #ffffffcc;
    height: 90px;
    width: 90px;
    position: absolute;
    bottom: 10px;
    right: 300px;
  }
  .desktop .transparent-circle-2 {
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    background-color: #cc1e1e0d;
    border-radius: 44px/44.5px;
    box-shadow: inset 0px 4px 12px #ffffff66, inset 0px -40px 40px #00000033, inset 0px -4px 4px #ffffff40,
    inset 0px 20px 40px #fffbfb40, inset 0px 1px 2px #ffffffcc;
    height: 90px;
    width: 90px;
    left: 420px;
    position: absolute;
  }

  .desktop .red-circle-1 {
    background: linear-gradient(180deg, rgba(225, 135, 135, 0.74) 0%, rgb(238, 66.44, 66.44) 100%);
    border-radius: 120px;
    height: 200px;
    position: absolute;
    width: 200px;
    left: 80px;
    bottom: 20px;
  }

  .desktop .red-circle-2 {
    background: linear-gradient(180deg, rgba(225, 135, 135, 0.74) 0%, rgb(238, 66.44, 66.44) 100%);
    border-radius: 120px;
    height: 110px;
    position: absolute;
    width: 110px;
    left: 40px;
    top: 20px;
  }

  .desktop .purple-circle {
    background: linear-gradient(180deg, rgba(173, 72, 188, 1) 20%, rgb(206.13, 60.98, 156.78) 100%);
    border-radius: 128px;
    height: 256px;
    width: 256px;
    right: 20px;
    position: absolute;
    top: -40px;
  }
  .button-circle-shadow {
    background: linear-gradient(180deg, rgb(225, 135, 135) 0%, rgb(238, 66.44, 66.44) 100%);
    border-radius: 120px;
    height: 50px;
    position: absolute;
    width: 130px;
    bottom: 265px;
    left: 250px;
  }

  .button-circle-shadow-2 {
    background: linear-gradient(180deg, rgb(225, 135, 135) 0%, rgb(238, 66.44, 66.44) 100%);
    border-radius: 120px;
    height: 50px;
    position: absolute;
    width: 130px;
    bottom: 265px;
    left: 450px;
  }

  .image-circle-shadow {
    background: linear-gradient(180deg, rgb(225, 135, 135) 0%, rgb(238, 66.44, 66.44) 100%);
    border-radius: 120px;
    height: 250px;
    position: absolute;
    width: 250px;
    left: 750px;
    bottom: 300px;
  }
</style>
