
# To build and run

```
cmake -DCMAKE_TOOLCHAIN_FILE=./wasm.cmake -S .. -B .

# Run SpiderMonkey
js --wasm-compiler=ion --wasm-simd-wormhole run-test-js.js

# Run Node.js
node --experimental-wasm-simd --wasm-simd-post-mvp run-test-node.js
```

