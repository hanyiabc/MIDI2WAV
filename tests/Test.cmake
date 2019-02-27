
# Compare the output to the expected
execute_process( 
  COMMAND ${COMPARE_EXE_FILE} ${OUTPUT_FILE}.wav ${TEST_FILE}
  RESULT_VARIABLE DIFFERENT
  )

if(DIFFERENT)
  message(FATAL_ERROR "Test failed - instrument output incorrect")
endif()
