# Mixxx-Kinetic Development Workflow

## Repository Architecture

This project uses **Git Worktrees** to maintain separate working directories for development and building:

```
~/github/mixxx-kinetic/
├── mixxx-kinetic/          # Main worktree (main branch)
│   ├── .git/               # Shared git repository
│   ├── docs/               # Documentation (including this file)
│   └── ...                 # Source code
└── .claude/                # Claude configuration

~/repo/                     # Build worktree (dev-build branch)
├── build/                  # Build artifacts (not in git)
├── configure_cmake.sh      # CMake helper script
├── remote-build.sh         # Remote build trigger
├── remote-test.sh          # Remote test runner
└── ...                     # Source code (dev-build branch)
```

### Key Concepts

**Git Worktrees** allow multiple branches to be checked out simultaneously:
- Both directories share the same `.git` repository
- Same remotes, same refs (branches, tags)
- Different working directories and checked-out branches
- No syncing needed between directories—they're automatically synchronized
- Changes committed in either location are immediately visible to the other

## Workflow Patterns

### Local Development on Build Machine

**For development work:**
```bash
cd ~/github/mixxx-kinetic/mixxx-kinetic
git checkout main  # or dev-build
# Make changes, commit, push
git push origin main
```

**For build-focused work:**
```bash
cd ~/repo
# This is always on dev-build branch
# Make changes, commit, push
git push origin dev-build
```

### Remote Development Workflow

When working from your remote computer (lower core count):

1. **Clone from GitHub** (first time only):
   ```bash
   git clone git@github.com:b0id/mixxx-kinetic.git
   cd mixxx-kinetic
   git checkout dev-build  # Switch to development branch
   ```

2. **Make changes and test remotely:**
   ```bash
   # Edit code
   git add .
   git commit -m "Your changes"

   # Push and trigger remote build
   ./remote-build.sh
   ```

3. **What `remote-build.sh` does:**
   - Auto-commits any uncommitted changes (WIP commit)
   - Pushes to GitHub (`origin/dev-build`)
   - SSHs into build machine (chi-big)
   - Pulls latest changes on build machine
   - Triggers build with all CPU cores

4. **Run tests remotely:**
   ```bash
   ./remote-test.sh
   ```

### Switching Between Branches

**In main worktree** (`~/github/mixxx-kinetic/mixxx-kinetic`):
```bash
git checkout main       # Switch to main
git checkout dev-build  # ERROR! dev-build is checked out in ~/repo
```

You **cannot** check out `dev-build` in the main worktree because it's already checked out in `~/repo`. This is a safety feature of worktrees.

**To work on dev-build:**
```bash
cd ~/repo  # Build worktree is always on dev-build
```

### Merging dev-build to main

When you're ready to merge development work into main:

```bash
cd ~/github/mixxx-kinetic/mixxx-kinetic
git checkout main
git pull origin main       # Get latest main
git merge dev-build        # Merge dev-build into main
git push origin main       # Push merged changes
```

The `dev-build` branch in `~/repo` will automatically see the new state of `main` because they share the same git repository.

## Build Configuration

### Initial CMake Configuration

The `configure_cmake.sh` script sets up the build with optimization:

```bash
cd ~/repo
./configure_cmake.sh [additional cmake options]
```

This configures CMake with:
- Ninja build system (faster than Make)
- ccache for faster rebuilds
- Compile commands export (for IDE integration)

### Building

**Local build:**
```bash
cd ~/repo
cmake --build build -j$(nproc)
```

**Remote build** (from remote computer):
```bash
./remote-build.sh  # Auto-commits, pushes, triggers remote build
```

### Testing

**Local tests:**
```bash
cd ~/repo/build
ctest --output-on-failure
```

**Remote tests:**
```bash
./remote-test.sh
```

## SSH Configuration

The `remote-build.sh` and `remote-test.sh` scripts expect an SSH host named `chi-big`. Configure this in `~/.ssh/config` on your remote computer:

```ssh-config
Host chi-big
    HostName <your-build-machine-hostname-or-ip>
    User <your-username>
    IdentityFile ~/.ssh/id_ed25519
    # Optional: keep connection alive
    ServerAliveInterval 60
```

Test SSH access:
```bash
ssh chi-big hostname
```

## Git Remotes

Both worktrees share the same remote configuration:

```bash
origin: git@github.com:b0id/mixxx-kinetic.git
```

All pushes and pulls go through GitHub, ensuring consistency across all machines.

## Worktree Management

### List all worktrees:
```bash
git worktree list
```

### Remove a worktree:
```bash
cd ~/github/mixxx-kinetic/mixxx-kinetic
git worktree remove ~/repo
```

### Add a new worktree:
```bash
cd ~/github/mixxx-kinetic/mixxx-kinetic
git worktree add ~/repo dev-build
```

### Prune stale worktree references:
```bash
git worktree prune
```

## Troubleshooting

### "fatal: 'branch' is already checked out"

This means the branch is checked out in another worktree. You cannot check out the same branch in multiple worktrees simultaneously.

**Solution**: Go to the worktree where the branch is checked out, or create a new worktree.

### Build directory is missing

The `build/` directory is not tracked by git. If you remove `~/repo` and recreate it as a worktree, you'll need to reconfigure CMake:

```bash
cd ~/repo
./configure_cmake.sh
cmake --build build -j$(nproc)
```

### Remote build fails with "permission denied"

1. **Check SSH access:**
   ```bash
   ssh chi-big
   ```

2. **Check SSH key is loaded:**
   ```bash
   ssh-add -l
   ```

3. **Add your key if needed:**
   ```bash
   ssh-add ~/.ssh/id_ed25519
   ```

### "Your branch and 'origin/branch' have diverged"

This usually happens when using `--force` push or when multiple people work on the same branch.

**Safe resolution:**
```bash
git fetch origin
git log origin/dev-build..HEAD  # See what you have that remote doesn't
git log HEAD..origin/dev-build  # See what remote has that you don't
git pull --rebase origin dev-build  # Replay your commits on top of remote
```

## Best Practices

1. **Commit frequently** on dev-build branch during development
2. **Push to GitHub regularly** so your work is backed up
3. **Use meaningful commit messages** (not just "WIP")
4. **Test before merging** dev-build into main
5. **Keep main stable** - main should always build successfully
6. **Document architectural decisions** in `docs/`

## Migration Notes

This setup was migrated from a local filesystem remote (`~/repo` → `~/github/mixxx-kinetic/mixxx-kinetic`) to a GitHub-based worktree architecture on 2026-01-22.

**Changes made:**
- Converted `~/repo` from independent clone to worktree
- Updated `remote-build.sh` to push to GitHub instead of direct SSH
- Moved architectural docs into `docs/` directory
- All remotes now point to GitHub

**Backup:** A full backup of the old setup is available at:
```
~/repo-backup-20260122-051210.tar.gz
```
