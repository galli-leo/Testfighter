Testfighter
============

[![Build Status](https://travis-ci.org/DJLectr0/Testfighter.svg)](https://travis-ci.org/DJLectr0/BetaLauncher)

This is a rudementary implementation of Testflight on desktop.

Features
--------

#####Key Features:

- Upload a beta version of your app / game using an easy to use UI
- Only new / changed files get uploaded so you do not have to upload a 2-5 GB patch if you work on a medium sized game
- Also only new / changed files get downloaded
- Distribute the Launcher to your clients without needing to compile anything
- Expanding with more options (e.g. Should steam be present for game) is simple 
- Auto updating to latest versions (still needs more testing)
- Display of latest releases and updates in a feed
- CLI Interface for scripting

#####Planned Features:

- Github script for UE4 games
- New UI for both client and server
- Login or beta key requirement
- Stats
- Maybe iOS and Android compability

Installation
------------
#####Server

1. Upload the server folder to a VPS or a Webserver which has PHP installed and name it whatever you like.
2. Open the upload_single.php
3. Edit the line: `if($_POST["password"]=="testfighter2015")` and change 'testfighter2015' to a password of your choice
4. In a Browser go to http://yourdomain.com/path/to/the/server/folder/remove_placeholders.php
5. Finished!

#####Windows Clients
1. Unzip the Windows release
2. Go to the folder of the exe and open the settings.json
3. Edit the url entry to point to: "http://yourdomain.com/path/to/the/server/folder/" (for the Uploader client also change the password you set in the server)
4. Rezip it and destribute!

#####Mac Clients
1. Right click on the .app and choose open Package contents
2. Go to Contents/MacOS and open the settings.json
3. Edit the url entry to point to: "http://yourdomain.com/path/to/the/server/folder/" (for the Uploader client also change the password you set in the server)
4. Distribute!

#####All Clients
- Your beta testers just need to unzip them and put them in a convinient location.
- You also need to upload any updates yourself. (Although changing the settings.json is not required anymore.) You can do this by uploading the zipped releases to "/path/to/server/folder/builds/apps/os_short_name/".
