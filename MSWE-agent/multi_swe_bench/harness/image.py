from dataclasses import dataclass
from typing import Optional, Union
from pathlib import Path

from multi_swe_bench.harness.pull_request import PullRequest


@dataclass
class File:
    dir: str
    name: str
    content: str


@dataclass
class Config:
    need_clone: bool
    global_env: Optional[dict[str, str]]
    clear_env: bool


class Image:
    def __lt__(self, other: "Image") -> bool:
        return self.image_full_name() < other.image_full_name()

    def __repr__(self) -> str:
        return self.image_full_name()

    def __hash__(self):
        return hash(self.image_full_name())

    def __eq__(self, other):
        if not isinstance(other, Image):
            return NotImplemented
        return self.image_full_name() == other.image_full_name()

    @property
    def pr(self) -> PullRequest:
        raise NotImplementedError

    @property
    def config(self) -> Config:
        raise NotImplementedError

    @property
    def global_env(self) -> str:
        if not self.config.global_env:
            return ""
        return "\n".join(
            [f"ENV {key}={value}" for key, value in self.config.global_env.items()]
        )

    @property
    def need_copy_code(self) -> bool:
        if isinstance(self.dependency(), str) and not self.config.need_clone:
            return True
        return False

    @property
    def clear_env(self) -> str:
        if not self.config.clear_env or not self.config.global_env:
            return ""
        return "\n".join([f'ENV {key}=""' for key in self.config.global_env.keys()])

    def dependency(self) -> Union[str, "Image"]:
        return NotImplementedError

    def image_full_name(self) -> str:
        return f"{self.image_name()}:{self.image_tag()}"

    def image_name(self) -> str:
        raise NotImplementedError

    def image_tag(self) -> str:
        raise NotImplementedError

    def workdir(self) -> str:
        raise NotImplementedError

    def files(self) -> list[File]:
        raise NotImplementedError

    def fix_patch_path(self) -> str:
        return "/home/fix.patch"

    def dockerfile_name(self) -> str:
        return "Dockerfile"

    def dockerfile(self) -> str:
        raise NotImplementedError

    @classmethod
    def from_docker_name(cls, name: str) -> "Image":
        if ":" in name:
            org, tag = name.split(":")
        else:
            org, tag = name, "latest"
        return SimpleImage(org=org, tag=tag, file=Path("Dockerfile"))


@dataclass
class SimpleImage(Image):
    org: str
    tag: str
    file: Path

    def dependency(self) -> str:
        return ""

    def image_name(self) -> str:
        return self.org

    def image_tag(self) -> str:
        return self.tag

    def workdir(self) -> str:
        return "/workspace"

    def files(self) -> list[File]:
        return []

    def dockerfile(self) -> str:
        return self.file.read_text()

    @property
    def config(self) -> Config:
        return Config(need_clone=False, global_env=None, clear_env=False)

    @property
    def pr(self) -> PullRequest:
        raise NotImplementedError("This image is not linked to a PR.")
