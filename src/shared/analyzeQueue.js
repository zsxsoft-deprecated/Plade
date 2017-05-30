const os = require('os')
const bluebird = require('bluebird')
const cp = require('child_process')
const path = require('path')

module.exports.default = class AnalyzeQueue {
  static startQueue (list, succeedCallback, rejectCallback) {
    const cpuCount = os.cpus().length
    const memoryFreeCount = Math.floor(os.totalmem() / 1024 / 1024 / 512)
    const queueCount = cpuCount < memoryFreeCount ? cpuCount : memoryFreeCount
    bluebird.map(list, item => new Promise((resolve, reject) => {
      const event = cp.fork(path.resolve('src/main/analyzer'), [item])
      event.on('message', (message) => {
        if (message === 'ok') {
          resolve()
          if (succeedCallback) succeedCallback(item)
        } else {
          resolve() // reject(message)
          if (succeedCallback) rejectCallback(item, message)
        }
      })
      event.on('error', resolve)
    }), {concurrency: queueCount})
  }
}
