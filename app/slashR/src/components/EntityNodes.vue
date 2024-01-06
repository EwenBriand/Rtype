<template>
    <ul class="treeContainer">
      <li v-for="(child, key) in props.node" :key="key" class="treeList">
        <div>
          <div @click="toggleNode(key)" class="currentNode">
            <img v-if="depth < 1" alt="icon" src="../assets/icon-entity-purple.png" class="entityIcon"/>
            <p class="nodeValue">{{ key }}</p>
            <img v-if="hasChild(child) && isOpen(key)" alt="drop-icon" src="../assets/icon-drop.png" class="dropIcon"/>
            <img v-if="hasChild(child) && !isOpen(key)" alt="drop-icon" src="../assets/icon-undrop.png" class="unDropIcon"/>
          </div>
          <EntityNode :node="child" depth="1" v-if="hasChild(child) && isOpen(key)" class="noSpace"/>
          </div>
      </li>
    </ul>
</template>

<script setup>
  import { ref, defineProps } from 'vue';

  const props = defineProps(['node', 'depth']);
  const openNodes = ref([]);

  const isOpen = (key) => openNodes.value.includes(key);

  const toggleNode = (key) => {
    if (isOpen(key)) {
      openNodes.value = openNodes.value.filter((node) => node !== key);
    } else {
      openNodes.value.push(key);
    }
  };

  const hasChild = (node) => {
    if (node === undefined || node === null || Object.keys(node).length < 1)
      return false;
    return true;
  };
</script>

<script>
  export default {
    name: 'EntityNode',
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
