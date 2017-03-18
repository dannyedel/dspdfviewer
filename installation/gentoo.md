---
title: Installation on Gentoo
layout: default
---

Gentoo
======

Install from ebuild
-------------------

Two kinds of ebuilds are available for dspdfviewer: the git ebuild
(with version number 9999), which downloads and installs the latest
git version of dspdfviewer, and regular ebuilds with normal version
numbers.

To install, move the ebuilds in the ["gentoo" subdirectory][0] into an
overlay (for more information on overlays, see [the gentoo manual][1])
in the "app-text/dspdfviewer" directory. Then, unmask the version you
want and run "emerge dspdfviewer".

[0]: https://github.com/dannyedel/dspdfviewer/tree/master/gentoo
[1]: https://www.gentoo.org/proj/en/overlays/userguide.xml

-----

The inclusion request for the main Gentoo repository is filed under
[bug 520120 in the Gentoo bugzilla][gentoobug].

[gentoobug]: https://bugs.gentoo.org/show_bug.cgi?id=520120
