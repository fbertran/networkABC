test_that("Non-regression: showHp() and showNp() stable with a fixed seed", {
  skip_on_cran()
  n_genes <- 30; n_times <- 6
  M <- {
    set.seed(20250828)
    M <- matrix(rnorm(n_genes * n_times), nrow = n_genes, ncol = n_times)
  }
  set.seed(20250828)
  res <- networkABC::abc(data = M)

  hp <- networkABC::showHp(res)
  np <- as.matrix(networkABC::showNp(res))

  # Fixtures
  fx_hp <- read_fixture("hp_baseline.rds")
  fx_np <- read_fixture("np_baseline.rds")

  # Structural checks
  expect_equal(length(hp), length(fx_hp$hp))
  expect_equal(dim(np), dim(fx_np$np))

  # Numerical comparisons  (tolerance for different FP min)
  expect_equal(hp, fx_hp$hp, tolerance = 1e-8, ignore_attr = TRUE)
  expect_equal(np, fx_np$np, tolerance = 1e-8, ignore_attr = TRUE)
})

test_that("Non-regression: stable distance distribution (quantiles)", {
  skip_on_cran()
  M <- {
    set.seed(20250828)
    matrix(rnorm(30 * 6), nrow = 30, ncol = 6)
  }
  set.seed(20250828)
  res <- networkABC::abc(data = M)
  if (is.null(res$dist)) testthat::skip("res$dist is NULL — skipping distance quantiles NR test.")
  q <- stats::quantile(res$dist, probs = c(0, 0.25, 0.5, 0.75, 1), na.rm = TRUE, names = FALSE)

  fx <- read_fixture("distq_baseline.rds")
  expect_equal(as.numeric(q), as.numeric(fx$distq$quantiles), tolerance = 1e-8, ignore_attr = TRUE)
})
