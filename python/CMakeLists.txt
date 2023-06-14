install(DIRECTORY edm4hep DESTINATION python
  REGEX .*\\.in EXCLUDE
  PATTERN __pycache__ EXCLUDE
)
