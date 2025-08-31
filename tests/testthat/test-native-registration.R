test_that("The native routines are properly registered", {
  dlls <- getDLLRegisteredRoutines("networkABC")
  # At least one .C routine is registered
  expect_true(length(dlls$.C) >= 1)
})
