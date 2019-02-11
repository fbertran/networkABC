## ----setup, include = FALSE----------------------------------------------
knitr::opts_chunk$set(
  collapse = TRUE,
  comment = "#>"
)

## ---- cache=TRUE---------------------------------------------------------
library(networkABC)
net<-network_gen(100,0.33)

## ------------------------------------------------------------------------
require(network)
plot(network(net$network))

## ------------------------------------------------------------------------
require(PCIT)
f<-function(a){
  a<-a[!is.nan(a)]
}

