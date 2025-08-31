test_that("network_gen() returns a well-formed binary matrix", {
  skip_on_cran()
  skip_on_ci()
  skip_if_not(exists("network_gen", where = asNamespace("networkABC"), inherits = FALSE))
  
  gen <- get("network_gen", envir = asNamespace("networkABC"))
  set.seed(123)
  adj <- gen(60, 0.30)
  
  expect_true(is.list(adj))
  expect_true(is.matrix(adj$network))
  expect_equal(nrow(adj), ncol(adj))
  expect_true(all(adj$network %in% c(-1,0,1)))
  expect_equal(diag(adj$network) |> as.integer() |> sum(), 0L)  # no self-loops
  
  # If it’s undirected, we can test symmetry and clustering
  if (isTRUE(all(adj$network == t(adj$network)))) {
    cc <- global_clustering(adj)
    expect_true(is.finite(cc))
    expect_gte(cc, 0)
    expect_lte(cc, 1)
    expect_lt(abs(cc - 0.30), 0.10)  # +-0.10 tolerance for small n
  }
})
