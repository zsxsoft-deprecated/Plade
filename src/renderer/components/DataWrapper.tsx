import * as React from 'react'
import { inject, observer } from 'mobx-react'
import AppState from '../stores/GlobalState'

export default function DataWrapper (Component: any): any {
  @inject('store') @observer
    class DataFetcher extends React.Component<any, any> {
    store: AppState

    constructor (props: any) {
      super(props)
      this.store = this.props.store
    }

    componentDidMount () {
      console.log(this.props)
    }

    // tslint:disable-next-line:no-empty
    componentWillUnmount () {}

    render () {
      return <Component {...this.props} />
    }

  }
  return DataFetcher
}
