#!/bin/bash
set -e

# Configuration
REMOTE_HOST="chi-big"
REMOTE_DIR="~/repo"

echo "Running tests on $REMOTE_HOST..."
ssh $REMOTE_HOST "cd $REMOTE_DIR && cd build && ctest --output-on-failure"
