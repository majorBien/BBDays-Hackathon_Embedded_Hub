set(component_srcs "main.c" "lora.c" "wifi.c" "http_server.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS ".")
