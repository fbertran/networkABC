# Helpers to read fixtures for non-regression tests

fixture_path <- function(name) {
  # pick from tests/fixtures
  f <- file.path(testthat::test_path("fixtures"), name)
  if (!file.exists(f)) {
    testthat::skip(paste("Fixture not found:", name, "— run nonreg/generate-fixtures.R to create it."))
  }
  f
}

read_fixture <- function(name) {
  readRDS(fixture_path(name))
}
