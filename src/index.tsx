import './renderer/styles/main.scss'
import * as React from 'react'
import { render } from 'react-dom'
import { AppContainer } from 'react-hot-loader'

import App from './renderer/components/App'
import AppState from './renderer/stores/AppState'

const appState = new AppState()
const container = document.getElementById('root')

const renderApp = (Renderer: any) =>
  render(
    <AppContainer>
      <Renderer store={appState} />
    </AppContainer>,
    container
  )

if (process.env.NODE_ENV === 'development' && (module as any).hot) {
  (module as any).hot.accept('./index');
  (module as any).hot.accept(
    './renderer/components/App',
    () => renderApp(require('./renderer/components/App').default)
  )
}

renderApp(App)
