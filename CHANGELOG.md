dspdfviewer changelog
=====================

This file aims to record all user relevant changes to dspdfviewer.

If you're intrested in the specific source-level changes between versions,
or non user-relevant changes (like build system or packaging related), please
inspect the output of commands like `git diff -w v1.11..v1.12` directly.



v1.13.1 - 2015-08-06
--------------------

This is a bugfix and maintenance release for v1.13.

Bugfixes:

* Regression: Changing slide while blanked crashed the program

Other:

* A `dspdfviewer.desktop` file has been added, helping desktop enviroments
  to include the program in their menus
* `dspdfviewer --help` informs about the manpage and the F1 help box
* This changelog has been started, listing changes since v1.8.



v1.13 - 2015-07-30
------------------

New features:

* dspdfviewer will open a file chooser dialog when started without a
  command-line file parameter (instead of aborting with error message)
* Hyperlinks between pages are now clickable
* The height of the bottom pane (thumbnails and clocks) on the second
  screen can now be configured



v1.12 - 2015-07-23
------------------

New features:

* Added support for the Logitech R400 presenter remote control.
* Secondary screen can be toggled between notes and main presentation.
* `F1` or `?` key show a quick help box.
* A configuration file at `~/.config/dspdfviewer.ini` can now be
  used in addition to command-line parameters.



v1.11 - 2014-07-12
------------------

New features:

* The window for the presenter now spawns on the primary screen
  (see `man xrandr` on how to define this) which is assumed to be
  the notebook you carry to the presenation site.
* The windows now have different window titles and window roles.
  This enables scripting, especially useful on tiling window managers.
* dspdfviewer will now watch the file on disk and re-read when it changes.
  This is useful if you're using dspdfviewer as a previewer while
  writing a presentation.



v1.10.1 - 2013-10-22
--------------------

This is a bugfix and maintenance release for 1.10.

Bugfixes:

* Regression: Primary and secondary window could not be swapped on Kubuntu 13.10.



1.10 - 2013-03-25
-----------------

Improvement:

* Speed up initial rendering



1.9 - 2013-01-23
----------------

New feature:

* The "go to" command (`G`) now displays the range of valid page numbers



1.8 - 2012-12-11
----------------

Initial *free and open source software* release:

* dspdfviewer is now distributed under the GNU GPL, version 2, or
  (at your option) any later version.
