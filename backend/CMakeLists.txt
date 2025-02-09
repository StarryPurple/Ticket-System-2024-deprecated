include_directories(
    "${PROJECT_SOURCE_DIR}/lib/include"
    "${PROJECT_SOURCE_DIR}/lib/template"
)

file(GLOB backend_modules "*")
# message(STATUS "backend_modules: ${backend_modules}")

include_directories(
    ${PROJECT_SOURCE_DIR}/backend/include
    ${PROJECT_SOURCE_DIR}/backend/template
)

foreach (module ${backend_modules})
  if(IS_DIRECTORY ${module})
    get_filename_component(module_name ${module} NAME)
    file(GLOB module_sources "${module}/*.cpp")
    list(LENGTH module_sources num_sources)
    if(num_sources GREATER 0)
      # only .cpp
      # message(STATUS "Backend find cpp file(s): ${module_sources}")
      add_library(backend_${module_name} STATIC ${module_sources})
      target_include_directories(backend_${module_name} PUBLIC ${module})
    else()
      #only .h or only .tcc
      add_library(backend_${module_name} INTERFACE)
      target_include_directories(backend_${module_name} INTERFACE ${module})
    endif()
    list(APPEND backend_targets backend_${module_name})
  endif()
endforeach()

# message(STATUS "Backend: ${backend_targets}")
set(backend_targets ${backend_targets} PARENT_SCOPE)