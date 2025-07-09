import os
import json
import time
from urllib import request, error
import http.client
import logging
logging.basicConfig(level=logging.INFO)

# Thin wrapper around the OpenAI chat completion API used by the helper scripts.


class ContextLengthError(RuntimeError):
    """Exception raised when the prompt exceeds the model's context length."""
    pass

API_URL = "https://api.openai.com/v1/chat/completions"


def call_llm(
    prompt: str,
    model: str = "gpt-4o-mini",
    *,
    api_key: str | None = None,
    temperature: float | None = None,
    timeout: float = 30.0,
    max_retries: int = 3,
    backoff: float = 2.0,
) -> str:
    """Send a prompt to the LLM and return the response text.

    Parameters
    ----------
    prompt:
        The prompt text to send to the model.
    model:
        Model name to use when querying the API.
    api_key:
        Optional API key. If not provided ``OPENAI_API_KEY`` will be used.
    temperature:
        Sampling temperature. ``None`` uses the API default.
    timeout:
        Request timeout in seconds.
    max_retries:
        Number of times to retry failed requests.
    backoff:
        Exponential backoff factor between retries.
    """
    api_key = api_key or os.getenv("OPENAI_API_KEY")
    if not api_key:
        raise RuntimeError("OPENAI_API_KEY not provided")

    payload = {
        "model": model,
        "messages": [{"role": "user", "content": prompt}],
    }
    if temperature is not None:
        payload["temperature"] = temperature
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json",
    }

    req = request.Request(
        API_URL,
        data=json.dumps(payload).encode("utf-8"),
        headers=headers,
        method="POST",
    )

    for attempt in range(1, max_retries + 1):
        try:
            logging.info(f"LLM request attempt {attempt}/{max_retries}")
            with request.urlopen(req, timeout=timeout) as resp:
                data = json.loads(resp.read())
            break
        except error.HTTPError as exc:
            logging.error(f"HTTPError: {exc.code} {exc.reason}")
            try:
                detail = exc.read().decode()
            except Exception:
                detail = ""
            lower_detail = detail.lower()
            if (
                "context_length_exceeded" in lower_detail
                or "maximum context length" in lower_detail
            ):
                raise ContextLengthError(detail) from exc
            if attempt == max_retries or exc.code < 500:
                raise RuntimeError(
                    f"Failed to call LLM: {exc.code} {exc.reason} {detail}"
                ) from exc
        except (error.URLError, TimeoutError, http.client.RemoteDisconnected) as exc:
            if attempt == max_retries:
                raise RuntimeError(f"Failed to call LLM: {exc}") from exc
        print(f"Retrying in {backoff ** (attempt - 1)} seconds...")
        time.sleep(backoff ** (attempt - 1))

    return data["choices"][0]["message"]["content"]

