'''
Tests if all datatypes are used in the cxx file.
'''

import sys
import re
import argparse
import yaml


def test(yamlfile_path, cxxfile_path):
    '''
    Test itself.
    Takes two parameters, Podio YAML file location and cxx file to be checked.
    '''

    with open(yamlfile_path, mode='r', encoding="utf-8") as yamlfile:
        datamodel = yaml.safe_load(yamlfile)

    # List stores lines of cxx code on which `insertToJson<CollType>` is used
    datatypes_found = []

    with open(cxxfile_path, mode='r', encoding="utf-8") as cxxfile:
        for cxxline in cxxfile:
            cxxline = cxxfile.readline()
            result = re.search('insertIntoJson<edm4hep::(.+?)Collection>',
                               cxxline)
            if result:
                datatypes_found += ['edm4hep::' + result.group(1)]

    datatypes_found = set(datatypes_found)

    datatypes = set(datamodel['datatypes'])

    if not datatypes.issubset(datatypes_found):
        missing_datatypes = datatypes - datatypes_found
        print('ERROR: One or more datatypes are not being converted:')
        for datatype in missing_datatypes:
            print('       ' + datatype)
        sys.exit(2)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Test all collections')
    parser.add_argument('yamlfile')
    parser.add_argument('cxxfile')
    args = parser.parse_args()

    test(args.yamlfile, args.cxxfile)
