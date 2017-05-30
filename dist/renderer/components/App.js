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
const react_router_dom_1 = require("react-router-dom");
const mobx_react_1 = require("mobx-react");
const mobx_react_devtools_1 = require("mobx-react-devtools");
const Fabric_1 = require("office-ui-fabric-react/lib/Fabric");
const StartScreen_1 = require("../pages/StartScreen");
let App = class App extends React.Component {
    constructor(props) {
        super(props);
        this.store = this.props.store;
    }
    render() {
        return (React.createElement(react_router_dom_1.HashRouter, null,
            React.createElement(mobx_react_1.Provider, { store: this.store },
                React.createElement(Fabric_1.Fabric, null,
                    React.createElement("div", { className: 'wrapper' },
                        React.createElement(react_router_dom_1.Route, { exact: true, path: '/', component: StartScreen_1.default })),
                    process.env.NODE_ENV === 'development' ? React.createElement(mobx_react_devtools_1.default, null) : null))));
    }
};
App = __decorate([
    mobx_react_1.observer,
    __metadata("design:paramtypes", [Object])
], App);
exports.default = App;
//# sourceMappingURL=App.js.map