---
title: Compile the stable release from source
---


Note: At the time of writing, the current release is `v1.13.1`. You can check at
https://github.com/dannyedel/dspdfviewer/releases
if a newer version was released since then.
	Please adapt the version numbers as needed.

{% highlight bash %}
wget https://github.com/dannyedel/dspdfviewer/archive/v1.13.1.tar.gz
wget https://github.com/dannyedel/dspdfviewer/releases/download/v1.13.1/dspdfviewer-1.13.1.tar.gz.asc
gpgv dspdfviewer-1.13.1.tar.gz.asc v1.13.1.tar.gz # verify archive integrity
cd dspdfviewer-1.13.1
mkdir build
cd build
cmake .. # You can pass options here
make
# You can now call the software as ./dspdfviewer
make install # optional, requires root privileges
{% endhighlight %}

Note that there is no uninstall script, so please consider replacing the final
`make install` with something more appropriate for your distribution.
It will respect the usual DESTDIR parameter.
