import * as React from 'react'
import { HashRouter as Router, Route } from 'react-router-dom'
import { Provider, observer } from 'mobx-react'
import DevTools from 'mobx-react-devtools'

import GlobalState from '../stores/GlobalState'
import { Fabric } from 'office-ui-fabric-react/lib/Fabric'

import StartScreen from '../pages/StartScreen'


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
