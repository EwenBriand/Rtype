<template>
    <!-- <div class="sidebar">
      <div class="home">
        <img src="../assets/icon-home.png" alt="home" />
      </div>

      <img src="../assets/divider.png" alt="divider" /> -->

      <!-- <div class="menu">
        <div class="editor">
          <div class="selector">
            <img src="../assets/selector.png" alt="selector" />
          </div>
          <img src="../assets/icon-editor.png" alt="editor" />
        </div>
        <div class="animator">
          <img src="../assets/icon-animator.png" alt="animator" />
        </div>
        <div class="workshop">
          <img src="../assets/icon-workshop.png" alt="workshop" />
        </div>
      </div>
      <div class="settings">
        <img src="../assets/icon-settings.png" alt="settings" />
      </div>
    </div> -->

    <div class="entityTree">
      <div class="header">
        <ul class="currentFilesList">
          <img @click="openSceneSelector" src="../assets/icon-add.png" alt="add scene" class="addSceneLogo"/>
          <li class="currentFilename" v-for="file in currentFiles" :key="file">
            <img @click="removeSceneFromList" src="../assets/icon-close.png" alt="close" class="closeFile"/>
            <div @click="selectSceneFromList(file)">{{ file }}</div>
          </li>
        </ul>
      </div>
      <EntityCreator v-if="currentFiles[fileSelected] == 'SceneSelector'"/>
    </div>

    <div class="fileExplorer">
      <div class="header">
        <div @click="changeComponentToFile" class="selectorExplorer" :class="{componentSelected: componentSelectedId == 1}">
          <img src="../assets/icon-explorer.png" alt="fileExplorer" />
          <h3 class="selectorExplorerText">Explorer</h3>
        </div>

        <div @click="changeComponentToTerminal" class="selectorTerminal" :class="{componentSelected: componentSelectedId == 2}">
          <img src="../assets/icon-terminal.png" alt="terminal" />
          <h3 class="selectorTerminalText">Terminal</h3>
        </div>
      </div>

      <FileExplorer v-if="componentSelectedId == 1" />
      <MyTerminal v-if="componentSelectedId == 2"/>
    </div>

    <div class="properties">
      <div class="header">
        <p>{{ projectName }}</p>
      </div>
    </div>
</template>

<script setup>
  import { ref } from "vue";
  import { useRouter } from "vue-router";

  let componentSelectedId = ref(1);
  let currentFiles = ref([]);
  let fileSelected = ref(-1);
  const router = useRouter();
  const projectName = router.currentRoute.value.params.projectName;

  const changeComponentToFile = () => {
    componentSelectedId.value = 1;
  };

  const changeComponentToTerminal = () => {
    componentSelectedId.value = 2;
  };

  const openSceneSelector = () => {
    const existingFileIndex = currentFiles.value.indexOf('SceneSelector');
    if (existingFileIndex != -1) {
      fileSelected.value = existingFileIndex;
      return;
    }
    currentFiles.value.push('SceneSelector');
    fileSelected.value = fileSelected.value + 1;
  };

  const selectSceneFromList = (file) => {
    fileSelected.value = currentFiles.value.indexOf(file.value);
  };

  const removeSceneFromList = () => {
    currentFiles.value.splice(fileSelected.value, 1);
    if (fileSelected.value > -1)
      fileSelected.value = fileSelected.value - 1;
  };

</script>

<script>
  import FileExplorer from "./FileExplorer.vue";
  import MyTerminal from "./Terminal.vue";
  import EntityCreator from "./EntityCreator.vue";

  export default {
    name: "workspaceEmpty",
    components: {
      FileExplorer,
      MyTerminal,
    },
  };
</script>

<style>

  /* SIDEBAR */
    .sidebar {
        background-color: #1D1D1D;
        width: 50px;
        height: 100%;
        position: absolute;
        left: 0;
        top: 0;
        display: flex;
        flex-direction: column;
    }

    .sidebar > .home {
        align-self: center;
        margin-top: 5px;
        margin-bottom: 5px;
    }

    .sidebar > img {
        width: 90%;
        height: 3px;
        align-self: center;
    }
    .sidebar > .menu {
        margin-top: 20px;
        align-self: center;
        display: flex;
        flex-direction: column;
        gap: 10px;
    }

    .sidebar > .settings {
        bottom: 1px;
        position: absolute;
        align-self: center;
    }

    .selector {
        position: absolute;
        width: 45px;
        height: 0px;
        left: 49px;
        top: 33px;

        box-shadow: 0px 0px 33px #AD2A2A, 0px 0px 128px #AD2A2A, inset 0px 0px 13px #AD2A2A;
        filter: blur(1.5px);
        transform: rotate(90deg);
    }

    .selector > img {
      transform: rotate(90deg);
    }

    /* ENTITY TREE */

    .entityTree {
        background-color: #272727;
        display: flex;
        flex-direction: column;
        position: absolute;
        width: 50%;
        height: 50%;
        left: 50px;
        top: 0;
    }

    .entityTree > .header {
        display: flex;
        background-color: #3C3C3C;
        height: 32px;
    }

    .entityTree > .header > .currentFilesList {
        display: flex;
        flex-direction: row;
        list-style-type: none;
        margin: 0;
        padding: 0;
    }

    .addSceneLogo {
        height: 20px;
        width: 20px;
        align-self: center;
        margin-left: 5px;
    }

    .addSceneLogo:hover {
        cursor: pointer;
        background-color: #4D4D4D;
        border-radius: 10px;
    }

    .closeFile {
        align-self: center;
        margin-right: 2px;
    }

    .closeFile:hover {
      cursor: pointer;
      background-color: #AD2A2A;
    }

    .entityTree > .header > .currentFilesList > .currentFilename {
        color: white;
        font-family: 'Roboto', sans-serif;
        align-items: center;
        font-size: 11px;
        font-weight: 500;
        background-color: #4D4D4D;
        border-radius: 5px;
        margin: 4px;
        padding: 6px;
        display: flex;
    }

    /* FILE EXPLORER */

    .fileExplorer {
        background-color: #272727;
        display: flex;
        flex-direction: column;
        position: absolute;
        width: 50%;
        height: 50%;
        left: 50px;
        bottom: 0;
    }

    .fileExplorer > .header {
      display: flex;
      padding-top: 2px;
      justify-content: flex-start;
      background-color: #1D1D1D;
      height: 25px;
    }

    .selectorExplorer {
      display: flex;
      flex-direction: row;
      align-items: center;
      margin-left: 10px;
      margin-left: 0px;
      padding-right: 15px;
      padding-left: 5px;
    }

    .selectorExplorer:hover, .selectorTerminal:hover {
      cursor: pointer;
      background-color: #4D4D4D;
    }

    .selectorExplorerText {
      color: white;
      font-family: 'Roboto', sans-serif;
      font-size: 12px;
      font-weight: 700;
      padding-left: 5px;
    }

    .selectorTerminal {
      display: flex;
      flex-direction: row;
      align-items: center;
      margin-left: 5px;
      padding-right: 15px;
      padding-left: 5px;
    }

    .selectorTerminalText {
        color: white;
        font-family: 'Roboto', sans-serif;
        font-size: 12px;
        font-weight: 700;
        padding-left: 4px;
    }

    .componentSelected {
      background-color: #272727;
      border-top-right-radius: 8px;

    }

    /* PROPERTIES */

    .properties {
        background-color: #272727;
        display: flex;
        flex-direction: column;
        position: absolute;
        width: 48%;
        height: 100%;
        right: 0;
        top: 0;
    }

    .properties > .header {
        background-color: #3C3C3C;
        height: 32px;
    }
</style>
