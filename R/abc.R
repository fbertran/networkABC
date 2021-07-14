#' @title ABC algorithm for network reverse-engineering
#' @description  ABC algorithm for network reverse-engineering
#' @name abc
#' @param data : Any microarray data in the form of a matrix (rows are genes and columns are time points)
#' @param clust_coeffs : one dimensional array of size clust_size of clustering coefficients (these clustering coefficient are tested in the ABc algorithm).
#' @param tolerance : a real value based for the tolerance between  the generated networks and the reference network
#' @param number_hubs : number of hubs in the network
#' @param iterations : number of times to repeat ABC algorithm
#' @param number_networks : number of generated networks in each iteration of the ABC algorithm
#' @param hub_probs : one-dimensional array of size number_genes for the each label to be in the role of a hub
#' @param neighbour_probs : this is the matrix of neighbour probabilities of size number_nodes*number_nodes
#' @param is_probs : this needs to be set either to one (if you specify hub_probs and neighbour_probs) or to zero (if neither probabilities are specified). Warning: you should specify both hub_probs and neighbour_probs if is_probs is one. If is_prob is zero these arrays should simply indicate an array of a specified size..
#' 
#' 
#' @examples
#' M<-matrix(rnorm(30),10,3)
#' result<-abc(data=M)
NULL
#> NULL

#' @rdname abc
#'
#' @export
#' @useDynLib networkABC C_abc

abc<-function(data,
              clust_coeffs=c(0.33,0.66,1),
              tolerance=NA,
              number_hubs=NA,
              iterations=10,
              number_networks=1000,
              hub_probs=NA,
              neighbour_probs=NA,
              is_probs=1){

ngenes<-nrow(data)
ntimes<-ncol(data)
clust_size<-length(clust_coeffs)

if(is.na(number_hubs)){number_hubs<-max(3,round(0.05*ngenes))}
if(prod(!is.na(hub_probs))!=1){hub_probs<-rep(1,ngenes)}
if(prod(!is.na(neighbour_probs))!=1){neighbour_probs<-matrix(1,ngenes,ngenes)}

iterations2<-iterations
number_networks2<-number_networks
if(is.na(tolerance)){
                cat("First run of abc to find tolerance\n")
                res222<-networkABC::abc(data,
                clust_coeffs=clust_coeffs,
                tolerance=sum(data^2),
                number_hubs=number_hubs,
                iterations=1,
                number_networks=(number_networks),
                hub_probs=hub_probs,
                neighbour_probs=neighbour_probs,
                is_probs=is_probs)
                tolerance<-quantile(res222$dist,0.05)
                cat("Tolerance value\n")
                print(tolerance)
                cat("===============================\n")
                cat("Beginning main run of abc\n")
}
iterations<-iterations2
number_networks<-number_networks2


data<-c(matrix(t(data),30,1,byrow=TRUE))

if(length(hub_probs)!=ngenes){stop("The length of hub_probs should be equal to the number of genes")}
if(length(neighbour_probs)!=(ngenes^2)){stop("neighbour_probs should be a squared matrix of size nb of genes * nb of genes")}

neighbour_probs<-c(neighbour_probs)

result=.C(C_abc, as.double(data), 
          as.integer(ngenes), 
          as.integer(ntimes), 
          as.integer(clust_size), 
          as.double(clust_coeffs), 
          as.double(tolerance), 
          as.integer(number_hubs), 
          as.integer(iterations), 
          as.integer(number_networks), 
          number_accepted=as.integer(0), 
          dist=as.double(1:number_networks), 
          hp=as.double(hub_probs), 
          np=as.double(neighbour_probs), 
          as.integer(is_probs))
#No longer needed since the symbol is registered in the NAMESPACE
#          ,PACKAGE="networkABC")

result$np<-matrix(result$np,ngenes,ngenes,byrow=TRUE)
return(result)

}

