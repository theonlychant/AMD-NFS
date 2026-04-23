find_program(GOEXE go)
if(GOEXE)
  add_custom_target(go_server ALL
    COMMAND ${GOEXE} build -o amdinfer_server ./
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()
