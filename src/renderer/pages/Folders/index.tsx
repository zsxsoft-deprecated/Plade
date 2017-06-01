import * as React from 'react'
import * as fs from 'fs'
import * as path from 'path'
import { observer, inject } from 'mobx-react'
import { css } from 'office-ui-fabric-react/lib/Utilities'
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button'
import { ProgressIndicator } from 'office-ui-fabric-react/lib/ProgressIndicator'
import Page from '../../components/Page'
import AnalyzeQueue from '../../../shared/analyzeQueue'
import diffQueue from '../../../shared/diffQueue'
import GlobalState, { FileASTStatusEnum } from '../../stores/GlobalState'
import getCacheFileName from '../../../shared/getCacheFileName'

@inject('store') @observer
export default class Folders extends Page<{
  currentSelectedLeftIndex: number,
  maybeThreshold: number,
  mustbeThreshold: number
}> {
  store: GlobalState
  listRef: any
  mouseDownFunctionCache: React.MouseEventHandler<HTMLDivElement>[] = []

  constructor (props: any) {
    super(props)
    this.store = this.props.store
    this.state = {
      currentSelectedLeftIndex: 0,
      maybeThreshold: 0.6,
      mustbeThreshold: 0.3
    }
  }

  componentDidMount () {
    const currentPath = this.store.scanPath
    if (this.store.fileASTStatus.length === 0) {
      const files = fs.readdirSync(currentPath).filter(file => fs.statSync(path.resolve(currentPath, file)).isDirectory())
      const data = files.map((filePath, index) => ({
        path: filePath,
        realPath: path.resolve(currentPath, filePath),
        md5Path: getCacheFileName(path.resolve(currentPath, filePath)),
        size: 0,
        analayzeStatus: FileASTStatusEnum.none,
        diffCount: 0,
        diffScore: new Array(files.length).fill(0),
        maybeItemsCount: 0,
        mustbeItemsCount: 0,
        index
      }))
      this.store.fileASTStatus = data
    }
  }

  startAnalyze = () => {
    AnalyzeQueue.startQueue(
      this.store.fileASTStatus.map((file, index) => [file.realPath, file.md5Path, index]),
    (item: any[], index: number) => {
      this.store.setFileStatusById(item[2], {
        analayzeStatus: FileASTStatusEnum.queuing
      })
    },
    (item: any[], index: number, data: any) => {
      this.store.setFileStatusById(item[2], {
        size: data.size,
        analayzeStatus: FileASTStatusEnum.succeed,
        diffCount: 0,
        diffScore: new Array(this.store.fileASTStatus.length).fill(0)
      })
    },
    (item: any[], index: number, data: any) => {
      console.error(item, data)
      this.store.setFileStatusById(item[2], {
        size: 0,
        analayzeStatus: FileASTStatusEnum.failed
      })
    })
  }

  startDiff = () => {
    const cacheNameList = this.store.fileASTStatus.map((file, index) => [file.md5Path, index])
    diffQueue.startQueue(
      Object.assign([], cacheNameList).sort((a: any, b: any) => a.md5Path > b.md5Path ? 1 : 0),
      (item: any[], index: number, data: any) => {
        this.store.setFileStatusById(item[1], {
          analayzeStatus: FileASTStatusEnum.queuing
        })
      },
      (item: any[], index: number, data: any) => {
        console.log(data)
        this.store.setFileStatusById(item[1], {
          analayzeStatus: FileASTStatusEnum.succeed
        })
      },
      (item: any[], index: number, data: any) => {
        console.error(item, data)
        this.store.setFileStatusById(item[1], {
          analayzeStatus: FileASTStatusEnum.failed
        })
      },
      (item: any[], fakeIndex: number, message: any) => {
        console.log(message)
        let anotherItem: any = null
        let anotherIndex = -1
        const index = item[1]
        for (let i = 0; i < this.store.fileASTStatus.length; i++) {
          anotherItem = this.store.fileASTStatus[i]
          if (anotherItem.md5Path === message.cacheName) {
            anotherIndex = i
            break
          }
        }
        if (anotherIndex === -1) return
        const updateStatus1 = this.store.fileASTStatus[index].diffScore.slice(0)
        const updateStatus2 = anotherItem.diffScore.slice(0)
        const maybePlus = (message.score > this.state.mustbeThreshold && message.score <= this.state.maybeThreshold ? 1 : 0)
        const mustbePlus = (message.score > 0 && message.score <= this.state.mustbeThreshold ? 1 : 0)
        updateStatus1[anotherIndex] = message.score
        updateStatus2[index] = message.score
        this.store.setFileStatusById(index, {
          diffCount: this.store.fileASTStatus[index].diffCount + 1,
          diffScore: updateStatus1,
          maybeItemsCount: this.store.fileASTStatus[index].maybeItemsCount + maybePlus,
          mustbeItemsCount: this.store.fileASTStatus[index].mustbeItemsCount + mustbePlus
        })
        this.store.setFileStatusById(anotherIndex, {
          diffCount: anotherItem.diffCount + 1,
          diffScore: updateStatus2,
          maybeItemsCount: anotherItem.maybeItemsCount + maybePlus,
          mustbeItemsCount: anotherItem.mustbeItemsCount + mustbePlus
        })
      }
    )
  }

  backToTop = () => {
    this.store.fileASTStatus = []
    this.props.history.replace('/')
  }

  onItemMouseDown = (index: number) => {
    if (!this.mouseDownFunctionCache[index]) {
      const ret: React.MouseEventHandler<HTMLDivElement> = (event) => {
        this.setState({
          currentSelectedLeftIndex: index
        })
      }
      this.mouseDownFunctionCache[index] = ret
    }
    return this.mouseDownFunctionCache[index]
  }
  startCheck = () => {}

  getDiffColor = (data: number) => {
    if (data === 0) return 'green'
    if (data <= this.state.mustbeThreshold) return 'red'
    if (data <= this.state.maybeThreshold) return 'yellow'
    return 'green'
  }

  render () {
    const queueColor = {
      [FileASTStatusEnum.none]: 'black',
      [FileASTStatusEnum.succeed]: 'green',
      [FileASTStatusEnum.failed]: 'red',
      [FileASTStatusEnum.queuing]: 'blue'
    }

    return (
      <div className='ms-Grid'>
        <div className='ms-Grid-row'>
          <div className='ms-Grid-col ms-sm5'>
            <div className='ms-List'>
              {this.store.fileASTStatus.sort((a, b) => b.mustbeItemsCount - a.mustbeItemsCount).map((item, index) => (
                <div key={item.path} data-is-focusable={true}>
                  <div className={ css('ms-ListScrollingExample-itemContent', {
                    'ms-ListScrollingExample-itemContent-even': index % 2 === 0,
                    'ms-ListScrollingExample-itemContent-odd': index % 2 === 1
                  }) }>
                  <div
                    className='ms-ListItem is-selectable'
                    style={{color: queueColor[item.analayzeStatus]}}
                    onMouseDown={this.onItemMouseDown(item.index)}
                    >
                    <div className='ms-font-xl'>{item.path}</div>
                    <div className='ms-ListBasicExample-itemIndex'>
                      {item.analayzeStatus === FileASTStatusEnum.succeed ? <span>
                          <span>分析大小：{Math.round(item.size / 1024)} KiB</span>
                          <span>&nbsp;&nbsp;疑似数目：{item.maybeItemsCount}</span>
                          <span>&nbsp;&nbsp;确认数目：{item.mustbeItemsCount}</span>
                        </span> : null}
                      {item.analayzeStatus === FileASTStatusEnum.queuing ? <span>正在分析</span> : null}
                      {item.analayzeStatus === FileASTStatusEnum.none ? <span>等待分析</span> : null}
                      {item.analayzeStatus === FileASTStatusEnum.failed ? <span>分析失败</span> : null}
                    </div>
                    {item.diffCount === 0 || item.analayzeStatus === FileASTStatusEnum.succeed ? null :
                    <div>
                      <ProgressIndicator
                        description={`分析中：${item.diffCount} / ${item.diffScore.length}`}
                        percentComplete={item.diffCount / item.diffScore.length} />
                    </div>
                    }
                  </div>
                  </div>
                </div>
              ))}
            </div>
          </div>
          <div className='ms-Grid-col ms-sm2' style={{width: '100px'}}>
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
                onClick={this.startDiff}
                style={{marginBottom: '1.5em'}}
              />
              <DefaultButton
                data-automation-id='checkButton'
                text='判断'
                onClick={this.startCheck}
                style={{marginBottom: '1.5em'}}
              />
              <DefaultButton
                data-automation-id='backButton'
                text='返回'
                onClick={this.backToTop}
              />
            </div>
          </div>
          <div className='ms-Grid-col ms-sm5' style={{float: 'right'}}>
            <div className='ms-List'>
              {this.store.fileASTStatus
              .sort((a, b) => a.diffScore[this.state.currentSelectedLeftIndex] - b.diffScore[this.state.currentSelectedLeftIndex])
              .map((item, index) => (
                <div key={item.path} data-is-focusable={true}>
                  <div>
                  <div
                    className='ms-ListItem'
                    style={{color:
                      item.diffScore[this.state.currentSelectedLeftIndex] === 0 ?
                      queueColor[item.analayzeStatus] :
                      this.getDiffColor(item.diffScore[this.state.currentSelectedLeftIndex])
                    }}
                    >
                    <div className='ms-font-xl'>{item.path}</div>
                    <div className='ms-ListBasicExample-itemIndex'>
                      对比结果：{item.diffScore[this.state.currentSelectedLeftIndex]}
                    </div>
                  </div>
                  </div>
                </div>
              ))}
            </div>
          </div>
        </div>
      </div>
    )
  }
}
