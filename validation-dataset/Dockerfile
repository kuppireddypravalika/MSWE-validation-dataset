FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Update repositories and handle potential package update issues robustly
RUN apt-get update  -o Acquire::Retries=3 && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
      build-essential \
      g++ \
      python3 \
      python3-pip \
      libeigen3-dev \
      libboost-all-dev && \
    pip3 install numpy && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . /workspace

RUN chmod +x evaluation_scripts/*.py

ENTRYPOINT ["python3", "evaluation_scripts/build_and_validate.py"]
