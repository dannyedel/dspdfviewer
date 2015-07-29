#  Dual-Screen PDF Viewer for latex-beamer

[![Build Status](https://travis-ci.org/dannyedel/dspdfviewer.svg?branch=master)](https://travis-ci.org/dannyedel/dspdfviewer)

This is a simple viewer for latex-beamer presentations that are built
with the **show notes on second screen** option of latex-beamer.

It will take your PDF file, split it in a left and right half and
render the two halves individually to the screens.

## Usage
There is a manpage included that explains all the options and keybindings. Most important ones are probably:

Command line:

dspdfviewer [options] *pdf-file*

where the most useful option is probably -f, because it enables you to use the software with a standard pdf (i.e. not specifically built for latex-beamer).

In-Program-Controls:

Left/Right, Mouse Buttons or Mouse Wheel: Back/Forward

S or F12: Swap screens (if you see the wall clock on the projector)

B or . (period): blank/unblank audience screen

Q/Esc: Quit

## Installation

Please read the [installation section] on the project's website for
detailed instructions, broken down by operating system.

For generic from-source installation instructions, you can also consult
the INSTALL file.

[installation section]: http://dspdfviewer.danny-edel.de/#how-do-i-install-it
