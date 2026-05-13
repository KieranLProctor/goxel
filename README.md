# Goxel

**A modern voxel game engine built in C++23**

![Goxel Banner](https://via.placeholder.com/1200x400/0f172a/22d3ee?text=Goxel+-+Voxel+Engine)
<!-- Replace with actual gameplay screenshot or render when available -->

---

## Overview

**Goxel** is a high-performance, modern voxel engine written in **C++23**. It features a clean, modular architecture with a strong emphasis on performance, maintainability, and extensibility.

Designed with professional game engine principles in mind, Goxel provides a solid foundation for building immersive voxel-based worlds using modern OpenGL rendering techniques.

---

## Features

- **Modern C++23** with latest language features and best practices
- **Layered architecture** for flexible game state and UI management
- **Robust event system** with type-safe dispatching
- **Dear ImGui** integration with docking and professional tooling
- **Modern OpenGL 4.6 Core** rendering pipeline
- **High-performance math** using GLM
- Structured logging with **spdlog**
- Cross-platform support via **GLFW**
- Clean separation of core engine, renderer, and gameplay systems

---

## Tech Stack

| Component          | Technology                          |
|--------------------|-------------------------------------|
| Language           | C++23                               |
| Windowing & Input  | GLFW 3.4                            |
| Graphics           | OpenGL 4.6 + GLAD                   |
| UI Framework       | Dear ImGui (Docking)                |
| Mathematics        | GLM 1.0.1                           |
| Logging            | spdlog 1.14.1                       |
| Image Loading      | stb_image                           |
| Build System       | CMake 3.22+                         |

---

## Architecture

```text
assets/
├── shaders/
docs/
external/
├── glad/
├── stb/
src/
├── core/           # Application, Window, Events, Layer system, Renderer
├── game/           # Camera, World, Player, Voxel data structures
├── ui/             # Game layers and ImGui integration
├── utils/          # Utilities and system diagnostics
└── main.cpp
```

---

## Building from Source

### Prerequisites

CMake 3.22 or higher
C++23 compatible compiler (MSVC 2022+, GCC 14+, or Clang 16+)
Graphics drivers with OpenGL 4.6 support

### Quick Start

```bash
git clone https://github.com/KieranLProctor/goxel.git
cd goxel

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -j

# Run
./build/bin/goxel
```