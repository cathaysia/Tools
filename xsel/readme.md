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