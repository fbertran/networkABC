//
// Created by Frédéric Bertrand on 2019-02-20.
//
//#include <Rinternals.h> // for SEXP
//extern SEXP abc(SEXP data_array, SEXP pnumber_genes, SEXP pnumber_times, SEXP pclust_size, SEXP clust_coeffs, SEXP ptolerance, SEXP pnumber_hubs, SEXP pnumber_outer_iter, SEXP pnumber_inner_iter, SEXP number_accepted, SEXP dist_array, SEXP hub_probs, SEXP rneighbour_probs, SEXP is_probs);
//extern SEXP spline_value(SEXP knots, SEXP coeff, SEXP order, SEXP x, SEXP deriv);

#ifndef SRC_ABC_R_H
#define SRC_ABC_R_H

void abc(double *data_array, int *pnumber_genes, int *pnumber_times, int *pclust_size, double *clust_coeffs, double *ptolerance, int *pnumber_hubs, int *pnumber_outer_iter, int *pnumber_inner_iter, int *number_accepted, double *dist_array, double *hub_probs, double *rneighbour_probs, int *is_probs);

#endif //SRC_ABC_R_H
