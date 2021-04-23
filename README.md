# Tools
重写了一部分常用的小工具

# 构建

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

现在，编译完成的软件在 `./build/bin` 目录下

# 文档

文档现在先暂时放到 README 下

## xsel
powershell 实际上是有类似 xsel 的命令：

```powershell
Set-Alias -name xsel -value Set-ClipBoard
```

当然，既然有 Set-ClipBoard，自然也有 Get-ClipBoard。
这个命令主要是给 cmd 用的。

相比 linux 下的 xsel 而言，这个只实现了两个功能：

```shell
cat 1.txt | xsel # powershell
type 1.txt | xsel # cmd
xsel -o
```
只有用来读写剪贴板的功能

## xxd

用来显示一个文件的二进制形式及其对应字符串
整个文件会以 ASCII 的形式解析，所有不可见字符均被 FALLBACK 字符替换，这意味着你没办法用这个程序来看 utf-8 文件的 BOM 是多少
而且这个程序的效率比较低，在于其他程序通过管道符使用时延迟比较明显

### 使用

```shell
cat 1.txt | xxd
xxd 1.txt
xxd 1.txt | head -n 20
```

## cppname

类似c++filt的软件

c++filt 已经很好了，可以只能看函数的名字，没法看类型名，对 [getFullName](./include/Utils.h) 包装一下

# 规范

项目遵循

- [Google 设计规范](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/)
- [Modern CMake#
  How to structure your project](https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/chapters/basics/structure.md)
- [Modern CMake 最佳实践#交叉编译](https://www.jianshu.com/p/8abf754654c4)
