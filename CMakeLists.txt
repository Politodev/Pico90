
cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico2_w)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(PICO_90DNS_ROUTER C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(PICO-90DNS-ROUTER
    main.c
    dhcpserver/dhcpserver.c
)

target_include_directories(PICO-90DNS-ROUTER PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/dhcpserver
)

target_link_libraries(PICO-90DNS-ROUTER
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
)

pico_enable_stdio_usb(PICO-90DNS-ROUTER 1)
pico_enable_stdio_uart(PICO-90DNS-ROUTER 0)

pico_configure_ip4_address(PICO-90DNS-ROUTER PRIVATE
    CYW43_DEFAULT_IP_AP_ADDRESS 192.168.4.1
)

pico_add_extra_outputs(PICO-90DNS-ROUTER)
