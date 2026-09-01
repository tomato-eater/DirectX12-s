target_include_directories("${target}" ${public} [==[$<BUILD_INTERFACE:C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/src/v4.6.7-5b0676bd97.clean/include>]==] ${private} [==[$<BUILD_INTERFACE:C:/MyGameP/DX12/Project12_s/vcpkg_installed/x64-windows/vcpkg/blds/simdjson/src/v4.6.7-5b0676bd97.clean/src>]==])
target_compile_features("${target}" ${public} [==[cxx_std_11]==])
target_link_libraries("${target}" ${public} [==[Threads::Threads]==])
target_compile_definitions("${target}" ${public} [==[SIMDJSON_THREADS_ENABLED=1]==])
