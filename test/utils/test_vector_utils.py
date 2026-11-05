#!/usr/bin/env python3
"""Tests to ensure that the vector utilities defined in vector_utils.h are
usable from python"""

import math

import pytest

import edm4hep

utils = edm4hep.utils
Vector2f = edm4hep.Vector2f
Vector3f = edm4hep.Vector3f
Vector3d = edm4hep.Vector3d
Vector4f = edm4hep.Vector4f

# Vector types and their components for the "standard" (1, 2, 3, 4) vector
VECTOR_TYPES = {
    Vector2f: (1.0, 2.0),
    Vector3f: (1.0, 2.0, 3.0),
    Vector3d: (1.0, 2.0, 3.0),
    Vector4f: (1.0, 2.0, 3.0, 4.0),
}


def create(vec_type):
    """Create the standard test vector for the given type"""
    return vec_type(*VECTOR_TYPES[vec_type])


def type_name(vec_type):
    """Use the type name as test id for parametrized tests"""
    return vec_type.__name__


def by_type(*cases):
    """Turn (vec_type, *args) tuples into pytest params with the type name as id"""
    return [pytest.param(*case, id=type_name(case[0])) for case in cases]


all_types = pytest.mark.parametrize("vec_type", list(VECTOR_TYPES), ids=type_name)
types_3d = pytest.mark.parametrize("vec_type", [Vector3f, Vector3d], ids=type_name)


# --- Uniform getters and operators ---


@pytest.mark.parametrize(
    "vec_type,expected",
    by_type(
        # 2D vectors fill the z and t component with 0
        (Vector2f, (1.0, 2.0, 0.0, 0.0)),
        # 3D vectors fill the t component with 0
        (Vector3f, (1.0, 2.0, 3.0, 0.0)),
        (Vector3d, (1.0, 2.0, 3.0, 0.0)),
        (Vector4f, (1.0, 2.0, 3.0, 4.0)),
    ),
)
def test_uniform_getters(vec_type, expected):
    """Check that the uniform getters work for all vector types"""
    vector = create(vec_type)
    getters = (utils.vector_x, utils.vector_y, utils.vector_z, utils.vector_t)
    assert tuple(get(vector) for get in getters) == expected


@all_types
def test_operators(vec_type):
    """Check that the arithmetic operators work for all vector types"""
    vector1 = create(vec_type)
    vector2 = create(vec_type)

    sum_v = vector1 + vector2
    assert isinstance(sum_v, vec_type)
    assert sum_v == vector1 * 2
    assert sum_v / 2 == vector2

    # check that subtraction works
    assert sum_v - vector1 == vector2

    # make sure the inputs have not been modified
    assert vector1 == create(vec_type)
    assert vector2 == create(vec_type)


@pytest.mark.parametrize(
    "vec_type,expected",
    by_type((Vector2f, 5), (Vector3f, 14), (Vector3d, 14), (Vector4f, -2)),
)
def test_dot_product(vec_type, expected):
    """Check that the scalar product works (including the Minkowski metric
    for 4D vectors)"""
    assert create(vec_type) * create(vec_type) == expected


@pytest.mark.xfail(
    raises=NotImplementedError,
    strict=True,
    reason="cppyy does not pick up the operator*(double, V) free function template as __rmul__",
)
def test_scalar_left_multiplication():
    """Multiplication with the scalar on the left hand side, i.e. the
    operator*(double, V) overload"""
    vector = create(Vector3f)
    assert 2 * vector == vector + vector


def test_mixed_types_not_supported():
    """Operators are only defined between vectors of the same type"""
    with pytest.raises(NotImplementedError):
        _ = create(Vector3f) + create(Vector3d)


# --- Utility functions ---


@pytest.mark.parametrize("vec_type", [Vector2f, Vector3f, Vector3d], ids=type_name)
def test_normalize_and_projection(vec_type):
    """Normalizing a vector and projecting it onto the original vector"""
    vector = create(vec_type)
    norm_v = utils.normalizeVector(vector)
    assert isinstance(norm_v, vec_type)
    assert utils.magnitude(norm_v) == pytest.approx(1, abs=1e-6)
    assert utils.projection(norm_v, vector) == pytest.approx(1, abs=1e-6)

    # Normalize to a different norm
    norm3_v = utils.normalizeVector(vector, 3.0)
    assert utils.magnitude(norm3_v) == pytest.approx(3, abs=1e-5)

    # Negative norms yield a null vector
    assert utils.magnitude(utils.normalizeVector(vector, -1.0)) == 0

    # Null vectors are returned unchanged
    assert utils.normalizeVector(vec_type()) == vec_type()


@pytest.mark.parametrize(
    "vec_type,expected,tol",
    by_type(
        (Vector2f, math.sqrt(5), 1e-6),
        (Vector3f, math.sqrt(14), 1e-6),
        (Vector3d, math.sqrt(14), 1e-7),
    ),
)
def test_magnitude(vec_type, expected, tol):
    """Check the magnitude for the spatial vector types"""
    assert utils.magnitude(create(vec_type)) == pytest.approx(expected, abs=tol)


