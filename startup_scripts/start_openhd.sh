#!/bin/bash

# This script is expected to be called from /usr/share/xsessions/openhd.desktop
# Note also that default session "openhd" should also be set in /var/lib/AccountsService/users/<user_name>
# It starts QOpenHD on pure X11 instead of desktop environment


DEBUG_MODE=true

# It seems, no need to change resolution for now.
#/usr/bin/xrandr --output HDMI-1 --primary --mode 1920x1080

# Disable screen going blank/off and the screen saver
# https://unix.stackexchange.com/questions/638022/how-to-disable-screen-going-blank-off-in-x11
xset -dpms
xset s off


# Restart QOpenHD in case of failure.
while true
do
	# Run QOpenHD.
	/usr/local/bin/QOpenHD &
	qopenhd_pid=$!

	# Find and maximize QOpenHD window.
	while true
	do 
		qopenhd_window=`xdotool search --pid "$qopenhd_pid" --onlyvisible --all`
		if [ -n "$qopenhd_window" ]
		then
			break
		fi
	done
	xdotool windowsize "$qopenhd_window" 100% 100%

	wait "$qopenhd_pid"

	# Run regular ubuntu shell for debugging.
	if [ "$DEBUG_MODE" = true ]
	then
		export GNOME_SHELL_SESSION_MODE=ubuntu
		export XDG_CURRENT_DESKTOP=ubuntu:GNOME
		export XDG_CONFIG_DIRS=/etc/xdg/xdg-ubuntu:/etc/xdg
		/usr/bin/gnome-session --session=ubuntu
		break
	fi
done

