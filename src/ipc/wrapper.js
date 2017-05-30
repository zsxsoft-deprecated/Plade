const list = require('./list')
const { ipcRenderer } = require('electron')
const uuidV4 = require('uuid/v4')

module.exports.default = (itemName) => {
  console.log(itemName)
  const itemId = list[itemName]
  return (...args) => new Promise((resolve, reject) => {
    const uuid = uuidV4()
    const succeedChannel = `callback-succeed-function-${uuid}`
    const failedChannel = `callback-failed-function-${uuid}`
    ipcRenderer.send(`call-function-${itemId}`, {
      uuid,
      args
    })
    ipcRenderer.once(succeedChannel, (event, arg) => {
      resolve(arg)
      ipcRenderer.removeAllListeners(failedChannel)
    })
    ipcRenderer.once(failedChannel, (event, arg) => {
      reject(arg)
      ipcRenderer.removeAllListeners(succeedChannel)
    })
  })
}
