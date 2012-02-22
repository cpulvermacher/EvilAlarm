#!/bin/sh
#reset volume and profile regularly, will be restored on exit

#workaround for weird race condition that would lock up evilalarm when calling pasr
#seems to occur only while playing audio and shortly afterwards; results in 100% CPU usage, clone() syscall that it should be doing at this point never appears in strace output
#somehow looks similar to this: http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=575534

pgrep -f evilalarm | grep $PPID > /dev/null
if [ $? -ne 0 ]
then
	echo "$0 needs to be started by evilalarm"
	exit 1
fi

cleanup()
{
	#restore profile
	echo "restoring profile: $OLD_PROFILE"
	dbus-send --type=method_call --dest=com.nokia.profiled /com/nokia/profiled com.nokia.profiled.set_profile string:"$OLD_PROFILE"

	#restore volume
	if [ -f /tmp/evilalarm_sinkstate.backup ]
	then
		echo "restoring volume"
		pasr --restore < /tmp/evilalarm_sinkstate.backup
		rm /tmp/evilalarm_sinkstate.backup
	fi
}
trap "cleanup" 1 2 15

#stop media player to stop it from interfering with audio playback (pause isn't enough)
dbus-send --dest=com.nokia.mafw.renderer.Mafw-Gst-Renderer-Plugin.gstrenderer /com/nokia/mafw/renderer/gstrenderer com.nokia.mafw.renderer.stop


#save current volume
pasr --store > /tmp/evilalarm_sinkstate.backup

#save old profile
OLD_PROFILE=`dbus-send --print-reply=literal --type=method_call --dest=com.nokia.profiled /com/nokia/profiled com.nokia.profiled.get_profile`
OLD_PROFILE=`echo $OLD_PROFILE` #trim leading whitespace

while true;
do
	#check if parent is still alive
	kill -0 $PPID > /dev/null 2>&1
	if [ $? -ne 0 ]
	then
		cleanup
		exit 0
	fi

	#reset profile
	dbus-send --type=method_call --dest=com.nokia.profiled /com/nokia/profiled com.nokia.profiled.set_profile string:"general"

	#reset volume to maximum
	pasr -u -s sink-input-by-media-role:x-maemo -l 100

	sleep 5 &
	wait $! #done to make traps work
	if [ $? -ne 0 ]
	then
		cleanup
		exit 1
	fi
done
