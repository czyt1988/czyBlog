# 前言

对于许多脚本都希望有多个参数的输入，对于`shell`这种脚本更是不例外，一般程序的起始函数(`main`)都会有一个args选项代表着输入的参数项，那么解析这些参数项目就是主要的问题，这里有两种解析方法解析脚本输入的参数：
- 一个是通过逻辑遍历所有输入参数(主要利用'shift'函数或者shell对数组的处理)
- 一个是利用系统自带的函数(`getopt`)配合`shift`进行格式化解析(不一定所有系统都，需要检测一下)

# 使用·shift解析脚本的参数输入

在shell脚本中，命令`$`后加一些特殊的标记可以取脚本的传入参数，如`$0`取脚本输入的第一个参数，`$1`取第二个参数等：

| 参数 | 描述|
| :---- | :------: |
| `$0` | 命令本身，类似c的`argv[0]` |
|`$1`、`$2`……|第1个参数、第2个参数……|
|`$#`|参数的个数，不包括`$0`|
|`$@`|以列表的形式返回参数列表，不包括`$0`|
|`$?`|最后运行的命令结束代码|

在shell中`$1`到`$9`相当于一个双向队列，脚本的参数按顺序排入1~9这些变量中，若脚本参数多于9个，就需要通过shift函数，让第一个参数出队，队列中顺序左移，第10个参数入队到`$9`中，因此`shift`可以处理脚本超过10个参数的情况，但更多的是用来解析脚本多参数

`shift`可以把参数列表左移1个，同时参数个数会减少1，`shift 2`可以把参数列表左移2个，同时参数个数会减少2
**但有个情况：若只有2个参数，运行`shift 3` 函数会运行出错返回1，可以通过`$?`检查是否执行成功 **
xxx.sh:
```shell
shift 3;
echo "$?"
echo "$#"
```
执行
```shell
xxx.sh -1 -2
```
输出
```
1
2
```
可以看到，`shift`并没有执行，返回1，参数并没有移动。
因此，在处理脚本参数的时候，必要情况需要判断`shift`是否成功。当然`getopt`函数可以很大程度的避免这些判断逻辑。
这里主要运用`$#`和`$@`这两个变量即可遍历所有的传入参数，在不使用`getopt`这个函数的情况下，要遍历所有的输入参数可以参考如下脚本：

```shell
until [ $# -eq 0 ]
do
	case "$1" in
		-h|--help) echo "-h or --help";
		exit 0;;
		-a|--along) echo "-a or --along";shift;echo "after shift $#";;
		-b|--blong) echo "-b or --blong";
			case "$2" in
				bval1) echo "  b value 1";shift 2;echo "after shift $#";;
				bval2) echo "  b value 2";shift 2;echo "after shift $#";;
				bval3) echo "  b value 3";shift 2;echo "after shift $#";;
				bval4) echo "  b value 4";shift 2;echo "after shift $#";;
#不能判断-b后面是否会有别的参数，因此不能直接shift 2
				*) shift; echo "  unknow b value";
					if [ $# -eq 0 ];then
						exit 0;
					fi
					shift;;
			esac;;
		-c|--clong) echo "-c or --clong";
			case "$2" in
				cval1) echo "  c value 1";shift 2;;
				cval2) echo "  c value 2";shift 2;;
				cval3) echo "  c value 3";shift 2;;
				cval4) echo "  c value 4";shift 2;;
				*) shift; echo "  unknow c value";
					if [ $# -eq 0 ];then
						exit 0;
					fi
					shift;;
			esac;;
		*) echo " unknow prop $1";shift;;
	esac
done
```
上面，在case 判断-b 后面的值时，再不确定-b后面一定有值，不能直接`shift 2`而是需要先进行一次判读
但按照Linus的风格，对于一多个单一属性可以连起来写，如对于`xxx.sh -a -b -c`可以写成`xxx.sh -abc`
若按照上面这种写法，要处理还得写好几个case，因此轮到重要的一个函数`getopt`登场

# 使用`getopt`辅助参数的解析

`getopt`不是标准的unix命令，但它在大多数的发行版中都会带有，`getopt`虽然是带个get但此函数其实主要不是获取而是规范

`getopt`后面接受`-o`选项表示程序可接受的短选项 如`-o ab:c::`，表示程序参数后面可接受的选项为-a -b -c 其中-a后面不接受参数，-b后面必须接受参数(:)，-c后面参数可选(::)
`getopt`后面接受`--long`选项表示程序可接受的短选项 如`--long along,blong:,clong::`，长选项用逗号分隔开，后面接参数的标记号和短选项一致
`getopt`后面 `-n`选项为解析错误时提示的脚本名字

在对参数进行解析前先通过getopt进行解析
```shell
ARGS=`getopt -o ab:c: --long along,blong:,clong: -- "$@"`
#判断是否执行成功，没执行成功退出
if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi
#重新设置参数
eval set -- "$ARGS"
```
通过`getopt `重新解析过的参数会更好解析，如对`-a -b -c`合起来写成`-abc`也可以解析，如下面xxx.sh
```shell
ARGS=`getopt -o abc -- "$@"`
if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi
eval set -- "$ARGS"
while true;do
	case "$1" in
		-a)
			echo "a"
			shift;;
		-b) 
			echo "b"
			shift;;
		-c) 
			echo "c"
			shift;;
		--)
			echo "--"
			shift
			break
			;;
		*) 
			echo "??"
			shift
			;;
	esac
done
```
执行:
```shell
xxx.sh -abc
```
输出：
```
a
b
c
--
```
输入`xxx.sh -cab`或`xxx.sh -bac`都可以，若不使用getopt就需要借助正则来判断这种多样化的组合了

使用`getopt`还有个好处，可以检测一些异常输入，如上例子输入`xxx.sh -g`会输出：
```
getopt：无效选项 -- g
```
一个完整的例子`example.sh`：
```shell
fun_do_a_property()
{
	echo "a($1)"
}

fun_do_b_property()
{
	echo "b($1)"
}

ARGS=`getopt -o hva:b: --long help,version,along:,blong: -- "$@"`
if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi
eval set -- "$ARGS"
while true;do
	case "$1" in
		-a|--along)
			echo "-a | --along"
			fun_do_a_property $2
			shift 2
			;;
		-b|--blong)
			echo "-b | --blong"
			fun_do_b_property $2
			shift 2
			;;
		-v|--version)
			echo "-v | --version"
			shift
			;;
		-h|--help)
			echo "-h | --help"
			shift
			;;
		--)
			shift
			break
			;;
		*) 
			echo "未知的属性:{$1}"
			exit 1
			;;
	esac
done
```
执行`example.sh -a 123 -b 456`
```
-a | --along
a(123)
-b | --blong
b(456)
```