# Passilic

Passilic is a frontend for the [Pass password manager](https://www.passwordstore.org)
for Sailfish OS.

Pass is a simple Unix password manager where each password is stored GPG encrypted in
a file. Passilic allows users to easily browse and access their passwords. Tapping a
passwords prompts the "master key" (GPG key passphrase) and copies the password into
clipboard. The password is then automatically cleared from the clipboard after 45 seconds
by Passilic.

Passilic is a free and open source project under GPLv3 license.

# Screenshots

![Main screen](https://raw.githubusercontent.com/danvratil/harbour-passilic/master/images/mainscreen.png)
![Password expiration](https://raw.githubusercontent.com/danvratil/harbour-passilic/master/images/mainscreen-expire.png)

# Installation

Latest version of Passilic is available on [OpenRepos](https://openrepos.net/content/dvratil/passilic).

# TODO

* Remember passphrase (gpg-agent integration)
* Generating passwords
* Editing passwords
* Removing passwords
