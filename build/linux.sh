clang++ -shared -fPIC -std=c++14 -fno-exceptions -O3 -msse4.1 \
-D ROVE_GL -D ROVE_CL -D ROVE_LINUX \
-lGL -lOpenCL -lGLEW \
-Wno-unknown-escape-sequence \
-o ../unity_projects/ExampleProject/Assets/Rove/ImportedPlugins/x86_64/libRoveGLCL.so \
../src/rove_api.cpp ../src/compute/cl/cl_setup.cpp \
../src/compute/cl/cl_compute.cpp ../src/scene/mesh.cpp \
../src/scene/scene.cpp ../src/scene/tree.cpp ../src/scene/material.cpp

clang++ -shared -fPIC -std=c++14 -fno-exceptions -O3 -msse4.1 \
-D ROVE_GL -D ROVE_CUDA -D ROVE_LINUX \
-I /opt/cuda/include \
-L /opt/cuda/lib64 \
-lGL -lcuda -lGLEW \
-Wno-unknown-escape-sequence \
-o ../unity_projects/ExampleProject/Assets/Rove/ImportedPlugins/x86_64/libRoveGLCUDA.so \
../src/rove_api.cpp ../src/compute/cuda/cuda_setup.cpp \
../src/compute/cuda/cuda_compute.cpp  ../src/scene/mesh.cpp \
../src/scene/scene.cpp ../src/scene/tree.cpp ../src/scene/material.cpp
