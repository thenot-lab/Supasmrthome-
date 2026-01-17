# Build & Release — mobile-ide

Save as BUILD_AND_RELEASE.md in the repo root (or paste into an agent). This single file includes: repo setup, local build steps, GitHub Actions workflow to scaffold/build/release an APK, keystore/signing notes, OAuth setup, runtime permissions, install instructions, and troubleshooting. Follow each numbered step.

## 1) Create repository
- Create a new repo on GitHub:
  - Owner: thenot-lab
  - Name: mobile-ide
  - Branch: main
  - Initialize with README (optional)

## 2) Add this file and the workflow
- Create these files in the repository (paths shown). You can paste them manually or use the unifile approach.
  - BUILD_AND_RELEASE.md (this file)
  - .github/workflows/scaffold-build-release.yml (workflow below)
  - Optionally: main.py, buildozer.spec, requirements.txt, assets/, src/ if you already have code.

## 3) Minimal/Scaffold GitHub Actions workflow
- Create .github/workflows/scaffold-build-release.yml with the exact contents below. This workflow will:
  - Scaffold a minimal Kivy app if main.py/buildozer.spec missing
  - Create a pull request with the scaffolded files (does NOT push directly to main)
  - After PR is merged, subsequent runs will build the APK
  - Install Android SDK tools, Buildozer, build the APK
  - Create a GitHub Release and upload the APK
  - Emit a direct download URL for the APK

See `.github/workflows/scaffold-build-release.yml` for the complete workflow.

## 4) Trigger the build
- Push to main:
  ```bash
  git add . && git commit -m "Add workflow" && git push origin main
  ```
- Or run Actions → Scaffold, Build & Release APK → Run workflow (workflow_dispatch).
- **Note**: If the workflow needs to scaffold files (first run), it will create a pull request instead of building immediately. Review and merge the PR, then trigger the workflow again to build the APK.

## 5) After the workflow completes
- Open the workflow run; view step "Output install link" or check the job summary for:
  ```
  https://github.com/thenot-lab/mobile-ide/releases/latest/download/<apk-file>.apk
  ```
- Open that URL on your Android device to download the APK. Enable "Install unknown apps" for your browser/file manager if required. Tap to install.

## 6) Keystore & signed releases (recommended for distribution)
- Create keystore locally:
  ```bash
  keytool -genkeypair -v -keystore my-release.keystore -alias mobileide -keyalg RSA -keysize 2048 -validity 10000
  ```
- To sign in CI:
  - Base64-encode keystore and store secrets in repo Settings → Secrets:
    - ANDROID_KEYSTORE_BASE64
    - KEYSTORE_PASSWORD
    - KEY_ALIAS
    - KEY_PASSWORD
  - Add steps to the workflow to decode keystore, place at ~/.buildozer/android/platform/android-*/ and set buildozer.spec signing config:
    ```ini
    android.release_keyalias = <alias>
    android.release_keystore = <path>
    android.release_storepass = <pass>
    android.release_keypass = <keypass>
    ```
- I can provide the workflow signing snippet on request.

## 7) GitHub OAuth (GitHub login & Copilot)
- Create an OAuth App: https://github.com/settings/developers → New OAuth App
  - Name: Mobile IDE
  - Homepage: https://github.com/thenot-lab/mobile-ide
  - Authorization callback URL: mobile-ide://auth/callback
- Copy Client ID and Client Secret; do NOT commit them.
- Store secrets in repo or in-app config. Implement OAuth flow inside the app via a WebView or external browser + redirect handling.
- Copilot: user must have Copilot subscription. Copilot token exchange is not public—use GitHub APIs/copilot proxy per current available docs and ensure compliance with GitHub terms.

## 8) Runtime storage permissions & file access
- buildozer.spec includes READ/WRITE external storage. On Android 8.1 (API 27) this is enough for /sdcard access.
- For Android 11+ (API 30+), scoped storage applies. Options:
  - Request MANAGE_EXTERNAL_STORAGE (special permission) in manifest (play store restrictions apply) and handle runtime request via Intent ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION.
  - Better: use Storage Access Framework (SAF) and DocumentFile APIs for file picking and read/write.
- In-app: request runtime permissions using Android API or python-for-android Android modules. Prompt user on first use.

## 9) Inter-app communication (Copilot/Claude)
- Use Intent filters and FileProvider to share files securely. Declare intent-filters in AndroidManifest and configure provider in buildozer.spec (android.manifest_additions) if needed.
- Prefer content:// URIs returned by FileProvider rather than raw filesystem paths.

## 10) Local development & desktop testing
- To run locally (desktop), install Python dependencies and run:
  ```bash
  python3 -m venv venv && source venv/bin/activate
  pip install -r requirements.txt
  python main.py
  ```

## 11) Troubleshooting (quick)
- Build fails → check build logs (Actions run), search for missing system libs or dependency version conflicts.
- "Timeout / disk space" errors → re-run; consider self-hosted runner for heavy builds.
- App permission denied → ensure runtime permission request implemented and user granted.

## 12) OAuth customization (pick one)
- Add full UI, file manager, Copilot client, OAuth code: I can provide code snippets or a PR if you permit.
- Add APK signing snippet to CI: provide keystore and I will show workflow changes.
- Add GitHub Pages install page to present "Install" button linking to release asset.

## 13) Best practices (brief)
- Never commit secrets (Client Secret, keystore passwords). Use GitHub Secrets.
- Use HTTPS for all network calls.
- Validate and sandbox any executed code; running arbitrary code on device is risky—warn users and implement runtime limits.

## 14) Contact / reproduction notes
- This file targets thenot-lab/mobile-ide on branch main and the GitHub Actions runner environment ubuntu-latest (2026-01-17). Adjust PYVER, android.api, ndk and buildozer versions if you have constraints.

## Next Steps

If you want, I will:
- (A) produce the exact keystore signing workflow snippet,
- (B) generate a richer scaffold (editor+file browser+terminal) and open a PR (I will need permission to create PRs),
- (C) produce a GitHub Pages install page job.

Tell me which option to do next.
