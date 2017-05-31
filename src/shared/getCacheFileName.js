const cache = new Map()
const crypto = require('crypto')

module.exports.default = (fileName) => {
  if (cache.has(fileName)) return cache.get(fileName)
  cache.set(fileName, crypto.createHash('md5').update(fileName).digest('hex'))
  return cache.get(fileName)
}
