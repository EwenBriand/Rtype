<script setup>
    import { ref } from 'vue'
    const history = ref([])
    const cmdInput = ref('')

    const addToHistory = (line) => {
        history.value.push("console > " + line);
    }

    addToHistory("Hello world");
    addToHistory("Big mega enormous text that is so big that it is bigger than the screen bigger and kjksdjdjkhzs fs fs df sdf se fdg sdjkfhsodfh osief zojsrdfo_hz eohz erfoh ezfhspùofjzeoidhfoqs  pj osjfùsporjj zùeoijf fjspeojfggôsejf esfgpo ezofihjsdùpoj er^f fezùpfj zeoifjeùpiojezpifjoùi erùipf eoijùezpofj zeofùpzoierjfozeijf ù pesojf es    poefgjé   j oi     o&m    h&  oh  om  o                iomoaih    oh  miuh    moiuh   moiu    h   iouh    mouih   iopmuh              " );

</script>

<script>
export default {
    name: 'MyTerminal',
    props: {
        dataPath: String
    },
}
</script>

<template>
    <div class="terminal-body">
        <div class="terminal-body-history">
            <p v-for="line in history" :key="line" class="line">
                {{ line }}
            </p>
        </div>
        <div class="terminal-body-input">
            <p class="input-style">console > </p>
            <input v-model="cmdInput" @keyup.enter="addToHistory(cmdInput)" class="input" type="text"/>
        </div>
    </div>
</template>

<style>

    .terminal {
        width: 100%;
        height: 100%;
        background-color: #272727;
        display: flex;
    }

    .terminal-body {
        display: flex;
        flex-direction: column;
        background-color: #272727;
        height: 94%;
    }

    .terminal-body-history {
        padding-top: 5px;
        overflow-y: scroll;
        max-height: 90%;
        padding-left: 5px;
        background-color: #272727;
        font-size: 12px;
        font-family: 'Roboto Mono', monospace;
    }

    .terminal-body-history > .line {
        color: white;
        margin: 0;
        padding: 0;
        margin-bottom: 5px;
    }

    .terminal-body-input {
        background-color: #272727;
        display: flex;
        flex-wrap: nowrap;
    }

    .input-style {
        align-self: center;
        color: #AD2A2A;
        font-size: 12px;
        font-family: 'Roboto Mono', monospace;
        font-weight: 600;
        margin-left: 5px;
        margin-right: 5px;
    }

    .input {
        align-self: center;
        background-color: #272727;
        width: 80%;
        border: none;
        outline: none;
        color: white;
        font-size: 12px;
        font-family: 'Roboto Mono', monospace;
    }

 /* Scroll bar stylings */
    ::-webkit-scrollbar {
        width: 4px;
        height: 10px;
      }
      /* Handle */
      ::-webkit-scrollbar-thumb {
        background: #888;
        border-radius: 5px;
      }

      /* Handle on hover */
      ::-webkit-scrollbar-thumb:hover {
        background: #555;
        cursor: pointer;
      }


    /* .line {
        background-color: sandybrown;
    } */
</style>