#!/bin/bash
# Build the documentation in the "www" directory.
# Note that the asm-dom examples must be built in examples/asm-dom/build before
# running this script.
set -e
mkdir -p www
cp docs/* www
cp examples/asm-dom/build/*.js examples/asm-dom/build/*.wasm www
cp examples/asm-dom/snippets/* www
