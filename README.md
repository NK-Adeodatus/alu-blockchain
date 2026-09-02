# alu-blockchain

A C implementation of core blockchain cryptographic primitives built on top of
OpenSSL, following the Holberton / ALU curriculum.

## Repository layout

```
holbertonschool-blockchain/
├── crypto/
│   ├── hblk_crypto.h          # Master header — all prototypes & types
│   ├── sha256.c               # Task 0 – SHA-256 wrapper
│   ├── ec_create.c            # Task 1 – EC key-pair generation
│   ├── ec_to_pub.c            # Task 2 – Extract raw public key
│   ├── ec_from_pub.c          # Task 3 – Rebuild EC_KEY from public key
│   ├── ec_save.c              # Task 4 – Persist key pair to disk (PEM)
│   ├── ec_load.c              # Task 5 – Load key pair from disk
│   ├── ec_sign.c              # Task 6 – ECDSA sign
│   ├── ec_verify.c            # Task 7 – ECDSA verify
│   ├── Makefile
│   ├── provided/
│   │   └── _print_hex_buffer.c
│   └── test/
│       ├── sha256-main.c
│       ├── ec_create-main.c
│       ├── ec_from_pub-main.c
│       ├── ec_load-main.c
│       ├── ec_save-main.c
│       ├── ec_sign-main.c
│       ├── ec_to_pub-main.c
│       └── ec_verify-main.c
└── README.md
```

## Dependencies

* **OpenSSL** (`libssl-dev` / `libcrypto`)

  ```bash
  sudo apt-get install libssl-dev
  ```

## Build

```bash
cd crypto
make          # builds libhblk_crypto.a
make sha256-test   # builds the Task-0 test binary
```

## Compilation flags

```
gcc -Wall -Wextra -Werror -pedantic -I. ... -lssl -lcrypto
```

## Tasks

| # | File | Description |
|---|------|-------------|
| 0 | `sha256.c` | Compute SHA-256 digest of an arbitrary byte sequence |
| 1 | `ec_create.c` | Generate a secp256k1 EC key pair |
| 2 | `ec_to_pub.c` | Serialise the public key to 65 raw bytes |
| 3 | `ec_from_pub.c` | Reconstruct an EC key from raw public-key bytes |
| 4 | `ec_save.c` | Save key pair as PEM files in a folder |
| 5 | `ec_load.c` | Load a key pair from PEM files |
| 6 | `ec_sign.c` | ECDSA-sign a message hash |
| 7 | `ec_verify.c` | ECDSA-verify a signature |

## Style

Code follows the **Betty** C style guide and is compiled with strict warnings
(`-Wall -Wextra -Werror -pedantic`).
