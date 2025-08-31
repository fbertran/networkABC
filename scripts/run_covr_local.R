# Local HTML coverage report
if (!requireNamespace("covr", quietly = TRUE)) {
  stop("Please install 'covr' first: install.packages('covr')")
}
message("Computing test coverage...")
covrep <- covr::package_coverage(type = "tests")
dir.create("coverage", showWarnings = FALSE)
covr::report(covrep, file = "coverage/index.html", browse = interactive())
message("Coverage report written to coverage/index.html")
