# Bohr: High-Performance Real-Time Molecular Surface Renderer

[![License: GPL](https://img.shields.io/badge/License-GPL-yellow.svg)](./LICENSE.md)
[![build](https://img.shields.io/badge/build-passing-brightgreen)]()
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![OpenGL](https://img.shields.io/badge/OpenGL-3.x-brightgreen)
![Linux](https://img.shields.io/badge/platform-Linux-blue?logo=linux)

---

## 🚀 Overview

**Bohr** is a high-performance C++/C application for real-time interactive visualization of molecular surfaces, leveraging the **Marching Cubes** algorithm and modern OpenGL. Load molecular structures from `.pdb` files and explore their van der Waals surfaces, rendered on-the-fly with physically meaningful representations — ideal for computational chemistry, bioinformatics, educational, or research purposes.

---

## 📜 Table of Contents

- [Features](#features)
- [Scientific Motivation](#scientific-motivation)
- [Implementation & Architecture](#implementation--architecture)
- [Screenshots / Results](#screenshots--results)
- [Installation](#installation)
- [Usage](#usage)
- [Documentation](#documentation)
- [Project Structure](#project-structure)
- [Demo](#demo)
- [FAQ / Support](#faq--support)
- [Contributing](#contributing)
- [License & Citations](#license--citations)
- [Acknowledgements](#acknowledgements)

---

## ✨ Features

- ⚡️ **Efficient C/C++17 Core**: Real-time molecular visualization using Marching Cubes.
- 🔬 **PDB Loader**: Parses and visualizes standard `.pdb` molecular structure files.
- 🎨 **OpenGL 3.x Rendering**: Physically convincing van der Waals surface exploration via modern rendering pipelines.
- 🧑‍💻 **Interactive Exploration**: Pan, rotate, and zoom surfaces in real time.
- 🚦 **Parallelized Backend**: Multi-threading with POSIX support for fast marching and rendering computations.
- 🛠️ **Cross-library Power**: Integrates GLEW, GLFW, FreeType, and X11 extensions (Linux).
- 🪢 **Extensible Design**: Modular codebase for research and further development.

---

## 🧬 Scientific Motivation

Chemical and biological processes are intimately linked with molecular shapes and interactions. By visualizing van der Waals surfaces—an accessible 3D representation of molecular boundaries—scientists and students can better understand reactivity, docking, and biochemical properties.

> **Foundational Reference:**  
> Raposo, A. N., & Gomes, A. J. P. (2019).  
> “Pi-surfaces: products of implicit surfaces towards constructive composition of 3D objects.”  
> [arXiv:1906.06751 [cs.GR]](https://arxiv.org/abs/1906.06751)

*(Expand here based on your PDF: objectives, real use cases, educational impact, etc. Paste in your specific intro/justification if you want.)*

---

## 🏗️ Implementation & Architecture

Bohr is composed of the following modules:

- **PDB Parser** (`src/pdb/`): Extracts atomic coordinates, radii, and element info from Protein Data Bank files.
- **Surface Generator** (`src/core/`): Implements the Marching Cubes algorithm; multi-threaded for large molecules.
- **Renderer** (`src/opengl/`): Uses Modern OpenGL (GLEW, GLFW) for fast isosurface rendering, supporting camera controls and shading.
- **UI Layer**: (optional) Handles window system, user input, and visual overlays.

**Key Libraries**  
GLEW, GLFW3, FreeType, POSIX threads, X11/XRandR (for Linux builds)

*(Expand here with architecture diagrams, data flow, or unique implementation details from your pdf. You can also add benchmarks, e.g., fps for typical molecules, memory profiling, or optimization tricks.)*

---

## 📸 Screenshots / Results

> *Please add/render some sample images here!*
> 
> ![Rendering Example](doc/screenshots/bohr_sample1.png)
> ![Another Example](doc/screenshots/bohr_sample2.png)
>
> *(Or paste benchmark/visualization results, "Figure 1" or result images from your PDF if available.)*

---

## 🛠️ Installation

```bash
# Clone the repo
git clone https://github.com/AshKetshup/Bohr.git
cd Bohr

# Build for release (Linux)
make release     # or: make debug
```

### Prerequisites

| Dependency | Version/Notes      |
|------------|-------------------|
| g++        | 7.0+ (C++17)      |
| make       | UNIX standard     |
| OpenGL     | 3.x+              |
| GLEW       | Development libs  |
| GLFW3      | Development libs  |
| FreeType   | Development libs  |
| X11/XRandR | Linux/Xorg only   |

---

## ▶️ Usage

```bash
./bin/bohr path/to/file.pdb
```

**Command-line options**:
- `-h` / `--help`: Show usage.
- `--threads N`: Specify thread count.
- *(Add/describe all CLI arguments supported as per your report.)*

---

## 📖 Documentation

- Core source: [`src/`](./src)
- Documentation: [`docs/`](./docs)

---

## 🗂️ Project Structure

```
.
├── src/         # C/C++ sources
├── include/     # Public/header files
├── bin/         # Compiled executables
├── doc/         # Documentation & reports
│   └── relatorio-projeto.pdf
└── README.md
```

[//]: <> (---)

[//]: <> (## 🎥 Demo)

[//]: <> (Add a GIF, mp4, or YouTube link to a screen recording of the live app here!)

---

## 🙋 FAQ / Support

- For questions, bug reports, or ideas: [Open an Issue](https://github.com/AshKetshup/Bohr/issues)

---

## 🤝 Contributing

Contributions are welcome!  
See [`CONTRIBUTING.md`](./CONTRIBUTING.md) for instructions.

---

## 📚 License & Citations

Distributed under the [MIT License](./LICENSE.md).

If you use this work, please cite the algorithmic basis:
```
@article{raposo2019pisurfaces,
  title={Pi-surfaces: products of implicit surfaces towards constructive composition of 3D objects},
  author={Raposo, Adriano N and Gomes, Abel JP},
  journal={arXiv preprint arXiv:1906.06751},
  year={2019}
}
```

---

## 🙏 Acknowledgements

- [Raposo & Gomes, 2019](https://arxiv.org/abs/1906.06751) for theoretical foundation.
- OpenGL, GLEW, GLFW, FreeType, X11
- Project contributors & testers.

[//]: <> (Add your team, supervisor, funding, or other credits as in your PDF.)
