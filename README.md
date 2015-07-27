#  Dual-Screen PDF Viewer for latex-beamer

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

## Installation from binaries
### Debian wheezy/jessie/sid
If you are using Debian wheezy, jessie or sid, you should be able to
get a binary from my repository:

http://danny-edel.de/deb/

If you open the link with a web browser, there are some copy-and-paste
instructions on how to add the repository. These packages are built by myself.

### Ubuntu 12.04 LTS (Precise Pangolin) or newer

For Ubuntu, there are two PPAs configured:
A "daily" repository and a "release" repository. Use the daily builds only
if you want to help in development (test bugs in the latest git revision),
otherwise stick to the "release" PPA:

1. Daily
   https://launchpad.net/~dannyedel/+archive/dspdfviewer-daily
2. Release
   https://launchpad.net/~dannyedel/+archive/dspdfviewer

### Arch Linux
In the [AUR](https://wiki.archlinux.org/index.php/Arch_User_Repository) there is
a PKGBUILD for installing dspdfviewer from
[latest release](https://aur.archlinux.org/packages/dspdfviewer/) or from
[git](https://aur.archlinux.org/packages/dspdfviewer-git/).


## Installing from source

### Debian-based systems
You need the "devscripts" debian package installed to build from source.

Note: Depending on your system, you must either use
`su -c 'command --arguments'` or `sudo command --arguments`
to run a command as root.

1. (once)
   `git clone git://github.com/dannyedel/dspdfviewer.git`
2. (update)
   `cd dspdfviewer; git pull`
3. (install build dependencies, **as root**)
   `mk-build-deps --install --remove`
4. (build)
   `debuild -tc`
5. (install, **as root**)
   `debi`

### Other systems
You will need a C++11 compiler and the cmake build system.

1. (once)
   git clone git://github.com/dannyedel/dspdfviewer.git
2. (update)
   cd dspdfviewer; git pull
3. (make a build directory)
   mkdir /path/to/builddir ; cd /path/to/builddir
4. (create Makefile)
   cmake /path/to/sourcedir
   (*note: this step should tell you about missing build-deps*)
5. (build)
   make
6. (install)
   make install
   (optional)

### MacPorts
If you use a mac with macports, it is very easy to set up a qt4 environment

1. install macports
2. port install poppler +qt4+quartz
3. port install cmake

then follow the normal build instructions
