clang++ -dynamiclib -fPIC -std=c++14 -fno-exceptions -O3 -msse4.1 \
-D ROVE_GL -D ROVE_CL -D ROVE_MAC \
-F /Library/Frameworks \
-framework OpenGL -framework OpenCL \
-Wno-unknown-escape-sequence \
-o ../unity_projects/ExampleProject/Assets/Rove/ImportedPlugins/x86_64/RoveGLCL.bundle \
../src/rove_api.cpp ../src/compute/cl/cl_setup.cpp \
../src/compute/cl/cl_compute.cpp ../src/scene/mesh.cpp \
../src/scene/scene.cpp ../src/scene/tree.cpp ../src/scene/material.cpp

clang++ -dynamiclib -fPIC -std=c++14 -fno-exceptions -O3 -msse4.1 \
-D ROVE_GL -D ROVE_CUDA -D ROVE_MAC \
-I /Developer/NVIDIA/CUDA-8.0/include \
-F /Library/Frameworks \
-framework OpenGL -framework CUDA \
-Wno-unknown-escape-sequence \
-o ../unity_projects/ExampleProject/Assets/Rove/ImportedPlugins/x86_64/RoveGLCUDA.bundle \
../src/rove_api.cpp ../src/compute/cuda/cuda_setup.cpp \
../src/compute/cuda/cuda_compute.cpp ../src/scene/mesh.cpp \
../src/scene/scene.cpp  ../src/scene/tree.cpp ../src/scene/material.cpp
