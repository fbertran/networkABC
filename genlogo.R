#install.packages("hexSticker")
library(hexSticker)
#install.packages("ggplot2")
library(ggplot2)


library(png)
library(grid)
library(cowplot)
library(magick)
theme_set(theme_cowplot())



r = ggdraw() +
  draw_image(
          image_background(
        image_read_pdf("man/figures/ABC.pdf"), 
        'none'), 
    scale = .55)

sticker(
  r,
  package = "networkABC",
  p_size = 7,
  s_x = .98,
  s_y = 0.72,
  s_width = 1.7,
  s_height = 1.3,
  p_x = 1,
  p_y = 1.3,
  url = "https://cran.r-project.org/package=networkABC",
  u_color = "white",
  u_size = 1.05,
  h_fill = "black",
  h_color = "grey",
  filename = "man/figures/logo.png"
)


sticker(
  r,
  package = "networkABC",
  p_size = 7,
  s_x = .98,
  s_y = 0.72,
  s_width = 1.7,
  s_height = 1.3,
  p_x = 1,
  p_y = 1.3,
  url = "https://cran.r-project.org/package=networkABC",
  u_color = "white",
  u_size = 1.05,
  h_fill = "black",
  h_color = "grey",
  filename = "man/figures/logo_networkABC.svg"
)
