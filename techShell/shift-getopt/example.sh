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
