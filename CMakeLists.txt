cmake_minimum_required(VERSION 3.17)

project(RayTracer)

set(SRCS
    src/main.cpp
    src/Core.cpp
    src/Scene/Scene.cpp
    src/Camera/Camera.cpp
    src/Physics/LightUtils.cpp
    src/Primitives/MeshTriangle/MeshTriangle.cpp
    src/Primitives/Sphere/Sphere.cpp
    src/Primitives/Plane/Plane.cpp
    src/Parser/Object/ObjParser.cpp
    src/Parser/Config/CfgParser.cpp
    shared/math/analysis/analysis.cpp
    shared/math/Vectors/VecUtils.cpp
    shared/math/utils.cpp
    shared/math/MonteCarlo/MonteCarlo.cpp
    shared/math/Matrix/MatrixTransformation.cpp
)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++20 -O3 -g")
else()
    set(CMAKE_CXX_FLAGS "--std=c++20 -Wall -Wextra -lpthread -ffast-math -fauto-inc-dec -fbranch-count-reg -fcombine-stack-adjustments -fcompare-elim -fcprop-registers -fdce -fdefer-pop -fdelayed-branch -fdse -fforward-propagate -fguess-branch-probability -fif-conversion -fif-conversion2 -finline-functions-called-once -fipa-modref -fipa-profile -fipa-pure-const -fipa-reference -fipa-reference-addressable -fmerge-constants -fmove-loop-invariants -fomit-frame-pointer -freorder-blocks -fshrink-wrap -fshrink-wrap-separate -fsplit-wide-types -fssa-backprop -fssa-phiopt -ftree-bit-ccp -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-pta -ftree-scev-cprop -ftree-sink -ftree-slsr -ftree-sra -ftree-ter -funit-at-a-time  -falign-functions  -falign-jumps -falign-labels  -falign-loops -fcaller-saves -fcode-hoisting -fcrossjumping -fcse-follow-jumps  -fcse-skip-blocks -fdelete-null-pointer-checks -fdevirtualize  -fdevirtualize-speculatively -fexpensive-optimizations -ffinite-loops -fgcse  -fgcse-lm -fhoist-adjacent-loads -finline-functions -finline-small-functions -findirect-inlining -fipa-bit-cp  -fipa-cp  -fipa-icf -fipa-ra  -fipa-sra  -fipa-vrp -fisolate-erroneous-paths-dereference -flra-remat -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -fpeephole2 -freorder-blocks-algorithm=stc -freorder-blocks-and-partition  -freorder-functions -frerun-cse-after-loop -fschedule-insns  -fschedule-insns2 -fsched-interblock  -fsched-spec -fstore-merging -fstrict-aliasing -fthread-jumps -ftree-builtin-call-dce -ftree-loop-vectorize -ftree-pre -ftree-slp-vectorize -ftree-switch-conversion  -ftree-tail-merge -ftree-vrp -fvect-cost-model=very-cheap  -fgcse-after-reload -fipa-cp-clone -floop-interchange -floop-unroll-and-jam -fpeel-loops -fpredictive-commoning -fsplit-loops -fsplit-paths -ftree-loop-distribution -ftree-partial-pre -funswitch-loops -fvect-cost-model=dynamic -fversion-loops-for-strides -g")
endif()

include_directories(include)

find_package(libconfig++ REQUIRED)
link_directories(${LIBCONFIG++_LIBRARY_DIR})

include_directories(shared include ${LIBCONFIG++_INCLUDE_DIR})

add_executable(raytracer ${SRCS} ${HEADERS})
target_link_libraries(raytracer ${LIBCONFIG++_LIBRARY})
target_link_libraries(raytracer config++)
target_link_libraries(raytracer sfml-window sfml-graphics sfml-system)
