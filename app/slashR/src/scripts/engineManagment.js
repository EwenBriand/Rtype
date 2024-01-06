import fs from 'fs';
import * as fsp from 'fs/promises'
import { delay } from './utils'
import { getAppPaths } from './getPaths'

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

export const createProject = async (projectName) => {
    const paths = getAppPaths()

    const projectPath = paths.homePath + '.slashR/projects/' + projectName + '/'

    try {
        await fsp.mkdir(projectPath)
        await fsp.mkdir(projectPath + 'assets')
        await fsp.mkdir(projectPath + 'assets/animations')
        await fsp.mkdir(projectPath + 'assets/meshes')
        await fsp.mkdir(projectPath + 'assets/Muse')
        await fsp.mkdir(projectPath + 'assets/prefabs')
        await fsp.mkdir(projectPath + 'assets/sprites')

        await fsp.mkdir(projectPath + '.engine')
        await fsp.mkdir(projectPath + '.engine/config')
        await fsp.mkdir(projectPath + '.engine/scenes')

        await fsp.mkdir(projectPath + 'scripts')


        const baseFd = await fsp.open(projectPath + '.engine/config/base.cfg', 'w')
        const cMakeFd = await fsp.open(projectPath + 'CMakeLists.txt', 'w')

        await fsp.writeFile(baseFd, 'scenesSavePath="../../scenes/"\nuserScriptsPath="../../../scripts/"\nassetRoot="../../../assets/"\ngame="../../../../build/game_rtype/libgame_target.so"\ntmpBuildDir="../../../../"')
        await fsp.writeFile(cMakeFd, 'todo')
        // TODO add Cmake file content

        await baseFd.close()
        await cMakeFd.close()
        return projectName
    } catch (err) {
        if (err.code === 'EEXIST') {
            console.log("Project already exists, testing next", projectName + "1")
            return createProject(projectName + "1")
        }
        console.error("Project creation failed:" + err);
    }
    return projectName
}

export const createScene = async (sceneName, projectName) => {
    const paths = getAppPaths()
    const homePath = paths.homePath
    const scenePath = homePath + '.slashR/projects/' + projectName + '/.engine/scenes/' + sceneName

    try {
        await fsp.mkdir(scenePath)
        return sceneName
    } catch (err) {
        if (err.code === 'EEXIST') {
            console.log("Scene already exists, testing next", sceneName + "1")
            return createScene(sceneName + "1", projectName)
        }
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

