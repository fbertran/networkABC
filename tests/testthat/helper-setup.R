# Edition 3 de testthat
local_edition(3)

skip_if_not_installed("networkABC")

# Toy matrix generator
toy_matrix <- function(n_genes = 20, n_times = 5, seed = 1) {
  set.seed(seed)
  matrix(rnorm(n_genes * n_times), nrow = n_genes, ncol = n_times)
}

# Clustering (global) for non directed binary graph
global_clustering <- function(adj) {
  stopifnot(is.matrix(adj), nrow(adj) == ncol(adj))
  # strictly binary, no self-loop
  A <- (adj > 0) * 1L
  diag(A) <- 0L
  # triangles = trace(A^3)/6 (non directed)
  A2 <- A %*% A
  A3 <- A2 %*% A
  triangles <- sum(diag(A3)) / 6
  # connected triples: sum(choose(deg, 2))
  deg <- rowSums(A)
  triples <- sum(ifelse(deg >= 2, deg * (deg - 1) / 2, 0))
  if (triples == 0) return(0)
  3 * triangles / triples
}
