#!/usr/bin/env python3
"""pytest config module to make passing of input file name possible"""

"""The pytest options after configuration"""
options = None


def pytest_addoption(parser):
    """Hook to add an inputfile argument to pytest for checking EDM4hep file
    contents"""
    parser.addoption("--inputfile", action="store")


def pytest_configure(config):
    """This is a slighty hacky solution to make the pytest configuration
    available in test modules outside of fixtures"""
    global options
    options = config.option
