"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
require("./styles/main.scss");
const React = require("react");
const react_dom_1 = require("react-dom");
const react_hot_loader_1 = require("react-hot-loader");
const App_1 = require("./renderer/components/App");
const AppState_1 = require("./renderer/stores/AppState");
const appState = new AppState_1.default();
const container = document.getElementById('root');
const renderApp = (Renderer) => react_dom_1.render(React.createElement(react_hot_loader_1.AppContainer, null,
    React.createElement(Renderer, { store: appState })), container);
if (process.env.NODE_ENV === 'development' && module.hot) {
    module.hot.accept('./index');
    module.hot.accept('./components/App', () => renderApp(require('./components/App').default));
}
renderApp(App_1.default);
//# sourceMappingURL=index.js.map