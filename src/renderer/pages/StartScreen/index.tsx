import * as React from 'react'
import { observer } from 'mobx-react'
import { PrimaryButton } from 'office-ui-fabric-react/lib/Button'
import Page from '../../components/Page'

import showOpenDialog from '../../../ipc/showOpenDialog'
import './index.scss'

@observer
export default class StartScreen extends Page {

  constructor (props: any) {
    super(props)
  }

  selectDirectory = () => {
    showOpenDialog({
      properties: ['openDirectory']
    }).then((list: string[]) => {
      if (list !== null) {
        this.store.setScanPath(list[0])
      }
    })
  }

  render () {
    return (
      <div>
        <PrimaryButton
          data-automation-id='selectButton'
          text='请选择文件夹'
          onClick={this.selectDirectory}
        />
      </div>
    )
  }
}
