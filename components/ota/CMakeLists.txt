idf_build_get_property(project_dir PROJECT_DIR)
set(COMPONENT_SRCS "ota.c")
set(COMPONENT_ADD_INCLUDEDIRS "./include")
set(COMPONENT_REQUIRES esp_http_client esp_https_ota nvs_flash app_update esp-tls)
register_component()