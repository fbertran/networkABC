test_that("abc() is (reasonably) reproducible with a fixed seed", {
  skip_on_cran()
  skip_on_ci()
  M <- toy_matrix(20, 5, seed = 7)
  set.seed(1); res1 <- networkABC::abc(data = M)
  set.seed(1); res2 <- networkABC::abc(data = M)
  # We compare derived outputs rather than the full object
  hp1 <- networkABC::showHp(res1)
  hp2 <- networkABC::showHp(res2)
  expect_equal(hp1, hp2, tolerance = 1e-12)
  
  np1 <- as.matrix(networkABC::showNp(res1))
  np2 <- as.matrix(networkABC::showNp(res2))
  expect_equal(np1, np2, tolerance = 1e-12)
})
