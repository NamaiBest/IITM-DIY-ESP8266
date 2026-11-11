#!/usr/bin/env bash
set -euo pipefail

app_dir="$(cd "$(dirname "$0")" && pwd)"
: "${ANDROID_HOME:?Set ANDROID_HOME to your Android SDK directory}"
: "${APK_KEYSTORE:?Set APK_KEYSTORE to your signing keystore}"
: "${APK_KEY_ALIAS:?Set APK_KEY_ALIAS to your signing alias}"
tools="$ANDROID_HOME/build-tools/35.0.0"
platform="$ANDROID_HOME/platforms/android-36/android.jar"
output="${1:-$app_dir/../iitm-diy-controller.apk}"

build="$(mktemp -d)"
trap 'rm -rf "$build"' EXIT
mkdir -p "$build/assets" "$build/classes" "$build/dex"

python3 - "$app_dir/../new-ui/dual_driver_mecanum_ui/controller.html" "$build/assets/controller.html" <<'PY'
from pathlib import Path
import sys

source, destination = map(Path, sys.argv[1:])
html = source.read_text(encoding="utf-8")
replacements = {
    "const live = false;": "const live = true;",
    "Arrow keys to drive · Esc to stop": "Join IITM-DIY-Mecanum-UI in Wi-Fi settings",
    "Connection lost": "Bot unreachable",
    ' target="_blank"': "",
}
for original, updated in replacements.items():
    if html.count(original) != 1:
        raise SystemExit(f"Expected exactly one occurrence of {original!r}")
    html = html.replace(original, updated)
destination.write_text(html, encoding="utf-8")
PY

"$tools/aapt2" compile --dir "$app_dir/res" -o "$build/resources.zip"
"$tools/aapt2" link -I "$platform" --manifest "$app_dir/AndroidManifest.xml" \
    --min-sdk-version 26 --target-sdk-version 36 -A "$build/assets" \
    -o "$build/unsigned.apk" "$build/resources.zip"

"${JAVA_HOME:+$JAVA_HOME/bin/}javac" --release 8 -classpath "$platform" \
    -d "$build/classes" "$app_dir/src/in/namai/iitmdiy/controller/ControllerActivity.java"
jar cf "$build/classes.jar" -C "$build/classes" .
"$tools/d8" --release --min-api 26 --lib "$platform" --output "$build/dex" "$build/classes.jar"
(cd "$build/dex" && zip -q "$build/unsigned.apk" classes.dex)
"$tools/zipalign" -f 4 "$build/unsigned.apk" "$build/aligned.apk"

sign_args=(--ks "$APK_KEYSTORE" --ks-key-alias "$APK_KEY_ALIAS")
if [[ -n "${APK_KEY_PASSWORD_FILE:-}" ]]; then
    sign_args+=(--ks-pass "file:$APK_KEY_PASSWORD_FILE")
fi
"$tools/apksigner" sign "${sign_args[@]}" --out "$output" "$build/aligned.apk"
"$tools/apksigner" verify --verbose "$output"
