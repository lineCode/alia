#!/bin/bash
# Build the documentation in the "www" directory.
# Note that the asm-dom examples must be built in examples/asm-dom/build before
# running this script.
set -e
mkdir -p www
cd www
rm -rf *
cp ../docs/* .
unzip favicon_io.zip && rm favicon_io.zip
cp ../examples/asm-dom/build/*.js ../examples/asm-dom/build/*.wasm .
cp ../examples/asm-dom/demos/* .