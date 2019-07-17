file(GLOB INCS *.h sinks/*.h fmt/*.h fmt/bundled*.h details/*.h)
install(FILES ${INCS} DESTINATION include/spdlog/)
