# USRT

USRT is a de novo ligand deduplication and clustering algorithm using Ultrafast Shape Recognition with Torsions. It is inspired by [USR], and is hosted on GitHub at https://github.com/HongjianLi/usrt.


## Supported operating systems and compilers

* Arch Linux 3.13.11 x86_64 and clang 3.4
* Windows 7 SP1 x64 and Visual Studio 2013 Update 1


## Compilation

### Compilation on Linux

The Makefile uses clang as the default compiler. To compile,

    make

One may modify the Makefile to use a different compiler or different compilation options.

The generated objects will be placed in the `obj` folder, and the generated executable will be placed in the `bin` folder.

### Compilation on Windows

Visual Studio 2013 solution and project files are provided. To compile,

    msbuild /t:Build /p:Configuration=Release

Or one may open `usrt.sln` in Visual Studio 2013 and do a full rebuild.

The generated objects will be placed in the `obj` folder, and the generated executable will be placed in the `bin` folder.


## Usage

The `examples` folder contains example ligands in PDBQT format. For example, to extract the features of docked poses of ZINC00968327,

    bin/usrt < examples/ZINC00968327.pdbqt


## Change Log

### 1.0 (under construction)

* Initial release.


## Author

[Jacky Lee]


## License

[Apache License 2.0]


[USR]: http://dx.doi.org/10.1002/jcc.20681
[Jacky Lee]: http://www.cse.cuhk.edu.hk/~hjli
[Apache License 2.0]: http://www.apache.org/licenses/LICENSE-2.0.html
