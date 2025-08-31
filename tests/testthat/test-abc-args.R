test_that("abc() valid its arguments and fails nicely", {
  # non numeric data
  expect_error(networkABC::abc(data = matrix(letters[1:10], 5, 2)))
  # bad input
  expect_error(networkABC::abc(data = 1:10))
  # invalid tolerance
  M <- toy_matrix(10, 3, seed = 9)
  expect_error(networkABC::abc(data = M, tolerance = -0.1))
})

test_that("abc() handles NAs clearly (error or explicit message).", {
  M <- toy_matrix(10, 3, seed = 3)
  M[1,1] <- NA_real_
  expect_error(networkABC::abc(data = M), regexp = "NA|NaN|Inf", ignore.case = TRUE)
})

test_that("abc() validates its arguments and fails cleanly (tolerance).", {
  M <- toy_matrix(10, 3, seed = 9)
  expect_error(networkABC::abc(data = M, tolerance = -0.1),
               regexp = "tolerance.*>0", ignore.case = TRUE)
  expect_error(networkABC::abc(data = M, tolerance = c(0.1, 0.2)),
               regexp = "single", ignore.case = TRUE)
  expect_no_error(networkABC::abc(data = M, tolerance = 0.1))
  expect_no_warning(networkABC::abc(data = M, tolerance = 0.1))
})