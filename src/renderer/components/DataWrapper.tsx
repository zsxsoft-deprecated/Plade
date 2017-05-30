import * as React from 'react'
import { inject, observer } from 'mobx-react'
import { Redirect } from 'react-router-dom'
import AppState from '../stores/AppState'

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
//      let pathname = this.props.match.url
//      let id = this.props.match.id ? this.props.match.id : null
    }

    // tslint:disable-next-line:no-empty
    componentWillUnmount () {}

    render () {
      return <Component {...this.props} />
    }

  }
  return DataFetcher
}
