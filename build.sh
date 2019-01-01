# echo $1
# echo $2

CMD=$1
BUILD=$2
NOCODE=$3

cwd=${PWD##*/}

if [[ $OSTYPE == 'linux-gnu' || $OSTYPE == 'cygwin' ]]; then
	export PLATFORM=linux
	export NAME=$cwd
elif [[ $OSTYPE == 'darwin'* ]]; then
	export PLATFORM=osx
	export NAME=$cwd
elif [[ $OSTYPE == 'msys' || $OSTYPE == 'win32' ]]; then
	export PLATFORM=windows
	export NAME=$cwd.exe
fi

if [[ $NOCODE == 'nocode' ]] ; then
	if [[ $PLATFORM == 'windows' ]]; then
		export PATH="$PATH:/c/mingw32/bin:/c/SFML-2.5.1/bin"
	else
		export PATH="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
	fi
fi

export MAKE_EXEC=make
if [[ $PLATFORM == 'windows' ]]; then
	if [ $(type -P "mingw32-make.exe") ]; then
		export MAKE_EXEC=mingw32-make.exe
	elif [ $(type -P "make.exe") ]; then
		export MAKE_EXEC=make.exe
	fi
fi

if [[ $BUILD != "Release" || $BUILD != 'Debug' ]]; then
	BUILD=Release
fi

PROF_EXEC=gprof
PROF_ANALYSIS_FILE=profiler_analysis.stats


dec=\=\=\=\=\=\=

build_success() {
	tput setaf 2
	tput bold
	echo $dec Build Succeeded $dec
	tput sgr0
}

build_success_launch() {
	tput setaf 2
	tput bold
	echo $dec Build Succeeded: Launching bin/$BUILD/$NAME $dec
	tput sgr0
}

build_fail() {
	tput setaf 1
	tput bold
	echo $dec Build Failed: Review the compile errors above $dec
	tput sgr0
}

launch() {
	tput setaf 2
	tput bold
	echo $dec Launching bin/$BUILD/$NAME $dec
	tput sgr0
}

launch_prod() {
	tput setaf 2
	tput bold
	echo $dec Launching Production Build: $NAME $dec
	tput sgr0
}

profiler_done() {
	tput setaf 2
	tput bold
	echo $dec Profiler Completed: View $PROF_ANALYSIS_FILE for details $dec
	tput sgr0
}

profiler_error() {
	tput setaf 1
	tput bold
	echo $dec Error: Profiler must be run on Debug build. $dec
	tput sgr0
}

prod_osx() {
	tput setaf 1
	tput bold
	echo $dec Production building is not supported on macOS. Use Xcode to bundle your final build. $dec
	echo $dec See https://www.sfml-dev.org/tutorials/2.5/start-osx.php for more information. $dec
	tput sgr0
}

tput setaf 4
if [[ $CMD == 'buildrun' ]] ; then
	if $MAKE_EXEC BUILD=$BUILD; then
		build_success_launch
		bin/$BUILD/$NAME
	else
		build_fail
	fi

elif [[ $CMD == 'build' ]] ; then
	if $MAKE_EXEC BUILD=$BUILD; then
		build_success
	else
		build_fail
	fi

elif [[ $CMD == 'rebuild' ]] ; then
	if $MAKE_EXEC BUILD=$BUILD rebuild; then
		build_success
	else
		build_fail
	fi

elif [[ $CMD == 'run' ]] ; then
	launch
	bin/$BUILD/$NAME

elif [[ $CMD == 'buildprod' ]] ; then
	if $MAKE_EXEC BUILD=$BUILD buildprod; then
		build_success
	else
		build_fail
	fi


elif [[ $CMD == 'profile' ]] ; then
	if [[ $BUILD == 'Debug' ]] ; then
		if $MAKE_EXEC BUILD=$BUILD; then
			build_success_launch
			tput sgr0
			bin/$BUILD/$NAME
			tput setaf 4
			gprof bin/Debug/$NAME gmon.out > $PROF_ANALYSIS_FILE
			profiler_done
		else
			build_fail
		fi
	else
		profiler_error
	fi

else
	tput setaf 1
	tput bold
	echo $dec Error: Command \"$CMD\" not recognized. $dec
	tput sgr0
fi

tput sgr0