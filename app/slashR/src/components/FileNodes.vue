<template>
    <ul class="treeContainer">
      <li v-for="(child, key) in props.node" :key="key" class="treeList">
        <div>
          <div @click="toggleNode(key)" class="currentNode">
            <img v-if="depth < 1" alt="icon" src="../assets/icon-folder.png" class="entityIcon"/>
            <p class="nodeValue">{{ key }}</p>
            <img v-if="hasChild(child) && isOpen(key)" alt="drop-icon" src="../assets/icon-drop.png" class="dropIcon"/>
            <img v-if="hasChild(child) && !isOpen(key)" alt="drop-icon" src="../assets/icon-undrop.png" class="unDropIcon"/>
          </div>
          <FileNode :node="child" depth="1" v-if="hasChild(child) && isOpen(key)" class="noSpace"/>
          </div>
      </li>
    </ul>
</template>

<script setup>
  import { ref, defineProps, defineEmits } from 'vue';
  import { getAppPaths } from '@/scripts/getPaths';

  const props = defineProps(['node', 'depth']);
  const emits = defineEmits(['selectFilePath']);

  const openNodes = ref([]);

  const isOpen = (key) => openNodes.value.includes(key);

  const toggleNode = (key) => {
    if (isOpen(key)) {
      openNodes.value = openNodes.value.filter((node) => node !== key);
    } else {
      openNodes.value.push(key);
    }
    const paths = getAppPaths();
    console.log("node[]: ", props.node);
    console.log("key: ", key);
    console.log("node[key]: ", props.node[key]);
    emits(`childValue`, props.node[key]);
  };

  const hasChild = (node) => {
    if (node === undefined || node === null || Object.keys(node).length < 1)
      return false;
    return true;
  };
</script>

<script>
  export default {
    name: 'FileNode',
  };
</script>

<style>
  .icon-plus::before {
    content: '[+]';
  }

  .icon-minus::before {
    content: '[-]';
  }

  li.treeList {
    list-style-type: none;
  }

  .currentNode {
    display: flex;
    height: 30px;
    align-items: center;
  }

  .nodeValue {
    margin-right: 5px;
    cursor: pointer;
  }

  .dropIcon {
    cursor: pointer;
  }

  .unDropIcon {
    cursor: pointer;
  }

  .entityIcon {
    padding-right: 3px;
    cursor: pointer;
  }
</style>
