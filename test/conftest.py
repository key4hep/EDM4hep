#!/usr/bin/env python3
"""pytest config module to make passing of input file name possible"""


def pytest_addoption(parser):
    """Hook to add an inputfile argument to pytest for checking EDM4hep file
    contents"""
    parser.addoption("--inputfile", action="store")
