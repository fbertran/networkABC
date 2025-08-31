# Generate non-regression fixtures for networkABC
message("Generating non-regression fixtures for networkABC ...")
suppressPackageStartupMessages(library(networkABC))

dir.create(file.path("tests","fixtures"), recursive = TRUE, showWarnings = FALSE)

meta <- list(
  pkg_version = as.character(utils::packageVersion("networkABC")),
  R_version = paste(R.version$major, R.version$minor, sep="."),
  timestamp = as.character(Sys.time())
)

# fixed seed for stability
# Small but representative matrix
n_genes <- 30; n_times <- 6
M <- {
  set.seed(20250828)
  M <- matrix(rnorm(n_genes * n_times), nrow = n_genes, ncol = n_times)
}
set.seed(20250828)
res <- networkABC::abc(data = M)

hp  <- showHp(res)
np  <- as.matrix(showNp(res))

# Dist summary is more stable cross-platform than full vector equality
distq <- NULL
if (!is.null(res$dist)) {
  q <- stats::quantile(res$dist, probs = c(0, 0.25, 0.5, 0.75, 1), na.rm = TRUE, names = FALSE)
  distq <- list(quantiles = q, probs = c(0, 0.25, 0.5, 0.75, 1))
}

saveRDS(list(meta = meta, hp = hp), file = file.path("tests","fixtures","hp_baseline.rds"))
saveRDS(list(meta = meta, np = np), file = file.path("tests","fixtures","np_baseline.rds"))
if (!is.null(distq)) saveRDS(list(meta = meta, distq = distq), file = file.path("tests","fixtures","distq_baseline.rds"))

cat("Fixtures written to tests/fixtures/*.rds\n")

