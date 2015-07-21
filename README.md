Testfighter
============

[![Build Status](https://travis-ci.org/DJLectr0/Testfighter.svg?branch=master)](https://travis-ci.org/DJLectr0/Testfighter)

This is a rudementary implementation of Testflight on desktop.

Features
--------

#####Key Features:

- Upload a beta version of your app / game using an easy to use UI
- Only new / changed files get uploaded so you do not have to upload a 2-5 GB patch if you work on a medium sized game. (__Note for UE4:__ It is best if you disable PAK files and Full rebuild in packaging settings. This way you only upload changed / new uassets.)
- Also only new / changed files get downloaded
- Distribute the Launcher to your clients without needing to compile anything
- Expanding with more options (e.g. Should steam be present for game) is simple 
- Auto updating to latest versions
- Display of latest releases and updates in a feed
- CLI Interface for scripting

#####Planned Features:

- Github script for UE4 games
- New UI for both client and server
- Login or beta key requirement
- Stats
- Maybe iOS and Android compability

#####Known Limitations
- The display name of an application is the one of the folder you are uploading.
- The executable files must have the exact same name as the display name. (e.g. Folder name is YourGame, the executable must be named YourGame.exe, YourGame.app)
- At the moment .app applications need to be placed into a folder to be uploaded.
- Updates must be uploaded by you to your server

Installation
------------
#####Server

1. Upload the server folder to a VPS or a Webserver which has PHP installed and name it whatever you like.
2. Open the file config.json
3. Edit the `"password" : "testfighter2015"` and change 'testfighter2015' to a password of your choice
4. In a Browser go to http://yourdomain.com/path/to/the/server/folder/remove_placeholders.php
5. If your webserver does not support a user ini file in php, change upload_max_filesize in your php.ini to something big. (500M should work fine)
6. Finished!

#####Windows and Mac Clients
1. Unzip the release
2. Go to the folder of the .exe or .app and open settings.json
3. Edit the url entry to point to: "http://yourdomain.com/path/to/the/server/folder/" (for the Uploader client also change the password you set in the server)
4. Rezip it and destribute!

#####All Clients
- Your beta testers just need to unzip them and put them in a convinient location.
- You also need to upload any updates yourself. (Although changing the settings.json is not required anymore.) You can do this by uploading the zipped releases (without the os name suffix) to "/path/to/server/folder/builds/apps/os_short_name/".
