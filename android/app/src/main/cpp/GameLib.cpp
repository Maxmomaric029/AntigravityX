#include <jni.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "Offsets.h"
#include "KittyMemory/KittyInclude.hpp"
#include "KittyMemory/KittyMemory.hpp"
#include "KittyMemory/MemoryPatch.hpp"

using namespace Offsets;

// Storage for patches
struct HackPatches {
    MemoryPatch recoil;
    MemoryPatch speed;
} g_patches;

bool g_Initialized = false;
uintptr_t g_il2cppBase = 0;

// Function to find the base address of libil2cpp.so
uintptr_t get_base_address(const char* name) {
    uintptr_t base = 0;
    while (base == 0) {
        base = KittyMemory::get_module_base(name);
        if (base == 0) usleep(100000);
    }
    return base;
}

void hack_thread() {
    g_il2cppBase = get_base_address("libil2cpp.so");
    
    // Initialize patches with placeholders or real hex
    // For No Recoil (V7A/PLAY: 0x0C inside WeaponData)
    // Here we'll patch the function that uses it if we have the address
    // But since the user gave me 0x18F4A20 for Speedhack in a previous file:
    g_patches.speed = MemoryPatch::createWithHex("libil2cpp.so", 0x18F4A20, "00 00 A0 41"); // Example: 5.0f hex
}

extern "C" JNIEXPORT void JNICALL
Java_com_antigravityx_MenuState_init(JNIEnv* env, jobject /* this */) {
    if (!g_Initialized) {
        std::thread(hack_thread).detach();
        g_Initialized = true;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_antigravityx_MenuState_applyNoRecoil(JNIEnv* env, jobject /* this */, jboolean enable) {
    // Offset for No Recoil usually target a float or a bool inside weapon logic
    // Using the PLAY offset 0x0C as a reference for the field
    // In many cases, it's easier to NOP the recoil function
    // For now, let's assume we have a function offset if provided later
}

extern "C" JNIEXPORT void JNICALL
Java_com_antigravityx_MenuState_applySpeedHack(JNIEnv* env, jobject /* this */, jboolean enable) {
    if (enable) {
        g_patches.speed.Modify();
    } else {
        g_patches.speed.Restore();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_antigravityx_MenuState_applyAntena(JNIEnv* env, jobject /* this */, jboolean enable) {
    // Antena logic: usually involves finding player bones and scaling them
    // or patching the rendering of the player model
}
