<template>
    <div class="container">
        <div class="box">
                <div class="leftPart">
                    <p class="eCreatorTitle">Select or create a scene</p>
                    <div class="inputBox">
                        <input v-model="cmdInput" @keyup.enter="createNewScene(cmdInput)" class="input" type="text" placeholder="Scene name"/>
                        <img @click="createNewScene(cmdInput)" class="createIcon" alt="add icon" src="../assets/icon-add-big.png" />
                    </div>
                </div>
                <div class="sceneList">
                    <img class="sceneSeparator" alt="sceneSeparator" src="../assets/separator.svg" />
                    <template v-for="(item, index) in sceneList" :key="index">
                        <li @click="addScene(item)" class="sceneElement" > {{ item }} </li>
                        <img class="sceneSeparator" alt="sceneSeparator" src="../assets/separator.svg" />
                    </template>
                </div>
        </div>
    </div>
</template>

<script setup>
    import { ref } from 'vue';

    const getScene = () => {
        return ["SceneBoss", "NewScene"]
    }

    const createNewScene = (name) => {
        if (name === undefined || name == "")
            console.log("Creating NewScene")
        else
            console.log("Creating ", name)
    }

    const addScene = (name) => {
        console.log("Adding ", name)
    }
    const cmdInput = ref('');
    const sceneList = getScene();
</script>

<script>
export default {
    name: 'EntityCreator',
}
</script>

<style>


    .container {
        width: 100%;
        height: 100%;
        background-color: #272727;
        display: flex;
        justify-content: center;
        align-items: center;
    }

    .box {
        width: 80%;
        height: 70%;
        background-color: #1D1D1D;
        display: flex;
        flex-direction: row;
        justify-content: space-between;
        border-radius: 15px;
        overflow-y: auto;
    }

    .leftPart {
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        margin-left: auto;
        margin-right: auto;
        padding-left: 10px;
    }

    .eCreatorTitle {
        font-family: "Roboto-Light", Helvetica;
        font-size: 20px;
        font-weight: 600;
        text-align: center;
    }

    .inputBox {
        border-radius: 12px;
        display: flex;
    }

    .input {
        border-radius: 5px;
    }

    .createIcon {
        width: 25px;
        height: 25px;
        margin-left: 5px;
        border-radius: 40px;
    }

    .createIcon:hover {
        background-color: #4D4D4D;
    }


    .sceneList {
        margin-right: 20px;
        padding-right: 10px;
        padding-top: 10px;
    }

    .sceneElement {
        color: #ffffff;
        font-family: "Roboto-Light", Helvetica;
        font-size: 14px;
        list-style: none;
        padding-left: 20px;
        padding-top: 10px;

    }

    .sceneElement:hover {
        background-color: #4D4D4D;
    }

    .sceneSeparator {
        margin: 0px;
        padding: 0px;
    }

</style>