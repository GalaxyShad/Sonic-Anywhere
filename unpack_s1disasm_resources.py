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

def creation_of_file_h_from_any_etension(file_path):
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

def generate_index_h(file_list):
    with open('index.h', "w+") as file:
        for path in file_list:
            file_path, file_tail = os.path.split(path)
            file_name_lower = re.sub(r'[ .]', '_', re.sub(r'( -| &|\(|\)|\-)', '', file_tail.lower()))
            write_middle = (
                '#include \"' + ((os.path.join(file_path, (file_name_lower + '.h')))[2:]).replace('\\', '/') + '\"'
                )
            file.write(write_middle + '\n')

def generate_resourcemap_h(file_list, check_sum=0):    
    write_begin = (
        '#ifndef SONIC_ANYWHERE_TESTS_RESOURCEMAP_H\n' +
        '#define SONIC_ANYWHERE_TESTS_RESOURCEMAP_H\n\n' +
        'typedef enum ResourceID {\n'
        )
    write_end = (
        '} ResourceID;\n' +
        '#define RESOURCE_COUNT ' + str(len(file_list)) + '\n'+
        '#define RESOURCE_CHECK_SUM ' + str(check_sum) + '\n\n' +
        '#endif // SONIC_ANYWHERE_TESTS_RESOURCEMAP_H\n'
        )

    with open('resourcemap.h', "w+") as file:
        file.write(write_begin)
        for path in file_list:
            file_path, file_tail = os.path.split(path)
            file_name_upper = re.sub(r'[ .]', '_', re.sub(r'( -| &|\(|\)|\-)', '', file_tail.upper()))
            write_middle = (
                '\tRESOURCE__' + ((os.path.basename(file_path).upper()) + '__' + file_name_upper).replace('\\', '/')
                )
            file.write(write_middle + ',\n')
        file.write(write_end)

def generate_resourceid_h(file_list,check_sum):
    write_begin = (
        '#ifndef SONIC_ANYWHERE_RESOURCEID_H\n' +
        '#define SONIC_ANYWHERE_RESOURCEID_H\n\n' +
        'typedef enum ResourceID {\n'
        )
    write_end = (
        '} ResourceID;\n' +
        '#define RESOURCE_COUNT ' + str(len(file_list)) + '\n'+
        '#define RESOURCE_CHECK_SUM ' + str(check_sum) + '\n\n' +
        '#endif // SONIC_ANYWHERE_RESOURCEID_H\n'
        )

    with open('resourceid.h', "w+") as file:
        file.write(write_begin)
        for path in file_list:
            file_path, file_tail = os.path.split(path)
            file_name_upper = re.sub(r'[ .]', '_', re.sub(r'( -| &|\(|\)|\-)', '', file_tail.upper()))
            write_middle = (
                '\tRESOURCE__' + ((os.path.basename(file_path).upper()) + '__' + file_name_upper).replace('\\', '/')
                )
            file.write(write_middle + ',\n')
        file.write(write_end)

def generate_resourcestore_c(file_list):
    write_begin = (
        '#include \"resourcestore.h\"\n' +
        '#include \"include_backend/debug.h\"\n' +
        '#include \"src_s1disasm_assets/index.h\"\n\n' +
        'ReadonlyByteArray resource_store__get(ResourceID res_id) {\n' +
        '#define $(X)                      \t\t\\\n' +
        '\t(ReadonlyByteArray) {           \t\\\n' +
        '\t\tX, (sizeof(X) / sizeof(X[0])) \t\\\n' +
        '\t}\n\n' +
        '\t// clang-format off\n' +
        '\tswitch (res_id) {\n'
        )
    write_end = (
        '\tdefault:\n' +                                                                         
        '\t\tLOG_ERROR("[Resource Store] No res_id DEC: %d HEX: 0x%04X implemented", res_id, res_id)\n' +
        '\t\treturn $(S1DISASM__PALETTE__SEGA1_BIN_BODY);\n' +
        '\t}\n' +
        '\t// clang-format on\n' +
        '#undef $ //(X)\n' +
        '}'
        )

    with open('resourcestore.c', "w+") as file:
        file.write(write_begin)
        for path in file_list:
            file_path, file_tail = os.path.split(path)
            file_name_upper = re.sub(r'[ .]', '_', re.sub(r'( -| &|\(|\)|\-)', '', file_tail.upper()))
            write_middle = (
                '\tcase RESOURCE__' + ((os.path.basename(file_path).upper()) + '__' + file_name_upper).replace('\\', '/') +
                ': return $(S1DISASM__' + ((os.path.basename(file_path).upper()) + '__' + file_name_upper).replace('\\', '/') + '_BODY);'
                )
            file.write(write_middle + '\n')
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
    check_sum = 0
    for file_path in file_list:
        for c in (file_path[2:]):
            check_sum += ord(c)

    # change directory to ./src_s1disasm_assets
    if (os.path.isdir('../src_s1disasm_assets') == False):
        os.makedirs('../src_s1disasm_assets')
    os.chdir(os.path.join(dir, 'src_s1disasm_assets'))

    # generate _.h files from _.* files
    for file in file_list:
        creation_of_file_h_from_any_etension(file)

    # generate assets in ./src_s1disasm_assets
    generate_index_h(file_list)
    generate_resourcemap_h(file_list,check_sum)

    # change directory to ./src/resources
    dir = os.path.dirname(sys.argv[0])
    os.chdir(os.path.join(dir, 'src/resources'))    

    # generate resource in ./src/resources
    generate_resourceid_h(file_list,check_sum)
    generate_resourcestore_c(file_list)




