# Dev Container Build Setup

This repository includes a Dev Container configuration for building the Copilot 365 Agent in an isolated environment.

## Prerequisites

- Docker installed and running
- Visual Studio Code with Dev Containers extension (or compatible IDE)

## Quick Start

1. **Open in Dev Container**
   - Open this repository in VS Code
   - When prompted, click "Reopen in Container"
   - Or use Command Palette: `Dev Containers: Reopen in Container`

2. **Build the Agent**
   ```bash
   ./build.sh
   ```

3. **Find the Binaries**
   The compiled binaries will be in the `build/` directory:
   - `copilot365_agent_win.exe` - Windows 64-bit PE executable
   - `copilot365_agent_linux` - Linux 64-bit ELF executable

## Project Structure

```
/opt/copilot-365-agent/
├── .devcontainer/
│   └── devcontainer.json    # Dev Container configuration
├── Dockerfile               # Ubuntu 20.04 with build tools
├── build.sh                 # Cross-platform build script
├── src/                     # Source code modules
│   ├── ingress.c           # Main entry point
│   ├── propagation.c       # Mutation engine
│   ├── targeting.c         # Target identification
│   ├── effect.c            # Payload execution
│   ├── conceal.c           # Stealth capabilities
│   ├── persistence.c       # Survival mechanisms
│   └── mutation.h          # Header file
└── tests/
    └── sandbox_tests.md    # Test documentation
```

## Environment Details

The Dev Container is configured with:
- **Base Image**: Ubuntu 20.04
- **Network**: Isolated (`--network=none`)
- **Capabilities**: `SYS_ADMIN` added
- **Tools Installed**:
  - `build-essential` - GCC and build tools
  - `mingw-w64` - Windows cross-compiler
  - `gcc-multilib` - Multi-architecture support
  - `git` - Version control
  - `python3` - Python support

## Build Process

The `build.sh` script:
1. Generates a random mutation seed
2. Compiles for Windows using `x86_64-w64-mingw32-gcc`
3. Compiles for Linux using `gcc`
4. Both builds use `-Os` optimization and the same mutation seed

Each build produces a unique binary with compile-time variation based on the random seed.

## Testing

Refer to `tests/sandbox_tests.md` for comprehensive testing procedures.

## Security

- The build environment has no network access
- Builds are performed in an isolated container
- Build artifacts are excluded from version control

## Troubleshooting

**Issue**: Permission denied when running `build.sh`
**Solution**: Make the script executable: `chmod +x build.sh`

**Issue**: Build directory not found
**Solution**: The script automatically creates the `build/` directory

**Issue**: Compiler not found
**Solution**: Rebuild the Dev Container to ensure all tools are installed
