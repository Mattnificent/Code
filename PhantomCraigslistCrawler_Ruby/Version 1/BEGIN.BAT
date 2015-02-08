@echo off

START The_Phantom_Webcrawler_v1.1.rb

:START

set /p var1= <Crash_watcher.txt

echo %var1%

rem This makes the program wait, and is adjustable
ping -n 30 127.0.0.1 > nul

set /p var2= <Crash_watcher.txt

echo %var2%

ping -n 6 127.0.0.1 > nul

IF %var1%==%var2% GOTO REBOOT

echo Hello, world!


GOTO START

:REBOOT

echo Bootsie!


START The_Phantom_Webcrawler_v1.rb

GOTO START


rem Make Batch run 10 times only (inc i until it ==10, then GOTO end), and copy Crash_watcher.txt into some array of files
rem see why my cell is receiving dumb messages
rem see if its possible to monitor internet connection