@pytest.mark.parametrize(
    "components,expected,tol",
    [
        ((4, 3, 2, 1), math.sqrt(29 - 1), 1e-5),
        ((1, 2, 3, 10), math.sqrt(100 - 14), 1e-5),
        ((2, 3, 6, 7), 0, 1e-7),
    ],
    ids=["spacelike", "timelike", "lightlike"],
)
def test_magnitude_minkowski(components, expected, tol):
    """4D vectors use the Minkowski metric for the magnitude"""
    assert utils.magnitude(Vector4f(*components)) == pytest.approx(expected, abs=tol)


@types_3d
def test_3d_components(vec_type):
    """Transverse and longitudinal components of 3D vectors"""
    vector = create(vec_type)
    assert utils.magnitudeTransverse(vector) == pytest.approx(math.sqrt(5), abs=1e-6)
    assert utils.magnitudeLongitudinal(vector) == 3

    vec_t = utils.vectorTransverse(vector)
    assert isinstance(vec_t, vec_type)
    assert vec_t == vec_type(1, 2, 0)

    vec_l = utils.vectorLongitudinal(vector)
    assert isinstance(vec_l, vec_type)
    assert vec_l == vec_type(0, 0, 3)


def test_angle_between_2d():
    """Angle between two 2D vectors"""
    # acos(11 / (5 * sqrt(5)))
    angle = utils.angleBetween(Vector2f(3, 4), create(Vector2f))
    assert angle == pytest.approx(0.17985349, abs=1e-6)


@types_3d
def test_angle_between_3d(vec_type):
    """Angle between two 3D vectors"""
    vector = create(vec_type)
    # acos(26 / (sqrt(14) * sqrt(50)))
    angle = utils.angleBetween(vec_type(3, 4, 5), vector)
    assert angle == pytest.approx(0.1862387, abs=1e-6)
    angle = utils.angleBetween(vec_type(1, 0, 0), vec_type(0, 1, 0))
    assert angle == pytest.approx(math.pi / 2)
    assert utils.angleBetween(vector, vector * -1) == pytest.approx(math.pi, abs=1e-6)

    # Null vectors yield 0 as angle
    assert utils.angleBetween(vec_type(), vector) == 0
    assert utils.angleBetween(vector, vec_type()) == 0


def test_projection_onto_null_vector():
    """Projecting onto a null vector returns the magnitude of the vector"""
    vector = create(Vector3d)
    assert utils.projection(vector, Vector3d()) == pytest.approx(math.sqrt(14))


def test_angles():
    """Polar and azimuthal angles"""
    assert utils.anglePolar(Vector3d(1, 0, 1)) == pytest.approx(math.pi / 4)
    assert utils.anglePolar(Vector3d(0, 0, -1)) == pytest.approx(math.pi)
    assert utils.anglePolar(Vector3d(0, 1, 0)) == pytest.approx(math.pi / 2)

    assert utils.angleAzimuthal(Vector2f(1, 1)) == pytest.approx(math.pi / 4, abs=1e-6)
    assert utils.angleAzimuthal(Vector3d(0, -1, 5)) == pytest.approx(-math.pi / 2)
    assert utils.angleAzimuthal(Vector4f(-1, 0, 2, 3)) == pytest.approx(math.pi, abs=1e-6)


def test_eta():
    """Pseudorapidity and its relation to the polar angle"""
    assert utils.etaToAngle(0) == pytest.approx(math.pi / 2)
    assert utils.angleToEta(math.pi / 2) == pytest.approx(0, abs=1e-7)

    assert utils.eta(Vector3d(1, 0, 0)) == pytest.approx(0, abs=1e-7)
    vector = create(Vector3d)
    assert utils.eta(vector) == pytest.approx(utils.angleToEta(utils.anglePolar(vector)))


def test_spherical_to_vector():
    """Creating a vector from spherical coordinates"""
    expected = (0.4546487134, 0.7080734183, 0.5403023059)

    # By default a Vector3f is created
    spher_vec = utils.sphericalToVector(1, 1, 1)
    assert isinstance(spher_vec, Vector3f)
    assert (spher_vec.x, spher_vec.y, spher_vec.z) == pytest.approx(expected, abs=1e-6)

    # The return type can be chosen explicitly
    spher_vec = utils.sphericalToVector[Vector3d](1, 1, 1)
    assert isinstance(spher_vec, Vector3d)
    assert (spher_vec.x, spher_vec.y, spher_vec.z) == pytest.approx(expected, abs=1e-9)

    # Round trip via the angle functions
    spher_vec = utils.sphericalToVector[Vector3d](2.5, 0.7, -1.2)
    assert utils.magnitude(spher_vec) == pytest.approx(2.5)
    assert utils.anglePolar(spher_vec) == pytest.approx(0.7)
    assert utils.angleAzimuthal(spher_vec) == pytest.approx(-1.2)


def test_constraints_respected():
    """Functions are only available for vector types satisfying the
    corresponding concepts"""
    with pytest.raises(TypeError):
        utils.vectorTransverse(create(Vector2f))
    with pytest.raises(TypeError):
        utils.magnitudeTransverse(create(Vector4f))
    with pytest.raises(TypeError):
        utils.normalizeVector(create(Vector4f))
