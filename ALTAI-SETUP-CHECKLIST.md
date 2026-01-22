# Setup Checklist for altai (Remote Workstation)

This checklist helps you configure altai to work with the new git worktree workflow and secure SSH setup.

## ✅ Tasks to Complete on altai

### 1. Clone or Update Repository

**If you don't have the repo yet:**
```bash
git clone git@github.com:b0id/mixxx-kinetic.git
cd mixxx-kinetic
git checkout dev-build
```

**If you already have the repo:**
```bash
cd ~/path/to/mixxx-kinetic
git fetch origin
git checkout dev-build
git pull origin dev-build
```

### 2. Configure SSH ControlMaster

**Create socket directory:**
```bash
mkdir -p ~/.ssh/sockets
```

**Add to `~/.ssh/config`:**

Open the file:
```bash
vim ~/.ssh/config  # or nano, or your preferred editor
```

Add this configuration (also available in `ssh-config-for-altai.txt`):
```ssh-config
Host home
    HostName colB0idHomeEnd
    User b0id
    IdentityFile ~/.ssh/id_ed25519  # Your PASSWORD-PROTECTED key

    # ControlMaster: Reuse SSH connections for 50 minutes
    ControlMaster auto
    ControlPath ~/.ssh/sockets/%r@%h-%p
    ControlPersist 3000  # 50 minutes (3000 seconds)

    # Keep connection alive
    ServerAliveInterval 60
    ServerAliveCountMax 3

Host github.com
    IdentityFile ~/.ssh/id_ed25519
    AddKeysToAgent yes
```

**Test the configuration:**
```bash
ssh home hostname
# Should prompt for password ONCE
# Should return: colB0idHomeEnd

# Try again within 50 minutes
ssh home hostname
# Should NOT prompt for password
```

### 3. Remove Passwordless Key (Recommended)

**Find the passwordless key on altai:**
```bash
ls -la ~/.ssh/
# Look for keys like id_rsa, id_ed25519_nopass, etc.
```

**Test which key has no password:**
```bash
# This will prompt for password if key is protected
ssh-keygen -y -f ~/.ssh/id_ed25519_name
# If NO password prompt, it's passwordless
```

**Remove it from chi-big:**
```bash
# SSH into chi-big (using password-protected key)
ssh home

# Edit authorized_keys
vim ~/.ssh/authorized_keys

# Find and DELETE the line with the passwordless key's public key
# Look for a comment at the end that might identify it
# Save and exit

# Logout from chi-big
exit
```

**Delete the passwordless key files on altai:**
```bash
# ONLY delete after confirming password-protected key works!
rm ~/.ssh/id_passwordless_keyname
rm ~/.ssh/id_passwordless_keyname.pub
```

**Remove old SSH config entries:**
```bash
vim ~/.ssh/config
# Remove any old 'chi-big' Host entries if they exist
```

### 4. Test the Workflow

**Make a small change:**
```bash
cd ~/mixxx-kinetic
echo "// Test from altai" >> README.md
```

**Run remote build:**
```bash
./remote-build.sh
```

**What should happen:**
1. ✅ Auto-commits your change
2. ✅ Pushes to GitHub (may prompt for SSH password if first GitHub access)
3. ✅ SSHes to home (no password if within 50 minutes!)
4. ✅ Pulls changes on home
5. ✅ Triggers build

**Run remote tests:**
```bash
./remote-test.sh
```

Should run tests without password prompt (if within 50-minute window).

### 5. Optional: Setup SSH Agent for Session

If you want to avoid entering passwords for GitHub and home separately, use ssh-agent:

```bash
# Add to ~/.bashrc or run at start of session
if [ -z "$SSH_AUTH_SOCK" ]; then
    eval $(ssh-agent -s)
fi

# At start of work session
ssh-add ~/.ssh/id_ed25519
# Password: **** (enter once for all SSH operations)
```

## 🔍 Verification Checklist

After setup, verify these work:

- [ ] `ssh home hostname` returns `colB0idHomeEnd`
- [ ] Second `ssh home` within 50 minutes doesn't prompt for password
- [ ] `./remote-build.sh` successfully triggers build on chi-big
- [ ] `./remote-test.sh` successfully runs tests on chi-big
- [ ] No passwordless keys remain in `~/.ssh/`
- [ ] `git push origin dev-build` works (to GitHub)

## 📝 Daily Workflow

Once setup is complete, your typical work session looks like:

```bash
# Start work session
cd ~/mixxx-kinetic
ssh home hostname  # Enter password ONCE
# Password: ****

# Work on code
vim src/some_file.cpp

# Build remotely (no password for 50 minutes!)
./remote-build.sh

# Test remotely (no password!)
./remote-test.sh

# More changes...
vim src/another_file.cpp
./remote-build.sh  # Still no password!

# After 50 minutes of inactivity, next SSH will prompt again
```

## 🆘 Troubleshooting

**"Permission denied (publickey)"**
- Check `~/.ssh/config` has correct `IdentityFile` path
- Verify key exists: `ls -la ~/.ssh/id_ed25519`
- Test key: `ssh -i ~/.ssh/id_ed25519 home hostname`

**"Could not resolve hostname colB0idHomeEnd"**
- Add IP address to SSH config instead:
  ```ssh-config
  Host home
      HostName 192.168.x.x  # Use actual IP
  ```

**"Control socket connect(...): Connection refused"**
- Old socket is stale, remove it:
  ```bash
  rm ~/.ssh/sockets/*
  ```

**Still prompting for password every time**
- ControlMaster might not be working
- Check socket directory exists: `ls -la ~/.ssh/sockets/`
- Check for typos in `~/.ssh/config`
- Test: `ssh -O check home` (should say "Master running")

## 📚 Documentation

- [WORKFLOW.md](docs/WORKFLOW.md) - Complete workflow documentation
- [SSH-SECURITY.md](docs/SSH-SECURITY.md) - Detailed SSH security guide
- `ssh-config-for-altai.txt` - Ready-to-use SSH config

## 🎯 What Changed?

**Old setup:**
- Used passwordless SSH key
- Direct push to chi-big via git remote
- Security concern with passwordless key

**New setup:**
- Uses password-protected SSH key
- Push to GitHub, pull on chi-big
- ControlMaster: enter password once per 50 minutes
- Git worktrees on chi-big (no sync issues)

---

**Need help?** Check the documentation in `docs/` or review this checklist.
