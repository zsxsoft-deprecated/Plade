"use strict";
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __metadata = (this && this.__metadata) || function (k, v) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
};
Object.defineProperty(exports, "__esModule", { value: true });
const React = require("react");
const mobx_react_1 = require("mobx-react");
const Button_1 = require("office-ui-fabric-react/lib/Button");
const Page_1 = require("../../components/Page");
const showOpenDialog_1 = require("../../../ipc/showOpenDialog");
let StartScreen = class StartScreen extends Page_1.default {
    constructor(props) {
        super(props);
        this.selectDirectory = () => {
            showOpenDialog_1.default({
                properties: ['openDirectory']
            }).then((list) => {
                console.log(list);
            });
        };
    }
    render() {
        return (React.createElement("div", null,
            React.createElement(Button_1.PrimaryButton, { "data-automation-id": 'test', text: '请选择文件夹', onClick: this.selectDirectory })));
    }
};
StartScreen = __decorate([
    mobx_react_1.observer,
    __metadata("design:paramtypes", [Object])
], StartScreen);
exports.default = StartScreen;
//# sourceMappingURL=index.js.map