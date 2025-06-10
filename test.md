1. MINGW SETUp

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake git
```

2. BOOST INSTALL

```bash
pacman -S mingw-w64-x86_64-boost
```

3. GIT CLONE

```bash
git clone https://github.com/CrowCpp/Crow.git crow
```

4. BUILD

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
( if error here : https://cmake.org/download/) and then (cmake --version)

5. IF ERROR IN STEP 4


download : https://sourceforge.net/projects/asio/files/asio/1.30.2%20%28Stable%29/

```css
guestbook/
├── asio/
│   └── asio/
│       └── include/
│           └── asio.hpp
├── crow/
├── main.cpp
└── CMakeLists.txt
```

g++ --version

```bash
cd build
cmake ..           # regenerates with MSVC
cmake --build .    # builds with MSBuild
```

then in build/Debug you should find server.exe , run it



#### more instructions

1. If changes in main.cpp only
```bash
cmake --build .
```