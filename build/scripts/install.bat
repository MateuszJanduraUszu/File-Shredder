:: install.bat

:: Copyright (c) Mateusz Jandura. All rights reserved.
:: SPDX-License-Identifier: Apache-2.0

@echo off
set dir_path=%~dp0
reg add "HKEY_CLASSES_ROOT\*\shell\DestroyFile" /f /ve /d "Destroy file"
reg add "HKEY_CLASSES_ROOT\*\shell\DestroyFile\command" /f /ve /d "%dir_path%fshred.exe ""%%1"" -d"
reg add "HKEY_CLASSES_ROOT\*\shell\DestroyFileContents" /f /ve /d "Destroy file contents"
reg add "HKEY_CLASSES_ROOT\*\shell\DestroyFileContents\command" /f /ve /d "%dir_path%fshred.exe ""%%1""