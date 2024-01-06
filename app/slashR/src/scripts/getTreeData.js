import * as fsp from 'fs/promises'

export const getTreeData = async (path) => {
    let tree = {}

    try {
        let files = await fsp.readdir(path)

        for (let i = 0; i < files.length; i++) {
            let file = files[i]
            let stat = await fsp.stat(path + file)
            if (stat.isDirectory()) {
                tree[file] = await getTreeData(path + file + '/')
            } else {
                tree[file] = null
            }
        }
        return tree
    } catch (err) {
        console.log("get tree data failed:" + err)
    }
}