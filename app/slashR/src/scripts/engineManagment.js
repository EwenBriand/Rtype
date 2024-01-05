import fs from 'fs';
import * as fsp from 'fs/promises'
import { delay } from './utils'

export const isInstalled = (homePath) => {
    if (fs.existsSync(homePath + '.slashR'))
        return true;
    return false;
}
export const installer = async (homePath, appPath) => {
    try {
        await fsp.mkdir(homePath + '.slashR')
        await fsp.mkdir(homePath + '.slashR/projects')
        await fsp.mkdir(homePath + '.slashR/data')

        const baseFd = await fsp.open(homePath + '.slashR/data/base.cfg', 'w')
        await fsp.writeFile(baseFd, 'homedir=' + homePath +'\ndatadir=' + homePath + './slashR/\nappdir=' + appPath + '/\nprojectsdir=' + homePath + '.slashR/projects/')
        await delay(1000);
    } catch (err) {
        console.error("Installation failed:" + err);
    }
}

export const createProject = async (homePath, projectName) => {
    const projectPath = homePath + '.slashR/projects' + projectName
    console.log("projectPath:" + projectPath)

    try {
        await fsp.mkdir(projectPath)
        await fsp.mkdir(projectPath + '/assets')
        await fsp.mkdir(projectPath + '/assets/meshes') // ask to keep name or not
        await fsp.mkdir(projectPath + '/assets/Muse') // same
        await fsp.mkdir(projectPath + '/assets/sprites') // same
        await fsp.mkdir(projectPath + '/scripts')
        await fsp.mkdir(projectPath + './engine')
        await fsp.mkdir(projectPath + './engine/config')
        await fsp.mkdir(projectPath + './engine/scenes')


        const baseFd = await fsp.open(projectPath + './engine/config/base.cfg', 'w')

        await fsp.writeFile(baseFd, 'scenesSavePath="../../scenes/"\nuserScriptsPath="../../../scripts/"\nassetRoot="../../../assets/"\ngame="../../../../build/game_rtype/libgame_target.so"\ntmpBuildDir="../../../../"')
        await baseFd.close()
    } catch (err) {
        console.error("Project creation failed:" + err);
    }
}

export const createScene = async (homePath, projectName, sceneName) => {
    const scenePath = homePath + '.slashR/projects/' + projectName + '/.engine/scenes/' + sceneName

    try {
        await fsp.mkdir(scenePath)
    } catch (err) {
        console.error("Scene creation failed:" + err);
    }
}

export const createEntity = async (homePath, projectName, sceneName, entityName) => {
    const entityPath = homePath + '.slashR/projects/' + projectName + '/.engine/scenes/' + sceneName + '/' + entityName

    try {
        await fsp.mkdir(entityPath)
    } catch (err) {
        console.error("Scene creation failed:" + err);
    }
}

