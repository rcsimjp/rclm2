list_items_in_dir() {
    dir=$1

    for t in ${dir}/*
    do
	name=`basename $t`
	case $name in
	    '*')
		    # 
		;;
	    *)  
		echo "    $name: $t"
		;;
	esac
    done
}

## echo message.
## _echo(message, ...)
function _echo {
    echo "rclm2: $*"
}


## Wait few seconds with echo message.
## countdown(second=10, message="")
function countdown {
    local count=$1
    local message=$2

    count=${count:=10}
    while [ $count != 0 ]; do
	count=$(($count - 1))
	printf "rclm2: %3d %s\r" $count "$message"
	sleep 1
    done
    echo
}

## echo message and exit 1.
## die(message, ...)
function die {
    echo "Error: $*"
    echo
    exit 1
}


copy() {
    src=$1
    dist=$2
    echo "	Copying $dist ."
    if [ -d $src ]; then
	cp -r $src $dist
    else
	cp $src $dist
    fi
}

link() {
    src=$1
    dist=$2
    echo "	Sym linking $dist ."
    ln -s $src $dist
}
