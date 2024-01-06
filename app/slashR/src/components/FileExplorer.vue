<template>
    <div class="fileExplorerContainer">
        <div class="fileExplorerTree">
            <FileNode :node="fileExplorerTree" :depth="0" @childValue="selectFilePath"/>
        </div>
        <div class="fileViewer">
            <template v-for="(file, index) in viewFileList" :key="index">
                <div class="viewFile">
                    <div class="viewFileBox">
                        <p>file</p>
                    </div>
                    <div class="viewFileName">{{ file }}</div>
                </div>
            </template>
        </div>
    </div>
</template>

<script setup>
    import { ref, defineProps, onMounted } from 'vue'
    import { getConfigData, getDirs } from '../scripts/getPaths';
    import { getTreeData } from '../scripts/getTreeData';
    import FileNode from './FileNodes.vue';

    const props = defineProps(['projectName']);
    const paths = ref({});
    const projectName = ref(props.projectName);
    const projectPath = ref("");
    const fileExplorerTree = ref({});
    // const viewFilePath = ref("");
    const viewFileList = ref(['newFeature.txt', 'coming.soon', 'stay.tuned']);

    const selectFilePath = (path) => {
        // viewFilePath.value = path;
        // viewFileList.value = getList();
    };

    onMounted(async () => {
        const data = await getConfigData();
        paths.value = getDirs(data);
        projectPath.value = `${paths.value.projectsdir}${projectName.value}/`;
        fileExplorerTree.value = await getTreeData(projectPath.value);
    });
</script>

<script>
    export default {
        name: 'FileExplorer',
    }
</script>


<style>
    .fileExplorerContainer {
        display: flex;
        flex-direction: row;
        height: 100%;
        overflow-y: hidden;
    }

    .fileExplorerTree {
        background-color: #3C3C3C;
        display: flex;
        flex-direction: row;
        overflow-y: scroll;
        width: 45%;
        height: 100%;
    }

    .fileViewer {
        display: flex;
        flex-direction: row;
        flex-wrap: wrap;
        justify-content: flex-start;
        align-items: flex-start;
        width: 50%;
        padding: 5px;
        overflow-y: scroll;
    }

    .viewFile {
        display: flex;
        flex-direction: column;
        height: 80px;
        width: 80px;
        align-items: center;
    }

    .viewFileBox {
        background-color: #D9D9D9;
        display: flex;
        justify-content: center;
        height: 50px;
        width: 50px;
    }

    .viewFileBox > p {
        color: #3C3C3C;
    }
    .viewFileName {
        display: flex;
        justify-content: center;
        color: white;
        margin-top: 5px;
        font-size: 12px;
    }
</style>