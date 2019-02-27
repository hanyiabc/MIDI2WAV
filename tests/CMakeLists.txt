
add_executable(compare compare.cpp)
INSTALL(TARGETS compare DESTINATION bin)

if(WIN32)
  set(COMPARE_EXE_NAME compare.exe)
else()
  set(COMPARE_EXE_NAME compare)
endif()


foreach(ID RANGE 4)

add_test(NAME test${ID}
  COMMAND ${CMAKE_COMMAND}
  -DCOMPARE_EXE_FILE=${CMAKE_BINARY_DIR}/bin/${COMPARE_EXE_NAME}
    -DOUTPUT_FILE=test0${ID}
    -DERR_FILE=test${ID}.stderr
    -DOUT_FILE=test${ID}.stdout
    -DTEST_FILE=${CMAKE_SOURCE_DIR}/tests/test0${ID}.wav
    -P ${CMAKE_CURRENT_SOURCE_DIR}/Test.cmake
    )

add_test(NAME test${ID}_1stHarmonic
  COMMAND ${CMAKE_COMMAND}
  -DCOMPARE_EXE_FILE=${CMAKE_BINARY_DIR}/bin/${COMPARE_EXE_NAME}
    -DOUTPUT_FILE=test0${ID}_harmonic1
    -DERR_FILE=test${ID}_1stHarmonic.stderr
    -DOUT_FILE=test${ID}_1stHarmonic.stdout
    -DTEST_FILE=${CMAKE_SOURCE_DIR}/tests/test0${ID}_harmonic1.wav
    -P ${CMAKE_CURRENT_SOURCE_DIR}/Test.cmake
    )

add_test(NAME test${ID}_2ndHarmonic
  COMMAND ${CMAKE_COMMAND}
  -DCOMPARE_EXE_FILE=${CMAKE_BINARY_DIR}/bin/${COMPARE_EXE_NAME}
    -DOUTPUT_FILE=test0${ID}_harmonic2
    -DERR_FILE=test${ID}_2ndHarmonic.stderr
    -DOUT_FILE=test${ID}_2ndHarmonic.stdout
    -DTEST_FILE=${CMAKE_SOURCE_DIR}/tests/test0${ID}_harmonic2.wav
    -P ${CMAKE_CURRENT_SOURCE_DIR}/Test.cmake
    )

add_test(NAME test${ID}_bothHarmonic
  COMMAND ${CMAKE_COMMAND}
  -DCOMPARE_EXE_FILE=${CMAKE_BINARY_DIR}/bin/${COMPARE_EXE_NAME}
    -DOUTPUT_FILE=test0${ID}_harmonicboth
    -DERR_FILE=test${ID}_bothHarmonic.stderr
    -DOUT_FILE=test${ID}_bothHarmonic.stdout
    -DTEST_FILE=${CMAKE_SOURCE_DIR}/tests/test0${ID}_harmonicboth.wav
    -P ${CMAKE_CURRENT_SOURCE_DIR}/Test.cmake
    )
endforeach(ID)
