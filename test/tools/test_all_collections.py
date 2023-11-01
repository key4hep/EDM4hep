'''
Tests if all collection header files are included in the cxx file.
'''

import sys
import argparse
import yaml


def test(yamlfile_path, cxxfile_path):
    '''
    Test itself.
    Takes two parameters, Podio YAML file location and cxx file to be checked.
    '''

    with open(yamlfile_path, mode='r', encoding="utf-8") as yamlfile:
        datamodel = yaml.safe_load(yamlfile)

    with open(cxxfile_path, mode='r', encoding="utf-8") as cxxfile:
        cxxfile_lines = cxxfile.readlines()

    datatypes = datamodel['datatypes']

    for collname in datatypes:
        include_string = '#include "' + collname.replace('::', '/') + \
                         'Collection.h"'
        include_found = False
        for line in cxxfile_lines:
            if include_string in line:
                include_found = True

        if not include_found:
            print('ERROR: Following collection not included in the cxx file.')
            print('       ' + collname)
            sys.exit(2)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Test all collections')
    parser.add_argument('yamlfile')
    parser.add_argument('cxxfile')
    args = parser.parse_args()

    test(args.yamlfile, args.cxxfile)
