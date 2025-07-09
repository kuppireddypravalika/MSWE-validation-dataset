import logging
import os

# Miscellaneous helper functions shared by the maintenance scripts.


def setup_logger(level=logging.INFO):
    """Configure basic logger."""
    logging.basicConfig(
        format="%(asctime)s [%(levelname)s] %(message)s",
        level=level,
    )


def ensure_dir(path: str):
    """Create directory if it doesn't already exist."""
    os.makedirs(path, exist_ok=True)


def read_text(path: str) -> str:
    """Return the contents of a text file."""
    with open(path, "r", encoding="utf-8") as f:
        return f.read()


def write_text(path: str, text: str) -> None:
    """Write text to a file, overwriting any existing content."""
    with open(path, "w", encoding="utf-8") as f:
        f.write(text)
