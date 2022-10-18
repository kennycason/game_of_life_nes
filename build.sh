#!/bin/sh

NAME=boxxle
NES_FILE=${NAME}.nes
WORKSPACE=$(pwd)
SRC_DIR=${WORKSPACE}/src
BUILD_DIR=${WORKSPACE}/build
GRAPHICS_DIR=${WORKSPACE}/assets/gfx
SOUND_DIR=${WORKSPACE}/assets/sfx
#CONFIG_DIR=${WORKSPACE}/config
LIB_DIR=${WORKSPACE}/lib
NES_LIB_DIR=${LIB_DIR}/nes
#NES_LIB_DIR=${LIB_DIR}/NESLibrary

TEMP_DIR=${BUILD_DIR}/temp
rm -r ${TEMP_DIR}
mkdir ${TEMP_DIR}

echo "Copy code + data to ${TEMP_DIR}"
cp ${NES_LIB_DIR}/* ${TEMP_DIR}
#cp ${CONFIG_DIR}/* ${TEMP_DIR}
cp ${GRAPHICS_DIR}/* ${TEMP_DIR}
cp ${SOUND_DIR}/* ${TEMP_DIR}
cp ${SRC_DIR}/* ${TEMP_DIR}

echo "Building "${NES_FILE}
cd ${TEMP_DIR}
cc65 -t nes ${NAME}.c -o ${NES_FILE}
cp ${NES_FILE} ${BUILD_DIR}


NES_FILE_FULL_PATH=${BUILD_DIR}/${NES_FILE}
if [[ -f "$NES_FILE_FULL_PATH" ]]; then
  fceux ${NES_FILE_FULL_PATH}
fi

