{
  "targets": [
    {
      "target_name": "cryptoAddon",
      "sources": ["src/addon/cryptoAddon.cpp"],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "libraries": ["-L/usr/local/opt/openssl/lib", "-lssl", "-lcrypto"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
    }
  ]
}