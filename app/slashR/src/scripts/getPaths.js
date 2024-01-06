import * as fsp from 'fs/promises'

export const getAppPaths = () => {
    const appPath = process.cwd();
    const homePath = getHomePath(appPath);

    return {
        appPath: appPath,
        homePath: homePath,
    };
}

function getHomePath(fullPath) {
    let result = "/";
    let temp = [];
    let flag = false;

    if (fullPath === '/') {
      return fullPath;
    }
    const directories = fullPath.split('/');
    for (const tmpDir of directories) {
        if (tmpDir === "")
            continue;
        if (flag) {
            temp.push(tmpDir);
            break;
        }
        if (tmpDir === "home")
            flag = true;

        temp.push(tmpDir);
    }
    for (const dir of temp) {
        result += dir + '/';
    }
    return result;
}

const getConfigPath = () => {
    const appPath = process.cwd();
    const homePath = getHomePath(appPath);

    return homePath + '.slashR/data/base.cfg';
}

export const getConfigData = async () => {
    const configPath = getConfigPath();

    try {
        const data = await fsp.readFile(configPath,  { encoding: 'utf8' })
        return data;
    } catch (err) {
        console.error("get configuration data failed:" + err)
    }
}

export const getDirs = (data) => {
    const result = {};

    const lines = data.split('\n');
    lines.forEach(line => {
        const [key, value] = line.split('=');
        result[key] = value;
    });
    return result;
}


export const getList = async (path) => {
    try {
        const result = await fsp.readdir(path);
        return result;
    } catch (err) {
        console.error("get project list failed:" + err)
    }
}