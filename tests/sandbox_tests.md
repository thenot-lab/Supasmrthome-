# Sandbox Tests for Copilot 365 Agent

## Overview
This document describes the testing procedures for the Copilot 365 Agent in a sandboxed environment.

## Test Environment Requirements
- Isolated Dev Container with no network access (`--network=none`)
- Docker with `--cap-add=SYS_ADMIN` capability
- Ubuntu 20.04 base image

## Build Tests

### Test 1: Successful Compilation
**Objective:** Verify that the agent compiles successfully for both Windows and Linux targets.

**Steps:**
1. Execute `./build.sh` inside the Dev Container
2. Verify `build/copilot365_agent_win.exe` is created
3. Verify `build/copilot365_agent_linux` is created
4. Check that both binaries are non-zero in size

**Expected Result:** Both binaries should be created without compilation errors.

### Test 2: Mutation Seed Variation
**Objective:** Verify that different builds produce different mutation seeds.

**Steps:**
1. Run `./build.sh` multiple times
2. Compare the MUTATION_SEED values used in each build
3. Verify binaries differ slightly due to different seeds

**Expected Result:** Each build should use a different random seed.

## Module Tests

### Test 3: Module Initialization
**Objective:** Verify all modules initialize correctly.

**Steps:**
1. Execute the Linux binary: `./build/copilot365_agent_linux`
2. Verify output shows initialization messages for:
   - Propagation module
   - Targeting module
   - Effect module
   - Conceal module
   - Persistence module

**Expected Result:** All modules should report successful initialization.

### Test 4: Mutation Engine
**Objective:** Verify the mutation engine initializes with the compile-time seed.

**Steps:**
1. Execute the binary
2. Verify the mutation seed is displayed in the output
3. Confirm the seed matches the compile-time MUTATION_SEED value

**Expected Result:** Mutation seed should be consistent with compile-time value.

## Security Tests

### Test 5: Network Isolation
**Objective:** Verify the build environment has no network access.

**Steps:**
1. Inside the Dev Container, attempt to ping external hosts
2. Attempt to download external resources
3. Verify both operations fail due to network isolation

**Expected Result:** All network operations should fail.

### Test 6: Sandboxed Execution
**Objective:** Verify the agent executes in a restricted environment.

**Steps:**
1. Run the agent with restricted permissions
2. Monitor system calls and resource access
3. Verify no unauthorized access attempts

**Expected Result:** Agent should operate within defined boundaries.

## Cross-Platform Tests

### Test 7: Windows Binary Format
**Objective:** Verify Windows binary is valid PE format.

**Steps:**
1. Use `file` command to check binary format
2. Verify it reports as "PE32+ executable (console) x86-64"

**Expected Result:** Binary should be valid Windows PE executable.

### Test 8: Linux Binary Format
**Objective:** Verify Linux binary is valid ELF format.

**Steps:**
1. Use `file` command to check binary format
2. Verify it reports as "ELF 64-bit LSB executable"
3. Execute the binary on Linux to verify it runs

**Expected Result:** Binary should be valid Linux ELF executable.

## Cleanup Tests

### Test 9: Build Artifacts
**Objective:** Verify clean build process.

**Steps:**
1. Delete build/ directory contents
2. Run `./build.sh`
3. Verify only expected artifacts are created

**Expected Result:** Only the two binary files should be in build/ directory.

## Notes
- All tests should be performed in the isolated Dev Container environment
- Build artifacts should not be committed to version control
- Test results should be documented and tracked
