find_package(PythonInterp REQUIRED)
find_package(Python COMPONENTS Development OPTIONAL)

add_custom_target(pyext ALL
  COMMAND ${PYTHON_EXECUTABLE} setup.py build_ext --inplace
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
