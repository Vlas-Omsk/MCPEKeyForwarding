@echo off
F:\Utils\ADB\adb shell "su -c 'mkdir -p /data/mastering_ndk && chmod 777 /data/mastering_ndk'"
F:\Utils\ADB\adb push ./libs/arm64-v8a/keyforwarding /storage/emulated/0
F:\Utils\ADB\adb shell "su -c mv -f /storage/emulated/0/keyforwarding /data/mastering_ndk"
F:\Utils\ADB\adb shell "su -c 'cd /data/mastering_ndk && chmod 777 ./keyforwarding && ./keyforwarding -i event9 -o event8'"