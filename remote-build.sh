#!/bin/bash
set -e

# Configuration
REMOTE_HOST="home"  # SSH hostname for build machine (uses password-protected key)
REMOTE_DIR="~/repo"
BRANCH="dev-build"

# Auto-commit WIP
if [[ -n $(git status -s) ]]; then
    echo "Auto-committing changes..."
    git add .
    git commit -m "WIP: $(date +%Y-%m-%d\ %H:%M:%S)" --no-verify
fi

# Push to GitHub (origin)
echo "Pushing to GitHub..."
git push origin HEAD:$BRANCH

# Trigger remote build via SSH
echo "Triggering remote build on $REMOTE_HOST..."
ssh $REMOTE_HOST "cd $REMOTE_DIR && git pull origin $BRANCH && cmake --build build -j\$(nproc)"
