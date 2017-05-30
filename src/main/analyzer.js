const fs = require('fs')
const path = require('path')
const crypto = require('crypto')
const mkdirp = require('mkdirp')
const dirName = process.argv[2]
const parser = require('bindings')({
  bindings: 'PladeParser.node',
  module_root: path.resolve(__dirname, '../../')
})

const astCacheDir = path.resolve(__dirname, '../../data/cache-ast')
const astCacheFileName = astCacheDir + '/' + crypto.createHash('md5').update(dirName).digest('hex') + '.json'
mkdirp(astCacheDir, (err) => {
  try {
    if (err) {
      process.send(err)
    }
    if (fs.existsSync(astCacheFileName)) {
      process.send('ok')
      process.exit(0)
    }
    const mainFiles = parser.main(dirName)
    if (mainFiles.length === 0) {
      process.send({message: '找不到主文件'})
    }
    const astString = parser.parse(mainFiles[0])
    fs.writeFileSync(astCacheFileName, astString, 'utf-8')
    process.send('ok')
    process.exit(0)
  } catch (e) {
    process.send(e)
    process.exit(3)
  }
})
