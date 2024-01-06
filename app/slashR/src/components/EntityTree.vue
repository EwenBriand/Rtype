<template>
    <div class="entitytree">
        <EntityNode :node="entityTree" depth="0" iconPath="../assets/icon-entity-purple.png"/>
    </div>
</template>

<script setup>
  import { ref, defineProps, onMounted } from 'vue';
  import { getConfigData, getDirs } from '../scripts/getPaths';
  import { getTreeData } from '../scripts/getTreeData';
  import EntityNode from './EntityNodes.vue';

  const props = defineProps(['projectName', 'sceneName']);

  const paths = ref({});
  const entityPath = ref('');
  const projectName = ref(props.projectName);
  const sceneName = ref(props.sceneName);
  const entityTree = ref({});

  onMounted(async () => {
    const data = await getConfigData();
    paths.value = getDirs(data);
    if (sceneName.value !== undefined) {
        entityPath.value = `${paths.value.projectsdir}${projectName.value}/.engine/scenes/${sceneName.value}/`;
        entityTree.value = await getTreeData(entityPath.value);
    }
  });
</script>

<script>
  export default {
    name: 'EntityTree',
  };
</script>

<style>
  .entitytree {
    display: flex;
    flex-direction: column;
    height: 100%;
    width: 100%;
    overflow-y: scroll;
  }

  
</style>
