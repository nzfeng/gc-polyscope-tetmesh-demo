# Contact patch transfer demo

![teaser](images/teaser.png?raw=true)

# Getting started

This demo takes the following as input:
* a tetmesh file

```
mkdir build
cd build
cmake ..
make -j8	# or however many cores you have
bin/main <path_to_tetmesh>
```

Some example data is in the `data/` directory. For example, to test out the hand mesh,

```
bin/main ../data/hand.tet
```

Also try out the `volumes_and_views` branches of Polyscope to get enhanced options for volume meshes. I believe Thomas Carey added smoother slice plane, isosurface visuals.