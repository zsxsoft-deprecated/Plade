import * as React from 'react'
import { observer, inject } from 'mobx-react'

import AppState from '../../stores/AppState'
import { PageProps } from './Page.d'

@inject('score') @observer
export default class Page extends React.PureComponent<PageProps, any> {
  store: AppState

  constructor (props: any) {
    super(props)
    this.store = this.props.store
  }

  render () {
    return <div />
  }
}
