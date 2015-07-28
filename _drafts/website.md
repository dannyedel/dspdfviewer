### What is dspdfviewer?
dspdfviewer is a caching full-screen pdf viewer specifically designed for latex-beamer presentations with the `show notes on second screen=right` option.

It will take your double-width PDF file, split it in half, render the left half to the audience beamer and the right half on your laptop screen. In addition, your laptop screen will contain some counters and a wall-clock.

Both screens will be rendered and scaled independent of each other, so you can configure both screens to their native resolution.

Also dspdfviewer will cache rendered pages, allowing you to instantly swap the page.

### How do I install it?

This varies depending on your operating system. Please follow the specific instructions:

* Debian (7 "wheezy", 8 "jessie", "sid")
* Ubuntu (12.04 "precise", 14.04 "trusty", 15.04 "vivid", 15.10 "wily")
* Gentoo
* Mac OS with macports
* Other Posix systems with a C++11 compiler

Windows support coming soon™ (see #38)
