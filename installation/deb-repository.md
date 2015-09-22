---
title: Installation on Debian wheezy release
layout: default
---

[Go back to debian installation](/installation/debian.html)

## Wheezy is now oldstable!

If you are using Debian wheezy, please consider that it is now
`oldstable` and will therefore stop receiving security updates soon.

Please consider [upgrading] to the current stable release `jessie`,
especially on a desktop system.

[upgrading]: https://www.debian.org/releases/jessie/amd64/release-notes/ch-upgrading.en.html

## Repository information

That said, I (Danny Edel / mail@danny-edel.de) maintain a
third-party repository to install `dspdfviewer` on wheezy.

Although this not required, you are encouraged to send an e-mail to me
if you use the repository, to show there is an active userbase; If there
is no interest I will most likely shut down the repository and maintain
only the official Debian packages in jessie-backports, stretch and sid.

## Repository installation

Since this is a third-party repository, please contact me directly with
any issues and *do not* report them to the Debian Bug Tracking System.
The packages are built by me ([@dannyedel]) and signed with my GPG key
[0x7183343C].
Again, please consider upgrading to `jessie` and using the official
`jessie-backports` repository instead.

[@dannyedel]: https://github.com/dannyedel
[0x7183343C]: https://sks-keyservers.net/pks/lookup?op=get&search=0xF132F84C7183343C

These initial steps will *require root permissions.* Depending on your system,
either `su -c 'command --arguments'` or `sudo command --arguments`
will run a command as root. An interactive root shell (note that this will change working
directory to root's home) can be opened with `su -` or `sudo -i`.

The main problem is that until you have installed my gpg key, apt will tell you
(and rightfully so), that it cannot verify the integrity of my packages.
This will be remedied *after* you install my keying *and* update package sources.

<div class="root">
{% highlight bash %}
# Tell apt about the repository
echo 'deb http://danny-edel.de/deb wheezy main' >> /etc/apt/sources.list.d/danny-edel.list
echo 'deb-src http://danny-edel.de/deb wheezy main' >> /etc/apt/sources.list.d/danny-edel.list

# Make apt read the package list
apt-get update    # note: apt-get will complain about the repository signing key

# Make apt install the keyring
apt-get install danny-edel-keyring    # note: apt-get will complain that it can't verify the package

# Make it re-read the package list with the key now known and install dspdfviewer
apt-get update
apt-get install dspdfviewer
{% endhighlight %}
</div>

<span class="root">
From now on, you will receive updates to dspdfviewer with your regular
`apt-get update && apt-get upgrade` routine.
