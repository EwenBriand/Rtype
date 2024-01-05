<script>
import fs from 'fs'

import { app } from '@electron/remote'
import { ref } from 'vue'

export default {
    name: 'FileExplorer',
    props: {
        dataPath: String
    },
    setup() {
        const path = ref(app.getAppPath())
        const fileNames = fs.readdirSync(path.value)
        return {
            appPath: path,
            files: fileNames
        }
    }
}
</script>

<template>
    <div>
        <h1>File Explorer</h1>
        <p>Path: {{ appPath }}</p>
        <p>String: {{ dataPath }}</p>
        <ul>
            <li v-for="file in files" :key="file">
                {{ file }}
            </li>
        </ul>
    </div>
</template>

<style>
    div {
        color: white;
    }
</style>