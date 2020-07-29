#!/bin/bash

../package/webgui/xls2lang.pl ../package/webgui/strings.xls
mv lang.bin ../package/webgui/lang.bin 
cp -r ../package/webgui/lang.bin ../package/webgui/files/www/lang.bin 
git add ../package/webgui/strings.xls
git add ../package/webgui/files/www/lang.bin 
git commit -m "Webgui: updated languagemarks"