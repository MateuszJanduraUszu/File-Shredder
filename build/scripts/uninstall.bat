:: uninstall.bat

:: Copyright (c) Mateusz Jandura. All rights reserved.
:: SPDX-License-Identifier: Apache-2.0

@echo off
set dir_path=%~dp0
reg delete "HKEY_CLASSES_ROOT\*\shell\DestroyFile" /f
reg delete "HKEY_CLASSES_ROOT\*\shell\DestroyFileContents" /f
del "%dir_path%fshred.exe"
del "%dir_path%install.bat"