import * as React from 'react'
import * as fs from 'fs'
import * as path from 'path'
import { observer } from 'mobx-react'
import { css } from 'office-ui-fabric-react/lib/Utilities'
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button'
import { List } from 'office-ui-fabric-react/lib/List'
import Page from '../../components/Page'
import AnalyzeQueue from '../../../shared/analyzeQueue'

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
    const currentPath = this.context.mobxStores.store.scanPath
    const files = fs.readdirSync(currentPath).filter(file => fs.statSync(path.resolve(currentPath, file)).isDirectory())
    this.setState({files})
  }

  startAnalyze = () => {
    const currentPath = this.context.mobxStores.store.scanPath
    AnalyzeQueue.startQueue(this.state.files.map(file => path.resolve(currentPath, file)), (item: string) => {
      console.log(item)
    }, (item: string, message: any) => {
      console.error(item, message)
    })
  }

  backToTop = () => {
    this.props.history.replace('/')
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
      </div>)

    return (
      <div className='ms-Grid'>
        <div className='ms-Grid-row'>
          <div className='ms-Grid-col ms-sm5'>
            <List
              items={this.state.files}
              onRenderCell={cells}
            />
          </div>
          <div className='ms-Grid-col ms-sm2'>
            <div style={{position: 'absolute', marginTop: '100%'}}>
              <PrimaryButton
                data-automation-id='analyzeButton'
                text='分析'
                onClick={this.startAnalyze}
                style={{marginBottom: '1.5em'}}
              />
              <DefaultButton
                data-automation-id='diffButton'
                text='对比'
                onClick={this.startAnalyze}
                style={{marginBottom: '1.5em'}}
              />
              <DefaultButton
                data-automation-id='backButton'
                text='返回'
                onClick={this.backToTop}
              />
            </div>
          </div>
          <div className='ms-Grid-col ms-sm5' />
        </div>
      </div>
    )
  }
}
