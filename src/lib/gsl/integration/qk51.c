/* integration/qk51.c
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000, 2007 Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <config.h>
#include <gsl_integration.h>

/* Gauss quadrature weights and kronrod quadrature abscissae and
   weights as evaluated with 80 decimal digit arithmetic by
   L. W. Fullerton, Bell Labs, Nov. 1981. */

static const double xgk[26] =   /* abscissae of the 51-point kronrod rule */
{
  0.999262104992609834193457486540341,
  0.995556969790498097908784946893902,
  0.988035794534077247637331014577406,
  0.976663921459517511498315386479594,
  0.961614986425842512418130033660167,
  0.942974571228974339414011169658471,
  0.920747115281701561746346084546331,
  0.894991997878275368851042006782805,
  0.865847065293275595448996969588340,
  0.833442628760834001421021108693570,
  0.797873797998500059410410904994307,
  0.759259263037357630577282865204361,
  0.717766406813084388186654079773298,
  0.673566368473468364485120633247622,
  0.626810099010317412788122681624518,
  0.577662930241222967723689841612654,
  0.526325284334719182599623778158010,
  0.473002731445714960522182115009192,
  0.417885382193037748851814394594572,
  0.361172305809387837735821730127641,
  0.303089538931107830167478909980339,
  0.243866883720988432045190362797452,
  0.183718939421048892015969888759528,
  0.122864692610710396387359818808037,
  0.061544483005685078886546392366797,
  0.000000000000000000000000000000000
};

/* xgk[1], xgk[3], ... abscissae of the 25-point gauss rule. 
   xgk[0], xgk[2], ... abscissae to optimally extend the 25-point gauss rule */

static const double wg[13] =    /* weights of the 25-point gauss rule */
{
  0.011393798501026287947902964113235,
  0.026354986615032137261901815295299,
  0.040939156701306312655623487711646,
  0.054904695975835191925936891540473,
  0.068038333812356917207187185656708,
  0.080140700335001018013234959669111,
  0.091028261982963649811497220702892,
  0.100535949067050644202206890392686,
  0.108519624474263653116093957050117,
  0.114858259145711648339325545869556,
  0.119455763535784772228178126512901,
  0.122242442990310041688959518945852,
  0.123176053726715451203902873079050
};

static const double wgk[26] =   /* weights of the 51-point kronrod rule */
{
  0.001987383892330315926507851882843,
  0.005561932135356713758040236901066,
  0.009473973386174151607207710523655,
  0.013236229195571674813656405846976,
  0.016847817709128298231516667536336,
  0.020435371145882835456568292235939,
  0.024009945606953216220092489164881,
  0.027475317587851737802948455517811,
  0.030792300167387488891109020215229,
  0.034002130274329337836748795229551,
  0.037116271483415543560330625367620,
  0.040083825504032382074839284467076,
  0.042872845020170049476895792439495,
  0.045502913049921788909870584752660,
  0.047982537138836713906392255756915,
  0.050277679080715671963325259433440,
  0.052362885806407475864366712137873,
  0.054251129888545490144543370459876,
  0.055950811220412317308240686382747,
  0.057437116361567832853582693939506,
  0.058689680022394207961974175856788,
  0.059720340324174059979099291932562,
  0.060539455376045862945360267517565,
  0.061128509717053048305859030416293,
  0.061471189871425316661544131965264,
  0.061580818067832935078759824240066
};

/* wgk[25] was calculated from the values of wgk[0..24] */

void
gsl_integration_qk51 (const gsl_function * f, double a, double b,
                      double *result, double *abserr,
                      double *resabs, double *resasc)
{
  double fv1[26], fv2[26];
  gsl_integration_qk (26, xgk, wg, wgk, fv1, fv2, f, a, b, result, abserr, resabs, resasc);
}

