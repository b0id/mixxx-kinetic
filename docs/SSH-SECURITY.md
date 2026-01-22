# SSH Security Best Practices for Mixxx-Kinetic Workflow

## Overview

This document covers secure SSH key management for the Mixxx-Kinetic development workflow, which involves:
1. Pushing code to GitHub
2. SSHing into build machine (chi-big) to trigger builds

## Security Levels

### Level 1: SSH Agent with Timeout ⭐ RECOMMENDED

Use password-protected keys with ssh-agent that has a timeout.

**On your remote computer**, create a startup script:

```bash
# ~/.ssh/start-agent.sh
#!/bin/bash

# Start ssh-agent with 4-hour timeout (14400 seconds)
# Adjust timeout based on your typical work session length
eval $(ssh-agent -t 14400)

# Add your password-protected keys
ssh-add ~/.ssh/id_ed25519  # GitHub key
ssh-add ~/.ssh/id_chi_big   # chi-big key (if separate)

# Display loaded keys
echo "Loaded keys:"
ssh-add -l
```

**Usage:**
```bash
# At start of work session
source ~/.ssh/start-agent.sh
# Enter passwords for your keys (once per session)

# Work normally - no more password prompts for 4 hours
./remote-build.sh
git push origin dev-build
ssh chi-big
```

**Benefits:**
- Keys are password-protected on disk
- Passwords entered only once per session
- Keys auto-expire after timeout
- Works seamlessly with scripts

**Security note**: While keys are in ssh-agent memory, they can be used by any process running as your user. The timeout limits exposure window.

### Level 2: Separate Keys with Command Restrictions

Use a separate, passwordless key for chi-big that can ONLY run build commands.

**1. On remote computer, create restricted key:**
```bash
ssh-keygen -t ed25519 -f ~/.ssh/id_chi_big_build -C "chi-big-build-only"
# Press Enter for no password (this key will be restricted)
```

**2. On chi-big, add to `~/.ssh/authorized_keys` with restrictions:**
```bash
# ~/.ssh/authorized_keys
command="~/bin/build-command.sh",no-port-forwarding,no-X11-forwarding,no-agent-forwarding ssh-ed25519 AAAAC3... chi-big-build-only
```

**3. On chi-big, create the allowed command script:**
```bash
#!/bin/bash
# ~/bin/build-command.sh

# Only allow specific commands
case "$SSH_ORIGINAL_COMMAND" in
    "cd ~/repo && git pull origin dev-build && cmake --build build -j$(nproc)")
        cd ~/repo && git pull origin dev-build && cmake --build build -j$(nproc)
        ;;
    "cd ~/repo && cd build && ctest --output-on-failure")
        cd ~/repo && cd build && ctest --output-on-failure
        ;;
    *)
        echo "Command not allowed: $SSH_ORIGINAL_COMMAND"
        exit 1
        ;;
esac
```

**Security benefit**: Even if the passwordless key is stolen, attacker can ONLY trigger builds, not get shell access.

**Update remote-build.sh:**
```bash
# Use specific key for chi-big
ssh -i ~/.ssh/id_chi_big_build $REMOTE_HOST "cd $REMOTE_DIR && git pull origin dev-build && cmake --build build -j\$(nproc)"
```

### Level 3: SSH ControlMaster (Reuse Connections)

Share a single SSH connection across multiple commands, requiring password only once.

**On remote computer, add to `~/.ssh/config`:**
```ssh-config
Host chi-big
    HostName colB0idHomeEnd
    User b0id
    IdentityFile ~/.ssh/id_ed25519

    # Connection sharing
    ControlMaster auto
    ControlPath ~/.ssh/sockets/%r@%h-%p
    ControlPersist 4h

    # Keep connection alive
    ServerAliveInterval 60
    ServerAliveCountMax 3
```

**Create socket directory:**
```bash
mkdir -p ~/.ssh/sockets
```

**How it works:**
```bash
# First SSH connection prompts for password
ssh chi-big hostname
# Password: ****

# Subsequent connections (within 4 hours) reuse the first connection
ssh chi-big hostname  # No password prompt!
./remote-build.sh     # No password prompt!
```

**Benefits:**
- Password-protected key on disk
- One password entry per 4-hour window
- Faster subsequent connections (no handshake)
- Works with all SSH-based tools

### Level 4: GitHub CLI with Token

For GitHub operations, use GitHub CLI instead of SSH.

**Setup:**
```bash
# Install GitHub CLI
# Arch: sudo pacman -S github-cli
# Debian/Ubuntu: sudo apt install gh

# Authenticate
gh auth login
# Choose: HTTPS, paste token, etc.
```

**Update remote-build.sh to use HTTPS:**
```bash
# Change remote to HTTPS
git remote set-url origin https://github.com/b0id/mixxx-kinetic.git

# Push uses token instead of SSH key
git push origin HEAD:$BRANCH
```

**Benefits:**
- No SSH key needed for GitHub
- Token can have limited scopes (repo access only)
- Can revoke token without changing keys
- Still need SSH for chi-big, but reduced attack surface

**Drawbacks:**
- HTTPS is slightly slower than SSH
- Token needs to be stored somewhere

## Recommended Combined Approach

For best security and convenience:

### On Remote Computer

