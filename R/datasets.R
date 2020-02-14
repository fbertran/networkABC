#' Simulated network
#'
#' Result of the use of the \code{network_gen} function.
#' @docType data
#' @format A list of three objects :
#' \describe{
#'   \item{number_genes}{The number of genes in the network}
#'   \item{clust_coef}{The clustering coefficient}
#'   \item{network}{The simulated network}
#'   }
#' @name netsimul
#' @rdname netsimul
"netsimul"
#> [1] "netsimul"

#' Result of an ABC inference
#'
#' Result for the reverse engineering of a simulated Cascade network
#' @docType data
#' @format A list of 14 objects :
#' \describe{
#' \item{data}{: The microarray data used, rows are genes and columns are time points.)}
#' \item{ngenes}{: The number of genes.)}
#' \item{ntimes}{: The number of timepoints)}
#' \item{clust_size}{: the size of clusters}
#' \item{clust_coeffs}{: the clustering coefficient}
#' \item{tolerance}{: the tolerance between the generated networks and the reference network}
#' \item{number_hubs}{: number of hubs in the network}
#' \item{iterations}{: number of times to repeat ABC algorithm}
#' \item{number_networks}{: number of generated networks in each iteration of the ABC algorithm}
#' \item{hub_probs}{: one-dimensional array of size number_genes for the each label to be in the role of a hub}
#' \item{neighbour_probs}{: matrix of neighbour probabilities of size number_nodes*number_nodes}
#' \item{is_probs}{: is equal to 1 since hub_probs and neighbour_probs were specified}
#' }
#' @name resabc
#' @rdname resabc
"resabc"
#> [1] "resabc"
