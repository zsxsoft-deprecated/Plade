import * as React from 'react'
import * as fs from 'fs'
import { observer } from 'mobx-react'
import { css } from 'office-ui-fabric-react/lib/Utilities'
import { PrimaryButton } from 'office-ui-fabric-react/lib/Button'
import { List } from 'office-ui-fabric-react/lib/List'
import Page from '../../components/Page'

interface FolderState {
  files: string[]
}

@observer
export default class Folders extends Page<FolderState> {

  constructor (props: any) {
    super(props)
    this.state = {
      files: []
    }
  }

  componentDidMount () {
    fs.readdir(this.context.mobxStores.store.scanPath, (err, files) => {
      if (!err) this.setState({files})
    })
  }

  render () {
    const cells = (item: string, index: number) => (
      <div className='ms-ListScrollingExample-itemCell' data-is-focusable={true}>
        <div className={ css('ms-ListScrollingExample-itemContent', {
          'ms-ListScrollingExample-itemContent-even': index % 2 === 0,
          'ms-ListScrollingExample-itemContent-odd': index % 2 === 1
        }) }>
        {index} &nbsp; {item}
        </div>
      </div>
            )
    return (
      <div>
        <div>
          <List
            items={this.state.files}
            onRenderCell={cells}
          />

        </div>
        <div />
        <div />
      </div>
    )
  }
}
