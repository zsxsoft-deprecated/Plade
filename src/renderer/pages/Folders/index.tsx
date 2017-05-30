import * as React from 'react'
import * as fs from 'fs'
import * as path from 'path'
import { observer, inject } from 'mobx-react'
import { css } from 'office-ui-fabric-react/lib/Utilities'
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button'
import { List } from 'office-ui-fabric-react/lib/List'
import Page from '../../components/Page'
import AnalyzeQueue from '../../../shared/analyzeQueue'
import GlobalState, { FileASTStatusEnum, IFileASTStatus } from '../../stores/GlobalState'

@inject('store') @observer
export default class Folders extends Page<{}> {
  store: GlobalState
  listRef: any

  constructor (props: any) {
    super(props)
    this.store = this.props.store
  }

  componentWillReact () {
    console.log(this)
  }

  componentDidMount () {
    const currentPath = this.store.scanPath
    const files = fs.readdirSync(currentPath).filter(file => fs.statSync(path.resolve(currentPath, file)).isDirectory()).map(path => ({
      path,
      size: 0,
      status: FileASTStatusEnum.none
    }))
    this.store.fileASTStatus = files
  }

  startAnalyze = () => {
    const currentPath = this.store.scanPath
    AnalyzeQueue.startQueue(this.store.fileASTStatus.map(file => path.resolve(currentPath, file.path)),
    (item: string, index: number, data: any) => {
      this.store.setFileStatusById(index, {
        size: data.size,
        status: FileASTStatusEnum.succeed
      })
      this.listRef.forceUpdate()
    }, (item: string, index: number, message: any) => {
      console.error(item, message)
      this.store.setFileStatusById(index, {
        size: 0,
        status: FileASTStatusEnum.failed
      })
    })
  }

  backToTop = () => {
    this.props.history.replace('/')
  }

  setListRef = (ref: any) => {
    this.listRef = ref
  }

  render () {
    const cells = (item: IFileASTStatus, index: number) => (
      <div className='ms-ListScrollingExample-itemCell' data-is-focusable={true}>
        <div className={ css('ms-ListScrollingExample-itemContent', {
          'ms-ListScrollingExample-itemContent-even': index % 2 === 0,
          'ms-ListScrollingExample-itemContent-odd': index % 2 === 1
        }) }>
        <div
          className='ms-ListBasicExample-itemContent'
          style={{color: item.status === FileASTStatusEnum.succeed ? 'green' : item.status === FileASTStatusEnum.failed ? 'red' : 'auto'}}
          >
          <div className='ms-ListBasicExample-itemName ms-font-xl'>{item.path}</div>
          <div className='ms-ListBasicExample-itemIndex'>分析大小：{Math.floor(item.size / 1024)} KiB</div>
        </div>
        </div>
      </div>)

    return (
      <div className='ms-Grid'>
        <div className='ms-Grid-row'>
          <div className='ms-Grid-col ms-sm5'>
            <List
              items={this.store.fileASTStatus}
              ref={this.setListRef}
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
