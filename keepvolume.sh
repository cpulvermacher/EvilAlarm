#!/bin/sh
#workaround for weird race condition that would lock up evilalarm when calling pasr

if [ $# -ne 1 ]
then
	echo "Usage: $0 max_volume"
fi


cleanup()
{
	echo "restoring volume"
	#restore volume
	pasr --restore < /tmp/evilalarm_sinkstate.backup
	rm /tmp/evilalarm_sinkstate.backup
}

#trap SIGHUP, SIGINT & SIGTERM, do nothing. this causes busybox to emit a signal 0 that's not there otherwise!
trap "true" 1 2 15

#so we'll trap that.
trap "cleanup" 0
#no, directly running cleanup() in the first trap doesn't work. 0 needs to be trapped.


#stop media player to stop it from interfering with audio playback (pause isn't enough)
dbus-send --dest=com.nokia.mafw.renderer.Mafw-Gst-Renderer-Plugin.gstrenderer /com/nokia/mafw/renderer/gstrenderer com.nokia.mafw.renderer.stop


#save current volume
pasr --store > /tmp/evilalarm_sinkstate.backup

while true;
do
	#don't keep running when evilalarm dies/is killed
	ps | grep evilalarm | egrep -v 'grep|keep' > /dev/null
	if [ $? -ne 0 ]
	then
		echo "keepvolume.sh: no evilalarm process found!"
		exit 0
	fi


	#reset profile
	dbus-send --type=method_call --dest=com.nokia.profiled /com/nokia/profiled com.nokia.profiled.set_profile string:"general"

	#reset volume to maximum
	pasr -u -s sink-input-by-media-role:x-maemo -l $1

	sleep 5
done
