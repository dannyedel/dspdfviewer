Gentoo
======

Install from ebuild
-------------------

Two kinds of ebuilds are available for dspdfviewer: the git ebuild
(with version number 9999), which downloads and installs the latest
git version of dspdfviewer, and regular ebuilds with normal version
numbers.

To install, move the ebuilds in the "gentoo" subdirectory into an
overlay (for more information on overlays, see [the gentoo manual][1])
in the "app-text/dspdfviewer" directory. Then, unmask the version you
want and run "emerge dspdfviewer".

[1]: https://www.gentoo.org/proj/en/overlays/userguide.xml
