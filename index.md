---
layout: default
---
# DS PDF Viewer

WARNING: This documentation website is a work-in-progress. It's far from done
and most links end in 404.

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

[read the manual online]: manpage.html

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

## How do I contribute?

First, you help a lot if you use the software and report any problems or
missing features in the [issues system].

If you want to help on development, please use github's
amazing [pull request] feature.

Both the [sourcecode] and [this website] are available to view and improve,
and any  greatly appreciated!

[sourcecode]: https://github.com/dannyedel/dspdfviewer
[this website]: https://github.com/dannyedel/dspdfviewer/tree/gh-pages
[pull request]: https://help.github.com/articles/using-pull-requests/

## Who built this program?

Github maintains a [contributors] list for the sourcecode.

[contributors]: https://github.com/dannyedel/dspdfviewer/graphs/contributors

## Contact information

If you have any questions that don't fit the [issues system],
you can contact me ([@dannyedel]) by e-mail at `mail@danny-edel.de`.

Since e-mails on the internet are everything but secure,
please use my [gpg key] to send encrypted.

[@dannyedel]: https://github.com/dannyedel
[gpg key]: https://sks-keyservers.net/pks/lookup?op=get&search=0xF132F84C7183343C
