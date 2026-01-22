#!/bin/bash
set -e

# Configuration
REMOTE_HOST="home"  # SSH hostname for build machine (uses password-protected key)
REMOTE_DIR="~/repo"

echo "Running tests on $REMOTE_HOST..."
ssh $REMOTE_HOST "cd $REMOTE_DIR && cd build && ctest --output-on-failure"
