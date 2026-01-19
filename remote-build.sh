#!/bin/bash
set -e

# Configuration
REMOTE_HOST="chi-big"
REMOTE_DIR="~/repo"
BRANCH="dev-build"

# Auto-commit WIP
if [[ -n $(git status -s) ]]; then
    echo "Auto-committing changes..."
    git add .
    git commit -m "WIP: Auto-commit for remote build"
fi

# Push to remote
echo "Pushing to $REMOTE_HOST..."
git push $REMOTE_HOST HEAD:$BRANCH --force

# Trigger remote build
echo "Triggering remote build..."
ssh $REMOTE_HOST "cd $REMOTE_DIR && git checkout $BRANCH && git reset --hard HEAD && cmake --build build -j$(nproc)"
