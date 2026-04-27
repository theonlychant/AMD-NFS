"""Simple CLI to run the Python ML session locally."""
# For AMD-Native Inference, Author: theonlychant, 2026, Advanced Micro Devices, Inc. All rights reserved.
import argparse
from .session import MLInferenceSession, get_tokenizer

def main():
    p = argparse.ArgumentParser()
    p.add_argument("prompt", nargs="+", help="prompt words")
    p.add_argument("--model", default="stub", help="model path")
    args = p.parse_args()

    sess = MLInferenceSession()
    sess.load_model(args.model)
    prompt = " ".join(args.prompt)
    out = sess.generate(prompt, max_tokens=64)
    print(out)

if __name__ == "__main__":
    main()
