#
# Regular cron jobs for the dspdfviewer package
#
0 4	* * *	root	[ -x /usr/bin/dspdfviewer_maintenance ] && /usr/bin/dspdfviewer_maintenance
