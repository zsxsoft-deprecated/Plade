import * as React from 'react'
import { HashRouter as Router, Route, RouteComponentProps } from 'react-router-dom'
import { Provider, observer } from 'mobx-react'
import DevTools from 'mobx-react-devtools'

import GlobalState from '../stores/GlobalState'
import { Fabric } from 'office-ui-fabric-react/lib/Fabric'

import StartScreen from '../pages/StartScreen'

function wrapperFunc <Props, State, CompState> (
    WrappedComponent: typeof React.Component
): React.ComponentClass<Props & State> {
  return class extends React.Component<Props & State, CompState> {
    public render () {
      return <WrappedComponent {...this.props} {...this.state} />
    }
  }
}

@observer
export default class App extends React.Component<{ store: GlobalState }, any> {
  store: GlobalState

  constructor (props: any) {
    super(props)
    this.store = this.props.store
  }

  render () {
    return (
      <Router>
        <Provider store={this.store}>
          <Fabric>
            <div className='wrapper'>
              <Route exact={true} path='/' component={StartScreen} />
            </div>
            {process.env.NODE_ENV === 'development' ? <DevTools /> : null}
          </Fabric>
        </Provider>
      </Router>
    )
  }
}
