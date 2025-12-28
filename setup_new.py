from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11
import sys

# Define the C++ source files
cpp_sources = [
    'pathfinder_wrapper.cpp',
    'cpp_src/src/Graph.cpp',
    'cpp_src/src/ShortestPath.cpp',
    'cpp_src/src/LongestPath.cpp',
    'cpp_src/src/FewestStops.cpp',
    'cpp_src/src/ReachableCities.cpp',
    'cpp_src/src/MultiCityTour.cpp',
    'cpp_src/src/CheapestNetwork.cpp',
    'cpp_src/src/PathFinder.cpp',
]

# Define the extension module
ext_modules = [
    Pybind11Extension(
        'pathfinder',
        sources=cpp_sources,
        include_dirs=[
            pybind11.get_include(),
            'cpp_src/include',
        ],
        language='c++',
        extra_compile_args=['-std=c++17', '-O3'],
    ),
]

setup(
    name='pathfinder',
    version='2.0.0',
    author='Path Finder Team',
    description='Modular Path Finding Engine',
    ext_modules=ext_modules,
    cmdclass={'build_ext': build_ext},
    zip_safe=False,
    python_requires='>=3.7',
)
