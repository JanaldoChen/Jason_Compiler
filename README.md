# Jason_Compiler
一个C语言实现的类ALGOL的Jason(Just Another Sample Original Notion)语言编译器.
## 运行方法
### Step 1
新建build文件夹(如果存在build文件, 可以尝试直接进入build文件目录下进行Step 3, 若不能正常运行, 可以删除build文件夹, 按如下方式重新编译)
```
mkdir build
```
### Step 2
进入build文件目录下, 编译程序生成Jason编译器 Jason.exe.
```
cd build
cmake ..
make
```
### Step 3
使用Jason编译器编译源文件 sample.txt, 生成汇编文件 sample.asm.
#### mac用户
```
./Jason ../sample.txt
```
#### windows用户
```
Jason ../sample.txt
```
### Step 4
编译汇编代码, 生成可执行文件(建议使用[DOSBox](https://www.dosbox.com/download.php?main=1) + masm)
* 回到上一级 Jason/目录下, 将生成的汇编代码 sample.asm 拷贝到 masm/ 目录下
```
cd ..
cp sample.asm masm/sample.asm
```
* 打开DOSBox, 将工作环境映射到 masm/ 目录下(其中****表示存放 Jason/ 的路径)
```
mount c ~/****/Jason/masm
c:
```
* 对汇编代码 sample.asm 进行编译、链接生成 sample.exe 可执行文件
```
masm sample.asm
link sample.obj
```
* 运行程序, 样例代码功能是输入一个正整数n, 输出前n项fibonacci数
```
sample.exe
```
* 运行结果
```
10
1
1
2
3
5
8
13
21
34
55
```
## 操作示例
### Jason_compiler
![Jason](Instructions/Jason.gif)
### DOSBox
![DOSBox](Instructions/DOSBox.gif)
