cmake_minimum_required(VERSION 3.22.1)

project("antigravity")

add_library(
        antigravity
        SHARED
        GameLib.cpp
        KittyMemory/KittyMemory.cpp
        KittyMemory/KittyUtils.cpp
        KittyMemory/KittyScanner.cpp
        KittyMemory/KittyIOFile.cpp
        KittyMemory/MemoryPatch.cpp
        KittyMemory/MemoryBackup.cpp
        KittyMemory/KittyPtrValidator.cpp
        KittyMemory/KittyAsm.cpp
)

include_directories(KittyMemory)

find_library(
        log-lib
        log
)

target_link_libraries(
        antigravity
        ${log-lib}
)
