#  Dual-Screen PDF Viewer for latex-beamer

This is a simple viewer for latex-beamer presentations that are built
with the **show notes on second screen** option of latex-beamer.

It will take your PDF file, split it in a left and right half and
render the two halves individually to the screens.

## Installation from binaries
### Debian
If you are using Debian wheezy or sid, you should be able to
get a binary from my repository:
  
http://danny-edel.de/deb/

If you open the link with a web browser, there are some copy-and-paste
instructions on how to add the repository. These packages are built by myself.

### Ubuntu

For Ubuntu Oneiric (11.04) and newer, there are two PPAs configured:
A "daily" repository and a "release" repository. Use the daily builds only 
if you want to help in development (test bugs in the latest git revision),
otherwise stick to the "release" PPA:

1. Daily
   https://launchpad.net/~dannyedel/+archive/dspdfviewer-daily
2. Release
   https://code.launchpad.net/~dannyedel/+archive/dspdfviewer

## Installing from source

### Debian-based systems
You need the "devscripts" debian package installed to build from source.

1. (once)
   git clone git://github.com/dannyedel/dspdfviewer.git
2. (update)
   cd dspdfviewer; git pull
3. (build)
   debuild -tc
4. (install)
   su -c debi # (or sudo debi, if you're ubuntu-ish)


## Usage
There is a manpage included that explains all the options and keybindings. Most important ones are probably:

Command line:

dspdfviewer [options] *pdf-file*

where the most useful option is probably -f, because it enables you to use the software with a standard pdf (i.e. not specifically built for latex-beamer).

In-Program-Controls:

Left/Right mouse button or wheel up/down: Back/Forward

S/F12: Swap screens (in case the timers end up on the beamer)

Q/Esc: Quit