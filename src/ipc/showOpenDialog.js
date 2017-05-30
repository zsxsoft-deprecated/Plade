const wrapper = require('./wrapper')
const { dialog } = require('electron')

module.exports.default = wrapper.default('showOpenDialog')
module.exports.main = window => options => new Promise((resolve, reject) => {
  dialog.showOpenDialog(window, options, (fileNames) => resolve(fileNames))
})
