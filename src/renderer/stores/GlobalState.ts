import { observable, action } from 'mobx'

export default class AppState {
  @observable scanPath: string

  constructor () {
    this.scanPath = ''
  }

  @action setScanPath (data: string) {
    this.scanPath = data
  }

}
