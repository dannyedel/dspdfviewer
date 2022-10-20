dspdfviewer changelog
=====================

This file aims to record all user relevant changes to dspdfviewer.

If you're intrested in the specific source-level changes between versions,
or non user-relevant changes (like build system or packaging related), please
inspect the output of commands like `git diff -w v1.11..v1.12` directly.

v1.15.2 - UNRELEASED
--------------------

This is a maintenance release, not introducing any new functionality,
but restoring compatibility with Qt 5.15 LTS.

Building with Qt4 is no longer supported.  If you really need Qt4
support, please get in touch.


v1.15.1 - 2016-09-13
--------------------

Bugfix:

* No longer advances two slides on mouse wheel when
  used with recent Qt5.  Thanks to @nbecker for reporting
  this on GitHub.


v1.15 - 2016-04-05
------------------

Changes in behaviour:

* When compiling dspdfviewer yourself:
  * You must now execute the testsuite within a running X Server
    (Linux) or within a graphical environment (Windows/OSX).
    It will no longer try to set up one on its own.
  * The testsuite expects two screens to be connected by default.
    If your environment only has one screen, you can pass
    -DRunDualScreenTests=OFF at CMake time to avoid a test failure.
  * Instead of offering pre-rendered PDFs for download, they are now
    included in the source tree.  The option `DownloadTestPDF` has
    been replaced with `UsePrerenderedPDF` accordingly.
  * Qt5 is now the default.  If you want to build against Qt4,
    you will have to pass -DUseQtFive=OFF to CMake.
    Note that Qt4 support is now considered deprecated and will be
    removed in one of the next versions.  Please file a bug if your
    system does not work correctly with Qt5.

New features:

* Predictable memory usage and configurable cache size limit
  * Previously, dspdfviewer allowed to cache 100 images.  It did not
    matter whether these where thumbnails or full pages, resulting in
    unpredictable memory usage.
  * Now you can specify memory limit for the cache in MiB, using
    the command-line parameter --cache-size.  The default value 1024
    is normally fine, setting is below 250 will result in severe
    performance crippling.
  * This only affects the *cache* memory.  The total memory usage of
    dspdfviewer can be up to 300M higher, mainly depending on whether
    the shared libraries (poppler/qt) are also used by other programs,
    or are specifically loaded for dspdfviewer.

Other:

* A lot of work has gone into the Windows port, and compiling on
  Windows with MSVC is now part of the automated testing.
* The testsuite is currently disabled on big-endian machines.
  It keeps failing with what appears to be an endian issue.
  If you want to help out, you can force the testsuite to run
  by passing -DRunTestsOnBigEndian=ON at cmake time.


v1.14 - 2015-12-01
------------------

New features:

* Translation support
  * For now, a german translation is included, thanks to projekter.
* Experimental i3 support
  * If you specify the --i3-workaround=true option, dspdfviewer will try to
    use i3-cmd to move the audience window one screen to the right.
  * This code is not well tested, please leave feedback and suggestions.
* Add support for Qt5
* Add support for Windows, thanks to projekter.


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
