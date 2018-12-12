# Jason_Compiler
一个C语言实现的类ALOGO的Jason(Just Another Sample Original Notion)语言编译器.
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
使用Jason编译器编译源文件 sample.txt, 生成汇编文件sample.asm.
```
./Jason ../sample.txt
```
### Step 4
编译汇编代码, 生成可执行文件(建议使用DOSBox + masm)
