test_that("abc() runs on a small dataset and returns a well-formed object.", {
  skip_on_cran()
  skip_on_ci()
  M <- toy_matrix(20, 5, seed = 42)
  set.seed(123)
  
  expect_no_error(res <- networkABC::abc(data = M))
  expect_no_warning(res <- networkABC::abc(data = M))
  expect_type(res, "list")
  
  # Components and accessor functions that are agnostic to the object’s exact structure.
  if (!is.null(res$dist)) {
    expect_true(is.numeric(res$dist))
    expect_true(all(is.finite(res$dist)))
  }
  
  expect_silent(hp <- networkABC::showHp(res))
  expect_true(is.data.frame(hp))
  expect_true(all(apply(hp,1:2,is.numeric)))
  expect_gte(min(hp$gene.hubs), 0)
  expect_gte(min(hp$hubs.proba), 0)
  expect_lte(max(hp$hubs.proba), 1)
  expect_true(all(apply(hp,1:2,is.finite)))
  
  expect_silent(np <- networkABC::showNp(res))
  np <- as.matrix(np)
  expect_true(is.matrix(np))
  expect_equal(nrow(np), nrow(M))
  expect_equal(ncol(np), nrow(M))
  expect_gte(min(np), 0)
  expect_lte(max(np), 1)
  expect_true(all(is.finite(np)))
})
