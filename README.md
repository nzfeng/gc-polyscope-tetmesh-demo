# Manipulating tet meshes with geometry-central and Polyscope

![teaser](data/teaser.png?raw=true)

# Getting started

Clone:
```
git clone --recursive https://github.com/nzfeng/gc-polyscope-tetmesh-demo
```

This demo takes the following as input:
* a tetmesh file

```
mkdir build
cd build
cmake ..
make -j8	# or however many cores you have
bin/main <path_to_tetmesh>
```

Some example data is in the `data/` directory. For example, to test out the armadillo mesh (which has meshed the inside/outside of an armadillo), run

```
bin/main ../data/armadillo.mesh
```
You can add slice planes, etc. by hitting the `Options` button in the Polyscope GUI.

# Notes

This demo uses the `volumes_and_views2` branch of Polyscope. I believe Thomas Carey added smoother slice plane, isosurface visuals. 

It seems like libigl only has support reading from .mesh and .msh files (the latter being output from TetGen.)
