const list = require('./list')
const { ipcMain } = require('electron')

module.exports = (window) => {
  Object.keys(list).forEach(item => {
    console.log(`call-function-${list[item]}`)
    ipcMain.on(`call-function-${list[item]}`, (event, arg) => {
      require('./' + item).main(window)(...arg.args).then(data => {
        event.sender.send(`callback-succeed-function-${arg.uuid}`, data)
      }).catch(data => {
        event.sender.send(`callback-failed-function-${arg.uuid}`, data)
      })
    })
  })
}
