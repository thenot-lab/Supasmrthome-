# BUILD_AND_RELEASE.md — Mobile IDE (scaffold, build & release)

Purpose: single-file, copy & paste instructions and CI workflow to scaffold, build, and publish a debug APK for thenot-lab/mobile-ide (branch: main).

Quick summary
- Create the repository thenot-lab/mobile-ide (main).
- Add this file and the workflow (.github/workflows/scaffold-build-release.yml).
- Workflow will scaffold minimal Kivy app if needed, install Android SDK + Buildozer, build debug APK, attach it to a GitHub Release and provide a direct download URL you can open on your Android device.

1) Create repo
- Owner: thenot-lab
- Name: mobile-ide
- Branch: main
- Enable Actions for the repo

2) Add the workflow
Create file: `.github/workflows/scaffold-build-release.yml` with the exact YAML below.

--- BEGIN WORKFLOW (paste exactly) ---
name: Scaffold, Build & Release APK

on:
  workflow_dispatch:
  push:
    branches: [ main ]

jobs:
  scaffold-build-release:
    runs-on: ubuntu-latest
    env:
      PYVER: '3.9'
      ANDROID_SDK_ROOT: ${{ runner.temp }}/android-sdk
      APK_NAME: mobileidepro-debug.apk

    steps:
    - name: Checkout repository
      uses: actions/checkout@v4
      with:
        fetch-depth: 0

    - name: Scaffold minimal app if missing
      id: scaffold
      run: |
        set -e
        if [ ! -f main.py ] || [ ! -f buildozer.spec ]; then
          echo "Scaffolding minimal mobile-ide app..."
          cat > main.py <<'PY'
#!/usr/bin/env python3
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.core.window import Window

