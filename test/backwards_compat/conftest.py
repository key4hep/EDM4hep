#!/usr/bin/env python3
"""pytest config module to make passing of input file name possible"""


def pytest_addoption(parser):
    parser.addoption("--inputfile", action="store", default="heeeeyyyoooo")
