REM gcc -Wall -O3 -fno-strict-aliasing -D_JNI_IMPLEMENTATION_ -Wl,--kill-at -I ../ -I C:/Java/jdk1.6.0_16/include -I C:/Java/jdk1.6.0_16/include/win32 -shared org_jnativehook_GlobalScreen.c -o GlobalHook_Keyboard.dll

gcc -march=i586 -m32 -Wall -O3 -fno-strict-aliasing -xc -c -shared -D_JNI_IMPLEMENTATION_ -I Z:/src/org/jnativehook/c_source -I C:/Java/jdk1.6.0_16/include -I C:/Java/jdk1.6.0_16/include/win32 org_jnativehook_GlobalScreen.c 

