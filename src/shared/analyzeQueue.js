const path = require('path')
const NativeQueue = require('./nativeQueue')

module.exports.default = class AnalyzeQueue {
  static startQueue (...args) {
    const queue = new NativeQueue(path.resolve('src/main/analyzer'))
    queue.startQueue(...args)
  }
}
