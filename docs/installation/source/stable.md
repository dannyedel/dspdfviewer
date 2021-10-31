---
title: Compile the stable release from source
---


Note: Please take a look at the [github releases][rel] page to get an
overview of the released versions and how they changed.

You can also jump directly to the [latest version][latest].

[rel]: https://github.com/dannyedel/dspdfviewer/releases
[latest]: https://github.com/dannyedel/dspdfviewer/releases/latest

Info:  When compiling software yourself, you have the opportunity to
change certain aspects.
Please read the [compile-time options list][opts] if you want to
fine-tune certain things.

[opts]: /installation/source/options.html

{% highlight bash %}
VERSION=1.33.7 # Change this!
wget https://github.com/dannyedel/dspdfviewer/archive/v$VERSION.tar.gz
wget https://github.com/dannyedel/dspdfviewer/releases/download/v$VERSION/dspdfviewer-$VERSION.tar.gz.asc
gpgv dspdfviewer-$VERSION.tar.gz.asc v$VERSION.tar.gz # verify archive integrity
cd dspdfviewer-$VERSION
mkdir build
cd build
cmake .. # You could pass options here (see above)
make

#
# You can now use the software as ./dspdfviewer
#

make install # optional, requires root privileges
{% endhighlight %}

Note that there is no uninstall script, so please consider replacing the final
`make install` with something more appropriate for your distribution.
It will respect the usual DESTDIR parameter.

If you are willing to write and maintain a dspdfviewer installation
script/package for a specific distribution, please get in touch.
