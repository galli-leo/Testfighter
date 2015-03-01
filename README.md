Testfighter
============

[![Build Status](https://travis-ci.org/DJLectr0/Testfighter.svg)](https://travis-ci.org/DJLectr0/BetaLauncher)

This is a rudementary implementation of Testflight on desktop.

Features
--------

#####What is currently working:

- Easy installation without needing to compile anything
- You can download apps
- They can be launched
- Updating works but after every restart it forgets you updated (This needs fixing)
- Uploading works (But on yosemite dropping does not work)
- Populating the news section using an rss feed with e.g. new updates and initial releases
- CLI Interface for uploader

#####What is planned:

- Github script for UE4 games
- New UI for both client and server
- Login or beta key requirement
- Stats
- Maybe iOS and Android compability

Installation
------------
#####Server
1. Go to the server folder in a terminal and then do `sh remove_placeholders.sh` (Mac only, may work on linux)
1. Upload the server folder to a VPS or a Webserver which has PHP installed and name it whatever you like.
2. Open the upload_single.php
3. Edit the line: `if($_POST["password"]=="testfighter2015")` and change 'testfighter2015' to a password of your choice
4. Finished!

#####Windows Clients
1. Unzip the Windows release
2. Go to the folder of the exe and open the settings.json
3. Edit the url entry to point to: "http://yourdomain.com/path/to/the/server/folder" (for the Uploader client also change the password you set in the server)
4. Rezip it and destribute!

#####Mac Clients
1. Right click on the .app and choose open Package contents
2. Go to Contents/MacOS and open the settings.json
3. Edit the url entry to point to: "http://yourdomain.com/path/to/the/server/folder" (for the Uploader client also change the password you set in the server)
4. Distribute!