1. **Use password-protected keys for everything**
2. **Use SSH ControlMaster for chi-big** (Level 3)
3. **Start ssh-agent at beginning of work session** (Level 1)
4. **Optional: Use GitHub CLI for GitHub access** (Level 4)

**~/.ssh/config:**
```ssh-config
# GitHub - password-protected key
Host github.com
    IdentityFile ~/.ssh/id_ed25519
    AddKeysToAgent yes

# chi-big - password-protected key with ControlMaster
Host chi-big
    HostName colB0idHomeEnd  # or IP address
    User b0id
    IdentityFile ~/.ssh/id_ed25519_chi_big  # Separate key (optional)
    AddKeysToAgent yes

    # Connection reuse
    ControlMaster auto
    ControlPath ~/.ssh/sockets/%r@%h-%p
    ControlPersist 4h

    # Keep alive
    ServerAliveInterval 60
    ServerAliveCountMax 3
```

**Session startup script:**
```bash
# ~/start-work-session.sh
#!/bin/bash

echo "Starting work session..."

# Create socket directory if needed
mkdir -p ~/.ssh/sockets

# Start ssh-agent with 6-hour timeout
eval $(ssh-agent -t 21600)

# Add keys (will prompt for passwords)
echo "Adding GitHub key..."
ssh-add ~/.ssh/id_ed25519

echo "Adding chi-big key..."
ssh-add ~/.ssh/id_ed25519_chi_big

# Test connections
echo -e "\nTesting GitHub connection..."
ssh -T git@github.com

echo -e "\nTesting chi-big connection..."
ssh chi-big hostname

echo -e "\nSession ready! Keys will expire in 6 hours."
echo "Loaded keys:"
ssh-add -l
```

### On chi-big (Build Machine)

1. **Use password-protected keys**
2. **Start ssh-agent at login** (if pushing from chi-big)

**~/.bashrc or ~/.zshrc:**
```bash
# Start ssh-agent on login if not already running
if [ -z "$SSH_AUTH_SOCK" ]; then
    eval $(ssh-agent -s) > /dev/null
    # Optionally auto-add keys (will prompt for password)
    # ssh-add ~/.ssh/id_ed25519 2>/dev/null
fi
```

## Audit Your Current Keys

### Check existing keys:
```bash
ls -la ~/.ssh/
```

### Check which keys have passwords:
```bash
# This will prompt for password if key is encrypted
# Press Ctrl+C to cancel
ssh-keygen -y -f ~/.ssh/id_ed25519
```

### List keys in ssh-agent:
```bash
ssh-add -l
```

### Remove all keys from agent:
```bash
ssh-add -D
```

## Key Rotation

Periodically rotate your SSH keys (every 6-12 months):

```bash
# Generate new key
ssh-keygen -t ed25519 -f ~/.ssh/id_ed25519_new -C "your@email"

# Add new key to GitHub
cat ~/.ssh/id_ed25519_new.pub
# Paste into GitHub settings

# Add new key to chi-big
ssh-copy-id -i ~/.ssh/id_ed25519_new.pub chi-big

# Test new key
ssh -i ~/.ssh/id_ed25519_new chi-big hostname
git -c core.sshCommand="ssh -i ~/.ssh/id_ed25519_new" push origin dev-build

# If working, update config and remove old key
mv ~/.ssh/id_ed25519 ~/.ssh/id_ed25519_old
mv ~/.ssh/id_ed25519_new ~/.ssh/id_ed25519
```

## Troubleshooting

### "Permission denied (publickey)"
```bash
# Check which key is being used
ssh -v chi-big

# Check if key is in agent
ssh-add -l

# Add key to agent
ssh-add ~/.ssh/id_ed25519
```

### "Could not open a connection to your authentication agent"
```bash
# Start ssh-agent
eval $(ssh-agent)

# Add keys
ssh-add ~/.ssh/id_ed25519
```

### Keys not persisting between terminal sessions

ssh-agent runs per session. Options:
1. **Use shell integration** (start agent in .bashrc)
2. **Use keychain utility** (persists across sessions)
3. **Use systemd user service** (start agent at login)

### Forgot if key has password

```bash
# Try to extract public key - will prompt for password if protected
ssh-keygen -y -f ~/.ssh/id_ed25519 > /tmp/test_pub
# If no password prompt, key is passwordless
```

## Security Checklist

- [ ] All SSH keys are password-protected
- [ ] SSH keys are mode 600 (`chmod 600 ~/.ssh/id_*`)
- [ ] ~/.ssh directory is mode 700 (`chmod 700 ~/.ssh`)
- [ ] ssh-agent has timeout configured
- [ ] ControlMaster is configured for frequent connections
- [ ] Separate keys for different purposes (optional but recommended)
- [ ] Keys are rotated every 6-12 months
- [ ] Old/unused keys are removed from ~/.ssh and from authorized_keys on servers

## Further Reading

- [GitHub SSH Key Security](https://docs.github.com/en/authentication/connecting-to-github-with-ssh)
- [SSH Agent Forwarding Risks](https://matrix.org/blog/2019/05/08/post-mortem-and-remediations-for-apr-11-security-incident)
- [OpenSSH Certificate Authentication](https://www.lorier.net/docs/ssh-ca)
