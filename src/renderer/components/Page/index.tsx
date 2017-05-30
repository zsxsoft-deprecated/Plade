import * as React from 'react'
import { observer, inject } from 'mobx-react'

import AppState from '../../stores/GlobalState'
import { PageProps } from './Page.d'

@inject('score') @observer
export default class Page<T> extends React.PureComponent<PageProps, T> {
  store: AppState

  constructor (props: any) {
    super(props)
    this.store = this.props.store
  }

  render () {
    return <div />
  }
}
