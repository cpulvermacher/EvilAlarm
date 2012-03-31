Introduction
============
EvilAlarm is an alarm clock for Maemo which cannot be turned off while asleep.
The alarm behaviour can be configured, multiple modules are available:

Normal (default)
  A well-behaved alarm that has a "Stop" button to turn it off.
Blubbels
  A small game that will momentarily pause the alarm while you play.
  Requires you too reach a configurable number of points to turn it off.
Movement
  Moving the phone will silence the alarm for a short while, the alarm
  will stop a fixed time after it started. The idea is to, e.g. put the phone in
  your pocket while you walk around and make breakfast. Optionally, you can
  also turn off the alarm by plugging in the AC adapter.

* Project homepage: https://github.com/cpulvermacher/EvilAlarm
* Information about UI redesign & screenshots: http://www.pilpi.net/software/evilalarm/


Preventing Cheating
===================

Once the alarm goes off, the program cannot be closed normally and will restart
itself when killed via Maemo's "End current process".  Persistent sleepers may
however find other ways to end the program: If you can type ``killall evilalarm``
while asleep, you should probably check the "Disable multi-tasking" option in
the "Advanced" settings tab.

To prevent you from turning off the phone, you can remove the "Switch off!"
entry from the power button menu. When no alarm is active (or you have the
"Disable device lock" option disabled), you will still be able to turn off your
phone by choosing "Secure device" and pressing the blank space above the key
pad dialog.

Note that the following might break your phone and will require root access.
To remove the "Switch off!" button, edit ``/etc/systemui/systemui.xml``,
comment/remove the lines

::

   <menuitem priority="90" name="powerup_memainmenu_poweroff">
    <return>5</return>
    <po>osso-powerup-shutdown</po>
   </menuitem>

and restart your phone.
