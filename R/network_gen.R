#' @title Random scale-free network generation. This function is used intensively in the abc function.
#' @description  Generate random network topology
#' @param number_genes A number
#' @param clust_coef A number
#' @return A list with the number of of genes, the targeted clustering coefficient and the resulting network
#' @examples
#' network_gen(10,1)
#' @export
#' @useDynLib networkABC run



network_gen<-function(number_genes,clust_coef){
  result=.C("run",number_genes= as.integer(number_genes), clust_coef=as.numeric(clust_coef), network=as.integer(1:(number_genes*number_genes)),PACKAGE="networkABC")
  
  result$network<-matrix(result$network,result$number_genes,result$number_genes,byrow=TRUE)
return(result)
}
