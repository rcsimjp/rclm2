# By Hedayat Vatankhah <hedayat@grad.com>, 2008
# Based on 3D local_func.bash

# Servers Settings
SERVER=172.16.7.194
CLIENT1=root@172.16.7.181
CLIENT2=root@172.16.7.182

# Simspark Settings
SERVER_PREFIX_DIR="/usr/local/"
SOCCERSIM_CONF=naosoccersim.rb
TOTAL_GAME_TIME=600

ARCHIVE_DIR=archives
RESULTPRINTER="${PREFIX}/bin/rclmresultprinter.3d"
RESULTFILE=./var/results
HALF_TIME=$((TOTAL_GAME_TIME/2))

# load user setting.
#[ -f ${RCLM_USER_DIR}/3DSpark ] &&  source ${RCLM_USER_DIR}/3DSpark ]
test  -f ./var/3DSpark && source ./var/3DSpark

function _countdown {
    countdown $1 $2
}

## __before_match(team1, team2)
function __before_match {
    rm -f sparkmonitor.log
    echo 0 > gametime
    rm -f scoresswitched
    cp $SERVER_PREFIX_DIR/share/rcssserver3d/$SOCCERSIM_CONF .
    return 0
}

## __run_match(team1, team2)
function __run_match {
    local TEAM1=$1
    local TEAM2=$2
    
    echo "$TEAM1 vs $TEAM2"

    NAME1=${TEAM1#*:}
    NAME2=${TEAM2#*:}
    DIR1=${TEAM1%:*}
    DIR2=${TEAM2%:*}

    
    simspark &> console.log &
    PID=$!
    sleep 2
    
    ssh ${CLIENT1} "su - $NAME1 -c './start.sh $SERVER &> play.log'" &
	
    sleep 5
    ssh ${CLIENT2} "su - $NAME2 -c './start.sh $SERVER &> play.log'" &

    wait $PID

    _countdown 5 ': if you want to stop league(CANCEL THIS GAME), press Ctrl+C.'
    return 0
}

## __after_match(team1, team2)
function __after_match {
    local TEAM1=$1
    local TEAM2=$2

    NAME1=${TEAM1#*:}
    NAME2=${TEAM2#*:}
    DIR1=${TEAM1%:*}
    DIR2=${TEAM2%:*}

    ssh ${CLIENT1} "su - $NAME1 -c './kill.sh &> kill.log'" &
	
    sleep 1
    ssh ${CLIENT2} "su - $NAME2 -c './kill.sh &> kill.log'" &

    GAMEDATE=`date +%Y%m%d%H%M`
    [ -d ${ARCHIVE_DIR} ] || mkdir ${ARCHIVE_DIR}
    local GAME=`${RESULTPRINTER} sparkmonitor.log`

    LOGNAME="${GAMEDATE}_${NAME1}_vs_${NAME2}"
    if [ -n "${GAME}" ]; then
	echo $GAME >> ${RESULTFILE}
    	set ${GAME}
	LOGNAME="${GAMEDATE}_$1_$3_vs_$2_$4"
	SCORE_LEFT=$3
	SCORE_RIGHT=$4
    fi

    GAME_TIME=`tail -n1 sparkmonitor.log | sed "s/.*time \([0-9]*\).*/\1/"`

    LOGDIR="archives/${LOGNAME}"
    mkdir $LOGDIR
    mv {sparkmonitor,console}.log $LOGDIR

    # Check to see if the game was complete
#    TOTAL_PLAYED=$((GAME_TIME+`cat gametime`))
    TOTAL_PLAYED=$GAME_TIME
    if [ $TOTAL_PLAYED -lt $TOTAL_GAME_TIME ]; then
	echo $TOTAL_PLAYED > gametime
        echo "Game finished at time(rounded): $GAME_TIME"
	echo "Total played time is: $TOTAL_PLAYED"
	echo "Total game time is: $TOTAL_GAME_TIME"
	echo -n "Run complementary match? [y/N]:"
    	read ans
	case $ans in
	[yY]*)
    		_countdown 15 ": wait complementary match."
#		sed -i "s/addSoccerVar('RuleHalfTime',.*)/addSoccerVar('RuleHalfTime',$TOTAL_GAME_TIME - $TOTAL_PLAYED)/" $SOCCERSIM_CONF
		sed -i "s/setTime(.*)/setTime($TOTAL_PLAYED)/" $SOCCERSIM_CONF
		if [ ! $TOTAL_PLAYED -lt $HALF_TIME ] && [ ! -f scoresswitched ]; then
		   sed -i "s/setScores(.*)/setScores($SCORE_RIGHT,$SCORE_LEFT)/" $SOCCERSIM_CONF
		else
 	           sed -i "s/setScores(.*)/setScores($SCORE_LEFT,$SCORE_RIGHT)/" $SOCCERSIM_CONF
		fi

                if [ $TOTAL_PLAYED -lt $HALF_TIME ] || [ -f scoresswitched ]; then
      		    __run_match $TEAM1 $TEAM2
    		    __after_match $TEAM1 $TEAM2
		else
		   touch scoresswitched
      		    __run_match $TEAM2 $TEAM1
    		    __after_match $TEAM2 $TEAM1
		fi
		return 0
		;;
	esac
    fi

    TIE=""
    if [ -z "${GAME}" ]; then
	echo -n "Tie game? [y/N]:"
    	read ans
	case $ans in
	[yY]*)
		TIE=y
		;;
	esac
    else
    	set ${GAME}
	echo 
    	[ "x$3" = "x$4" ] && TIE=y
    fi
    if [ "x$TIE" = 'xy' ]; then
	echo -n "Tie break, Run match again? [y/N]:"
    	read ans
	case $ans in
	[yY]*)
    		_countdown 15 ": wait extend match."
    		__run_match $TEAM1 $TEAM2
    		__after_match $TEAM1 $TEAM2
		;;
	esac
    fi
    rm gametime
    return 0
}

## __wait_next_match
function __wait_next_match {
    _countdown 15 ': wait next match.' 
}
