file(GLOB lib_modules "*")
# message(STATUS "lib_modules: ${lib_modules}")

include_directories(
    ${PROJECT_SOURCE_DIR}/lib/include
    ${PROJECT_SOURCE_DIR}/lib/template
)

foreach (module ${lib_modules})
  if(IS_DIRECTORY ${module})
    get_filename_component(module_name ${module} NAME)
    file(GLOB module_sources "${module}/*.cpp")
    list(LENGTH module_sources num_sources)
    if(num_sources GREATER 0)
      # only .cpp
      message(STATUS "Lib find cpp file(s): ${module_sources}")
      add_library(lib_${module_name} STATIC ${module_sources})
      target_include_directories(lib_${module_name} PUBLIC ${module})
    else()
      # only .h or only .tcc
      add_library(lib_${module_name} INTERFACE)
      target_include_directories(lib_${module_name} INTERFACE ${module})
    endif()
    list(APPEND lib_targets lib_${module_name})
  endif()
endforeach()

message(STATUS "Lib: ${lib_targets}")
set(lib_targets ${lib_targets} PARENT_SCOPE)