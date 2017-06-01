import * as React from 'react'

import './index.scss'

export default class NoEnv extends React.Component<{
  isLLVM: boolean
}, any> {

  constructor (props: any) {
    super(props)
  }

  componentDidMount () {
    const shell = require('electron').shell
    document.querySelectorAll('a[target="_blank"]').forEach(element => {
      element.addEventListener('click', function (this: HTMLLinkElement, e) {
        shell.openExternal(this.href)
        e.preventDefault()
      })
    })
  }

  render () {
    return (
      <div className='error'>
        <div>
          <p>你还没有安装LLVM，本程序无法运行。</p>
          <p>请点击<a href='http://releases.llvm.org/download.html#4.0.0' target='_blank'>http://releases.llvm.org/download.html#4.0.0</a>，下载并安装LLVM。</p>
          <p>然后重启程序。</p>
        </div>
      </div>
    )
  }
}
