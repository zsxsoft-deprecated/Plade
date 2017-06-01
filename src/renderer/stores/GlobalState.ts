import { observable, action } from 'mobx'
import autoSave from './autoSave'

export enum FileASTStatusEnum {
  none = 0,
  succeed = 1,
  failed = 2,
  queuing = 3
}

export interface IFileASTStatus {
  path: string
  realPath: string
  md5Path: string
  size: number
  analayzeStatus: FileASTStatusEnum
  diffCount: number
  diffScore: number[],
  maybeItemsCount: number,
  mustbeItemsCount: number,
  index: number
}

export default class GlobalState {
  @observable scanPath: string = ''
  @observable fileASTStatus: IFileASTStatus[] = observable([])

  constructor () {
    autoSave('globalState', this)
  }

  @action setScanPath (data: string) {
    this.scanPath = data
  }

  @action setFileStatusById (id: number, data: IFileASTStatus | any) {
    // this.fileASTStatus[id] = Object.assign({}, this.fileASTStatus[id], data)
    Object.keys(data).forEach(key => {
      (this.fileASTStatus[id] as any)[key] = data[key]
    })
  }

}
