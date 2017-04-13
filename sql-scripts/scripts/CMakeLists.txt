
configure_file(build-extension-update-files1.pl build-extension-update-files1.pl @ONLY)
configure_file(build-extension-file.pl build-extension-file.pl @ONLY)

add_custom_target(build_update_script
    DEPENDS build-extension-file.pl  build-extension-update-files1.pl
    )
