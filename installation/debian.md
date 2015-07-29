---
title: Installation on debian
layout: default
---

## Installation of "release" binaries

On debian, the easiest way to install is adding a binary repository
(once) and then simply using your normal package manager (apt-get or
any other apt frontend you choose) to install and track upgrades.
The packages are built by [@dannyedel] and signed with his GPG key
[0x7183343C].

[@dannyedel]: https://github.com/dannyedel
[0x7183343C]: https://sks-keyservers.net/pks/lookup?op=get&search=0xF132F84C7183343C

### Initial setup (once)

These initial steps will *require root permissions.* Depending on your system,
either `su -c 'command --arguments'` or `sudo command --arguments`
will run a command as root. An interactive root shell (note that this will change working
directory to root's home) can be opened with `su -` or `sudo -i`.

The main problem is that until you have installed my gpg key, apt will tell you
(and rightfully so), that it cannot verify the integrity of my packages.
This will be remedied *after* you install my keying *and* update package sources.

Apt will ask you if you want to continue without authentication. Right now, you must.

<div class="root">
{% highlight bash %}
# replace YOURDISTRIBUTION with wheezy, jessie or sid, depending on your system.
echo 'deb http://danny-edel.de/deb YOURDISTRIBUTION main' >> /etc/apt/sources.list.d/danny-edel.list
echo 'deb-src http://danny-edel.de/deb YOURDISTRIBUTION main' >> /etc/apt/sources.list.d/danny-edel.list
apt-get update    # note: apt-get will complain about the repository signing key
apt-get install danny-edel-keyring    # note: apt-get will complain again
apt-get update    # Now all is good.
apt-get install dspdfviewer
{% endhighlight %}
</div>

## Installation of daily debs, directly from git
If you want to stay up-to-date from git (for example because you want to help test features
in development), you can install the daily debs that my jenkins generates and signs for you.

Jenkins' packages are signed with gpg key [0x61E9E242].

<div class="root">
{% highlight bash %}
echo 'deb http://jenkins.danny-edel.de/jenkins-deb dspdfviewer-YOURDISTRIBUTION main' >> /etc/apt/sources.list.d/jenkins.list
apt-key adv --keyserver hkp://hkps.pool.sks-keyservers.net --recv-keys 0xC86A1F0861E9E242
apt-get update
apt-get install dspdfviewer
{% endhighlight %}
</div>

[0x61E9E242]: https://sks-keyservers.net/pks/lookup?op=get&search=0xC86A1F0861E9E242

### Upgrading

Since the package repository is set up, your normal `apt-get update && apt-get upgrade` will
include updates to dspdfviewer. This also means that using the (even graphical) apt frontend
of your choosing will see and install updates.

You can swap between the two repositories, but note that since apt doesn't do *down*grades by
default, you will either have to wait for the next relese when swapping "down" from daily
to release, or do an uninstallation followed by a fresh download of dspdfviewer.



## Installation from source

If you prefer to install from source, please execute the following steps.

Install `build-essential`, `git`, `devscripts` and `equivs`.

<div class="root">
{% highlight bash %}
apt-get install build-essential git devscripts equivs
{% endhighlight %}
</div>

As normal user, clone the git repository.

If you want to build from a release tag (instead of current git)
you can checkout that tag.

{% highlight bash %}
git clone git://github.com/dannyedel/dspdfviewer.git
cd dspdfviewer

# optional:
git checkout v1.12
{% endhighlight %}

As root, install build dependencies (You will most likely not have to repeat this,
since build dependencies don't change often)

<div class="root">
{% highlight bash %}
#from within the dspdfviewer directory
mk-build-deps --install --remove
{% endhighlight %}
</div>

As normal user, build your package with debian automation
{% highlight bash %}
debuild -tc -uc -us
{% endhighlight %}

As root, install the freshly-built package

<div class="root">
{% highlight bash %}
debi
{% endhighlight %}
</div>

