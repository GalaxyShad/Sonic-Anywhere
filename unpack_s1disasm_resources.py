# Script by Adol 2024
# https://github.com/Adolf-Eleador-II

import os
import sys
import re


def get_list_files(path_file, folders=None):
    if folders is None:
        folders = []

    list_file = []
    for x in os.listdir(path_file):
        if os.path.isfile(os.path.join(path_file, x)) and folders == []:
            list_file.append(os.path.join(path_file, x))
        elif os.path.isdir(os.path.join(path_file, x)) and ((x in folders) or folders == []):
            indir = get_list_files(os.path.join(path_file, x))
            if indir:
                list_file += indir

    return list_file


def ext_to_h(file_path):
    orig_path = file_path
    file_path, file_tail = os.path.split(orig_path)
    file_name, file_ext = os.path.splitext(file_tail)

    file_name_lower = re.sub(r'[ ]', '_', re.sub(r'( -| &|\(|\)|\-)', '', file_name.lower()))
    file_name_upper = file_name_lower.upper()

    if (os.path.isdir(file_path) == False):
        os.makedirs(file_path)

    read_file_name = os.path.join('../s1disasm', orig_path[2:])
    write_file_name = os.path.join(file_path, (file_name_lower + '_' + file_ext[1:] + '.h'))

    text_bin = []
    with open(read_file_name, "rb") as file:
        text_bin = file.read()

    text_hex = '\t' + ('\n\t'.join(
        ' '.join('0x{:02X},'.format(c) for c in text_bin[i:i + 16]) for i in range(0, len(text_bin), 16)))[:-1]

    name_define = 'S1DISASM__' + (
        os.path.basename(file_path).upper()) + '__' + file_name_upper + '_' + file_ext.upper()[1:]
    write_begin = '#ifndef ' + name_define + '_H' + '\n#define ' + name_define + '_H' + '\n\nconst unsigned char ' + name_define + '_BODY[] = {\n'
    write_end = '\n};\n\n#endif // ' + name_define + '_H' + '\n'

    with open(write_file_name, "w+") as file:
        file.write(write_begin)
        file.write(text_hex)
        file.write(write_end)


if __name__ == '__main__':
    FOLDERS = [
        'artkos', 'artnem', 'artunc', 'collide', 'demodata',
        'levels', 'map16', 'map256', 'misc', 'objpos',
        'palette', 'sslayout', 'startpos', 'tilemaps'
    ]

    dir = os.path.dirname(sys.argv[0])

    os.chdir(os.path.join(dir, 's1disasm'))

    file_list = get_list_files('.', FOLDERS)

    if (os.path.isdir('../src_s1disasm_assets') == False):
        os.makedirs('../src_s1disasm_assets')
    os.chdir(os.path.join(dir, 'src_s1disasm_assets'))

    for file in file_list:
        ext_to_h(file)

    with open('index.h', "w+") as file:
        for path in file_list:
            file_path, file_tail = os.path.split(path)
            file_name_lower = re.sub(r'[ .]', '_', re.sub(r'( -| &|\(|\)|\-)', '', file_tail.lower()))
            gg = '#include \"' + ((os.path.join(file_path, (file_name_lower + '.h')))[2:]).replace('\\', '/') + '\"'
            file.write(gg + '\n')

    check_sum = 0
    for file_path in file_list:
        for c in (file_path[2:]):
            check_sum += ord(c)

    name_define = 'SONIC_ANYWHERE_TESTS_RESOURCEMAP_H'
    write_begin = '#ifndef ' + name_define + '\n#define ' + name_define + '\n\ntypedef enum ResourceID {\n'
    write_end = '\n} ResourceID;\n#define RESOURCE_COUNT ' + str(len(file_list)) + '\n#define RESOURCE_CHECK_SUM ' + str(
        check_sum) + '\n\n#endif // ' + name_define + '\n'

    with open('resourcemap.h', "w+") as file:
        file.write(write_begin)
        for path in file_list:
            file_path, file_tail = os.path.split(path)
            file_name_upper = re.sub(r'[ .]', '_', re.sub(r'( -| &|\(|\)|\-)', '', file_tail.upper()))
            gg = '\tRESOURCE__' + ((os.path.basename(file_path).upper()) + '__' + file_name_upper).replace('\\', '/')
            file.write(gg + ',\n')
        file.write(write_end)
