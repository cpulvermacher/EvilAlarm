#!/bin/sh

#workaround for weird race condition that would lock up evilalarm when calling pasr

if [ $# -ne 1 ]
then
	echo "Usage: $0 max_volume"
fi

while true;
do
	ps | grep evilalarm
	if [ $? -ne 0 ]
	then
		#no evilalarm process found, abort
		exit
	fi

	#reset profile
	dbus-send --type=method_call --dest=com.nokia.profiled /com/nokia/profiled com.nokia.profiled.set_profile string:"general"

	#reset volume to maximum
	pasr -u -s sink-input-by-media-role:x-maemo -l $1

	sleep 5
done
