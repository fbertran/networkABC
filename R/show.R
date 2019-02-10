#' @description Plot for the hub probabilities ; there is one probability for each node in the network.
#' @title Plot for the hub probabilities
#' @param result : The result of the abc algorithm.
#' 
#' @examples
#' data(resabc)
#' showHp(resabc)
#' @export
showHp<-function(result){
  
  hubs<-rev(sort(result$hp))[1:result[[4]]]
  hubs<-which(result$hp%in%hubs)
  color<-rep("red",result[[2]])
  color[hubs]<-"green"
  barplot(result$hp,col=color)
  
  hubs<-data.frame(gene.hubs = hubs, hubs.proba = result$hp[hubs])
  return(hubs)
}


#' @description Plot for the neighbourhood probabilities ; there is one probability for each pair of node in the network.
#' @title Plot for the neighbourhood probabilities
#' @param result : The result of the abc algorithm.
#' 
#' @examples
#' data(resabc)
#' showNp(resabc)
#' @export
showNp<-function(result){
  
  
  myImagePlot(result$np)
  
  
  
}


#' @description Plot the final network.
#' @title Plot the final network.
#' @param res : The result of the abc algorithm.
#' @param min_prob : numeric ; under this probabilitie value, the link between two genes is set to 0. 
#' 
#' @examples
#' data(resabc)
#' showNetwork(resabc,.2)
#' @export
showNetwork<-function(res,min_prob){
  
  requireNamespace("network")
  requireNamespace("sna")
  requireNamespace("RColorBrewer")
  Net<-res$np
  Net[Net<min_prob]<-0
  Net<-network::as.network(Net,directed=TRUE)
  dede<-sna::degree(Net,cmode="outdegree")
  dede<-dede+1
  dede<-log(dede)+1
  dede[dede>4]<-4
  colo<-colorRampPalette(RColorBrewer::brewer.pal(9,"Blues"))(200)
  plot(Net,displaylabels=TRUE,vertex.cex=dede,vertex.col=colo[floor(res$hp/max(res$hp)*199)+1])  
}






myImagePlot <- function(x, ...){
  min <- min(x)
  max <- max(x)
  yLabels <- rownames(x)
  xLabels <- colnames(x)
  title <-c()
  # check for additional function arguments
  if( length(list(...)) ){
    Lst <- list(...)
    if( !is.null(Lst$zlim) ){
      min <- Lst$zlim[1]
      max <- Lst$zlim[2]
    }
    if( !is.null(Lst$yLabels) ){
      yLabels <- c(Lst$yLabels)
    }
    if( !is.null(Lst$xLabels) ){
      xLabels <- c(Lst$xLabels)
    }
    if( !is.null(Lst$title) ){
      title <- Lst$title
    }
  }
  # check for null values
  if( is.null(xLabels) ){
    xLabels <- c(1:ncol(x))
  }
  if( is.null(yLabels) ){
    yLabels <- c(1:nrow(x))
  }
  
  layout(matrix(data=c(1,2), nrow=1, ncol=2), widths=c(4,1), heights=c(1,1))
  
  # Red and green range from 0 to 1 while Blue ranges from 1 to 0
  ColorRamp <- rgb( seq(0,1,length=256),  # Red
                    seq(0,1,length=256),  # Green
                    seq(1,0,length=256))  # Blue
  ColorLevels <- seq(min, max, length=length(ColorRamp))
  
  # Reverse Y axis
  reverse <- nrow(x) : 1
  yLabels <- yLabels[reverse]
  x <- x[reverse,]
  
  # Data Map
  par(mar = c(3,5,2.5,2))
  image(1:length(xLabels), 1:length(yLabels), t(x), col=ColorRamp, xlab="",
        ylab="", axes=FALSE, zlim=c(min,max))
  if( !is.null(title) ){
    title(main=title)
  }
  axis(BELOW<-1, at=1:length(xLabels), labels=xLabels, cex.axis=0.7)
  axis(LEFT <-2, at=1:length(yLabels), labels=yLabels, las= HORIZONTAL<-1,
       cex.axis=0.7)
  
  # Color Scale
  par(mar = c(3,2.5,2.5,2))
  image(1, ColorLevels,
        matrix(data=ColorLevels, ncol=length(ColorLevels),nrow=1),
        col=ColorRamp,
        xlab="",ylab="",
        xaxt="n")
  
  layout(1)
}
# ----- END plot function ----- #
