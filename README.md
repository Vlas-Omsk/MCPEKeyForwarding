# MCPEKeyForwarding
Just an application that triggers a keyboard button when the right mouse button is pressed. Useful if your device treats the right mouse button as a back button by default and this can't just be changed.

The application is intended for use in Minecraft PE, but can also be used in other applications.

When you press the right mouse button, Minecraft suppresses the exit from the application, but at the same time does not perceive the click as setting a block, while the application also catches the event from mouse and sends a keyboard button click event that Minecraft can process as a block setting.

This solution will be relevant until the Minecraft developers fix this problem officially.
[MCPE-122678](https://bugs.mojang.com/browse/MCPE-122678)

***Root is required.***

## Example
***Use ADB or Termux or whatever***

Find your mouse and keyboard event id with the command:
```bash
su -c 'getevent -p'
```

And then run this application with arguments, where -i is the id of the mouse, and -o is the id of the keyboard:
```bash
su -c 'cd /path/to/application/folder && chmod 777 ./keyforwarding && ./keyforwarding -i event9 -o event8'
```

## Tests
| Model               | Firmware           | Android   | Working?     |
| :------------------ | :----------------- | :-------- | :----------: |
| Xiaomi Redmi Note 7 | MIUI Global 11.0.9 | Android 9 | ```Stable``` |
