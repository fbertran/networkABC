#' networkABC
#'
#' An inference tool based on approximate Bayesian computations to decipher 
#' network data and assess the strength of their inferred links.
#'
#' @docType package
#' @name networkABC
#' @references \emph{networkABC: An inference tool for networks based on approximate Bayesian computation}, Myriam Maumy-Bertrand, Frédéric Bertrand, preprint.
#\url{https://arxiv.org/abs/}
# @import lars glmnet igraph parallel msgps Rfast doMC foreach
#' @importFrom grDevices colorRampPalette rgb
#' @importFrom graphics axis barplot image layout par plot
#' @importFrom stats quantile
#' @importFrom network as.network
#' @importFrom sna degree
#' @importFrom RColorBrewer brewer.pal
#' @useDynLib networkABC
NULL
