# Debug

@see    Module @ref AH_Debug for more information.
@see    @ref Debug.ino "examples/Arduino-Helpers/Debug" for example usage.

## Adding a 'Debug' menu in the Arduino IDE

If you are going to be debugging a lot, it might be useful to just add a menu 
option in the IDE to disable/enable debugging.  
This can be done relatively easily by editing the `boards.txt` file.

Open the `boards.txt` file of the board you are using. If you're using version 
1.8.x of the Arduino IDE, it'll be located in 
`~/.arduino15/packages/<package-name>/hardware/<architecture>/<version>/` or 
`C:\users\<username>\AppData\Local\Arduino15\packages\<package-name>\hardware\<architecture>\<version>\`.  
Open it using a text editor (e.g. Gedit on Linux, or Notepad on Windows).

First, create the menu option by adding the following line at the top of the 
file:

```
menu.debug=Debug output
```

Then for your board, just add the different debug options.  
For example, if you're using an Arduino UNO:

```
uno.menu.debug.None=None
uno.menu.debug.None.build.debug_output=
uno.menu.debug.Serial=Serial
uno.menu.debug.Serial.build.debug_output=-DDEBUG_OUT=Serial
```

Next, add the debugging flags to the compilation options by adding the line:

```
uno.build.extra_flags={build.debug_output}
```

If your board already has an `extra_flags` entry, just add 
` {build.debug_output}` to the end (separated by a space). For example:

```
leonardo.build.extra_flags={build.usb_flags} {build.debug_output}
```

A complete list of all the AVR boards and their added debug options can be found
[**here**](https://github.com/tttapa/Arduino-Helpers/blob/master/docs/extra/boards.txt.example).

Finally, restart the IDE.  
If you now open your the `Tools` menu in the Arduino IDE, you should see the 
debug options:
![Selecting the Debug output in the Arduino IDE](Screenshot-Arduino-IDE-Debug.png)