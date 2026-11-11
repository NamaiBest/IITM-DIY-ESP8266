# IITM DIY Controller for Android

The app bundles the [new controller UI](../new-ui/) and sends movement commands over Wi-Fi to the ESP8266. It requires Android 8.0 or later and an up-to-date Android System WebView.

The new dual-driver sketch supports two control options over the same bot Wi-Fi. Use the **Web UI** at [192.168.4.1](http://192.168.4.1/) in a browser, or install the **Android app (APK)** using the steps below.

## Install and drive

1. Upload the [new controller sketch](../new-ui/) to the dual-driver mecanum bot.
2. On your phone, [download the signed APK](https://github.com/NamaiBest/IITM-DIY-ESP8266/releases/download/android-v1.0.0/iitm-diy-controller.apk) and open it. If Android prompts you, allow app installs from the app that opened the file.
3. In Android's Wi-Fi settings, join **IITM-DIY-Mecanum-UI** with password **`12345678`**. If Android asks whether to stay connected without internet, choose to stay connected.
4. Open **IITM DIY Controller**. Hold a direction to drive and release to stop.

## Build the APK

Install JDK 17, Python 3, and the Android SDK with **Android SDK Platform 36** and **Build Tools 35.0.0**. Set `ANDROID_HOME` to the SDK path. Set `APK_KEYSTORE` and `APK_KEY_ALIAS` to your signing key, then run:

```bash
./android/build-apk.sh /path/to/iitm-diy-controller.apk
```

The script prompts for the keystore password unless `APK_KEY_PASSWORD_FILE` points to a private password file. Use the same signing key and increment the version code in the manifest when publishing an update.
