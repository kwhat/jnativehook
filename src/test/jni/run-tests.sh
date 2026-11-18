#!/usr/bin/env bash
# Run Linux JNI regression fixtures without capturing or injecting system input.
set -euo pipefail
repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.." && pwd)
: "${JAVA_HOME:?Set JAVA_HOME to the JDK used to compile and run these fixtures}"
libuiohook_include=${LIBUIOHOOK_INCLUDE:-"$repo_root/src/external/libuiohook/include"}
if [[ ! -f "$libuiohook_include/uiohook.h" ]]; then
    echo 'Initialize src/external/libuiohook or set LIBUIOHOOK_INCLUDE.' >&2
    exit 1
fi
build_dir=$(mktemp -d)
trap 'rm -rf -- "$build_dir"' EXIT
mkdir -p "$build_dir/classes" "$build_dir/headers" "$build_dir/tests"
mapfile -d '' main_sources < <(find "$repo_root/src/main/java" -name '*.java' ! -name module-info.java -print0)
"$JAVA_HOME/bin/javac" -source 8 -target 8 -Xlint:-options \
    -h "$build_dir/headers" -d "$build_dir/classes" "${main_sources[@]}"
includes=("-I$JAVA_HOME/include" "-I$JAVA_HOME/include/linux" "-I$libuiohook_include"
    "-I$build_dir/headers" "-I$repo_root/src/main/jni" "-I$repo_root/src/main/jni/include")
native_sources=()
for source in jni_Converter.c jni_Errors.c jni_EventDispatcher.c jni_Globals.c \
    jni_Load.c jni_Logger.c com_github_kwhat_jnativehook_GlobalScreen.c; do
    native_sources+=("$repo_root/src/main/jni/$source")
done
java_options=(-Xcheck:jni -Djava.awt.headless=true)
if "$JAVA_HOME/bin/java" --enable-native-access=ALL-UNNAMED -version >/dev/null 2>&1; then
    java_options+=(--enable-native-access=ALL-UNNAMED)
fi
for test_source in "$repo_root"/src/test/jni/*Test.java; do
    if [[ ! -f "$test_source" ]]; then
        continue
    fi
    test_name=$(basename -- "$test_source" .java)
    stub="$repo_root/src/test/jni/${test_name}_stub.c"
    if [[ ! -f "$stub" ]]; then
        continue
    fi
    "$JAVA_HOME/bin/javac" -source 8 -target 8 -Xlint:-options \
        -cp "$build_dir/classes" -d "$build_dir/tests" "$test_source"
    flags=()
    if [[ "$test_name" == InitializationTest ]]; then
        flags+=(-Dcalloc=test_calloc)
    fi
    "${CC:-cc}" -std=c99 -shared -fPIC -pthread -Wl,-z,defs "${flags[@]}" \
        "${includes[@]}" "${native_sources[@]}" "$stub" -o "$build_dir/libJNativeHook.so"
    command=("$JAVA_HOME/bin/java" "${java_options[@]}" "-Djava.library.path=$build_dir"
        -cp "$build_dir/classes:$build_dir/tests" "$test_name")
    timeout 30s "${command[@]}"
    if [[ "$test_name" == InitializationTest ]]; then
        for allocation in {1..12}; do
            JNATIVEHOOK_FAIL_ALLOCATION=$allocation timeout 30s "${command[@]}"
        done
    fi
done
if [[ -f "$repo_root/src/test/jni/keycode_test.c" ]]; then
    "${CC:-cc}" -std=c99 "${includes[@]}" "$repo_root/src/main/jni/jni_Converter.c" \
        "$repo_root/src/test/jni/keycode_test.c" -o "$build_dir/keycode_test"
    "$build_dir/keycode_test"
    echo 'Native keycode regression tests passed'
fi
