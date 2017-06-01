import './renderer/styles/main.scss'
import * as React from 'react'
import { render } from 'react-dom'
import { AppContainer } from 'react-hot-loader'
import * as cp from 'child_process'

import App from './renderer/components/App'
import NoEnv from './renderer/pages/NoEnv'
import GlobalState from './renderer/stores/GlobalState'

const globalState = new GlobalState()
const container = document.getElementById('root')

cp.exec('clang -v', (err, stdout, stderr) => {
  console.log(stderr)
  const renderApp = (Renderer: any) =>
    render(
      <AppContainer>
        {err ?
          <NoEnv isLLVM={true} />
        : <Renderer store={globalState} /> }
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
})

