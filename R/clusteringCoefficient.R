#' @title Calculate the clustering coefficient
#' @description Calculate the clustering coefficient for an adjacency matrix. By default, the local clustering coefficient is calculated. From the PCIT package after it was archived on the CRAN.
#' @param adj An adjacency matrix. Calculating the clustering coefficient only makes sense if some connections are zero i.e. no connection.
#' @param FUN The function for calculating the clustering coefficient.
#' @param ... Arguments to pass to FUN
#' @return The clustering coefficient(s) for the adjacency matrix.
#' @author Nathan S. Watson-Haigh
#' @seealso \link{localClusteringCoefficient}
#' @examples
#' clusteringCoefficient(network_gen(50,.33)$network)
#' @export

clusteringCoefficient <- function(adj, FUN='localClusteringCoefficient', ...) {
	FUN <- match.fun(FUN)
	
	coef <- FUN(adj, ...)
	
	return(coef)
}
