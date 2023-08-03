# File Shredder

File Shredder is an easy-to-use application that securely deletes/shreds files.

## Build

To successfully build the project, follow these steps:

1. Ensure that you have CMake and a compiler known to CMake properly installed.
2. Clone the repository using the following command:

```bat
git clone https://github.com/MateuszJanduraUszu/File-Shredder.git
```

3. Build the `fshred` executable:

```bat
cd build\scripts
build.bat {x64|Win32} "{Compiler}"
```

These steps will help you compile the project's executable using the specified platform architecture
and compiler.

## Installation

1. Download the appropriate package based on your CPU architecture:
	* For 64-bit systems, download `Bin-x64.zip`.
	* For 32-bit systems, download `Bin-x86.zip`.
2. Extract the downloaded package. You should see the following files:
	* `fshred.exe` - The File Shredder executable.
	* `install.bat` - Batch script for installation.
	* `uninstall.bat` - Batch script for uninstallation.
3. Run `install.bat` as an administrator. The script will set up the
   necessary registry entries and configurations for the application.

## Uninstallation

Run `uninstall.bat` as an administrator to revert the changes made by `install.bat`.
After that, delete the `uninstall.bat` script.

## Usage

1. Open File Explorer
2. Locate the file you want to delete/shred.
3. Right-click on the file.
4. Select either `Destroy file` or `Destroy file contents` to delete/shred the file.
5. Confirm the prompt to proceed with the deletion/shredding process.
6. Done, the file will be securely deleted/shredded.

## How it works

The File Shredder uses the [`DoD 5220.22-M (ECE)`](https://www.media-clone.net/v/vspfiles/downloads/DoDEandECE.pdf)
to securely shred files. The application performs 7 passes specified by this standard,
resizes the file to 0, and optionally deletes the file.

## Compatibility

The File Shredder has been compiled with support for `Bcrypt.dll` and utilizes C\++17 features.
While efforts have been made to ensure compatibility across different Windows versions,
it is important to note that the application may have limitations on older operating
systems that lack support for the required C\++17 features. For optimal compatibility,
it is recommended to run the File Shredder on Windows Vista or newer operating systems
that support both `Bcrypt.dll` and the required C\++17 features.

## Questions and support

If you have any questions, encounter issues, or need assistance with the File Shredder
application, feel free to reach out. You can reach me through the `Issues` section
or email ([mjandura03@gmail.com](mailto:mjandura03@gmail.com)).

## License

Copyright © Mateusz Jandura.

SPDX-License-Identifier: Apache-2.0