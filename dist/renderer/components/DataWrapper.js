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
function DataWrapper(Component) {
    let DataFetcher = class DataFetcher extends React.Component {
        constructor(props) {
            super(props);
            this.store = this.props.store;
        }
        componentDidMount() {
            console.log(this.props);
            //      let pathname = this.props.match.url
            //      let id = this.props.match.id ? this.props.match.id : null
        }
        // tslint:disable-next-line:no-empty
        componentWillUnmount() { }
        render() {
            return React.createElement(Component, Object.assign({}, this.props));
        }
    };
    DataFetcher = __decorate([
        mobx_react_1.inject('store'), mobx_react_1.observer,
        __metadata("design:paramtypes", [Object])
    ], DataFetcher);
    return DataFetcher;
}
exports.default = DataWrapper;
//# sourceMappingURL=DataWrapper.js.map