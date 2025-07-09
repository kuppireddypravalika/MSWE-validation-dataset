#!/bin/bash
# Usage: safe_push.sh <branch>
# Attempt to safely push current HEAD to the given branch, handling concurrent
# pushes by rebasing and retrying.

set -euo pipefail
BRANCH="$1"
MAX_RETRIES=5

for attempt in $(seq 1 $MAX_RETRIES); do
  git fetch origin "$BRANCH" || true
  if git show-ref --verify --quiet "refs/remotes/origin/$BRANCH"; then
    # Rebase on top of the latest remote branch to avoid conflicts
    git rebase "origin/$BRANCH" || { git rebase --abort; continue; }
  fi
  if git push origin "HEAD:$BRANCH"; then
    exit 0
  else
    echo "Push attempt $attempt failed. Retrying..." >&2
    sleep $((attempt * 5))
  fi
done

echo "Failed to push after $MAX_RETRIES attempts." >&2
exit 1
