"use strict";
const wrapper = require('./wrapper');
const { dialog } = require('electron');
module.exports.default = wrapper('showOpenDialog');
module.exports.main = (options) => new Promise((resolve, reject) => {
    dialog.showOpenDialog(options, (fileNames) => resolve(fileNames));
});
//# sourceMappingURL=showOpenDialog.js.map