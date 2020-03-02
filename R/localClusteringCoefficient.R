#' @title Calculate the local clustering coefficient
#' @description Calculate the local clustering coefficient for each node in an adjacency matrix. The clustering coefficient is defined as the proportion of existing connections from the total possible (Watts and Strogatz, 1998).
#' @param adj An adjacency matrix. Calculating the clustering coefficient only makes sense if some connections are zero i.e. no connection.
#' @return A vector of local clustering coefficients for each node/gene of the adjacency matrix.
#' @author Nathan S. Watson-Haigh
#' @references D.J. Watts and S.H. Strogatz. (1998) Collective dynamics of 'small-world' networks. Nature. 393(6684). 440-442.
#' @seealso \link{clusteringCoefficient}
#' @examples
#' 
#' localClusteringCoefficient(network_gen(50,.33)$network)
#' @export

localClusteringCoefficient <- function(adj) {
#  adj=as.single(adj)
  nGenes=as.integer(nrow(adj))
  cc=as.single(rep(0, length=nrow(adj)))
  E=as.single(rep(0,nrow(adj)))
  k=as.integer(rep(0,nrow(adj)))
	
  for(i in 1:nGenes){
	neighbours = (adj[i,1:nGenes] != 0 | adj[1:nGenes,i] != 0)
	k[i] = sum(neighbours)
	if(k[i]<2){cc=NaN} else {
	  E[i] <- sum(adj[neighbours,neighbours])
	}
  }
	cc = E / ( k * (k-1) )
	return(as.vector(cc))
}
