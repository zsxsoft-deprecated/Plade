const fs = require('fs')
const path = require('path')
const mkdirp = require('mkdirp')
const dirName = process.argv[2]
const cacheName = process.argv[3]
const parser = require('bindings')({
  bindings: 'PladeParser.node',
  module_root: path.resolve(__dirname, '../../')
})

const astCacheDir = path.resolve(__dirname, '../../data/cache-ast')
const astCacheFileName = astCacheDir + '/' + cacheName
mkdirp(astCacheDir, (err) => {
  try {
    if (err) {
      process.send(err)
    }
    if (fs.existsSync(astCacheFileName)) {
      process.send({message: 'ok', size: fs.statSync(astCacheFileName).size})
      process.exit(0)
    }
    const mainFiles = parser.main(dirName)
    if (mainFiles.length === 0) {
      process.send({message: '找不到主文件'})
      process.exit(1)
    }
    const astString = parser.parse(mainFiles[0])
    if (astString.length === 0) {
      process.send({message: '未知原因'})
      process.exit(1)
    }
    fs.writeFileSync(astCacheFileName, astString, 'utf-8')
    process.send({message: 'ok', size: astString.length})
    process.exit(0)
  } catch (e) {
    process.send(e)
    process.exit(3)
  }
})
