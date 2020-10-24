set(SOURCES
  channel.cpp
  mpmc.cpp
  mpsc.cpp
  spsc.cpp
)

add_library(sync ${SOURCES})