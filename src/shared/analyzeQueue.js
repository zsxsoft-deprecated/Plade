const path = require('path')
const NativeQueue = require('./nativeQueue')

module.exports.default = class AnalyzeQueue {
  startQueue (list, succeedCallback, rejectCallback) {
    const queue = new NativeQueue(path.resolve('src/main/analyzer'))
    queue.startQueue(list, succeedCallback, rejectCallback)
  }
}
