---
layout: default
---
# DS PDF Viewer

## What is dspdfviewer?
dspdfviewer is a pre-rendering and caching (read: *fast*) full-screen pdf
viewer specifically designed for latex-beamer presentations,
that were created with the `show notes on second screen=right` option.

It will take your double-width PDF file, split it in half, render the
left half to the audience (via a beamer) and the right half - which will
contain the notes you entered in your LaTeX source - on your laptop screen.

In addition, your laptop screen will contain some counters and a wall-clock,
which may help you time your presentation right.

Both screens will be rendered and scaled completely independent of each other,
so you can use two screens with different resolutions and/or aspect ratios.

## How do I use it?

Please read the program's manual page for usage instructions. If you have
dspdfviewer installed, the correct version should be available
as `man dspdfviewer`. In addition, you can [read the manual online].

[read the manual online]: {{site.baseurl}}manpage.html

If the manual doesn't answer your question, feel free to open a ticket in
the [issues system] requesting help.

## How do I install it?

This varies depending on your operating system.
Please follow the specific instructions:

* Linux and other Unixes
  * [Debian](installation/debian.html)
  * [Ubuntu](installation/ubuntu.html)
  * [Gentoo](installation/gentoo.html)
  * [Arch](installation/arch.html)
  * [Generic (from source)](installation/)
* [Mac OS](installation/macos.html)
* [Windows](installation/windows.html)

If your system isn't listed here, please try the Generic instructions. Feel free
to open a ticket in the [issues system] asking for help if something goes wrong.

[issues system]: https://github.com/dannyedel/dspdfviewer/issues

Windows support coming soon™ (see #38)
