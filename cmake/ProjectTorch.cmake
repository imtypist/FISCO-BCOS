include(ExternalProject)


ExternalProject_Add(libtorch
    PREFIX ${CMAKE_SOURCE_DIR}/deps
    DOWNLOAD_NAME libtorch-shared-with-deps-1.10.1+cpu.zip
    DOWNLOAD_NO_PROGRESS 1
    URL https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-1.10.1%2Bcpu.zip
    URL_HASH SHA256=a424e3d5621032287f8c0274a8a9ad920aef5ba2fc9ddd39f0340f8565ae122a
    BUILD_IN_SOURCE 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/deps/src/libtorch")
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")