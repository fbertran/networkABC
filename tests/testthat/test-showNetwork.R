test_that("showNetwork() does not fail with a sensible cutoff", {
  skip_on_cran()
  skip_on_ci()
  M <- toy_matrix(20, 5, seed = 11)
  set.seed(1)
  res <- networkABC::abc(data = M)
  # The test only checks for the absence of errors (no snapshot)
  expect_silent(networkABC::showNetwork(res, min_prob = 0.3))
})
