# Generate Cobertura XML for CI upload
if (!requireNamespace("covr", quietly = TRUE)) {
  stop("Please install 'covr' first: install.packages('covr')")
}
cov <- covr::package_coverage(type = "tests")
dir.create("coverage", showWarnings = FALSE)
covr::to_cobertura(cov, file = "coverage/coverage.xml")
cat("Wrote coverage/coverage.xml\n")
