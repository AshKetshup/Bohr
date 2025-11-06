# Bohr

High-performance C++/C molecular renderer leveraging the Marching Cubes algorithm and Modern OpenGL.  
Bohr allows you to load a `.pdb` file describing a molecule structure and interactively explore and visualize its van der Waals surface, generated in real-time using Marching Cubes. The project draws inspiration from "Pi-surfaces: products of implicit surfaces towards constructive composition of 3D objects" by Adriano N. Raposo and Abel J. P. Gomes.

---

## ⭐ Features

- Efficient **Marching Cubes** implementation for isosurface extraction
- Parses and visualizes **.pdb molecular structures**
- Real-time **OpenGL** rendering
- Interactive exploration of molecular van der Waals surfaces
- Powered by **GLEW**, **GLFW**, **FreeType**, and a rich set of **X11** extensions (Linux)
- Multi-threaded backend with POSIX support
- Extensible and well-structured C++17/C foundation

---

## 📸 Screenshots

<!-- Please provide one or more project screenshots, e.g.: -->
<!-- ![Bohr Rendering Example](URL_TO_SCREENSHOT) -->

---

## 🥼 Scientific Basis

Visualization approach and surface rendering inspired by:
- Adriano N. Raposo and Abel J. P. Gomes,  
  "Pi-surfaces: products of implicit surfaces towards constructive composition of 3D objects",  
  arXiv:1906.06751 [cs.GR], 2019.  
  [Read on arXiv](https://arxiv.org/abs/1906.06751)

---

## 🌱 Getting Started

### Prerequisites

| Dependency | Version/Notes       |
|------------|--------------------|
| g++        | 7.0+ (C++17)       |
| make       | UNIX standard      |
| OpenGL     | 3.x+ preferred     |
| GLEW       | Development libs   |
| GLFW3      | Development libs   |
| FreeType   | Development libs   |
| X11/XRandR | Linux/Xorg only    |

### Linux Build Instructions

```bash
git clone https://github.com/AshKetshup/Bohr.git
cd Bohr
make release      # or: make debug
```

#### Running

```bash
./bin/bohr path/to/molecule.pdb
```

<!-- Please add info about CLI arguments, additional options, or file format support if needed. -->

---

## 📚 Documentation

- Main documentation: [docs/](./docs)
- Source code: [src/](./src)
- [External documentation/Wiki if any – please provide]

---

## 🎥 Demo

<!-- Provide a GIF/video or image sequence of the visualization. -->
<!-- ![Demo GIF](URL_TO_DEMO) -->

---

## 🙋 FAQ / Support

For questions, bug reports, or suggestions, please open an issue at [issues](https://github.com/AshKetshup/Bohr/issues).

---

## 🏗️ Contributing

Pull requests and feature suggestions are welcome!
See [CONTRIBUTING.md](./CONTRIBUTING.md) for how to help.

---

## 📜 License

Distributed under the [LICENSE.md](./LICENSE.md) (specify license type or link).

---

## 💡 Acknowledgements

- Adriano N. Raposo and Abel J. P. Gomes for algorithmic inspiration on Pi-Surfaces ([arXiv:1906.06751](https://arxiv.org/abs/1906.06751))
- OpenGL, GLEW, GLFW, FreeType, X11 projects
- [Any additional project or paper citations, assets or thanks? Please add.]

---

<!-- 
MISSING FROM ME: 
- Project logo/screenshot/demo images
- Specifications on .pdb parsing, CLI flags, additional supported file formats (if any)
- External documentation links, tutorials, or wikis (if available)
- Any other special thanks or dependencies to credit
Fill in those marked fields, and your README will be on par with MarchGL!
 -->
