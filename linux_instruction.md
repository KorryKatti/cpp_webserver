1. change the cmakelist file to make it linux compatible
2. sudo pacman -S cmake gcc boost asio
3. ```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```
4. TADA

hell yeah, here's the ✨ *ninja edition* ✨ you can slap on top:


5. (optional but fast af) install ninja

```bash
sudo pacman -S ninja
```

6. generate build files with ninja instead of make

```bash
cmake -G Ninja ..
```

7. build it — lightning round ⚡

```bash
ninja
```

8. run it

```bash
./server
```

---

### 🔁 ninja protip:

to rebuild after any code change:

```bash
ninja
```

it’ll *only* recompile changed files, way faster than `make`.

---


---
# FROM CHATGPT


---

# 🚀 Crow C++ Web Server on Arch Linux (VS Code + Ninja Edition)

Crow is a fast, expressive C++ web microframework. Here's how to run it on Arch 

---

## 🧰 Prereqs

Install core deps:

```bash
sudo pacman -S cmake gcc boost asio ninja
```

Install [VS Code](https://code.visualstudio.com/) and add extensions:

* `CMake Tools` (by Microsoft)
* (optional) `CMake` syntax highlighter

---

## 📂 Project Structure

```
project-root/
├── main.cpp
├── CMakeLists.txt
├── crow/
│   └── include/...
├── asio/
│   └── asio/include/...
├── build/
└── .vscode/
```

---

## ✍️ Your `main.cpp`

```cpp
#include "crow.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "testing a very small change i or did ?";
    });

    app.port(18080).multithreaded().run();
}
```

---

## ⚙️ Your `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.14)
project(crow_server)

set(CMAKE_CXX_STANDARD 17)

include_directories(crow/include)
include_directories(asio/asio/include)

add_executable(server main.cpp)
```

---

## 🧠 VS Code Setup

### 🔧 Step 1: Enable Ninja

Create `.vscode/settings.json`:

```json
{
  "cmake.generator": "Ninja",
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "cmake.sourceDirectory": "${workspaceFolder}"
}
```

---

### 🧰 Step 2: Configure CMake Kits

Press `Ctrl+Shift+P` → `CMake: Edit User-Local CMake Kits` → paste:

```json
[
  {
    "name": "GCC 15.1.1 x86_64-pc-linux-gnu",
    "compilers": {
      "C": "/usr/bin/gcc",
      "CXX": "/usr/bin/g++"
    },
    "isTrusted": true
  },
  {
    "name": "Clang 20.1.6 x86_64-pc-linux-gnu",
    "compilers": {
      "C": "/usr/bin/clang",
      "CXX": "/usr/bin/clang++"
    },
    "isTrusted": true
  }
]
```

Then, bottom blue bar → click kit selector → pick `GCC` or `Clang`.

---

## 🏗️ Build & Run

### ✅ First Time:

* `F7` or click **Build** button
* `Ctrl+F5` or click **Run** button

### ✅ On Code Changes:

* just hit `F7` to recompile instantly via Ninja

---

## 🎯 Optional: Launch Config

Create `.vscode/launch.json` for debug/run:

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Run Crow Server",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/server",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "MIMode": "gdb"
    }
  ]
}
```

---

## 🧪 Test

```bash
./build/server
```

Visit [http://localhost:18080](http://localhost:18080)

---

## 💨 TL;DR

| Step         | Command / Action                            |
| ------------ | ------------------------------------------- |
| Install deps | `sudo pacman -S cmake gcc boost asio ninja` |
| Setup CMake  | generator = Ninja, build dir = `./build`    |
| Build        | `F7` or click Build                         |
| Run          | `Ctrl+F5` or click Run                      |
| Rebuild fast | `ninja` in `build/` dir                     |

---
