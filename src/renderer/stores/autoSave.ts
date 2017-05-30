import { autorun, extendObservable, toJS } from 'mobx'

export default (instanceStorageId: string, instance: any) => {
  let firstRun = true

  autorun(() => {
    if (firstRun) {
      const existingStore = localStorage.getItem(instanceStorageId)
      if (existingStore) {
        extendObservable(instance, JSON.parse(existingStore))
      }
    }
    const data = toJS(instance)
    localStorage.setItem(instanceStorageId, JSON.stringify(data))
  })

  firstRun = false
}
