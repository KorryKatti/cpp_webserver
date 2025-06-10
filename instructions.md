# ⚙️ Crow C++ Web Server (Windows/MSVC Setup)

This is a quickstart guide to build a C++ web server using [Crow](https://github.com/CrowCpp/Crow) on Windows with MSVC. Ideal for local dev and future Dockerizing.

---

## 🔧 Requirements

- [MSYS2](https://www.msys2.org/)
- MSVC (comes with Visual Studio)
- CMake
- Git

---

## 🛠️ Setup Steps

### 1. Install Toolchain (via MSYS2)

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake git
```

Optional: Add `C:\msys64\mingw64\bin` to your system PATH.

---

### 2. Install Boost (header-only)

```bash
pacman -S mingw-w64-x86_64-boost
```

---

### 3. Clone Crow

```bash
git clone https://github.com/CrowCpp/Crow.git crow
```

---

### 4. Download Standalone ASIO

Get from: https://think-async.com/Asio/Download.html  
Extract like this:

```
guestbook/
├── asio/
│   └── asio/
│       └── include/
│           └── asio.hpp
```

---

### 5. Folder Structure

```
guestbook/
├── asio/
├── crow/
├── build/
├── main.cpp
└── CMakeLists.txt
```

---

### 6. Write CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.14)
project(crow_server)

set(CMAKE_CXX_STANDARD 17)

include_directories(crow/include)
include_directories(asio/asio/include)
include_directories("C:/msys64/mingw64/include") # boost (optional)

add_executable(server main.cpp)
```

---

### 7. Build (MSVC)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

If successful, `server.exe` will be generated inside `build/Debug/` (or just `build/` if using MinGW).

---

## 🚀 Run It

```bash
cd build/Debug
./server.exe
```

Visit [http://localhost:18080](http://localhost:18080)

---

## 🔁 Rebuilding After Changes

If you only changed `main.cpp`:

```bash
cmake --build .
```

No need to re-run `cmake ..` unless:
- You changed `CMakeLists.txt`
- You added/removed source files

---

## 🧯 Troubleshooting

### ❌ asio.hpp not found?

- Download standalone ASIO
- Ensure it's extracted to: `asio/asio/include/asio.hpp`
- Ensure CMakeLists has:  
  ```cmake
  include_directories(asio/asio/include)
  ```

---



# CHATGPT YAPPING INSTRUCTIONS

yup, you got it — **just the `.exe` from `build/Debug/`** (or `build/Release/` if you build in release mode) is enough **for running**, *but* there’s a catch:

---

## 🧠 what to share

### ✅ for local testing or friends (Windows)

just share:

```
build/Debug/server.exe
```

⚠️ BUT — they must have:

* the **same MSVC runtime** installed (or your exe will error out with DLL issues)
* same architecture (x64)

---

### 🧳 if you want it to "just work" everywhere

bundle the runtime:

#### ✅ option 1: **statically link MSVC runtime**

edit `CMakeLists.txt`:

```cmake
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
```

this bakes in the MSVC runtime → the `.exe` works standalone
(no `MSVCP140.dll` missing bs)

---

#### ✅ option 2: **use Docker (later)**

you’ll have full control over:

* compiler
* OS env
* libs

just wrap it in a small image like `mcr.microsoft.com/windows/nanoserver` or Alpine (if cross-compiling via mingw-w64)

---

### ❌ what NOT to share

don’t ship:

* the whole `build/` folder (too messy, compiler-specific)
* source files unless open source
* `.vcxproj`, `.sln`, `.pdb`, etc.

---

### 👁️‍🗨️ TL;DR

| Goal              | Share this                     | Notes                            |
| ----------------- | ------------------------------ | -------------------------------- |
| Friend testing    | `server.exe` only              | Must have same MSVC runtime      |
| Safe standalone   | Statically-linked `server.exe` | Add `CMAKE_MSVC_RUNTIME_LIBRARY` |
| Dev collaboration | source + `.gitignore` setup    | Standard git workflow            |
| Prod deployment   | Docker container               | Fully reproducible               |

---
