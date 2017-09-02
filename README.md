# Cardenal Lite™ Desktop

## What is this ?

Cardenal Lite™ Desktop is a free and open source tool to encrypt your personal files in order to keep them safe.

You only will need a secret password to protect your files. We won't save that password. Once the file has been protected only you will have access to unlock your files. Be careful! if you lose your password you won't be able to recover your files.

This program is totally compatible with Cardenal Lite™ Web.

## How it works ?

We use a simple but efficient and secure encryption algorithm based on bitwise XOR logical operations. You can encrypt up than 10GB of data per file.

## Build

### Linux / MingGW:
    # In Debian/Ubuntu based distros:
    $ sudo apt install git g++ libcrypto++-dev

    # In Fedora/RedHat based distros:
    $ sudo dnf install git gcc-c++ cryptopp-devel

Then you download the project using [git](https://git-scm.com/):

    $ git clone https://github.com/Root404EngineeringTeam/Cardenal-Lite-Desktop

Then you can build a console application using:

    $ make console

This should create an executable called **_cardenal_** within the **bin** directory that is inside the **cli** directory.
You can run it with:

    $ ./cardenal --help

Otherwise, if you wan't a GUI application you can build it with:

    $ make gui

Then you can execute the binary called **_cardenal_** generated inside the **bin**
directory that is inside the **gui** directory with two clicks or through the console with:

    $ ./cardenal

## Contribute

[https://github.com/Root404EngineeringTeam/Cardenal-Lite-Desktop](https://github.com/Root404EngineeringTeam/Cardenal-Lite-Desktop)
