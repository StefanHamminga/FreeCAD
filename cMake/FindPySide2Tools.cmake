# Try to find PySide2 utilities, PYSIDE2UIC and PYSIDE2RCC:
# PYSIDE2UICBINARY - Location of PYSIDE2UIC executable
# PYSIDE2RCCBINARY - Location of PYSIDE2RCC executable
# PYSIDE2_TOOLS_FOUND - PySide2 utilities found.

# Also provides macro similar to FindQt4.cmake's WRAP_UI and WRAP_RC,
# for the automatic generation of Python code from Qt4's user interface
# ('.ui') and resource ('.qrc') files. These macros are called:
# - PYSIDE_WRAP_UI
# - PYSIDE_WRAP_RC

IF(PYSIDE2UICBINARY AND PYSIDE2RCCBINARY)
  # Already in cache, be silent
  set(PYSIDE2_TOOLS_FOUND_QUIETLY TRUE)
ENDIF(PYSIDE2UICBINARY AND PYSIDE2RCCBINARY)

if(WIN32 OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #pyside2 tools are often in same location as python interpreter
    get_filename_component(PYTHON_BIN_DIR ${PYTHON_EXECUTABLE} PATH)
    set(PYSIDE_BIN_DIR ${PYTHON_BIN_DIR})
endif(WIN32 OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

FIND_PROGRAM(PYSIDE2UICBINARY NAMES python2-pyside2-uic pyside2-uic pyside2-uic-${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} HINTS ${PYSIDE_BIN_DIR})
FIND_PROGRAM(PYSIDE2RCCBINARY NAMES pyside2-rcc pyside2-rcc-${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} HINTS ${PYSIDE_BIN_DIR})

MACRO(PYSIDE_WRAP_UI outfiles)
  FOREACH(it ${ARGN})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.py)
    #ADD_CUSTOM_TARGET(${it} ALL
    #  DEPENDS ${outfile}
    #)
    if(WIN32)
        ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
          COMMAND ${PYSIDE2UICBINARY} ${infile} -o ${outfile}
          MAIN_DEPENDENCY ${infile}
        )
    else(WIN32)
        # Especially on Open Build Service we don't want changing date like
        # pyside2-uic generates in comments at beginning.
        EXECUTE_PROCESS(
          COMMAND ${PYSIDE2UICBINARY} ${infile}
          COMMAND sed "/^# /d"
          OUTPUT_FILE ${outfile}
        )
    endif(WIN32)
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)
ENDMACRO (PYSIDE_WRAP_UI)

MACRO(PYSIDE_WRAP_RC outfiles)
  FOREACH(it ${ARGN})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}_rc.py)
    #ADD_CUSTOM_TARGET(${it} ALL
    #  DEPENDS ${outfile}
    #)
    if(WIN32)
        ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
          COMMAND ${PYSIDE2RCCBINARY} ${infile} -o ${outfile}
          MAIN_DEPENDENCY ${infile}
        )
    else(WIN32)
        # Especially on Open Build Service we don't want changing date like
        # pyside2-rcc generates in comments at beginning.
        EXECUTE_PROCESS(
          COMMAND ${PYSIDE2RCCBINARY} ${infile}
          COMMAND sed "/^# /d"
          OUTPUT_FILE ${outfile}
       )
    endif(WIN32)
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)
ENDMACRO (PYSIDE_WRAP_RC)

IF(EXISTS ${PYSIDE2UICBINARY} AND EXISTS ${PYSIDE2RCCBINARY})
   set(PYSIDE2_TOOLS_FOUND TRUE)
ENDIF(EXISTS ${PYSIDE2UICBINARY} AND EXISTS ${PYSIDE2RCCBINARY})

if(PYSIDE2RCCBINARY AND PYSIDE2UICBINARY)
    if (NOT PySide2Tools_FIND_QUIETLY)
        message(STATUS "Found PySide2 tools: ${PYSIDE2UICBINARY}, ${PYSIDE2RCCBINARY}")
    endif (NOT PySide2Tools_FIND_QUIETLY)
else(PYSIDE2RCCBINARY AND PYSIDE2UICBINARY)
    if(PySide2Tools_FIND_REQUIRED)
        message(FATAL_ERROR "PySide2 tools could not be found, but are required.")
    else(PySide2Tools_FIND_REQUIRED)
        if (NOT PySide2Tools_FIND_QUIETLY)
                message(STATUS "PySide2 tools: not found.")
        endif (NOT PySide2Tools_FIND_QUIETLY)
    endif(PySide2Tools_FIND_REQUIRED)
endif(PYSIDE2RCCBINARY AND PYSIDE2UICBINARY)
