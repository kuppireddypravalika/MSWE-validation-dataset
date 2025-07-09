#!/bin/bash

# Build and execute benchmarks inside the Docker environment.
IMAGE_64="perf-bench"
IMAGE_32="perf-bench-32"

# Pass along benchmarks arguments
BENCHMARK_ARGS="$@"

# Determine which benchmarks to check for '-m32'
if [ -z "$BENCHMARK_ARGS" ]; then
    SEARCH_DIR="benchmarks/"
else
    SEARCH_DIR="$BENCHMARK_ARGS"
fi

# Scan bench_config.json files for the -m32 flag. Use grep to avoid
# invoking any external dependencies. Skip check if no config files exist.
CONFIG_FILES=$(find "$SEARCH_DIR" -name bench_config.json 2>/dev/null | tr '\n' ' ')
if [ -n "$CONFIG_FILES" ] && grep -q "-m32" $CONFIG_FILES; then
    IMAGE="$IMAGE_32"
    echo "⚙️  32-bit benchmarks detected. Using 32-bit environment."
else
    IMAGE="$IMAGE_64"
    echo "⚙️  No 32-bit benchmarks detected. Using default 64-bit environment."
fi

echo "DEBUG host  GITHUB_STEP_SUMMARY = $GITHUB_STEP_SUMMARY"
docker build -t "$IMAGE" -f Dockerfile .

########################################
# Forward step‑summary into container  #
########################################
# The GitHub workflow sets GITHUB_STEP_SUMMARY to collect Markdown logs. When
# running locally this variable is usually unset.
if [[ -n "${GITHUB_STEP_SUMMARY:-}" ]]; then
  # Forward the env‑var *and* bind‑mount the summary file itself
  touch "$GITHUB_STEP_SUMMARY"
  DOCKER_ENV=(-e "GITHUB_STEP_SUMMARY=${GITHUB_STEP_SUMMARY}")
  DOCKER_VOL=(-v "${GITHUB_STEP_SUMMARY}:${GITHUB_STEP_SUMMARY}")
else
  # Local run: no special summary
  DOCKER_ENV=()
  DOCKER_VOL=()
fi

docker run --rm "${DOCKER_ENV[@]}" "${DOCKER_VOL[@]}" -v "$PWD:/workspace" "$IMAGE" $BENCHMARK_ARGS
