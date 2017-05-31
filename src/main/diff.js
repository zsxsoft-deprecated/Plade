const fs = require('fs')
const path = require('path')
const dirName = process.argv[2]
const similar = require('bindings')({
  bindings: 'PladeSimilar.node',
  module_root: path.resolve(__dirname, '../../')
})

try {
  const astCacheDir = path.resolve(__dirname, '../../data/cache-ast')
  const astCacheFileName = astCacheDir + '/' + dirName
  const fileNameData = fs
  .readdirSync(astCacheDir)
  .map(file => ({fileName: file, path: path.resolve(astCacheDir, file)}))
  .filter(file => fs.statSync(file.path).isFile())
  .filter(file => file.fileName > dirName)
  const data = similar.similar(astCacheFileName, fileNameData.map(data => data.path), (index, ret) => {
    process.send({message: 'data', cacheName: fileNameData[index].fileName, score: ret})
  })
  process.send({message: 'ok', data})
  process.exit(0)
} catch (e) {
  process.send(e)
  process.exit(1)
}
