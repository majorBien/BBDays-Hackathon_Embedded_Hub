set(component_srcs "main.c" "lora.c" "uart.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS ".")
