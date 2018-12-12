# Jason_Compiler
C语言实现的类ALOGO语言
## 运行方法
### Step 1
新建build文件夹
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
使用Jason编译器编译源文件 sample.jsn, 生成汇编文件sample.asm.
```
./Jason sample
```
### Step 4
编译汇编代码, 生成可执行文件(建议使用DOSBox + masm)
