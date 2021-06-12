#!/usr/bin/python3
# -*- coding: utf-8

# Apply AX58100 specific patches to ESI files tha were generated for standard ET1100 ESC.

import os, shutil
import struct
import xml.etree.ElementTree as ET


binary_to_patch = "./lib/soes-esi/cia402.bin"
binary_patched = "./lib/soes-esi/ax58100_patched/cia402patched.bin"
xml_to_patch = "./lib/soes-esi/cia402.xml"
xml_patched = "./lib/soes-esi/ax58100_patched/cia402patched.xml"

# ESI .bin address for configuration CRC
CRC_OFFSET_0x0E = 0x0E
# Host Interface Extend Setting and Drive Strength (0x0A)
HIES_Index = 0x0A    # Address in ESI binary configuration 
HIES_Value = b'\x1a' # 0x1A value: (INT edge pulse length, 8 mA Control + IO 9:0 Drive Select)
HIES_ValueStr = "1A"


def calcChecksum(data, offset = CRC_OFFSET_0x0E, crc = 0xFF):
    crc_table = [0, 7, 14, 9, 28, 27, 18, 21, 56, 63, 54, 49, 36, 35, 42, 45, 112, 119, 126, 121, 108, 107, 98, 101, 72, 79, 70, 65, 84, 83, 90, 93, 224, 231, 238, 233, 252, 251, 242, 245, 216, 223, 214, 209, 196, 195, 202, 205, 144, 151, 158, 153, 140, 139, 130, 133, 168, 175, 166, 161, 180, 179, 186, 189, 199, 192, 201, 206, 219, 220, 213, 210, 255, 248, 241, 246, 227, 228, 237, 234, 183, 176, 185, 190, 171, 172, 165, 162, 143, 136, 129, 134, 147, 148, 157, 154, 39, 32, 41, 46, 59, 60, 53, 50, 31, 24, 17, 22, 3, 4, 13, 10, 87, 80, 89, 94, 75, 76, 69, 66, 111, 104, 97, 102, 115, 116, 125, 122, 137, 142, 135, 128, 149, 146, 155, 156, 177, 182, 191, 184, 173, 170, 163, 164, 249, 254, 247, 240, 229, 226, 235, 236, 193, 198, 207, 200, 221, 218, 211, 212, 105, 110, 103, 96, 117, 114, 123, 124, 81, 86, 95, 88, 77, 74, 67, 68, 25, 30, 23, 16, 5, 2, 11, 12, 33, 38, 47, 40, 61, 58, 51, 52, 78, 73, 64, 71, 82, 85, 92, 91, 118, 113, 120, 127, 106, 109, 100, 99, 62, 57, 48, 55, 34, 37, 44, 43, 6, 1, 8, 15, 26, 29, 20, 19, 174, 169, 160, 167, 178, 181, 188, 187, 150, 145, 152, 159, 138, 141, 132, 131, 222, 217, 208, 215, 194, 197, 204, 203, 230, 225, 232, 239, 250, 253, 244, 243];

    for ix in range(0, offset):
        tbl_idx = (crc >> 0 ^ data[ix]) & 255
        crc = (crc_table[tbl_idx] ^ crc << 8) & 255

    return crc & 255


def calculate_esc_checksum(config_area_bytes):
    """
    Word 0x7 [Byte 0x0D] Checksum
    Low byte contains remainder of division of word 0 to word 6 as unsigned number 
    divided by the polynomial x8+x²+x+1 (initial value 0xFF).
    NOTE: For debugging purposes it is possible to disable the checksum validation
    with a checksum value of 0x88A4. Never use this for production! 
    """
    if len(config_area_bytes) != CRC_OFFSET_0x0E:
        raise ValueError("Wrong config bytes length: got {}, should {}".format(len(config_area_bytes), CRC_OFFSET_0x0E))

    checksum = calcChecksum(config_area_bytes)
    result = bytearray([checksum & 0xFF, 0])

    return result


def apply_binary_AX58100_patches(filename):
    checksum = -1

    with open(filename, 'r+b') as f_out:
        # Write 0x1A value (INT edge pulse length, 8 mA Control + IO 9:0 Drive Select)
        # to 0x0A (Host Interface Extend Setting and Drive Strength
        f_out.seek(HIES_Index)
        f_out.write(HIES_Value)
        # Calculate checksum for 0xE (0d14) first bytes
        f_out.seek(0x00)
        configurationArea = f_out.read(CRC_OFFSET_0x0E)
        checksum = calculate_esc_checksum(configurationArea)
        # Write checksum at 0x0E
        f_out.seek(CRC_OFFSET_0x0E)
        f_out.write(checksum)


def patch_binary(file_in, file_out):
    if file_in != file_out:
        if os.path.exists(file_out):
            print("Overwriting binary file '{}'".format(file_out))
            os.remove(file_out)
        shutil.copyfile(file_in, file_out)
        print("Binary '{}' is created".format(file_out))
    else:
        print("Editing '{}' in place".format(file_out))
    apply_binary_AX58100_patches(file_out)
    print("Done patching binary.")


def apply_xml_AX58100_patches(filename):
    Hies_charIndex = HIES_Index * 2  # byte is 2 characters
    tree = ET.parse(filename)
    root = tree.getroot()
    configDataNodes = root.findall(".//ConfigData")

    for configData in configDataNodes:
        updatedConfig = configData.text
        while len(updatedConfig) < 28:
            updatedConfig = updatedConfig + "0"
        updatedConfig = updatedConfig[:Hies_charIndex] + HIES_ValueStr + updatedConfig[Hies_charIndex + 2:]
        configData.text = updatedConfig

    tree.write(filename)


def patch_xml(file_in, file_out):
    '''SDK outputs           
    <ConfigData>050600440A000000</ConfigData>
    ASIX code samples have config like
    <ConfigData>050403440a00000000001a000000</ConfigData>
    so .xml can be used by Beckhoff EEPROM programmer 
    '''
    if file_in != file_out:
        if os.path.exists(file_out):
            print("Overwriting xml file '{}'".format(file_out))
            os.remove(file_out)
        shutil.copyfile(file_in, file_out)
        print("xml '{}' is created".format(file_out))
    else:
        print("Editing '{}' in place".format(file_out))
    apply_xml_AX58100_patches(file_out)
    print("Done patching xml.")


if __name__ == "__main__":
    patch_binary(binary_to_patch, binary_patched)
    patch_xml(xml_to_patch, xml_patched)
