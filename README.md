# Plade

一个C语言抄袭检测项目（其实是作业）。基于libclang。

暂时先写成这样来交差吧，目前实现功能：
1. 寻找一个文件夹内的所有文件的主执行函数。
1. 用``libclang``解析C代码的AST。
1. 比较俩AST的差别，然后根据阈值判断是否抄袭。

使用说明：https://github.com/zsxsoft/Plade/wiki/%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E

参见：https://github.com/zsxsoft/Plade/wiki/%E5%BD%93%E5%89%8D%E8%AE%BE%E8%AE%A1

其实我觉得它写完就会被太监了……目前仅测试过Windows。

## 编译

### 环境要求

1. Nodejs 7.0+
1. Python 2.7
1. (Windows) Visual Studio 2017 / (*nix) g++ 5.4+ / clang （连gyp配置都还没有写，根本编译不起来的啦。反正需要支持C++17的编译器。）

### 编译方式
1. ``git clone https://github.com/zsxsoft/Plade``
1. ``npm install electron -g``
1. ``npm install``
1. 下载并安装LLVM。
1. Windows用户需要把``x64``的``llvm/lib``下的所有文件复制到``third-party/x64/clang``，把``x86``的``llvm/lib``下的所有文件复制到``third-party/x86/clang``。*nix还没测试，我猜安装libclang就可以啦。
1. ``node-gyp rebuild --target=1.6.8 --arch=x64 --dist-url=https://atom.io/download/electron --debug``

## 运行
1. ``electron .``

## 编译
1. ``npm run build``
1. ``build``
