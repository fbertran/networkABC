## ----setup, include = FALSE----------------------------------------------
knitr::opts_chunk$set(
  collapse = TRUE,
  comment = "#>"
)

## ------------------------------------------------------------------------
library(networkABC)

## ---- messages=FALSE, fig.width=8, fig.height=8--------------------------
require(network)
plot(network(net$network))

## ------------------------------------------------------------------------
require(PCIT)
f<-function(a){
  a<-a[!is.nan(a)]
}

## ------------------------------------------------------------------------
mean(clco)

## ------------------------------------------------------------------------
sd(clco)

## ---- message=FALSE, fig.width=8, fig.height=8---------------------------
ggplot2::qplot(clco)

## ---- fig.width=8, fig.height=8------------------------------------------
networkABC::showHp(result)

## ---- fig.width=8, fig.height=8------------------------------------------
showNp(result)

## ---- fig.width=8, fig.height=8------------------------------------------
showNetwork(result,0.3)

## ---- fig.width=8, fig.height=8------------------------------------------
hist(result$dist)

## ---- eval=FALSE---------------------------------------------------------
#  result<-abcR(data=M,
#              clust_coeffs=0.33, #you can specify more than one clustering coefficient
#              tolerance=3.5, #maximal distance between simulated and real data
#              # to accept the network
#              number_hubs=3,#the number of hubs
#              iterations=10, #number of iterations
#              number_networks=1000000,#number of network simulated at each iteration
#              hub_probs=NA,#specify the a priori probabilty for each gene to be a hub
#              neighbour_probs=NA,#specify the a priori probability for each couple
#              #of gene to be linked
#              is_probs=1)#set this last option to one.