class MobileIDERoot(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.orientation = 'vertical'
        top = BoxLayout(size_hint_y=0.12)
        top.add_widget(Label(text='Mobile IDE (Minimal)', bold=True))
        top.add_widget(Button(text='Login (GitHub)', on_press=lambda x: None))
        self.add_widget(top)
        editor = Label(text='Code editor placeholder\nStart typing here...', halign='left')
        self.add_widget(editor)
        bottom = BoxLayout(size_hint_y=0.15)
        bottom.add_widget(Button(text='Run', on_press=lambda x: None))
        bottom.add_widget(Button(text='Cloud Sync', on_press=lambda x: None))
        self.add_widget(bottom)

class MobileIDEApp(App):
    def build(self):
        Window.clearcolor = (0.12,0.12,0.13,1)
        return MobileIDERoot()

if __name__ == '__main__':
    MobileIDEApp().run()
PY
          cat > buildozer.spec <<'INI'
[app]
title = Mobile IDE
package.name = mobileidepro
package.domain = com.thenotlab
source.dir = .
source.include_exts = py,png,jpg,kv,atlas,json,ttf
version = 1.0.0
requirements = python3,kivy,pygments,aiohttp,requests,Pillow
orientation = portrait
android.permissions = INTERNET,READ_EXTERNAL_STORAGE,WRITE_EXTERNAL_STORAGE,ACCESS_NETWORK_STATE
android.minapi = 27
android.api = 33
android.ndk = 25b
icon.filename = assets/icon.png
INI
          cat > requirements.txt <<'REQ'
kivy>=2.2.0
pygments>=2.0
REQ
          mkdir -p assets/fonts assets/themes src/ui src/copilot src/auth src/filesystem src/executor
          echo "Replace with real PNG" > assets/placeholder_icon.txt
          git config user.name "github-actions[bot]"
          git config user.email "41898282+github-actions[bot]@users.noreply.github.com"
          git add main.py buildozer.spec requirements.txt assets || true
          git commit -m "scaffold: add minimal Kivy app and buildozer.spec" || true
          echo "SCaffolded=1" >> $GITHUB_OUTPUT
        else
          echo "SCaffolded=0" >> $GITHUB_OUTPUT
        fi

    - name: Install OS packages
      run: |
        sudo apt-get update -y
        sudo apt-get install -y openjdk-11-jdk unzip wget libc6-i386 lib32stdc++6 lib32gcc1

    - name: Install Android tools & SDK packages
      run: |
        set -e
        mkdir -p "$ANDROID_SDK_ROOT/cmdline-tools"
        cd $RUNNER_TEMP
        wget -q https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip -O cmdline-tools.zip
        unzip -q cmdline-tools.zip -d "$ANDROID_SDK_ROOT/cmdline-tools"
        mv "$ANDROID_SDK_ROOT/cmdline-tools/cmdline-tools" "$ANDROID_SDK_ROOT/cmdline-tools/latest" || true
        export PATH="$ANDROID_SDK_ROOT/cmdline-tools/latest/bin:$PATH"
        yes | "$ANDROID_SDK_ROOT/cmdline-tools/latest/bin/sdkmanager" --sdk_root="$ANDROID_SDK_ROOT" "platform-tools" "platforms;android-33" "build-tools;33.0.2" || true
        echo "ANDROID_SDK_ROOT=$ANDROID_SDK_ROOT" >> $GITHUB_ENV
        echo "$ANDROID_SDK_ROOT/cmdline-tools/latest/bin" >> $GITHUB_PATH

    - name: Accept Android SDK licenses
      run: |
        yes | "$ANDROID_SDK_ROOT/cmdline-tools/latest/bin/sdkmanager" --licenses || true

    - name: Setup Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.9'

    - name: Install Buildozer, Cython and requirements
      run: |
        python -m pip install --upgrade pip wheel setuptools
        pip install buildozer==1.4.2 cython==0.29.36
        pip install -r requirements.txt || true

    - name: Build APK with Buildozer
      run: |
        set -e
        buildozer android clean || true
        buildozer -v android debug
        ls -la bin || true

    - name: Upload APK artifact
      uses: actions/upload-artifact@v3
      with:
        name: mobileide-apk
        path: bin/*.apk

    - name: Create Release and upload APK
      uses: softprops/action-gh-release@v1
      with:
        files: bin/*.apk
        tag_name: v${{ github.run_number }}
        name: "mobile-ide Release v${{ github.run_number }}"
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}

    - name: Output install link
      run: |
        APK_FILE=$(basename bin/*.apk)
        INSTALL_URL="https://github.com/${GITHUB_REPOSITORY}/releases/latest/download/${APK_FILE}"
        echo "InstallURL=${INSTALL_URL}" >> $GITHUB_OUTPUT
        echo "Download/Install: ${INSTALL_URL}"
--- END WORKFLOW ---

3) Trigger build
- git add . && git commit -m "Add CI build" && git push origin main
- Or run Actions → "Scaffold, Build & Release APK" → Run workflow

4) After build completes
- Open the workflow run summary; look for the "Download/Install" URL (also available as the Release asset).
- Open that URL on your Android device (browser) to download the APK, then install (allow "Install unknown apps" if prompted).

5) Keystore & signed releases (concise)
- Create keystore locally:
  keytool -genkeypair -v -keystore my-release.keystore -alias mobileide -keyalg RSA -keysize 2048 -validity 10000
- Store secrets in Repo Settings → Secrets:
  - ANDROID_KEYSTORE_BASE64 (base64 of keystore)
  - KEYSTORE_PASSWORD
  - KEY_ALIAS
  - KEY_PASSWORD
- Add workflow steps to decode the keystore, place it where Buildozer expects, and configure signing in buildozer.spec:
  android.release_keyalias = <alias>
  android.release_keystore = <path>
  android.release_storepass = <pass>
  android.release_keypass = <keypass>
- Ask me if you want the exact signing snippet.

6) GitHub OAuth & Copilot (concise)
- Create OAuth App: https://github.com/settings/developers → New OAuth App
  - Name: Mobile IDE
  - Homepage: https://github.com/thenot-lab/mobile-ide
  - Callback URL: mobile-ide://auth/callback
- Save Client ID & Client Secret (do not commit).
- Implement OAuth in app via external browser or WebView + URL scheme redirect handling.
- Copilot usage: user must have Copilot subscription; follow GitHub/Copilot API access rules and token flow.

7) Permissions & file access (concise)
- buildozer.spec includes READ/WRITE external storage and INTERNET for Android 8.1 (API 27).
- For Android 11+ use SAF or request MANAGE_EXTERNAL_STORAGE and handle runtime flow (Play Store restrictions).
- For inter-app file sharing use FileProvider + content:// URIs and add android.manifest_additions in buildozer.spec if needed.

8) Local development (desktop)
- Setup:
  python3 -m venv venv
  source venv/bin/activate
  pip install -r requirements.txt
  python main.py

9) Troubleshooting (most common)
- Build fails: open Actions log, examine python-for-android / Buildozer errors; common fixes: pin dependency versions, increase runner resources (self-hosted), clear buildozer cache.
- Release upload fails: ensure GITHUB_TOKEN has write access and Releases are allowed.
- Install blocked: enable "Install unknown apps" for browser/file manager on device.

10) Next options you can request (I can provide):
- (A) Signing snippet for CI to produce a release-signed APK (requires keystore and secrets).
- (B) Richer scaffold (editor, file browser, terminal) committed to repo or provided as a unifile (I will create files; need repo presence/permission to push).
- (C) GitHub Pages install page that shows an "Install" button linking to the Release asset.

Security reminders
- Never commit secrets to the repo. Use GitHub Secrets.
- Validate and sandbox code execution features — running user code on device is dangerous; communicate risks to users.

If you want me to produce the CI keystore signing snippet, say: "Add signing snippet" and provide the keystore (base64) and secret names to use.
