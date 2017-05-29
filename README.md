# Plade

一个C语言抄袭检测项目。基于libclang。

暂时先写成这样来交差吧，目前实现功能：
1. 寻找一个文件夹内的所有文件的主执行函数。
1. 用``libclang``解析C代码的AST。
1. 用啥算法比较俩AST的差别，然后根据阈值判断是否抄袭。

## 编译

1. 安装``Nodejs``和[node-gyp](https://github.com/nodejs/node-gyp)，参考其安装条件。
1. ``git clone https://github.com/zsxsoft/Plade``
1. ``npm install electron -g``
1. ``npm install``
1. Windows用户需要把``x64``的``llvm/lib``下的所有文件复制到``third-party/x64/clang``，把``x86``的``llvm/lib``下的所有文件复制到``third-party/x86/clang``。*nix还没测试，我猜安装libclang就可以啦。
1. ``node-gyp rebuild --target=1.6.8 --arch=x64 --dist-url=https://atom.io/download/electron --debug``

## 运行
1. ``electron .``