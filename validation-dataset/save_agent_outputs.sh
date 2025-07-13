#!/bin/bash

BENCH=$1
COST=$2   # e.g. 2.50

OUT_DIR=agent_outputs/gpt-4o/$BENCH
mkdir -p $OUT_DIR

# Copy outputs
cp benchmarks/$BENCH/agent_optimized.cpp $OUT_DIR/ 2>/dev/null || echo "❌ agent_optimized.cpp not found for $BENCH"
cp ../MSWE-agent/trajectories/Pravali/gpt4o__validation_dataset__default__t-0.00__p-0.95__c-${COST}__install-1/${BENCH}.traj $OUT_DIR/ 2>/dev/null || echo "❌ .traj file not found for $BENCH"

# Run evaluation (writes evaluation.json in OUT_DIR)
sudo docker run --rm -v "$PWD:/workspace" -w /workspace \
  --entrypoint python3 perf-bench evaluation_scripts/evaluate_agent_vs_human.py benchmarks/$BENCH
