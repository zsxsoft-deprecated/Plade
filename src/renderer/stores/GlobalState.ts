import { observable, action } from 'mobx'
import autoSave from './autoSave'

export default class GlobalState {
  @observable scanPath: string

  constructor () {
    this.scanPath = ''
    autoSave('globalState', this)
  }

  @action setScanPath (data: string) {
    this.scanPath = data
  }

}
