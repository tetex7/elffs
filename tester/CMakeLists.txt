#
# Copyright (c) 2026. Tetex7
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

# Setup paths to your files
set(EFS_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assset")
set(EFS_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/efs_data.o")
set(ELFFSCTL_BIN "${CMAKE_CURRENT_BINARY_DIR}/../elffsctl/elffsctl")

# Command to execute elffsctl
add_custom_command(
        OUTPUT ${EFS_OUTPUT_FILE}
        COMMAND ${ELFFSCTL_BIN} -p ${EFS_INPUT_DIR} --make-object-file -o ${EFS_OUTPUT_FILE}.efs -b efs_blob
        COMMAND ${ELFFSCTL_BIN} --make-object-file -o ${EFS_OUTPUT_FILE} -i ${EFS_OUTPUT_FILE}.efs -b efs_blob
        DEPENDS ${EFS_INPUT_DIR} # Re-builds if source files/dir change
        COMMENT "Packing EFS assets into object file..."
        VERBATIM
)

# Attach the generated file to your main target
add_executable(tester main.c ${EFS_OUTPUT_FILE})
target_link_libraries(tester PUBLIC libelffs_rt)