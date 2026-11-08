"""Test package initialization without requiring PyROOT or built libraries."""

import importlib.util
from pathlib import Path
import sys
from types import ModuleType, SimpleNamespace
from unittest.mock import Mock, call

import pytest


@pytest.fixture
def import_package(monkeypatch):
    """Import the real initializer with isolated, mocked dependencies."""

    def load(has_rdf=True, load_results=None, header_results=None):
        package_name = "_edm4hep_import_test"
        source = Path(__file__).resolve().parents[1] / "python/edm4hep/__init__.py"
        spec = importlib.util.spec_from_file_location(package_name, source)
        package = importlib.util.module_from_spec(spec)
        version = ModuleType(f"{package_name}.__version__")
        version.__version__ = "1.2.3"
        config = ModuleType(f"{package_name}._build_config")
        config.HAS_RDF = has_rdf
        root = ModuleType("ROOT")
        root.edm4hep = SimpleNamespace(utils=SimpleNamespace())
        root.gSystem = SimpleNamespace(
            Load=Mock(side_effect=lambda lib: (load_results or {}).get(lib, 0))
        )
        root.gInterpreter = SimpleNamespace(
            LoadFile=Mock(side_effect=lambda header: (header_results or {}).get(header, 0))
        )
        pythonizations = ModuleType("podio.pythonizations")
        pythonizations.load_pythonizations = Mock()
        for name, module in (
            (package_name, package),
            (version.__name__, version),
            (config.__name__, config),
            ("ROOT", root),
            ("podio", ModuleType("podio")),
            ("podio.pythonizations", pythonizations),
        ):
            monkeypatch.setitem(sys.modules, name, module)
        # The initializer replaces this entry; restore any real bindings afterwards.
        monkeypatch.setitem(sys.modules, "edm4hep", None)
        spec.loader.exec_module(package)
        assert sys.modules["edm4hep"] is root.edm4hep
        assert root.edm4hep.__version__ == version.__version__
        pythonizations.load_pythonizations.assert_called_once_with("edm4hep")
        return root

    return load


@pytest.mark.parametrize("has_rdf", [False, True])
@pytest.mark.parametrize("load_result", [0, 1])
def test_import_optional_rdf(import_package, has_rdf, load_result):
    root = import_package(
        has_rdf=has_rdf,
        load_results={"libedm4hepDict": load_result, "libedm4hepRDF": load_result},
    )
    libraries = [call("libedm4hepDict")]
    if has_rdf:
        libraries.append(call("libedm4hepRDF"))
    assert root.gSystem.Load.call_args_list == libraries
    headers = [
        call("edm4hep/utils/kinematics.h"),
        call("edm4hep/Constants.h"),
        call("edm4hep/utils/vector_utils.h"),
    ]
    if has_rdf:
        headers.insert(0, call("edm4hep/utils/dataframe.h"))
    assert root.gInterpreter.LoadFile.call_args_list == headers


@pytest.mark.parametrize("library", ["libedm4hepDict", "libedm4hepRDF"])
@pytest.mark.parametrize("load_result", [-1, -2])
def test_library_load_failure(import_package, library, load_result):
    with pytest.raises(RuntimeError, match=f"Failed to load {library}"):
        import_package(load_results={library: load_result})


@pytest.mark.parametrize(
    "header", ["dataframe.h", "kinematics.h", "Constants.h", "vector_utils.h"]
)
def test_header_load_failure(import_package, header):
    path = f"edm4hep/{header}" if header == "Constants.h" else f"edm4hep/utils/{header}"
    with pytest.raises(RuntimeError, match=f"Failed to load {header}"):
        import_package(header_results={path: -1})
