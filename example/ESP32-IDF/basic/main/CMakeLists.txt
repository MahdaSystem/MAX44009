idf_component_register(
  SRCS "main.c" "./main.c" "../../../../src/MAX44009.c" "../../../../port/ESP32-IDF/MAX44009_platform.c"
  INCLUDE_DIRS "../../../../src/include" "../../../../port/ESP32-IDF"
)