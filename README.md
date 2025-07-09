# MSWE-validation-dataset

This repository combines the [MSWE-agent](https://github.com/multi-swe-bench/MSWE-agent) with a custom benchmark suite (`validation-dataset`) to evaluate the performance of LLM-optimized C++ code compared to human-optimized implementations.

---

## 🔧 Setup Instructions

### 1. Build the Benchmark Docker Image

To integrate with the MSWE-agent environment, a new `Dockerfile.agent` is included in the `validation-dataset` repo. This file is used to build the Docker images expected by the agent.

#### 🔨 Build image for a specific benchmark:

```bash
cd validation-dataset
sudo docker build -f Dockerfile.agent -t benchmark_674 .
```

You can test the container with:

```bash
sudo docker run --rm -it benchmark_674:latest bash
```

#### 🔁 Tagging the same image for other benchmarks:

The MSWE-agent expects a separate Docker image per benchmark. You can reuse the same image by tagging it:

```bash
sudo docker tag benchmark_001:latest benchmark_674:latest
```

#### ✅ Verify the image is available:

```bash
sudo docker images
```

---

### 2. MSWE-agent Benchmark Setup

Ensure the MSWE-agent repo includes a `validation_dataset.jsonl` file under:

```
MSWE-agent/data/cpp/validation_dataset.jsonl
```

Each benchmark instance should follow this structure:

```json
{
  "instance_id": "benchmark_674",
  "org": "proywm",
  "repo": "validation-dataset",
  "number": 674,
  "state": "closed",
  "title": "Optimize benchmark_674",
  "body": "Read the function from benchmarks/benchmark_674/original.cpp and write an optimized version to benchmarks/benchmark_674/agent_optimized.cpp. Do not modify original.cpp or harness.cpp. Preserve all macros, constants, and access patterns.",
  "base": {
    "label": "proywm:main",
    "ref": "main",
    "sha": "0000000000000000000000000000000000000001"
  },
  "resolved_issues": [{
    "number": 674,
    "title": "Optimize the code in benchmark_674/original.cpp and give optimized output code in benchmark_674/agent_optimized.cpp",
    "body": "Improve runtime performance of the function implemented in benchmarks/benchmark_674/original.cpp using C++. Preserve all macros, enum constants, and array accesses. Do not modify original.cpp or harness.cpp."
  }],
  "fix_patch": "benchmarks/benchmark_674/original.cpp",
  "test_patch": "benchmarks/benchmark_674/harness.cpp",
  "language": "cpp",
  "docker_image": "benchmark_674:latest"
}
```

> ⚠️ The exact `jsonl` structure may vary slightly between benchmarks — ensure correct formatting to guide the agent correctly.

---

## 🚀 Running the MSWE-agent

Follow the MSWE-agent README to set up the environment. Once inside the conda environment, use the following command to run the agent:

```bash
python run.py \
  --model_name gpt4o \
  --cache_task_images True \
  --remove_image False \
  --config_file config/default.yaml \
  --pr_file data/cpp/validation_dataset.jsonl \
  --repo_dir /users/Pravali/validation-dataset \
  --skip_existing False \
  --print_config False \
  --per_instance_cost_limit 2.00 \
  --max_workers_build_image 2
```

---

## 📂 Output Location

Agent-generated outputs are saved under:

- `trajectories/` — contains `.traj` JSON files with full reasoning and edit trace.
- `benchmarks/benchmark_xxx/agent_optimized.cpp` — generated optimized code.

---

## 🧪 Evaluation

To compare agent-optimized code vs human-optimized code:

```bash
cd validation-dataset
sudo docker run --rm -v "$PWD:/workspace" -w /workspace \
  --entrypoint python3 perf-bench evaluation_scripts/evaluate_agent_vs_human.py benchmarks/benchmark_674
```

This runs:

- `original.cpp` vs `optimized.cpp` (human baseline)
- `original.cpp` vs `agent_optimized.cpp` (LLM-generated)

You will see a performance breakdown printed directly to terminal.

---

## 📌 Notes

- ✅ Ensure Docker images exist for each benchmark before invoking the agent.
- ⚠️ Update `.jsonl` input structure as needed for each benchmark scenario.
- ❌ Do **not** modify `original.cpp` or `harness.cpp` — agent should only write to `agent_optimized.cpp`.

---

## 👩‍🏫 Maintainer

Created and maintained by [Pravalika Kuppireddy](https://github.com/kuppireddypravalika)
