import { observable } from 'mobx'

export default class AppState {
  @observable scanPath: string

  constructor () {
    this.scanPath = ''
  }
/*
  @action setData(data) {
    this.items = data
  }

  @action setSingle(data) {
    this.item = data
  }

  @action clearItems() {
    this.items = []
    this.item = {}
  }
*/

}
