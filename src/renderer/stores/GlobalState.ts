import { observable, action } from 'mobx'
import autoSave from './autoSave'

export enum FileASTStatusEnum {
  none = 0,
  succeed = 1,
  failed = 2
}

export interface IFileASTStatus {
  path: string
  size: number,
  status: FileASTStatusEnum
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
